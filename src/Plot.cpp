#include "headers/Plot.h"

Plot::Plot() {
    max = 0;
    min = 0;

    graph = cv::Mat(GRAPH_HEIGHT, GRAPH_WIDTH, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::namedWindow("graph");
    cv::moveWindow("graph", 0, 1000);

    data.resize(0);
}

Plot::~Plot() {
}

void Plot::addData(scoreData_t data) {
    if (data.BLAS > max)
        max = data.BLAS;
    if (data.BSS > max)
        max = data.BSS;
    if (data.BAS > max)
        max = data.BAS;

    if (data.MLAS < min)
        min = data.MLAS;
    if (data.MSS < min)
        min = data.MSS;
    if (data.MAS < min)
        min = data.MAS;

    this->data.push_back(data);
}

void Plot::plotData() {
    graph.setTo(cv::Scalar(0, 0, 0));

    float dif = (max - min) * 1.2;
    float scaleY = GRAPH_HEIGHT / dif;
    float scaleX = (float)GRAPH_WIDTH / (data.size() - 1);

    float offset = (max - min) * 0.1;

    for (int i = 1; i < data.size() - 1; i++) {
        cv::line(graph, cv::Point((i - 1) * scaleX, GRAPH_HEIGHT - (data[i - 1].BLAS + offset) * scaleY),
                 cv::Point((i)*scaleX, GRAPH_HEIGHT - (data[i].BLAS + offset) * scaleY),
                 LIGHT_ASSAULT_COLOR);
        cv::line(graph, cv::Point((i - 1) * scaleX, GRAPH_HEIGHT - (data[i - 1].BSS + offset) * scaleY),
                 cv::Point((i)*scaleX, GRAPH_HEIGHT - (data[i].BSS + offset) * scaleY),
                 SNIPER_COLOR);
        cv::line(graph, cv::Point((i - 1) * scaleX, GRAPH_HEIGHT - (data[i - 1].BAS + offset) * scaleY),
                 cv::Point((i)*scaleX, GRAPH_HEIGHT - (data[i].BAS + offset) * scaleY),
                 ASSAULT_COLOR);

        cv::line(graph, cv::Point((i - 1) * scaleX, GRAPH_HEIGHT - (data[i - 1].MLAS + offset) * scaleY),
                 cv::Point((i)*scaleX, GRAPH_HEIGHT - (data[i].MLAS + offset) * scaleY),
                 LIGHT_ASSAULT_COLOR);
        cv::line(graph, cv::Point((i - 1) * scaleX, GRAPH_HEIGHT - (data[i - 1].MSS + offset) * scaleY),
                 cv::Point((i)*scaleX, GRAPH_HEIGHT - (data[i].MSS + offset) * scaleY),
                 SNIPER_COLOR);
        cv::line(graph, cv::Point((i - 1) * scaleX, GRAPH_HEIGHT - (data[i - 1].MAS + offset) * scaleY),
                 cv::Point((i)*scaleX, GRAPH_HEIGHT - (data[i].MAS + offset) * scaleY),
                 ASSAULT_COLOR);
    }

    if (data.size() > 1) {
        cv::line(graph, cv::Point((data.size() - 2) * scaleX, GRAPH_HEIGHT - (data[data.size() - 2].BLAS + offset) * scaleY),
                 cv::Point(GRAPH_WIDTH - 1, GRAPH_HEIGHT - (data[data.size() - 2].BLAS + offset) * scaleY),
                 LIGHT_ASSAULT_COLOR);
        cv::line(graph, cv::Point((data.size() - 2) * scaleX, GRAPH_HEIGHT - (data[data.size() - 2].BSS + offset) * scaleY),
                 cv::Point(GRAPH_WIDTH - 1, GRAPH_HEIGHT - (data[data.size() - 2].BSS + offset) * scaleY),
                 SNIPER_COLOR);
        cv::line(graph, cv::Point((data.size() - 2) * scaleX, GRAPH_HEIGHT - (data[data.size() - 2].BAS + offset) * scaleY),
                 cv::Point(GRAPH_WIDTH - 1, GRAPH_HEIGHT - (data[data.size() - 2].BAS + offset) * scaleY),
                 ASSAULT_COLOR);

        cv::line(graph, cv::Point((data.size() - 2) * scaleX, GRAPH_HEIGHT - (data[data.size() - 2].MLAS + offset) * scaleY),
                 cv::Point(GRAPH_WIDTH - 1, GRAPH_HEIGHT - (data[data.size() - 2].MLAS + offset) * scaleY),
                 LIGHT_ASSAULT_COLOR);
        cv::line(graph, cv::Point((data.size() - 2) * scaleX, GRAPH_HEIGHT - (data[data.size() - 2].MSS + offset) * scaleY),
                 cv::Point(GRAPH_WIDTH - 1, GRAPH_HEIGHT - (data[data.size() - 2].MSS + offset) * scaleY),
                 SNIPER_COLOR);
        cv::line(graph, cv::Point((data.size() - 2) * scaleX, GRAPH_HEIGHT - (data[data.size() - 2].MAS + offset) * scaleY),
                 cv::Point(GRAPH_WIDTH - 1, GRAPH_HEIGHT - (data[data.size() - 2].MAS + offset) * scaleY),
                 ASSAULT_COLOR);
    }

    cv::imshow("graph", graph);
    cv::waitKey(1);
}