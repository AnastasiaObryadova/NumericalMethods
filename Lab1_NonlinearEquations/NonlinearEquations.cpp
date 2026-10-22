#include <Windows.h>
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <conio.h>
#include <stdio.h>
float func(float t)
{  // функция
    return cos(t + 0.5) - pow(t, 3);
}
float prfunc(float t)
{  // функция
    return -sin(t + 0.5) - 3 * pow(t, 2);
}

int main(void)
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    printf("   f(x) = cos(x + 0.5) - x^3 \n");
    printf("   Интервал уточнения корня - [-1 ; 2]\n");
    float x;
    printf("   1 - метод дихотомии \n");
    printf("   2 - метод простых итераций \n");
    printf("   3 - хорд \n");
    printf("   4 - касательнфх \n");
    printf("   5 - хорд и касательных \n");
    printf("   Выберите метод: ");
    int var;
    scanf_s("%d", &var);
    if (var == 1) //половинчатое деление
    {
        float a = -1;
        float b = 2;
        float c, x;
        float e = 0.0001;
        while (b - a > 2 * e)
        {
            c = (a + b) / 2;
            if (func(a) * func(c) < 0)
                b = c;
            else a = c;

        }
        x = (a + b) / 2;

        printf("   Полученный корень = %f\n", x);
    }
    else if (var == 2) //простые итерации
    {
        float a = -1;
        float b = 2;
        float e = 0.0001;
        float h = (b - a) / 100;
        float x0 = (a + b) / 2;
        float x = a;
        float y = a + h;
        float M = fabs(func(y) - func(x)) / h;
        float c, x1;
        while (y < b)
        {
            x = y;
            y = x + h;
            c = fabs(func(y) - func(x)) / h;
            if (c > M)
                M = c;
        }
        M = M + 1;
        if (func(a) < func(b))
            c = 1;
        else
            c = -1;
        x1 = x0 - c * func(x0) / M;
        while (fabs(x1 - x0) > e)
        {
            x0 = x1;
            x1 = x0 - c * func(x0) / M;
        }
        x = x1;
        printf("   Полученный корень = %f\n", x);
    }
    else if (var == 3)//хорд
    {
        float x0, x1, c;
        float a = -1;
        float b = 2;
        x0 = a;
        c = b;
        float e = 0.0001;
        x1 = x0 - (c - x0) * func(x0) / (func(c) - func(x0));
        while (abs(func(x1)) > e)
        {
            x1 = x0 - (c - x0) * func(x0) / (func(c) - func(x0));
            float y = func(x0);
            float d = func(x1);
            if (y * d < 0) {
                c = x1;
            }
            else {
                x0 = x1;
            }
        }
        printf("   Полученный корень = %f\n", x1);
    }
    else if (var == 4)//касательных
    {
        float x0, x1, c;
        float a = -1;
        float b = 2;
        float e = 0.0001;
        float h = (b - a) / 100;
        float x = func(a);
        float y = func(a + 2 * h) - 2 * func(a + h) + func(a);
        if (x * y > 0)
            x0 = a;
        else
            x0 = b;
        x1 = x0 - func(x0) / prfunc(x0);
        while (fabs(x1 - x0) > e)
        {
            x0 = x1;
            x1 = x0 - func(x0) / prfunc(x0);
        }
        x = x1;
        printf("   Полученный корень = %f\n", x);
    }
    else if (var == 5)//хорд и касательных
    {
        float x0, x1, x2, x3, c;
        float a = -1;
        float b = 2;
        float e = 0.0001;
        float h = (b - a) / 100;
        float x = func(a);
        float y = func(a + 2 * h) - 2 * func(a + h) + func(a);
        if (x * y < 0)
        {
            x0 = a;
            x1 = b;
            c = b;
        }
        else
        {
            x0 = b;
            x1 = a;
            c = a;
        }
        x2 = x0 - (c - x0) * func(x0) / (func(c) - func(x0));
        x3 = x1 - func(x1) / prfunc(x1);
        while (fabs(x3 - x2) > 2 * e)
        {
            x3 = x1 - func(x1) / prfunc(x1);
            x2 = x0 - (c - x0) * func(x0) / (func(c) - func(x0));
            if (func(x0) * func(x1) < 0)
                c = x2;
            else
                x0 = x2;
            x1 = x3;
        }
        x = (x2 + x3) / 2;
        printf("   Полученный корень = %f\n", x);

    }

    HDC hDC = GetDC(GetConsoleWindow());
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    SelectObject(hDC, Pen);

    MoveToEx(hDC, 400, 285, NULL);
    LineTo(hDC, 800, 285);
    MoveToEx(hDC, 600, 100, NULL);
    LineTo(hDC, 600, 470);

    MoveToEx(hDC, 700, 275, NULL);
    LineTo(hDC, 700, 295);

    MoveToEx(hDC, 550, 275, NULL);
    LineTo(hDC, 550, 295);

    for (x = -8.0f; x <= 8.0f; x += 0.01f) // O(600,285) - center  
    {
        MoveToEx(hDC, 50 * x + 600, -50 * (cos(x + 0.5) - pow(x, 3)) + 285, NULL);//10 - scale
        LineTo(hDC, 50 * x + 600, -50 * (cos(x + 0.5) - pow(x, 3)) + 285);
    }
    _getch();
    return 0;
}