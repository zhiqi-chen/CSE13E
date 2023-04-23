// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//CMPE13 Support Library
#include "BOARD.h"




// User libraries:
#include "MatrixMath.h"


// Module-level variables:
float matrix1[3][3] = {
    {1.10, 2.20, 3.30},
    {4.40, 5.50, 6.60},
    {7.70, 8.80, 9.90}};
float matrix2[3][3] = {
    {3.87, 1.79, 5.32},
    {9, 4.80, 7.48},
    {2.86, 0, 0.67}};
float matrix3[3][3] = {
    {3.97, 0, 8.05},
    {7.69, 6.47, 8.99},
    {0.07, 4.31, 2}};

int main() {
    BOARD_Init();

    printf("Beginning zchen287's mml test harness, compiled on %s %s\n", __DATE__, __TIME__);

    printf("Output of MatrixPrint():\n");
    MatrixPrint(matrix1);
    printf("Expected output of MatrixPrint():\n");
    printf("____________________\n| 1.10 | 2.20 | 3.30 |\n--------------------\n");
    printf("| 4.40 | 5.50 | 6.60 |\n--------------------\n");
    printf("| 7.70 | 8.80 | 9.90 |\n--------------------\n");

    printf("Output of MatrixPrint():\n");
    MatrixPrint(matrix2);
    printf("Expected output of MatrixPrint():\n");
    printf("____________________\n| 3.87 | 1.79 | 5.32 |\n--------------------\n");
    printf("| 9.00 | 4.80 | 7.48 |\n--------------------\n");
    printf("| 2.86 | 0.00 | 0.67 |\n--------------------\n");

    // Add more tests here!

    // test for MatrixEquals()
    int checker_MatrixEquals = 0;
    float matrix4[3][3] = {
        {1.10, 2.20, 3.30},
        {4.40, 5.50, 6.60},
        {7.70, 8.80, 9.90}};
    int result1;
    int result2;

    result1 = MatrixEquals(matrix1, matrix1);
    if ((result1) == 1) {
        checker_MatrixEquals += 1;
    }
    result2 = MatrixEquals(matrix1, matrix4);
    if ((result2) == 1) {
        checker_MatrixEquals += 1;
    }
    printf("PASSES (%d/2): MatrixEquals()\n", checker_MatrixEquals);

    // test for MatrixMultiply()
    int checker_MatrixMultiply = 0;
    float expected_MatrixMultiply1[3][3] = {
        {33.495, 12.529, 24.519},
        {85.404, 34.276, 68.97},
        {137.313, 56.023, 113.421}};
    float expected_MatrixMultiply2[3][3] = {
        {53.097, 65.175, 77.253},
        {88.616, 112.024, 135.432},
        {8.305, 12.188, 16.071}};
    float output_MatrixMultiply1[3][3];
    float output_MatrixMultiply2[3][3];

    MatrixMultiply(matrix1, matrix2, output_MatrixMultiply1);
    if (MatrixEquals(output_MatrixMultiply1, expected_MatrixMultiply1)) {
        checker_MatrixMultiply += 1;
    }
    MatrixMultiply(matrix2, matrix1, output_MatrixMultiply2);
    if (MatrixEquals(output_MatrixMultiply2, expected_MatrixMultiply2)) {
        checker_MatrixMultiply += 1;
    }
    printf("PASSES (%d/2): MatrixMultiply()\n", checker_MatrixMultiply);

    // test for MatrixScalarMultiply()
    int checker_MatrixScalarMultiply = 0;
    float expected_MatrixScalarMultiply1[3][3] = {
        {2.2, 4.4, 6.6},
        {8.8, 11, 13.2},
        {15.4, 17.6, 19.8}};
    float expected_MatrixScalarMultiply2[3][3] = {
        {11.61, 5.37, 15.96},
        {27, 14.4, 22.44},
        {8.58, 0, 2.01}};
    float output_MatrixScalarMultiply1[3][3];
    float output_MatrixScalarMultiply2[3][3];

    MatrixScalarMultiply(2, matrix1, output_MatrixScalarMultiply1);
    if (MatrixEquals(output_MatrixScalarMultiply1, expected_MatrixScalarMultiply1)) {
        checker_MatrixScalarMultiply += 1;
    }
    MatrixScalarMultiply(3, matrix2, output_MatrixScalarMultiply2);
    if (MatrixEquals(output_MatrixScalarMultiply2, expected_MatrixScalarMultiply2)) {
        checker_MatrixScalarMultiply += 1;
    }
    printf("PASSES (%d/2): MatrixScalarMultiply()\n", checker_MatrixScalarMultiply);

    // test for MatrixDeterminant()
    int checker_MatrixDeterminant = 0;
    float expected_MatrixDeterminant1 = -33.087628;
    float expected_MatrixDeterminant2 = 160.709157;
    float output_MatrixDeterminant1 = 0.0;
    float output_MatrixDeterminant2 = 0.0;

    output_MatrixDeterminant1 = MatrixDeterminant(matrix2);
    if (fabs(output_MatrixDeterminant1 - expected_MatrixDeterminant1) <= FP_DELTA) {
        checker_MatrixDeterminant += 1;
    }
    output_MatrixDeterminant2 = MatrixDeterminant(matrix3);
    if (fabs(output_MatrixDeterminant2 - expected_MatrixDeterminant2) <= FP_DELTA) {
        checker_MatrixDeterminant += 1;
    }
    printf("PASSES (%d/2): MatrixDeterminant()\n", checker_MatrixDeterminant);

    // test for MatrixAdd()
    int checker_MatrixAdd = 0;
    float expected_MatrixAdd1[3][3] = {
        {4.97, 3.99, 8.62},
        {13.4, 10.3, 14.08},
        {10.56, 8.8, 10.57}};
    float expected_MatrixAdd2[3][3] = {
        {7.84, 1.79, 13.37},
        {16.69, 11.27, 16.47},
        {2.93, 4.31, 2.67}};
    float output_MatrixAdd1[3][3];
    float output_MatrixAdd2[3][3];

    MatrixAdd(matrix1, matrix2, output_MatrixAdd1);
    if (MatrixEquals(output_MatrixAdd1, expected_MatrixAdd1)) {
        checker_MatrixAdd += 1;
    }
    MatrixAdd(matrix2, matrix3, output_MatrixAdd2);
    if (MatrixEquals(output_MatrixAdd2, expected_MatrixAdd2)) {
        checker_MatrixAdd += 1;
    }
    printf("PASSES (%d/2): MatrixAdd()\n", checker_MatrixAdd);

    // test for MatrixScalarAdd()
    int checker_MatrixScalarAdd = 0;
    float expected_MatrixScalarAdd1[3][3] = {
        {4.8, 5.9, 7},
        {8.1, 9.2, 10.3},
        {11.4, 12.5, 13.6}};
    float expected_MatrixScalarAdd2[3][3] = {
        {8.87, 6.79, 10.32},
        {14, 9.8, 12.48},
        {7.86, 5, 5.67}};
    float output_MatrixScalarAdd1[3][3];
    float output_MatrixScalarAdd2[3][3];

    MatrixScalarAdd(3.70, matrix1, output_MatrixScalarAdd1);
    if (MatrixEquals(output_MatrixScalarAdd1, expected_MatrixScalarAdd1)) {
        checker_MatrixScalarAdd += 1;
    }
    MatrixScalarAdd(5, matrix2, output_MatrixScalarAdd2);
    if (MatrixEquals(output_MatrixScalarAdd2, expected_MatrixScalarAdd2)) {
        checker_MatrixScalarAdd += 1;
    }
    printf("PASSES (%d/2): MatrixScalarAdd()\n", checker_MatrixScalarAdd);

    // test for MatrixInverse()
    int checker_MatrixInverse = 0;
    float expected_MatrixInverse1[3][3] = {
        {-0.0972, 0.0362, 0.3671},
        {-0.4643, 0.3815, -0.5722},
        {0.4149, -0.1547, -0.0745}};
    float expected_MatrixInverse2[3][3] = {
        {-0.1606, 0.2159, -0.3241},
        {-0.0918, 0.0459, 0.1631},
        {0.2034, -0.1065, 0.1598}};
    float output_MatrixInverse1[3][3];
    float output_MatrixInverse2[3][3];

    MatrixInverse(matrix2, output_MatrixInverse1);
    if (MatrixEquals(output_MatrixInverse1, expected_MatrixInverse1)) {
        checker_MatrixInverse += 1;
    }
    MatrixInverse(matrix3, output_MatrixInverse2);
    if (MatrixEquals(output_MatrixInverse2, expected_MatrixInverse2)) {
        checker_MatrixInverse += 1;
    }
    printf("PASSES (%d/2): MatrixInverse()\n", checker_MatrixInverse);

    // test for MatrixTranspose()
    int checker_MatrixTranspose = 0;
    float expected_MatrixTranspose1[3][3] = {
        {1.1, 4.4, 7.7},
        {2.2, 5.5, 8.8},
        {3.3, 6.6, 9.9}};
    float expected_MatrixTranspose2[3][3] = {
        {3.87, 9, 2.86},
        {1.79, 4.8, 0},
        {5.32, 7.48, 0.67}};
    float output_MatrixTranspose1[3][3];
    float output_MatrixTranspose2[3][3];

    MatrixTranspose(matrix1, output_MatrixTranspose1);
    if (MatrixEquals(output_MatrixTranspose1, expected_MatrixTranspose1)) {
        checker_MatrixTranspose += 1;
    }
    MatrixTranspose(matrix2, output_MatrixTranspose2);
    if (MatrixEquals(output_MatrixTranspose2, expected_MatrixTranspose2)) {
        checker_MatrixTranspose += 1;
    }
    printf("PASSES (%d/2): MatrixTranspose()\n", checker_MatrixTranspose);

    // test for MatrixTrace()
    int checker_MatrixTrace = 0;
    float expected_MatrixTrace1 = 16.5;
    float expected_MatrixTrace2 = 9.34;
    float output_MatrixTrace1 = 0.0;
    float output_MatrixTrace2 = 0.0;

    output_MatrixTrace1 = MatrixTrace(matrix1);
    if (fabs(output_MatrixTrace1 - expected_MatrixTrace1) <= FP_DELTA) {
        checker_MatrixTrace += 1;
    }
    output_MatrixTrace2 = MatrixTrace(matrix2);
    if (fabs(output_MatrixTrace2 - expected_MatrixTrace2) <= FP_DELTA) {
        checker_MatrixTrace += 1;
    }
    printf("PASSES (%d/2): MatrixTrace()\n", checker_MatrixTrace);

    BOARD_End();
    while (1);
}
