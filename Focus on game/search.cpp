#include "search.h"

bool THandler::ProcessMap(const std::string &fileMap) {
    
    std::ifstream inputFile(fileMap);

    if (!inputFile.is_open()) {
        std::cout << "Error while open file: " << fileMap << std::endl;
        return false;
    }

    std::string string, type;
    char sign;

    while(inputFile >> string) {
        if (string == "type") {
            inputFile >> type;
        } else if (string == "height") {
            inputFile >> this->mapHeight;
        } else if (string == "width") {
            inputFile >> this->mapWidth;
        } else if (string == "map") {
            this->Map.resize(this->mapWidth, std::vector<int>(this->mapHeight));

            for (int i = 0; i < this->mapWidth; i++) {
               for (int j = 0; j < this->mapHeight; j++) {
                   inputFile >> sign;
                   this->Map[i][j] = (sign == '.' || sign == 'G') ? 1 : 0;
                }
            }
        }
     }


    /*for (int i = 0; i < mapWidth; i++) {
        for (int j = 0; j < mapWidth; j++) {
            std::cout << this->Map[i][j] << " ";
        }
        std::cout << std::endl;
    }*/
    return true;
}

bool THandler::Search(const std::string &fileQuaries, bool fullOutput) {

    std::ifstream inputFile(fileQuaries);

    if (!inputFile.is_open()) {
        std::cout << "Error while open file: " << fileQuaries << std::endl;
        return false;
    }

    int x1, x2, y1, y2;
    while (inputFile >> x1 >> y1 >> x2 >> y2) {
        std::pair<int, int> source(x1 - 1, y1 - 1);
        std::pair<int, int> destination(x2 - 1, y2 - 1);

        if (!isValid(source) || !isValid(destination)) continue;

        Response response = AStar(source, destination);

        int pathLength = response.pathLength;
        std::cout << pathLength << std::endl;
        
        if (!fullOutput) continue;
        
        while (!response.pathDots.empty()) {
            std::pair<int, int> point = response.pathDots.top();
            std::cout << " " << point.first + 1 << " " << point.second + 1 << '\n';
            response.pathDots.pop(); 
        }
    }
    return true;
}



Response THandler::AStar(std::pair<int, int> source, std::pair<int, int> destination) {

    std::priority_queue<std::pair   <std::pair<int, int>, int>, 
                        std::vector <std::pair<std::pair<int, int>, int>>,
                        Comparison
                        > priorityQueue;

    std::vector<std::vector<int>> distance(this->mapWidth, std::vector<int>(this->mapHeight, 0));
    std::vector<std::vector<std::pair<int, int>>> parent(this->mapWidth, std::vector<std::pair<int, int>>(this->mapHeight));

    distance[source.first][source.second] = 0;
    priorityQueue.emplace(source, 0);
    
    Response response;
    response.pathLength = -1;
 
    while (!priorityQueue.empty()) {
        std::pair<int, int> pointLower = priorityQueue.top().first;
        priorityQueue.pop();

        if (pointLower == destination) {
            response.pathLength = distance[pointLower.first][pointLower.second];
            while (pointLower != source) {
                response.pathDots.push(pointLower);
                pointLower = parent[pointLower.first][pointLower.second];
            }
            response.pathDots.push(source);
            return response;
        }

        for (int move = 0; move < movesQuantity; move++) {

            std::pair<int, int> neighbourPoint(pointLower.first + movesX[move],  pointLower.second + movesY[move]);

            if (!isReachable(neighbourPoint)) continue;

            double h = Heuristic(neighbourPoint, destination);
            double g = distance[pointLower.first][pointLower.second] + 1;

            if (!distance[neighbourPoint.first][neighbourPoint.second] || g < distance[neighbourPoint.first][neighbourPoint.second]) {
                distance[neighbourPoint.first][neighbourPoint.second] = g;
                parent[neighbourPoint.first][neighbourPoint.second] = pointLower;
                priorityQueue.emplace(neighbourPoint, h + g);
            } 
            //continue;
        }

    }
    return response;
}


int Heuristic(std::pair<int, int> current, std::pair<int, int> destination) {
    return abs(current.first - destination.first) + abs(current.second - destination.second);
}


bool THandler::isReachable(std::pair<int, int> point) {
    return (isValid(point) && (this->Map[point.first][point.second])) ? true : false;
}


bool THandler::isValid(std::pair<int, int> point) {
    if (point.first  > this->mapWidth  - 1 || point.first  < 0 || 
        point.second > this->mapHeight - 1 || point.second < 0 ) {
        return false;
    }
    return true;
}


