#include "AnimationTree.h"

AnimationTree::AnimationTree() {
	this->scene = NULL;
}

AnimationTree::AnimationTree(const aiScene *scene, aiMatrix4x4 globalInverse) {
	this->scene = scene;
	this->globalInverse = globalInverse;
}

AnimationTree::~AnimationTree() {
	if(scene != NULL)
		delete scene;
}

void AnimationTree::boneTransfrom(float animationTime, vector<BoneInfo> &boneInfos, unordered_map<string, unsigned int> boneMapping) {
	if (scene == NULL) {
		return;
	}

	aiMatrix4x4 identity;

	float TicksPerSecond = (float)(this->scene->mAnimations[0]->mTicksPerSecond != 0 ? this->scene->mAnimations[0]->mTicksPerSecond : 25.0f);
	float TimeInTicks = animationTime * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, (float)this->scene->mAnimations[0]->mDuration);

	readNodeHierarchy(AnimationTime, this->scene->mRootNode, identity, boneInfos, boneMapping);
}

void AnimationTree::readNodeHierarchy(float animationTime, const aiNode* node, aiMatrix4x4 parentMatrix, vector<BoneInfo> &boneInfos, unordered_map<string, unsigned int> boneMapping) {

	string NodeName(node->mName.data);

	const aiAnimation* pAnimation = this->scene->mAnimations[0];

	aiMatrix4x4 NodeTransformation(node->mTransformation);

	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

	if (pNodeAnim) {
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scaling;
		CalcInterpolatedScaling(Scaling, animationTime, pNodeAnim);
		aiMatrix4x4 ScalingM;
		ScalingM.Scaling(Scaling, ScalingM);

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, animationTime, pNodeAnim);
		aiMatrix4x4 RotationM = aiMatrix4x4(RotationQ.GetMatrix());

		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		CalcInterpolatedPosition(Translation, animationTime, pNodeAnim);
		aiMatrix4x4 TranslationM;
		TranslationM.Translation(Translation, TranslationM);

		// Combine the above transformations
		NodeTransformation = TranslationM * RotationM * ScalingM;
	}

	aiMatrix4x4 GlobalTransformation = parentMatrix * NodeTransformation;

	if (boneMapping.find(NodeName) != boneMapping.end()) {
		unsigned int BoneIndex = boneMapping[NodeName];
		boneInfos[BoneIndex].FinalTransformation = this->globalInverse * GlobalTransformation * boneInfos[BoneIndex].BoneOffset;
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		readNodeHierarchy(animationTime, node->mChildren[i], GlobalTransformation, boneInfos, boneMapping);
	}

}

unsigned int AnimationTree::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}


unsigned int AnimationTree::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}


unsigned int AnimationTree::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}


void AnimationTree::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	unsigned int NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}


void AnimationTree::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	unsigned int NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();
}


void AnimationTree::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1) {
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	unsigned int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	unsigned int NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

const aiNodeAnim* AnimationTree::FindNodeAnim(const aiAnimation* pAnimation, const string NodeName)
{
	for (unsigned int i = 0; i < pAnimation->mNumChannels; i++) {
		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

		if (string(pNodeAnim->mNodeName.data) == NodeName) {
			return pNodeAnim;
		}
	}

	return NULL;
}

const aiScene* AnimationTree::getScene() {
	return this->scene;
}

//void AnimationTree::setTransform(aiMatrix4x4 transform){
//	this->transform = transform;
//}
//
//void AnimationTree::setInverseMat(aiMatrix4x4 inverseMat){
//	this->inverseMat = inverseMat;
//}
//
//void AnimationTree::setName(string name) {
//	this->name = name;
//}
//
//void AnimationTree::addNode(AnimationTree node) {
//	this->children.push_back(node);
//}
//
//void AnimationTree::readNodeHierarchy(float animationTime, vector<BoneInfo> &boneInfos, unordered_map<string, unsigned int> boneMapping){
//	aiMatrix4x4 NodeTransformation(transform);
//	
//	//animation
//
//	aiMatrix4x4 globalTransform = NodeTransformation;
//
//	if (boneMapping.find(name) != boneMapping.end()) {
//		unsigned int BoneIndex = boneMapping[name];
//		 boneInfos[BoneIndex].FinalTransformation = inverseMat * globalTransform * boneInfos[BoneIndex].BoneOffset;
//		//boneInfos[BoneIndex].FinalTransformation = boneInfos[BoneIndex].BoneOffset * globalTransform * inverseMat;
//	}
//
//	for(int i =0; i<this->children.size() && i < 1; i++){
//		readNodeHierarchy(animationTime, this->children[i], globalTransform, boneInfos, boneMapping);
//	}
//}
//
//void AnimationTree::readNodeHierarchy(float animationTime, AnimationTree tree, aiMatrix4x4 parentMatrix, vector<BoneInfo> &boneInfos, unordered_map<string, unsigned int> boneMapping){
//	aiMatrix4x4 NodeTransformation(tree.transform);
//	
//	//animation
//
//	aiMatrix4x4 globalTransform = parentMatrix * NodeTransformation;
//	//aiMatrix4x4 globalTransform =  NodeTransformation * parentMatrix;
//
//	if (boneMapping.find(tree.name) != boneMapping.end()) {
//		unsigned int BoneIndex = boneMapping[tree.name];
//		boneInfos[BoneIndex].FinalTransformation = tree.inverseMat * globalTransform * boneInfos[BoneIndex].BoneOffset;
//		//boneInfos[BoneIndex].FinalTransformation = boneInfos[BoneIndex].BoneOffset * globalTransform * tree.inverseMat;
//
//	}
//
//	for(int i =0; i<tree.children.size(); i++){
//		readNodeHierarchy(animationTime, tree.children[i], globalTransform, boneInfos, boneMapping);
//	}
//}
