

#include <GLM/glm.hpp>
#include <map>
#include <vector>
#include <string>

class MeshLoader
{
public:
	MeshLoader();
	~MeshLoader();
	void Draw(glm::mat4 worldMat);
private:
	void LoadMesh(const char * path, glm::vec3 color=glm::vec3(0.4f));
	bool LoadObj(	const char * path, 	
				std::vector<glm::vec3> & out_vertices, 	
				std::vector<glm::vec2> & out_uvs,	
				std::vector<glm::vec3> & out_normals );

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 uvs;
	};
	
	std::vector<Vertex>* mVertexArray;
	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;
	unsigned int mNumOfVertices;
};
