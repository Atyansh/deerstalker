#include "Model.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/ext.hpp>
#include <boost\filesystem.hpp>

Model::Model(const char* path, Shader *shader) : SGeode()
{
	this->shader = shader;
	numBones = 0;
	this->mAnimTree = new AnimationTree();
	this->loadModel(path);
	
	/*if (boneMapping.size() > 0 && boneInfos.size() > 0) {
		this->mAnimTree.readNodeHierarchy(0, boneInfos, boneMapping);
	}*/

	this->mAnimTree->getScene();
	cout << endl;

}

Model::~Model()
{

}

void Model::draw(DrawData &data){
	shader->bind();

	animationFrame = animationFrame + 0.05f;
	if (animationFrame >= 3.7f) {
		animationFrame = 0;
	}

	this->mAnimTree->boneTransfrom(animationFrame, boneInfos, boneMapping);

	for (GLuint i = 0; i < this->meshes.size(); i++){
		for (int j = 0; j < boneInfos.size(); j++) {
			this->meshes[i].setBoneMatrix(j, boneInfos[j].FinalTransformation); //set the transforms
		}
		this->meshes[i].draw(data);
	}
}

void Model::update(UpdateData &updateData){

}

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
	if (boneMapping.size() > 0 && boneInfos.size() > 0) {
		this->mAnimTree = new AnimationTree(scene, modelInverseMat);
	}
	cout << endl;
}

// Processes a node in a recursive fashio
void Model::processNode(aiNode* node, const aiScene* scene)
{
	/*animTree.setInverseMat(this->modelInverseMat);
	animTree.setTransform(node->mTransformation);
	animTree.setName(node->mName.C_Str());*/

	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->processMesh(mesh, scene));
	}
	
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		//AnimationTree childAnimTree;
		this->processNode(node->mChildren[i], scene);
		//animTree.addNode(childAnimTree);
	}

}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	// Data to fill
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

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
	processMaterial(mesh, scene, textures);

	return Mesh(vertices, indices, textures, this->shader, mesh->mNumBones > 0);
}

void Model::processVerts(aiMesh* mesh, vector<Vertex> &vertices){
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
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

void Model::loadBones(const aiMesh* mesh, vector<Vertex>& vertices)
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
		} else {
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

void Model::processFaces(aiMesh* mesh, vector<GLuint> &indices){
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
}

void Model::processMaterial(aiMesh* mesh, const aiScene* scene, vector<Texture> &textures){
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// Diffuse: texture_diffuseN
		// Specular: texture_specularN
		// Normal: texture_normalN

		// 1. Diffuse maps
		vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. Specular maps
		vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
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
			this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

GLint Model::TextureFromFile(const char* path, string directory)
{
	//Generate texture ID and load texture data 
	string filename = string(path);
	filename = directory + '/' + filename;

	int width, height;
	unsigned char* image = loadPPM(filename.c_str(), width, height);

	// Create Texture
	GLuint textureID;
	glGenTextures(1, &textureID);
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
	return textureID;
}

/** Load a ppm file from disk.
@input filename The location of the PPM file.  If the file is not found, an error message
will be printed and this function will return 0
@input width This will be modified to contain the width of the loaded image, or 0 if file not found
@input height This will be modified to contain the height of the loaded image, or 0 if file not found
@return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured
**/
unsigned char* Model::loadPPM(const char* filename, int& width, int& height)
{
	const int BUFSIZE = 128;
	FILE* fp;
	size_t read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	//Open the texture file
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}

	// Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);

	// Read width and height:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width = atoi(buf[1]);
	height = atoi(buf[2]);

	// Read maxval:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}

	return rawData;
}

//bool Model::hasPPMTextureFiles() {
//
//	// With the assumption that there is nothing wrong with the
//	// directory path.
//	
//	boost::filesystem::recursive_directory_iterator beg(this->directory);
//	boost::filesystem::recursive_directory_iterator endit;
//	while (beg != endit) {
//		if (boost::filesystem::is_regular_file(*beg) && beg->path().extension() == ".ppm") {
//			return true;
//		}
//		beg++;
//	}
//
//	return false;
//}