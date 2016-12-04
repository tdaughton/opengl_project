//
//  vect.hpp
//  project_1
//
//  Created by Tess Daughton on 11/9/16.
//  Copyright © 2016 Tess Daughton. All rights reserved.
//

#ifndef vect_hpp
#define vect_hpp

#include <stdio.h>
//using namespace std;

// define class in here
class vect {
    // class variables
    // and function calls
public:
    void setValues(float, float, float);
    float x;
    float y;
    float z;
    float w = 1.0;
    
};

#endif /* vect_hpp */
