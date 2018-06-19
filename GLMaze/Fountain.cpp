#include "Fountain.h"

namespace Fountain {
	Fountain::Fountain() {
		deltaTime = 0.0f;
		lastFrame = 0.0f;
		frameRate = 0;
		frameCount = 0;

		mCurVBOIndex = 0;
		mCurTransformFeedbackIndex = 1;
		mFirst = true;
		mTimer = 0;
		const GLchar* varyings[5] = { "Type1","Position1",
			"Velocity1","Age1","Size1"
		};
		mUpdateShader = new GLShader("../res/Shaders/Update.vs", "../res/Shaders/Update.fs",
			"../res/Shaders/Update.gs", varyings, 5);


		mRenderShader = new GLShader("../res/Shaders/Render.vs", "../res/Shaders/Render.fs");

		initRandomTexture(512);
		mSparkTexture.loadTexture("../res/Textures/water.bmp");
		mRenderShader->use();
		mRenderShader->setInt("water", 0);
		initFountain();
	}

	Fountain::~Fountain()
	{
	}

	bool Fountain::initFountain()
	{
		WaterParticle particles[max_particles];
		//WaterParticle particles = new WaterParticle[MAX_PARTICLES];
		memset(particles, 0, sizeof(particles));
		genInitLocation(particles, init_particles);
		GLuint * test = new GLuint[1024];
		glGenTransformFeedbacks(2, test);
		glGenBuffers(2, mParticleBuffers);
		glGenVertexArrays(2, mParticleArrays);
		for (int i = 0; i < 2; i++)
		{
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, mTransformFeedbacks[i]);
			glBindBuffer(GL_ARRAY_BUFFER, mParticleBuffers[i]);
			glBindVertexArray(mParticleArrays[i]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_DYNAMIC_DRAW);
			glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, mParticleBuffers[i]);
		}
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
		glBindVertexArray(0);

		mUpdateShader->use();
		glBindTexture(GL_TEXTURE_1D, mRandomTexture);
		mUpdateShader->setInt("gRandomTexture", 0);
		//delete[] particles;
		return true;
	}

	void Fountain::render(float frametimeMills, glm::mat4& worldMatrix,
		glm::mat4 viewMatrix, glm::mat4& projectMatrix)
	{
		mTimer += frametimeMills * 1000.0f;
		updateParticles(frametimeMills*1000.0f);
		renderParticles(worldMatrix, viewMatrix, projectMatrix);
		mCurVBOIndex = mCurTransformFeedbackIndex;
		mCurTransformFeedbackIndex = (mCurTransformFeedbackIndex + 1) & 0x1;
	}

	void Fountain::updateParticles(float frametimeMills)
	{
		mUpdateShader->use();
		mUpdateShader->setFloat("gDeltaTimeMillis", frametimeMills);
		mUpdateShader->setFloat("gTime", mTimer);
		mUpdateShader->setFloat("MAX_SIZE", max_size);
		mUpdateShader->setFloat("MIN_SIZE", min_size);
		mUpdateShader->setFloat("MAX_LAUNCH", max_launch);
		mUpdateShader->setFloat("MIN_LAUNCH", min_launch);
		mUpdateShader->setFloat("angle", angle);
		mUpdateShader->setFloat("R", radius);
		mUpdateShader->setVec3("NORMAL", glm::vec3(0, 1, 0));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_1D, mRandomTexture);

		glEnable(GL_RASTERIZER_DISCARD);
		glBindVertexArray(mParticleArrays[mCurVBOIndex]);
		glBindBuffer(GL_ARRAY_BUFFER, mParticleBuffers[mCurVBOIndex]);
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, mTransformFeedbacks[mCurTransformFeedbackIndex]);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(WaterParticle), (void*)offsetof(WaterParticle, type));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(WaterParticle), (void*)offsetof(WaterParticle, position));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(WaterParticle), (void*)offsetof(WaterParticle, velocity));
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(WaterParticle), (void*)offsetof(WaterParticle, lifetimeMills));
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(WaterParticle), (void*)offsetof(WaterParticle, size));
		glBeginTransformFeedback(GL_POINTS);
		if (mFirst)
		{
			glDrawArrays(GL_POINTS, 0, init_particles);
			mFirst = false;
		}
		else {
			glDrawTransformFeedback(GL_POINTS, mTransformFeedbacks[mCurVBOIndex]);
		}
		glEndTransformFeedback();
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
		glDisable(GL_RASTERIZER_DISCARD);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Fountain::renderParticles(glm::mat4& worldMatrix,
		glm::mat4& viewMatrix, glm::mat4& projectMatrix)
	{
		//glEnable(GL_POINT_SPRITE);   ǿ��ע�͵�
		glEnable(GL_POINT_SPRITE_COORD_ORIGIN);
		glEnable(GL_PROGRAM_POINT_SIZE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		mRenderShader->use();
		mRenderShader->setMat4("model", worldMatrix);
		mRenderShader->setMat4("view", viewMatrix);
		mRenderShader->setMat4("projection", projectMatrix);
		//glBindVertexArray(mParticleArrays[mCurTransformFeedbackIndex]);
		//glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER,0,mParticleBuffers[mCurTransformFeedbackIndex]);
		glBindBuffer(GL_ARRAY_BUFFER, mParticleBuffers[mCurTransformFeedbackIndex]);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(WaterParticle),
			(void*)offsetof(WaterParticle, position));
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(WaterParticle),
			(void*)offsetof(WaterParticle, size));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mSparkTexture.textureID);
		glDrawTransformFeedback(GL_POINTS, mTransformFeedbacks[mCurTransformFeedbackIndex]);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisable(GL_BLEND);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Fountain::initRandomTexture(unsigned int size)
	{
		srand(time(NULL));
		glm::vec3* pRandomData = new glm::vec3[size];
		for (int i = 0; i < size; i++)
		{
			pRandomData[i].x = float(rand()) / float(RAND_MAX);
			pRandomData[i].y = float(rand()) / float(RAND_MAX);
			pRandomData[i].z = float(rand()) / float(RAND_MAX);
		}
		glGenTextures(1, &mRandomTexture);
		glBindTexture(GL_TEXTURE_1D, mRandomTexture);
		glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, size, 0, GL_RGB, GL_FLOAT, pRandomData);
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		delete[] pRandomData;
		pRandomData = nullptr;
	}

	void Fountain::genInitLocation(WaterParticle particles[], int nums) {
		srand(time(NULL));
		for (int x = 0; x < nums; x++) {
			glm::vec3 record(0.0f);
			record.x = (2.0f*float(rand()) / float(RAND_MAX) - 1.0f)*radius;
			record.z = (2.0f*float(rand()) / float(RAND_MAX) - 1.0f)*radius;
			while (sqrt(record.x*record.x + record.z*record.z) > radius) {
				record.x = (2.0f*float(rand()) / float(RAND_MAX) - 1.0f)*radius;
				record.z = (2.0f*float(rand()) / float(RAND_MAX) - 1.0f)*radius;
			}
			record.y = 0.0f;
			particles[x].type = particle_type_launcher;
			particles[x].position = record;
			particles[x].velocity = glm::vec3(0.0f);
			particles[x].size = init_size;
			particles[x].lifetimeMills = (max_launch - min_launch)*(float(rand()) / float(RAND_MAX)) + min_launch;
		}
	}
}
