/*	COMP 6761 Assignment 3 Framework
	Created by Kaustubha Mendhurwar on 10/11/14.
	Copyright (c) 2014 Concordia University. All rights reserved.
*/

#ifndef PPM_LOADER_H
#define PPM_LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define ILUT_USE_OPENGL		// This MUST be defined before calling the DevIL headers or we don't get OpenGL functionality

#include<IL/il.h>
#include<IL/ilu.h>
#include<IL/ilut.h>

//#pragma comment(lib, "DevIL.lib")
//#pragma comment(lib, "ILU.lib")
//#pragma comment(lib, "ILUT.lib")

unsigned int loadTexture(const char* filename);
void initDevIL();

#endif