// Пример создания потока функцией CreateThread
#include <windows.h>
#include <math.h>
#include <stdio.h>
//#include <iostream>
using namespace std;
volatile int result;
DWORD WINAPI calculate(LPVOID iNum)
{
	printf_s("Thread is started\n");
	float tmp = *(float*)iNum;
	int n = (ceil(tmp) - tmp >= 0.0 ? ceil(tmp) : ceil(tmp) + 1);
	float result = 0;
	float value = *((float*)iNum + 1);
	for (int i = 0; i < n + 1; i++)
	{
		result += pow(value, (double)(n - i)) * *((float*)iNum + i + 2);
//		printf_s("%4.2f\n", result);
		Sleep(15);
	}
	*((float*)iNum + 1) = result;
	printf_s("Thread is finished\n");
	return 0;
}
int main()
{
	HANDLE polynom[2];
	DWORD IDThread[2];
	int n;
	float value;
	float* numerator;
	float* denominator;

	printf_s("Enter degree of polynom in numerator: ");
	scanf_s("%d", &n);
	numerator = new float[n + 3];
	printf_s("Enter coeff of polynom from max degree to min:\n");
	for (int i = 0; i < n + 1; i++)
	{
		scanf_s("%f", &(numerator[i + 2]));
	}
	numerator[0] = (float)n;

	printf_s("Enter degree of polynom in denominator: ");
	scanf_s("%d", &n);
	denominator = new float[n + 3];
	printf_s("Enter coeff of polynom from max degree to min:\n");
	for (int i = 0; i < n + 1; i++)
	{
		scanf_s("%f", &(denominator[i + 2]));
	}
	denominator[0] = (float)n;

	printf_s("Enter value: ");
	scanf_s("%f", &value);
	denominator[1] = value;
	numerator[1] = value;;

	polynom[0] = CreateThread(NULL, 0, calculate, (void*)numerator, 0, &(IDThread[0]));
	polynom[1] = CreateThread(NULL, 0, calculate, (void*)denominator, 0, &(IDThread[1]));
	if (polynom[0] == NULL || polynom[1] == NULL)
	{
		return GetLastError();
	}
	// ждем пока поток закончит работу
	WaitForMultipleObjects(2, polynom, true, INFINITE);
	// закрываем дескриптор потока
	CloseHandle(polynom[0]);
	CloseHandle(polynom[1]);
	printf_s("f(%4.2f) = %4.2f", value, numerator[1] * 1. / denominator[1]);
	system("pause");
	return 0;
}
