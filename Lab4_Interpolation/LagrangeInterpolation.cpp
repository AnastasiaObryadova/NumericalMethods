#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <float.h>
#include <Windows.h>
#include <conio.h>

float fx(float x, float* vx, float* vy, int n)
{
    float f = 0;
    float pr = 1;
    for (int i = 0; i <= n; i++)
    {
        pr = 1;
        for (int j = 0; j <= n; j++)
        {
            if (i == j)
                pr = pr * 1;
            else
                pr = pr * (x - vx[j]) / (vx[i] - vx[j]);
        }
        f = f + pr * vy[i];
    }
    return f;
}

float Px(float x, long double XI_1[][6], float* vy, int n)
{
    float a[6] = { 0,0,0,0,0,0 };
    for (int i = 0; i <= n;i++)
    {
        for (int j = 0; j <= n;j++)
        {
            a[i] += XI_1[i][j] * vy[j];
        }
    }
    return a[5] * pow(x, 5) + a[4] * pow(x, 4) + a[3] * pow(x, 3) + a[2] * pow(x, 2) + a[1] * x + a[0];
}
int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    printf(" »нтерпол€ционный полином Ћагранжа\n");
    float vx[6] = { 0.35, 0.41, 0.47, 0.51, 0.56, 0.64 };
    float vy[6] = { 2.73951, 2.30080, 1.96864, 1.78776, 1.59502, 1.34310 };
    float x = 0.526;//искома€ точка
    int n = 5;//показатель степени многочлена
    //находим значение функции в искомой точке
    printf(" f(x) = %f \n", fx(x, vx, vy, n));
    //находим матрицу определенного вида
    float XI[6][6];
    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            XI[j][i] = pow(vx[j], i);
        }
    }

    //находим коэфициенты полинома
    float a[6] = { 0,0,0,0,0,0 };
    //обратна€ матрица
    long double XI_1[6][6] = { {157.85477433612473784515,   -475.93008567231449621873,   468.31025235013981436511,    -0.07917882035162692458, -190.95518132152370983323,  41.79941912792528076629},\
                        {-1249.48505546697159364299, 3965.72836332183913987637,   -4047.68051142185265378457,  0.84971816761005077344,  1715.35457626655505659664,  -384.76709086717999981889},\
                        {3673.71028768741346537448,  -12163.48426755390538207504, 12871.54849646211889142135,  -3.60635160371744591756, -5693.36275727535763317111, 1315.19459228344810436787},\
                        {-4755.06610606783757798484, 16305.30939598335437884241,  -17843.06091277379615542809, 7.56671746840978573287,  8263.55864480191830949191,  -1978.30773941204874065425},\
                        {2286.41506829417351228669,  -8073.0251352700441008915,  9106.386096769976216265,   -7.84888813527408097801, -4416.62337108138126995004, 1104.69622942254972326786},\
                        {-0.19047630553334877281,    1.07694442120513162677,      -3.03832864712541449364,     3.22028504252657552201,  -1.18225342237054354983,    0.11382891129759966749} };

    for (int i = 0; i <= n;i++)
    {
        for (int j = 0; j <= n;j++)
        {
            a[i] += XI_1[i][j] * vy[j];
        }
    }

    printf(" P(x) = %f * x^5 + %f * x^4 + %f * x^3 + %f * x^2 + %f * x + %f\n", a[5], a[4], a[3], a[2], a[1], a[0]);
    printf(" P(x) = %f\n", Px(x, XI_1, vy, n));



    //оси
    HDC hDC = GetDC(GetConsoleWindow());
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    SelectObject(hDC, Pen);

    MoveToEx(hDC, 600, 400, NULL);//center (600;400) = (0; 0)
    LineTo(hDC, 1000, 400);
    MoveToEx(hDC, 600, 100, NULL);
    LineTo(hDC, 600, 400);
    //начальные точки
    HDC hDC2 = GetDC(GetConsoleWindow());
    HPEN Pen2 = CreatePen(PS_SOLID, 7, RGB(255, 0, 0));
    SelectObject(hDC2, Pen2);
    for (int i = 0; i < 6; i++)
    {
        MoveToEx(hDC2, 100 * vx[i] + 600, -100 * vy[i] + 400, NULL);
        LineTo(hDC2, 100 * vx[i] + 600, -100 * vy[i] + 400);
    }
    //fx
    HDC hDC3 = GetDC(GetConsoleWindow());
    HPEN Pen3 = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
    SelectObject(hDC3, Pen3);
    for (float i = 0.35; i <= 0.64; i = i + 0.01)
    {
        MoveToEx(hDC3, 100 * i + 600, -100 * fx(i, vx, vy, n) + 400, NULL);
        LineTo(hDC3, 100 * (i + 0.01) + 600, -100 * fx(i + 0.01, vx, vy, n) + 400);

    }
    //Px
    HDC hDC4 = GetDC(GetConsoleWindow());
    HPEN Pen4 = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
    SelectObject(hDC4, Pen4);
    for (float i = 0.35; i <= 0.64; i = i + 0.01)
    {
        MoveToEx(hDC4, 100 * i + 600, -100 * Px(i, XI_1, vy, n) + 400, NULL);
        LineTo(hDC4, 100 * (i + 0.01) + 600, -100 * Px(i, XI_1, vy, n) + 400);

    }
    _getch();

}