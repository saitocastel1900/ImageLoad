#pragma once
//
//Å@include
//
#include <iostream>
#include <GL/glut.h>
using namespace std;

//////////////////////////////////////////////////////////////////////////
//Å@Å@RAWImage class
//////////////////////////////////////////////////////////////////////////
class RAWImage
{
protected:
    GLuint imageSize;
    GLubyte* imageData;
    GLenum format;
    GLuint internalFormat;
    GLuint width;
    GLuint height;
    GLuint bpp;
public:
    GLuint ID;
    RAWImage();
    ~RAWImage();
    bool ReadRAW(const char* filename, const GLuint width, const GLuint height, bool alphaFlag = false);
    bool Load(const char* filename, const GLuint width, const GLuint height, bool alphaFlag = false);
};
