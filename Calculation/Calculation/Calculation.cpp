//#include "stdafx.h"
#include"Spreadsheet/Spreadsheet.h"
#include <iostream>
#include <omp.h>
#include <algorithm>
#include <cstdlib>
#include <string>
using namespace std;

string Calculation(string expression)
{
	string leftOperand;
	string rightOperand;
	string op;
	float result;

	for (int i = 0; i < expression.length(); i++) {

		if (isdigit(expression[i])) {
			if (op.empty()) {
				leftOperand += expression[i];
				continue;
			}
			rightOperand += expression[i];
			continue;
		}
		op = expression[i];
	}

	if (0 == op.compare("+"))
		result = stof(leftOperand) + stof(rightOperand);
	else if (0 == op.compare("-"))
		result = stof(leftOperand) - stof(rightOperand);
	else if (0 == op.compare("/"))
		result = stof(leftOperand) / stof(rightOperand);
	else if (0 == op.compare("*"))
		result = stof(leftOperand) * stof(rightOperand);

	return to_string(result);

}
void Spreadsheets()
{
	int numOfRows = 10, numOfCols = 4;
	Spreadsheet spreadsheet;
	spreadsheet.generate(numOfRows, numOfCols, spreadsheet);
	spreadsheet.printSpreadsheet();
	string expression;

#pragma omp parallel
	{
#pragma omp for 
		for (int i = 0; i < numOfRows; i++)
		{
			for (int j = 0; j < numOfCols; j++)
			{
				expression = spreadsheet[i][j];
				spreadsheet[i][j] = Calculation(expression);
			}
		}
	}
	spreadsheet.printSpreadsheet();
}

int main()
{
	Spreadsheets();
	system("pause");
	return 0;
}