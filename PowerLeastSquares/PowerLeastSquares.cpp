#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <float.h>
#include <Windows.h>
#include <conio.h>
double Ln(double x, double a[][3], int n)
{
    return a[1][n] * pow(x, a[0][n]);
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    printf(" Степенная функция\n");

    double mx[8] = { log(-0.64 + 1), log(-0.36 + 1), log(-0.08 + 1), log(0.2 + 1), log(0.48 + 1), log(0.76 + 1), log(1.04 + 1), log(1.32 + 1) };
    double my[8] = { log(29.51), log(18.86), log(12.05), log(7.7), log(4.92), log(3.14), log(2.01), log(1.28) };

    double mx1[8] = { -0.64 + 1, -0.36 + 1, -0.08 + 1, 0.2 + 1, 0.48 + 1, 0.76 + 1, 1.04 + 1, 1.32 + 1 };
    double my1[8] = { 29.51, 18.86, 12.05, 7.7, 4.92, 3.14, 2.01, 1.28 };

    //double mx[8] = { log(1), log(1.71), log(2.42), log(3.13), log(3.84), log(4.55), log(5.26), log(5.97) };
    //double my[8] = { log(12.49), log(4.76), log(2.55), log(1.6), log(1.11), log(0.82), log(0.63), log(0.5) };
    //double mx1[8] = { 1, 1.71, 2.42, 3.13, 3.84, 4.55, 5.26, 5.97 };
    //double my1[8] = { 12.49, 4.76, 2.55, 1.6, 1.11, 0.82, 0.63, 0.5 };

    int n1 = 7;
    double sumx = 0;
    double sumxx = 0;
    double sumy = 0;
    double sumxy = 0;
    for (int i = 0; i <= n1;i++)
    {
        sumx += mx[i];
        sumxx += mx[i] * mx[i];
        sumy += my[i];
        sumxy += mx[i] * my[i];
    }
    double a[2][3] = { {sumxx, sumx, sumxy},{sumx, 8.0, sumy} };
    //printf(" %f %f %f %f %f %f\n", a[0][0], a[0][1], a[0][2], a[1][0], a[1][1], a[1][2]);
    //Поиск коэф методом Гаусса
    double e = 0.001;
    double d = 0;
    int i, j, k;
    int n = 2;
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
    a[1][n] = exp(a[1][n]);
    printf(" f(x) = %f * x ^ %f\n", a[1][n], a[0][n]);


    //Проверка точности
    double sumtoch = 0;
    for (int i = 0; i <= n1;i++)
    {
        //printf(" %f\n", Ln(mx1[i], a, n));
        sumtoch = sumtoch + ((my1[i] - Ln(mx1[i], a, n)) * (my1[i] - Ln(mx1[i], a, n)));
    }
    printf(" Отклонение = %f\n", sumtoch);


    //оси

    HDC hDC = GetDC(GetConsoleWindow());
    HPEN Pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    SelectObject(hDC, Pen);

    MoveToEx(hDC, 400 + 10, 400, NULL);//center (400;400) = (0; 0)
    LineTo(hDC, 1000, 400);
    MoveToEx(hDC, 400 + 10, 100, NULL);
    LineTo(hDC, 400 + 10, 400);

    HDC hDC3 = GetDC(GetConsoleWindow());
    HPEN Pen3 = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
    SelectObject(hDC3, Pen3);
    for (int i = 0; i < 8; i++)
    {
        MoveToEx(hDC3, 5 * mx1[i] + 400, -5 * my1[i] + 401, NULL);
        LineTo(hDC3, 5 * mx1[i] + 400, -5 * my1[i] + 401);

    }
    HDC hDC4 = GetDC(GetConsoleWindow());
    HPEN Pen4 = CreatePen(PS_SOLID, 2, RGB(0, 255, 255));
    SelectObject(hDC4, Pen4);
    for (double i = 0; i < 4; i = i + 0.0001)//
    {
        MoveToEx(hDC4, 5 * i + 400, -5 * Ln(i, a, n) + 400, NULL);
        LineTo(hDC4, 5 * (i + 0.0001) + 400, -5 * Ln(i + 0.0001, a, n) + 400);

    }
    _getch();

}