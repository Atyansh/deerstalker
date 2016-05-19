#pragma once

#include <assimp/scene.h>
#include <vector>
#include <unordered_map>

#include "BoneInfo.h"

using namespace std;
class AnimationTree {

private:
	const aiScene *scene;
	aiMatrix4x4 globalInverse;

	void readNodeHierarchy(float animationTime, const aiNode* node, aiMatrix4x4 parentMatrix, vector<BoneInfo> &boneInfos, unordered_map<string, unsigned int> boneMapping);
	aiVector3D calcInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
	aiQuaternion calcInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	aiVector3D calcInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int findScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int findRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int findPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	const aiNodeAnim* findNodeAnim(const aiAnimation* pAnimation, const string NodeName);

public:
	AnimationTree(const aiScene* scene, aiMatrix4x4 globalInverse);
	~AnimationTree();

	void boneTransfrom(float animationTime, vector<BoneInfo> &boneInfos, unordered_map<string, unsigned int> boneMapping);
	aiAnimation* getAnim();
};