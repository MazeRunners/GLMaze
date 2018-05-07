#pragma once
#include <assimp/scene.h>

#include "Mesh.h"

class Model {
public:
	Model(const char* modelPath);
	void draw(GLShader* shader, bool no_texture);

private:
	std::string modelDirectory;
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;

	void loadModel(const char* modelPath);
	void initNode(aiNode* node, const aiScene* scene);
	Mesh initMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> loadTexturesForMesh(aiMaterial* material, aiTextureType type, std::string typeName);
	unsigned int loadTextureFromFile(const char *textureFile);
};
