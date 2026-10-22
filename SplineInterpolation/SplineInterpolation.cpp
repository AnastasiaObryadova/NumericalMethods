#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <float.h>
#include <Windows.h>
#include <conio.h>

// Количество точек
#define n 6

// Функция для вычисления сплайнов
double Spl(double x, double x_points[], double y_points[])
{
    double h[n - 1], alpha[n - 1], l[n], mu[n], z[n], c[n], b[n], d[n];
    double a[n];
    int i;

    // Вычисление коэффициентов сплайна
    for (i = 0; i < n - 1; i++) {
        h[i] = x_points[i + 1] - x_points[i];
        alpha[i] = (3.0 / h[i]) * (y_points[i + 1] - y_points[i]) - (3.0 / h[i - 1]) * (y_points[i] - y_points[i - 1]);
    }

    l[0] = 1.0;
    mu[0] = z[0] = 0.0;

    for (i = 1; i < n - 1; i++) {
        l[i] = 2.0 * (x_points[i + 1] - x_points[i - 1]) - h[i - 1] * mu[i - 1];
        mu[i] = h[i] / l[i];
        z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
    }

    l[n - 1] = 1.0;
    z[n - 1] = c[n - 1] = 0.0;

    for (i = n - 2; i >= 0; i--) {
        c[i] = z[i] - mu[i] * c[i + 1];
        b[i] = (y_points[i + 1] - y_points[i]) / h[i] - h[i] * (c[i + 1] + 2.0 * c[i]) / 3.0;
        d[i] = (c[i + 1] - c[i]) / (3.0 * h[i]);
        a[i] = y_points[i];
    }

    // Нахождение сегмента, в котором находится x
    int segment = -1;
    for (i = 0; i < n - 1; i++) {
        if (x >= x_points[i] && x <= x_points[i + 1]) {
            segment = i;
            break;
        }
    }

    // Вычисление значения сплайна в точке x
    double dx = x - x_points[segment];
    double y = a[segment] + b[segment] * dx + c[segment] * pow(dx, 2) + d[segment] * pow(dx, 3);

    return y;
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    printf(" Интерполирование сплайнами\n");
    double x[n] = { 0.35, 0.41, 0.47, 0.51, 0.56, 0.64 };
    double y[n] = { 2.73951, 2.3008, 1.96864, 1.78776, 1.59502, 1.3431 };

    double xx = 0.526;
    printf(" f(%f) = %.5f\n", xx, Spl(xx, x, y));

    //оси
    HDC hDC = GetDC(GetConsoleWindow());
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    SelectObject(hDC, Pen);

    MoveToEx(hDC, 300, 400, NULL);//center (300;400) = (0; 0)
    LineTo(hDC, 1100, 400);
    MoveToEx(hDC, 300, 100, NULL);
    LineTo(hDC, 300, 400);

    HDC hDC3 = GetDC(GetConsoleWindow());
    HPEN Pen3 = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
    SelectObject(hDC3, Pen3);
    for (int i = 0; i < 6; i++)
    {
        MoveToEx(hDC3, 100 * x[i] + 300, -100 * y[i] + 401, NULL);
        LineTo(hDC3, 100 * x[i] + 300, -100 * y[i] + 401);

    }

    HDC hDC4 = GetDC(GetConsoleWindow());
    HPEN Pen4 = CreatePen(PS_SOLID, 2, RGB(0, 255, 255));
    SelectObject(hDC4, Pen4);
    for (double i = 0.35; i <= 0.63; i = i + 0.001)
    {
        MoveToEx(hDC4, 100 * i + 300, -100 * Spl(i, x, y) + 400, NULL);
        LineTo(hDC4, 100 * (i + 0.001) + 300, -100 * Spl(i + 0.001, x, y) + 400);

    }
    _getch();
    return 0;
}
