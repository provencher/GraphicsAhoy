//Created Jordan Rutty
//Thanks to OpenGl Tutorial for help

#include "MeshLoader.h"
#include "Renderer.h"
#include <GL/glew.h>

#include <iostream>
#include <fstream>

/*/////////////////////////////////////////
	INCOMPLETE
*///////////////////////////////////////////
using namespace glm;



// Work in progress
// Error is passing array to Open GL

MeshLoader::MeshLoader(){
	vec3 color = vec3(0.7f);
	vec3 size = vec3(1);
	vec3 rcol, lcol, tcol, bcol, ncol, fcol;
		 rcol = color;
		 lcol = color;
		 tcol = color;
		 bcol = color;
		 ncol = color;
		 fcol = color;

		//string x = "";

	//LoadMesh("../Scenes/Obj/XA-20_Razorback_Strike_Fighter/XA-20_Razorback_Strike_Fighter.obj", vec3(0.4f));
	
	
	//*
	vec3 halfSize = size * 0.5f;
	
	
	Vertex V[] = {  // position,                normal,                  color
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

	
	mNumOfVertices = sizeof(V) / sizeof(Vertex);

	for(unsigned int i=0; i<(sizeof(V) / sizeof(Vertex)); i++){
		mVertexArray.push_back(V[i]);
	}

	//trying to build a dynamic array of structs and pass over (for the cube)
	//then pass over the XA-20 RazorBack Model
	Vertex *vb = V;
	std::cout << sizeof(vb)<<"########\n";
	// Create a vertex array
	glGenVertexArrays(1, &mVertexArrayID);

	// Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
	glGenBuffers(1, &mVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mVertexArray), &mVertexArray[0], GL_STATIC_DRAW);
	//*/
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


void MeshLoader::LoadMesh(const char * path, vec3 color){
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals; // Won't be used at the moment.
	//bool res = LoadObj("../Scenes/Obj/XA-20_Razorback_Strike_Fighter/XA-20_Razorback_Strike_Fighter.obj", vertices, uvs, normals);
	LoadObj("../Scenes/Obj/asteroid1.obj", vertices, uvs, normals);

	
	
	//Load data into our Vertices's structure 
	
	unsigned int size = vertices.size();
	vertexBuffer = new Vertex[size];
	for( unsigned int i=0; i<size; i++){
		//*
		vertexBuffer[i].position = vertices[i];
		vertexBuffer[i].normal = normals[i];
		vertexBuffer[i].uvs = uvs[i];
		vertexBuffer[i].color = color;
		//*/
	}


	//*
	mNumOfVertices = size;
	std::cout << mNumOfVertices << "####\n";
	// Create a vertex array
	glGenVertexArrays(1, &mVertexArrayID);

	// Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
	glGenBuffers(1, &mVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);
	//*/
}

//this function from opengl tutorial
bool MeshLoader::LoadObj(	const char * path, 	
						std::vector<glm::vec3> & out_vertices, 	
						std::vector<glm::vec2> & out_uvs,	
						std::vector<glm::vec3> & out_normals ){
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices; 
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	FILE * file = fopen("../Scenes/Obj/XA-20_Razorback_Strike_Fighter/XA-20_Razorback_Strike_Fighter.obj", "r");
	if (file == NULL){
		printf("Impossible to open the file !\n");
		return false;
	}

	while(true){
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		if (strcmp(lineHeader, "v") == 0){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);

		} else if (strcmp(lineHeader, "vt") == 0){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);

		} else if (strcmp(lineHeader, "vn") == 0){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);

		} else if (strcmp(lineHeader, "f") == 0){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9){
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	//*

	//Convert Vectors to glm::vec3
	unsigned int size = vertexIndices.size();
	for( unsigned int i=0; i<size; i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex	 = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		glm::vec3 vertex	= temp_vertices[ vertexIndex-1 ];
		glm::vec2 uv		= temp_uvs[ uvIndex-1 ];
		glm::vec3 normal	= temp_normals[ normalIndex-1 ];
		
		//pass
		out_vertices.push_back(vertex);
		out_uvs     .push_back(uv);
		out_normals .push_back(normal);
	}
	//*/
	return true;
}