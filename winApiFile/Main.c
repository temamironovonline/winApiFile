#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <Windows.h>

float* getCoefficients();
void decisionCompleteEquation(float*, char*, int);
void decisionIncompleteEquation(float*, char*, int);
void writeResultToFile(char*, int);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	float* argumentsForCalculation = getCoefficients();
	char* result = "";
	int resultCountSymbols = 0;

	if (argumentsForCalculation[0] == 0)
	{
		result = calloc(strlen("Уравнение не является квадратным, т.к. a == 0"), sizeof(char));
		sprintf(result, "Уравнение не является квадратным, т.к. a == 0");
		resultCountSymbols = strlen(result);
		writeResultToFile(result, resultCountSymbols);
	}
	else if (argumentsForCalculation[1] != 0 && argumentsForCalculation[2] != 0) decisionCompleteEquation(argumentsForCalculation, result, resultCountSymbols);
	else decisionIncompleteEquation(argumentsForCalculation, result, resultCountSymbols);
}



float* getCoefficients() // Чтение из файла и запись в массив
{
	HANDLE fileStart;
	fileStart = CreateFile(L"arguments.txt", // открываемый файл
		GENERIC_READ, // открываем для чтения
		FILE_SHARE_READ, // для совместного чтения
		NULL, // защита по умолчанию
		OPEN_EXISTING, // только существующий файл
		FILE_ATTRIBUTE_NORMAL, // обычный файл
		NULL); // атрибутов шаблона нет

	if (fileStart == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, L"При открытие файла возникла ошибка!", L"Окно программы", MB_OK);
		return 0;
	}

	DWORD countFileSymbols;
	LPCSTR argumentsFromFile = calloc(100, sizeof(CHAR));
	if (!ReadFile(fileStart, argumentsFromFile, 100, &countFileSymbols, NULL))
	{
		MessageBox(NULL, L"При чтении данных из файла возникла ошибка!", L"Окно программы", MB_OK);
		return 0;
	}

	CloseHandle(fileStart); //Закрываем дескриптор подключения к файлу

	char* argumentsForSplit = calloc(countFileSymbols, sizeof(CHAR)); //Выделяем память исходя из действительного количества символов в файле (countFileSymbols)
	for (int i = 0; i < (int)(countFileSymbols + 1); i++) //Записываем значения в новый указатель
	{
		argumentsForSplit[i] = argumentsFromFile[i];
	}
	free(argumentsFromFile); //Освобождаем старый указатель

	char* argumentsForConverting = strtok(argumentsForSplit, " ");
	float* arguments = calloc(3, sizeof(float));

	for (int i = 0; argumentsForConverting != NULL; i++)
	{
		arguments[i] = atof(argumentsForConverting);
		argumentsForConverting = strtok(NULL, " ");
	}

	return arguments;
}

void decisionCompleteEquation(float* forCoefficients, char* forAnswer, int forAnswerCountSymbols) //Вычисления при полном квадратном уравнении
{
	float discriminant;
	float x1, x2;

	discriminant = pow(forCoefficients[1], 2) - 4 * forCoefficients[0] * forCoefficients[2];

	if (discriminant > 0)
	{
		x1 = (-forCoefficients[1] + sqrt(discriminant)) / (2 * forCoefficients[0]);
		x2 = (-forCoefficients[1] - sqrt(discriminant)) / (2 * forCoefficients[0]);

		forAnswer = calloc(strlen("Дискриминант = %f, x1 = %f, x2 = %f", discriminant, x1, x2), sizeof(char));
		sprintf(forAnswer,"Дискриминант = %f, x1 = %f, x2 = %f", discriminant, x1, x2);
		forAnswerCountSymbols = strlen(forAnswer);
	}
	else if (discriminant == 0)
	{
		x1 = (-forCoefficients[1] + sqrt(discriminant)) / (2 * forCoefficients[0]);

		forAnswer = calloc(strlen("Дискриминант = %f, x1 = %f", discriminant, x1), sizeof(char));
		sprintf(forAnswer,"Дискриминант = %f, x1 = %f", discriminant, x1);
		forAnswerCountSymbols = strlen(forAnswer);
	}
	else
	{
		forAnswer = calloc(strlen("Корней нет!"), sizeof(char));
		sprintf(forAnswer, "Корней нет!");
		forAnswerCountSymbols = strlen(forAnswer);
	}
	writeResultToFile(forAnswer, forAnswerCountSymbols);
}

void decisionIncompleteEquation(float* forCoefficients, char* forAnswer, int forAnswerCountSymbols) //Вычисления при неполном квадратном уравнении
{
	float x1, x2;

	if (forCoefficients[1] == 0 && forCoefficients[2] != 0)
	{
		if (-(forCoefficients[2] / forCoefficients[0]) >= 0)
		{
			x1 = sqrt(-(forCoefficients[2] / forCoefficients[0]));
			x2 = -x1;

			forAnswer = calloc(strlen("Неполное квадратное уравнение, b = 0, x1 = %f, x2 = %f", x1, x2), sizeof(char));
			sprintf(forAnswer, "Неполное квадратное уравнение, b = 0, x1 = %f, x2 = %f", x1, x2);
			forAnswerCountSymbols = strlen(forAnswer);
		}
		else
		{
			forAnswer = calloc(strlen("Нет корней, т.к. -(c/a) < 0"), sizeof(char));
			sprintf(forAnswer, "Нет корней, т.к. -(c/a) < 0");
			forAnswerCountSymbols = strlen(forAnswer);
		}
	}
	else if (forCoefficients[2] == 0 && forCoefficients[1] != 0)
	{
		x1 = 0;
		x2 = -(forCoefficients[1] / forCoefficients[0]);

		forAnswer = calloc(strlen("Неполное квадратное уравнение, c = 0, x1 = %f, x2 = %f", x1, x2), sizeof(char));
		sprintf(forAnswer, "Неполное квадратное уравнение, c = 0, x1 = %f, x2 = %f", x1, x2);
		forAnswerCountSymbols = strlen(forAnswer);
	}
	else
	{
		x1 = 0;

		forAnswer = calloc(strlen("Неполное квадратное уравнение, c = 0 b = 0, x = %f", x1), sizeof(char));
		sprintf(forAnswer, "Неполное квадратное уравнение, c = 0 b = 0, x = %f", x1);
		forAnswerCountSymbols = strlen(forAnswer);
	}
	writeResultToFile(forAnswer, forAnswerCountSymbols);
}

void writeResultToFile(char* forAnswer, int forCountSymbols) //Запись ответа в файл
{
	HANDLE fileResult = CreateFile(L"Result.txt", // создаваемый файл
		GENERIC_WRITE, // открывается для записи
		FILE_SHARE_WRITE, // совместно не используется
		NULL, // защита по умолчанию
		CREATE_ALWAYS, // переписывает существующий
		FILE_ATTRIBUTE_NORMAL, // асинхронный ввод/вывод I/O
		NULL); // атрибутов шаблона нет
	DWORD countFileSymbols;
	WriteFile(fileResult, forAnswer, forCountSymbols, &countFileSymbols, NULL);
	CloseHandle(fileResult);
}