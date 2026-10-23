#include <Windows.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

void GaussMethod(double sourceMatrix[3][4], int totalEquations)
{
    printf("Метод Гаусса:\n");
    //перебор строк системы
    for (int leadRow = 1; leadRow <= totalEquations; leadRow++)
    {
        // Нормализация строки (деление элементов на диагональный)
        double diagonalEl = sourceMatrix[leadRow - 1][leadRow - 1];
        for (int col = totalEquations + 1; col >= leadRow; col--)
        {
            sourceMatrix[leadRow - 1][col - 1] = sourceMatrix[leadRow - 1][col - 1] / diagonalEl;
        }
        // Исключение неизвестных из остальных строк матрицы
        for (int subRow = 1; subRow <= totalEquations; subRow++)
        {
            if (subRow != leadRow)
            {
                // Вычитание ведущей строки, умноженной на коэффициент, из текущей строки
                for (int col = totalEquations + 1; col >= 1; col--)
                {
                    sourceMatrix[subRow - 1][col - 1] = sourceMatrix[subRow - 1][col - 1] - 
                        sourceMatrix[subRow - 1][leadRow - 1] * sourceMatrix[leadRow - 1][col - 1];
                }
            }
        }
    }
    //Вывод результатов в консоль
    for (int row = 1; row <= totalEquations; row++)
    {
        printf("x%d = %f\n", row, sourceMatrix[row - 1][totalEquations]);
    }
}

void SimpleIterationMethod(double diagonalDominantMatrix[3][4], int totalEquations, double eps)
{
    printf("\nМетод Простых итераций:\n");

    double iterationMatrix[3][4] = { {0.68, -0.29, 0.19, 1.53}, {0.29, -1.06, -0.43, -0.03}, {-0.05, 0.52, 0.86, 0.65} };
    double sumOfSquares = 0;

    // Приведение исходной матрицы к итерационному виду (выражение x)
    for (int row = 0; row < totalEquations; row++)
    {
        for (int col = 0; col <= totalEquations; col++)
        {
            if (col == 3)
            {
                iterationMatrix[row][col] = diagonalDominantMatrix[row][col] / diagonalDominantMatrix[row][row];
            }
            else
            {
                iterationMatrix[row][col] = -diagonalDominantMatrix[row][col] / diagonalDominantMatrix[row][row];
            }
        }
        iterationMatrix[row][row] = 0;

    }

    double currentError = 0;
    double currentX[3] = { 0.0, 0.0, 0.0 };
    double nextX[3] = { 1.0, 1.0, 1.0 };

    // Подсчет критерия сходимости (суммы квадратов коэффициентов)
    for (int row = 0; row < totalEquations; row++)
    {
        for (int col = 0; col < totalEquations; col++)
        {
            sumOfSquares += iterationMatrix[row][col] * iterationMatrix[row][col];
        }

    }
    if (sumOfSquares > 1)
    {
        printf("Условие сходимости не выполняется");
    }
    else
    {
        // Заполнение начального приближения первыми свободными членами
        for (int row = 1; row <= totalEquations; row++)
        {
            currentX[row - 1] = iterationMatrix[row - 1][totalEquations + 1 - 1];
        }
        // Первая итерация вне цикла
        currentError = 0;
        for (int row = 1; row <= totalEquations; row++)
        {
            nextX[row - 1] = iterationMatrix[row - 1][totalEquations + 1 - 1];
            for (int col = 1; col <= totalEquations; col++)
            {
                nextX[row - 1] = nextX[row - 1] + iterationMatrix[row - 1][col - 1] * currentX[col - 1];
            }
            currentError = currentError + (nextX[row - 1] - currentX[row - 1]) * (nextX[row - 1] - currentX[row - 1]);

        }
        currentError = sqrt(currentError);
        // Основной цикл итерационного процесса
        while (currentError > eps)
        {
            for (int row = 1; row <= totalEquations; row++)
            {
                currentX[row - 1] = nextX[row - 1];
            }
            currentError = 0;
            for (int row = 1; row <= totalEquations; row++)
            {
                nextX[row - 1] = iterationMatrix[row - 1][totalEquations + 1 - 1];
                for (int col = 1; col <= totalEquations; col++)
                {
                    nextX[row - 1] = nextX[row - 1] + iterationMatrix[row - 1][col - 1] * currentX[col - 1];
                }
                currentError = currentError + (nextX[row - 1] - currentX[row - 1]) * (nextX[row - 1] - currentX[row - 1]);
            }
            currentError = sqrt(currentError);
        }
        //Вывод результатов в консоль
        for (int i = 1; i <= totalEquations; i++)
        {
            printf("x%d = %f\n", i, currentX[i - 1]);
        }

    }
}


void SeidelMethod(double diagonalDominantMatrix[3][4], int totalEquations, double eps)
{
    printf("\nМетод Зейделя:");

    double iterationMatrix[3][4] = { {0.68, -0.29, 0.19, 1.53}, {0.29, -1.06, -0.43, -0.03}, {-0.05, 0.52, 0.86, 0.65} };
    double currentError = 0;
    double currentX[3] = { 0.0, 0.0, 0.0 };
    double nextX[3] = { 1.0, 1.0, 1.0 };
    double matrixNorm = 0;

    // Приведение исходной матрицы к итерационному виду (выражение x)
    for (int row = 0; row < totalEquations; row++)
    {
        for (int col = 0; col <= totalEquations; col++)
        {
            if (col == 3)
            {
                iterationMatrix[row][col] = diagonalDominantMatrix[row][col] / diagonalDominantMatrix[row][row];
            }
            else
            {
                iterationMatrix[row][col] = -diagonalDominantMatrix[row][col] / diagonalDominantMatrix[row][row];
                if (col != row)
                {
                    matrixNorm += iterationMatrix[row][col] * iterationMatrix[row][col];
                }
            }

        }
        iterationMatrix[row][row] = 0;

    }
    
    // Проверка достаточного условия сходимости
    if (sqrt(matrixNorm) > 1)
    {
        printf("Условие сходимости не выполняется");
    }
    else
    {
        currentError = 2 * eps;
        // Итерационный процесс
        while (currentError > eps)
        {
            currentError = 0;
            for (int row = 0; row < totalEquations; row++)
            {
                nextX[row] = iterationMatrix[row][totalEquations];
                for (int col = 0; col < totalEquations; col++)
                {
                    nextX[row] = nextX[row] + iterationMatrix[row][col] * currentX[col];
                }
                currentError = currentError + (nextX[row] - currentX[row]) * (nextX[row] - currentX[row]);
                currentX[row] = nextX[row];
            }
            currentError = sqrt(currentError);

        }
        //Вывод результатов в консоль
        for (int row = 0; row < totalEquations; row++)
        {
            printf("\nx%d = %f", row, currentX[row]);
        }
    }
}
int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    
    int totalEquations = 3;
    double eps = 0.00001;
    double sourceMatrix[3][4] = { {0.92, -0.83, 0.62, 2.15}, {0.24, -0.54, 0.43, 0.62}, {0.73, -0.81, -0.67, 0.88} };
    double diagonalDominantMatrix[3][4] = { {0.68, -0.29, 0.19, 1.53}, {0.29, -1.06, -0.43, -0.03}, {-0.05, 0.52, 0.86, 0.65} };
    GaussMethod(sourceMatrix, totalEquations);
    SimpleIterationMethod(diagonalDominantMatrix, totalEquations, eps);
    SeidelMethod(diagonalDominantMatrix, totalEquations, eps);

    return 0;
}