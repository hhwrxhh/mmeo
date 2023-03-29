#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <iomanip>   
#include <Windows.h>   

// not tested witn >= or =
int main()
{
	std::cout << std::setprecision(3);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int num_variables = 2; // x1 x2
	std::vector<std::string> variables{"x1", "x2"};

	int num_equation = 3;
	
	int columns_initial = 2;
	int rows_initial = num_equation + 1; // 1 is f(x)

	std::vector<std::vector<double>> initial
	{
		{5, -2}, // coefficien near x in equation
		{-1, 1},
		{1, 1},
		{-1, -1} // f(x)
	};
	// f(x) = x1 + x2 - in vector should be with opposite sing
	std::vector<double> b{ 7, 5, 6, 0 };

	std::vector<std::vector<double>> unit; // it should be filled by yourself 
	unit.resize(num_equation + 1, std::vector<double>(num_equation + 1));

	std::vector<std::vector<double>> result, result_1;
	result.resize(10, std::vector<double>(10));
	result_1.resize(10, std::vector<double>(10));
	std::fill(result.begin(), result.end(), std::vector<double>(20, 0));
	std::fill(result_1.begin(), result_1.end(), std::vector<double>(20, 0));

	for (int i = 0; i < num_equation; i++) // 3 - number of equation
		unit[i][i] = 1;

	int columns_result = columns_initial + 4; // 3 equation and 1 b
	std::cout << "Col Res " << columns_result << std::endl;
	std::cout << std::endl;

	std::vector<std::pair<std::string, int>> basis; // name of variables and index
	std::vector<std::string> symbols_table;
	symbols_table.resize(columns_result + 1); // 1 is for basis

	symbols_table[0] = "basis";
	for (int i = 1; i < symbols_table.size() - 1; i++)
	{
		symbols_table[i] = "x" + std::to_string(i);

		if (i > columns_initial)
			basis.push_back(std::make_pair(symbols_table[i], i));
	}
	basis.push_back(std::make_pair("max", 999));
	symbols_table[symbols_table.size() - 1] = "b";

	for (int i = 0; i < rows_initial; i++)
	{
		for (int j = 0; j < columns_result; j++)
		{
			
			if (j >= columns_initial)
				result[i][j] = unit[i][j - columns_initial];
			else
				result[i][j] = initial[i][j];

			if (j == columns_result - 1)
				result[i][j] = b[i];
		}
	}

	std::vector<double> line; // line that has min element of b in row
	std::map<std::string, std::pair<double, int>> min_elements;
	std::pair<double, int> min_elements_pair; // value index

	while (true)
	{
		double min = 999.;

		for (int i = 0; i < columns_result; i++)
		{
			if (min > result[rows_initial - 1][i])
			{
				min = result[rows_initial - 1][i];
				min_elements_pair = std::make_pair(min, i);
			}
		}
		min_elements.emplace("min_columns", min_elements_pair);
		auto min_col = min_elements.find("min_columns");

		min = 999;
		for (int i = 0; i < rows_initial; i++)
		{
			//std::cout << "Division: ";
			double res = result[i][columns_result - 1] / result[i][min_col->second.second];
			if (min > res && res > 0)
			{
				min = res;
				min_elements_pair = std::make_pair(min, i);
			}
		}
		min_elements.emplace("min_rows", min_elements_pair);

		min_col = min_elements.find("min_columns");
		auto min_row = min_elements.find("min_rows");


		for (int i = 0; i < columns_result; i++)
		{
			result_1[min_row->second.second][i] = result[min_row->second.second][i]; 
			line.push_back(result_1[min_row->second.second][i]);
		}

		std::cout << std::endl;
		for (int i = 0; i < symbols_table.size(); i++)
		{
			std::cout << symbols_table[i] << '\t';
		}
		std::cout << std::endl;

		for (int i = 0; i < rows_initial; i++)
		{
			std::cout << basis[i].first << '\t';
			for (int j = 0; j < columns_result; j++)
			{
				if (j == min_col->second.second && i == min_row->second.second)
				{
					SetConsoleTextAttribute(hConsole, 207);
					std::cout << result[i][j] << '\t';
				}
				else
				{
					SetConsoleTextAttribute(hConsole, 15);
					std::cout << result[i][j] << '\t';
				}
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;
		for (int i = 0; i < rows_initial; i++)
		{
			if (i != min_row->second.second)
			{
				for (int j = 0; j < columns_result; j++)
				{
					double n = result[i][min_col->second.second] / result[min_row->second.second][min_col->second.second];
					result_1[i][j] = result[i][j] - line[j] * n;
				}
			}
		}
		for (int i = 0; i < rows_initial; i++)
		{
			for (int j = 0; j < columns_result; j++)
			{
				if (i == min_row->second.second)
				{
					result[i][j] = line[j] / result_1[min_row->second.second][min_col->second.second];
				}
			}
		}
		std::string tmp = "x" + std::to_string(min_col->second.second + 1);
		basis[min_row->second.second] = std::make_pair(tmp, min_row->second.second);
		std::cout << std::endl;
		

		bool good = true;
		for (int i = 0; i < columns_result; i++)
		{
			if (result_1[rows_initial - 1][i] < 0)
			{
				good = false;
			}
		}
		for (int i = 0; i < rows_initial; i++)
			for (int j = 0; j < columns_result; j++)
			{
				if (i == min_row->second.second)
				{
					result[i][j] = line[j] / result_1[min_row->second.second][min_col->second.second];
				}
				else {
					result[i][j] = result_1[i][j];
				}
			}

		line.clear();
		min_elements.clear();

		if (!good)
			std::cout << "Do it again" << std::endl;
		else
			break;
	}

	std::cout << "Final Result" << std::endl;
	for (int i = 0; i < symbols_table.size(); i++)
	{
		std::cout << symbols_table[i] << '\t';
	}
	std::cout << std::endl;

	for (int i = 0; i < rows_initial; i++)
	{
		std::cout << basis[i].first << '\t';
		for (int j = 0; j < columns_result; j++)
		{
			std::cout << result[i][j] << '\t';
		}
		std::cout << std::endl;
	}

	for (int i = 0; i < rows_initial; i++)
	{
		auto it = std::find(basis.begin(), basis.end(), basis[i]);

		for (int j = 0; j < num_variables; j++)
		{
			if (basis[i].first == variables[j])
			{
				int index = it - basis.begin();
				std::cout << basis[i].first << " = " << result[index][columns_result - 1] << std::endl;
			}
		}
	}

}

