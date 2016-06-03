#include "AnimationTree.h"

AnimationTree::AnimationTree(const aiScene *scene, aiMatrix4x4 globalInverse) {
	this->scene = scene;
	this->globalInverse = globalInverse;
}

AnimationTree::~AnimationTree() {
	if(scene != NULL)
		delete scene;
}

void AnimationTree::boneTransfrom(float animationTime, vector<BoneInfo> &boneInfos, unordered_map<string, unsigned int> boneMapping) {
	if (scene == NULL || !this->scene->HasAnimations()) {
		return;
	}

	aiMatrix4x4 identity;

	float TicksPerSecond = (float)(this->scene->mAnimations[0]->mTicksPerSecond != 0 ? this->scene->mAnimations[0]->mTicksPerSecond : 25.0f);
	float TimeInTicks = animationTime * TicksPerSecond;
	float newAnimationTime = fmod(TimeInTicks, (float)this->scene->mAnimations[0]->mDuration);

	readNodeHierarchy(newAnimationTime, this->scene->mRootNode, identity, boneInfos, boneMapping);
}

aiAnimation* AnimationTree::getAnim() {
	return this->scene->mAnimations[0];
}

void AnimationTree::readNodeHierarchy(float animationTime, const aiNode* node, aiMatrix4x4 parentMatrix, vector<BoneInfo> &boneInfos, unordered_map<string, unsigned int> boneMapping) {
	string nodeName(node->mName.data);

	const aiAnimation* pAnimation = this->scene->mAnimations[0];

	aiMatrix4x4 nodeTransformation(node->mTransformation);

	const aiNodeAnim* pNodeAnim = findNodeAnim(pAnimation, nodeName);

	if (pNodeAnim) {
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D scaling = calcInterpolatedScaling(animationTime, pNodeAnim);
		aiMatrix4x4 scalingM;
		scalingM.Scaling(scaling, scalingM);

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion rotationQ = calcInterpolatedRotation(animationTime, pNodeAnim);
		aiMatrix4x4 rotationM = aiMatrix4x4(rotationQ.GetMatrix());

		// Interpolate translation and generate translation transformation matrix
		aiVector3D translation = calcInterpolatedPosition(animationTime, pNodeAnim);
		aiMatrix4x4 translationM;
		translationM.Translation(translation, translationM);

		// Combine the above transformations
		// LATER, optimize by doing calculations manually 
		nodeTransformation = translationM * rotationM * scalingM;
	}

	aiMatrix4x4 globalTransformation = parentMatrix * nodeTransformation;

	if (boneMapping.find(nodeName) != boneMapping.end()) {
		unsigned int boneIndex = boneMapping[nodeName];
		boneInfos[boneIndex].FinalTransformation = this->globalInverse * globalTransformation * boneInfos[boneIndex].BoneOffset;
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		readNodeHierarchy(animationTime, node->mChildren[i], globalTransformation, boneInfos, boneMapping);
	}

}

unsigned int AnimationTree::findPosition(float animationTime, const aiNodeAnim* pNodeAnim)
{
	for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		if (animationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}

	return 0;
}


unsigned int AnimationTree::findRotation(float animationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);
	for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		if (animationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}

	return 0;
}


unsigned int AnimationTree::findScaling(float animationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		if (animationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}

	return 0;
}


aiVector3D AnimationTree::calcInterpolatedPosition(float animationTime, const aiNodeAnim* pNodeAnim)
{
	aiVector3D outVec;
	if (pNodeAnim->mNumPositionKeys == 1) {
		return pNodeAnim->mPositionKeys[0].mValue;
	}

	unsigned int positionIndex = findPosition(animationTime, pNodeAnim);
	unsigned int nextPositionIndex = (positionIndex + 1);
	assert(nextPositionIndex < pNodeAnim->mNumPositionKeys);
	float deltaTime = (float)(pNodeAnim->mPositionKeys[nextPositionIndex].mTime - pNodeAnim->mPositionKeys[positionIndex].mTime);
	float factor = (animationTime - (float)pNodeAnim->mPositionKeys[positionIndex].mTime) / deltaTime;
	const aiVector3D& start = pNodeAnim->mPositionKeys[positionIndex].mValue;
	const aiVector3D& end = pNodeAnim->mPositionKeys[nextPositionIndex].mValue;
	aiVector3D delta = end - start;
	return start + factor * delta;
}


aiQuaternion AnimationTree::calcInterpolatedRotation(float animationTime, const aiNodeAnim* pNodeAnim)
{
	aiQuaternion outVec;
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1) {
		return pNodeAnim->mRotationKeys[0].mValue;
	}

	unsigned int rotationIndex = findRotation(animationTime, pNodeAnim);
	unsigned int nextRotationIndex = (rotationIndex + 1);
	assert(nextRotationIndex < pNodeAnim->mNumRotationKeys);
	float deltaTime = (float)(pNodeAnim->mRotationKeys[nextRotationIndex].mTime - pNodeAnim->mRotationKeys[rotationIndex].mTime);
	float factor = (animationTime - (float)pNodeAnim->mRotationKeys[rotationIndex].mTime) / deltaTime;
	const aiQuaternion& startRotationQ = pNodeAnim->mRotationKeys[rotationIndex].mValue;
	const aiQuaternion& endRotationQ = pNodeAnim->mRotationKeys[nextRotationIndex].mValue;
	aiQuaternion::Interpolate(outVec, startRotationQ, endRotationQ, factor);
	return outVec.Normalize();
}


aiVector3D AnimationTree::calcInterpolatedScaling(float animationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1) {
		return pNodeAnim->mScalingKeys[0].mValue;
	}

	unsigned int scalingIndex = findScaling(animationTime, pNodeAnim);
	unsigned int nextScalingIndex = (scalingIndex + 1);
	assert(nextScalingIndex < pNodeAnim->mNumScalingKeys);
	float deltaTime = (float)(pNodeAnim->mScalingKeys[nextScalingIndex].mTime - pNodeAnim->mScalingKeys[scalingIndex].mTime);
	float factor = (animationTime - (float)pNodeAnim->mScalingKeys[scalingIndex].mTime) / deltaTime;
	const aiVector3D& start = pNodeAnim->mScalingKeys[scalingIndex].mValue;
	const aiVector3D& end = pNodeAnim->mScalingKeys[nextScalingIndex].mValue;
	aiVector3D delta = end - start;
	return start + factor * delta;
}

const aiNodeAnim* AnimationTree::findNodeAnim(const aiAnimation* pAnimation, const string nodeName)
{
	for (unsigned int i = 0; i < pAnimation->mNumChannels; i++) {
		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

		if (string(pNodeAnim->mNodeName.data) == nodeName) {
			return pNodeAnim;
		}
	}

	return NULL;
}
