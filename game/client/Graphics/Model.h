#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <assimp/scene.h>
#include <unordered_map>

#include "SGeode.h"
#include "Mesh.h"
#include "BoneInfo.h"
#include "AnimationTree.h"

class Model : public SGeode
{
private:
	/*  Model Data  */
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded;
	Shader *shader;
	aiMatrix4x4 modelInverseMat;
	unsigned int numBones;
	unordered_map<string, unsigned int> boneMapping;
	vector<BoneInfo> boneInfos; // transformation for bones
	AnimationTree mAnimTree;

	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene, AnimationTree &animTree);
	void processVerts(aiMesh* mesh, vector<Vertex> &vertices);
	void processFaces(aiMesh* mesh, vector<GLuint> &indices);
	void processMaterial(aiMesh* mesh, const aiScene* scene, vector<Texture> &textures);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	GLint TextureFromFile(const char* path, string directory);
	unsigned char* loadPPM(const char*, int&, int&);
	void loadBones(const aiMesh* mesh, vector<Vertex>& bones);
	// bool hasPPMTextureFiles();


public:
	Model(const char* path, Shader *shader);
	virtual ~Model(void);

	virtual void update(UpdateData&);
	virtual void draw(DrawData&);

	/*void printShaderPID() {
		fprintf(stderr, "Shader PID in model: %u\n", shader->getPid());
		for (int i = 0; i < meshes.size(); i++) {
			fprintf(stderr, "Mesh #%d:\n", i);
			meshes[i].printShaderPID();
		}
	}*/
};
