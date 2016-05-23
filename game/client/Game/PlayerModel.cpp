#include "PlayerModel.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/ext.hpp>
#include <boost\filesystem.hpp>

PlayerModel::PlayerModel(const char* path, Shader *shader, PlayerState state) : Model()
{
	this->shader = shader;
	numBones = 0;
	height = 0.f;
	this->mAnimTree = NULL;
	this->state = state;
	this->loadModel(path);
	if(state == _standing){
		this->mAnimTree->boneTransfrom(0.f, boneInfos, boneMapping);
	}
}

PlayerModel::~PlayerModel()
{

}

void PlayerModel::draw(DrawData &data) {
	shader->bind();

	if(state != _standing){
		if (this->mAnimTree != NULL) {
			this->mAnimTree->boneTransfrom(data.animTime, boneInfos, boneMapping);
		}
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

/*Functions*/

void PlayerModel::addLoadMethod() {
	if (boneMapping.size() > 0 && boneInfos.size() > 0) {
		this->mAnimTree = new AnimationTree(scene, modelInverseMat);
	}
	cout << endl;
}

void PlayerModel::addProcessVertMethod(float y) {
	if (y > height) {
		height = y;
	}
}

void PlayerModel::addProcessMeshMethod(const aiMesh* mesh, vector<Vertex>& vertices) {
	cout << "Bones: " << mesh->mNumBones << endl;
	if (mesh->mNumBones > 0) {
		loadBones(mesh, vertices);
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