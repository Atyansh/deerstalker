#pragma once

#include <assimp/scene.h>
#include <vector>
#include <unordered_map>

#include "BoneInfo.h"

using namespace std;
class AnimationTree {

private:
	/*string name;
	aiMatrix4x4 transform;
	aiMatrix4x4 inverseMat;
	vector<AnimationTree> children;*/

	const aiScene *scene;
	aiMatrix4x4 globalInverse;

	void readNodeHierarchy(float animationTime, const aiNode* node, aiMatrix4x4 parentMatrix, vector<BoneInfo> &boneInfos, unordered_map<string, unsigned int> boneMapping);
	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName);

	//void readNodeHierarchy(float animationTime, AnimationTree tree, aiMatrix4x4 parentMatrix, vector<BoneInfo> &boneInfos, unordered_map<string, unsigned int> boneMapping);

public:
	AnimationTree();
	AnimationTree(const aiScene* scene, aiMatrix4x4 globalInverse);
	~AnimationTree();

	/*void setTransform(aiMatrix4x4 transform);
	void setInverseMat(aiMatrix4x4 inverseMat);
	void setName(string name);

	void readNodeHierarchy(float animationTime, vector<BoneInfo> &boneInfos, unordered_map<string, unsigned int> boneMapping);

	void addNode(AnimationTree node);*/

	void boneTransfrom(float animationTime, vector<BoneInfo> &boneInfos, unordered_map<string, unsigned int> boneMapping);

	const aiScene* getScene();
};