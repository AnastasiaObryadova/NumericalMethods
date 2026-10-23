#include <Windows.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <stdio.h>

// Функция: f(x) = cos(x + 0.5) - x^3
double GetFunctionValue(double x)
{
    return cos(x + 0.5) - pow(x, 3);
}

// Первая производная функции: f'(x) = -sin(x + 0.5) - 3*x^2
double GetDerivativeValue(double x)
{
    return -sin(x + 0.5) - 3 * pow(x, 2);
}

// 1. Метод  дихотомии (половинного деления)
void DichotomyMethod(double leftBorder, double rightBorder, double precision)
{
    double midPoint;

    // Делим отрезок пополам, пока его длина больше заданной погрешности
    while ((rightBorder - leftBorder) > (2 * precision))
    {
        midPoint = (leftBorder + rightBorder) / 2.0;

        // Определяем, в какой половине отрезка функция меняет знак
        if (GetFunctionValue(leftBorder) * GetFunctionValue(midPoint) < 0)
        {
            rightBorder = midPoint;
        }
        else
        {
            leftBorder = midPoint;
        }
    }

    double root = (leftBorder + rightBorder) / 2.0;
    printf("   Полученный корень = %lf\n", root);

}

// 2. Метод простых итераций
void SimpleIterationMethod(double leftBorder, double rightBorder, double precision)
{
    double step = (rightBorder - leftBorder) / 100.0;
    double currentX = leftBorder;
    double nextX = leftBorder + step;

    // Оценка максимума модуля производной на заданном отрезке
    double maxDerivative = fabs(GetFunctionValue(nextX) - GetFunctionValue(currentX)) / step;
    double currentDerivativeValue;

    while (nextX < rightBorder)
    {
        currentX = nextX;
        nextX = currentX + step;
        currentDerivativeValue = fabs(GetFunctionValue(nextX) - GetFunctionValue(currentX)) / step;
        if (currentDerivativeValue > maxDerivative)
        {
            maxDerivative = currentDerivativeValue;
        }
    }
    maxDerivative += 1; // Коэффициент сходимости

    double sign = (GetFunctionValue(leftBorder) < GetFunctionValue(rightBorder)) ? 1.0 : -1.0;

    double previousX = (leftBorder + rightBorder) / 2.0;
    double calculatedX = previousX - sign * GetFunctionValue(previousX) / maxDerivative;

    // Итерационный процесс до достижения заданной точности
    while (fabs(calculatedX - previousX) > precision)
    {
        previousX = calculatedX;
        calculatedX = previousX - sign * GetFunctionValue(previousX) / maxDerivative;
    }

    printf("   Полученный корень = %lf\n", calculatedX);
}

// 3. Метод хорд
void SecantMethod(double leftBorder, double rightBorder, double eps)
{
    
    

    double h = (rightBorder - leftBorder) / 100.0;
    double d = GetFunctionValue(leftBorder + 2 * h) - 2 * GetFunctionValue(leftBorder + h) +
        GetFunctionValue(leftBorder);
    double currentX = 0.0;
    double fixedPoint = 0.0;
    if (GetFunctionValue(leftBorder) * d < 0)
    {
        currentX = rightBorder;
        fixedPoint = leftBorder;
    }
    else
    {
        
        currentX = leftBorder;
        fixedPoint = rightBorder;
    }

    double nextX = currentX - (fixedPoint - currentX) * GetFunctionValue(currentX) /
        (GetFunctionValue(fixedPoint) - GetFunctionValue(currentX));

    while (fabs(nextX - currentX) > eps)
    {
        currentX = nextX;
        nextX = currentX - (fixedPoint - currentX) * GetFunctionValue(currentX) / 
            (GetFunctionValue(fixedPoint) - GetFunctionValue(currentX));

    }

    printf("   Полученный корень = %lf\n", nextX);
}

// 4. Метод касательных (Ньютона)
void TangentMethod(double leftBorder, double rightBorder, double eps)
{
    double step = (rightBorder - leftBorder) / 100.0;
    double currentX;

    // Приближенное вычисление второй производной для выбора начальной точки
    double secondDerivative = GetFunctionValue(leftBorder + 2 * step) - 2 * GetFunctionValue(leftBorder + step) +
        GetFunctionValue(leftBorder);

    // Выбор точки, где знак функции совпадает со знаком второй производной
    if (GetFunctionValue(leftBorder) * secondDerivative > 0)
    {
        currentX = leftBorder;
    }
    else
    {
        currentX = rightBorder;
    }

    double nextX = currentX - GetFunctionValue(currentX) / GetDerivativeValue(currentX);

    while (fabs(nextX - currentX) > eps)
    {
        currentX = nextX;
        nextX = currentX - GetFunctionValue(currentX) / GetDerivativeValue(currentX);
    }

    printf("   Полученный корень = %lf\n", nextX);
}

// 5. Метод хорд и касательных
void SecantTangentMethod(double leftBorder, double rightBorder, double eps)
{
    double secantX, tangentX, fixedPoint;
    double step = (rightBorder - leftBorder) / 100.0;

    // Приближенное вычисление второй производной
    double secondDerivative = GetFunctionValue(leftBorder + 2 * step) - 2 * GetFunctionValue(leftBorder + step) +
        GetFunctionValue(leftBorder);

    if (GetFunctionValue(leftBorder) * secondDerivative < 0)
    {
        secantX = leftBorder;
        tangentX = rightBorder;
        fixedPoint = rightBorder;
    }
    else
    {
        secantX = rightBorder;
        tangentX = leftBorder;
        fixedPoint = leftBorder;
    }

    // Одновременное приближение к корню с двух сторон
    double nextSecantX = secantX - (fixedPoint - secantX) * GetFunctionValue(secantX) /
        (GetFunctionValue(fixedPoint) - GetFunctionValue(secantX));
    double nextTangentX = tangentX - GetFunctionValue(tangentX) / GetDerivativeValue(tangentX);

    while (fabs(nextTangentX - nextSecantX) > (2 * eps))
    {
        nextTangentX = tangentX - GetFunctionValue(tangentX) / GetDerivativeValue(tangentX);
        nextSecantX = secantX - (fixedPoint - secantX) * GetFunctionValue(secantX) / 
            (GetFunctionValue(fixedPoint) - GetFunctionValue(secantX));

        if (GetFunctionValue(secantX) * GetFunctionValue(tangentX) < 0)
        {
            fixedPoint = nextSecantX;
        }
        else
        {
            secantX = nextSecantX;
        }
        tangentX = nextTangentX;
    }

    double root = (nextSecantX + nextTangentX) / 2.0;
    printf("   Полученный корень = %lf\n", root);
}

// Отрисовка графика функции
void DrawGraph()
{
    HDC hDC = GetDC(GetConsoleWindow());
    //серое перо для осей 
    HPEN AxisPen = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
    //синее перо для графика
    HPEN GraphPen = CreatePen(PS_SOLID, 2, RGB(0, 102, 204));

    SelectObject(hDC, AxisPen);

    // Отрисовка оси X
    MoveToEx(hDC, 400, 285, NULL);
    LineTo(hDC, 800, 285);

    // Отрисовка оси Y
    MoveToEx(hDC, 600, 100, NULL);
    LineTo(hDC, 600, 470);

    // Границы отрезка
    MoveToEx(hDC, 700, 275, NULL);
    LineTo(hDC, 700, 295);

    MoveToEx(hDC, 550, 275, NULL);
    LineTo(hDC, 550, 295);

    SelectObject(hDC, GraphPen);

    // Вынос графика функции на экран (Масштаб 50:1, Центр в точке 600, 285)
    for (double x = -8.0; x <= 8.0; x += 0.01)
    {
        int screenX = (int)(50 * x + 600);
        int screenY = (int)(-50 * GetFunctionValue(x) + 285);

        MoveToEx(hDC, screenX, screenY, NULL);
        LineTo(hDC, screenX, screenY);
    }
}





int main(void)
{
    // Настройка консоли для корректного отображения кириллицы
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    printf("   Функция: f(x) = cos(x + 0.5) - x^3 \n");
    printf("   Интервал уточнения корня: [-1 ; 2] \n");
    printf("   1 - метод дихотомии \n");
    printf("   2 - метод простых итераций \n");
    printf("   3 - метод хорд \n");
    printf("   4 - метод касательных \n");
    printf("   5 - метод хорд и касательных \n");
    printf("   Выберите метод: ");

    // Общие параметры для всех численных методов
    double a = -1;
    double b = 2;
    double epsilon = 0.0001;

    int method;
    scanf_s("%d", &method);

    //вычисление корня выбранным методом
    switch (method)
    {
    case 1:
        DichotomyMethod(a, b, epsilon);
        break;
    case 2:
        SimpleIterationMethod(a, b, epsilon);
        break;
    case 3:
        SecantMethod(a, b, epsilon);
        break;
    case 4:
        TangentMethod(a, b, epsilon);
        break;
    case 5:
        SecantTangentMethod(a, b, epsilon);
        break;
    default:
        printf("   Неверный выбор метода\n");
        break;
    }


    // Визуализация графика функции
    DrawGraph();

    (void)_getch();
    return 0;
}