#include "my_solver.h"


void practice3_1() {
    int i, ia, j, n, l[4];
    float a[16], b[4], x[4], s[4];

    /********************************/
    /*
    1 0 0 0  1
    2 1 1 0  2
    0 1 0 1  3
    0 0 0 1  4
    */
    a[0]  = 1; a[1]  = 2; a[2]  = 0; a[3]  = 0;
    a[4]  = 0; a[5]  = 1; a[6]  = 1; a[7]  = 0;
    a[8]  = 0; a[9]  = 1; a[10] = 0; a[11] = 0;
    a[12] = 0; a[13] = 0; a[14] = 1; a[15] = 1;

    b[0] = 1; b[1] = 2; b[2] = 3; b[3] = 4;

    n = 4; ia = 4;
    printf("***************************************");
    printf("****************Equation***************");
    printf("***************************************\n");
    
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("a[%d][%d] = %f ,",i,j,a[n*j+i]);
        }
        printf("\n");
    }
    printf("b[0] = %f, b[1] = %f, b[2] = %f, b[3] = %f\n", b[0], b[1], b[2], b[3]);
    gespp_(&n, a, &ia, l, s);


    printf("***************************************");
    printf("**************Decomposition************");
    printf("***************************************\n");


    printf("s[0] = %f, s[1] = %f, s[2] = %f, s[3] = %f\n", s[0], s[1], s[2], s[3]);
    printf("l[0] = %d, l[1] = %d, l[2] = %d\n", l[0], l[1], l[2]);

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("a[%d][%d] = %f ,", i, j, a[n * j + i]);
        }
        printf("\n");
    }

    solve_(&n, a, &ia, l ,b, x);

    printf("***************************************");
    printf("*****************Result****************");
    printf("***************************************\n");

    printf("x[0] = %f\n x[1] = %f\n x[2] = %f\n x[3] = %f\n", x[0], x[1], x[2], x[3]);
    /********************************/
}
