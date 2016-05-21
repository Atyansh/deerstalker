#include "Model.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/ext.hpp>
#include <boost\filesystem.hpp>
#include <SOIL/SOIL.h>

Model::Model() : SGeode()
{
	
}

Model::Model(const char* path, Shader *shader) : SGeode()
{
	this->shader = shader;
	// numBones = 0;
	// this->mAnimTree = NULL;
	this->loadModel(path);
}

Model::~Model()
{

}

void Model::draw(DrawData &data){
	shader->bind();

	//if (this->mAnimTree != NULL) {

	//	float time = clock() / float(CLOCKS_PER_SEC);
	//	float delta = time - prevTime;
	//	prevTime = time;

	//	currAnimTime += min(delta, 0.07f);

	//	this->mAnimTree->boneTransfrom(currAnimTime, boneInfos, boneMapping);
	//}

	for (GLuint i = 0; i < this->meshes.size(); i++){
		//for (int j = 0; j < boneInfos.size(); j++) {
		//	this->meshes[i].setBoneMatrix(j, boneInfos[j].FinalTransformation); //set the transforms
		//}
		this->meshes[i].draw(data);
	}
}

void Model::update(UpdateData &updateData){

}

//float Model::getHeight() {
//	return height;
//}
/*Virtual helpers*/
void Model::addLoadMethod() {}

void Model::addProcessMeshMethod(const aiMesh* mesh, vector<Vertex>& vertices) {}

void Model::addProcessVertMethod(float y) {}


/*  Functions   */
// Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
void Model::loadModel(string path)
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
	/*if (boneMapping.size() > 0 && boneInfos.size() > 0) {
		this->mAnimTree = new AnimationTree(scene, modelInverseMat);
	}
	cout << endl;*/
}

// Processes a node in a recursive fashio
void Model::processNode(aiNode* node, const aiScene* scene)
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

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
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

	////process bones
	//cout << "Bones: " << mesh->mNumBones << endl;
	//if (mesh->mNumBones > 0) {
	//	loadBones(mesh, vertices);
	//}
	addProcessMeshMethod(mesh, vertices);

	//process faces
	processFaces(mesh, indices);

	//process material w/ textures
	processMaterial(mesh, scene, textures, materialNoTex);

	return Mesh(vertices, indices, textures, materialNoTex, this->shader, mesh->mNumBones > 0);
}

void Model::processVerts(aiMesh* mesh, vector<Vertex> &vertices){
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

//void Model::loadBones(const aiMesh* mesh, vector<Vertex>& vertices)
//{
//	for (unsigned int i = 0; i < mesh->mNumBones; i++) {
//		string boneName(mesh->mBones[i]->mName.data);
//		unsigned int boneIndex = 0;
//		cout << boneName << endl;
//
//		//for animation
//		if (boneMapping.find(boneName) == boneMapping.end()) {
//			// Allocate a new bone
//			boneIndex = numBones;
//			numBones++;
//			BoneInfo bi;
//			bi.BoneOffset = mesh->mBones[i]->mOffsetMatrix;
//			boneInfos.push_back(bi);
//			boneMapping.insert(pair<string, unsigned int>(boneName, boneIndex));
//		} else {
//			boneIndex = boneMapping[boneName];
//		}
//
//		//bone stuff
//		for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
//			unsigned int VertexID = mesh->mBones[i]->mWeights[j].mVertexId;
//			float weight = mesh->mBones[i]->mWeights[j].mWeight;
//			for (int k = 0; k < vertices[VertexID].BoneIds.length(); k++) {
//				if (vertices[VertexID].Weights[k] == 0.0f) {
//					vertices[VertexID].BoneIds[k] = boneIndex;
//					vertices[VertexID].Weights[k] = weight;
//					/*cout << VertexID << endl;
//					cout << glm::to_string(vertices[VertexID].Position) << endl;
//					cout << glm::to_string(vertices[VertexID].BoneIds) << endl;
//					cout << glm::to_string(vertices[VertexID].Weights) << endl;
//					cout << endl;*/
//					break;
//				}
//			}
//			
//		}
//		
//	}
//}

void Model::processFaces(aiMesh* mesh, vector<GLuint> &indices){
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
}

void Model::processMaterial(aiMesh* mesh, const aiScene* scene, vector<Texture> &textures, MaterialNoTex &materialNoTex){
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		if (mesh->HasTextureCoords(0)) // Does the mesh contain texture coordinates?
		{
			vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");

			if (diffuseMaps.size() == 0 && specularMaps.size() == 0) {
				aiColor3D color(0.f, 0.f, 0.f);
				float shininess = 0.0f;
				material->Get(AI_MATKEY_COLOR_AMBIENT, color);
				materialNoTex.ambient = glm::vec3(color[0], color[1], color[2]);
				material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
				materialNoTex.diffuse = glm::vec3(color[0], color[1], color[2]);
				material->Get(AI_MATKEY_COLOR_SPECULAR, color);
				materialNoTex.specular = glm::vec3(color[0], color[1], color[2]);
				material->Get(AI_MATKEY_SHININESS, shininess);
				materialNoTex.shininess = shininess;
			} else {
				textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
				textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			}
			
		} else {
			aiColor3D color(0.f, 0.f, 0.f);
			float shininess = 0.0f;
			material->Get(AI_MATKEY_COLOR_AMBIENT, color);
			materialNoTex.ambient = glm::vec3(color[0], color[1], color[2]);
			material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			materialNoTex.diffuse = glm::vec3(color[0], color[1], color[2]);
			material->Get(AI_MATKEY_COLOR_SPECULAR, color);
			materialNoTex.specular = glm::vec3(color[0], color[1], color[2]);
			material->Get(AI_MATKEY_SHININESS, shininess);
			materialNoTex.shininess = shininess;
		}
	}
}

// Checks all material textures of a given type and loads the textures if they're not loaded yet.
// The required info is returned as a Texture struct.
vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		GLboolean skip = false;
		for (GLuint j = 0; j < textures_loaded.size(); j++)
		{
			if (textures_loaded[j].path == str)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // If texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			this->textures_loaded.push_back(texture);

			//TODO, be able to load texture from FBX
			// https://github.com/acgessler/open3mod/blob/047be0a5be39f36064c97af08d5b7b37cc1182b1/open3mod/MaterialMapperClassicGl.cs
		}
	}
	return textures;
}

//GLint Model::TextureFromFile(const char* path, string directory)
//{
//	//Generate texture ID and load texture data 
//	string filename = string(path);
//	filename = directory + '/' + filename;
//
//	int width, height;
//	unsigned char* image = loadPPM(filename.c_str(), width, height);
//	if (image == NULL || image[0] == '\0') {
//		return -1;
//	}
//
//	// Create Texture
//	GLuint textureID;
//	glGenTextures(1, &textureID);
//	// Assign texture to ID
//	glBindTexture(GL_TEXTURE_2D, textureID);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
//	glGenerateMipmap(GL_TEXTURE_2D);
//
//	// Parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glBindTexture(GL_TEXTURE_2D, 0);
//	return textureID;
//}

//unsigned char* Model::loadPPM(const char* filename, int& width, int& height)
//{
//	const int BUFSIZE = 128;
//	FILE* fp;
//	size_t read;
//	unsigned char* rawData;
//	char buf[3][BUFSIZE];
//	char* retval_fgets;
//	size_t retval_sscanf;
//
//	//Open the texture file
//	if ((fp = fopen(filename, "rb")) == NULL)
//	{
//		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
//		width = 0;
//		height = 0;
//		return NULL;
//	}
//
//	// Read magic number:
//	retval_fgets = fgets(buf[0], BUFSIZE, fp);
//
//	// Read width and height:
//	do
//	{
//		retval_fgets = fgets(buf[0], BUFSIZE, fp);
//	} while (buf[0][0] == '#');
//
//	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
//	width = atoi(buf[1]);
//	height = atoi(buf[2]);
//
//	// Read maxval:
//	do
//	{
//		retval_fgets = fgets(buf[0], BUFSIZE, fp);
//	} while (buf[0][0] == '#');
//
//	// Read image data:
//	rawData = new unsigned char[width * height * 3];
//	read = fread(rawData, width * height * 3, 1, fp);
//	fclose(fp);
//	if (read != 1)
//	{
//		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
//		delete[] rawData;
//		width = 0;
//		height = 0;
//		return NULL;
//	}
//
//	return rawData;
//}


GLint Model::TextureFromFile(const char* path, string directory)
{
	//Generate texture ID and load texture data 
	string filename = string(path);
	filename = directory + '/' + filename;
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
	// SOIL_free_image_data(image);
	return textureID;
}