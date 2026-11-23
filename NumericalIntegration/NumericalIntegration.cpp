#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>

// Генерация случайного действительного числа в диапазоне [minVal, maxVal]
double GenerateRandomDouble(double minVal, double maxVal)
{
    return (double)(rand()) / RAND_MAX * (maxVal - minVal) + minVal;
}

// Подынтегральная функция: f(x) = -15 / (2 - x)^3
double EvaluateFunction(double x)
{
    return -15 / pow((2 - x), 3);
}

// Метод левых прямоугольников
double IntegrateLeftRectangles(double lowerBorder, double upperBorder, int stepsCount)
{
    double step = (upperBorder - lowerBorder) * 1.0 / stepsCount;
    double sum = 0.0;
    for (int i = 0; i <= stepsCount - 1; i++)
    {
        sum = sum + EvaluateFunction(lowerBorder + i * step);
    }
    return step * sum;
}

// Метод правых прямоугольников
double IntegrateRightRectangles(double lowerBorder, double upperBorder, int stepsCount)
{
    double step = (upperBorder - lowerBorder) * 1.0 / stepsCount;
    double sum = 0.0;
    for (int i = 1;i <= stepsCount;i++)
    {
        sum += EvaluateFunction(lowerBorder + i * step);
    }
    return step * sum;
}

// Метод Симпсона
double IntegrateSimpson(double lowerBorder, double upperBorder, int stepsCount)
{
    double step = (upperBorder - lowerBorder) / stepsCount;
    double oddSum = 0.0;
    double evenSum = 0.0;

    for (int i = 1; i <= stepsCount; i++)
    {
        if (i % 2 != 0)
            oddSum = oddSum + EvaluateFunction(lowerBorder + i * step);
        else
            evenSum = evenSum + EvaluateFunction(lowerBorder + i * step);
    }
    return (((EvaluateFunction(lowerBorder) - EvaluateFunction(upperBorder)) / 2 + 2 * oddSum + evenSum) / 3) * 2 * step;
}

// Метод трапеций
double IntegrateTrapezoidal(double lowerBorder, double upperBorder, int stepsCount)
{
    double sum = 0.0;
    double step = (upperBorder - lowerBorder) * 1.0 / stepsCount;

    for (int i = 1;i <= stepsCount - 1;i++)
    {
        sum = sum + EvaluateFunction(lowerBorder + i * step);
    }
    return (sum + (EvaluateFunction(lowerBorder) + EvaluateFunction(upperBorder)) / 2) * step;
}

// Метод Монте-Карло
double IntegrateMonteCarlo(double lowerBorder, double upperBorder, int stepsCount)
{
    double sum = 0.0;
    double step = (upperBorder - lowerBorder) * 1.0 / stepsCount;
    for (int i = 0;i <= stepsCount;i++)
    {
        sum = sum + EvaluateFunction(lowerBorder + (upperBorder - lowerBorder) *
            GenerateRandomDouble(0.0, 1.0));
    }
    return sum * step;
}

int main()
{
    SetConsoleOutputCP(1251);
    double lowerBorder = 3.0;
    double upperBorder = 5.0;
    int stepsCount = 1000000;
    
    printf("   Численное интегрирование   \n");
    printf("   Отрезок: [%.1f, %.1f], число разбиений = %d\n\n", lowerBorder, upperBorder, stepsCount);

    printf("   Метод левых прямоугольников: %f\n", IntegrateLeftRectangles(lowerBorder, upperBorder, stepsCount));
    printf("   Метод правых прямоугольников: %f\n", IntegrateRightRectangles(lowerBorder, upperBorder, stepsCount));
    printf("   Метод Симпсона: %f\n", IntegrateSimpson(lowerBorder, upperBorder, stepsCount));
    printf("   Метод трапеций: %f\n", IntegrateTrapezoidal(lowerBorder, upperBorder, stepsCount));
    printf("   Метод Монте-Карло: %f\n", IntegrateMonteCarlo(lowerBorder, upperBorder, stepsCount));
    (void)_getch();
}
