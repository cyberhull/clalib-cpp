/*
 * C++ Port of the SLALIB library.
 * Written by Vadim Sytnikov.
 * Copyright (C) 2021 CyberHULL, Ltd.
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#include "slalib.h"

namespace sla {

/**
 * Calculates product of two 3x3 matrices (single precision):
 *
 *   matrix `c`  =  matrix `a`  x  matrix `b`
 *
 * Original FORTRAN code by P.T. Wallace.
 *
 * @param a First matrix (first multiplicand).
 * @param b Second matrix (second multiplicand).
 * @param c Output: product of the two matrices; may be the same matrix as either `a` or `b`.
 */
void mxm(const matrix<float> a, const matrix<float> b, matrix<float> c) {
    // multiply into scratch matrix
    matrix <float> result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            float element = 0.0f;
            for (int k = 0; k < 3; k++) {
                element += a[k][i] * b[j][k];
            }
            result[j][i] = element;
        }
    }

    // return the result
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 3; i++) {
            c[j][i] = result[j][i];
        }
    }
}

}