#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>

// Вычисление полинома Лагранжа напрямую через базовую формулу интерполяции
double CalculateLagrangeDirect(double targetX, double* xPoints, double* yPoints, int polynomialDegree)
{
    double totalSum = 0;
    double product = 1;
    for (int currentPoint = 0; currentPoint <= polynomialDegree; currentPoint++)
    {
        product = 1;
        for (int otherPoint = 0; otherPoint <= polynomialDegree; otherPoint++)
        {
            if (currentPoint != otherPoint)
                product = product * (targetX - xPoints[otherPoint]) / (xPoints[currentPoint] - xPoints[otherPoint]);
        }
        totalSum = totalSum + product * yPoints[currentPoint];
    }
    return totalSum;
}

// Вычисление полинома по коэффициентам
double CalculatePolynomialByCoefficients(double targetX, double* coefficients)
{
    return coefficients[5] * pow(targetX, 5) + coefficients[4] * pow(targetX, 4) + coefficients[3] * pow(targetX, 3) + coefficients[2] * pow(targetX, 2) + coefficients[1] * targetX + coefficients[0];
}

void LagrangeInterpolation(double* xPoints, double* yPoints, int polynomialDegree, double vandermondeMatrix[6][6], long double inverseMatrix[6][6], double* coefficients)
{
    printf(" Интерполяционный полином Лагранжа\n");
    double targetX = 0.526;
    //вычисление значения функции в искомой точке
    printf(" Значение через формулу Лагранжа f(x) = %f \n", CalculateLagrangeDirect(targetX, xPoints, yPoints, polynomialDegree));
    
    // Заполнение матрицы Вандермонда
    for (int col = 0; col <= polynomialDegree; col++)
    {
        for (int row = 0; row <= polynomialDegree; row++)
        {
            vandermondeMatrix[row][col] = pow(xPoints[row], col);
        }
    }

    // Расчет коэффициентов полинома
    for (int coefIndex = 0; coefIndex <= polynomialDegree;coefIndex++)
    {
        coefficients[coefIndex] = 0.0;
        for (int nodeIndex = 0; nodeIndex <= polynomialDegree;nodeIndex++)
        {
            coefficients[coefIndex] += inverseMatrix[coefIndex][nodeIndex] * yPoints[nodeIndex];
        }
    }

    printf(" Многочлен канонического вида P(x) = %f * x^5 + %f * x^4 + %f * x^3 + %f * x^2 + %f * x + %f\n", coefficients[5], coefficients[4], coefficients[3], coefficients[2], coefficients[1], coefficients[0]);
    printf(" P(x) = %f\n", CalculatePolynomialByCoefficients(targetX, coefficients));
}

void DrawGraph(double* xPoints, double* yPoints, int polynomialDegree, double vandermondeMatrix[6][6], long double inverseMatrix[6][6], double* coefficients)
{
    
    HDC hDC = GetDC(GetConsoleWindow());

    HPEN axisPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    HPEN pointPen = CreatePen(PS_SOLID, 7, RGB(255, 0, 0));
    HPEN functionPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
    HPEN polynomialPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));

    //оси
    SelectObject(hDC, axisPen);
    MoveToEx(hDC, 150, 400, NULL); 
    LineTo(hDC, 950, 400);        
    MoveToEx(hDC, 200, 100, NULL); 
    LineTo(hDC, 200, 450);

    // Отрисовка начальных точек (масштаб по X: 1000, центр по X: 200)
    SelectObject(hDC, pointPen);
    for (int x = 0; x < 6; x++)
    {
        int screenX = (int)(1000.0 * xPoints[x] + 200.0);
        int screenY = (int)(-100.0 * yPoints[x] + 400.0);

        MoveToEx(hDC, screenX, screenY, NULL);
        LineTo(hDC, screenX, screenY);
    }

    // Отрисовка графика fx (масштаб по X: 1000, центр по X: 200)
    SelectObject(hDC, functionPen);
    for (double x = 0.35; x <= 0.64; x = x + 0.01)
    {
        int currentScreenX = (int)(1000.0 * x + 200.0);
        int currentScreenY = (int)(-100.0 * CalculateLagrangeDirect(x, xPoints, yPoints, polynomialDegree) + 400.0);

        int nextScreenX = (int)(1000.0 * (x + 0.01) + 200.0);
        int nextScreenY = (int)(-100.0 * CalculateLagrangeDirect(x + 0.01, xPoints, yPoints, polynomialDegree) + 400.0);

        MoveToEx(hDC, currentScreenX, currentScreenY, NULL);
        LineTo(hDC, nextScreenX, nextScreenY);
    }

    // Отрисовка графика Px (масштаб по X: 1000, центр по X: 200)
    SelectObject(hDC, polynomialPen);
    for (double x = 0.35; x <= 0.64; x = x + 0.01)
    {
        int currentScreenX = (int)(1000.0 * x + 200.0);
        int currentScreenY = (int)(-100.0 * CalculatePolynomialByCoefficients(x, coefficients) + 400.0);

        int nextScreenX = (int)(1000.0 * (x + 0.01) + 200.0);
        int nextScreenY = (int)(-100.0 * CalculatePolynomialByCoefficients(x + 0.01, coefficients) + 400.0);

        MoveToEx(hDC, currentScreenX, currentScreenY, NULL);
        LineTo(hDC, nextScreenX, nextScreenY);
    }

    ReleaseDC(GetConsoleWindow(), hDC);
    DeleteObject(axisPen);
    DeleteObject(pointPen);
    DeleteObject(functionPen);
    DeleteObject(polynomialPen);

    (void)_getch();
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    
    double xPoints[6] = { 0.35, 0.41, 0.47, 0.51, 0.56, 0.64 };
    double yPoints[6] = { 2.73951, 2.30080, 1.96864, 1.78776, 1.59502, 1.34310 };
    int polynomialDegree = 5;
    double vandermondeMatrix[6][6];
    double coefficients[6] = { 0,0,0,0,0,0 };
    long double inverseMatrix[6][6] = { {157.85477433612473784515,   -475.93008567231449621873,   468.31025235013981436511,    -0.07917882035162692458, -190.95518132152370983323,  41.79941912792528076629},\
                        {-1249.48505546697159364299, 3965.72836332183913987637,   -4047.68051142185265378457,  0.84971816761005077344,  1715.35457626655505659664,  -384.76709086717999981889},\
                        {3673.71028768741346537448,  -12163.48426755390538207504, 12871.54849646211889142135,  -3.60635160371744591756, -5693.36275727535763317111, 1315.19459228344810436787},\
                        {-4755.06610606783757798484, 16305.30939598335437884241,  -17843.06091277379615542809, 7.56671746840978573287,  8263.55864480191830949191,  -1978.30773941204874065425},\
                        {2286.41506829417351228669,  -8073.0251352700441008915,  9106.386096769976216265,   -7.84888813527408097801, -4416.62337108138126995004, 1104.69622942254972326786},\
                        {-0.19047630553334877281,    1.07694442120513162677,      -3.03832864712541449364,     3.22028504252657552201,  -1.18225342237054354983,    0.11382891129759966749} };

    LagrangeInterpolation(xPoints, yPoints, polynomialDegree, vandermondeMatrix, inverseMatrix, coefficients);
    DrawGraph(xPoints, yPoints, polynomialDegree, vandermondeMatrix, inverseMatrix, coefficients);
    
    return 0;
}