//
//  linearalgebra.hpp
//  project_1
//
//  Created by Tess Daughton on 11/9/16.
//  Copyright © 2016 Tess Daughton. All rights reserved.
//

#ifndef linearAlgebra_hpp
#define linearAlgebra_hpp

#include <stdio.h>
#include "matrix.hpp"
#include "vect.hpp"
using namespace std;

// define class in here
class linearAlgebra {
    // class variables
    // and function calls
public:
    matrix multiplication(matrix, matrix);
    vect multiplication(matrix, vect);
    matrix multiplication(vect, matrix);
    matrix addition(matrix, matrix);
    vect addition(vect, vect);
    matrix subtraction(matrix, matrix);
    vect subtraction(vect, vect);
    matrix transpose(matrix);
    float dotProduct(vect, vect);
    vect crossProduct(vect, vect);
    void print(vect);
    void print(matrix);
    matrix ModelView(vect, vect, vect, vect);
    matrix Projection(float, float, float, float, float, float);
    vect scale(vect, float);
    matrix rotation(vect, float);
    matrix translation(vect, float);
    float length(vect);
    vect normalize(vect);
    vect normal(vect, vect, vect);
};

#endif /* linearAlgebra_hpp */
