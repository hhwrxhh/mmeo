#include <iostream>
#include <string>
#include <vector>
#include <utility>

const int size = 5;

struct Node
{
    Node(int Value, bool Visited, bool Include_edge,
        std::pair<int, int> Max_position,
        std::vector<std::vector<std::pair<int, int>>> Matrix,
        std::vector<int> Di, std::vector<int> Dj,
        std::vector<int> Row, std::vector<int> Column)
        :value(Value), visited(Visited), include_edge(Include_edge),
        max_position(Max_position), matrix(Matrix), di(Di), dj(Dj),
        row(Row), column(Column)
    {
    }

    int value;
    bool visited, include_edge;
    std::vector<std::vector<std::pair<int, int>>> matrix;
    std::pair<int, int> max_position;
    std::vector<int> di, dj, row, column;
};

void print(std::vector<std::vector<std::pair<int, int>>> matrix)
{
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
        {
            std::cout << matrix[i][j].first << "(" << matrix[i][j].second << ")\t";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

std::pair<int, int> find_max_mark(std::vector<std::vector<std::pair<int, int>>> matrix)
{
    std::pair<int, int> max_position;
    int max_mark = -1;
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
        {
            if (max_mark < matrix[i][j].second && i != j)
            {
                max_position = { i, j };
                max_mark = matrix[i][j].second;
            }
        }
    }
    return max_position;
}

void find_marks(std::vector<std::vector<std::pair<int, int>>>& matrix)
{
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
        {
            if (matrix[i][j].first == 0)
            {
                int min_row = 999;
                int min_column = 999;

                for (int k = 0; k < matrix[i].size(); k++)
                    if (min_row > matrix[i][k].first && j != k)
                        min_row = matrix[i][k].first;

                for (int k = 0; k < matrix[i].size(); k++)
                    if (min_column > matrix[k][j].first && i != k)
                        min_column = matrix[k][j].first;

                matrix[i][j].second = min_row + min_column;
            }
        }
    }
}

int calculation_route(std::vector<std::vector<std::pair<int, int>>>& matrix,
    std::vector<int>& di, std::vector<int>& dj)
{
    int min_row = 999;
    int min_column = 999;

    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
        {
            if (min_row > matrix[i][j].first)
                min_row = matrix[i][j].first;
        }
        if (min_row != 999)
        {
            for (int j = 0; j < matrix[i].size(); j++)
                if (i != j)
                    matrix[i][j].first -= min_row;

            di[i] = min_row;
            min_row = 999;
        }
    }

    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
        {
            if (min_column > matrix[j][i].first)
                min_column = matrix[j][i].first;
        }
        if (min_column != 999)
        {
            for (int j = 0; j < matrix[i].size(); j++)
                if (i != j)
                    matrix[j][i].first -= min_column;

            dj[i] = min_column;
            min_column = 999;
        }
    }

    int h = 0;
    for (int i = 0; i < dj.size(); i++)
        h += di[i] + dj[i];

    std::cout << h << std::endl;
    return h;
}

std::pair<int, int> local_bound(std::vector<Node>& vec, int i) // return local bound for 
{
    std::vector<std::vector<std::pair<int, int>>> vec_include = vec[i].matrix;
    int general_incl_route, general_not_incl_route;

    if (vec[i].visited)
    {
        // for NOT INCLUDE edge
        vec[i].matrix[vec[i].max_position.first][vec[i].max_position.second] = std::make_pair(99, 99);
        int current_route = calculation_route(vec[i].matrix, vec[i].di, vec[i].dj);
        general_not_incl_route = current_route + vec[0].value;

        Node not_incl_node(general_not_incl_route, 0, 0, vec[i].max_position, vec[i].matrix, 
                            vec[i].di, vec[i].dj, vec[i].row, vec[i].column);
        vec.push_back(not_incl_node);


        // for INCLUDE edge
        std::pair<int, int> pair_tmp = vec[i].max_position; // change the name
        std::vector<int> di(vec_include.size(), 0), dj(vec_include.size(), 0);
        std::vector<int> row = vec[i].row, column = vec[i].column;

        
        //vec_include[pair_tmp.second][pair_tmp.first] = { 99, 99 };
        // to exclude the formation of a non - Hamiltonian cycle.
        if (row[pair_tmp.first] == column[pair_tmp.first])
        {
            if (column[pair_tmp.second] == row[pair_tmp.second])
            {
                row.erase(row.begin() + pair_tmp.first);
                column.erase(column.begin() + pair_tmp.second);
                vec_include[pair_tmp.second][pair_tmp.first] = { 99, 99 };
            }
        }

        // row delete
        if (vec_include.size() > pair_tmp.first)
            vec_include.erase(vec_include.begin() + pair_tmp.first);

        // column delete
        for (int j = 0; j < vec_include.size(); j++)
            if (vec_include[j].size() > pair_tmp.second)
                vec_include[j].erase(vec_include[j].begin() + pair_tmp.second);


        int tmp = calculation_route(vec_include, di, dj);
        general_incl_route = tmp + vec[i].value;
        std::cout << tmp << " + " << vec[i].value << " = " << tmp + vec[i].value << std::endl;

        Node include_node(general_incl_route, 0, 0, vec[i].max_position, 
                          vec_include, di, dj, row, column);
        vec.push_back(include_node);

        return { general_not_incl_route, general_incl_route };
    }
    
    return { -99, -99 };
}

std::pair<int, int> helping(std::vector<Node>& vec, int i)
{
    vec[i].include_edge = true;
    vec[i].visited = true;

    find_marks(vec[i].matrix);
    vec[i].max_position = find_max_mark(vec[i].matrix);

    return local_bound(vec, i);
}

void branch_and_bound(std::vector<Node>& vec)
{
	
	// TODO: add breakpoint for stopping this loop
	// need while true, not for i < 7
	// And figure out about last steps
	// And cout the final result(lenght and cities)
	
    std::vector<int> vect;
    find_marks(vec[0].matrix);
    vec[0].max_position = find_max_mark(vec[0].matrix);

    auto pair_compare = local_bound(vec, 0);
    for (int i = 2; i < 7; i += 2)
    {
        if (pair_compare.first != -99)
        {
            if (pair_compare.first < pair_compare.second)
                pair_compare = helping(vec, i - 1);
            else if (pair_compare.first > pair_compare.second)
                pair_compare = helping(vec, i);
            if (pair_compare.first == pair_compare.second)
                pair_compare = helping(vec, i);
        }
        else
            std::cout << "SOme problems" << std::endl;
    }

}

int main()
{
    std::vector<std::vector<std::pair<int, int>>> initial =
    {
        {{99, 99},  {20, 0},     {5, 0},    {5, 0},     {8, 0}}, // value(cost), mark
        {{8, 0},   {99, 99},     {9, 0},   {12, 0},    {18, 0}}, // 99 is huge number
        {{14, 0},    {5, 0},   {99, 99},    {4, 0},    {13, 0}},
        {{13, 0},   {13, 0},    {18, 0},  {99, 99},     {4, 0}},
        {{12, 0},    {3, 0},    {16, 0},   {13, 0},   {99, 99}}
    };
    std::vector<int> di(size, 0), dj(size, 0), row, column;

    for (int i = 0; i < size; i++)
    {
        row.push_back(i);
        column.push_back(i);
    }
    print(initial);

    int length_route = calculation_route(initial, di, dj);
    print(initial);

    find_marks(initial);
    auto max_mark_position = find_max_mark(initial);
    print(initial);

    std::vector<Node> a;
    Node tmp(length_route, 1, 0, max_mark_position, initial, di, dj, row, column);
    a.push_back(tmp);

    branch_and_bound(a);
}