#include <iostream>
#include <vector>
#include <map>
#include <algorithm>#include <limits>

std::multimap<int, double> company_1{ {2, 0.5}, {4, 0.8}, {3, 1.1}, {2, 0.6}, {5, 1.3} };
std::multimap<int, double> company_2{ {1, 0.3}, {3, 0.6}, {1, 0.4}, {5, 1.5}, {3, 0.8} };
std::multimap<int, double> company_3{ {1, 0.3}, {2, 0.4}, {1, 0.4}, {5, 1.5}, {4, 1} };

std::vector<std::multimap<int, double>> a;
std::vector < std::pair<double, double>> b;
int investmnet = 8;
   
std::vector<std::vector<double>> generate_triangle_matrix(int size)
{
    std::vector<std::vector<double>> matrix(size, std::vector < double >(size, -99));

    for (int i = 0; i < matrix.size(); i++)
        for (int j = i; j < matrix[i].size(); j++)
            matrix[j][i] = 0;

    return matrix;
}
std::pair<double, double> max_profit(std::vector<std::pair<double, double>> result)
{
    double maxFirst = std::numeric_limits<double>::lowest();
    double minSecond = std::numeric_limits<double>::max();

    for (const auto& pair : result) {
        if (pair.second <= investmnet) {
            if (pair.first > maxFirst) {
                maxFirst = pair.first;
                minSecond = pair.second;
            }
            else if (pair.first == maxFirst && pair.second < minSecond) {
                minSecond = pair.second;
            }
        }
    }
    investmnet -= minSecond;
    return std::make_pair(maxFirst, minSecond);
}

void print_matrix(std::vector <std::vector<double>> matrix)
{
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
        {
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}
void calculate_matrix(std::vector <std::vector<double>>& matrix, int company)
{
    for (int i = 0; i < investmnet; i++)
    {
        auto it = a[company].find(i);
        std::multimap<double, double> tmp;
        if (it != a[company].end())
        {
            if (a[company].count(i) > 1)
            {
                const auto& [start, end] = a[company].equal_range(it->first);
                //// Get max element and show result
                double max_elem = static_cast<double>(std::max_element(start, end,
                    [](const auto i1, const auto i2) {return i1.second < i2.second; })->second);
                for (int j = i, k = 0; j < matrix[i].size(); j++, k++)
                    matrix[j][i] = max_elem + b[k].first;
            }
            else if (a[company].count(i) == 1)
            {
                for (int j = i, k = 0; j < matrix[i].size(); j++, k++)
                    matrix[j][i] = it->second + b[k].first;
            }
        }
        else
        {
            for (int j = i, k = 0; j < matrix[i].size(); j++, k++)
                matrix[j][i] = b[k].first;
        }
    }
}

void calculate_first_cos(std::vector<std::pair<int, std::pair<double, double>>>& result)
{
    std::vector <std::pair<double, double>> tmp;

    for (int i = 0; i <= investmnet; i++)
    {
        auto it = company_1.find(i);
        if (it != company_1.end())
        {
            if (company_1.count(i) > 1)
            {
                const auto& [start, end] = company_1.equal_range(it->first);
                double max_elem = static_cast<double>(std::max_element(start, end,
                    [](const auto i1, const auto i2) {return i1.second < i2.second; })->second);

                tmp.push_back(std::make_pair(max_elem, it->first));
            }
            else if (company_1.count(i) == 1)
                tmp.push_back(std::make_pair(it->second, it->first));
        }
        else
            tmp.push_back(std::make_pair(0, 0));
    }

    result.reserve(tmp.size());

    for (const auto& pair : tmp)
        result.emplace_back(0, std::make_pair(pair.first, pair.second));
}
void calculate_cost(std::vector<std::pair<int, std::pair<double, double>>>& result,
                    std::vector <std::vector<double>>& matrix, int company)
{
    for (size_t i = 0; i < matrix.size(); ++i) {
        const auto& row = matrix[i];
        auto maxElement = std::max_element(row.begin(), row.end());
        double maxValue = *maxElement;
        double maxIndex = std::distance(row.begin(), maxElement);
        result.emplace_back(company, std::make_pair(maxValue, maxIndex));
    }
}

int main()
{
    a.push_back(company_1);
    a.push_back(company_2);
    a.push_back(company_3);

    std::vector<std::pair<int, std::pair<double, double>>> result; // index f(c) c

    std::vector <std::vector<double>> matrix = generate_triangle_matrix(investmnet + 1);
    
    for (int company = 0; company < 3; company++)
    {
        std::cout << "COMPANY " << company + 1 << std::endl << std::endl;
        if (company == 0)
        {
            calculate_first_cos(result);
        }
        else
        {
            // get the cost from previous company
            for (const auto& pair : result)
            {
                if (pair.first == company - 1)
                    b.push_back(pair.second);
            }
            calculate_matrix(matrix, company);
            print_matrix(matrix);
            calculate_cost(result, matrix, company);

            matrix = generate_triangle_matrix(investmnet + 1);
            b.clear();
        }
            
        std::cout << "f(c)" << "\tc" << std::endl;
        for (int j = result.size() - 1; j >= 0; j--)
        {
            if (result[j].first == company)
                std::cout << result[j].second.first << "\t" << result[j].second.second << std::endl;
        }
        std::cout << "-_--_--_--_--_--_--_--_-" << std::endl;
        std::cout << std::endl;
    }

    for (int i = 2; i >= 0; i--)
    {
        for (int j = result.size() - 1; j >= 0; j--)
        {
            if (result[j].first == i)
                b.push_back(result[j].second);
        }
        auto pair_res = max_profit(b);
        std::cout << "For company " << i + 1<< " investments will be " << pair_res.second << 
                                               " and profit " << pair_res.first << std::endl;
        b.clear();
    }
    return 0;
}