//
// COMP 371 Assignment Framework - Team 4
// 
// Base code initially by Nicolas Bergeron and Gary Chang
// Updated by Rita Phommarath on 2015/04/03


// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>
#include <vector>

#include "Skybox.h"
#include "../Renderer.h"
#include "../TextureLoader.h"

using namespace glm;

/*

Added by Rita P.
Incomplete - testing object loading to load a skybox/skydome eventually
Must add library for loading obj
*/



Skybox::Skybox() : Model()
{
	// Load the texture
	//Texture = loadBMP("../Source/Textures/pluto.bmp");

	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals; // Won't be used at the moment.
	bool obj = Renderer::LoadOBJ("../Source/skybox.obj", vertices, uvs, normals);


	// Load it into a VBO
	glGenBuffers(1, &mVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);


	//Allows collision detection
	CreateDefaultCollisionCube();

}

Skybox::~Skybox()
{
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mVertexBufferID);
	glDeleteVertexArrays(1, &mVertexArrayID);
}

void Skybox::Update(float dt)
{
	Model::Update(dt);
}

void Skybox::Draw()
{
	// Draw the Vertex Buffer
	glBindVertexArray(mVertexArrayID);

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID = glGetUniformLocation(Renderer::GetShaderProgramID(), "mySamplerTexture");

	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]);

	//enable texture
	glEnable(GL_TEXTURE_2D);

	// Bind our texture in Texture Unit 8
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, Texture);

	// Set our "myTextureSampler" sampler to user Texture Unit 8
	glUniform1i(TextureID, 8);

	// 1st attribute buffer : vertex Positions
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(0,				// attribute. No particular reason for 0, but must match the layout in the shader.
		3,				// size
		GL_FLOAT,		// type
		GL_FALSE,		// normalized?
		0,				// stride
		(void*)0        // array buffer offset
		);

	// 2nd attribute buffer : vertex normal
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)sizeof(vec3)    // Normal is Offseted by vec3 (see class Vertex)
		);


	// Draw the triangles !
	glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices: 3 * 2 * 6 (3 per triangle, 2 triangles per face, 6 faces)

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

bool Skybox::ParseLine(const std::vector<ci_string> &token){
	if (token.empty()){
		return true;
	}
	else {
		return Model::ParseLine(token);
	}
}

