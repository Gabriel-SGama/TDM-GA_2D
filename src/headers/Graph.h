#pragma once
#include "Evolution.h"

#define GRAPH_LENGHT 600
#define GRAPH_HEIGHT 400

const cv::Scalar GRAPH_BACKGROUND_COLOR = cv::Scalar(0, 0, 0);
const cv::Scalar LA_Color = cv::Scalar(0, 255, 0);
const cv::Scalar S_Color = cv::Scalar(0, 0, 255);
const cv::Scalar A_Color = cv::Scalar(255, 0, 0);

class Graph {
   private:
    cv::Mat graph;

    std::vector<float> BLAS_List;
    std::vector<float> BSS_List;
    std::vector<float> BAS_List;

   public:
    Graph();
    ~Graph();

    void clear();
    void addBestScores(topScore_t topScore);
    void showGraphAll();
    void showGraph();
};
