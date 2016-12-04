//
//  normals.cpp
//  project_3
//
//  Created by Tess Daughton on 11/19/16.
//  Copyright © 2016 Tess Daughton. All rights reserved.
//
//
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include "glm/glm.hpp"

using namespace std;

class Normals
{
private:
    
    vector<vector <glm::vec3> > vert_norms;
    vector<GLfloat> coord_all;
    vector<GLfloat> vertices;
    vector<GLuint> faces;
    
    vector<GLfloat> face_normals;
    vector<GLfloat> vert_normals;
    
    GLfloat* vert_norms_arr;
    GLfloat* face_norms_arr;
    GLfloat* norm_all_arr;
    
    GLfloat* shader_vert_norms_arr;
    GLfloat* shader_face_norms_arr;
    
    
    glm::vec3 get_face_normal(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3 )
    {
        return glm::normalize(glm::cross(pos2-pos1,pos3-pos1));
    }
    
    void calc_all_face_normals()
    {
        GLuint face_1, face_2, face_3;
        
        GLfloat x1, y1, z1;
        GLfloat x2, y2, z2;
        GLfloat x3, y3, z3;
        
        glm::vec3 vec1, vec2, vec3;
        
        glm::vec3 temp_norm;
        
        vert_norms.resize(vertices.size()/3);
        
        for(int i=0; i<faces.size(); i+=3)
        {
            
            face_1 = faces.at(i);
            face_2= faces.at(i+1);
            face_3 = faces.at(i+2);
            
            x1 = vertices.at(face_1*3);
            y1 = vertices.at(face_1*3+1);
            z1 = vertices.at(face_1*3+2);
            
            x2 = vertices.at(face_2*3);
            y2 = vertices.at(face_2*3+1);
            z2 = vertices.at(face_2*3+2);
            
            x3 = vertices.at(face_3*3);
            y3 = vertices.at(face_3*3+1);
            z3 = vertices.at(face_3*3+2);
            
            vec1 = {x1, y1, z1};
            vec2 = {x2, y2, z2};
            vec3 = {x3, y3, z3};
            
            temp_norm = get_face_normal(vec1, vec2, vec3);
            
            for(int i=0; i<3; i++)
            {
                face_normals.push_back(temp_norm.x);
                face_normals.push_back(temp_norm.y);
                face_normals.push_back(temp_norm.z);
            }
            
            vert_norms.at(face_1).push_back(temp_norm);
            vert_norms.at(face_2).push_back(temp_norm);
            vert_norms.at(face_3).push_back(temp_norm);
        }
    }

    void translate_normals()
    {
        glm::vec3 average;
        vert_norms_arr = (GLfloat *) malloc(sizeof(GLfloat) * 3 * vertices.size());
        face_norms_arr = (GLfloat *) malloc(sizeof(GLfloat) * face_normals.size());
    
    
        unsigned long num_norms=0;
        
        for(int i=0; i<vert_norms.size(); i++)
        {
            num_norms = vert_norms.at(i).size();
            for(int j=0; j<num_norms; j++)
            {
                average+= vert_norms.at(i).at(j);
            }
            
            average /=num_norms;
            
            average=normalize(average);
            
            vert_norms_arr[i*3]=average.x;
            vert_norms_arr[i*3+1]=average.y;
            vert_norms_arr[i*3+2]=average.z;
            
            vert_norms.at(i).clear();
            vert_norms.at(i).push_back(average);
            average={0,0,0};
        }
        
        for(int i =0; i<face_normals.size(); i++)
        {
            face_norms_arr[i]=face_normals.at(i);
        }
    }
    
    void create_norm_all()
    {
        norm_all_arr = (GLfloat *) malloc(sizeof(GLfloat) * coord_all.size());
        glm::vec3 temp;
        for(int i=0; i<faces.size(); i++)
        {
            temp = vert_norms.at(faces.at(i)).at(0);
            norm_all_arr[i*3] = temp.x;
            norm_all_arr[i*3+1] = temp.y;
            norm_all_arr[i*3+2] = temp.z;
        }
    }
    
    void create_norms_for_shaders()
    {
        shader_vert_norms_arr = (GLfloat *) malloc(sizeof(GLfloat) * vertices.size()*2);
        shader_face_norms_arr = (GLfloat *) malloc(sizeof(GLfloat) * vertices.size()*2);

        for(int i=0; i<vertices.size(); i+=6)
        {
            shader_vert_norms_arr[i]=vertices.at(i);
            shader_vert_norms_arr[i+1]=vertices.at(i+1);
            shader_vert_norms_arr[i+2]=vertices.at(i+2);
 
            shader_face_norms_arr[i]=vertices.at(i);
            shader_face_norms_arr[i+1]=vertices.at(i+1);
            shader_face_norms_arr[i+2]=vertices.at(i+2);
            
        }
        for(int i=0; i<vert_norms.size(); i++)
        {
            shader_vert_norms_arr[i+3]=vert_norms.at(i).at(0).x;
            shader_vert_norms_arr[i+4]=vert_norms.at(i).at(0).y;
            shader_vert_norms_arr[i+5]=vert_norms.at(i).at(0).z;
            shader_face_norms_arr[i+3]=vert_norms.at(i).at(0).x;
            shader_face_norms_arr[i+4]=vert_norms.at(i).at(0).y;
            shader_face_norms_arr[i+5]=vert_norms.at(i).at(0).z;
        }
    }
    
    void print_all()
    {
//        for(int i=0; i<faces.size()*3; i+=3)
//        {
        //            printf("%f%f%f\n",norm_all_arr[i],norm_all_arr[i+1],norm_all_arr[i+2]);
//        }
        
//        
        for(int i=0; i<faces.size(); i++)
        {
            printf("Faces: %f %f %f\n", face_norms_arr[i*3], face_norms_arr[i*3+1], face_norms_arr[i*3+2]);
        }
        
//
//        printf("faces.size() %lu \n", faces.size());
//        
//        printf("verts.size() %lu \n", vertices.size());
//        
//        printf("verts norms list.size() %lu \n", vert_norms.size());
//        
//        printf("coord_all.size() %lu \n", coord_all.size());
//        
//        printf("faces.size() %lu \n", face_normals.size());
        
    }
    
    
public:
    
    void calc_normals(vector<GLfloat> coords, vector<GLfloat> verts_all, vector<GLuint> faces_all)
    {
        coord_all=coords;
        vertices=verts_all;
        faces=faces_all;
        calc_all_face_normals();
        translate_normals();
        create_norm_all();
        create_norms_for_shaders();
//        print_all();
    }
    
    void recalculate_normals(vector<GLfloat> coords, vector<GLfloat> verts_all, vector<GLuint> faces_all)
    {
        coord_all=coords;
        vertices=verts_all;
        faces=faces_all;
        calc_all_face_normals();
        translate_normals();
        create_norm_all();
        create_norms_for_shaders();

    }
    
    GLfloat* get_vert_norms()
    {
        return norm_all_arr;
    }
    
    GLfloat* get_face_norms()
    {
        return face_norms_arr;
    }
    
    GLfloat* get_shader_vert_norms()
    {
        return shader_vert_norms_arr;
    }
    
    GLfloat* get_shader_face_norms()
    {
        return shader_face_norms_arr;
    }
    
    unsigned long get_face_size()
    {
        return faces.size();
    }
    
    unsigned long get_vert_size()
    {
        return vertices.size();
    }
    
    
    void free_all()
    {
        delete [] norm_all_arr;
        delete [] vert_norms_arr;
        delete [] face_norms_arr;
        delete [] shader_vert_norms_arr; 
        delete [] shader_face_norms_arr;
        
    }
    
};
