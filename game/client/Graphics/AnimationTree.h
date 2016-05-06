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
	void calcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void calcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void calcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
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