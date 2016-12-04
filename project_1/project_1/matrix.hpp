//
//  matrix.hpp
//  project_1
//
//  Created by Tess Daughton on 11/9/16.
//  Copyright © 2016 Tess Daughton. All rights reserved.
//

#ifndef matrix_hpp
#define matrix_hpp

#include <stdio.h>
#include <vector>
using namespace std;

// define class in here
class matrix {
    // class variables
    // and function calls
    
public:
    vector<vector<float> > contents;
    int rows=0;
    int columns=0;
    void setSize(int, int);
    void setColumn(int, vector<float>);
    void addColumn(vector<float>);
    float get(int, int);
};

#endif /* matrix_hpp */
