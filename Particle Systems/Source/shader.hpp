/*	COMP 6761 Assignment 3 Framework
	Created by Kaustubha Mendhurwar on 10/11/14.
	Copyright (c) 2014 Concordia University. All rights reserved.
*/

#ifndef SHADER_HPP
#define SHADER_HPP

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
GLuint LoadCompute(const char * vertex_file_path);

#endif
