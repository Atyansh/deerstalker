#include "PlayerModel.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/ext.hpp>
#include <boost\filesystem.hpp>

PlayerModel::PlayerModel(const char* path, Shader *shader) : Model(path, shader)
{
	this->shader = shader;
	numBones = 0;
	this->mAnimTree = NULL;
	this->loadModel(path);
}

PlayerModel::~PlayerModel()
{

}

void PlayerModel::draw(DrawData &data) {
	shader->bind();

	if (this->mAnimTree != NULL) {

		float time = clock() / float(CLOCKS_PER_SEC);
		float delta = time - prevTime;
		prevTime = time;

		currAnimTime += min(delta, 0.07f);

		this->mAnimTree->boneTransfrom(currAnimTime, boneInfos, boneMapping);
	}

	for (GLuint i = 0; i < this->meshes.size(); i++) {
		for (int j = 0; j < boneInfos.size(); j++) {
			this->meshes[i].setBoneMatrix(j, boneInfos[j].FinalTransformation); //set the transforms
		}
		this->meshes[i].draw(data);
	}
}

void PlayerModel::update(UpdateData &updateData) {

}

float PlayerModel::getHeight() {
	return height;
}

/*  Functions   */
Mesh PlayerModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
	// Data to fill
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;
	MaterialNoTex materialNoTex;

	// bool hasTexture = hasPPMTextureFiles();
	// fprintf(stderr, "Has textures? : %s", hasTexture ? "yes" : "no");

	//process vertices
	processVerts(mesh, vertices);

	//process bones
	cout << "Bones: " << mesh->mNumBones << endl;
	if (mesh->mNumBones > 0) {
		loadBones(mesh, vertices);
	}

	//process faces
	processFaces(mesh, indices);

	//process material w/ textures
	processMaterial(mesh, scene, textures, materialNoTex);

	return Mesh(vertices, indices, textures, materialNoTex, this->shader, mesh->mNumBones > 0);
}

void PlayerModel::processVerts(aiMesh* mesh, vector<Vertex> &vertices) {
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		if (mesh->mVertices[i].y > height) {
			height = mesh->mVertices[i].y;
		}
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		if (mesh->HasTextureCoords(0)) // Does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
}

void PlayerModel::loadBones(const aiMesh* mesh, vector<Vertex>& vertices)
{
	for (unsigned int i = 0; i < mesh->mNumBones; i++) {
		string boneName(mesh->mBones[i]->mName.data);
		unsigned int boneIndex = 0;
		cout << boneName << endl;

		//for animation
		if (boneMapping.find(boneName) == boneMapping.end()) {
			// Allocate a new bone
			boneIndex = numBones;
			numBones++;
			BoneInfo bi;
			bi.BoneOffset = mesh->mBones[i]->mOffsetMatrix;
			boneInfos.push_back(bi);
			boneMapping.insert(pair<string, unsigned int>(boneName, boneIndex));
		}
		else {
			boneIndex = boneMapping[boneName];
		}

		//bone stuff
		for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
			unsigned int VertexID = mesh->mBones[i]->mWeights[j].mVertexId;
			float weight = mesh->mBones[i]->mWeights[j].mWeight;
			for (int k = 0; k < vertices[VertexID].BoneIds.length(); k++) {
				if (vertices[VertexID].Weights[k] == 0.0f) {
					vertices[VertexID].BoneIds[k] = boneIndex;
					vertices[VertexID].Weights[k] = weight;
					/*cout << VertexID << endl;
					cout << glm::to_string(vertices[VertexID].Position) << endl;
					cout << glm::to_string(vertices[VertexID].BoneIds) << endl;
					cout << glm::to_string(vertices[VertexID].Weights) << endl;
					cout << endl;*/
					break;
				}
			}

		}

	}
}