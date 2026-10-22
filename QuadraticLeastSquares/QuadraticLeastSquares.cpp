#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <float.h>
#include <Windows.h>
#include <conio.h>
double Ln(double x, double a[][4], int n)
{
    return a[0][n] * x * x + a[1][n] * x + a[2][n];
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    printf(" Квадратичная функция\n");

    double mx[8] = { -0.64, -0.36, -0.08, 0.2, 0.48, 0.76, 1.04, 1.32 };
    double my[8] = { 29.51, 18.86, 12.05, 7.7, 4.92, 3.14, 2.01, 1.28 };
    int n1 = 7;
    double sumx = 0;
    double sumx2 = 0;
    double sumx3 = 0;
    double sumx4 = 0;
    double sumy = 0;
    double sumxy = 0;
    double sumx2y = 0;
    for (int i = 0; i <= n1;i++)
    {
        sumx += mx[i];
        sumx2 += mx[i] * mx[i];
        sumx3 += mx[i] * mx[i] * mx[i];
        sumx4 += mx[i] * mx[i] * mx[i] * mx[i];
        sumy += my[i];
        sumxy += mx[i] * my[i];
        sumx2y += mx[i] * mx[i] * my[i];
    }
    double a[3][4] = { {sumx4, sumx3, sumx2, sumx2y},\
                       {sumx3, sumx2, sumx, sumxy},\
                       {sumx2, sumx, 8.0, sumy} };
    for (int i = 0; i <= 2;i++)
    {
        for (int j = 0; j <= 3;j++)
        {
            //printf(" %f ", a[i][j]);
        }
        // printf("\n");
    }
    //Поиск коэф методом Гаусса
    double e = 0.001;
    double d = 0;
    int i, j, k;
    int n = 3;
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
    printf(" f(x) = %f * x^2 + %f * x + %f\n", a[0][n], a[1][n], a[2][n]);
    //Проверка точности
    double sumtoch = 0;
    for (int i = 0; i <= n1;i++)
    {
        //printf(" %f\n", Ln(mx[i], a, n));
        sumtoch = sumtoch + ((my[i] - Ln(mx[i], a, n)) * (my[i] - Ln(mx[i], a, n)));
    }
    printf(" Отклонение = %f\n", sumtoch);

    //оси

    HDC hDC = GetDC(GetConsoleWindow());
    HPEN Pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    SelectObject(hDC, Pen);

    MoveToEx(hDC, 400, 400, NULL);//center (400;400) = (0; 0)
    LineTo(hDC, 1200, 400);
    MoveToEx(hDC, 400, 100, NULL);
    LineTo(hDC, 400, 400);

    HDC hDC3 = GetDC(GetConsoleWindow());
    HPEN Pen3 = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
    SelectObject(hDC3, Pen3);
    for (int i = 0; i < 8; i++)
    {
        MoveToEx(hDC3, 5 * mx[i] + 400, -5 * my[i] + 401, NULL);
        LineTo(hDC3, 5 * mx[i] + 400, -5 * my[i] + 401);

    }
    HDC hDC4 = GetDC(GetConsoleWindow());
    HPEN Pen4 = CreatePen(PS_SOLID, 2, RGB(0, 255, 255));
    SelectObject(hDC4, Pen4);
    for (double i = -2; i < 1.32; i = i + 0.0001)
    {
        MoveToEx(hDC4, 5 * i + 400, -5 * Ln(i, a, n) + 400, NULL);
        LineTo(hDC4, 5 * (i + 0.0001) + 400, -5 * Ln(i + 0.0001, a, n) + 400);

    }
    _getch();

}
