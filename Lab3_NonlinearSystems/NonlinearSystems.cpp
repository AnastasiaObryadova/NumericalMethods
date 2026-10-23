#include <Windows.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

void NewtonMethod(double* currentX, double* nextX, double eps)
{
	printf("Метод Ньютона\n");
	double determinant;//определитель
	double inverseJacobian[2][2];//обратная матрица
	double transposedAdjugate[2][2];//транспонированная матрица алгебраических дополнений
	double functionValues[2];

	while (fabs(nextX[0] - currentX[0]) >= eps || fabs(nextX[1] - currentX[1]) >= eps)
	{
		//итерация
		currentX[0] = nextX[0];
		currentX[1] = nextX[1];

		//вычисление обратной матрицы
		transposedAdjugate[0][0] = -sin(currentX[0] - 1.0) + 1.0;
		transposedAdjugate[0][1] = -2.0;
		transposedAdjugate[1][0] = -1.0;
		transposedAdjugate[1][1] = cos(currentX[1]);

		determinant = cos(currentX[0]) * (-sin(currentX[1] - 1.0) + 1.0) - 2.0;

		inverseJacobian[0][0] = transposedAdjugate[0][0] * 1 / determinant;
		inverseJacobian[0][1] = transposedAdjugate[0][1] * 1 / determinant;
		inverseJacobian[1][0] = transposedAdjugate[1][0] * 1 / determinant;
		inverseJacobian[1][1] = transposedAdjugate[1][1] * 1 / determinant;

		//вычисление Xi
		functionValues[0] = sin(currentX[0]) + 2.0 * currentX[1] - 1.6;
		functionValues[1] = currentX[0] + cos(currentX[1] - 1.0) - 1.0;

		nextX[0] = currentX[0] - (inverseJacobian[0][0] * functionValues[0] + inverseJacobian[0][1] * functionValues[1]);
		nextX[1] = currentX[1] - (inverseJacobian[1][0] * functionValues[0] + inverseJacobian[1][1] * functionValues[1]);
	}
	printf("x = %lf\ny = %lf\n\n", nextX[0], nextX[1]);
}

void IterationMethod(double* currentX, double* nextX, double eps)
{
	printf("Метод итераций\n");
	//Нулевое приближение
	currentX[0] = 2.0;
	currentX[1] = 2.0;

	//Первая итерация
	nextX[0] = 1.0 - cos(currentX[1] - 1.0);
	nextX[1] = 0.8 - sin(currentX[0]) * 0.5;
	
	//Основной цикл
	while (fabs(currentX[0] - nextX[0]) >= eps || fabs(currentX[1] - nextX[1]) >= eps)
	{
		//Сохранение старого значения
		currentX[0] = nextX[0];
		currentX[1] = nextX[1];

		//Вычисление очередного значения
		nextX[0] = 1.0 - cos(currentX[1] - 1.0);
		nextX[1] = 0.8 - sin(currentX[0]) * 0.5;
	}
	printf("x = %lf\ny = %lf\n", nextX[0], nextX[1]);
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	double eps = 0.00001;
	double nextX[2] = { 1,1 };
	double currentX[2] = { 2,2 };
	
	NewtonMethod(currentX, nextX, eps);
	IterationMethod(currentX, nextX, eps);

	return 0;
}