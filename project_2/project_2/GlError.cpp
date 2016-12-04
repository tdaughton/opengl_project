//
//  GlError.cpp
//  project_1
//
//  Created by ltess on 11/13/16.
//  Copyright (c) 2016 Tess Daughton. All rights reserved.
//

#include "GlError.h"
#include <stdlib.h>
#include <iostream>


#ifdef WIN32
#  include <GL/glew.h>
#elif __APPLE__
#  include <OpenGL/gl3.h>
#else
#  include <GL3/gl3.h>
#endif

using namespace std;

void _check_gl_error(const char *file, int line) {
    GLenum err (glGetError());
    
    while(err!=GL_NO_ERROR) {
        string error;
        
        switch(err) {
            case GL_INVALID_OPERATION:      error="INVALID_OPERATION";      break;
            case GL_INVALID_ENUM:           error="INVALID_ENUM";           break;
            case GL_INVALID_VALUE:          error="INVALID_VALUE";          break;
            case GL_OUT_OF_MEMORY:          error="OUT_OF_MEMORY";          break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
        }
        
        cerr << "GL_" << error.c_str() <<" - "<<file<<":"<<line<<endl;
        err=glGetError();
    }
}