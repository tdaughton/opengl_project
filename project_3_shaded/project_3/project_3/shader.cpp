//
//  shader.cpp
//  project_1
//
//  Created by Tess Daughton on 11/9/16.
//  Copyright © 2016 Tess Daughton. All rights reserved.
//

#pragma debug

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include "shader.hpp"

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){
    
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream;
    
    VertexShaderStream.exceptions(std::ifstream::badbit);
    try {
        VertexShaderStream.open(vertex_file_path);
        std::stringstream vertStream;
        vertStream << VertexShaderStream.rdbuf();
        VertexShaderStream.close();
        VertexShaderCode = vertStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    
    
    // Read the Fragment Shader code from the file
    std::string FragShaderCode;
    std::ifstream FragShaderStream;
    
    FragShaderStream.exceptions(std::ifstream::badbit);
    try {
        FragShaderStream.open(fragment_file_path);
        std:: stringstream fragStream;
        fragStream << FragShaderStream.rdbuf();
        FragShaderStream.close();
        FragShaderCode = fragStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    
//    GLint Vertex_Compile_Status= GL_FALSE;
//    int InfoLogLength=0;
    
    // Compile Vertex Shader
    //    printf("Compiling shader : %s\n", vertex_file_path);
    const GLchar * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);
    
    // Check Vertex Shader
    //    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Vertex_Compile_Status);
    //    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    //    if(Vertex_Compile_Status==GL_FALSE) printf("VERTEX SHADER NOT COMPILED");
    //
    //    if ( InfoLogLength>0){
    //        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
    //        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    //        printf("%s\n", &VertexShaderErrorMessage[0]);
    //    }
    
    
    
    // Compile Fragment Shader
    //    printf("Compiling shader : %s\n", fragment_file_path);
    const GLchar * FragmentSourcePointer = FragShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);
    //
    //    GLint compile_status=GL_FALSE;
    //    InfoLogLength=0;
    
    // Check Fragment Shader
    //    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &compile_status);
    //    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    //    if(compile_status==GL_FALSE) printf("FRAGMENT SHADER NOT COMPILED");
    //
    //    if ( InfoLogLength>0 ){
    //        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
    //        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    //        printf("%s\n", &FragmentShaderErrorMessage[0]);
    //}
    
    // Link the program
    //    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);
    
    glBindAttribLocation(ProgramID,0,"vertexPosition");
    glBindAttribLocation(ProgramID,1,"inColor");
    
    
    //    GLint validate_link_status;
    //    // Check the program
    //    glGetProgramiv(ProgramID, GL_LINK_STATUS, &validate_link_status);
    //    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    //    if(validate_link_status==GL_FALSE) printf("ITS NOT LINKED");
    //        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
    //        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    //        printf("%s\n", &ProgramErrorMessage[0]);
    
    
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    
    return ProgramID;
}
