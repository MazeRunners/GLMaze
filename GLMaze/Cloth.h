#pragma once
#include <glm/glm.hpp>

#include <vector>

#include "GLShader.h"
#include "Lighting.h"

struct GLFWwindow;

class Cloth {
public:
	Cloth();  // initialization    
	~Cloth();  // free the space for safety   

	void render(glm::vec3 cameraPos, glm::mat4 viewTransformation, Lighting* lighting);  // Cloth Simulation main entrance    
	void processInput(GLFWwindow* window);

private:
	struct ClothVertex {
		glm::vec3 vPos;
		glm::vec3 vNor;
		glm::vec2 vTex;
		glm::vec3 vVel;

		float mass;
		glm::vec3 fSpring;
		glm::vec3 fGravity;
		glm::vec3 fDamping;
		glm::vec3 fViscous;
		glm::vec3 fFuse;  // force fusion 

		ClothVertex() : vPos(0, 0, 0), vNor(0, 0, 1), vTex(0, 0), vVel(0, 0, 0), mass(1.0f) {}
	};

	float gridWidth;  // width of a grid  
	std::vector<ClothVertex> cVers;  // vertices set pointer    
	std::vector<unsigned int> indices;

	int width;
	int height;

	// some global params    
	float restLen[3];   // structural, shear, and flexion    
	float stiff[3];     // structural, shear, and flexion    
	float coefG;           // gravity coefficient    
	float coefD;           // damping coefficient    
	float coefV;           // viscous coefficient    
	glm::vec3 coefFluid;   // viscous coefficient

	float lastCalcTime;

	unsigned int VAO, VBO, EBO;
	unsigned int texture;
	const char* texturePath = "./resource/cloth.jpg";
	GLShader clothShader = GLShader("./shader/clothShader.vert", "./shader/clothShader.frag");

	void readConfig();

	void loadTexture(const char* path);
	void createClothVertex();     // create all vertex around center (0, 0)
	void initClothVertex(int i, int j);
	void initBuffers();
	void updateVertexPosition();  // first part of ClothSimulating()    
	void renderClothPlane();      // second part of ClothSimulating()    
	glm::vec3 calNormal(int i, int j);        // Calculate normal of a vertex

	// forces  
	glm::vec3 calSpringForce(int i, int j);   // Calculate SpringForce of a vertex
	glm::vec3 calSpringForceBetween(glm::vec3 p, glm::vec3 q, float k, float l);  // Calculate SpringForce between 2 vertices  
	glm::vec3 calSpringForceStruct(int i, int j);   // Structural Springs part of CalSpringForce()    
	glm::vec3 calSpringForceShear(int i, int j);    // Shear Springs part of CalSpringForce()    
	glm::vec3 calSpringForceFlexion(int i, int j);  // Flexion Springs part of CalSpringForce()  

	glm::vec3 calGravityForce(int i, int j);  // Calculate GravityForce of a vertex    
	glm::vec3 calDampingForce(int i, int j);  // Calculate DampingForce of a vertex    
	glm::vec3 calViscousForce(int i, int j);  // Calculate ViscousForce of a vertex    

	void addManualForce();  // Add manual force to the cloth 

	glm::vec3 updateForceFusion(int i, int j);
	glm::vec3 calAccelaration(int i, int j);
};
