//
//  read.cpp
//  project_1
//
//  Created by Tess Daughton on 10/24/16.
//  Copyright © 2016 Tess Daughton. All rights reserved.
//
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include "glm/glm.hpp"

using namespace std;

class Reader
{
    
    
private:
    vector<GLfloat> coord_all;
    vector<GLfloat> vertices;
    vector<GLuint> faces;
    GLfloat current_color[3];
    char* current_file_name;
    GLfloat max_dist_to_origin;
    
    void read_file()
    {
        coord_all.clear();
        vertices.clear();
        faces.clear();
        FILE* file = fopen(current_file_name, "r");
        if (file==NULL) {
            printf("Cannot open file");
            return;
        }
        
        char current[128];
        char vertex_1[128];
        char vertex_2[128];
        char vertex_3[128];
        vector<GLfloat> coords;
        vector<vector <GLfloat> > coord_list;
        
        GLfloat v_1;
        GLfloat v_2;
        GLfloat v_3;
        GLfloat distance;
        
        
        vector<GLfloat> temp_1;
        
        
        string::size_type sz; // alias of size_t
        
        
        while(1)
        {
            if(fscanf(file, "%s", current)==EOF) { break; }
            
            if(strcmp(current,"v")==0)
            {
                fscanf(file, "%s %s %s", vertex_1, vertex_2, vertex_3);
                string::size_type sz;     // alias of size_t
                v_1 = stof(vertex_1,&sz);
                v_2 = stof(vertex_2,&sz);
                v_3 = stof(vertex_3,&sz);
                
                distance = sqrt((v_1-0)*(v_1-0) + (v_2-0)*(v_2-0) + (v_3-0)*(v_3-0));
                if(distance>max_dist_to_origin)
                    max_dist_to_origin=distance;
                
                //for drawing w/o shading
                coords.push_back(v_1);
                coords.push_back(v_2);
                coords.push_back(v_3);
                
                coord_list.push_back(coords);
                coords.clear();
                
                //for drawing w shading, buffers
                vertices.push_back(v_1);
                vertices.push_back(v_2);
                vertices.push_back(v_3);
                vertices.push_back(current_color[0]);
                vertices.push_back(current_color[1]);
                vertices.push_back(current_color[2]);

            }
            
            if(strcmp(current,"f")==0)
            {
                fscanf(file, "%s %s %s", vertex_1, vertex_2, vertex_3);
                v_1 = stoi(vertex_1,&sz)-1;
                v_2 = stoi(vertex_2,&sz)-1;
                v_3 = stoi(vertex_3,&sz)-1;
                
                //for drawing w/o shading
                temp_1=coord_list.at(v_1);
                coord_all.push_back(temp_1.at(0));
                coord_all.push_back(temp_1.at(1));
                coord_all.push_back(temp_1.at(2));

                
                temp_1=coord_list.at(v_2);
                coord_all.push_back(temp_1.at(0));
                coord_all.push_back(temp_1.at(1));
                coord_all.push_back(temp_1.at(2));
                
                
                temp_1=coord_list.at(v_3);
                coord_all.push_back(temp_1.at(0));
                coord_all.push_back(temp_1.at(1));
                coord_all.push_back(temp_1.at(2));
 
                
                temp_1.clear();
                
                //for drawing w shading, buffers
                faces.push_back(v_1);
                faces.push_back(v_2);
                faces.push_back(v_3);

                
            }
        }
        coords.clear();
        coord_list.clear();
    }

    
public:
    Reader(GLfloat red, GLfloat green, GLfloat blue){
        max_dist_to_origin=0.0;
        current_file_name="bunny.obj";
        current_color[0]=red;
        current_color[1]=green;
        current_color[2]=blue;

        read_file();
    }
    void set_file(char* file_name)
    {
        current_file_name=file_name;
        read_file();
    }
    void set_color(GLfloat* color)
    {
        current_color[0]=color[0];
        current_color[1]=color[1];
        current_color[2]=color[2];

    }
    vector<GLfloat> get_contiguous_array()
    {
        return coord_all;
    }
    vector<GLfloat> get_vertices()
    {
        return vertices;
    }
    vector<GLuint> get_faces()
    {
        return faces;
    }
    glm::vec3 get_camera_position()
    {
        return glm::vec3{-3*max_dist_to_origin,0,0};
    }
};
