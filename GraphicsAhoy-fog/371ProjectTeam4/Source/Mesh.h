#include <iostream>
#include <vector>
#include <GL/glew.h>

#include <GLM/glm.hpp>


class Mesh{
	
/*/////////////////////////////////////////
	INCOMPLETE
*///////////////////////////////////////////
public:
	void draw(unsigned int programId);

private:
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 uvs;
	};
	std::vector<Vertex> vertexBuffer;
	
	std::vector<unsigned int> indexes;
	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;
	unsigned int mNumOfVertices;
	

};