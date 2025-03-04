/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"
#include "contracts.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. The REQUIRES and ENSURES from 15-122 are included
 *     for your convenience. They can be removed if you like.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, s, k, r1, r2, r3, r4, r5, r6, r7, r8;
    if(N == 64 && M == 64){
        for(i = 0; i < N; i += 8){
            for(j = 0; j < M; j += 8){
                if(i == j){
                    for(s = i; s < i + 8; s += 2){
                        r1 = A[s][j];
                        r2 = A[s][j + 1];
                        r3 = A[s][j + 2];
                        r4 = A[s][j + 3];
                        r5 = A[s + 1][j];
                        r6 = A[s + 1][j + 1];
                        r7 = A[s + 1][j + 2];
                        r8 = A[s + 1][j + 3];
                        B[j][s] = r1;
                        B[j + 1][s] = r2;
                        B[j + 2][s] = r3;
                        B[j + 3][s] = r4;
                        B[j][s + 1] = r5;
                        B[j + 1][s + 1] = r6;
                        B[j + 2][s + 1] = r7;
                        B[j + 3][s + 1] = r8;
                    }
                    j += 4;
                    for(s = i; s < i + 8; s += 2){
                        r1 = A[s][j];
                        r2 = A[s][j + 1];
                        r3 = A[s][j + 2];
                        r4 = A[s][j + 3];
                        r5 = A[s + 1][j];
                        r6 = A[s + 1][j + 1];
                        r7 = A[s + 1][j + 2];
                        r8 = A[s + 1][j + 3];
                        B[j][s] = r1;
                        B[j + 1][s] = r2;
                        B[j + 2][s] = r3;
                        B[j + 3][s] = r4;
                        B[j][s + 1] = r5;
                        B[j + 1][s + 1] = r6;
                        B[j + 2][s + 1] = r7;
                        B[j + 3][s + 1] = r8;
                    }
                    j -= 4;
                }
            else {
                for(s = i; s < i + 4; s++){
                    for(k = j; k < j + 4; k++){
                        B[k][s] = A[s][k];
                    }
                }
                r1 = A[i][j + 4];
                r2 = A[i][j + 5];
                r3 = A[i][j + 6];
                r4 = A[i][j + 7];
                r5 = A[i + 1][j + 4];
                r6 = A[i + 1][j + 5];
                r7 = A[i + 1][j + 6];
                r8 = A[i + 1][j + 7];
                i += 4;
                for(s = i; s < i + 4; s++){
                    for(k = j; k < j + 4; k++){
                        B[k][s] = A[s][k];
                    }
                }
                j += 4;
                for(s = i; s < i + 4; s++){
                    for(k = j; k < j + 4; k++){
                        B[k][s] = A[s][k];
                    }
                }
                i -= 4;
                for(s = i + 2; s < i + 4; s++){
                    for(k = j; k < j + 4; k++){
                        B[k][s] = A[s][k];
                    }
                }
                B[j + 0][i + 0] = r1;
                B[j + 0][i + 1] = r5;
                B[j + 1][i + 0] = r2;
                B[j + 1][i + 1] = r6;
                B[j + 2][i + 0] = r3;
                B[j + 2][i + 1] = r7;
                B[j + 3][i + 0] = r4;
                B[j + 3][i + 1] = r8;
                j -= 4;
            }
            }
        }
    }
    else if(N == 61 && M == 67){
        for(i = 0; i < N; i += 17)
        for(j = 0; j < M; j += 17)
        for(s = i; s < i + 17 && s < N; s++)
        for(k = j; k < j + 17 && k < M; k++)
            B[k][s] = A[s][k];      
        
    }
    else if(N == 32 && M == 32){
        for(i = 0; i < N; i += 8){
            for(j = 0; j < M; j += 8){
                for(k = i; k < i + 8; k++){
                    r1 = A[k][j + 0];
                    r2 = A[k][j + 1];
                    r3 = A[k][j + 2];
                    r4 = A[k][j + 3];
                    r5 = A[k][j + 4];
                    r6 = A[k][j + 5];
                    r7 = A[k][j + 6];
                    r8 = A[k][j + 7];
                    B[j + 0][k] = r1;
                    B[j + 1][k] = r2;
                    B[j + 2][k] = r3;
                    B[j + 3][k] = r4;
                    B[j + 4][k] = r5;
                    B[j + 5][k] = r6;
                    B[j + 6][k] = r7;
                    B[j + 7][k] = r8;
                }
            }
        }
    }

}

/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

 /*
  * trans - A simple baseline transpose function, not optimized for the cache.
  */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{ 
    int i, j, tmp;

    REQUIRES(M > 0);
    REQUIRES(N > 0);

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

    ENSURES(is_transpose(M, N, A, B));
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);

}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}
