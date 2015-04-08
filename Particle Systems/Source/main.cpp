



/*	COMP 6761 Assignment 3 Framework
	Created by Kaustubha Mendhurwar on 10/11/14.
	Copyright (c) 2014 Concordia University. All rights reserved.
*/

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <time.h>
#include <math.h>
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
using namespace std;
const int NUM_PARTICLES = 1000;
const int WORKGROUPSIZE = 1;
const double PI = 3.1415926;
float dt = 1;
float iteration = 0.02;
double currentTime;
double lastTime;
int nbFrames=0;
char * p;
int numberParticleShow=100;
int textureMode = 1;
float design=0.0;
GLuint vbo;
GLuint sbo;
bool _checkincres=false;
bool _checkdecres=false;
extern "C" {
_declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

struct ParticlePos{
	float X0pos; float Y0pos; float Z0pos;
	float X1pos; float Y1pos; float Z1pos;
	float X2pos; float Y2pos; float Z2pos;
	float X3pos; float Y3pos; float Z3pos;
	float X4pos; float Y4pos; float Z4pos;
	float X5pos; float Y5pos; float Z5pos;
};
struct ParticleVel{
	float Xv; float Yv; float Zv;
	float TimeToLive;
};
float Ran(float min,float max){
	assert(max>min);
	float random = ((float)rand()) / (float)RAND_MAX;
	float range = max - min;
	return (random + range) + min;
}
// Set up texture sampling offset storage
const GLint tcOffsetColumns = 5;
const GLint tcOffsetRows = 5;
GLfloat texCoordOffsets[tcOffsetColumns * tcOffsetRows * 2];
// Calculate texture coordinate offsets for kernel convolution effects
void genTexCoordOffsets(GLuint width, GLuint height, GLfloat step = 1.0f) // Note: Change this step value to increase the number of pixels we sample across...
{
	// Note: You can multiply the step to displace the samples further. Do this with diff values horiz and vert and you have directional blur of a sort...
	float xInc = step / (GLfloat)(width);
	float yInc = step / (GLfloat)(height);

	for (int i = 0; i < tcOffsetColumns; i++)
	{
		for (int j = 0; j < tcOffsetRows; j++)
		{
			texCoordOffsets[(((i * 5) + j) * 2) + 0] = (-2.0f * xInc) + ((GLfloat)i * xInc);
			texCoordOffsets[(((i * 5) + j) * 2) + 1] = (-2.0f * yInc) + ((GLfloat)j * yInc);
		}
	}
}

void setWindowFPS(GLFWwindow* win)
{
	// Measure speed
	currentTime = glfwGetTime();
	nbFrames++;
	
	char title[10];
	char t[20] = {"  FPS:"};
	char p[10];
	char pa[40] = {"Number of particle:"};
	if (currentTime - lastTime >= 1.0){ // If last cout was more than 1 sec ago
		//nbFrames = (int)(1000 / nbFrames);
		int n = numberParticleShow ;
		itoa(n, p, 10);
		strcat(pa, p);
		itoa(nbFrames, title, 10);
		strcat(t, title);
		strcat(pa, t);
		glfwSetWindowTitle(win, pa);
		
		nbFrames = 0;
		lastTime += 1.0;

	}
}
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mod)
{
	const float moveSpeed = 2.0;
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			return;
		case GLFW_KEY_UP:
			
			_checkincres=true;
			return;
		case GLFW_KEY_DOWN:
			
			_checkdecres=true;
			return;
		case GLFW_KEY_1:
			textureMode = 1;
			return;
		case GLFW_KEY_2:
			textureMode = 2;
			return;
		case GLFW_KEY_3:
			textureMode = 3;
			return;
		case GLFW_KEY_4:
			textureMode = 4;
			return;
			case GLFW_KEY_Z:
			design = 0.1;
			return;
		case GLFW_KEY_X:
			design = 0.2;
			return;
		case GLFW_KEY_C:
			design = 0.3;
			return;
		default:
			break;
		}
	}
}

//void resetPositionSSBO()
//{
//
//	// Reset to mouse cursor pos
//	double cursorX, cursorY;
//	int windowWidth, windowHeight;
//	glfwPollEvents();
//	glfwGetCursorPos(window, &cursorX, &cursorY);
//	glfwGetWindowSize(window, &windowWidth, &windowHeight);
//
//	float destPosX = (float)(cursorX / (windowWidth)-0.5f) * 2.0f;
//	float destPosY = (float)((windowHeight - cursorY) / windowHeight - 0.5f) * 2.0f;
//
//	struct ParticlePos * point = (struct ParticlePos *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES*sizeof(struct ParticlePos), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
//	for (int i = 0; i < NUM_PARTICLES; i++) {
//		float rnd = (float)rand() / (float)(RAND_MAX);
//		float rndVal = (float)rand() / (float)(RAND_MAX / (360.0f * 3.14f * 2.0f));
//		float rndRad = (float)rand() / (float)(RAND_MAX)* 0.2f; // TODO : Change multiplier to get cool effects (e.g. wider range)
//		point[i].X0pos = 0.0f;  point[i].Z0pos =0.0f;// cubeSize / 2.0f + fmod((float)rand(), cubeSize);
//		point[i].X1pos =  0.0f; point[i].Y1pos = 0.0f; point[i].Z1pos = 0.0f;
//		point[i].X2pos = 0.0f; point[i].Z2pos = 0.0f;
//		point[i].X3pos =  0.0f; point[i].Z3pos = 0.0f;
//		point[i].X4pos =  0.0f; point[i].Z4pos = 0.0f;
//		point[i].X5pos =  0.0f; point[i].Z5pos = 0.0f;
//	}
//	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
//}
//
//void resetVelocitySSBO()
//{
//	struct ParticleVel *velocity = (struct ParticleVel *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES*sizeof(struct ParticleVel), GL_MAP_WRITE_BIT|GL_MAP_INVALIDATE_BUFFER_BIT);
//	for (int i = 0; i < NUM_PARTICLES; i++){
//		velocity[i].Xv = 0.0f; 
//		velocity[i].Yv = 0.0f;
//		velocity[i].Zv = 0.0f;
//		velocity[i].TimeToLive = 3.0f;
//	}
//	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
//}
GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
//struct ParticlePos * point1 = (struct ParticlePos *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES*sizeof(struct ParticlePos), bufMask);
//struct ParticleVel *velocity1 = (struct ParticleVel *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES*sizeof(struct ParticleVel), bufMask);
//struct ParticlePos * point2 = (struct ParticlePos *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES*sizeof(struct ParticlePos), bufMask);
//struct ParticleVel *velocity2 = (struct ParticleVel *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES*sizeof(struct ParticleVel), bufMask);
struct ParticlePos * point1;
struct ParticlePos * point2;
struct ParticleVel * velocity1;
struct ParticleVel * velocity2;


void increaseparticles()
{
	numberParticleShow += 1000;
	point1=new ParticlePos[numberParticleShow];
	for (int i = 0; i < numberParticleShow; i++){
		point1[i].X0pos = 0.0f; point1[i].Y0pos = 0.0f; point1[i].Z0pos = 0.0f;// cubeSize / 2.0f + fmod((float)rand(), cubeSize);
		point1[i].X1pos = 0.02f; point1[i].Y1pos = 0.02f; point1[i].Z1pos = 0.0f;
		point1[i].X2pos = 0.0f; point1[i].Y2pos = 0.02f; point1[i].Z2pos = 0.0f;
		point1[i].X3pos = 0.0f; point1[i].Y3pos = 0.0f; point1[i].Z3pos = 0.0f;
		point1[i].X4pos = 0.02f; point1[i].Y4pos = 0.02f; point1[i].Z4pos = 0.0f;
		point1[i].X5pos = 0.02f; point1[i].Y5pos = 0.0f; point1[i].Z5pos = 0.0f;
		
	}
	
	
	//glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	
	velocity1=new ParticleVel[numberParticleShow];
	for (int i = 0; i < numberParticleShow; i++){
		velocity1[i].Xv = float(sin(double(i) * 2 * PI*20 / double(numberParticleShow))); 
		velocity1[i].Yv = 1.0+float((rand() % 600) + 0) / 100;
		velocity1[i].Zv = float(cos(double(i) * 2 * PI / double(numberParticleShow)));
		velocity1[i].TimeToLive = 3.0f;
	}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numberParticleShow*sizeof(struct ParticlePos), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, sbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numberParticleShow*sizeof(struct ParticleVel), NULL, GL_STATIC_DRAW);
	_checkincres=false;
	delete[]point1;
	delete[] velocity1;
	//glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}


void decreaseparticles()
{
	numberParticleShow -= 1000;
	GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
	//struct ParticlePos * point2 = (struct ParticlePos *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES*sizeof(struct ParticlePos), bufMask);
	point2=new ParticlePos[numberParticleShow];
	for (int i = 0; i < numberParticleShow; i++){
		point2[i].X0pos = 0.0f; point2[i].Y0pos = 0.0f; point2[i].Z0pos = 0.0f;// cubeSize / 2.0f + fmod((float)rand(), cubeSize);
		point2[i].X1pos = 0.02f; point2[i].Y1pos = 0.02f; point2[i].Z1pos = 0.0f;
		point2[i].X2pos = 0.0f; point2[i].Y2pos = 0.02f; point2[i].Z2pos = 0.0f;
		point2[i].X3pos = 0.0f; point2[i].Y3pos = 0.0f; point2[i].Z3pos = 0.0f;
		point2[i].X4pos = 0.02f; point2[i].Y4pos = 0.02f; point2[i].Z4pos = 0.0f;
		point2[i].X5pos = 0.02f; point2[i].Y5pos = 0.0f; point2[i].Z5pos = 0.0f;
		
	}
	
	
	//glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	velocity2=new ParticleVel[numberParticleShow];
	//struct ParticleVel *velocity2 = (struct ParticleVel *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES*sizeof(struct ParticleVel), bufMask);
	for (int i = 0; i < numberParticleShow; i++){
		velocity2[i].Xv = float(sin(double(i) * 2 * PI*20 / double(numberParticleShow))); 
		velocity2[i].Yv = 1.0+float((rand() % 600) + 0) / 100;
		velocity2[i].Zv = float(cos(double(i) * 2 * PI / double(numberParticleShow)));
		velocity2[i].TimeToLive = 3.0f;
	}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numberParticleShow*sizeof(struct ParticleVel), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, sbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numberParticleShow*sizeof(struct ParticleVel), NULL, GL_STATIC_DRAW);
	_checkdecres=false;
	delete[]point2;
	delete[] velocity2;
	//glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

}

int main( void )
{
	
	GLuint texture,texture2,texture3,texture4;
	static const float cubeSize = 4.0f;
	srand((unsigned int)(time(NULL)));
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
	window = glfwCreateWindow( 1024, 768, "COMP 6761 --- Assignment3_Framework", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 4.3 compatible.\n" );
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
	// Define the viewport dimensions
	glViewport(0, 0, 1024, 768);

	// Generate a list of 400 quad locations/translation-vectors
	glm::vec2 translations[400];
	int index = 0;
	GLfloat offset = 0.0001f;
	for (GLfloat y = -10.0; y < 10.0; y += 1)
	{
		for (GLfloat x = -10.0; x < 10.0; x += 1)
		{
			glm::vec2 translation;
			translation.x = (GLfloat)x / 10.0f + offset;
			translation.y = (GLfloat)y / 10.0f + offset;
			translations[index++] = translation;
		}
	}
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	
    
	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "../Source/Shaders/SimpleTransform.vertexshader", "../Source/Shaders/SimpleColor.fragmentshader" );

	//Create and compile GLSL program for compute shader
	GLuint computeID = LoadCompute("../Source/Shaders/SimpleCompute.computeshader");
	//instance buffer
	GLuint instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 400, &translations[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	
	// Projection matrix : 45?Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View       = glm::lookAt(
								glm::vec3(4,3,-10), // Camera is at (4,3,3), in World Space
								glm::vec3(0,1,0), // and looks at the origin
								glm::vec3(0,3,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model      = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around
	genTexCoordOffsets(512, 512,1.0);
	GLuint OFFSETS = glGetUniformLocation(programID, "texOffsets");
	//texture = loadBMP_custom("../Source/Textures/fire3.bmp");
	texture = loadTexture("../Source/Textures/gradient1.ppm");
	texture2 = loadTexture("../Source/Textures/gradient2.ppm");
	texture3 = loadTexture("../Source/Textures/gradient3.ppm");
	texture4 = loadTexture("../Source/Textures/gradient4.ppm");
	// Create one OpenGL texture
	GLuint TextureMode = glGetUniformLocation(programID, "textureMode");
	

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");
	GLuint TextureID2 = glGetUniformLocation(programID, "myTextureSampler2");
	GLuint TextureID3 = glGetUniformLocation(programID, "myTextureSampler3");
	GLuint TextureID4 = glGetUniformLocation(programID, "myTextureSampler4");
	static GLfloat uv_buffer_data[NUM_PARTICLES][2];
	for (int i = 0; i < NUM_PARTICLES; i++){
		float j = 0;
		uv_buffer_data[i][0] = 0.0; uv_buffer_data[i][1] = 0.0;// 0.5 + j / (NUM_PARTICLES + 55);
		uv_buffer_data[i + 1][0] = 0.0; uv_buffer_data[i + 1][1] = 1.0;
		uv_buffer_data[i + 2][0]=
		j++;
	}
	
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);//vao
	glBindVertexArray(VertexArrayID);
	
	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer_data), uv_buffer_data, GL_STATIC_DRAW);
	
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numberParticleShow*sizeof(struct ParticlePos), NULL, GL_STATIC_DRAW);
	GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
	struct ParticlePos * point = (struct ParticlePos *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, numberParticleShow*sizeof(struct ParticlePos), bufMask);
	for (int i = 0; i < numberParticleShow; i++){
		point[i].X0pos = 0.0f; point[i].Y0pos = 0.0f; point[i].Z0pos = 0.0f;// cubeSize / 2.0f + fmod((float)rand(), cubeSize);
		point[i].X1pos = 0.02f; point[i].Y1pos = 0.02f; point[i].Z1pos = 0.0f;
		point[i].X2pos = 0.0f; point[i].Y2pos = 0.02f; point[i].Z2pos = 0.0f;
		point[i].X3pos = 0.0f; point[i].Y3pos = 0.0f; point[i].Z3pos = 0.0f;
		point[i].X4pos = 0.02f; point[i].Y4pos = 0.02f; point[i].Z4pos = 0.0f;
		point[i].X5pos = 0.02f; point[i].Y5pos = 0.0f; point[i].Z5pos = 0.0f;
		
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glGenBuffers(1, &sbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, sbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numberParticleShow*sizeof(struct ParticleVel), NULL, GL_STATIC_DRAW);
	struct ParticleVel *velocity = (struct ParticleVel *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, numberParticleShow*sizeof(struct ParticleVel), bufMask);
	for (int i = 0; i < numberParticleShow; i++){
		velocity[i].Xv = float(sin(double(i) * 2 * PI*20 / double(numberParticleShow))); 
		velocity[i].Yv = 1.0+float((rand() % 600) + 0) / 100;
		velocity[i].Zv = float(cos(double(i) * 2 * PI / double(numberParticleShow)));
		velocity[i].TimeToLive = 3.0f;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	
	
	printf("end of initialization\n");
	GLuint vertexbuffer;
	
	//float lastTime = float(glfwGetTime());
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	lastTime = float(glfwGetTime());

	/*double lastTime = glfwGetTime();
	int nbFrames = 0;*/

	do{
		//double currentTime = glfwGetTime();
		// nbFrames++;
		// if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
		//	 // printf and reset timer
		//	 printf("%f ms/frame\n", 1000.0/double(nbFrames));
		//	 string Result;
		//		stringstream convert;
		//		convert << nbFrames;
		//		Result = convert.str();
		//	 glfwSetWindowTitle(window,Result.c_str());
		//	 nbFrames = 0;
		//	 lastTime += 1.0;
		// }
		setWindowFPS(window);//set the window title
		if(_checkincres)
		{
		increaseparticles();
		}
		else if(_checkdecres)
		{
		decreaseparticles();
		}
		GLuint FloatID1 = glGetUniformLocation(computeID, "dt");
		glUniform1f(FloatID1, dt);
		GLuint FloatID2 = glGetUniformLocation(computeID, "iteration");
		glUniform1f(FloatID2, iteration);
		GLuint IntID1 = glGetUniformLocation(computeID, "g_NumParticles");
		glUniform1i(IntID1, numberParticleShow);
		GLfloat _Design = glGetUniformLocation(programID, "design");
		glUniform1f(_Design, design);
		
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
		
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vbo);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, sbo);
		//compute shader start
		glUseProgram(computeID);//commpute shader
		// Get rolling!
		glDispatchCompute(numberParticleShow/2, 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);//
		// Get data back!
		
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo);
		struct ParticlePos * p = (struct ParticlePos *)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, sbo);
		struct ParticleVel *ptr2 = (struct ParticleVel *)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		//refresh vbo
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, numberParticleShow*sizeof(struct ParticlePos), p, GL_STATIC_DRAW);
		
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, sbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, numberParticleShow*sizeof(struct ParticleVel), ptr2, GL_STATIC_DRAW);
			
		//vertex and fragment shader start
		glUseProgram(programID);
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, numberParticleShow*sizeof(struct ParticlePos), p, GL_STATIC_DRAW);
		
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniform2fv(OFFSETS, 25, texCoordOffsets);
		glUniform1i(TextureMode, textureMode);
		

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(TextureID, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(TextureID2, 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture3);
		glUniform1i(TextureID3, 2);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glUniform1i(TextureID4, 3);
		
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		//glBufferData(GL_ARRAY_BUFFER, NUM_PARTICLES*sizeof(struct ParticlePos), p, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			3*sizeof(GLfloat),                  // stride
			(void*)0            // array buffer offset
		);
		
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);
		// Also set instance data
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glVertexAttribDivisor(2, 1); // Tell OpenGL this is an instanced vertex attribute.
		glBindVertexArray(0);
		// Draw the particles !
		
		glBindVertexArray(VertexArrayID);
		//glDrawArraysInstanced(GL_TRIANGLES, 0,6*400*NUM_PARTICLES, numberParticleShow); // 3 indices starting at 0 -> 1 triangle
		glDrawArrays(GL_TRIANGLES,0,6*numberParticleShow);
		//glBindVertexArray(0);  must be delete!!!!!!!
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
		//resetPositionSSBO();
		//resetVelocitySSBO();
		setWindowFPS(window);
	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteProgram(programID);
	glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(computeID);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &sbo);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
