/*	COMP 6761 Assignment 1 Framework
	Created by Kaustubha Mendhurwar on 09/09/14.
	Copyright (c) 2014 Concordia University. All rights reserved.
*/
#pragma once
struct GLFWwindow;

// Include GLEW - OpenGL Extension Wrangler
#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>
#include <GLM/glm.hpp>

enum ShaderType
{
	SHADER_SOLID_COLOR,
	SHADER_BLUE,
	NUM_SHADERS
};

class Renderer
{
public:
	static void Initialize();
	static void Shutdown();

	static void BeginFrame();
	static void EndFrame();

	static GLuint LoadShaders(const char * vertex_file_path, const char * tessCtrl_file_path, const char * tessEval_file_path, const char * geometry_file_path, const char * fragment_file_path);
	static bool   LoadOBJ(const char * path, std::vector<glm::vec3> & out_vertices, std::vector<glm::vec2> & out_uvs, std::vector<glm::vec3> & out_normals );

	static unsigned int GetShaderProgramID() { return sShaderProgramID; }	

private:
	static GLFWwindow* spWindow;
	static GLuint sShaderProgramID;	
};

