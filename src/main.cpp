#include <iostream>
#include <string>
#include <vector>
#include <utility>

const int size = 5;

struct Node
{
    Node(int Value, bool Visited, std::vector<std::vector<std::pair<int, int>>> Matrix,
        std::vector<int> Di, std::vector<int> Dj)
        :value(Value), visited(Visited), matrix(Matrix), di(Di), dj(Dj)
    {
    }

    int value;
    bool visited;
    std::vector<std::vector<std::pair<int, int>>> matrix;
    std::vector<int> di, dj;
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

std::pair<int, int> find_marks(std::vector<std::vector<std::pair<int, int>>>& matrix)
{
    std::pair<int, int> max_position;
    int max_mark = -1;

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
                if (max_mark < matrix[i][j].second)
                {
                    max_position = { i, j };
                    max_mark = matrix[i][j].second;
                }
            }
        }
    }

    return max_position;
}

void making_reduction(std::vector<std::vector<std::pair<int, int>>>& matrix,
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
}

int main()
{
    std::vector<std::vector<std::pair<int, int>>> initial =
    {
        {{99, 99},  {20, 0},   {5, 0},  {5, 0},   {8, 0}}, // value(cost), mark
        {{8, 0},   {99, 99},   {9, 0}, {12, 0},  {18, 0}}, // 99 is huge number
        {{14, 0},  {5, 0},   {99, 99},  {4, 0},  {13, 0}},
        {{13, 0}, {13, 0},  {18, 0},  {99, 99},   {4, 0}},
        {{12, 0},  {3, 0},  {16, 0}, {13, 0},   {99, 99}}
    };
    std::vector<int> di(size, 0), dj(size, 0);

    print(initial);

    making_reduction(initial, di, dj);
    print(initial);

    find_marks(initial);
    print(initial);

    std::vector<Node*> a;
    // need min value for two nodes
}