#include "headers/Plot.h"

Plot::Plot() {
    max = -10;
    min = 10;

    cv::namedWindow("graph");
    cv::moveWindow("graph", 0, 1000);
    
    graph = cv::Mat(GRAPH_HEIGHT, GRAPH_WIDTH, CV_8UC3, cv::Scalar(0, 0, 0));
   
    cv::imshow("graph", graph);
    cv::waitKey(1);
    
    data.resize(0);
}

Plot::~Plot() {
}

void Plot::addData(scoreData_t info) {
    //----------------MAX----------------
    if (info.BLAS > max)
        max = info.BLAS;
    if (info.BSS > max)
        max = info.BSS;
    if (info.BAS > max)
        max = info.BAS;

    //----------------MIN----------------
    if (info.MLAS < min)
        min = info.MLAS;
    if (info.MSS < min)
        min = info.MSS;
    if (info.MAS < min)
        min = info.MAS;

    this->data.push_back(info);
}

void Plot::plotData() {
    graph.setTo(cv::Scalar(0, 0, 0));
    
    if(data.size() == 0)
        return;

    const float dif = (max - min) * 1.2;
    const float scaleY = GRAPH_HEIGHT / dif;
    const float scaleX = (float)GRAPH_WIDTH / (data.size() - 1);

    const float offset = (max - min) * 0.1 - min;

    for (int i = 1; i < data.size() - 1; i++) {
        //----------------BEST----------------
        cv::line(graph, cv::Point((i - 1) * scaleX, GRAPH_HEIGHT - (data[i - 1].BLAS + offset) * scaleY),
                 cv::Point((i)*scaleX, GRAPH_HEIGHT - (data[i].BLAS + offset) * scaleY),
                 LIGHT_ASSAULT_COLOR);
        cv::line(graph, cv::Point((i - 1) * scaleX, GRAPH_HEIGHT - (data[i - 1].BSS + offset) * scaleY),
                 cv::Point((i)*scaleX, GRAPH_HEIGHT - (data[i].BSS + offset) * scaleY),
                 SNIPER_COLOR);
        cv::line(graph, cv::Point((i - 1) * scaleX, GRAPH_HEIGHT - (data[i - 1].BAS + offset) * scaleY),
                 cv::Point((i)*scaleX, GRAPH_HEIGHT - (data[i].BAS + offset) * scaleY),
                 ASSAULT_COLOR);

        //----------------MEDIUN----------------
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
        //----------------BEST----------------
        cv::line(graph, cv::Point((data.size() - 2) * scaleX, GRAPH_HEIGHT - (data[data.size() - 2].BLAS + offset) * scaleY),
                 cv::Point(GRAPH_WIDTH - 1, GRAPH_HEIGHT - (data[data.size() - 2].BLAS + offset) * scaleY),
                 LIGHT_ASSAULT_COLOR);
        cv::line(graph, cv::Point((data.size() - 2) * scaleX, GRAPH_HEIGHT - (data[data.size() - 2].BSS + offset) * scaleY),
                 cv::Point(GRAPH_WIDTH - 1, GRAPH_HEIGHT - (data[data.size() - 2].BSS + offset) * scaleY),
                 SNIPER_COLOR);
        cv::line(graph, cv::Point((data.size() - 2) * scaleX, GRAPH_HEIGHT - (data[data.size() - 2].BAS + offset) * scaleY),
                 cv::Point(GRAPH_WIDTH - 1, GRAPH_HEIGHT - (data[data.size() - 2].BAS + offset) * scaleY),
                 ASSAULT_COLOR);

        //----------------MEDIUN----------------
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
