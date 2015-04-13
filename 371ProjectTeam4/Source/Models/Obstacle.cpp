//
// COMP 371 Assignment Framework - Team 4
// 
// Base code initially by Nicolas Bergeron and Gary Chang
// Updated by Rita Phommarath on 2015/04/03


// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>

#include "Obstacle.h"
#include "../Renderer.h"
#include "../TextureLoader.h"

using namespace glm;


Obstacle::Obstacle(vec3 size) : Model()
{
	//Loads the texture
	Texture = loadBMP("../Source/Textures/tiles3.bmp");
	
	//Allows collision detection
	CreateDefaultCollisionCube();

	// Create Vertex Buffer for all the verices of the Cube
	vec3 halfSize = size * 0.5f;

	Vertex vertexBuffer[] = {  // position,                normal,                  color-white
		{ vec3(-halfSize.x, -halfSize.y, -halfSize.z), vec3(-1.0f, 0.0f, 0.0f), vec3(1.0, 1.0, 1.0) }, //left 
		{ vec3(-halfSize.x, -halfSize.y, halfSize.z), vec3(-1.0f, 0.0f, 0.0f), vec3(1.0, 1.0, 1.0) },
		{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3(-1.0f, 0.0f, 0.0f), vec3(1.0, 1.0, 1.0) },

		{ vec3(-halfSize.x, -halfSize.y, -halfSize.z), vec3(-1.0f, 0.0f, 0.0f), vec3(1.0, 1.0, 1.0) },
		{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3(-1.0f, 0.0f, 0.0f), vec3(1.0, 1.0, 1.0) },
		{ vec3(-halfSize.x, halfSize.y, -halfSize.z), vec3(-1.0f, 0.0f, 0.0f), vec3(1.0, 1.0, 1.0) },

		{ vec3(halfSize.x, halfSize.y, -halfSize.z), vec3(0.0f, 0.0f, -1.0f), vec3(1.0, 1.0, 1.0) }, // far
		{ vec3(-halfSize.x, -halfSize.y, -halfSize.z), vec3(0.0f, 0.0f, -1.0f), vec3(1.0, 1.0, 1.0) },
		{ vec3(-halfSize.x, halfSize.y, -halfSize.z), vec3(0.0f, 0.0f, -1.0f), vec3(1.0, 1.0, 1.0) },

		{ vec3(halfSize.x, halfSize.y, -halfSize.z), vec3(0.0f, 0.0f, -1.0f), vec3(1.0, 1.0, 1.0) },
		{ vec3(halfSize.x, -halfSize.y, -halfSize.z), vec3(0.0f, 0.0f, -1.0f), vec3(1.0, 1.0, 1.0) },
		{ vec3(-halfSize.x, -halfSize.y, -halfSize.z), vec3(0.0f, 0.0f, -1.0f), vec3(1.0, 1.0, 1.0) },

		{ vec3(halfSize.x, -halfSize.y, halfSize.z), vec3(0.0f, -1.0f, 0.0f), vec3(1.0, 1.0, 1.0) }, // bottom 
		{ vec3(-halfSize.x, -halfSize.y, -halfSize.z), vec3(0.0f, -1.0f, 0.0f), vec3(1.0, 1.0, 1.0) },
		{ vec3(halfSize.x, -halfSize.y, -halfSize.z), vec3(0.0f, -1.0f, 0.0f), vec3(1.0, 1.0, 1.0) },

		{ vec3(halfSize.x, -halfSize.y, halfSize.z), vec3(0.0f, -1.0f, 0.0f), vec3(1.0, 1.0, 1.0) },
		{ vec3(-halfSize.x, -halfSize.y, halfSize.z), vec3(0.0f, -1.0f, 0.0f), vec3(1.0, 1.0, 1.0) },
		{ vec3(-halfSize.x, -halfSize.y, -halfSize.z), vec3(0.0f, -1.0f, 0.0f), vec3(1.0, 1.0, 1.0) },

		{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3(0.0f, 0.0f, 1.0f), vec3(1.0, 1.0, 1.0) }, // near 
		{ vec3(-halfSize.x, -halfSize.y, halfSize.z), vec3(0.0f, 0.0f, 1.0f), vec3(1.0, 1.0, 1.0) },
		{ vec3(halfSize.x, -halfSize.y, halfSize.z), vec3(0.0f, 0.0f, 1.0f), vec3(1.0, 1.0, 1.0) },

		{ vec3(halfSize.x, halfSize.y, halfSize.z), vec3(0.0f, 0.0f, 1.0f), vec3(1.0, 1.0, 1.0) },
		{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3(0.0f, 0.0f, 1.0f), vec3(1.0, 1.0, 1.0) },
		{ vec3(halfSize.x, -halfSize.y, halfSize.z), vec3(0.0f, 0.0f, 1.0f), vec3(1.0, 1.0, 1.0) },

		{ vec3(halfSize.x, halfSize.y, halfSize.z), vec3(1.0f, 0.0f, 0.0f), vec3(1.0, 1.0, 1.0) }, // right 
		{ vec3(halfSize.x, -halfSize.y, -halfSize.z), vec3(1.0f, 0.0f, 0.0f), vec3(1.0, 1.0, 1.0) },
		{ vec3(halfSize.x, halfSize.y, -halfSize.z), vec3(1.0f, 0.0f, 0.0f), vec3(1.0, 1.0, 1.0) },

		{ vec3(halfSize.x, -halfSize.y, -halfSize.z), vec3(1.0f, 0.0f, 0.0f), vec3(1.0, 1.0, 1.0) },
		{ vec3(halfSize.x, halfSize.y, halfSize.z), vec3(1.0f, 0.0f, 0.0f), vec3(1.0, 1.0, 1.0) },
		{ vec3(halfSize.x, -halfSize.y, halfSize.z), vec3(1.0f, 0.0f, 0.0f), vec3(1.0, 1.0, 1.0) },

		{ vec3(halfSize.x, halfSize.y, halfSize.z), vec3(0.0f, 1.0f, 0.0f), vec3(1.0, 1.0, 1.0) }, // top 
		{ vec3(halfSize.x, halfSize.y, -halfSize.z), vec3(0.0f, 1.0f, 0.0f), vec3(1.0, 1.0, 1.0) },
		{ vec3(-halfSize.x, halfSize.y, -halfSize.z), vec3(0.0f, 1.0f, 0.0f), vec3(1.0, 1.0, 1.0) },

		{ vec3(halfSize.x, halfSize.y, halfSize.z), vec3(0.0f, 1.0f, 0.0f), vec3(1.0, 1.0, 1.0) },
		{ vec3(-halfSize.x, halfSize.y, -halfSize.z), vec3(0.0f, 1.0f, 0.0f), vec3(1.0, 1.0, 1.0) },
		{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3(0.0f, 1.0f, 0.0f), vec3(1.0, 1.0, 1.0) }
	};

	static const GLfloat g_uv_buffer_data[] =
	{
		0.05f, 0.05f, //left
		0.05f, 0.0f,
		0.0f, 0.0f,

		0.05f, 0.05f,
		0.0f, 0.0f,
		0.0f, 0.05f,

		0.0f, 0.05f, //far
		0.05f, 0.0f,
		0.0f, 0.0f,

		0.0f, 0.05f,
		0.05f, 0.05f,
		0.05f, 0.0f,

		0.05f, 0.05f, // bottom
		0.0f, 0.0f,
		0.0f, 0.05f,

		0.05f, 0.05f,
		0.05f, 0.0f,
		0.0f, 0.0f,

		0.0f, 0.0f, //near
		0.05f, 0.0f,
		0.05f, 0.05f,

		0.0f, 0.05f,
		0.0f, 0.0f,
		0.05f, 0.05f,

		0.0f, 0.0f, //right
		0.05f, 0.05f,
		0.0f, 0.05f,

		0.05f, 0.05f,
		0.0f, 0.0f,
		0.05f, 0.0f,

		0.05f, 0.05f, //top
		0.0f, 0.05f,
		0.0f, 0.0f,

		0.05f, 0.05f,
		0.0f, 0.0f,
		0.05f, 0.0f

	};


	// Create a vertex array
	glGenVertexArrays(1, &mVertexArrayID);

	// Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
	glGenBuffers(1, &mVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);


	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

	glDisable(GL_TEXTURE_2D);

}

Obstacle::~Obstacle()
{
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mVertexBufferID);
	glDeleteVertexArrays(1, &mVertexArrayID);
}

void Obstacle::Update(float dt)
{
	Model::Update(dt);
}

void Obstacle::Draw()
{
	// Draw the Vertex Buffer
	glBindVertexArray(mVertexArrayID);

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID = glGetUniformLocation(Renderer::GetShaderProgramID(), "mySamplerTexture");

	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]);

	//enable texture
	glEnable(GL_TEXTURE_2D);

	// Bind our texture in Texture Unit 5
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, Texture);

	// Set our "myTextureSampler" sampler to user Texture Unit 5
	glUniform1i(TextureID, 5);

	// 1st attribute buffer : vertex Positions
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(0,				// attribute. No particular reason for 0, but must match the layout in the shader.
		3,				// size
		GL_FLOAT,		// type
		GL_FALSE,		// normalized?
		sizeof(Vertex), // stride
		(void*)0        // array buffer offset
		);

	// 2nd attribute buffer : vertex normal
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)sizeof(vec3)    // Normal is Offseted by vec3 (see class Vertex)
		);


	// 3rd attribute buffer : vertex color
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(2 * sizeof(vec3)) // Color is Offseted by 2 vec3 (see class Vertex)
		);

	// 4th attribute buffer : UVs
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		3,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);


	// Draw the triangles !
	glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices: 3 * 2 * 6 (3 per triangle, 2 triangles per face, 6 faces)

	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

bool Obstacle::ParseLine(const std::vector<ci_string> &token){
	if (token.empty()){
		return true;
	}
	else {
		return Model::ParseLine(token);
	}
}

