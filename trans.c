/* 
 * trans.c - Matrix transpose B = A^T
 * Bindertsetseg 22B1NUM0027
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"
#define FOR(a,q,b,c) for(a=(q); a<(b); a+=(c))
#define FOR1(a,q,b,c) for(a=(q); a>=(b); a-=(c))

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i,j1,j,i1,k1,k2,k3,k4,k5,k6,k7,k8;
    if(N==32&&M==32){
        FOR(i,0,N,8)
            FOR(j,0,M,8)
                FOR(i1,i,i+8,1){
                    k1=A[i1][j];
                    k2=A[i1][j+1];
                    k3=A[i1][j+2];
                    k4=A[i1][j+3];
                    k5=A[i1][j+4];
                    k6=A[i1][j+5];
                    k7=A[i1][j+6];
                    k8=A[i1][j+7];
                    B[j][i1]=k1;
                    B[j+1][i1]=k2;
                    B[j+2][i1]=k3;
                    B[j+3][i1]=k4;
                    B[j+4][i1]=k5;
                    B[j+5][i1]=k6;
                    B[j+6][i1]=k7;
                    B[j+7][i1]=k8;
                }
        return;
    }
    if(N==64&&M==64){
        FOR(i,0,N,8)
            FOR(j,0,M,8){
                FOR(i1,i,i+4,1){
                    k1=A[i1][j];
                    k2=A[i1][j+1];
                    k3=A[i1][j+2];
                    k4=A[i1][j+3];
                    k5=A[i1][j+4];
                    k6=A[i1][j+5];
                    k7=A[i1][j+6];
                    k8=A[i1][j+7];
                    B[j+0][i1]=k1;
                    B[j+1][i1]=k2;
                    B[j+2][i1]=k3;
                    B[j+3][i1]=k4;
                    B[j+0][i1+4]=k5;
                    B[j+1][i1+4]=k6;
                    B[j+2][i1+4]=k7;
                    B[j+3][i1+4]=k8;
                }
                i += 4;
                FOR(i1,j,j+4,1){
                    k1=B[i1][i+0];
                    k2=B[i1][i+1];
                    k3=B[i1][i+2];
                    k4=B[i1][i+3];
                    k5=A[i+0][i1];
                    k6=A[i+1][i1];
                    k7=A[i+2][i1];
                    k8=A[i+3][i1];
                    B[i1][i+0]=k5;
                    B[i1][i+1]=k6;
                    B[i1][i+2]=k7;
                    B[i1][i+3]=k8;
                    B[i1+4][i-4]=k1;
                    B[i1+4][i-3]=k2;
                    B[i1+4][i-2]=k3;
                    B[i1+4][i-1]=k4;
                }
                j += 4;
                FOR(i1,i,i+4,1){
                    // k1=A[i1][j];
                    // k2=A[i1][j+1];
                    // k3=A[i1][j+2];
                    // k4=A[i1][j+3];
                    k5=A[i1][j+0];
                    k6=A[i1][j+1];
                    k7=A[i1][j+2];
                    k8=A[i1][j+3];
                    // B[j][i1]=k1;
                    // B[j+1][i1]=k2;
                    // B[j+2][i1]=k3;
                    // B[j+3][i1]=k4;
                    B[j+0][i1]=k5;
                    B[j+1][i1]=k6;
                    B[j+2][i1]=k7;
                    B[j+3][i1]=k8;
                }
                i -= 4, j -= 4;
            }
        return;
    }
    else{
        FOR(i,0,N,18)
            FOR(j,0,M,18)
                FOR(i1,i,i+18,1)
                    FOR(j1,j,j+18,1){
                        if(i1 >=N|| j1>=M) break;
                        k1=A[i1][j1];
                        B[j1][i1]=k1;
                    }
                
        return;
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

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

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
