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
}

void find_marks(std::vector<std::vector<std::pair<int, int>>>& matrix)
{

}

void making_reduction(std::vector<std::vector<std::pair<int, int>>> matrix)
{

}

int main()
{
	std::vector<std::vector<std::pair<int, int>>> initial = 
    {
        {{0, 0},  {20, 0},   {5, 0},  {5, 0},   {8, 0}}, // value(cost), mark
        {{8, 0},   {0, 0},   {9, 0}, {12, 0},  {18, 0}},
        {{14, 0},  {5, 0},   {0, 0},  {4, 0},  {13, 0}},
        {{13, 0}, {13, 0},  {18, 0},  {0, 0},   {5, 0}},
        {{13, 0},  {3, 0},  {16, 0}, {13, 0},   {0, 0}}
    };

    std::vector<int> di(size, 0), dj(size, 0);


    print(initial);

    std::vector<Node*> a;
    // need min value for two nodes
}