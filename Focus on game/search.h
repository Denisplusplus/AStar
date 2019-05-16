#ifndef SEARCH
#define SEARCH

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <queue>
#include <cmath>
#include <cstring>

const int movesX[4] = {0, 0, 1, -1};
const int movesY[4] = {1, -1, 0, 0};
const int movesQuantity = 4; 


typedef struct {
    std::stack<std::pair<int, int>> pathDots;
    int pathLength;
} Response;



class THandler
{
public:
    THandler() {};
    bool ProcessMap(const std::string &fileMap);
    bool Search(const std::string &fileQuaries, bool fullOutput);
    
    ~THandler() {};
private:
	bool isValid(std::pair<int, int> point);
    bool isReachable(std::pair<int, int> point);
    Response AStar(std::pair<int, int> source, std::pair<int, int> destination);

    std::vector<std::vector<int>> Map;
    int mapWidth;
    int mapHeight;

};

int Heuristic(std::pair<int, int> current, std::pair<int, int> destination);


class Comparison {
public:
    bool operator()(const std::pair<std::pair<int, int>, int> &a,const std::pair<std::pair<int, int>, int> &b) {
       return a.second>b.second;
    }
    ~Comparison () {};
};
#endif