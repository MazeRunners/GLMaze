#include "GLModel.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <fstream>
#include <sstream>
#include <iostream>

GLModel::GLModel(const char* path) {
	std::string pathString = std::string(path);
	modelDirectory = pathString.substr(0, pathString.find_last_of('/'));

	loadModel(path);
}

void GLModel::draw(GLShader* shader, bool draw_texture) {
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].draw(shader, draw_texture);
}

void GLModel::loadModel(const char* path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	initNode(scene->mRootNode, scene);
}

void GLModel::initNode(aiNode *node, const aiScene *scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(initMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		initNode(node->mChildren[i], scene);
	}

}

GLMesh GLModel::initMesh(aiMesh *mesh, const aiScene *scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vector;

		// the coordinate system of Blender is different from that of OpenGL
		// OpenGL Blender
		// x = x
		// y = z
		// z = -y
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].z;
		vector.z = -mesh->mVertices[i].y;
		vertex.position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].z;
		vector.z = -mesh->mNormals[i].y;
		vertex.normal = vector;

		if (mesh->mTextureCoords[0]) {
			glm::vec2 vec;

			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;

			vertex.textureCoords = vec;
		}
		else {
			vertex.textureCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	std::vector<Texture> diffuseMaps = loadTexturesForMesh(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	std::vector<Texture> specularMaps = loadTexturesForMesh(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	std::vector<Texture> normalMaps = loadTexturesForMesh(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	std::vector<Texture> heightMaps = loadTexturesForMesh(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return GLMesh(vertices, indices, textures);
}

std::vector<Texture> GLModel::loadTexturesForMesh(aiMaterial *material, aiTextureType type, std::string typeName) {
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
		aiString path;
		material->GetTexture(type, i, &path);

		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++) {
			if (std::strcmp(textures_loaded[j].path.data(), path.C_Str()) == 0) {
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Texture texture;
			texture.id = loadTextureFromFile(path.C_Str());
			texture.type = typeName;
			texture.path = path.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}

unsigned int GLModel::loadTextureFromFile(const char *textureFile) {
	std::string path = std::string(textureFile);
	path = modelDirectory + '/' + path;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, componentNum;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &componentNum, 0);

	if (data) {

		GLenum format;
		switch (componentNum) {
		case 1: {
			format = GL_RED;
			break;
		}
		case 3: {
			format = GL_RGB;
			break;
		}
		case 4: {
			format = GL_RGBA;
			break;
		}
		default:
			break;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Texture failed to load at path: " << textureFile << std::endl;
		stbi_image_free(data);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}