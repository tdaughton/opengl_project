//
//  main.cpp
//  project1
//
//  Created by Tess Daughton on 9/11/16.
//  Copyright © 2016 Tess Daughton. All rights reserved.
//
#pragma debug
#define GLFW_DLL

#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "read.cpp"
#include "shader.hpp"
#include "vect.hpp"
#include "matrix.hpp"
#include "linearalgebra.hpp"

#define CLIP_VALUE 45.0
using namespace std;

//default camera vectors for re-centering
glm::vec3 FOCUS_POINT = glm::vec3(0.0,0.0,0.0f);
glm::vec3 DEFAULT_CAMERA_POSITION = glm::vec3(-4,0,0);
glm::vec3 DEFAULT_CAMERA_FRONT = FOCUS_POINT-DEFAULT_CAMERA_POSITION;
glm::vec3 DEFAULT_CAMERA_UP = glm::vec3(0.0,1.0,0.0f);

//camera vectors for position
glm::vec3 camera_position = DEFAULT_CAMERA_POSITION;
glm::vec3 camera_front = DEFAULT_CAMERA_FRONT;
glm::vec3 camera_up = DEFAULT_CAMERA_UP;
glm::vec3 camera_right = glm::normalize(glm::cross(camera_up,camera_front));

//obj file reader object
Reader reader;

//matrices
glm::mat4 translation = glm::mat4(1.0f);
glm::mat4 scaling = glm::mat4(1.0f);
glm::mat4 rotation = glm::mat4(1.0f);
glm::mat4 modelview = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);
glm::mat4 MVP=glm::mat4(1.0f);


//globals for object properties (color, clipping, mode, etc)
GLfloat current_color[3] = {0.0,0.0,1.0};
GLfloat max_dist_to_origin = 0.0;
bool cw = true;
float near_clip = 0.01f;
float far_clip = 10.0f;
float aspect = 0.0f;
float camera_speed = 1.0f;
int window_width, window_height;
char mode='f';

GLFWwindow* shaded_window, *norm_window;

//shader and program ids
GLuint pid=0;
//buffer data handles
GLuint mvp_id=0; GLuint vp_h=0; GLuint color_h=0;

//vertex buffer object
GLuint VBO, EBO, VAO;

vector<GLfloat> coord_all;

GLfloat* mvp_arr=new GLfloat[16];
GLfloat* coord_all_arr ; //=new GLfloat[16];
GLfloat* projection_arr=new GLfloat[16];
GLfloat* model_view_arr=new GLfloat[16];

void draw_faces_shaded()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(pid);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(vp_h);
    glEnableVertexAttribArray(color_h);
    
    glVertexAttribPointer(vp_h, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glVertexAttribPointer(color_h, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, coord_all.size()/3);
    glDisableVertexAttribArray(vp_h);
    glDisableVertexAttribArray(color_h);
}

void draw_faces()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3fv(current_color);
    glVertexPointer(3, GL_FLOAT, 0, coord_all_arr );
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays( GL_TRIANGLES, 0, coord_all.size()/3);
    
    glfwSwapBuffers(norm_window);
    
}

void calc_perspective(float near, float far, float aspect, float fov)
{
    float frustrumDepth = far-near;
    float inv_depth = 1/frustrumDepth;
    
    projection[1][1]= 1/tan(glm::radians(0.5f*fov));
    projection[0][0]= projection[1][1]/aspect;
    projection[2][2]= far*inv_depth;
    projection[3][2]= (-far * near) * inv_depth;
    projection[2][3]= 1;
    projection[3][3]= 0;
}

void mat_to_arr(glm::mat4 mat, float* arr)
{
    int i=0;
    
    for(int j=0; j<4; j++)
        for(int k=0; k<4; k++)
        {
            arr[i]=mat[j][k];
            i++;
        }
}

void arr_to_mat(float* arr, glm::mat4 mat)
{
    int i=0;
    
    for(int j=0; j<4; j++)
        for(int k=0; k<4; k++)
        {
            mat[j][k]=arr[i];
            i++;
        }
}

void rotate(char axis, float inc)
{
    
    float rotx[16] = {1,0.0,0.0,0.0,0.0,cos(inc),-sin(inc),0,0,sin(inc),cos(inc),0,0,0,0,1};
    float roty[16] = {cos(inc),0,sin(inc),0,0,1,0,0,-sin(inc),0,cos(inc),0,0,0,0,1};
    float rotz[16] = {cos(inc), -sin(inc), 0.0, 0.0,sin(inc), cos(inc),0.0,0.0,0.0,0.0,1.0,0.0,
        0.0,      0.0,  0.0, 1.0};
    
    switch(axis){
        case 'x':
            
            rotation = glm::make_mat4(rotx);
            break;
        case 'y':
            rotation = glm::make_mat4(roty);
            break;
        case 'z':
            
            rotation = glm::make_mat4(rotz);
            break;
    }
    modelview = rotation * modelview;
}

void translate(float x, float y, float z)
{
    translation[3][0] = x;
    translation[3][1] = y;
    translation[3][2] = z;
    
    modelview = translation * modelview;
}

void scale(float zoom)
{
    scaling=glm::mat4();
    scaling[2][2] *= zoom;
    
    modelview = scaling * modelview;
}

void set_ModelViewProjection()
{
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    switch(mode){
        case 'l':
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            break;
        case 'f':
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            break;
        case 'p':
            glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );
            break;
    }
    
    if (cw){
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);
    }
    else{
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
    }
    
    calc_perspective(near_clip, far_clip, aspect, CLIP_VALUE);
    mat_to_arr(projection, projection_arr);
    glLoadMatrixf(projection_arr);
    glMatrixMode(GL_PROJECTION);
    glGetFloatv(GL_PROJECTION_MATRIX, projection_arr);
    glMatrixMode(GL_MODELVIEW);
    mat_to_arr(modelview, model_view_arr);
    glLoadMatrixf(model_view_arr);
    glMatrixMode(GL_MODELVIEW);
    glGetFloatv(GL_MODELVIEW_MATRIX, model_view_arr);
    
    MVP=projection*modelview;
    mat_to_arr(MVP, mvp_arr);
    glUniformMatrix4fv(mvp_id,1,GL_FALSE,mvp_arr);
    glUniform3d(color_h, current_color[0], current_color[1],
                current_color[2]);
    
}


void load_shaders()
{
    pid = LoadShaders("/Users/tessdaughton/Desktop/cs412/project_1/vertexShader.txt",
                      "/Users/tessdaughton/Desktop/cs412/project_1/fragmentShader.txt");
    glUseProgram(pid);
    
    mvp_id = glGetUniformLocation(pid,"ModelViewProjection");
    color_h = glGetAttribLocation(pid, "inColor");
    vp_h = glGetAttribLocation(pid,"vertexPosition");
    
    glVertexAttrib3f(color_h, current_color[0], current_color[1],
                     current_color[2]);
    glUniformMatrix4fv(mvp_id, 1, GL_FALSE, mvp_arr);
}

void create_buffers()
{
    
    VBO=0;
    
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, coord_all.size(),
                 coord_all_arr, GL_STATIC_DRAW);
    
}


void display()
{
    // Tess's code
    glfwMakeContextCurrent(shaded_window);
    draw_faces_shaded();
    set_ModelViewProjection();
    
    // Good code
    glfwMakeContextCurrent(norm_window);
    draw_faces();
    set_ModelViewProjection();
}

void reset_camera()
{
    camera_position = reader.get_camera_position();
    far_clip = camera_position.x;
    camera_front=DEFAULT_CAMERA_FRONT;
    camera_up=DEFAULT_CAMERA_UP;
    glMatrixMode(GL_PROJECTION);
    modelview = glm::mat4();
    calc_perspective(near_clip,far_clip,aspect,CLIP_VALUE);
    
    translate(0,0,4);
}




void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key==GLFW_KEY_LEFT && action==GLFW_PRESS)
        translate(1,0,0);
    else if (key==GLFW_KEY_RIGHT && action==GLFW_PRESS)
        translate(-1,0,0);
    else if (key==GLFW_KEY_UP && action==GLFW_PRESS)
        translate(0,0,-1);
    else if (key==GLFW_KEY_DOWN && action==GLFW_PRESS)
        translate(0,0,1);
    else if (key==GLFW_KEY_ESCAPE && action==GLFW_PRESS)
    {
        glfwDestroyWindow(shaded_window);
        glfwDestroyWindow(norm_window);
        
        glfwTerminate();
        exit(0);
    }
    else if (key==GLFW_KEY_W && action==GLFW_PRESS)
        scale(0.9);
    else if (key==GLFW_KEY_S && action==GLFW_PRESS)
        scale(1.1);
    else if (key==GLFW_KEY_A && action==GLFW_PRESS)
        translate(0,-1,0);
    else if (key==GLFW_KEY_D && action==GLFW_PRESS)
        translate(0,1,0);
    else if (key==GLFW_KEY_Q && action==GLFW_PRESS)
        rotate('x',-0.017);
    else if (key==GLFW_KEY_E && action==GLFW_PRESS)
        rotate('x',0.017);
    else if (key==GLFW_KEY_Z && action==GLFW_PRESS)
        rotate('y',-0.017);
    else if (key==GLFW_KEY_C && action==GLFW_PRESS)
        rotate('y',0.017);
    else if (key==GLFW_KEY_V && action==GLFW_PRESS)
        rotate('z',-0.017);
    else if (key==GLFW_KEY_B && action==GLFW_PRESS)
        rotate('z',0.017);
    else if (key==GLFW_KEY_R && action==GLFW_PRESS)
        reset_camera();
    else if (key==GLFW_KEY_X && action==GLFW_PRESS)
    {
        if (cw) cw=false;
        else cw=true;
    }
    else if (key==GLFW_KEY_EQUAL && action==GLFW_PRESS)
    {
        far_clip+=1.0;
        calc_perspective(near_clip,far_clip,aspect,CLIP_VALUE);
    }
    else if (key==GLFW_KEY_MINUS && action==GLFW_PRESS)
    {
        far_clip-=1.0;
        calc_perspective(near_clip,far_clip,aspect,CLIP_VALUE);
    }
    else if (key==GLFW_KEY_RIGHT_BRACKET && action==GLFW_PRESS)
    {
        near_clip+=0.01;
        calc_perspective(near_clip,far_clip,aspect,CLIP_VALUE);
    }
    else if (key==GLFW_KEY_LEFT_BRACKET && action==GLFW_PRESS)
    {
        near_clip-=0.01;
        calc_perspective(near_clip,far_clip,aspect,CLIP_VALUE);
    }
    else if (key==GLFW_KEY_H && action==GLFW_PRESS)
    {
        current_color[0]=0.0;
        current_color[1]=0.0;
        current_color[2]=1.0;
        reader.set_current_color(current_color);
    }
    else if (key==GLFW_KEY_J && action==GLFW_PRESS)
    {
        current_color[0]=0.0;
        current_color[1]=1.0;
        current_color[2]=0.0;
        reader.set_current_color(current_color);
        
        
    }
    else if (key==GLFW_KEY_K && action==GLFW_PRESS)
    {
        current_color[0]=1.0;
        current_color[1]=0.0;
        current_color[2]=0.0;
        reader.set_current_color(current_color);
    }
    else if (key==GLFW_KEY_N && action==GLFW_PRESS)
        mode = 'f';
    else if (key==GLFW_KEY_M && action==GLFW_PRESS)
        mode = 'p';
    else if (key==GLFW_KEY_COMMA && action==GLFW_PRESS)
        mode='l';
    else if (key==GLFW_KEY_LEFT_ALT && action==GLFW_PRESS)
    {
        max_dist_to_origin=0;
        reader.set_file("/Users/tessdaughton/Desktop/cs412/project_1/bunny.obj");
        coord_all=reader.get_contiguous_array();
        std::copy(coord_all.end(), coord_all.begin(), coord_all_arr);
        camera_position=reader.get_camera_position();
    }
    else if (key==GLFW_KEY_RIGHT_ALT && action==GLFW_PRESS)
    {
        max_dist_to_origin=0;
        coord_all.clear();
        reader.set_file("Resources/cactus.obj");
        coord_all=reader.get_contiguous_array();
        std::copy(coord_all.begin(), coord_all.end(), coord_all_arr);
        camera_position=reader.get_camera_position();
    }
}

void fbreshape(GLFWwindow *window, int w, int h)
{
    window_width=w;
    window_height=h;
    aspect = (GLfloat)w/(GLfloat) h;
}


void init_camera()
{
    coord_all = reader.get_contiguous_array();
    coord_all_arr = new GLfloat[coord_all.size()];
    std::copy(coord_all.begin(), coord_all.end(), coord_all_arr);
    camera_position=reader.get_camera_position();
    camera_front = glm::normalize(FOCUS_POINT-camera_position);
    translate(0,0,4);
}


void init_GLFW()
{
    
    /* Initialize GLFW */
    if (!glfwInit()) {
        fprintf(stderr, "Failed to open GLFW window\n");
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    
    shaded_window = glfwCreateWindow(500,400, "Shaded", NULL, NULL);
    norm_window = glfwCreateWindow(500,400, "Normal", NULL, NULL);
    
    if (!shaded_window || !norm_window)
    {
        printf("Windows not created correctly");
        glfwTerminate();
    }
    
    glfwSetWindowPos(shaded_window,0,0);
    glfwSetWindowPos(norm_window, 500, 0);
    
    glfwMakeContextCurrent(shaded_window);
    glfwGetFramebufferSize(shaded_window, &window_width,
                           &window_height);
    fbreshape(shaded_window, window_width, window_height);
    glfwSetInputMode(shaded_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetKeyCallback(shaded_window, keyboard);
    glClearColor(1.0,1.0,0,0);
    
    glfwMakeContextCurrent(norm_window);
    glfwGetFramebufferSize(norm_window, &window_width,
                           &window_height);
    fbreshape(norm_window, window_width, window_height);
    glfwSetInputMode(norm_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetKeyCallback(norm_window, keyboard);
    glClearColor(1.0,1.0,0,0);
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    //glDisable(GL_CULL_FACE);
    
}

void init_GLEW()
{
    glewExperimental=true;
    
    if(glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
    }
    
}

int main(int argc, char **argv){
    
    init_camera();
    
    init_GLFW();
    init_GLEW();
    
    glfwMakeContextCurrent(shaded_window);
    
    create_buffers();
    load_shaders();
    
    
    set_ModelViewProjection();
    
    while (!glfwWindowShouldClose(shaded_window) &&
           !glfwWindowShouldClose(norm_window))
    {
        display();
        glfwSwapBuffers(shaded_window);
        glfwSwapBuffers(norm_window);
        glfwPollEvents();
    }
    
    glDeleteBuffers(1, &VBO);
    
    glDeleteProgram(pid);
    delete [] coord_all_arr;
    
    glfwDestroyWindow(shaded_window);
    glfwDestroyWindow(norm_window);
    glfwTerminate();
    exit(0);
}
