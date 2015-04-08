/*	COMP 6761 Assignment 1 Framework
	Created by Kaustubha Mendhurwar on 09/09/14.
	Copyright (c) 2014 Concordia University. All rights reserved.
*/

#include "Renderer.h"
#include "EventManager.h"

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "ArcBall.h"
using namespace std;

// Time
double EventManager::sLastFrameTime = glfwGetTime();
float  EventManager::sFrameTime = 0.0f;

// Mouse
double EventManager::sLastMousePositionX = 0.0f;
float  EventManager::sMouseDeltaX = 0.0f;
double EventManager::sLastMousePositionY = 0.0f;
float  EventManager::sMouseDeltaY = 0.0f;
extern float tesslevelinner;
extern float tesslevelouter;
// Window
GLFWwindow* EventManager::spWindow = NULL;
#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT 768

//Initial arc ball rotation
glm::mat4 EventManager::viewRotationMatrix;

static ArcBall arcball( WINDOW_WIDTH, WINDOW_HEIGHT, 1.5f, true, true );
void ErrorCallback( int error, const char * desc );
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mod);
void FrameBufferSizeCallback( GLFWwindow *window, int width, int height );
void ScrollCallback( GLFWwindow *window, double x, double y );
void MouseButtonCallback( GLFWwindow * window, int button, int action, int mods );
void CursorCallback( GLFWwindow *window, double x, double y );

void EventManager::Initialize()
{
	// Initialise GLFW
	if(!glfwInit())
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		exit(-1);
	}
		
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	glfwWindowHint(GLFW_RESIZABLE, 0);
	spWindow = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "COMP 6761 - Assignment 1 Framework", NULL, NULL);

	if( spWindow == NULL )
	{
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 4.3 compatible.\n" );
		glfwTerminate();
		exit(-1);
	}

	/*Set Callbacks for GLFW*/
	glfwSetErrorCallback( ErrorCallback );
    glfwSetScrollCallback( spWindow, ScrollCallback );
    glfwSetKeyCallback( spWindow, KeyCallback );
    glfwSetFramebufferSizeCallback( spWindow, FrameBufferSizeCallback );
    glfwSetCursorPosCallback( spWindow, CursorCallback );
    glfwSetMouseButtonCallback( spWindow, MouseButtonCallback );

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(spWindow, GLFW_STICKY_KEYS, GL_TRUE);

	// Initial mouse position
	glfwPollEvents();
	double x, y;
	glfwGetCursorPos(spWindow, &x, &y);

	sLastMousePositionX = x;
	sLastMousePositionY = y;

	// Initial time
	sLastFrameTime = glfwGetTime();
}

void EventManager::Shutdown()
{
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	spWindow = NULL;
}

void EventManager::Update()
{
	// Update inputs / events
	glfwPollEvents();

	// Update mouse position
	double x, y;
	glfwGetCursorPos(spWindow, &x, &y);
	sMouseDeltaX = static_cast<float>( x - sLastMousePositionX );
	sMouseDeltaY = static_cast<float>( y - sLastMousePositionY );
	sLastMousePositionX = x;
	sLastMousePositionY = y;

	// Update frame time
	double currentTime = glfwGetTime();
	sFrameTime = static_cast<float>(currentTime - sLastFrameTime);
	sLastFrameTime = currentTime;

	//Update Arc Ball
	viewRotationMatrix = arcball.createViewRotationMatrix();
}

float EventManager::GetFrameTime()
{
	return sFrameTime;
}

bool EventManager::ExitRequested()
{
	return glfwGetKey(spWindow, GLFW_KEY_ESCAPE ) == GLFW_PRESS || glfwWindowShouldClose(spWindow);
}

GLFWwindow* EventManager::GetWindow()
{
	return spWindow;
}

float EventManager::GetMouseMotionX()
{
	return sMouseDeltaX;
}

float EventManager::GetMouseMotionY()
{
	return sMouseDeltaY;
}

void EventManager::EnableMouseCursor()
{
	glfwSetInputMode(spWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void EventManager::DisableMouseCursor()
{
	glfwSetInputMode(spWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void ErrorCallback( int error, const char * desc )
{
    fputs( desc, stderr );
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mod)
{
    if( action == GLFW_PRESS )
	{
        switch ( key)
		{
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose( window, GL_TRUE );
                return;
			/*case GLFW_KEY_MINUS:
                tesslevelinner--;
                return;
			case GLFW_KEY_KP_ADD:
				tesslevelinner++;
                return;*/
			case GLFW_KEY_KP_4:
               tesslevelouter--;
			   if(tesslevelouter>0)
			   {
			   cout << "Tessselation Outer Level : "<<tesslevelouter<<endl;
			   }
                return;
			case GLFW_KEY_KP_6:

				tesslevelouter++;
				
				cout << "Tessselation Outer Level : "<<tesslevelouter<<endl;
				
                return;
			case GLFW_KEY_KP_8:
               
			   tesslevelinner++;
			   
			   cout << "Tessselation Inner Level : "<<tesslevelinner<<endl;
			   
                return;
			case GLFW_KEY_KP_2:
				
				tesslevelinner--;
				if(tesslevelinner>0)
			   {
				cout << "Tessselation Inner Level : "<<tesslevelinner<<endl;
				}
                return;
            default:
                break;
        }
    }
}

void FrameBufferSizeCallback( GLFWwindow *window, int width, int height )
{
    glViewport( 0, 0, width, height );
}

void ScrollCallback( GLFWwindow *window, double x, double y ) 
{
}

void MouseButtonCallback( GLFWwindow * window, int button, int action, int mods )
{
    /* Pass the arguments to our arcball object */
    arcball.mouseButtonCallback( window, button, action, mods );
}

void CursorCallback( GLFWwindow *window, double x, double y ) 
{
    /* Pass the arguments to our arcball object */
    arcball.cursorCallback( window, x, y );
}

glm::mat4 EventManager::GetArcBallRotation()
{
	return viewRotationMatrix;
}