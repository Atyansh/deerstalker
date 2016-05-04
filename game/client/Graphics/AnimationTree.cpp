#include "AnimationTree.h"

AnimationTree::AnimationTree() {

}

AnimationTree::~AnimationTree() {

}

void AnimationTree::setTransform(aiMatrix4x4 transform){
	this->transform = transform;
}

void AnimationTree::setInverseMat(aiMatrix4x4 inverseMat){
	this->inverseMat = inverseMat;
}

void AnimationTree::setName(string name) {
	this->name = name;
}

void AnimationTree::addNode(AnimationTree node) {
	this->children.push_back(node);
}

void AnimationTree::readNodeHierarchy(float animationTime, vector<BoneInfo> &boneInfos, unordered_map<string, unsigned int> boneMapping){
	aiMatrix4x4 NodeTransformation(transform);
	
	//animation

	aiMatrix4x4 globalTransform = NodeTransformation;

	if (boneMapping.find(name) != boneMapping.end()) {
		unsigned int BoneIndex = boneMapping[name];
		 boneInfos[BoneIndex].FinalTransformation = inverseMat * globalTransform * boneInfos[BoneIndex].BoneOffset;
		//boneInfos[BoneIndex].FinalTransformation = boneInfos[BoneIndex].BoneOffset * globalTransform * inverseMat;
	}

	for(int i =0; i<this->children.size() && i < 1; i++){
		readNodeHierarchy(animationTime, this->children[i], globalTransform, boneInfos, boneMapping);
	}
}

void AnimationTree::readNodeHierarchy(float animationTime, AnimationTree tree, aiMatrix4x4 parentMatrix, vector<BoneInfo> &boneInfos, unordered_map<string, unsigned int> boneMapping){
	aiMatrix4x4 NodeTransformation(tree.transform);
	
	//animation

	aiMatrix4x4 globalTransform = parentMatrix * NodeTransformation;
	//aiMatrix4x4 globalTransform =  NodeTransformation * parentMatrix;

	if (boneMapping.find(tree.name) != boneMapping.end()) {
		unsigned int BoneIndex = boneMapping[tree.name];
		boneInfos[BoneIndex].FinalTransformation = tree.inverseMat * globalTransform * boneInfos[BoneIndex].BoneOffset;
		//boneInfos[BoneIndex].FinalTransformation = boneInfos[BoneIndex].BoneOffset * globalTransform * tree.inverseMat;

	}

	for(int i =0; i<tree.children.size(); i++){
		readNodeHierarchy(animationTime, tree.children[i], globalTransform, boneInfos, boneMapping);
	}
}
