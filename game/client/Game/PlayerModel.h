#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "client\Graphics\Mesh.h"
#include "client\Graphics\Model.h"
#include "BoneInfo.h"
#include "AnimationTree.h"

class PlayerModel : public Model
{
private:
	/*  PlayerModel Data  */
	unsigned int numBones;
	unordered_map<string, unsigned int> boneMapping;
	vector<BoneInfo> boneInfos; // transformation for bones

	/* Animation */
	AnimationTree *mAnimTree;
	float prevTime = 0;
	float currAnimTime = 0;

	float height = 0;

	virtual void processVerts(aiMesh* mesh, vector<Vertex> &vertices);
	virtual Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	void loadBones(const aiMesh* mesh, vector<Vertex>& bones);


public:
	PlayerModel(const char* path, Shader *shader);
	virtual ~PlayerModel(void);

	virtual void update(UpdateData&);
	virtual void draw(DrawData&);
	float getHeight();

};