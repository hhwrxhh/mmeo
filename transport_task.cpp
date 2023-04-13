#include <iostream>
#include <vector>
#include <string>

// tested only for balanced tasks

void print(std::vector<std::vector<std::pair<int, int>>> initial,
    std::vector<int> vec_1, std::vector<int> vec_2,
    std::string str_1, std::string str_2)
{
    std::cout << std::endl;
    if (initial.empty())
    {
        std::cout << "Vector is empty" << std::endl;
        return;
    }

    std::vector<std::string> a{ str_1 + std::to_string(1) };
    std::vector<std::string> b{ str_2 + std::to_string(1) };

    for (int i = 2; i <= initial[0].size(); i++)
    {
        a.push_back(str_1 + std::to_string(i));
        b.push_back(str_2 + std::to_string(i));
    }
    a.push_back("Demand");
    b.push_back("Supply");

    std::cout << '\t';
    for (int i = 0; i < a.size(); i++)
        std::cout << b[i] << '\t';

    std::cout << std::endl;

    for (int i = 0; i <= initial.size(); i++)
    {
        std::cout << a[i] << '\t';

        for (int j = 0; j < initial[0].size(); j++)
        {
            if (i == initial.size())
                std::cout << vec_1[j] << '\t';
            else
                std::cout << initial[i][j].first << '\t';
        }
        if (i != initial.size())
            std::cout << vec_2[i] << std::endl;
    }
    std::cout << std::endl << std::endl;
}

std::pair<int, int> find_min(std::vector<std::vector<std::pair<int, int>>> initial) // return the index of min elem in matrix
{
    int min = 999;
    int min_i, min_j;
    for (int i = 0; i < initial.size(); i++)
    {
        for (int j = 0; j < initial[0].size(); j++)
        {
            if (initial[i][j].second == 0 && min > initial[i][j].first)
            {
                min = initial[i][j].first;
                min_i = i;
                min_j = j;
            }
            else if (initial[i][j].second == 0 && min == initial[i][j].first)
                continue;
        }
    }

    std::cout << std::endl;
    std::cout << "Min is -> " << initial[min_i][min_j].first << std::endl;
    return std::make_pair(min_i, min_j);
}

bool is_finished_min_costs(std::vector<int> vec_1, std::vector<int> vec_2, int value)
{
    for (int i = 0; i < vec_1.size(); i++)
    {
        if (vec_1[i] != value || vec_2[i] != value)
            return false;
    }
    return true;
}

void min_cost(std::vector<std::vector<std::pair<int, int>>> initial,
    std::vector<std::vector<std::pair<int, int>>>& result,
    std::vector<int> supply, std::vector<int> demand)
{
    while (true)
    {
        auto tmp = find_min(initial);

        int var_dem = demand[tmp.second];
        int var_sup = supply[tmp.first];
        int min;

        if (var_dem > var_sup)
            min = var_sup;
        else
            min = var_dem;

        if (min == var_dem)
        {
            demand[tmp.second] = 0;
            supply[tmp.first] -= min;

            for (int i = 0; i < demand.size(); i++)
                initial[i][tmp.second].second = 1;
        }
        else
        {
            supply[tmp.first] = 0;
            demand[tmp.second] -= min;

            for (int i = 0; i < supply.size(); i++)
                initial[tmp.first][i].second = 1;
        }

        result[tmp.first][tmp.second].first = min;
        result[tmp.first][tmp.second].second = initial[tmp.first][tmp.second].first;
        print(initial, demand, supply, "a", "b");

        if (is_finished_min_costs(supply, demand, 0))
            break;
    }

    print(result, demand, supply, "a", "b");

    int cost_delivery = 0;
    for (int i = 0; i < result.size(); i++)
    {

        for (int j = 0; j < result[0].size(); j++)
        {
            if (result[i][j].first != 0)
                cost_delivery += result[i][j].first * result[i][j].second;
        }
    }

    std::cout << "The costs of delivery = " << cost_delivery << std::endl;
}

// potential method

bool is_finished_find_potential(std::vector<int> vec_1, std::vector<int> vec_2, int value)
{
    for (int i = 0; i < vec_1.size(); i++)
    {
        if (vec_1[i] == value || vec_2[i] == value)
            return false;
    }
    return true;
}

bool is_finished_potential(std::vector<std::vector<std::pair<int, int>>> result)
{
    for (int i = 0; i < result.size(); i++)
        for (int j = 0; j < result[i].size(); j++)
            // if all marks is positive, potential method has to been stopped
            if (result[i][j].first < 0)
                return false;
    return true;
}

void find_potential(std::vector<std::vector<std::pair<int, int>>>& result,
    std::vector<std::vector<bool>>& visited,
    std::vector<int>& u, std::vector<int>& v)
{
    while (true)
    {
        for (int i = 0; i < result.size(); i++)
        {
            for (int j = 0; j < result[0].size(); j++)
            {
                if (!visited[i][j] && result[i][j].second != 0 && result[i][j].first != 0)
                {
                    if (u[i] != -999 && v[j] == -999)
                    {
                        v[j] = result[i][j].second - u[i];
                        visited[i][j] = true;
                    }
                    else if (v[j] != -999 && u[i] == -999)
                    {
                        u[i] = result[i][j].second - v[j];
                        visited[i][j] = true;
                    }
                }
            }
        }
        if (is_finished_find_potential(u, v, -999))
            break;
    }
}

void find_cycle(std::vector<std::vector<std::pair<int, int>>>& result_cycle)
{
    bool p = true;
    int k = 2;

    while (p) {
        p = false;
        if (k % 2 != 0) {
            // Якщо у рядку матриці A не міститься парної кулькості відмінних від нуля елементів,
            // то елементу, який немає пари присвоюєм значення нуль, тобто вилучаємо його з розгляду
            for (int i = 0; i < result_cycle.size(); i++) {
                int z = 0;
                int jj = -1;
                for (int j = 0; j < result_cycle[i].size(); j++) {
                    if (result_cycle[i][j].first != 0) {
                        z++;
                        jj = j;

                    }
                }
                if (z == 1) {
                    result_cycle[i][jj].first = 0;
                    p = true;
                }
            }
        }
        else {
            // Аналогічним чином, якщо у стовпці матриці A не міститься парної кулькості відмінних від нуля елементів,
            // то елементу, який немає пари присвоюєм значення нуль, і таким чином, також вилучаємо його з розгляду
            for (int j = 0; j < result_cycle.size(); j++) {
                int z = 0;
                int ii = -1;
                for (int i = 0; i < result_cycle[j].size(); i++) {
                    if (result_cycle[i][j].first != 0) {
                        z++;
                        ii = i;
                    }
                }
                if (z == 1) {
                    result_cycle[ii][j].first = 0;
                    p = true;
                }
            }
        }
        k++;
    }

}

void set_signs(std::vector<std::vector<std::pair<int, int>>>& matrix, std::vector<std::vector<char>>& signs, int startRow, int startCol)
{
    int currentRow = startRow;
    int currentCol = startCol;
    char currentSign = '+';
    signs[currentRow][currentCol] = currentSign;
    while (true) {
        int minDistance = INT_MAX;
        int nextRow = -1;
        int nextCol = -1;
        for (int i = 0; i < matrix.size(); i++) {
            for (int j = 0; j < matrix[i].size(); j++) {
                if (matrix[i][j].first != 0 && signs[i][j] == ' ') {
                    int distance = abs(currentRow - i) + abs(currentCol - j);
                    if (distance < minDistance) {
                        minDistance = distance;
                        nextRow = i;
                        nextCol = j;
                    }
                }
            }
        }
        if (nextRow == -1 || nextCol == -1) {
            break;
        }
        if (currentSign == '+') {
            currentSign = '-';
        }
        else {
            currentSign = '+';
        }
        signs[nextRow][nextCol] = currentSign;
        currentRow = nextRow;
        currentCol = nextCol;
    }
}

void cycle_merged(std::vector<std::vector<std::pair<int, int>>>& cycle,
                  std::vector<std::vector<std::pair<int, int>>>& result,
                  std::vector<std::pair<int, int>> index)
{
    for (int i = 0; i < cycle.size(); i++)
    {
        for (int j = 0; j < cycle[i].size(); j++)
        {
            for (int k = 0; k < index.size(); k++)
            {
                if (i == index[k].first && j == index[k].second)
                {
                    result[i][j].first = cycle[i][j].first;
                }
            }
        }
    }
}



int main()
{
    std::vector<std::vector<std::pair<int, int>>> initial, result, result_cycle;
    initial =
    {
        {{7, 0}, {6, 0}, {1, 0}, {4, 0}, {2, 0} , {6, 0}},  // cost for delivery, checked
        {{5, 0}, {3, 0}, {6, 0}, {2, 0}, {7, 0} , {9, 0}},
        {{2, 0}, {7, 0}, {5, 0}, {4, 0}, {7, 0} , {6, 0}},
        {{2, 0}, {6, 0}, {5, 0}, {4, 0}, {8, 0} , {3, 0}},
        {{6, 0}, {2, 0}, {4, 0}, {2, 0}, {8, 0} , {3, 0}},
        {{7, 0}, {3, 0}, {6, 0}, {9, 0}, {4, 0} , {6, 0}}
    };
    result =
    {
        {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} , {0, 0}},  // value(how many product from supplier to user, 
        {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} , {0, 0}},  //cost for delivery(mark)
        {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} , {0, 0}},
        {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} , {0, 0}},
        {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} , {0, 0}},
        {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} , {0, 0}}
    };

    std::vector<int> demand = { 16, 14, 28, 12, 20, 10 };
    std::vector<int> supply = { 10, 15, 5, 20, 36, 14 };

    std::vector<int> u, v;
    u.resize(6);
    v.resize(6);
    std::fill(u.begin(), u.end(), -999);
    std::fill(v.begin(), v.end(), -999);

    u[0] = 0;

    std::vector<std::vector<bool>> visited;
    visited.resize(6, std::vector<bool>(6, false));
    
    std::vector<std::vector<char>> signs;
    signs.resize(6, std::vector<char>(6, ' '));

    min_cost(initial, result, supply, demand);

    // potentila method
    
    // in potential method we only print the first element of pair. If u want to change it, go to print function
    // and change to .second

    std::cout << std::endl;
    std::cout << "Potential method" << std::endl;
   
    int iter = 0;
    int min_value = 999;
    std::pair<int, std::vector<int>> min = { 999, std::vector<int>{999, 999} }; // value min, indexes
    std::vector<std::pair<int, int>> cycle_index; // index of elements in cycle

    while (true)
    {
        std::cout << "ITERATION " << iter << std::endl;
        find_potential(result, visited, u, v);

        for (int i = 0; i < result.size(); i++)
        {
            for (int j = 0; j < result[i].size(); j++)
            {
                if (!visited[i][j])
                {
                    result[i][j].second = initial[i][j].first - u[i] - v[j];
                    int res = result[i][j].second;

                    if (res < 0 && min.first > res && min.first != res)
                    {
                        min.first = res;
                        result[i][j].first = res;
                        result[i][j].second = initial[i][j].first;
                        min.second = { i, j };
                    }
                }
            }
        }

        result_cycle = result;
        find_cycle(result_cycle);

        std::cout << "After finding potentials and marks" << std::endl;
        print(result, v, u, "u", "v");
        print(result, v, u, "u", "v");

        if (is_finished_potential(result))
        {
            std::cout << "The method was stopped" << std::endl;
            break;
        }

        std::cout << "MIN " << min.first << "  Index " << min.second[0] << " " << min.second[1] << std::endl;
                
        print(result_cycle, v, u, "u", "v");

        set_signs(result_cycle, signs, min.second[0], min.second[1]);
        print(result_cycle, v, u, "u", "v");

        for (int i = 0; i < result_cycle.size(); i++)
        {
            for (int j = 0; j < result_cycle[0].size(); j++)
            {
                if (signs[i][j] != ' ')
                    cycle_index.push_back({ i, j });

                if (signs[i][j] == '-')
                    if (min_value > result_cycle[i][j].first)
                        min_value = result_cycle[i][j].first;
            }
        }

        std::cout << "MIN value " << min_value << std::endl;

        for (int i = 0; i < result_cycle.size(); i++)
        {
            for (int j = 0; j < result_cycle[i].size(); j++)
            {
                if (signs[i][j] == '-')
                    result_cycle[i][j].first -= min_value;
                else if (signs[i][j] == '+')
                    result_cycle[i][j].first += min_value;

                if (i == min.second[0] && j == min.second[1])
                    result_cycle[i][j].first = min_value;

            }
        }

        cycle_merged(result_cycle, result, cycle_index);
        print(result, v, u,  "u", "v");

        // reset all variables
        std::fill(result_cycle.begin(), result_cycle.end(), std::vector<std::pair<int, int>>(6, {0, 0}));
        std::fill(signs.begin(), signs.end(), std::vector<char>(6, (6, ' ')));
        std::fill(visited.begin(), visited.end(), std::vector<bool>(6, (6, false)));
        std::fill(u.begin(), u.end(), -999);
        std::fill(v.begin(), v.end(), -999);
        u[0] = 0;
        min = {};

        iter++;
    }

    int sum = 0;
    for (int i = 0; i < result.size(); i++)
        for (int j = 0; j < result[i].size(); j++)
            if (result[i][j].first != 0)
                sum += result[i][j].first * result[i][j].second;

    std::cout << "Genera costs for delivery =  " << sum << std::endl;

}

