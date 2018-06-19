<<<<<<< HEAD
#ifndef PARTICLE_H
#define PARTICLE_H
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"
#include "game_object.h"


// Represents a single particle and its state
struct Particle {
    glm::vec2 Position, Velocity;
    glm::vec4 Color;
    GLfloat Life;

    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};


// ParticleGenerator acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.
class ParticleGenerator
{
public:
    // Constructor
    ParticleGenerator(Shader shader, Texture2D texture, GLuint amount);
    // Update all particles
    void Update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    // Render all particles
    void Draw();
private:
    // State
    std::vector<Particle> particles;
    GLuint amount;
    // Render state
    Shader shader;
    Texture2D texture;
    GLuint VAO;
    // Initializes buffer and vertex attributes
    void init();
    // Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
    GLuint firstUnusedParticle();
    // Respawns particle
    void respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

#endif
=======
#pragma once

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <algorithm>


#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;


#include "shader.hpp"
#include "texture.hpp"
#include "Camera.h"
#include "GLShader.h"

struct particle{
	glm::vec3 pos, speed;
	unsigned char r,g,b,a; // Color
	float size, angle, weight;
	float life; // Remaining life of the particle. if <0 : dead and unused.
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

	bool operator<(const particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};

class Particle {
public:
	Particle();
	Particle(glm::vec3 cameraFront, glm::vec3 cameraUp, glm::vec3 cameraPosition, glm::mat4 viewMatrix);
	~Particle();
	void draw();
	void init();
	void generateVertices();
	void generateParticles();
	void simulate();
	void SortParticles();
	int FindUnusedParticle();


private:
	const int MaxParticles = 1;
	particle ParticlesContainer[1];
	int LastUsedParticle = 0;
	double lastTime;
	double delta;
	int ParticlesCount;

	GLfloat* vertices;
	GLuint VAO;
	GLuint TextureID;
	//GLuint particleShader;
	//GLShader particleShader;
	//GLShader particleShader = GLShader("./shader/particle.vs", "./shader/particle.fs");
	GLuint billVBO;
	GLuint posVBO;
	GLuint colVBO;
	GLuint Texture;
	GLfloat* g_particule_position_size_data = new GLfloat[1 * 4];
	GLubyte* g_particule_color_data = new GLubyte[1 * 4];
	GLuint CameraRight_worldspace_ID;
	GLuint CameraUp_worldspace_ID;
	GLuint ViewProjMatrixID;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 cameraPos;
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;
};
>>>>>>> 125b0e618ad5028643248fe1a91482687189ead6
