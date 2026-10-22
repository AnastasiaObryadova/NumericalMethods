#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <float.h>
#include <Windows.h>
#include <conio.h>
unsigned long fact(unsigned long n)
{
    if (n < 0)
    {
        return 0;
    }
    if (n == 0)
    {
        return 1;
    }
    else
    {
        return n * fact(n - 1);
    }
}
double K_Diff(double* my, int k, int i)
{
    int ik;
    double pr = 1;
    double sum = 0;
    for (ik = 0; ik <= k; ik++)
    {
        pr = 1;
        for (int j = 0; j <= ik; j++)
        {
            if (ik == j)
            {
                pr = pr * 1;
            }
            else
            {
                pr = pr * (k - j);
            }
        }
        sum = sum + pr * pow((-1), ik) * (my[i + k - ik]) / fact(ik);
    }
    return sum;
}

double N(double x, double* my, double* mx, double h, int n)
{
    double pr;
    double sum = 0;
    for (int i = 0; i <= n; i++)
    {
        pr = 1;
        for (int j = 0; j <= i; j++)
        {
            if (j == i)
            {
                pr = pr * 1;
            }
            else
            {
                pr = pr * ((x - mx[0]) / h - j);
            }
        }
        sum = sum + pr * (K_Diff(my, i, 0) / fact(i));
    }

    return sum;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    printf(" »нтерпол€ционный полином Ќьютона\n");

    double mx[10] = { 0.101, 0.106, 0.111, 0.116, 0.121, 0.126, 0.131, 0.136, 0.141, 0.146 };
    double my[10] = { 1.26183, 1.27644, 1.29122, 1.30617, 1.32130, 1.33660, 1.35207, 1.36773, 1.38357, 1.39959 };

    double x1[3] = { 0.1156, 0.1142, 0.1063 };
    double x2[3] = { 0.1457, 0.1452, 0.1369 };
    double h = mx[1] - mx[0];//шаг интерполировани€
    int n = 9;
    printf("%f\n", N(0.526, my, mx, h, n));
    printf("\n");
    printf("%f\n", N(x1[0], my, mx, h, n));
    printf("%f\n", N(x2[0], my, mx, h, n));
    printf("%f\n", N(x1[1], my, mx, h, n));
    printf("%f\n", N(x2[1], my, mx, h, n));
    printf("%f\n", N(x1[2], my, mx, h, n));
    printf("%f\n", N(x2[2], my, mx, h, n));

    //оси
    HDC hDC = GetDC(GetConsoleWindow());
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    SelectObject(hDC, Pen);

    MoveToEx(hDC, 200, 400, NULL);//center (200;400) = (0; 0)
    LineTo(hDC, 1000, 400);
    MoveToEx(hDC, 200, 100, NULL);
    LineTo(hDC, 200, 400);

    HDC hDC3 = GetDC(GetConsoleWindow());
    HPEN Pen3 = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    SelectObject(hDC3, Pen3);
    for (int i = 0; i < 9; i++)
    {
        MoveToEx(hDC3, 5000 * mx[i] + 200, -10000000000000000 * (N(mx[i], my, mx, h, n) - my[i]) + 400, NULL);
        LineTo(hDC3, 5000 * mx[i + 1] + 200, -10000000000000000 * (N(mx[i + 1], my, mx, h, n) - my[i + 1]) + 400);

    }
    _getch();

}