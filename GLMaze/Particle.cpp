#include "Particle.h"
#include <stb_image.h>
#include <iostream>
#include <fstream>


Particle::Particle() {

}

Particle::Particle(glm::vec3 right, glm::vec3 up, glm::vec3 cameraPosition, glm::mat4 viewMatrix) {
	cameraRight = right;
	cameraUp = up;
	projectionMatrix = viewMatrix;
	cameraPos = cameraPosition;
	init();
}

Particle::~Particle() {
	delete[] vertices;
}


	void Particle::generateParticles() {
		double currentTime = glfwGetTime();
		delta = currentTime - lastTime;
		lastTime = currentTime;

		// what the hell
		glm::mat4 ProjectionMatrix = projectionMatrix;
		glm::mat4 ViewMatrix = viewMatrix;

		// We will need the camera's position in order to sort the particles
		// w.r.t the camera's distance.
		// There should be a getCameraPosition() function in common/controls.cpp, 
		// but this works too.
		//glm::vec3 CameraPosition(glm::inverse(ViewMatrix)[3]);

		//glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;


		// Generate 10 new particule each millisecond,
		// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
		// newparticles will be huge and the next frame even longer.
		int newparticles = (int)(delta*10000.0);
		if (newparticles > (int)(0.016f*10000.0))
			newparticles = (int)(0.016f*10000.0);

		for (int i = 0; i < newparticles; i++) {
			int particleIndex = FindUnusedParticle();
			ParticlesContainer[particleIndex].life = 5.0f; // This particle will live 5 seconds.
			ParticlesContainer[particleIndex].pos = glm::vec3(0, 0, -20.0f);

			float spread = 1.5f;
			glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);
			// Very bad way to generate a random direction; 
			// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
			// combined with some user-controlled parameters (main direction, spread, etc)
			glm::vec3 randomdir = glm::vec3(
				(rand() % 2000 - 1000.0f) / 1000.0f,
				(rand() % 2000 - 1000.0f) / 1000.0f,
				(rand() % 2000 - 1000.0f) / 1000.0f
			);

			ParticlesContainer[particleIndex].speed = maindir + randomdir*spread;


			// Very bad way to generate a random color
			ParticlesContainer[particleIndex].r = rand() % 256;
			ParticlesContainer[particleIndex].g = rand() % 256;
			ParticlesContainer[particleIndex].b = rand() % 256;
			ParticlesContainer[particleIndex].a = (rand() % 256) / 3;

			ParticlesContainer[particleIndex].size = (rand() % 1000) / 2000.0f + 0.1f;
		}
		simulate();
	}

void Particle::simulate() {
	ParticlesCount = 0;
		for(int i=0; i<MaxParticles; i++){

			particle& p = ParticlesContainer[i]; // shortcut

			if(p.life > 0.0f){

				// Decrease life
				p.life -= delta;
				if (p.life > 0.0f){

					// Simulate simple physics : gravity only, no collisions
					p.speed += glm::vec3(0.0f,-9.81f, 0.0f) * (float)delta * 0.5f;
					p.pos += p.speed * (float)delta;
					p.cameradistance = glm::length2( p.pos - cameraPos );
					//ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

					// Fill the GPU buffer
					g_particule_position_size_data[4*ParticlesCount+0] = p.pos.x;
					g_particule_position_size_data[4*ParticlesCount+1] = p.pos.y;
					g_particule_position_size_data[4*ParticlesCount+2] = p.pos.z;
												   
					g_particule_position_size_data[4*ParticlesCount+3] = p.size;
												   
					g_particule_color_data[4*ParticlesCount+0] = p.r;
					g_particule_color_data[4*ParticlesCount+1] = p.g;
					g_particule_color_data[4*ParticlesCount+2] = p.b;
					g_particule_color_data[4*ParticlesCount+3] = p.a;

				}else{
					// Particles that just died will be put at the end of the buffer in SortParticles();
					p.cameradistance = -1.0f;
				}

				ParticlesCount++;

			}
		}

		SortParticles();
		draw();
}

void Particle::SortParticles() {
	std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}

void Particle::draw() {
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

	glBindBuffer(GL_ARRAY_BUFFER, colVBO);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Use our shader
	//glUseProgram(particleShader);
	//particleShader.use();
	//particleShader->use();

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(TextureID, 0);

	// Same as the billboards tutorial
	//particleShader.setVec3("CameraRight_worldspace", cameraRight);
	//particleShader->setVec3("CameraUp_worldspace", cameraUp);
	//particleShader->setMat4("VP", viewMatrix);
	//glUniform3f(CameraRight_worldspace_ID, viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
	//glUniform3f(CameraUp_worldspace_ID, viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);

	//glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &projectionMatrix[0][0]);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billVBO);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : x + y + z + size => 4
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 3rd attribute buffer : particles' colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, colVBO);
	glVertexAttribPointer(
		2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : r + g + b + a => 4
		GL_UNSIGNED_BYTE,                 // type
		GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// These functions are specific to glDrawArrays*Instanced*.
	// The first parameter is the attribute buffer we're talking about.
	// The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
	glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

								 // Draw the particules !
								 // This draws many times a small triangle_strip (which looks like a quad).
								 // This is equivalent to :
								 // for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4), 
								 // but faster.
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

int Particle::FindUnusedParticle() {

	for (int i = LastUsedParticle; i<MaxParticles; i++) {
		if (ParticlesContainer[i].life < 0) {
			LastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i<LastUsedParticle; i++) {
		if (ParticlesContainer[i].life < 0) {
			LastUsedParticle = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}

void Particle::init()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//particleShader = LoadShaders("particle.vs", "particle.fs");
	//particleShader = GLShader("particle.vs", "particle.fs");

	
		// Vertex shader
		/*CameraRight_worldspace_ID = glGetUniformLocation(particleShader, "CameraRight_worldspace");
		CameraUp_worldspace_ID = glGetUniformLocation(particleShader, "CameraUp_worldspace");
		ViewProjMatrixID = glGetUniformLocation(particleShader, "VP");

		// fragment shader
		TextureID = glGetUniformLocation(particleShader, "myTextureSampler");*/


		//static GLfloat* g_particule_position_size_data = new GLfloat[MaxParticles * 4];
		//static GLubyte* g_particule_color_data = new GLubyte[MaxParticles * 4];

	 for (int i = 0; i < MaxParticles; i++) {
		ParticlesContainer[i].life = -1.0f;
		ParticlesContainer[i].cameradistance = -1.0f;

		Texture = loadDDS("particle.DDS");
		vertices= new GLfloat[12] {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
			0.5f,  0.5f, 0.0f,
		};

		glGenBuffers(1, &billVBO);
		glBindBuffer(GL_ARRAY_BUFFER, billVBO);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

		// The VBO containing the positions and sizes of the particles
		//GLuint posVBO;
		glGenBuffers(1, &posVBO);
		glBindBuffer(GL_ARRAY_BUFFER, posVBO);
		// Initialize with empty (NULL) buffer : it will be updated later, each frame.
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

		// The VBO containing the colors of the particles
		//GLuint colVBO;
		glGenBuffers(1, &colVBO);
		glBindBuffer(GL_ARRAY_BUFFER, colVBO);
		// Initialize with empty (NULL) buffer : it will be updated later, each frame.
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	}
	lastTime = glfwGetTime();
	//generateParticles();
	//simulate();

}


