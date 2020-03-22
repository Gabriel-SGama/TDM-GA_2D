#ifndef SCREEN_H
#define SCREEN_H

#include <opencv2/opencv.hpp>

class Screen;

#define LENGTH 800
#define HEIGHT 600

#include "Player.h"

const int _OBSTACLE_LENGTH = RADIUS * 2;
const cv::Scalar BACKGROUND_COLOR = cv::Scalar(255, 255, 255); //white
const cv::Scalar OBSTACLE_COLOR = cv::Scalar(19, 69, 139);     //brown
const cv::Scalar OBSTACLE_RAY = cv::Scalar(20, 69, 139);       //brown ray

class Screen
{
private:
    cv::Mat map;

    std::string windowName;

    uchar **imgMatrix;

    int rows;
    int cols;

public:
    Screen();
    ~Screen();

    int colorToId(cv::Scalar color);

    void matToMatrix();
    void resetImage();

    void updateMap();
    void createObstacle();

    cv::Scalar getColor(cv::Point pt);
    cv::Scalar colorToRay(cv::Scalar color);
    cv::Scalar idToRay(int rayId);

    cv::Mat getMap();

    uchar **getMatrix();
};

#endif