#pragma once

#include <assimp/scene.h>
#include <vector>
#include <unordered_map>

#include "BoneInfo.h"

using namespace std;
class AnimationTree {

private:
	string name;
	aiMatrix4x4 transform;
	aiMatrix4x4 inverseMat;
	vector<AnimationTree> children;

	void readNodeHierarchy(float animationTime, AnimationTree tree, aiMatrix4x4 parentMatrix, vector<BoneInfo> &boneInfos, unordered_map<string, unsigned int> boneMapping);

public:

	AnimationTree();
	~AnimationTree();

	void setTransform(aiMatrix4x4 transform);
	void setInverseMat(aiMatrix4x4 inverseMat);
	void setName(string name);

	void readNodeHierarchy(float animationTime, vector<BoneInfo> &boneInfos, unordered_map<string, unsigned int> boneMapping);

	void addNode(AnimationTree node);
};