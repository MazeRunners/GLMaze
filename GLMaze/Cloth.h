#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

struct ClothVertex {
	glm::vec3 vPos;    
	glm::vec3 vNor;
	glm::vec3 vVel;
	float mass;
	glm::vec3 Fspring;
	glm::vec3 Fgravity;
	glm::vec3 Fdamping;
	glm::vec3 Fviscous;
	glm::vec3 Ffuse;  // force fusion 
	
	ClothVertex(): vPos(0,0,0), vNor(0,0,1), vVel(0,0,0), mass(1.0f) {}
};

class Cloth { 
public:    
	Cloth(float gridWidth_, int width_, int height_);  // initialization    
	~Cloth();  // free the space for safety    
	void draw();  // ClothSimulation main entrance    
	void ProcessInput(GLFWwindow* window); 

private:    
	float gridWidth;  // width of a grid  
	std::vector<ClothVertex> cVers;  // vertices set pointer    
	std::vector<unsigned int> indices;

	int width;
	int height;
						  
	// some global params    
	float restLen[3];   // structural, shear, and flexion    
	float stiff[3];     // structural, shear, and flexion    
	float Cg;           // gravity coefficient    
	float Cd;           // damping coefficient    
	float Cv;           // viscous coefficient    
	glm::vec3 Ufluid;   // viscous coefficient
	float lastCalcTime;

private:
	unsigned int VAO, VBO, EBO;

private:    
	void CreateClothVertex();     // create all vertex around center (0, 0)
	void InitClothVertex(int i, int j);
	void InitBuffers();
	void UpdateVertexPosition();  // first part of ClothSimulating()    
	void RenderClothPlane();      // second part of ClothSimulating()    
	glm::vec3 CalNormal(int i, int j);        // Calculate normal of a vertex
	
	// forces
	glm::vec3 CalSpringForceBetween(glm::vec3 p, glm::vec3 q, float k, float l);  // Calculate SpringForce between 2 vertices  
	glm::vec3 CalSpringForce(int i, int j);   // Calculate SpringForce of a vertex  
	glm::vec3 CalGravityForce(int i, int j);  // Calculate GravityForce of a vertex    
	glm::vec3 CalDampingForce(int i, int j);  // Calculate DampingForce of a vertex    
	glm::vec3 CalViscousForce(int i, int j);  // Calculate ViscousForce of a vertex    
	glm::vec3 CalSpringForceStruct(int i, int j);   // Structural Springs part of CalSpringForce()    
	glm::vec3 CalSpringForceShear(int i, int j);    // Shear Springs part of CalSpringForce()    
	glm::vec3 CalSpringForceFlexion(int i, int j);  // Flexion Springs part of CalSpringForce()    
	void AddManualForce();  // Add manual force to the cloth 

	glm::vec3 UpdateForceFusion(int i, int j);
	glm::vec3 CalAccelaration(int i, int j);
};
