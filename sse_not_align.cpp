#include<iostream>
#include <sys/time.h>                                                                                           
#include <unistd.h>
#include <nmmintrin.h>
using namespace std;

const unsigned long Converter = 1000 * 1000; // 1s == 1000 * 1000 us
const int N = 16;
float matrix[N][N];
//print function
void matrix_print()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << matrix[i][j] << ' ';
        }
        cout << endl;
    }
}
//initial function
void setmatrix()
{
    srand(2);
    for (int i = 0; i < N; i++)
    {
        matrix[i][i] = 1.0;
        for (int j = 0; j < N; j++)
        {
            if (i < j)
                matrix[i][j] = rand() % 2;
            if (i > j)
                matrix[i][j] = 0.0;
        }
    }
    for (int k = 0; k < N; k++)
        for (int i = k + 1; i < N; i++)
            for (int j = 0; j < N; j++)
                matrix[i][j] += matrix[k][j];

}
int main()
{
    //initial
    setmatrix();



    //start time
    struct timeval val;
    int ret = gettimeofday(&val, NULL);
    if (ret == -1)
    {
        printf("Error: gettimeofday()\n");
        return ret;
    }


  
    //operation
    for (int k = 0; k < N; k++)
    {
        __m128 vt = _mm_set1_ps(matrix[k][k]);
        int j = 0;
        for ( j = k + 1; j + 4 <= N; j += 4)
        {
            __m128 va = _mm_loadu_ps(&matrix[k][j]);
            va = _mm_div_ps(va,vt);
            _mm_storeu_ps(&matrix[k][j], va);
        }
        for (; j < N ; j++)
            matrix[k][j] /= matrix[k][k];
        matrix[k][k] = 1.0;
        for (int i = k + 1; i < N; i++)
        {
            __m128 vaik = _mm_set1_ps(matrix[i][k]);
            for (j = k + 1; j + 4 <= N; j += 4)
            {
                __m128 vakj = _mm_loadu_ps(&matrix[k][j]);
                __m128 vaij = _mm_loadu_ps(&matrix[i][j]);
                __m128 vx = _mm_mul_ps(vakj, vaik);
                vaij = _mm_sub_ps(vaij, vx);//原来是这里把减法写成立除法，导致和平凡算法完全不一样，debug了半天，原来如此呜呜
                _mm_storeu_ps(&matrix[i][j], vaij);
            }
            for (; j <N; j++)
                matrix[i][j] = matrix[i][j] - matrix[k][j] * matrix[i][k];
            matrix[i][k] = 0.0;
        }
    }


    //end time
    struct timeval newVal;
    ret = gettimeofday(&newVal, NULL);
    if (ret == -1)
    {
        printf("Error: gettimeofday()\n");
        return ret;
    }

    //print time
    printf("start: sec --- %ld, usec --- %ld\n", val.tv_sec, val.tv_usec);
    printf("end:   sec --- %ld, usec --- %ld\n", newVal.tv_sec, newVal.tv_usec);
    //time sub
    unsigned long diff = (newVal.tv_sec * Converter + newVal.tv_usec) - (val.tv_sec * Converter + val.tv_usec);
    printf("diff:  sec --- %ld, usec --- %ld\n", diff / Converter, diff % Converter);
    //matrix_print();
    return 0;
}