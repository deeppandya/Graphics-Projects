/*	COMP 6761 Assignment 1 Framework
	Created by Kaustubha Mendhurwar on 09/09/14.
	Copyright (c) 2014 Concordia University. All rights reserved.
*/

#include <vector>
#include <GLM/glm.hpp>

bool   LoadOBJ(const char * path, std::vector<glm::vec3> & out_vertices, std::vector<glm::vec2> & out_uvs, std::vector<glm::vec3> & out_normals );