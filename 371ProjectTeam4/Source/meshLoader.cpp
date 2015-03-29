


#include "MeshLoader.h"
#include "Renderer.h"
#include <GL/glew.h>

using namespace glm;



MeshLoader::MeshLoader(){
	vec3 color = vec3(1,1,1);
	vec3 size = vec3(1);
	vec3 rcol, lcol, tcol, bcol, ncol, fcol;
		 rcol = color;
		 lcol = color;
		 tcol = color;
		 bcol = color;
		 ncol = color;
		 fcol = color;
	


	vec3 halfSize = size * 0.5f;
	Vertex vertexBuffer[] = {  // position,                normal,                  color
								{ vec3(-halfSize.x,-halfSize.y,-halfSize.z), vec3(-1.0f, 0.0f, 0.0f),lcol }, //left - red
								{ vec3(-halfSize.x,-halfSize.y, halfSize.z), vec3(-1.0f, 0.0f, 0.0f),lcol },
								{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3(-1.0f, 0.0f, 0.0f),lcol },

								{ vec3(-halfSize.x,-halfSize.y,-halfSize.z), vec3(-1.0f, 0.0f, 0.0f),lcol },
								{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3(-1.0f, 0.0f, 0.0f),lcol },
								{ vec3(-halfSize.x, halfSize.y,-halfSize.z), vec3(-1.0f, 0.0f, 0.0f),lcol },

								{ vec3( halfSize.x, halfSize.y,-halfSize.z), vec3( 0.0f, 0.0f,-1.0f), fcol }, // far - blue
								{ vec3(-halfSize.x,-halfSize.y,-halfSize.z), vec3( 0.0f, 0.0f,-1.0f), fcol },
								{ vec3(-halfSize.x, halfSize.y,-halfSize.z), vec3( 0.0f, 0.0f,-1.0f), fcol },

								{ vec3( halfSize.x, halfSize.y,-halfSize.z), vec3( 0.0f, 0.0f,-1.0f), fcol },
								{ vec3( halfSize.x,-halfSize.y,-halfSize.z), vec3( 0.0f, 0.0f,-1.0f), fcol },
								{ vec3(-halfSize.x,-halfSize.y,-halfSize.z), vec3( 0.0f, 0.0f,-1.0f), fcol },

								{ vec3( halfSize.x,-halfSize.y, halfSize.z), vec3( 0.0f,-1.0f, 0.0f), bcol }, // bottom - turquoise
								{ vec3(-halfSize.x,-halfSize.y,-halfSize.z), vec3( 0.0f,-1.0f, 0.0f), bcol },
								{ vec3( halfSize.x,-halfSize.y,-halfSize.z), vec3( 0.0f,-1.0f, 0.0f), bcol },
																
								{ vec3( halfSize.x,-halfSize.y, halfSize.z), vec3( 0.0f,-1.0f, 0.0f), bcol },
								{ vec3(-halfSize.x,-halfSize.y, halfSize.z), vec3( 0.0f,-1.0f, 0.0f), bcol },
								{ vec3(-halfSize.x,-halfSize.y,-halfSize.z), vec3( 0.0f,-1.0f, 0.0f), bcol },

								{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3( 0.0f, 0.0f, 1.0f), ncol }, // near - green
								{ vec3(-halfSize.x,-halfSize.y, halfSize.z), vec3( 0.0f, 0.0f, 1.0f), ncol },
								{ vec3( halfSize.x,-halfSize.y, halfSize.z), vec3( 0.0f, 0.0f, 1.0f), ncol },

								{ vec3( halfSize.x, halfSize.y, halfSize.z), vec3( 0.0f, 0.0f, 1.0f), ncol },
								{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3( 0.0f, 0.0f, 1.0f), ncol },
								{ vec3( halfSize.x,-halfSize.y, halfSize.z), vec3( 0.0f, 0.0f, 1.0f), ncol },

								{ vec3( halfSize.x, halfSize.y, halfSize.z), vec3( 1.0f, 0.0f, 0.0f), rcol }, // right - purple
								{ vec3( halfSize.x,-halfSize.y,-halfSize.z), vec3( 1.0f, 0.0f, 0.0f), rcol },
								{ vec3( halfSize.x, halfSize.y,-halfSize.z), vec3( 1.0f, 0.0f, 0.0f), rcol },

								{ vec3( halfSize.x,-halfSize.y,-halfSize.z), vec3( 1.0f, 0.0f, 0.0f), rcol },
								{ vec3( halfSize.x, halfSize.y, halfSize.z), vec3( 1.0f, 0.0f, 0.0f), rcol },
								{ vec3( halfSize.x,-halfSize.y, halfSize.z), vec3( 1.0f, 0.0f, 0.0f), rcol },

								{ vec3( halfSize.x, halfSize.y, halfSize.z), vec3( 0.0f, 1.0f, 0.0f), tcol }, // top - yellow
								{ vec3( halfSize.x, halfSize.y,-halfSize.z), vec3( 0.0f, 1.0f, 0.0f), tcol },
								{ vec3(-halfSize.x, halfSize.y,-halfSize.z), vec3( 0.0f, 1.0f, 0.0f), tcol },

								{ vec3( halfSize.x, halfSize.y, halfSize.z), vec3( 0.0f, 1.0f, 0.0f), tcol },
								{ vec3(-halfSize.x, halfSize.y,-halfSize.z), vec3( 0.0f, 1.0f, 0.0f), tcol },
								{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3( 0.0f, 1.0f, 0.0f), tcol }
						};

	mNumOfVertices = sizeof(vertexBuffer) / sizeof(Vertex);

	// Create a vertex array
	glGenVertexArrays(1, &mVertexArrayID);

	// Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
	glGenBuffers(1, &mVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);
}
MeshLoader::~MeshLoader(){
	glDeleteBuffers(1, &mVertexBufferID);
    glDeleteVertexArrays(1, &mVertexArrayID);
}

void MeshLoader::Draw(mat4 worldMat){
	//GetWorldMatrix()
	//*
	// Draw the Vertex Buffer
    // Note this draws a unit Cube
    // The Model View Projection transforms are computed in the Vertex Shader
    glBindVertexArray(mVertexArrayID);

    GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform"); 
    glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &worldMat[0][0]);
    
    // 1st attribute buffer : vertex Positions
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
    glVertexAttribPointer(  0,              // attribute. No particular reason for 0, but must match the layout in the shader.
                            3,              // size
                            GL_FLOAT,       // type
                            GL_FALSE,       // normalized?
                            sizeof(Vertex), // stride
                            (void*)0        // array buffer offset
                        );

    // 2nd attribute buffer : vertex normal
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
    glVertexAttribPointer(  1,
                            3,
                            GL_FLOAT,
                            GL_FALSE,
                            sizeof(Vertex),
                            (void*)sizeof(vec3)    // Normal is Offseted by vec3 (see class Vertex)
                        );


    // 3rd attribute buffer : vertex color
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
    glVertexAttribPointer(  2,
                            3,
                            GL_FLOAT,
                            GL_FALSE,
                            sizeof(Vertex),
                            (void*) (2* sizeof(vec3)) // Color is Offseted by 2 vec3 (see class Vertex)
                        );

    // Draw the triangles !
    glDrawArrays(GL_TRIANGLE_STRIP, 0, mNumOfVertices);

    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
	//*/
}