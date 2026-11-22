#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>

#define POINTS_COUNT 8
#define MATRIX_ROWS 2
#define MATRIX_COLS 3

#define MODEL_EXPONENTIAL 1
#define MODEL_HYPERBOLIC 2

// Вычисление значения показательной функции: f(x) = coeffA * exp(coeffB * x)
double CalculateFuncLeastSquares(double targetX, double matrix[][MATRIX_COLS], int freeTermCol, int modelType)
{
    switch (modelType) {
    case MODEL_EXPONENTIAL:
        return matrix[1][freeTermCol] * exp(targetX * matrix[0][freeTermCol]);
    case MODEL_HYPERBOLIC:
        return 1.0 / (matrix[0][freeTermCol] * targetX + matrix[1][freeTermCol]);
    default:
        return 0.0;
    }
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
void DrawGraph(const double* xPoints, const double* yPoints, double matrix[][MATRIX_COLS], int freeTermCol, int choiceFunc)
{
    const int originX = 680;
    const int originY = 400;
    const double scale = 5.0;

    HWND consoleWindow = GetConsoleWindow();
    HDC hDC = GetDC(consoleWindow);

    HPEN axisPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    HPEN pointsPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
    HPEN graphPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 255));

    // Отрисовка осей координат 
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

    double startX = xPoints[0];
    double endX = xPoints[POINTS_COUNT - 1];

    // Отрисовка графика функции
    SelectObject(hDC, graphPen);
    double plotStep = 0.0001;
    for (double currentX = startX; currentX < endX; currentX += plotStep) {
        int currentScreenX = (int)(scale * currentX + originX);
        int currentScreenY = (int)(-scale * CalculateFuncLeastSquares(currentX, matrix, freeTermCol, choiceFunc) + originY);

        int nextScreenX = (int)(scale * (currentX + plotStep) + originX);
        int nextScreenY = (int)(-scale * CalculateFuncLeastSquares(currentX + plotStep, matrix, freeTermCol, choiceFunc) + originY);

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
    
    int choiceFunc = 0;
    printf("   Аппроксимация методом наименьших квадратов (МНК)\n");
    printf("   Выберите вид функции:\n");
    printf("   1 - Показательная (экспоненциальная);\n");
    printf("   2 - Гиперболическая;\n");
    printf("   Введите цифру: ");

    if (scanf_s("%d", &choiceFunc) != 1 || (choiceFunc != MODEL_EXPONENTIAL && choiceFunc != MODEL_HYPERBOLIC)) {
        printf("\nОшибка: введите цифру.\n");
        return 1;
    }

    double xPoints[POINTS_COUNT] = { -0.64 , -0.36 , -0.08 , 0.2 , 0.48 , 0.76 , 1.04 , 1.32 };
    double yPoints[POINTS_COUNT] = { 29.51, 18.86, 12.05, 7.7, 4.92, 3.14, 2.01, 1.28 };
    double linearizedY[POINTS_COUNT];

    // Линеаризация данных под выбранную функцию
    for (int i = 0; i < POINTS_COUNT; i++) {
        switch (choiceFunc) {
        case MODEL_EXPONENTIAL:
            linearizedY[i] = log(yPoints[i]);
            break;
        case MODEL_HYPERBOLIC:
            linearizedY[i] = 1.0 / yPoints[i];
            break;
        }
    }

    // Вычисление сумм для метода наименьших квадратов
    double sumX = 0.0;
    double sumXX = 0.0;
    double sumLinearY = 0.0;
    double sumXLinearY = 0.0;

    for (int i = 0; i < POINTS_COUNT; i++)
    {
        sumX += xPoints[i];
        sumXX += xPoints[i] * xPoints[i];
        sumLinearY += linearizedY[i];
        sumXLinearY += xPoints[i] * linearizedY[i];
    }
    
    // Составление расширенной матрицы и поиск коэффициентов
    double systemMatrix[MATRIX_ROWS][MATRIX_COLS] = {
       { sumXX, sumX, sumXLinearY },
       { sumX, (double)POINTS_COUNT, sumLinearY }
    };
    
    int equationsCount = 2;
    SolveGaussJordan(systemMatrix, equationsCount);

    printf("   Найденное уравнение:\n");
    switch (choiceFunc) {
    case MODEL_EXPONENTIAL:
        systemMatrix[1][equationsCount] = exp(systemMatrix[1][equationsCount]);
        printf("   f(x) = %.6f * e^(%.6f * x)\n\n", systemMatrix[1][equationsCount], systemMatrix[0][equationsCount]);
        break;
    case MODEL_HYPERBOLIC:
        printf("   f(x) = 1 / (%.6f * x + %.6f)\n\n", systemMatrix[0][equationsCount], systemMatrix[1][equationsCount]);
        break;
    }
    
    printf("   Таблица проверки точности:\n");
    printf("     i  |    x    |   y (исх)  |  f(x) (расч) |  Ошибка\n");
    printf("   -----+---------+------------+---------------+----------\n");
    
    // Проверка точности (сумма квадратов отклонений)
    double squaredErrorSum = 0.0;
    for (int i = 0; i < POINTS_COUNT;i++)
    {
        double approxY = CalculateFuncLeastSquares(xPoints[i], systemMatrix, equationsCount, choiceFunc);
        double diff = yPoints[i] - approxY;
        squaredErrorSum += diff * diff;

        printf("     %d  | %7.2f | %10.4f | %13.4f | %+8.4f\n", i, xPoints[i], yPoints[i], approxY, diff);
    }

    printf("\n   Сумма квадратов отклонений: %.6f\n", squaredErrorSum);

    DrawGraph(xPoints, yPoints, systemMatrix, equationsCount, choiceFunc);
    return 0;
}