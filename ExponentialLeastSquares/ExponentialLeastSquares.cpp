#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>

#define POINTS_COUNT 8
#define MATRIX_ROWS 2
#define MATRIX_COLS 3

// Вычисление значения показательной функции: f(x) = coeffA * exp(coeffB * x)
double CalculateExpLeastSquares(double targetX, double matrix[][MATRIX_COLS], int freeTermCol)
{
    return matrix[1][freeTermCol] * exp(targetX * matrix[0][freeTermCol]);
}

// Решение системы линейных уравнений методом Гаусса-Жордана
void SolveGaussJordan(double matrix[][MATRIX_COLS], int equationsCount)
{
    for (int i = 1; i <= equationsCount; i++) {
        for (int j = equationsCount + 1; j >= i; j--) {
            matrix[i - 1][j - 1] /= matrix[i - 1][i - 1];
        }
        for (int k = 1; k <= equationsCount; k++) {
            if (k != i) {
                for (int j = equationsCount + 1; j >= 1; j--) {
                    matrix[k - 1][j - 1] -= matrix[k - 1][i - 1] * matrix[i - 1][j - 1];
                }
            }
        }
    }
}

// Отрисовка осей, исходных точек и графика аппроксимирующей функции
void DrawGraph(const double* xPoints, const double* yPoints, double matrix[][MATRIX_COLS], int freeTermCol)
{
    const int originX = 680;
    const int originY = 400;
    const double scale = 5.0;

    HWND consoleWindow = GetConsoleWindow();
    HDC hDC = GetDC(consoleWindow);

    HPEN axisPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    HPEN pointsPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
    HPEN graphPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 255));

    // Отрисовка осей координат (центр 400, 400)
    SelectObject(hDC, axisPen);
    MoveToEx(hDC, originX, originY, NULL);
    LineTo(hDC, 1000, originY);// Ось X
    MoveToEx(hDC, originX, 100, NULL);
    LineTo(hDC, originX, originY);// Ось Y

    // Отрисовка исходных точек
    SelectObject(hDC, pointsPen);
    for (int i = 0; i < POINTS_COUNT; i++) {
        int screenX = (int)(scale * xPoints[i] + originX);
        int screenY = (int)(-scale * yPoints[i] + originY + 1);

        MoveToEx(hDC, screenX, screenY, NULL);
        LineTo(hDC, screenX, screenY);
    }

    // Отрисовка графика показательной функции
    SelectObject(hDC, graphPen);
    double plotStep = 0.0001;
    for (double currentX = -2.0; currentX < 6.0; currentX += plotStep) {
        int currentScreenX = (int)(scale * currentX + originX);
        int currentScreenY = (int)(-scale * CalculateExpLeastSquares(currentX, matrix, freeTermCol) + originY);

        int nextScreenX = (int)(scale * (currentX + plotStep) + originX);
        int nextScreenY = (int)(-scale * CalculateExpLeastSquares(currentX + plotStep, matrix, freeTermCol) + originY);

        MoveToEx(hDC, currentScreenX, currentScreenY, NULL);
        LineTo(hDC, nextScreenX, nextScreenY);
    }

    ReleaseDC(consoleWindow, hDC);
    DeleteObject(axisPen);
    DeleteObject(pointsPen);
    DeleteObject(graphPen);
    (void)_getch();
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    
    double xPoints[POINTS_COUNT] = { -0.64 , -0.36 , -0.08 , 0.2 , 0.48 , 0.76 , 1.04 , 1.32 };
    double yPoints[POINTS_COUNT] = { 29.51, 18.86, 12.05, 7.7, 4.92, 3.14, 2.01, 1.28 };
    double logYPoints[POINTS_COUNT];
    for (int i = 0; i < POINTS_COUNT; i++) {
        logYPoints[i] = log(yPoints[i]);
    }

    // Вычисление сумм для метода наименьших квадратов
    double sumX = 0.0;
    double sumXX = 0.0;
    double sumLogY = 0.0;
    double sumXLogY = 0.0;

    for (int i = 0; i < POINTS_COUNT; i++)
    {
        sumX += xPoints[i];
        sumXX += xPoints[i] * xPoints[i];
        sumLogY += logYPoints[i];
        sumXLogY += xPoints[i] * logYPoints[i];
    }
    
    // Составление расширенной матрицы и поиск коэффициентов
    double systemMatrix[MATRIX_ROWS][MATRIX_COLS] = {
       { sumXX, sumX, sumXLogY },
       { sumX, (double)POINTS_COUNT, sumLogY }
    };
    
    int equationsCount = 2;
    SolveGaussJordan(systemMatrix, equationsCount);
    systemMatrix[1][equationsCount] = exp(systemMatrix[1][equationsCount]);

    printf("   Аппроксимация показательной функцией (МНК)\n\n");
    printf("   Найденное уравнение:\n");
    printf("   f(x) = %f * e ^ (%f * x)\n", systemMatrix[1][equationsCount], systemMatrix[0][equationsCount]);
    printf("   Таблица проверки точности:\n");
    printf("     i  |    x    |   y (исх)  |  f(x) (расч) |  Ошибка\n");
    printf("   -----+---------+------------+---------------+----------\n");
    
    // Проверка точности (сумма квадратов отклонений)
    double squaredErrorSum = 0.0;
    for (int i = 0; i < POINTS_COUNT;i++)
    {
        double approxY = CalculateExpLeastSquares(xPoints[i], systemMatrix, equationsCount);
        double diff = yPoints[i] - approxY;
        squaredErrorSum += diff * diff;

        printf("     %d  | %7.2f | %10.4f | %13.4f | %+8.4f\n", i, xPoints[i], yPoints[i], approxY, diff);
    }
    printf("\n   Сумма квадратов отклонений: %.6f\n", squaredErrorSum);

    DrawGraph(xPoints, yPoints, systemMatrix, equationsCount);
    return 0;
}