#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>

#define POINTS_COUNT 6

// Вычисление значения кубического сплайна в заданной точке
double CalculateSpline(double targetX, double xPoints[], double yPoints[])
{
    double step[POINTS_COUNT - 1];
    double alpha[POINTS_COUNT - 1] = { 0.0 };
    double l[POINTS_COUNT];
    double mu[POINTS_COUNT];
    double z[POINTS_COUNT];

    double coeffC[POINTS_COUNT];
    double coeffB[POINTS_COUNT];
    double coeffD[POINTS_COUNT];
    double coeffA[POINTS_COUNT];

    // Вычисление шагов и вспомогательных коэффициентов
    for (int i = 0; i < POINTS_COUNT - 1; i++) {
        step[i] = xPoints[i + 1] - xPoints[i];
    }
    for (int i = 1; i < POINTS_COUNT - 1; i++) {
        alpha[i] = (3.0 / step[i]) * (yPoints[i + 1] - yPoints[i]) -
            (3.0 / step[i - 1]) * (yPoints[i] - yPoints[i - 1]);
    }

    // Прямой ход метода прогонки
    l[0] = 1.0;
    mu[0] = 0.0;
    z[0] = 0.0;

    for (int i = 1; i < POINTS_COUNT - 1; i++) {
        l[i] = 2.0 * (xPoints[i + 1] - xPoints[i - 1]) - step[i - 1] * mu[i - 1];
        mu[i] = step[i] / l[i];
        z[i] = (alpha[i] - step[i - 1] * z[i - 1]) / l[i];
    }

    // Обратный ход метода прогонки и расчет коэффициентов сплайна
    l[POINTS_COUNT - 1] = 1.0;
    z[POINTS_COUNT - 1] = 0.0;
    coeffC[POINTS_COUNT - 1] = 0.0;

    for (int i = POINTS_COUNT - 2; i >= 0; i--) {
        coeffC[i] = z[i] - mu[i] * coeffC[i + 1];
        coeffB[i] = (yPoints[i + 1] - yPoints[i]) / step[i] - step[i] * (coeffC[i + 1] + 2.0 * coeffC[i]) / 3.0;
        coeffD[i] = (coeffC[i + 1] - coeffC[i]) / (3.0 * step[i]);
        coeffA[i] = yPoints[i];
    }

    // Поиск отрезка, в который попадает targetX
    int segmentIndex = -1;
    for (int i = 0; i < POINTS_COUNT - 1; i++) {
        if (targetX >= xPoints[i] && targetX <= xPoints[i + 1]) {
            segmentIndex = i;
            break;
        }
    }
    if (segmentIndex == -1) {
        return 0.0;
    }
    // Вычисление значения полинома на найденном отрезке
    double deltaX = targetX - xPoints[segmentIndex];
    double resultY = coeffA[segmentIndex] + coeffB[segmentIndex] * deltaX + 
        coeffC[segmentIndex] * pow(deltaX, 2) + coeffD[segmentIndex] * pow(deltaX, 3);

    return resultY;
}
void DrawGraph(double* xPoints, double* yPoints)
{
    const int originX = 680;
    const int originY = 400;
    const double scale = 100.0;

    HDC hDC = GetDC(GetConsoleWindow());
    HPEN axisPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    SelectObject(hDC, axisPen);

    // Отрисовка осей координат 
    MoveToEx(hDC, originX, originY, NULL);
    LineTo(hDC, originX + 300, originY); // Ось X
    MoveToEx(hDC, originX, 100, NULL);
    LineTo(hDC, originX, originY);       // Ось Y

    // Отрисовка исходных узлов интерполяции
    HPEN pointsPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
    SelectObject(hDC, pointsPen);
    for (int i = 0; i < POINTS_COUNT; i++) {
        int screenX = (int)(scale * xPoints[i] + originX);
        int screenY = (int)(-scale * yPoints[i] + originY + 1);

        MoveToEx(hDC, screenX, screenY, NULL);
        LineTo(hDC, screenX, screenY);
    }

    // Отрисовка кривой сплайна
    HPEN splinePen = CreatePen(PS_SOLID, 2, RGB(0, 255, 255));
    SelectObject(hDC, splinePen);
    double plotStep = 0.001;
    for (double currentX = xPoints[0]; currentX <= xPoints[POINTS_COUNT - 1] - plotStep; currentX += plotStep) {
        int currentScreenX = (int)(scale * currentX + originX);
        int currentScreenY = (int)(-scale * CalculateSpline(currentX, xPoints, yPoints) + originY);

        int nextScreenX = (int)(scale * (currentX + plotStep) + originX);
        int nextScreenY = (int)(-scale * CalculateSpline(currentX + plotStep, xPoints, yPoints) + originY);

        MoveToEx(hDC, currentScreenX, currentScreenY, NULL);
        LineTo(hDC, nextScreenX, nextScreenY);
    }

    ReleaseDC(GetConsoleWindow(), hDC);
    DeleteObject(axisPen);
    DeleteObject(pointsPen);
    DeleteObject(splinePen);
    (void)_getch();
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    printf(" Интерполирование сплайнами\n\n");

    double xPoints[POINTS_COUNT] = { 0.35, 0.41, 0.47, 0.51, 0.56, 0.64 };
    double yPoints[POINTS_COUNT] = { 2.73951, 2.3008, 1.96864, 1.78776, 1.59502, 1.3431 };

    double targetX = 0.526;

    printf("  Исходные узлы интерполяции:\n");
    printf("  i  |    x    |    y    \n");
    printf("-----+---------+---------\n");
    for (int i = 0; i < POINTS_COUNT; i++) {
        printf("  %d  | %.5f | %.5f\n", i, xPoints[i], yPoints[i]);
    }

    double resultY = CalculateSpline(targetX, xPoints, yPoints);
    printf("  Искомая точка: x = %.5f\n", targetX);
    printf("  Значение сплайна: S(%.5f) = %.5f\n", targetX, resultY);

    DrawGraph(xPoints, yPoints);
    return 0;
}
