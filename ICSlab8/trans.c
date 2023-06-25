//521021910760 qianweike
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

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
void transpose_work32(int i1,int j1,int M,int N,int A[N][M],int B[M][N])
{
    int i,j,tmp[8];
    for(i=i1;i<i1+8;i++)
    {
        for(j=j1;j<j1+8;j++)
            tmp[j-j1]=A[i][j];
        for(j=j1;j<j1+8;j++)B[j][i]=tmp[j-j1];
    }
    return;    
}
void transpose_work64FULL(int i1,int j1,int M,int N,int A[N][M],int B[M][N])
{
    int i,j,tmp[12];
        
    //line i1
    for(j=j1;j<j1+8;j++)
        tmp[j-j1]=A[i1][j];
    for(i=i1;i<i1+8;i++)
        B[j1][i]=tmp[i-i1];
    //line i1+1
    for(j=j1;j<j1+8;j++)
        tmp[j-j1]=A[i1+1][j];
    tmp[9]=B[j1][i1+1],B[j1][i1+1]=tmp[0],B[j1+1][i1+0]=tmp[9],B[j1+1][i1+1]=tmp[1];
    tmp[9]=B[j1][i1+3],B[j1][i1+3]=tmp[2],B[j1+1][i1+2]=tmp[9],B[j1+1][i1+3]=tmp[3];
    tmp[9]=B[j1][i1+5],B[j1][i1+5]=tmp[4],B[j1+1][i1+4]=tmp[9],B[j1+1][i1+5]=tmp[5];
    tmp[9]=B[j1][i1+7],B[j1][i1+7]=tmp[6],B[j1+1][i1+6]=tmp[9],B[j1+1][i1+7]=tmp[7];
    //line i1+2
    for(j=j1;j<j1+8;j++)
        tmp[j-j1]=A[i1+2][j];
    tmp[9]=B[j1][i1+2],tmp[10]=B[j1][i1+3],B[j1+2][i1]=tmp[9],B[j1+2][i1+1]=tmp[10];
    tmp[9]=B[j1+1][i1+2],B[j1][i1+3]=tmp[9];
    B[j1][i1+2]=tmp[0],B[j1+1][i1+2]=tmp[1],B[j1+2][i1+2]=tmp[2],B[j1+2][i1+3]=tmp[3];

    tmp[9]=B[j1][i1+6],tmp[10]=B[j1][i1+7],B[j1+2][i1+4]=tmp[9],B[j1+2][i1+5]=tmp[10];
    tmp[9]=B[j1+1][i1+6],B[j1][i1+7]=tmp[9];
    B[j1][i1+6]=tmp[4],B[j1+1][i1+6]=tmp[5],B[j1+2][i1+6]=tmp[6],B[j1+2][i1+7]=tmp[7];
    //line i1+3
    for(j=j1;j<j1+8;j++)
        tmp[j-j1]=A[i1+3][j];

    tmp[9]=B[j1][i1+3],B[j1+3][i1]=tmp[9];
    tmp[9]=B[j1+1][i1+3],B[j1+3][i1+1]=tmp[9];
    tmp[9]=B[j1+2][i1+3],B[j1+3][i1+2]=tmp[9];
    B[j1][i1+3]=tmp[0],B[j1+1][i1+3]=tmp[1],B[j1+2][i1+3]=tmp[2],B[j1+3][i1+3]=tmp[3];

    tmp[9]=B[j1][i1+7],B[j1+3][i1+4]=tmp[9];
    tmp[9]=B[j1+1][i1+7],B[j1+3][i1+5]=tmp[9];
    tmp[9]=B[j1+2][i1+7],B[j1+3][i1+6]=tmp[9];
    B[j1][i1+7]=tmp[4],B[j1+1][i1+7]=tmp[5],B[j1+2][i1+7]=tmp[6],B[j1+3][i1+7]=tmp[7];

    //seperate 3 and 7
    for(j=0;j<3;j++)
    {
        for(i=0;i<4;i++)
            tmp[i]=B[j1+j][i1+i+4];
        for(i=0;i<4;i++)
            B[j1+j+4][i1+i]=tmp[i];
    }
    tmp[8]=B[j1+3][i1+4],tmp[9]=B[j1+3][i1+5],tmp[10]=B[j1+3][i1+6],tmp[11]=B[j1+3][i1+7];

    for(i=i1+4;i<i1+6;i++)
    {
        for(j=j1;j<j1+8;j++)
            tmp[j-j1]=A[i][j];
        for(j=j1+4;j<j1+7;j++)
            B[j][i]=tmp[j-j1],B[j][i+2]=tmp[j-j1-4];
        //7 to 3
        B[j1+3][i]=tmp[7],B[j1+3][i+2]=tmp[3];
    }
    //finish 3 and 7
    tmp[0]=B[j1+3][i1+4],tmp[1]=B[j1+3][i1+5],tmp[2]=B[j1+3][i1+6],tmp[3]=B[j1+3][i1+7];
    tmp[4]=A[i1+6][j1+3],tmp[5]=A[i1+6][j1+7],tmp[6]=A[i1+7][j1+3],tmp[7]=A[i1+7][j1+7];
    B[j1+3][i1+4]=tmp[2],B[j1+3][i1+5]=tmp[3],B[j1+3][i1+6]=tmp[4],B[j1+3][i1+7]=tmp[6];
    B[j1+7][i1+0]=tmp[8],B[j1+7][i1+1]=tmp[9],B[j1+7][i1+2]=tmp[10],B[j1+7][i1+3]=tmp[11];
    B[j1+7][i1+4]=tmp[0],B[j1+7][i1+5]=tmp[1],B[j1+7][i1+6]=tmp[5],B[j1+7][i1+7]=tmp[7];

    for(j=j1+4;j<j1+8;j++)
    {
        tmp[j-j1-4]=A[i1+6][j];
        tmp[j-j1]=A[i1+7][j];
    }
    //remove opt to 7
    for(j=j1+4;j<j1+7;j++)
    {
        tmp[9]=B[j][i1+6],B[j][i1+6]=tmp[j-j1-4],tmp[j-j1-4]=tmp[9];
        tmp[9]=B[j][i1+7],B[j][i1+7]=tmp[j-j1],tmp[j-j1]=tmp[9];
    }

    B[j1][i1+4]=tmp[0],B[j1][i1+5]=tmp[4];
    B[j1+1][i1+4]=tmp[1],B[j1+1][i1+5]=tmp[5];

    tmp[0]=A[i1+6][j1],tmp[1]=A[i1+6][j1+1];
    tmp[4]=A[i1+7][j1],tmp[5]=A[i1+7][j1+1];

    B[j1][i1+6]=tmp[0],B[j1][i1+7]=tmp[4];
    B[j1+1][i1+6]=tmp[1],B[j1+1][i1+7]=tmp[5];

    tmp[0]=A[i1+6][j1+2],tmp[1]=A[i1+6][j1+3];
    tmp[4]=A[i1+7][j1+2],tmp[5]=A[i1+7][j1+3];

    B[j1+2][i1+4]=tmp[2],B[j1+2][i1+5]=tmp[6];
    B[j1+2][i1+6]=tmp[0],B[j1+2][i1+7]=tmp[4];

    return;
}
void transpose_work64UNFULL(int i1,int i2,int j1,int j2,int M,int N,int A[N][M],int B[M][N])
{
    int i,j,tmp[8];
    if(j2-j1>=i2-i1)
    {
        for(i=i1;i<i2;i++)
        {
            for(j=j1;j<j2;j++)
                tmp[j-j1]=A[i][j];
            for(j=j1;j<j2;j++)
                B[j][i]=tmp[j-j1];
        }
    }
    else
    {
        for(j=j1;j<j2;j++)
        {
            for(i=i1;i<i2;i++)
                tmp[i-i1]=A[i][j];
            for(i=i1;i<i2;i++)
                B[j][i]=tmp[i-i1];
        }
    }
}
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i,j;
    if(M==32)
    {
        //work 32*32 with 8 local variable
        for(i=0;i<N;i+=8)
            for(j=0;j<M;j+=8)
                transpose_work32(i,j,M,N,A,B);
        return;
    }
    if(M==64)
    {
        //work 64*64  with 12 local variable
        for(i=0;i<N;i+=8)
            for(j=0;j<M;j+=8)
                transpose_work64FULL(i,j,M,N,A,B);
        return;
    }
    //work 67*61  with 8 local variable
    for(i=0;i<N;)
    {
        for(j=0;j<M;)
        {
            if(i<32)
            {
                if(j<40)
                {
                    transpose_work64UNFULL(i,i+8,j,j+8,M,N,A,B);
                    j+=8;
                }
                else
                {
                    transpose_work64UNFULL(i,i+8,j,j+7,M,N,A,B);
                    j+=7;
                }
            }
            else
            {
                if(j<40)
                {
                    transpose_work64UNFULL(i,i+7,j,j+8,M,N,A,B);
                    j+=8;
                }
                else
                {
                    transpose_work64UNFULL(i,i+7,j,j+7,M,N,A,B);
                    j+=7;
                }
            }
        }
        if(i<32)i+=8;
        else i+=7;
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
/*****************
//8*8
********************/
int min(int a,int b){return a<b?a:b;}
int max(int a,int b){return a>b?a:b;}
void transpose_test1_work32(int i1,int j1,int M,int N,int A[N][M],int B[M][N])
{
    //variable +=10
    int i,j,tmp[8];
    // if(i1==j1)
    // {
        //overlap
        for(i=i1;i<i1+8;i++)
        {
            for(j=j1;j<j1+8;j++)
                tmp[j-j1]=A[i][j];
            for(j=j1;j<j1+8;j++)B[j][i]=tmp[j-j1];
        }
        return;    
    // }
    // for(i=i1;i<i1+8;i++)
    // {
    //     for(j=j1;j<j1+8;j++)
    //     {
    //         tmp[0]=A[i][j];
    //         B[j][i]=tmp[0];
    //     }
    // }
}void transpose_test1_work32_2(int i1,int j1,int M,int N,int A[N][M],int B[M][N])
{
    //variable +=10
    int i,j,tmp[8];
    if(i1==j1)
    {
        //overlap
        if(i1+8==N)
        {
            for(i=i1;i<i1+8;i++)
            {
                for(j=j1;j<j1+8;j++)
                {
                    tmp[0]=A[i][j];
                    B[j][i-8]=tmp[0];
                }
            }
            for(i=j1;i<j1+8;i++)
                for(j=i1;j<i1+8;j++)
                    B[i][j]=B[i][j-8];
            return;
        }
        for(i=i1;i<i1+8;i++)
        {
            for(j=j1;j<j1+8;j++)
            {
                tmp[0]=A[i][j];
                B[j][i+8]=tmp[0];
            }
        }
        for(i=j1;i<j1+8;i++)
            for(j=i1;j<i1+8;j++)
                B[i][j]=B[i][j+8];
        // for(i=i1;i<i1+8;i++)
        // {
        //     for(j=j1;j<j1+8;j++)
        //         tmp[j-j1]=A[i][j];
        //     for(j=j1;j<j1+8;j++)B[j][i]=tmp[j-j1];
        // }
        return;    
    }
    for(i=i1;i<i1+8;i++)
    {
        for(j=j1;j<j1+8;j++)
        {
            tmp[0]=A[i][j];
            B[j][i]=tmp[0];
        }
    }
}
void transpose_test1_work64FULL(int i1,int j1,int M,int N,int A[N][M],int B[M][N])
{
    //variable +=10
    int i,j,tmp[12];
    // if(i1==j1)
    // {
        //overlap
        // for(i=i1;i<i1+4;i++)
        // {
        //     for(j=j1;j<j1+8;j++)
        //         tmp[j-j1]=A[i][j];
        //     for(j=j1;j<j1+4;j++)
        //         B[j][i]=tmp[j-j1];
        //     for(j=j1+4;j<j1+8;j++)
        //         B[j-4][i+4]=tmp[j-j1];
        // }
        
        //line i1
        for(j=j1;j<j1+8;j++)
            tmp[j-j1]=A[i1][j];
        for(i=i1;i<i1+8;i++)
            B[j1][i]=tmp[i-i1];
        //line i1+1
        for(j=j1;j<j1+8;j++)
            tmp[j-j1]=A[i1+1][j];
        tmp[9]=B[j1][i1+1],B[j1][i1+1]=tmp[0],B[j1+1][i1+0]=tmp[9],B[j1+1][i1+1]=tmp[1];
        tmp[9]=B[j1][i1+3],B[j1][i1+3]=tmp[2],B[j1+1][i1+2]=tmp[9],B[j1+1][i1+3]=tmp[3];
        tmp[9]=B[j1][i1+5],B[j1][i1+5]=tmp[4],B[j1+1][i1+4]=tmp[9],B[j1+1][i1+5]=tmp[5];
        tmp[9]=B[j1][i1+7],B[j1][i1+7]=tmp[6],B[j1+1][i1+6]=tmp[9],B[j1+1][i1+7]=tmp[7];
        //line i1+2
        for(j=j1;j<j1+8;j++)
            tmp[j-j1]=A[i1+2][j];
        tmp[9]=B[j1][i1+2],tmp[10]=B[j1][i1+3],B[j1+2][i1]=tmp[9],B[j1+2][i1+1]=tmp[10];
        tmp[9]=B[j1+1][i1+2],B[j1][i1+3]=tmp[9];
        B[j1][i1+2]=tmp[0],B[j1+1][i1+2]=tmp[1],B[j1+2][i1+2]=tmp[2],B[j1+2][i1+3]=tmp[3];

        tmp[9]=B[j1][i1+6],tmp[10]=B[j1][i1+7],B[j1+2][i1+4]=tmp[9],B[j1+2][i1+5]=tmp[10];
        tmp[9]=B[j1+1][i1+6],B[j1][i1+7]=tmp[9];
        B[j1][i1+6]=tmp[4],B[j1+1][i1+6]=tmp[5],B[j1+2][i1+6]=tmp[6],B[j1+2][i1+7]=tmp[7];
        //line i1+3
        for(j=j1;j<j1+8;j++)
            tmp[j-j1]=A[i1+3][j];

        tmp[9]=B[j1][i1+3],B[j1+3][i1]=tmp[9];
        tmp[9]=B[j1+1][i1+3],B[j1+3][i1+1]=tmp[9];
        tmp[9]=B[j1+2][i1+3],B[j1+3][i1+2]=tmp[9];
        B[j1][i1+3]=tmp[0],B[j1+1][i1+3]=tmp[1],B[j1+2][i1+3]=tmp[2],B[j1+3][i1+3]=tmp[3];

        tmp[9]=B[j1][i1+7],B[j1+3][i1+4]=tmp[9];
        tmp[9]=B[j1+1][i1+7],B[j1+3][i1+5]=tmp[9];
        tmp[9]=B[j1+2][i1+7],B[j1+3][i1+6]=tmp[9];
        B[j1][i1+7]=tmp[4],B[j1+1][i1+7]=tmp[5],B[j1+2][i1+7]=tmp[6],B[j1+3][i1+7]=tmp[7];

        //seperate 3 and 7
        for(j=0;j<3;j++)
        {
            for(i=0;i<4;i++)
                tmp[i]=B[j1+j][i1+i+4];
            for(i=0;i<4;i++)
                B[j1+j+4][i1+i]=tmp[i];
        }
        tmp[8]=B[j1+3][i1+4],tmp[9]=B[j1+3][i1+5],tmp[10]=B[j1+3][i1+6],tmp[11]=B[j1+3][i1+7];
        // for(i=i1+4;i<i1+8;i++)
        // {
        //     for(j=j1+4;j<j1+8;j++)
        //         tmp[j-j1]=A[i][j];
        //     for(j=j1+4;j<j1+8;j++)
        //         B[j][i]=tmp[j-j1];
        // }
        // for(i=i1+4;i<i1+8;i++)
        // {
        //     for(j=j1;j<j1+4;j++)
        //         tmp[j-j1]=A[i][j];
        //     for(j=j1;j<j1+4;j++)
        //         B[j][i]=tmp[j-j1];
        // }
        for(i=i1+4;i<i1+6;i++)
        {
            for(j=j1;j<j1+8;j++)
                tmp[j-j1]=A[i][j];
            for(j=j1+4;j<j1+7;j++)
                B[j][i]=tmp[j-j1],B[j][i+2]=tmp[j-j1-4];
          //7 to 3
            B[j1+3][i]=tmp[7],B[j1+3][i+2]=tmp[3];
        }
        //finish 3 and 7
        tmp[0]=B[j1+3][i1+4],tmp[1]=B[j1+3][i1+5],tmp[2]=B[j1+3][i1+6],tmp[3]=B[j1+3][i1+7];
        tmp[4]=A[i1+6][j1+3],tmp[5]=A[i1+6][j1+7],tmp[6]=A[i1+7][j1+3],tmp[7]=A[i1+7][j1+7];
        B[j1+3][i1+4]=tmp[2],B[j1+3][i1+5]=tmp[3],B[j1+3][i1+6]=tmp[4],B[j1+3][i1+7]=tmp[6];
        B[j1+7][i1+0]=tmp[8],B[j1+7][i1+1]=tmp[9],B[j1+7][i1+2]=tmp[10],B[j1+7][i1+3]=tmp[11];
        B[j1+7][i1+4]=tmp[0],B[j1+7][i1+5]=tmp[1],B[j1+7][i1+6]=tmp[5],B[j1+7][i1+7]=tmp[7];

        for(j=j1+4;j<j1+8;j++)
        {
            tmp[j-j1-4]=A[i1+6][j];
            tmp[j-j1]=A[i1+7][j];
        }
        //remove opt to 7
        for(j=j1+4;j<j1+7;j++)
        {
            tmp[9]=B[j][i1+6],B[j][i1+6]=tmp[j-j1-4],tmp[j-j1-4]=tmp[9];
            tmp[9]=B[j][i1+7],B[j][i1+7]=tmp[j-j1],tmp[j-j1]=tmp[9];
        }

        B[j1][i1+4]=tmp[0],B[j1][i1+5]=tmp[4];
        B[j1+1][i1+4]=tmp[1],B[j1+1][i1+5]=tmp[5];

        tmp[0]=A[i1+6][j1],tmp[1]=A[i1+6][j1+1];
        tmp[4]=A[i1+7][j1],tmp[5]=A[i1+7][j1+1];

        B[j1][i1+6]=tmp[0],B[j1][i1+7]=tmp[4];
        B[j1+1][i1+6]=tmp[1],B[j1+1][i1+7]=tmp[5];

        tmp[0]=A[i1+6][j1+2],tmp[1]=A[i1+6][j1+3];
        tmp[4]=A[i1+7][j1+2],tmp[5]=A[i1+7][j1+3];

        B[j1+2][i1+4]=tmp[2],B[j1+2][i1+5]=tmp[6];
        B[j1+2][i1+6]=tmp[0],B[j1+2][i1+7]=tmp[4];

        //remove opt to 3
        // B[j1+3][i1+4]=tmp[3],B[j1+3][i1+5]=tmp[7];
        // B[j1+3][i1+6]=tmp[1],B[j1+3][i1+7]=tmp[5];

        // B[j1][i1+6]=tmp[0],B[j1+1][i1+6]=tmp[1],B[j1+3][i1+6]=tmp[4];
        // B[j1+2][i1+4]=tmp[2],B[j1+2][i1+5]=tmp[6],B[j1+2][i1+6]=tmp[3];
        
        // tmp[0]=A[i1+7][j1],tmp[1]=A[i1+7][j1+1];
        // tmp[2]=A[i1+7][j1+2],tmp[3]=A[i1+7][j1+3];

        // B[j1][i1+7]=tmp[0],B[j1+1][i1+7]=tmp[1];
        // B[j1+2][i1+7]=tmp[2],B[j1+3][i1+7]=tmp[3];
        return;    
    // }
    // //normal
    // for(i=i1;i<i1+4;i++)
    // {
    //     for(j=j1;j<j1+4;j++)
    //     {
    //         tmp[0]=A[i][j];
    //         B[j][i]=tmp[0];
    //     }
    //     for(j=j1+4;j<j1+8;j++)
    //     {
    //         tmp[0]=A[i][j];
    //         B[j-4][i+4]=tmp[0];
    //     }
    // }
    // for(j=0;j<4;j+=2)
    // {
    //     for(i=0;i<4;i++)
    //         tmp[i]=B[j1+j][i1+i+4],tmp[i+4]=B[j1+j+1][i1+i+4];
    //     for(i=0;i<4;i++)
    //         B[j1+j+4][i1+i]=tmp[i],B[j1+j+1+4][i1+i]=tmp[i+4];
    // }
    // for(i=i1+4;i<i1+8;i++)
    // {
    //     for(j=j1+4;j<j1+8;j++)
    //     {
    //         tmp[0]=A[i][j];
    //         B[j][i]=tmp[0];
    //     }   
    // }
    // for(i=i1+4;i<i1+8;i++)
    // {
    //     for(j=j1;j<j1+4;j++)
    //     {
    //         tmp[0]=A[i][j];
    //         B[j][i]=tmp[0];
    //     }   
    // }
}
void transpose_test1_work64UNFULL(int i1,int i2,int j1,int j2,int M,int N,int A[N][M],int B[M][N])
{
    //variable +=10
    int i,j,tmp[8];
    if(j2-j1>=i2-i1)
    {
        for(i=i1;i<i2;i++)
        {
            for(j=j1;j<j2;j++)
                tmp[j-j1]=A[i][j];
            for(j=j1;j<j2;j++)
                B[j][i]=tmp[j-j1];
        }
    }
    else
    {
        for(j=j1;j<j2;j++)
        {
            for(i=i1;i<i2;i++)
                tmp[i-i1]=A[i][j];
            for(i=i1;i<i2;i++)
                B[j][i]=tmp[i-i1];
        }
    }
}
char transpose_test1_desc[] = "Transpose test1 with 8*8";
void transpose_test1(int M, int N, int A[N][M], int B[M][N])
{
    //variable +=2
    int i,j;
    if(M==32)
    {
        //work 32*32
        for(i=0;i<N;i+=8)
        {
            for(j=0;j<M;j+=8)
            {
                transpose_test1_work32(i,j,M,N,A,B);
            }
        }
        // for(i=0;i<N;i+=8)
        //     transpose_test1_work32_2(i,i,M,N,A,B);
        // for(i=0;i<N;i+=8)
        //     for(j=0;j<M;j+=8)
        //         if(i!=j)transpose_test1_work32_2(i,j,M,N,A,B);
        return;
    }
    if(M==64)
    {
        for(i=0;i<N;i+=8)
        {
            for(j=0;j<M;j+=8)
            {
                transpose_test1_work64FULL(i,j,M,N,A,B);
            }
        }
        //work 64*64
        return;
    }
    //work 67*61
    for(i=0;i<N;)
    {
        for(j=0;j<M;)
        {
            if(i<32)
            {
                if(j<40)
                {
                    transpose_test1_work64UNFULL(i,i+8,j,j+8,M,N,A,B);
                    j+=8;
                }
                else
                {
                    transpose_test1_work64UNFULL(i,i+8,j,j+7,M,N,A,B);
                    j+=7;
                }
            }
            else
            {
                if(j<40)
                {
                    transpose_test1_work64UNFULL(i,i+7,j,j+8,M,N,A,B);
                    j+=8;
                }
                else
                {
                    transpose_test1_work64UNFULL(i,i+7,j,j+7,M,N,A,B);
                    j+=7;
                }
            }
        }
        if(i<32)i+=8;
        else i+=7;
    }
    // for(i=N;i>=0;i-=8)
    // {
    //     for(j=M;j>=0;j-=8)
    //     {
    //         transpose_test1_work64UNFULL(max(0,i-8),i,max(0,j-8),j,M,N,A,B);
    //     }
    // }
    return;
            // else transpose_test1_work64UNFULL(i,min(i+8,N),j,min(j+8,N),M,N,A,B);
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
    // registerTransFunction(trans, trans_desc); 

    registerTransFunction(transpose_test1, transpose_test1_desc); 

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

