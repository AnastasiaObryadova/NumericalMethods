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
    return a[0][n] * x + a[1][n];
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    printf(" Линейная функция\n");

    double mx[8] = { -0.64, -0.36, -0.08, 0.2, 0.48, 0.76, 1.04, 1.32 };
    double my[8] = { 29.51, 18.86, 12.05, 7.7, 4.92, 3.14, 2.01, 1.28 };
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
    printf(" f(x) = %f * x + %f\n", a[0][n], a[1][n]);
    //Проверка точности
    double sumtoch = 0;
    for (int i = 0; i <= n1;i++)
    {
        //printf(" %f\n", Ln(mx[i], a, n));
        sumtoch = sumtoch + ((my[i] - Ln(mx[i], a, n)) * (my[i] - Ln(mx[i], a, n)));
    }
    printf(" Отклонение = %f\n", sumtoch);
    //Вычисление коэфициента корреляции
    double r = 0;
    double s1 = 0;
    double s2 = 0;
    double s3 = 0;
    for (int i = 0; i <= n1;i++)
    {
        s1 += (mx[i] - 1 / n1 * sumx) * (my[i] - 1 / n1 * sumy);
        s2 += (mx[i] - 1 / n1 * sumx) * (mx[i] - 1 / n1 * sumx);
        s3 += (my[i] - 1 / n1 * sumy) * (my[i] - 1 / n1 * sumy);
    }
    r = s1 / sqrtf(s2 * s3);
    printf(" Коэфициент корреляции = %f\n", r);

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
        MoveToEx(hDC3, 10 * mx[i] + 400, -10 * my[i] + 401, NULL);
        LineTo(hDC3, 10 * mx[i] + 400, -10 * my[i] + 401);

    }
    HDC hDC4 = GetDC(GetConsoleWindow());
    HPEN Pen4 = CreatePen(PS_SOLID, 2, RGB(0, 255, 255));
    SelectObject(hDC4, Pen4);
    for (double i = -2; i < 1.32; i = i + 0.0001)
    {
        MoveToEx(hDC4, 10 * i + 400, -10 * Ln(i, a, n) + 400, NULL);
        LineTo(hDC4, 10 * (i + 0.0001) + 400, -10 * Ln(i + 0.0001, a, n) + 400);

    }
    _getch();

}
