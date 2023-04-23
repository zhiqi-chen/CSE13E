

// Standard libraries
#include <stdio.h>
#include <math.h>


//User libraries:
#include "BOARD.h"
#include "MatrixMath.h"


/******************************************************************************
 * Matrix Display:
 *****************************************************************************/

/**
 * MatrixPrint displays a 3x3 array to standard output with clean, readable, 
 * consistent formatting.  
 * @param: mat, pointer to a 3x3 float array
 * @return: none
 * The printed matrix should be aligned in a grid when called with positive or
 *  negative numbers.  It should be able to display at least FP_DELTA precision, and
 *  should handle numbers as large as 999.0 or -999.0
 */
void MatrixPrint(float mat[3][3]) {
    printf("____________________\n");
    printf("| %.2f | %.2f | %.2f |\n", mat[0][0], mat[0][1], mat[0][2]);
    printf("--------------------\n");
    printf("| %.2f | %.2f | %.2f |\n", mat[1][0], mat[1][1], mat[1][2]);
    printf("--------------------\n");
    printf("| %.2f | %.2f | %.2f |\n", mat[2][0], mat[2][1], mat[2][2]);
    printf("--------------------\n");
}


/******************************************************************************
 * Matrix - Matrix Operations
 *****************************************************************************/

/**
 * MatrixEquals checks if the two matrix arguments are equal (to within FP_DELTA).
 * @param: mat1, pointer to a 3x3 float array
 * @param: mat2, pointer to a 3x3 float array
 * @return: TRUE if and only if every element of mat1 is within FP_DELTA of the corresponding element of mat2,  otherwise return FALSE
 * Neither mat1 nor mat2 is modified by this function.
 */
int MatrixEquals(float mat1[3][3], float mat2[3][3]) {
    int row;
    int column;
    for (row = 0; row < DIM; ++row) {
        for (column = 0; column < DIM; ++column) {
            if (fabs(mat1[row][column] - mat2[row][column]) > FP_DELTA) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

/**
 * MatrixAdd performs an element-wise matrix addition operation on two 3x3 matrices and 
 * "returns" the result by modifying the third argument.
 * @param: mat1, pointer to a summand 3x3 matrix
 * @param: mat2, pointer to a summand 3x3 matrix
 * @param: result, pointer to a 3x3 matrix that is modified to contain the sum of mat1 and mat2.
 * @return:  None
 * mat1 and mat2 are not modified by this function.  result is modified by this function.
 */
void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    int row;
    int column;
    for (row = 0; row < DIM; ++row) {
        for (column = 0; column < DIM; ++column) {
            result[row][column] = mat1[row][column] + mat2[row][column];
        }
    }
}

/**
 * MatrixMultiply performs a matrix-matrix multiplication operation on two 3x3
 * matrices and "returns" the result by modifying the third argument.
 * @param: mat1, pointer to left factor 3x3 matrix
 * @param: mat2, pointer to right factor 3x3 matrix
 * @param: result, pointer to matrix that is modified to contain the matrix product of mat1 and mat2.
 * @return: none
 * mat1 and mat2 are not modified by this function.  result is modified by this function.
 */
void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    int row;
    int column;
    int n;
    for (row = 0; row < DIM; ++row) {
        for (column = 0; column < DIM; ++column) {
            result[row][column]=0;
            for (n = 0; n < DIM; ++n) {
                result[row][column] += mat1[row][n] * mat2[n][column];
            }
        }
    }
}


/******************************************************************************
 * Matrix - Scalar Operations
 *****************************************************************************/

/**
 * MatrixScalarAdd performs the addition of a matrix and a scalar.  Each element of the matrix is increased by x.
 * The result is "returned"by modifying the third argument.
 * @param: x, a scalar float
 * @param: mat, pointer to a 3x3 matrix
 * @param: result, pointer to matrix that is modified to contain mat + x.
 * @return: none
 * x and mat are not modified by this function.  result is modified by this function.
 */
void MatrixScalarAdd(float x, float mat[3][3], float result[3][3]) {
    int row;
    int column;
    for (row = 0; row < DIM; ++row) {
        for (column = 0; column < DIM; ++column) {
            result[row][column] = mat[row][column] + x;
        }
    }
}

/**
 * MatrixScalarAdd performs the multiplication of a matrix and a scalar.
 * Each element of the matrix is multiplied x.
 * The result is "returned"by modifying the third argument.
 * @param: x, a scalar float
 * @param: mat, pointer to a 3x3 matrix
 * @param: result, pointer to matrix that is modified to contain mat + x.
 * @return: none
 * x and mat are not modified by this function.  result is modified by this function.
 */
void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3]) {
    int row;
    int column;
    for (row = 0; row < DIM; ++row) {
        for (column = 0; column < DIM; ++column) {
            result[row][column] = mat[row][column] * x;
        }
    }
}


/******************************************************************************
 * Unary Matrix Operations
 *****************************************************************************/

/**
 * MatrixTrace calculates the trace of a 3x3 matrix.
 * @param: mat, a pointer to a 3x3 matrix
 * @return: the trace of mat
 */
float MatrixTrace(float mat[3][3]) {
    int n;
    float result = 0.0;
    for (n = 0; n < DIM; ++n) {
        result += mat[n][n];
    }
    return result;
}

/**
 * MatrixTranspose calculates the transpose of a matrix and "returns" the
 * result through the second argument.
 * @param: mat, pointer to a 3x3 matrix
 * @param: result, pointer to matrix that is modified to transpose of mat
 * mat is not modified by this function.  result is modified by this function.
 */
void MatrixTranspose(float mat[3][3], float result[3][3]) {
    int row;
    int column;
    for (row = 0; row < DIM; ++row) {
        for (column = 0; column < DIM; ++column) {
            result[row][column] = mat[column][row];
        }
    }
}

/**
 * MatrixSubmatrix finds a submatrix of a 3x3 matrix that is 
 * formed by removing the i-th row and the j-th column.  The 
 * submatrix is "returned" by modifying the final argument.
 * 
 * @param: i, a row of the matrix, INDEXING FROM 0
 * @param: j, a column of the matrix, INDEXING FROM 0
 * @param: mat, a pointer to a 3x3 matrix
 * @param: result, a pointer to a 2x2 matrix
 * @return: none
 * 
 * mat is not modified by this function.  Result is modified by this function.
 */
void MatrixSubmatrix(int i, int j, float mat[3][3], float result[2][2]) {
    int original_row = 0;
    int original_column = 0;
    int new_row = 0;
    int new_column = 0;
    for (original_row = 0; original_row < DIM; ++original_row) {
        new_column = 0;
        if (original_row == i) {
            continue;
        }
        for (original_column = 0; original_column < DIM; ++original_column) {
            if (original_column == j) {
                continue;
            }
            result[new_row][new_column] = mat[original_row][original_column];
            ++new_column;
        }
        ++new_row;
    }
}

/**
 * MatrixDeterminant calculates the determinant of a 3x3 matrix 
 * and returns the value as a float.
 * @param: mat, a pointer to a 3x3 matrix
 * @return: the determinant of mat
 * mat is not modified by this function.
 * */

float MatrixDeterminant2x2(float mat[2][2]) {
    float result2x2;
    result2x2 = (mat[0][0] * mat[1][1])-(mat[0][1] * mat[1][0]);
    return result2x2;
}

float MatrixDeterminant(float mat[3][3]) {
    float result;
    float det11;
    float det12;
    float det13;
    float sub11[2][2];
    MatrixSubmatrix(0, 0, mat, sub11);
    float sub12[2][2];
    MatrixSubmatrix(0, 1, mat, sub12);
    float sub13[2][2];
    MatrixSubmatrix(0, 2, mat, sub13);
    det11 = MatrixDeterminant2x2(sub11);
    det12 = MatrixDeterminant2x2(sub12);
    det13 = MatrixDeterminant2x2(sub13);
    result = mat[0][0] * det11 - mat[0][1] * det12 + mat[0][2] * det13;
    return result;
}

/* MatrixInverse calculates the inverse of a matrix and
 * "returns" the result by modifying the second argument.
 * @param: mat, a pointer to a 3x3 matrix
 * @param: result, a pointer to a 3x3 matrix that is modified to contain the inverse of mat
 * @return: none
 * mat is not modified by this function.  result is modified by this function.
 */
void MatrixInverse(float mat[3][3], float result[3][3]) {
    int i;
    int j;
    float c[3][3];
    float transpose_c[3][3];
    int row;
    int column;
    float sub[2][2];
    for (i = 0; i < DIM; ++i) {
        for (j = 0; j < DIM; ++j) {
            MatrixSubmatrix(i, j, mat, sub);
            if ((i + j) % 2 == 0) {
                c[i][j] = MatrixDeterminant2x2(sub);
            } else {
                c[i][j] = -1 * MatrixDeterminant2x2(sub);
            }
        }
    }
    MatrixTranspose(c, transpose_c);
    for (row = 0; row < DIM; ++row) {
        for (column = 0; column < DIM; ++column) {
            result[row][column] = (transpose_c[row][column]) / (MatrixDeterminant(mat));
        }
    }
}