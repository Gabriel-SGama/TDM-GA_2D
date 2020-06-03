#include "headers/Graph.h"

Graph::Graph() {
    std::string windowName = "fit graph";
    graph = cv::Mat(GRAPH_HEIGHT, GRAPH_LENGHT, CV_8UC3, GRAPH_BACKGROUND_COLOR);
}

Graph::~Graph() {
}

void Graph::clear() {
}

void Graph::addBestScores(topScore_t topScore) {
}

void Graph::showGraphAll() {
}

void Graph::showGraph() {
}
