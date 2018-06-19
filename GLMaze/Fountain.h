#pragma once

#include <glm/glm.hpp>

#include "Camera.h"
#include "Texture.h"
#include "GLShader.h"

namespace Fountain {
	const float particle_type_launcher = 0.0f;
	const float particle_type_shell = 1.0f;

	const glm::vec3 max_veloc = glm::vec3(0.0, -3.0, 0.0);
	const glm::vec3 min_veloc = glm::vec3(0.0, -1.0, 0.0);

	const float max_launch = 2.0f*1000.0f;
	const float min_launch = 0.3f*1000.0f;

	const float init_size = 10.0f;
	const float max_size = 8.0f;
	const float min_size = 3.0f;

	const float angle = 120.0f;

	const int max_particles = 10000;
	const int init_particles = 5000;

	const glm::vec3 center(0.0f);
	const float radius = 0.01f;

	struct WaterParticle {
		float type;
		glm::vec3 position;
		glm::vec3 velocity;
		float lifetimeMills;
		float size;
	};

	class Fountain {
	public:
		Fountain();
		~Fountain();
		void render(float frametimeMills, glm::mat4& worldMatrix, glm::mat4 viewMatrix, glm::mat4& projectMatrix);

	private:
		bool initFountain();
		void updateParticles(float frametimeMills);
		void initRandomTexture(unsigned int size);
		void renderParticles(glm::mat4& worldMatrix, glm::mat4& viewMatrix, glm::mat4& projectMatrix);
		void genInitLocation(WaterParticle partciles[], int nums);

		float deltaTime;
		float lastFrame;
		string str_fps;

		char c[8];
		int frameRate;
		int frameCount;

		unsigned int mCurVBOIndex, mCurTransformFeedbackIndex;
		unsigned int mParticleBuffers[2];
		unsigned int mParticleArrays[2];
		unsigned int mTransformFeedbacks[2];
		unsigned int mRandomTexture;

		CTexture mSparkTexture;
		CTexture mStartTexture;
		float mTimer;
		bool mFirst;
		GLShader* mUpdateShader;
		GLShader* mRenderShader;
	};
}
