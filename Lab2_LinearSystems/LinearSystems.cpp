//#include <Windows.h>
#include <stdlib.h>
#include <math.h>
#include <cmath>
//#include <conio.h>
#include <stdio.h>
#include <iostream>


int main()
{
    setlocale(LC_ALL, "");
    printf("Метод Гаусса:\n\n");
    float e = 0.001;
    int n = 3;
    float d = 0;
    float x[3] = {};
    float y[3] = {};
    float a[3][4] = { {0.92, -0.83, 0.62, 2.15}, {0.24, -0.54, 0.43, 0.62}, {0.73, -0.81, -0.67, 0.88} };
    int i, j, k;
    for (i = 1; i <= n; i++)
    {
        for (j = n + 1; j >= i; j--)
        {
            a[i - 1][j - 1] = a[i - 1][j - 1] / a[i - 1][i - 1];
        }
        for (k = 1; k <= n; k++)
        {
            if (k != i)
            {
                for (j = n + 1; j >= 1; j--)
                {
                    a[k - 1][j - 1] = a[k - 1][j - 1] - a[k - 1][i - 1] * a[i - 1][j - 1];
                }
            }
        }
    }
    for (i = 1; i <= n; i++)
    {
        printf("x%d = %f\n", i, a[i - 1][n]);
    }
    printf("\n");
    printf("Метод Простых итераций:\n\n");

    float b[3][4] = { {92, -83, 62, 215}, {73, -81, -67, 88}, {-49, 27, 110, -26} };
    float bb[3][4] = { {92, -83, 62, 215}, {73, -81, -67, 88}, {-49, 27, 110, -26} };
    //float b[3][4] = { {7.6, 5.8, 4.7, 10.01}, {3.8, 4.1, 2.7, 9.7}, {2.9, 2.1, 3.89, 7.37} };
    //float bb[3][4] = { {7.6, 5.8, 4.7, 10.01}, {3.8, 4.1, 2.7, 9.7}, {2.9, 2.1, 3.89, 7.37} };
    float sum2 = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            if (j == 3)
            {
                bb[i][j] = b[i][j] / b[i][i];
            }
            else
            {
                bb[i][j] = -b[i][j] / b[i][i];
            }
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            bb[i][i] = 0;
            sum2 += bb[i][j] * bb[i][j];
        }
    }
    if (sum2 < 1)
    {
        printf("Условие сходимости не выполняется");
    }
    else
    {
        for (i = 1; i <= n; i++)
        {
            x[i - 1] = b[i - 1][n + 1 - 1];
        }
        d = 0;
        for (i = 1; i <= n; i++)
        {
            y[i - 1] = bb[i - 1][n + 1 - 1];
            for (j = 1; j <= n; j++)
            {
                y[i - 1] = y[i - 1] + bb[i - 1][j - 1] * x[i - 1];
            }
            d = d + (y[i - 1] - x[i - 1]) * (y[i - 1] - x[i - 1]);

        }
        d = sqrt(d);
        while (d > e)
        {
            for (i = 1; i <= n; i++)
            {
                x[i - 1] = y[i - 1];
            }
            d = 0;
            for (i = 1; i <= n; i++)
            {
                y[i - 1] = bb[i - 1][n + 1 - 1];
                for (j = 1; j <= n; j++)
                {
                    y[i - 1] = y[i - 1] + bb[i - 1][j - 1] * x[j - 1];
                }
                d = d + (y[i - 1] - x[i - 1]) * (y[i - 1] - x[i - 1]);
            }
            d = sqrt(d);
        }
        for (i = 1; i <= n; i++)
        {
            printf("x%d = %f\n", i, x[i - 1]);
        }

    }
    printf("\n");
    printf("Метод Зейделя:\n");
    float m[3][4] = { {16194, -14872, -4577, 27478}, {-14872, 14179, 3251, -25675}, {-4577, 3251, 20433, 4574} };

    d = 0;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (j != i)
            {
                m[i][j] = -m[i][j] / m[i][i];
                d = d + m[i][j] * m[i][j];
            }
        }
        m[i][n] = m[i][n] / m[i][i];
        m[i][i] = 0;

    }
    //printf("%g %g %g \n", m[0][0], m[0][1], m[0][2]);
    //printf("%g %g %g \n", m[1][0], m[1][1], m[1][2]);
    //printf("%g %g %g \n", m[2][0], m[2][1], m[2][2]);
    x[0] = 0;
    x[1] = 0;
    x[2] = 0;
    y[0] = 1;
    y[1] = 1;
    y[2] = 1;
    if (sqrtf(d) < 1)//
    {
        printf("%f Условие сходимости не выполняется", sqrtf(d));
    }
    else
    {
        d = 2 * e;
        while (d > e)
        {
            d = 0;
            for (i = 0; i < n; i++)
            {
                y[i] = m[i][n];
                for (j = 0; j < n; j++)
                {
                    y[i] = y[i] + m[i][j] * x[j];
                }
                d = d + (y[i] - x[i]) * (y[i] - x[i]);
                x[i] = y[i];
            }
            d = sqrtf(d);

        }
        for (i = 0; i < n; i++)
        {
            printf("\nx%d = %f", i, x[i]);
        }
    }
}