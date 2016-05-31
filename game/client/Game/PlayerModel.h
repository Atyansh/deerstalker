#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define MAX_PLAYERS 4

#include "PlayerMesh.h"
#include "Model.h"
#include "BoneInfo.h"
#include "AnimationTree.h"
#include "util\Protos.pb.h"

class PlayerModel : public Model
{
protected:
	/*  Model Data  */
	vector<PlayerMesh> meshes;
	string directory;
	unordered_map<std::uint32_t, vector<Texture>> textures_loaded;
	Shader *shader;
	aiMatrix4x4 modelInverseMat;
	unsigned int numBones;
	unordered_map<string, unsigned int> boneMapping;
	vector<BoneInfo> boneInfos; // transformation for bones

	/* Animation */
	Assimp::Importer importer;
	const aiScene *scene;
	AnimationTree *mAnimTree;
	float prevTime = 0;
	float currAnimTime = 0;

	float height = 0;

	protos::Message_GameObject_AnimationState state;

	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	void processVerts(aiMesh* mesh, vector<Vertex> &vertices);
	void processFaces(aiMesh* mesh, vector<GLuint> &indices);
	void processMaterial(aiMesh* mesh, const aiScene* scene, unordered_map<std::uint32_t, vector<Texture>> &textures, MaterialNoTex &materialNoTex);
	PlayerMesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, int id);
	virtual GLint TextureFromFile(const char* path, string directory, int id);

	//helper for extended classes
	virtual void addLoadMethod();
	virtual void addProcessMeshMethod(const aiMesh* mesh, vector<Vertex>& vertices);
	virtual void addProcessVertMethod(float y);
	// unsigned char* loadPPM(const char*, int&, int&);
	void loadBones(const aiMesh* mesh, vector<Vertex>& bones);
	// bool hasPPMTextureFiles();


public:
	PlayerModel();
	PlayerModel(const char* path, Shader *shader, protos::Message_GameObject_AnimationState state);
	virtual ~PlayerModel(void);

	virtual void update(UpdateData&);
	virtual void draw(DrawData&);
	float getHeight();


};
