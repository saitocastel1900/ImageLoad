#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include<iostream>
#include<fstream>
#include <GL/glew.h>
#include<vector>
#include<memory>
#include <GLFW/glfw3.h>
#include <fstream>
#include "RAWLoader.h"
using namespace std;


//////////////////////////////////////////////////////////////////////////
//�@�@RAWImage class
//////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------
//�@�@RAWImage
//�@�@Desc : �R���X�g���N�^
//----------------------------------------------------------------------------------------------------
RAWImage::RAWImage()
{
    imageSize = 0;
    imageData = NULL;
    format = GL_RGB;
    internalFormat = GL_RGB;
    width = 0;
    height = 0;
    bpp = 0;
    ID = 0;
}

//-----------------------------------------------------------------------------------------------------
//�@�@~RAWImage
//�@�@Desc : �f�X�g���N�^
//-----------------------------------------------------------------------------------------------------
RAWImage::~RAWImage()
{
    if (imageData)
    {
        delete[] imageData;
        imageData = NULL;
    }
}

//-----------------------------------------------------------------------------------------------------
//�@�@ReadRAW
//�@�@Desc : �t�@�C���ǂݍ���
//-----------------------------------------------------------------------------------------------------
bool RAWImage::ReadRAW(const char* filename, const GLuint width, const GLuint height, bool alphaFlag)
{
    ifstream file;

    file.open(filename, ios::in | ios::binary);
    if (!file.is_open())
    {
        cout << "Error : �w�肳�ꂽ�t�@�C�����J���܂���\n";
        cout << "File Name : " << filename << endl;
        return false;
    }

    if (alphaFlag)
    {
        imageSize = width * height * 4;
        format = GL_RGBA;
        internalFormat = GL_RGBA;
    }
    else
    {
        imageSize = width * height * 3;
        format = GL_RGB;
        internalFormat = GL_RGB;
    }

    imageData = new GLubyte[imageSize];

    file.read((char*)imageData, imageSize);

    file.close();

    return true;
}

//-----------------------------------------------------------------------------------------------------
//�@�@Load
//�@�@Desc : �t�@�C����ǂݍ��݃e�N�X�`�����쐬
//-----------------------------------------------------------------------------------------------------
bool RAWImage::Load(const char* filename, const GLuint width, const GLuint height, bool alphaFlag)
{
    if (!ReadRAW(filename, width, height, alphaFlag))
        return false;

    //�@�e�N�X�`���𐶐�
    glGenTextures(1, &ID);

    //�@�e�N�X�`�����o�C���h����
    glBindTexture(GL_TEXTURE_2D, ID);

    if (alphaFlag) glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    else glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    //�@�e�N�X�`���̊��蓖��
    gluBuild2DMipmaps(
        GL_TEXTURE_2D,
        internalFormat,
        width,
        height,
        format,
        GL_UNSIGNED_BYTE,
        imageData);

    //�@�e�N�X�`�����g��E�k��������@�̎w��
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    //�@�e�N�X�`����
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    //
    glBindTexture(GL_TEXTURE_2D, 0);

    //�@����Ȃ��Ȃ�����������j��
    delete[] imageData;
    imageData = NULL;

    return true;
}
//
//�@global
//
int WindowPositionX = 100;
int WindowPositionY = 100;
int WindowWidth = 512;
int WindowHeight = 512;
char WindowTitle[] = "Texture Mapping - Raw File -";
RAWImage texture;

//
//�@prototype
//
void Initialize();
void Display();
void Idle();



//----------------------------------------s------------------------------------------------------------
//�@�@Initialize
//�@�@Desc : ����������
//----------------------------------------------------------------------------------------------------
void Initialize()
{
    glClearColor(1.0, 0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    texture.Load("KAIT.raw", 132, 132);

}

//---------------------------------------------------------------------------------------------------
//�@�@Idle
//�@�@Desc : �A�C�h�����O���̏���
//---------------------------------------------------------------------------------------------------
void Idle()
{
    glutPostRedisplay();
}

//---------------------------------------------------------------------------------------------------
//�@�@Reshape
//�@�@Desc : �T�C�Y�ύX
//---------------------------------------------------------------------------------------------------
void Reshape(int x, int y)
{
    WindowWidth = x;
    WindowHeight = y;
    if (WindowWidth < 1) WindowWidth = 1;
    if (WindowHeight < 1) WindowHeight = 1;
}

//---------------------------------------------------------------------------------------------------
//�@�@Display
//�@�@Desc : �E�B���h�E�ւ̕`��
//---------------------------------------------------------------------------------------------------
void Display()
{
    double size = 0.5;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //�@�e�N�X�`���}�b�s���O�L����
    glEnable(GL_TEXTURE_2D);
    //�@�e�N�X�`�����o�C���h
    glBindTexture(GL_TEXTURE_2D, texture.ID);
    //�@�F�̎w��
    glColor4f(1.0, 1.0, 1.0, 1.0);
    //�@�l�p�`���e�N�X�`�����W���ŕ`��
    glBegin(GL_QUADS);
    glTexCoord2d(-1.0, 1.0);    glVertex3d(-size, -size, 0.0);
    glTexCoord2d(-1.0, 0.0);    glVertex3d(-size, size, 0.0);
    glTexCoord2d(0.0, 0.0); glVertex3d(size, size, 0.0);
    glTexCoord2d(0.0, 1.0); glVertex3d(size, -size, 0.0);
    glEnd();
    //�@
    glBindTexture(GL_TEXTURE_2D, 0);
    //�@�e�N�X�`���}�b�s���O������
    glDisable(GL_TEXTURE_2D);

    Idle();
    glutSwapBuffers();
}
//----------------------------------------------------------------------------------------------------
//�@�@main
//�@�@Desc : ���C���G���g���[�|�C���g
//----------------------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowPosition(WindowPositionX, WindowPositionY);
    glutInitWindowSize(WindowWidth, WindowHeight);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow(WindowTitle);
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutIdleFunc(Idle);
   

    Initialize();

    glutMainLoop();
    return 0;
}

