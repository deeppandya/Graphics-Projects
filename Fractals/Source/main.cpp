/*	COMP 6761 Assignment 2 Framework
	Created by Kaustubha Mendhurwar on 13/10/14.
	Copyright (c) 2014 Concordia University. All rights reserved.
*/

// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW - OpenGL Extension Wrangler
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW - OpenGL Framework (Multi Platform framework to Setup a window and OpenGL Context
#include <GLFW/glfw3.h>

GLFWwindow* window;

// Include GLM - OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "shader.hpp"
#include "ppm_loader.h"
#include "controls.hpp"

extern "C" {
_declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}
int _iteration=10;
float _zoom = 1.0;
int _method=1;
//vec2 movement = vec2(0, 0);
float _randx[15] = { 0.687, 0.6, 0.8, -1, 0.3, 0.25, -0.79, 0.3, 0.28, -0.5,-0.687,-0.162,-1.476,-0.12 ,-0.4};
float _randy[15] = { 0.312, 0.55, 0.6, 0, 0.6, 0, 0.15, -0.1, 0.008, 0.563, 0.312,1.04,0,-0.77,0.6 };
glm::vec2 movement = glm::vec2(0, 0);
glm::vec2 c = glm::vec2(0.2,0.2);
float _random;
int i = 0;
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mod)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			return;
		case GLFW_KEY_MINUS:
			_iteration--;
			return;
		case GLFW_KEY_KP_ADD:
			_iteration++;
			return;
		case GLFW_KEY_Z:
			_zoom-=0.1;
			return;
		case GLFW_KEY_X:
			_zoom+=0.1;
			return;
		case GLFW_KEY_1:
			_method=1;
			return;
		case GLFW_KEY_2:
			_method=2;
			return;
		case GLFW_KEY_KP_4:
			movement.x-=0.1;
			return;
		case GLFW_KEY_KP_6:
			movement.x+=0.1;
			return;
		case GLFW_KEY_KP_8:
			movement.y+=0.1;
			return;
		case GLFW_KEY_KP_2:
			movement.y-=0.1;
			return;
		case GLFW_KEY_C:
			//_random = rand() % 2000;
			
			//c.x = (sin(cos(_random / 10) * 10) + cos(_random * 2.0) / 4.0 + sin(_random * 3.0) / 6.0) * 0.8;
			//c.y = (cos(sin(_random / 10) * 10) + sin(_random * 2.0) / 4.0 + cos(_random * 3.0) / 6.0) * 0.8;
			//c.x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX)+0.05;
			//c.y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX)+0.01;
			c.x =_randx[i];
			c.y = _randy[i];
			i++;
			if (i > 14)
			{
				i = 0;
			}
			return;
		case GLFW_KEY_V:
			//_random = rand() % 2000;

			//c.x = (sin(cos(_random / 10) * 10) + cos(_random * 2.0) / 4.0 + sin(_random * 3.0) / 6.0) * 0.8;
			//c.y = (cos(sin(_random / 10) * 10) + sin(_random * 2.0) / 4.0 + cos(_random * 3.0) / 6.0) * 0.8;
			//c.x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX)+0.05;
			//c.y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX)+0.01;
			c.x = _randx[i];
			c.y = _randy[i];
			i--;
			if (i <0)
			{
				i = 0;
			}
			return;
		default:
			break;
		}
	}
}
int main( void )
{
	GLuint textureID;

	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "COMP 6761 --- Assignment2_Framework", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, KeyCallback);
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "../Source/Shaders/SimpleTransform.vertexshader", "../Source/Shaders/SingleColor.fragmentshader" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(300.0f, 4.0f / 4.0f, 0.1f, 100.0f);
	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
	
	// Camera matrix
	glm::mat4 View       = glm::lookAt(
								glm::vec3(0,0,1), // Camera is at (4,3,3), in World Space
								glm::vec3(0,0,0), // and looks at the origin
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model      = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around

	static const GLfloat g_vertex_buffer_data[] = { 
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
	};

	// Create one OpenGL texture
	textureID = loadTexture("../Source/Textures/gradient1.ppm");

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);



	do{
		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT );

		// Use our shader
		glUseProgram(programID);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		
		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_1D, textureID);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 6); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);
		GLuint iteration = glGetUniformLocation(programID, "iteration");
		glUniform1i(iteration, _iteration);
		GLuint zoom = glGetUniformLocation(programID, "zoom");
		glUniform1f(zoom, _zoom);
		GLuint method = glGetUniformLocation(programID, "method");
		glUniform1i(method, _method);
		GLuint Movement = glGetUniformLocation(programID, "Movement");
		glUniform2f(Movement, movement.x,movement.y);
		GLuint C = glGetUniformLocation(programID, "C");
		glUniform2f(C, c.x, c.y);
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
