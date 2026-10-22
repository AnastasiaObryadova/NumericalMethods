#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <float.h>
#include <Windows.h>
#include <conio.h>

double fact(int n)
{
    return (n < 2) ? 1 : n * fact(n - 1);
}
double F(double x, double y)
{
    //return 0.133 * (x * x + sin(2 * x)) + 0.872 * y;
    return x + cos(y / 3.14);
}
double maxn(double arr[])
{
    double max = 0;
    for (int i = 0; i < 11; i++)
    {
        if (arr[i] > max)
            max = arr[i];
    }
    return max;
}

float int_l(double a, double b, double y)
{
    float h, S, x;
    int i;
    int n = 1000;
    h = (b - a) * 1.0 / n;
    S = 0;
    for (i = 0;i <= n - 1;i++)
    {
        x = a + i * h;
        S = S + F(x, y);
    }
    S = h * S;
    return S;
}

double* pik(double a, double b, double h, double y0)
{
    double n = (b - a) / h;
    double y[11] = {};
    double x[11] = {};
    double out[11] = {};
    y[0] = y0;
    for (int i = 0; i <= n;i++)
    {
        x[i] = a + i * h;
    }
    for (int i = 1; i <= n;i++)
    {
        y[i] = y[0] + int_l(a, x[i], y[i - 1]);
    }
    return y;
}
double* eyl(double a, double b, double h, double y0)
{
    double n = (b - a) / h;
    double y[11] = {};
    y[0] = y0;
    double x[11] = {};
    double dy;
    for (int i = 0; i <= n;i++)
    {
        x[i] = a + i * h;
        dy = h * F(x[i], y[i]);
        if (i < n)
            y[i + 1] = y[i] + dy;
    }
    return y;
}

double* ys_eyl(double a, double b, double h, double y0)
{
    double n = (b - a) / h;
    double y[11] = {};
    y[0] = y0;
    double x[11] = {};
    double xi05, yi05;
    for (int i = 0; i <= n;i++)
    {
        x[i] = a + i * h;
        yi05 = y[i] + h / 2 * F(x[i], y[i]);
        xi05 = x[i] + h / 2;
        if (i < n)
            y[i + 1] = y[i] + h * F(xi05, yi05);
    }
    return y;
}

double* ryn(double a, double b, double h, double y0)
{
    double n = (b - a) / h;
    double y[11] = {};
    double x[11] = {};
    y[0] = y0;
    double k1, k2, k3, k4;
    double dy;
    for (int i = 0; i <= n;i++)
    {
        x[i] = a + i * h;
        k1 = h * F(x[i], y[i]);
        k2 = h * F(x[i] + h / 2, y[i] + k1 / 2);
        k3 = h * F(x[i] + h / 2, y[i] + k2 / 2);
        k4 = h * F(x[i] + h, y[i] + k3);
        dy = (k1 + 2 * k2 + 2 * k3 + k4) / 6;
        if (i < n)
            y[i + 1] = y[i] + dy;
    }
    return y;

}
double* ad(double a, double b, double h, double y0)
{
    double n = (b - a) / h;
    double y[11] = {};
    double x[11] = {};
    double z[10][10] = {};
    double k1, k2, k3, k4;
    double dy, m;
    int p, q, v;
    x[0] = a;
    for (int i = 0; i <= n - 1;i++)
    {
        x[i + 1] = x[i] + h;
    }
    y[0] = y0;
    for (int i = 0; i <= 3 - 1;i++)
    {
        k1 = h * F(x[i], y[i]);
        k2 = h * F(x[i] + h / 2, y[i] + k1 / 2);
        k3 = h * F(x[i] + h / 2, y[i] + k2 / 2);
        k4 = h * F(x[i] + h, y[i] + k3);
        dy = (k1 + 2 * k2 + 2 * k3 + k4) / 6;
        y[i + 1] = y[i] + dy;
    }
    m = 3;
    for (int i = 0; i <= n - 4;i++)
    {
        m = m + 1;
        p = 1;
        q = 0;
        for (int j = i; j <= m;j++)
        {
            q = q + 1;
            z[q][p] = h * F(x[j], y[j]);

        }
        for (v = 4; v >= 2; v--)
        {
            p = p + 1;
            for (int k = 1; k <= v - 1;k++)
            {
                z[k][p] = z[k + 1][p - 1] - z[k][p - 1];
            }
        }
        dy = h * F(x[3 + i], y[i + 3]) + 1 / 2 * z[3][2] + 5 / 12 * z[2][3] + 3 / 8 * z[1][4];
        y[4 + i] = y[3 + i] + dy;
    }
    return y;
}


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    double a = 0.2;
    double b = 1.2;
    double h = 0.1;
    double y0 = 0.25;
    printf("Метод Пикара:\n");
    printf("Метод Эйлера:\n");
    printf("усовершенствованный метод Эйлера:\n");
    printf("Метод Рунге-Кутта:\n");
    printf("Метод Адамса:\n");
    //printf("%f\n", ad(a, b, h, y0));

    //оси
    HDC hDC = GetDC(GetConsoleWindow());
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    SelectObject(hDC, Pen);

    MoveToEx(hDC, 300, 400, NULL);//center (300;400) = (0; 0)
    LineTo(hDC, 1100, 400);
    MoveToEx(hDC, 300, 100, NULL);
    LineTo(hDC, 300, 400);

    HDC hDC4 = GetDC(GetConsoleWindow());
    HPEN Pen4 = CreatePen(PS_SOLID, 2, RGB(0, 255, 255));
    SelectObject(hDC4, Pen4);
    int count = 0;
    for (double i = 0.2; i <= 1.1; i = i + 0.1)
    {
        MoveToEx(hDC4, 100 * i + 300, -100 * pik(a, b, h, y0)[count] + 400, NULL);
        LineTo(hDC4, 100 * (i + 0.1) + 300, -100 * pik(a, b, h, y0)[count + 1] + 400);
        count++;
    }


    HPEN Pen1 = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    SelectObject(hDC4, Pen1);
    count = 0;
    for (double i = 0.2; i <= 1.1; i = i + 0.1)
    {
        MoveToEx(hDC4, 100 * i + 300, -100 * eyl(a, b, h, y0)[count] + 400, NULL);
        LineTo(hDC4, 100 * (i + 0.1) + 300, -100 * eyl(a, b, h, y0)[count + 1] + 400);
        count++;
    }
    HPEN Pen2 = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
    SelectObject(hDC4, Pen2);
    count = 0;
    for (double i = 0.2; i <= 1.1; i = i + 0.1)
    {
        MoveToEx(hDC4, 100 * i + 300, -100 * ys_eyl(a, b, h, y0)[count] + 400, NULL);
        LineTo(hDC4, 100 * (i + 0.1) + 300, -100 * ys_eyl(a, b, h, y0)[count + 1] + 400);
        count++;
    }
    HPEN Pen3 = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
    SelectObject(hDC4, Pen3);
    count = 0;
    for (double i = 0.2; i <= 1.1; i = i + 0.1)
    {
        MoveToEx(hDC4, 100 * i + 300, -100 * ryn(a, b, h, y0)[count] + 400, NULL);
        LineTo(hDC4, 100 * (i + 0.1) + 300, -100 * ryn(a, b, h, y0)[count + 1] + 400);
        count++;
    }
    HPEN Pen5 = CreatePen(PS_SOLID, 2, RGB(255, 255, 0));
    SelectObject(hDC4, Pen5);
    count = 0;
    for (double i = 0.2; i <= 1.1; i = i + 0.1)
    {
        MoveToEx(hDC4, 100 * i + 300, -100 * ad(a, b, h, y0)[count] + 400, NULL);
        LineTo(hDC4, 100 * (i + 0.1) + 300, -100 * ad(a, b, h, y0)[count + 1] + 400);
        count++;
    }
    _getch();
}