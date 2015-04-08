/*	COMP 6761 Assignment 2 Framework
	Created by Kaustubha Mendhurwar on 13/10/14.
	Copyright (c) 2014 Concordia University. All rights reserved.
*/

#include "ppm_loader.h"
#include<sstream>
#include<iostream>

void initDevIL()
{
	ilutRenderer(ILUT_OPENGL);
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

unsigned int loadTexture(const char* filename)
{
	initDevIL();
	ILuint imageID;				// Create an image ID as a ULuint 
	GLuint textureID;			// Create a texture ID as a GLuint 
	ILboolean success;			// Create a flag to keep track of success/failure 
	ILenum error;				// Create a flag to keep track of the IL error state 
	ilGenImages(1, &imageID); 		// Generate the image ID 
	ilBindImage(imageID); 			// Bind the image
 
	std::stringstream sstm;
	sstm << filename;
	success = ilLoadImage((const ILstring) &sstm.str()[0]); 	// Load the image file
 
	// If we managed to load the image, then we can start to do things with it...
	if (success)
	{
		// Convert the image into a suitable format to work with
		// NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
		
		// Quit out if we failed the conversion
		if (!success)
		{
			error = ilGetError();
			std::cout << "Image conversion failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;
			exit(-1);
		}

		// Generate a new texture
		glGenTextures(1, &textureID);
 
		// Bind the texture to a name
		glBindTexture(GL_TEXTURE_1D, textureID);
 
		// Set texture clamping method
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP);
 
		// Set texture interpolation method to use linear interpolation (no MIPMAPS)
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 
		// Specify the texture specification
		glTexImage1D(GL_TEXTURE_1D, 				// Type of texture
					 0,				// Pyramid level (for mip-mapping) - 0 is the top level
					 ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
					 ilGetInteger(IL_IMAGE_WIDTH),	// Image width
					 0,				// Border width in pixels (can either be 1 or 0)
					 ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
					 GL_UNSIGNED_BYTE,		// Image data type
					 ilGetData());			// The actual image data itself
 	}
  	else // If we failed to open the image file in the first place...
  	{
		error = ilGetError();
		std::cout << "Image load failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;
		exit(-1);
  	}
 
 	ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.
 
	std::cout << "Texture creation successful." << std::endl;
 
	return textureID; // Return the GLuint to the texture so you can use it!
}
