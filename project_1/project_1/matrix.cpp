//
//  matrix.cpp
//  project_1
//
//  Created by Tess Daughton on 11/9/16.
//  Copyright © 2016 Tess Daughton. All rights reserved.
//

#include "matrix.hpp"

// put functions in here
// set the size of the matrix
// should only be called at the beginning when making it
// probably won't use this one
void matrix::setSize(int x, int y) {
    rows = x;
    columns = y;
    for(int i=0; i<columns; i++) {
        contents.push_back(vector<float>());
    }
}

// set a pre-existing column at a specified index
void matrix::setColumn(int index, vector<float> column) {
    if(column.size()==rows && index<columns) {
        contents.at(index) = column;
    }
}

// add a column to "contents"
void matrix::addColumn(vector<float> column) {
    if(rows==0 && columns==0) {
        contents.push_back(column);
        rows=column.size();
        columns+=1;
    } else if(column.size()==rows) {
        contents.push_back(column);
        columns+=1;
    }
}

// return a number at column, row
float matrix::get(int column, int row) {
    return contents.at(column).at(row);
}
