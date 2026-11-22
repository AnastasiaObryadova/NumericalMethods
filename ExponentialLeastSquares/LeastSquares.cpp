#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>

#define POINTS_COUNT 8
#define MATRIX_ROWS 3
#define MATRIX_COLS 4

#define MODEL_EXPONENTIAL 1
#define MODEL_HYPERBOLIC 2
#define MODEL_LINEAR 3
#define MODEL_LOGARITHMIC 4
#define MODEL_POWER 5
#define MODEL_QUADRATIC 6

// Вычисление значения функции
double CalculateFuncLeastSquares(double targetX, double matrix[][MATRIX_COLS], int freeTermCol, int modelType)
{
    switch (modelType) {
    case MODEL_EXPONENTIAL:
        return matrix[1][freeTermCol] * exp(targetX * matrix[0][freeTermCol]);
    case MODEL_HYPERBOLIC:
        return 1.0 / (matrix[0][freeTermCol] * targetX + matrix[1][freeTermCol]);
    case MODEL_LINEAR:
        return matrix[0][freeTermCol] * targetX + matrix[1][freeTermCol];
    case MODEL_LOGARITHMIC:
        return matrix[0][freeTermCol] * log(targetX) + matrix[1][freeTermCol];
    case MODEL_POWER:
        return matrix[1][freeTermCol] * pow(targetX, matrix[0][freeTermCol]);
    case MODEL_QUADRATIC:
        return matrix[0][freeTermCol] * targetX * targetX + matrix[1][freeTermCol] * targetX + matrix[2][freeTermCol];
    default:
        return 0.0;
    }
}

double CalculateCorrelationLin(double* xPoints, double* yPoints, double sumX, double sumY)
{
    double covarianceSum = 0.0;
    double varianceXSum = 0.0;
    double varianceYSum = 0.0;

    for (int i = 0; i < POINTS_COUNT; i++) {
        covarianceSum += (xPoints[i] - (1.0 / POINTS_COUNT) * sumX) * (yPoints[i] - (1.0 / POINTS_COUNT) * sumY);
        varianceXSum += (xPoints[i] - (1.0 / POINTS_COUNT) * sumX) * (xPoints[i] - (1.0 / POINTS_COUNT) * sumX);
        varianceYSum += (yPoints[i] - (1.0 / POINTS_COUNT) * sumY) * (yPoints[i] - (1.0 / POINTS_COUNT) * sumY);
    }

    return covarianceSum / sqrt(varianceXSum * varianceYSum);
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
    printf("   3 - Линейная;\n");
    printf("   4 - Логарифмическая;\n");
    printf("   5 - Степенная;\n");
    printf("   6 - Квадратичная;\n");
    printf("   Введите цифру: ");
    if (scanf_s("%d", &choiceFunc) != 1 || choiceFunc < MODEL_EXPONENTIAL || choiceFunc > MODEL_QUADRATIC) {
        printf("\n   Ошибка: введите цифру от 1 до 6.\n");
        return 1;
    }

    double xPoints[POINTS_COUNT] = { -0.64 , -0.36 , -0.08 , 0.2 , 0.48 , 0.76 , 1.04 , 1.32 };
    double yPoints[POINTS_COUNT] = { 29.51, 18.86, 12.05, 7.7, 4.92, 3.14, 2.01, 1.28 };
    double linearizedX[POINTS_COUNT];
    double linearizedY[POINTS_COUNT];

    // Линеаризация данных под выбранную функцию
    for (int i = 0; i < POINTS_COUNT; i++) {
        switch (choiceFunc) {
        case MODEL_EXPONENTIAL:
            linearizedX[i] = xPoints[i];
            linearizedY[i] = log(yPoints[i]);
            break;
        case MODEL_HYPERBOLIC:
            linearizedX[i] = xPoints[i];
            linearizedY[i] = 1.0 / yPoints[i];
            break;
        case MODEL_LINEAR:
            linearizedX[i] = xPoints[i];
            linearizedY[i] = yPoints[i];
            break;
        case MODEL_LOGARITHMIC:
            xPoints[i] += 1.0;//важно для вычисления отклонений
            linearizedX[i] = log(xPoints[i]);
            linearizedY[i] = yPoints[i];
            break;
        case MODEL_POWER:
            xPoints[i] += 1.0;//важно для вычисления отклонений
            linearizedX[i] = log(xPoints[i]);
            linearizedY[i] = log(yPoints[i]);
            break;
        case MODEL_QUADRATIC:
            linearizedX[i] = xPoints[i];
            linearizedY[i] = yPoints[i];
            break;
        }
    }

    // Вычисление сумм для метода наименьших квадратов
    double sumX = 0.0, sumX2 = 0.0, sumX3 = 0.0, sumX4 = 0.0;
    double sumLinearY = 0.0, sumXLinearY = 0.0, sumX2LinearY = 0.0;

    for (int i = 0; i < POINTS_COUNT; i++) {
        double x = linearizedX[i];
        double x2 = x * x;

        sumX += x;
        sumX2 += x2;
        sumX3 += x2 * x;
        sumX4 += x2 * x2;
        sumLinearY += linearizedY[i];
        sumXLinearY += x * linearizedY[i];
        sumX2LinearY += x2 * linearizedY[i];
    }

    // Составление расширенной матрицы (3x4 для квадратичной, 2x3 для остальных)
    double systemMatrix[MATRIX_ROWS][MATRIX_COLS] = { 0.0 };
    int equationsCount = (choiceFunc == MODEL_QUADRATIC) ? 3 : 2;

    if (choiceFunc == MODEL_QUADRATIC) {
        systemMatrix[0][0] = sumX4; systemMatrix[0][1] = sumX3; systemMatrix[0][2] = sumX2;                 systemMatrix[0][3] = sumX2LinearY;
        systemMatrix[1][0] = sumX3; systemMatrix[1][1] = sumX2; systemMatrix[1][2] = sumX;                  systemMatrix[1][3] = sumXLinearY;
        systemMatrix[2][0] = sumX2; systemMatrix[2][1] = sumX;  systemMatrix[2][2] = (double)POINTS_COUNT;  systemMatrix[2][3] = sumLinearY;
    }
    else {
        systemMatrix[0][0] = sumX2; systemMatrix[0][1] = sumX;                  systemMatrix[0][2] = sumXLinearY;
        systemMatrix[1][0] = sumX;  systemMatrix[1][1] = (double)POINTS_COUNT;  systemMatrix[1][2] = sumLinearY;
    }

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
    case MODEL_LINEAR:
        printf("   f(x) = %.6f * x + %.6f\n", systemMatrix[0][equationsCount], systemMatrix[1][equationsCount]);
        printf("   Коэффициент корреляции = %f\n\n", CalculateCorrelationLin(xPoints, yPoints, sumX, sumLinearY));
        break;
    case MODEL_LOGARITHMIC:
        printf("   f(x) = %.6f * ln(x) + %.6f\n\n", systemMatrix[0][equationsCount], systemMatrix[1][equationsCount]);
        break;
    case MODEL_POWER:
        systemMatrix[1][equationsCount] = exp(systemMatrix[1][equationsCount]);
        printf("   f(x) = %.6f * x ^ %.6f\n\n", systemMatrix[1][equationsCount], systemMatrix[0][equationsCount]);
        break;
    case MODEL_QUADRATIC:
        printf("   f(x) = %.6f * x^2 + %.6f * x + %.6f\n\n", systemMatrix[0][equationsCount], systemMatrix[1][equationsCount], systemMatrix[2][equationsCount]);
        break;
    }
    
    printf("   Таблица проверки точности:\n");
    printf("     i  |    x    |   y (исх)  |  f(x) (расч)  |  Ошибка\n");
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