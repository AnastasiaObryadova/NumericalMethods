#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
//#include <cmath>
#include <stdio.h>
//#include <iostream>
#include <float.h>
//#include <Windows.h>
#include <conio.h>
// Функция, генерирующая случайное действительное число от min до max
double rando(double min, double max)
{
    return (double)(rand()) / RAND_MAX * (max - min) + min;
}
double F(double x)
{
    return -15 / pow((2 - x), 3);
}

float int_l(int a, int b, int n)
{
    float h, S, x;
    int i;
    h = (b - a) * 1.0 / n;
    S = 0;
    for (i = 0;i <= n - 1;i++)
    {
        x = a + i * h;
        S = S + F(x);
    }
    S = h * S;
    return S;
}
float int_p(int a, int b, int n)
{
    float h, S, x;
    int i;
    h = (b - a) * 1.0 / n;
    S = 0;
    for (i = 1;i <= n;i++)
    {
        x = a + i * h;
        S = S + F(x);
    }
    S = h * S;
    return S;
}
float sim(int a, int b, int n)
{
    float h;
    h = (b - a) * 1.0 / n;
    float s1 = 0;
    float s2 = 0;
    for (int i = 1;i <= n;i++)
    {
        if (i % 2 != 0)
            s1 = s1 + F(a + i * h);
        else
            s2 = s2 + F(a + i * h);
    }
    return (((F(a) - F(b)) / 2 + 2 * s1 + s2) / 3) * 2 * h;
}
float trap(int a, int b, int n)
{
    float h;
    float s = 0;
    h = (b - a) * 1.0 / n;

    for (int i = 1;i <= n - 1;i++)
    {
        s = s + F(a + i * h);
    }
    return (s + (F(a) + F(b)) / 2) * h;
}
float mon(int a, int b, int n)
{
    n = 1000000;
    float h, x;
    float s = 0;
    h = (b - a) * 1.0 / n;
    for (int i = 0;i <= n;i++)
    {
        x = a + (b - a) * rando(0.0, 1.0);

        s = s + F(x);
    }
    return s * h;
}
int main()
{
    //SetConsoleCP(1251);
    //SetConsoleOutputCP(1251);
    int a = 3;
    int b = 5;
    int n = 1000;
    int left = 1;//= 1 левые прямоугольники 0 - правые
    //double dy = diff(x, F);
    printf("Метод левых прямоугольников: %f\n", int_l(a, b, n));
    printf("Метод правых прямоугольников: %f\n", int_p(a, b, n));
    printf("Метод Симпсона: %f\n", sim(a, b, n));
    printf("Метод трапеций: %f\n", trap(a, b, n));
    printf("Метод Монте-Карло: %f\n", mon(a, b, n));
    printf("%d", rand() % (1 - 0 + 1) + 0);





}
