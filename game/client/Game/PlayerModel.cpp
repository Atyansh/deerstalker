#include "PlayerModel.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/ext.hpp>
#include <boost\filesystem.hpp>
#include <SOIL/SOIL.h>

PlayerModel::PlayerModel(const char* path, Shader *shader, protos::Message_GameObject_AnimationState state) : Model()
{
	cout << path << endl;
	this->shader = shader;
	numBones = 0;
	height = 0.f;
	this->mAnimTree = NULL;
	this->state = state;
	this->loadModel(path);
	if(state == protos::Message_GameObject_AnimationState_STANDING || state == protos::Message_GameObject_AnimationState_FLYING){
		this->mAnimTree->boneTransfrom(0.f, boneInfos, boneMapping);
	}
}

PlayerModel::~PlayerModel()
{

}

void PlayerModel::draw(DrawData &data) {
	shader->bind();

	if(state != protos::Message_GameObject_AnimationState_STANDING && state != protos::Message_GameObject_AnimationState_FLYING){
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

void PlayerModel::loadModel(string path)
{
	scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	this->directory = path.substr(0, path.find_last_of('/'));

	modelInverseMat = scene->mRootNode->mTransformation;
	modelInverseMat.Inverse();

	this->processNode(scene->mRootNode, scene);
	addLoadMethod();
}

// Processes a node in a recursive fashio
void PlayerModel::processNode(aiNode* node, const aiScene* scene)
{
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->processMesh(mesh, scene));
	}

	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}

}

PlayerMesh PlayerModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
	// Data to fill
	vector<Vertex> vertices;
	vector<GLuint> indices;
	unordered_map<std::uint32_t, vector<Texture>> texturesMap;
	MaterialNoTex materialNoTex;

	// bool hasTexture = hasPPMTextureFiles();
	// fprintf(stderr, "Has textures? : %s", hasTexture ? "yes" : "no");

	//process vertices
	processVerts(mesh, vertices);

	addProcessMeshMethod(mesh, vertices);

	//process faces
	processFaces(mesh, indices);

	//process material w/ textures
	processMaterial(mesh, scene, texturesMap, materialNoTex);

	return PlayerMesh(vertices, indices, texturesMap, materialNoTex, this->shader, mesh->mNumBones > 0);
}

void PlayerModel::processVerts(aiMesh* mesh, vector<Vertex> &vertices) {
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		addProcessVertMethod(vector.y);
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

void PlayerModel::processFaces(aiMesh* mesh, vector<GLuint> &indices) {
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
}

void PlayerModel::processMaterial(aiMesh* mesh, const aiScene* scene, unordered_map<std::uint32_t, vector<Texture>> &textures, MaterialNoTex &materialNoTex) {
	for (int i = 1; i <= MAX_PLAYERS; i++) {
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			if (mesh->HasTextureCoords(0)) // Does the mesh contain texture coordinates?
			{
				vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", i);
				vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", i);

				if (diffuseMaps.size() == 0 && specularMaps.size() == 0) {
					aiColor3D color(0.f, 0.f, 0.f);
					material->Get(AI_MATKEY_COLOR_AMBIENT, color);
					materialNoTex.ambient = glm::vec3(color[0], color[1], color[2]);
					material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
					materialNoTex.diffuse = glm::vec3(color[0], color[1], color[2]);
					material->Get(AI_MATKEY_COLOR_SPECULAR, color);
					materialNoTex.specular = glm::vec3(color[0], color[1], color[2]);

					if (materialNoTex.ambient == glm::vec3(0.f)) {
						materialNoTex.ambient = materialNoTex.diffuse * 0.6f; // if no value, ambient is 60% color of diffuse
					}


				}
				else {
					textures[i].insert(textures[i].end(), diffuseMaps.begin(), diffuseMaps.end());
					textures[i].insert(textures[i].end(), specularMaps.begin(), specularMaps.end());
				}

			}
			else {
				aiColor3D color(0.f, 0.f, 0.f);
				material->Get(AI_MATKEY_COLOR_AMBIENT, color);
				materialNoTex.ambient = glm::vec3(color[0], color[1], color[2]);
				material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
				materialNoTex.diffuse = glm::vec3(color[0], color[1], color[2]);
				material->Get(AI_MATKEY_COLOR_SPECULAR, color);
				materialNoTex.specular = glm::vec3(color[0], color[1], color[2]);
			}
		}
	}
	
}

// Checks all material textures of a given type and loads the textures if they're not loaded yet.
// The required info is returned as a Texture struct.
vector<Texture> PlayerModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, int id)
{
	vector<Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		GLboolean skip = false;
		for (GLuint j = 0; j < textures_loaded[id].size(); j++)
		{
			if (textures_loaded[id][j].path == str)
			{
				textures.push_back(textures_loaded[id][j]);
				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // If texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory, id);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			this->textures_loaded[id].push_back(texture);

			//TODO, be able to load texture from FBX
			// https://github.com/acgessler/open3mod/blob/047be0a5be39f36064c97af08d5b7b37cc1182b1/open3mod/MaterialMapperClassicGl.cs
		}
	}
	return textures;
}

GLint PlayerModel::TextureFromFile(const char* path, string directory, int id)
{
	//Generate texture ID and load texture data 
	string filename = string(path);
	int index = filename.find("_x");
	if (index != -1) {
		switch (id) {
		case 1:
			filename.replace(index, index + 2, "_blue.png");
			break;
		case 2:
			filename.replace(index, index + 2, "_green.png");
			break;
		case 3:
			filename.replace(index, index + 2, "_purple.png");
			break;
		case 4:
			filename.replace(index, index + 2, "_red.png");
			break;
		default:
			filename.replace(index, index + 2, "_blue.png");
			break;
		}
	}
	filename = directory + '/' + filename;
	cout << filename << endl;
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	if (image == NULL || image[0] == '\0') {
		return -1;
	}

	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}