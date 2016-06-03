#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "Mesh.h"
#include "Model.h"
#include "BoneInfo.h"
#include "AnimationTree.h"
#include "util\Protos.pb.h"

class AnimModel : public Model
{
protected:
	/*  AnimModel Data  */
	unsigned int numBones;
	unordered_map<string, unsigned int> boneMapping;
	vector<BoneInfo> boneInfos; // transformation for bones

	/* Animation */
	AnimationTree *mAnimTree;
	bool isAnimated;

	float height = 0;

	void loadBones(const aiMesh* mesh, vector<Vertex>& bones);

	virtual void addLoadMethod();
	virtual void addProcessMeshMethod(const aiMesh* mesh, vector<Vertex>& vertices);
	virtual void addProcessVertMethod(float y);


public:
	AnimModel(const char* path, Shader *shader);
	virtual ~AnimModel(void);

	virtual void update(UpdateData&);
	virtual void draw(DrawData&);
	float getHeight();
	void setAnimation(bool isAnimated);
};