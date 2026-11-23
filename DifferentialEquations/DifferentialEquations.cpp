#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>

#define POINTS_COUNT 11

// Правая часть дифференциального уравнения: y' = f(x, y)
double EvaluateODE(double x, double y)
{
    return x + cos(y / 3.14);
}

// Интегрирование методом левых прямоугольников для метода Пикара
double IntegrateLeftRectangles(double lowerBorder, double upperBorder, double fixedY)
{
    int stepsCount = 1000;
    double step = (upperBorder - lowerBorder) / stepsCount;
    double sum = 0.0;

    for (int i = 0; i < stepsCount; i++) {
        double currentX = lowerBorder + i * step;
        sum += EvaluateODE(currentX, fixedY);
    }

    return step * sum;
}

// Метод последовательных приближений Пикара
void SolvePicard(double startX, double endX, double step, double initialY, double* yValues)
{
    int stepsCount = (int)round((endX - startX) / step);
    double xValues[POINTS_COUNT] = {};
    yValues[0] = initialY;

    for (int i = 0; i <= stepsCount;i++)
    {
        xValues[i] = startX + i * step;
    }
    for (int i = 1; i <= stepsCount;i++)
    {
        yValues[i] = yValues[0] + IntegrateLeftRectangles(startX, xValues[i], yValues[i - 1]);
    }
}

// Метод Эйлера
void SolveEuler(double startX, double endX, double step, double initialY, double* yValues)
{
    int stepsCount = (int)round((endX - startX) / step);
    yValues[0] = initialY;
    double deltaY;
    for (int i = 0; i < stepsCount;i++)
    {
        double currentX = startX + i * step;
        deltaY = step * EvaluateODE(currentX, yValues[i]);
        yValues[i + 1] = yValues[i] + deltaY;
    }
}

//Усовершенствованный метод Эйлера
void SolveImprovedEuler(double startX, double endX, double step, double initialY, double* yValues)
{
    int stepsCount = (int)round((endX - startX) / step);
    yValues[0] = initialY;

    for (int i = 0; i < stepsCount;i++)
    {
        double currentX = startX + i * step;
        double halfY = yValues[i] + step / 2 * EvaluateODE(currentX, yValues[i]);
        double halfX = currentX + step / 2;
        yValues[i + 1] = yValues[i] + step * EvaluateODE(halfX, halfY);
    }
}

//Метод Рунге-Кутта 4 - го порядка
void SolveRungeKutta(double startX, double endX, double step, double initialY, double* yValues)
{
    int stepsCount = (int)round((endX - startX) / step);
    yValues[0] = initialY;

    for (int i = 0; i < stepsCount;i++)
    {
        double currentX = startX + i * step;
        double k1 = step * EvaluateODE(currentX, yValues[i]);
        double k2 = step * EvaluateODE(currentX + step / 2, yValues[i] + k1 / 2);
        double k3 = step * EvaluateODE(currentX + step / 2, yValues[i] + k2 / 2);
        double k4 = step * EvaluateODE(currentX + step, yValues[i] + k3);
        double deltaY = (k1 + 2 * k2 + 2 * k3 + k4) / 6;
        yValues[i + 1] = yValues[i] + deltaY;
    }

}

// Метод Адамса
void SolveAdams(double startX, double endX, double step, double initialY, double* yValues)
{
    int stepsCount = (int)round((endX - startX) / step);
    double xValues[POINTS_COUNT] = { 0.0 };
    double diffTable[10][10] = { 0.0 };
    
    xValues[0] = startX;
    for (int i = 0; i <= stepsCount - 1;i++)
    {
        xValues[i + 1] = xValues[i] + step;
    }

    // Первые 3 шага методом Рунге-Кутта
    yValues[0] = initialY;
    for (int i = 0; i <= 3 - 1;i++)
    {
        double k1 = step * EvaluateODE(xValues[i], yValues[i]);
        double k2 = step * EvaluateODE(xValues[i] + step / 2, yValues[i] + k1 / 2);
        double k3 = step * EvaluateODE(xValues[i] + step / 2, yValues[i] + k2 / 2);
        double k4 = step * EvaluateODE(xValues[i] + step, yValues[i] + k3);
        
        yValues[i + 1] = yValues[i] + (k1 + 2 * k2 + 2 * k3 + k4) / 6;
    }

    // Основной участок
    int endIndex = 3;
    for (int i = 0; i <= stepsCount - 4;i++)
    {
        endIndex++;
        int col = 1;
        int row = 0;
        for (int j = i; j <= endIndex;j++)
        {
            row = row + 1;
            diffTable[row][col] = step * EvaluateODE(xValues[j], yValues[j]);

        }

        for (int order = 4; order >= 2; order--)
        {
            col = col + 1;
            for (int k = 1; k <= order - 1;k++)
            {
                diffTable[k][col] = diffTable[k + 1][col - 1] - diffTable[k][col - 1];
            }
        }
        double deltaY = step * EvaluateODE(xValues[3 + i], yValues[i + 3]) +
            (1.0 / 2.0) * diffTable[3][2] +
            (5.0 / 12.0) * diffTable[2][3] +
            (3.0 / 8.0) * diffTable[1][4];
        yValues[4 + i] = yValues[3 + i] + deltaY;
    }
}

// Вспомогательная функция отрисовки одной кривой заданным цветом
void DrawCurve(HDC hDC, double startX, double step, int stepsCount, const double* yValues, COLORREF color, int originX, int originY, double scale)
{
    HPEN curvePen = CreatePen(PS_SOLID, 2, color);
    HPEN oldPen = (HPEN)SelectObject(hDC, curvePen);

    for (int i = 0; i < stepsCount; i++) {
        double currentX = startX + i * step;
        double nextX = currentX + step;

        int screenX1 = (int)(scale * currentX + originX);
        int screenY1 = (int)(-scale * yValues[i] + originY);

        int screenX2 = (int)(scale * nextX + originX);
        int screenY2 = (int)(-scale * yValues[i + 1] + originY);

        MoveToEx(hDC, screenX1, screenY1, NULL);
        LineTo(hDC, screenX2, screenY2);
    }

    SelectObject(hDC, oldPen);
    DeleteObject(curvePen);
}

// Отрисовка осей и всех 5 графиков
void DrawAllGraphs(double startX, double step, int stepsCount,
    const double* yPicard, const double* yEuler,
    const double* yImprovedEuler, const double* yRungeKutta, const double* yAdams)
{
    const int originX = 680;
    const int originY = 400;
    const double scale = 100.0;

    HWND consoleWindow = GetConsoleWindow();
    HDC hDC = GetDC(consoleWindow);

    HPEN axisPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    HPEN oldPen = (HPEN)SelectObject(hDC, axisPen);

    // Отрисовка осей координат
    MoveToEx(hDC, originX, originY, NULL);
    LineTo(hDC, originX + 400, originY); // Ось X
    MoveToEx(hDC, originX, 100, NULL);
    LineTo(hDC, originX, originY);       // Ось Y

    SelectObject(hDC, oldPen);
    DeleteObject(axisPen);

    // Отрисовка кривых разными цветами
    DrawCurve(hDC, startX, step, stepsCount, yPicard, RGB(0, 255, 255), originX, originY, scale); // Голубой
    DrawCurve(hDC, startX, step, stepsCount, yEuler, RGB(255, 0, 0), originX, originY, scale); // Красный
    DrawCurve(hDC, startX, step, stepsCount, yImprovedEuler, RGB(0, 255, 0), originX, originY, scale); // Зеленый
    DrawCurve(hDC, startX, step, stepsCount, yRungeKutta, RGB(0, 0, 255), originX, originY, scale); // Синий
    DrawCurve(hDC, startX, step, stepsCount, yAdams, RGB(255, 255, 0), originX, originY, scale); // Желтый

    ReleaseDC(consoleWindow, hDC);
    (void)_getch();
}

int main()
{
    SetConsoleOutputCP(1251);
    const double startX = 0.2;
    const double endX = 1.2;
    const double step = 0.1;
    const double initialY = 0.25;
    const int stepsCount = (int)round((endX - startX) / step);

    double yPicard[POINTS_COUNT] = { 0.0 };
    double yEuler[POINTS_COUNT] = { 0.0 };
    double yImprovedEuler[POINTS_COUNT] = { 0.0 };
    double yRungeKutta[POINTS_COUNT] = { 0.0 };
    double yAdams[POINTS_COUNT] = { 0.0 };

    SolvePicard(startX, endX, step, initialY, yPicard);
    SolveEuler(startX, endX, step, initialY, yEuler);
    SolveImprovedEuler(startX, endX, step, initialY, yImprovedEuler);
    SolveRungeKutta(startX, endX, step, initialY, yRungeKutta);
    SolveAdams(startX, endX, step, initialY, yAdams);

    printf("   Численное решение ОДУ   \n\n");
    printf("     x   |  Пикар  |  Эйлер  | Усов.Эйл| Рунге-К |  Адамс  \n");
    printf("   ------+---------+---------+---------+---------+---------\n");
    for (int i = 0; i <= stepsCount; i++) {
        printf("   %5.2f | %7.4f | %7.4f | %7.4f | %7.4f | %7.4f\n",
            startX + i * step, yPicard[i], yEuler[i], yImprovedEuler[i], yRungeKutta[i], yAdams[i]);
    }

    printf("\n   Цвета графиков:\n");
    printf("   Голубой - Метод Пикара\n");
    printf("   Красный - Метод Эйлера\n");
    printf("   Зеленый - Усовершенствованный метод Эйлера\n");
    printf("   Синий   - Метод Рунге-Кутты\n");
    printf("   Желтый  - Метод Адамса\n");

    DrawAllGraphs(startX, step, stepsCount, yPicard, yEuler, yImprovedEuler, yRungeKutta, yAdams);
    return 0;

}