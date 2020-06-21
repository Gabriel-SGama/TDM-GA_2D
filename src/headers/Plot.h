#pragma once

#include "Evolution.h"

#define GRAPH_WIDTH 1000 
#define GRAPH_HEIGHT 400 

class Plot
{
private:
    float max;
    float min;
    
    cv::Mat graph;
    std::vector<scoreData_t> data;

public:
    Plot();
    ~Plot();

    void addData(scoreData_t data);
    void plotData();
};
