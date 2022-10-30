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
		result = calloc(strlen("��������� �� �������� ����������, �.�. a == 0"), sizeof(char));
		sprintf(result, "��������� �� �������� ����������, �.�. a == 0");
		resultCountSymbols = strlen(result);
		writeResultToFile(result, resultCountSymbols);
	}
	else if (argumentsForCalculation[1] != 0 && argumentsForCalculation[2] != 0) decisionCompleteEquation(argumentsForCalculation, result, resultCountSymbols);
	else decisionIncompleteEquation(argumentsForCalculation, result, resultCountSymbols);
}



float* getCoefficients() // ������ �� ����� � ������ � ������
{
	HANDLE fileStart;
	fileStart = CreateFile(L"arguments.txt", // ����������� ����
		GENERIC_READ, // ��������� ��� ������
		FILE_SHARE_READ, // ��� ����������� ������
		NULL, // ������ �� ���������
		OPEN_EXISTING, // ������ ������������ ����
		FILE_ATTRIBUTE_NORMAL, // ������� ����
		NULL); // ��������� ������� ���

	if (fileStart == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, L"��� �������� ����� �������� ������!", L"���� ���������", MB_OK);
		return 0;
	}

	DWORD countFileSymbols;
	LPCSTR argumentsFromFile = calloc(100, sizeof(CHAR));
	if (!ReadFile(fileStart, argumentsFromFile, 100, &countFileSymbols, NULL))
	{
		MessageBox(NULL, L"��� ������ ������ �� ����� �������� ������!", L"���� ���������", MB_OK);
		return 0;
	}

	CloseHandle(fileStart); //��������� ���������� ����������� � �����

	char* argumentsForSplit = calloc(countFileSymbols, sizeof(CHAR)); //�������� ������ ������ �� ��������������� ���������� �������� � ����� (countFileSymbols)
	for (int i = 0; i < (int)(countFileSymbols + 1); i++) //���������� �������� � ����� ���������
	{
		argumentsForSplit[i] = argumentsFromFile[i];
	}
	free(argumentsFromFile); //����������� ������ ���������

	char* argumentsForConverting = strtok(argumentsForSplit, " ");
	float* arguments = calloc(3, sizeof(float));

	for (int i = 0; argumentsForConverting != NULL; i++)
	{
		arguments[i] = atof(argumentsForConverting);
		argumentsForConverting = strtok(NULL, " ");
	}

	return arguments;
}

void decisionCompleteEquation(float* forCoefficients, char* forAnswer, int forAnswerCountSymbols) //���������� ��� ������ ���������� ���������
{
	float discriminant;
	float x1, x2;

	discriminant = pow(forCoefficients[1], 2) - 4 * forCoefficients[0] * forCoefficients[2];

	if (discriminant > 0)
	{
		x1 = (-forCoefficients[1] + sqrt(discriminant)) / (2 * forCoefficients[0]);
		x2 = (-forCoefficients[1] - sqrt(discriminant)) / (2 * forCoefficients[0]);

		forAnswer = calloc(strlen("������������ = %f, x1 = %f, x2 = %f", discriminant, x1, x2), sizeof(char));
		sprintf(forAnswer,"������������ = %f, x1 = %f, x2 = %f", discriminant, x1, x2);
		forAnswerCountSymbols = strlen(forAnswer);
	}
	else if (discriminant == 0)
	{
		x1 = (-forCoefficients[1] + sqrt(discriminant)) / (2 * forCoefficients[0]);

		forAnswer = calloc(strlen("������������ = %f, x1 = %f", discriminant, x1), sizeof(char));
		sprintf(forAnswer,"������������ = %f, x1 = %f", discriminant, x1);
		forAnswerCountSymbols = strlen(forAnswer);
	}
	else
	{
		forAnswer = calloc(strlen("������ ���!"), sizeof(char));
		sprintf(forAnswer, "������ ���!");
		forAnswerCountSymbols = strlen(forAnswer);
	}
	writeResultToFile(forAnswer, forAnswerCountSymbols);
}

void decisionIncompleteEquation(float* forCoefficients, char* forAnswer, int forAnswerCountSymbols) //���������� ��� �������� ���������� ���������
{
	float x1, x2;

	if (forCoefficients[1] == 0 && forCoefficients[2] != 0)
	{
		if (-(forCoefficients[2] / forCoefficients[0]) >= 0)
		{
			x1 = sqrt(-(forCoefficients[2] / forCoefficients[0]));
			x2 = -x1;

			forAnswer = calloc(strlen("�������� ���������� ���������, b = 0, x1 = %f, x2 = %f", x1, x2), sizeof(char));
			sprintf(forAnswer, "�������� ���������� ���������, b = 0, x1 = %f, x2 = %f", x1, x2);
			forAnswerCountSymbols = strlen(forAnswer);
		}
		else
		{
			forAnswer = calloc(strlen("��� ������, �.�. -(c/a) < 0"), sizeof(char));
			sprintf(forAnswer, "��� ������, �.�. -(c/a) < 0");
			forAnswerCountSymbols = strlen(forAnswer);
		}
	}
	else if (forCoefficients[2] == 0 && forCoefficients[1] != 0)
	{
		x1 = 0;
		x2 = -(forCoefficients[1] / forCoefficients[0]);

		forAnswer = calloc(strlen("�������� ���������� ���������, c = 0, x1 = %f, x2 = %f", x1, x2), sizeof(char));
		sprintf(forAnswer, "�������� ���������� ���������, c = 0, x1 = %f, x2 = %f", x1, x2);
		forAnswerCountSymbols = strlen(forAnswer);
	}
	else
	{
		x1 = 0;

		forAnswer = calloc(strlen("�������� ���������� ���������, c = 0 b = 0, x = %f", x1), sizeof(char));
		sprintf(forAnswer, "�������� ���������� ���������, c = 0 b = 0, x = %f", x1);
		forAnswerCountSymbols = strlen(forAnswer);
	}
	writeResultToFile(forAnswer, forAnswerCountSymbols);
}

void writeResultToFile(char* forAnswer, int forCountSymbols) //������ ������ � ����
{
	HANDLE fileResult = CreateFile(L"Result.txt", // ����������� ����
		GENERIC_WRITE, // ����������� ��� ������
		FILE_SHARE_WRITE, // ��������� �� ������������
		NULL, // ������ �� ���������
		CREATE_ALWAYS, // ������������ ������������
		FILE_ATTRIBUTE_NORMAL, // ����������� ����/����� I/O
		NULL); // ��������� ������� ���
	DWORD countFileSymbols;
	WriteFile(fileResult, forAnswer, forCountSymbols, &countFileSymbols, NULL);
	CloseHandle(fileResult);
}