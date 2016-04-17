#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <assimp/scene.h>

#include "SGeode.h"
#include "Mesh.h"

class Model : public SGeode
{
private:
	/*  Model Data  */
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded;
	Shader *shader;

	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	GLint TextureFromFile(const char* path, string directory);
	unsigned char* loadPPM(const char*, int&, int&);
	bool hasTextureFiles();


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
