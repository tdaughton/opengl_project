//
//  linearalgebra.cpp
//  project_1
//
//  Created by Tess Daughton on 11/9/16.
//  Copyright © 2016 Tess Daughton. All rights reserved.
//

#include "linearalgebra.hpp"

#include "linearAlgebra.hpp"
#include <iostream>
#include <cmath>
using namespace std;

// put functions in here
matrix linearAlgebra::multiplication(matrix a, matrix b) {
    matrix result;
    if(a.columns==b.rows) {
        vector<float> contents;
        float number;
        for(int i=0; i<b.columns; i++) {
            for(int j=0; j<a.rows; j++) {
                number=0;
                for(int k=0; k<a.columns; k++) {
                    number+=a.get(k,j)*b.get(i,k);
                }
                contents.push_back(number);
            }
            result.addColumn(contents);
            contents=vector<float>();
        }
    }
    return result;
}

vect linearAlgebra::multiplication(matrix a, vect b) {
    vect result;
    if(a.columns==4) {
        result.setValues((a.get(0,0)*b.x)+(a.get(1,0)*b.y)+(a.get(2,0)*b.z)+(a.get(3,0)*b.w),
                         (a.get(0,1)*b.x)+(a.get(1,1)*b.y)+(a.get(2,1)*b.z)+(a.get(3,1)*b.w),
                         (a.get(0,2)*b.x)+(a.get(1,2)*b.y)+(a.get(2,2)*b.z)+(a.get(3,2)*b.w));
    }
    return result;
}

matrix linearAlgebra::multiplication(vect a, matrix b) {
    matrix result;
    if(b.rows==1) {
        
    }
    return result;
}

matrix linearAlgebra::addition(matrix a, matrix b) {
    // verify that they are the same size
    matrix result;
    if(a.columns==b.columns && a.rows==b.rows) {
        vector<float> contents;
        for(int i=0; i<a.columns; i++) {
            for(int j=0; j<a.rows; j++) {
                contents.push_back(a.get(i,j)+b.get(i,j));
            }
            result.addColumn(contents);
            // clear contents for next column
            contents = vector<float>();
        }
    }
    return result;
}

vect linearAlgebra::addition(vect a, vect b) {
    vect result;
    result.setValues(a.x+b.x, a.y+b.y, a.z+b.z);
    return result;
}

matrix linearAlgebra::subtraction(matrix a, matrix b) {
    matrix result;
    if(a.columns==b.columns && a.rows==b.rows) {
        vector<float> contents;
        for(int i=0; i<a.columns; i++) {
            for(int j=0; j<a.rows; j++) {
                contents.push_back(a.get(i,j)-b.get(i,j));
            }
            result.addColumn(contents);
            // clear contents for next column
            contents = vector<float>();
        }
    }
    return result;
}

vect linearAlgebra::subtraction(vect a, vect b) {
    vect result;
    result.setValues(a.x-b.x, a.y-b.y, a.z-b.z);
    return result;
}

matrix linearAlgebra::transpose(matrix a) {
    matrix result;
    vector<float> column;
    for(int i=0; i<a.rows; i++) {
        for(int j=0; j<a.columns; j++) {
            column.push_back(a.get(j,i));
        }
        result.addColumn(column);
        column = vector<float>();
    }
    return result;
}

float linearAlgebra::dotProduct(vect a, vect b) {
    float result = (a.x*b.x)+(a.y*b.y)+(a.z*b.z);
    return result;
}

vect linearAlgebra::crossProduct(vect a, vect b) {
    vect result;
    result.setValues((a.y*b.z)-(a.z*b.y),(a.x*b.z)-(a.z*b.x),(a.x*b.y)-(a.y*b.x));
    return result;
}

void linearAlgebra::print(vect a) {
    cout << endl;
    cout << a.x << endl;
    cout << a.y << endl;
    cout << a.z << endl;
}

void linearAlgebra::print(matrix a) {
    cout << endl;
    for(int i=0; i<a.rows; i++) {
        for(int j=0; j<a.columns; j++) {
            cout << a.contents.at(j).at(i) << "\t\t";
        }
        cout << endl;
    }
}

matrix linearAlgebra::ModelView(vect u, vect v, vect n, vect c) {
    matrix modelView;
    linearAlgebra calc;
    
    vector<float> column;
    column.push_back(u.x);
    column.push_back(v.x);
    column.push_back(n.x);
    column.push_back(0);
    modelView.addColumn(column);
    
    column = vector<float>();
    column.push_back(u.y);
    column.push_back(v.y);
    column.push_back(n.y);
    column.push_back(0);
    modelView.addColumn(column);
    
    column = vector<float>();
    column.push_back(u.z);
    column.push_back(v.z);
    column.push_back(n.z);
    column.push_back(0);
    modelView.addColumn(column);
    
    vect negC;
    negC.setValues(-c.x, -c.y, -c.z);
    
    column = vector<float>();
    column.push_back(calc.dotProduct(negC, u));
    column.push_back(calc.dotProduct(negC, v));
    column.push_back(calc.dotProduct(negC, n));
    column.push_back(1);
    modelView.addColumn(column);
    
    return modelView;
}

matrix linearAlgebra::Projection(float nearBound, float farBound, float topBound, float bottomBound, float rightBound, float leftBound) {
    matrix projection;
    
    // first column
    vector<float> column;
    column.push_back((2*nearBound)/(rightBound-leftBound));
    column.push_back(0);
    column.push_back(0);
    column.push_back(0);
    projection.addColumn(column);
    
    // second column
    column = vector<float>();
    column.push_back(0);
    column.push_back((2*nearBound)/(topBound-bottomBound));
    column.push_back(0);
    column.push_back(0);
    projection.addColumn(column);
    
    // third column
    column = vector<float>();
    column.push_back((rightBound+leftBound)/(rightBound-leftBound));
    column.push_back((topBound+bottomBound)/(topBound-bottomBound));
    column.push_back((-1*(farBound+nearBound))/(farBound-nearBound));
    column.push_back(-1);
    projection.addColumn(column);
    
    // fourth column
    column = vector<float>();
    column.push_back(0);
    column.push_back(0);
    column.push_back((-1*(2*farBound*nearBound))/(farBound-nearBound));
    column.push_back(0);
    projection.addColumn(column);
    
    return projection;
}

vect linearAlgebra::scale(vect a, float factor) {
    vect result;
    result.setValues(a.x*factor, a.y*factor, a.z*factor);
    return result;
}

matrix linearAlgebra::rotation(vect axis, float degree) {
    matrix result;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float x2 = x*x;
    float y2 = y*y;
    float z2 = z*z;
    float length2 = x2+y2+z2;
    float length = sqrt(length2);
    
    vector<float> column;
    // column 1
    column.push_back((x2+((y2+z2)*cos(degree*(M_PI/180))))/(length2));
    column.push_back(((x*y*(1-cos(degree*(M_PI/180))))+(z*length*sin(degree*(M_PI/180))))/(length2));
    column.push_back(((x*z*(1-cos(degree*(M_PI/180))))-(y*length*sin(degree*(M_PI/180))))/(length2));
    column.push_back(0);
    result.addColumn(column);
    
    column = vector<float>();
    // column 2
    column.push_back(((x*y*(1-cos(degree*(M_PI/180))))-(z*length*sin(degree*(M_PI/180))))/(length2));
    column.push_back((y2+((x2+z2)*cos(degree*(M_PI/180))))/(length2));
    column.push_back(((y*z*(1-cos(degree*(M_PI/180))))+(x*length*sin(degree*(M_PI/180))))/(length2));
    column.push_back(0);
    result.addColumn(column);
    
    column = vector<float>();
    // column 3
    column.push_back(((x*z*(1-cos(degree*(M_PI/180))))+(y*length*sin(degree*(M_PI/180))))/(length2));
    column.push_back(((y*z*(1-cos(degree*(M_PI/180))))-(x*length*sin(degree*(M_PI/180))))/(length2));
    column.push_back((z2+((x2+y2)*cos(degree*(M_PI/180))))/(length2));
    column.push_back(0);
    result.addColumn(column);
    
    column = vector<float>();
    // column 4
    column.push_back(0);
    column.push_back(0);
    column.push_back(0);
    column.push_back(1);
    result.addColumn(column);
    
    return result;
}

matrix linearAlgebra::translation(vect direction, float amount) {
    matrix translate;
    vector<float> column;
    column.push_back(1);
    column.push_back(0);
    column.push_back(0);
    column.push_back(0);
    translate.addColumn(column);
    
    column = vector<float>();
    column.push_back(0);
    column.push_back(1);
    column.push_back(0);
    column.push_back(0);
    translate.addColumn(column);
    
    column = vector<float>();
    column.push_back(0);
    column.push_back(0);
    column.push_back(1);
    column.push_back(0);
    translate.addColumn(column);
    
    column = vector<float>();
    column.push_back(direction.x);
    column.push_back(direction.y);
    column.push_back(direction.z);
    column.push_back(1);
    translate.addColumn(column);
    
    return translate;
}

float linearAlgebra::length(vect a) {
    float length = sqrt((a.x*a.x)+(a.y*a.y)+(a.z*a.z));
    return length;
}

vect linearAlgebra::normalize(vect a) {
    float len = length(a);
    vect result = scale(a,(1/len));
    return result;
}

vect linearAlgebra::normal(vect p0, vect p1, vect p2) {
    vect normal;
    normal = normalize(crossProduct(subtraction(p1, p0), subtraction(p2, p0)));
    return normal;
}
