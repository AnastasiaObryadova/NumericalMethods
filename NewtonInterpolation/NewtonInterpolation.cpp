#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
double CalculateFactorial(int number)
{
    if (number <= 0) 
        return 1.0;
    double result = 1.0;
    for (int i = 1; i <= number; i++) {
        result *= i;
    }
    return result;
}
// Вычисление конечной разности заданного порядка для конкретного узла
double CalcFiniteDifference(double* yPoints, int differenceOrder, int targetNodeIndex)
{
    double totalSum = 0.0;
    //Сумма
    for (int addendIndex = 0; addendIndex <= differenceOrder; addendIndex++)
    {
        double product = 1.0;
        //Произведение
        for (int productIndex = 0; productIndex <= addendIndex; productIndex++)
        {
            
            if (addendIndex == productIndex) {
                product = product * 1.0;
            }
            else {
                product = product * (differenceOrder - productIndex);
            }
        }
        totalSum = totalSum + product * pow(-1.0, addendIndex) * 
            yPoints[targetNodeIndex + differenceOrder - addendIndex] / (double)CalculateFactorial(addendIndex);
    }
    return totalSum;
}

double NewtonForward(double targetX, double* yPoints, double* xPoints, double step, int degree)
{
    double totalSum = 0.0;
    //Сумма
    for (int currentDegree = 0; currentDegree <= degree; currentDegree++)
    {
        double product = 1.0;
        //Произведение
        for (int j = 0; j <= currentDegree; j++)
        {
            if (j == currentDegree) {
                product = product * 1.0;
            }
            else {
                // Движение вперед
                product = product * (((targetX - xPoints[0]) / step) - j);
            }
        }

        // Берем конечные разности из НАЧАЛА таблицы (индекс 0)
        totalSum = totalSum + product * (CalcFiniteDifference(yPoints, currentDegree, 0) / CalculateFactorial(currentDegree));
    }
    return totalSum;
}


double NewtonBackward(double targetX, double* yPoints, double* xPoints, double step, int degree)
{
    double totalSum = 0.0;
    //Сумма
    for (int currentDegree = 0; currentDegree <= degree; currentDegree++)
    {
        double product = 1.0;
        //Произведение
        for (int j = 0; j <= currentDegree; j++)
        {
            if (j == currentDegree) {
                product = product * 1.0;
            }
            else {
                // Движение назад
                product = product * (((targetX - xPoints[degree]) / step) + j);
            }
        }

        totalSum = totalSum + product * (CalcFiniteDifference(yPoints, currentDegree, degree - currentDegree) / CalculateFactorial(currentDegree));
    }
    return totalSum;
}
void DrawGraph(double* xPoints, double* yPoints, double step, int degree)
{
    HDC hDC = GetDC(GetConsoleWindow());

    HPEN axisPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    HPEN graphPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    SelectObject(hDC, axisPen);

    // Отрисовка осей координат (центр 200, 400)
    MoveToEx(hDC, 100, 400, NULL);
    LineTo(hDC, 1000, 400); // Ось X
    MoveToEx(hDC, 200, 100, NULL);
    LineTo(hDC, 200, 500);  // Ось Y

    SelectObject(hDC, graphPen);

    // Вывод графика погрешности первой формулы
    for (int x = 0; x < degree; x++)
    {
        // Находим погрешность вычислений
        double currentError = NewtonForward(xPoints[x], yPoints, xPoints, step, degree) - yPoints[x];
        double nextError = NewtonForward(xPoints[x + 1], yPoints, xPoints, step, degree) - yPoints[x + 1];

        int currentScreenX = (int)(5000 * (xPoints[x] - xPoints[0]) + 250);
        int nextScreenX = (int)(5000 * (xPoints[x + 1] - xPoints[0]) + 250);

        int currentScreenY = (int)(-100000000000000.0 * currentError + 400);
        int nextScreenY = (int)(-100000000000000.0 * nextError + 400);

        MoveToEx(hDC, currentScreenX, currentScreenY, NULL);
        LineTo(hDC, nextScreenX, nextScreenY);
    }
    ReleaseDC(GetConsoleWindow(), hDC);
    DeleteObject(axisPen);
    DeleteObject(graphPen);
    (void)_getch(); 
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    printf(" Интерполяционный полином Ньютона\n");

    double xPoints[10] = { 0.101, 0.106, 0.111, 0.116, 0.121, 0.126, 0.131, 0.136, 0.141, 0.146 };
    double yPoints[10] = { 1.26183, 1.27644, 1.29122, 1.30617, 1.32130, 1.33660, 1.35207, 1.36773, 1.38357, 1.39959 };

    double x1[3] = { 0.1156, 0.1142, 0.1063 };
    double x2[3] = { 0.1457, 0.1452, 0.1369 };
    //Шаг интерполирования
    double step = xPoints[1] - xPoints[0];
    int n = 9;

    for (int i = 0; i < 3; i++) {
        printf(" В точке x = %.4f    P(x) = %f\n", x1[i], NewtonForward(x1[i], yPoints, xPoints, step, n));
    }
    for (int i = 0; i < 3; i++) {
        printf(" В точке x = %.4f    P(x) = %f\n", x2[i], NewtonBackward(x2[i], yPoints, xPoints, step, n));
    }
    printf(" График погрешности первой формулы\n");
    DrawGraph(xPoints, yPoints, step, n);
    return 0;
}