////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//	4rce of Nature
//		Scene Render Framework
//
//	Contributors:
//		Eric Provencher
//		Rita Phom
//		Nicolas Bergeron
//		Gary Chang
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

struct GLFWwindow;

// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>

#include <vector>
#include <GLM/glm.hpp>


enum ShaderType
{
	SHADER_SOLID_COLOR,
	SHADER_PATH_LINES,
	SHADER_BLUE,
	SHADER_SHADOW,
	SHADER_TEXTURE,
	SHADER_FOG,
	NUM_SHADERS
};


class Renderer
{
public:
	static void Initialize();
	static void Shutdown();

	static void BeginFrame();
	static void BeginFrameFog(); // when toggling the fog, enable this to make the background gray
	static void EndFrame();
	static void BindFrame();

	static GLuint LoadShaders(std::string vertex_shader_path, std::string fragment_shader_path);
	static bool   LoadOBJ(const char * path, std::vector<glm::vec3> & out_vertices, std::vector<glm::vec2> & out_uvs, std::vector<glm::vec3> & out_normals );

	static unsigned int GetShaderProgramID() { return sShaderProgramID[sCurrentShader]; }
	static unsigned int GetCurrentShader() { return sCurrentShader; }
	static void SetShader(ShaderType type);

private:
	static GLFWwindow* spWindow;

	static std::vector<unsigned int> sShaderProgramID;
	static unsigned int sCurrentShader;

};

