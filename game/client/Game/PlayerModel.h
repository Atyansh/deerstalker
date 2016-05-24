#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "Mesh.h"
#include "Model.h"
#include "BoneInfo.h"
#include "AnimationTree.h"
#include "util\Protos.pb.h"

class PlayerModel : public Model
{
protected:
	/*  PlayerModel Data  */
	unsigned int numBones;
	unordered_map<string, unsigned int> boneMapping;
	vector<BoneInfo> boneInfos; // transformation for bones

	/* Animation */
	AnimationTree *mAnimTree;

	float height = 0;

	protos::Message_GameObject_AnimationState state;

	void loadBones(const aiMesh* mesh, vector<Vertex>& bones);

	virtual void addLoadMethod();
	virtual void addProcessMeshMethod(const aiMesh* mesh, vector<Vertex>& vertices);
	virtual void addProcessVertMethod(float y);


public:
	PlayerModel(const char* path, Shader *shader, protos::Message_GameObject_AnimationState state);
	virtual ~PlayerModel(void);

	virtual void update(UpdateData&);
	virtual void draw(DrawData&);
	float getHeight();
};