#ifndef SCREEN_H
#define SCREEN_H

#include <opencv2/opencv.hpp>

class Screen;

//screen dimensions
#define LENGTH 1800
#define HEIGHT 800
#define LIMIT_SIZE 5
#define OFFSET_LIMIT 2.5

#include "Player.h"

const int _OBSTACLE_LENGTH = RADIUS * 2;                       //size of obstacles
const cv::Scalar BACKGROUND_COLOR = cv::Scalar(255, 255, 255); //white
const cv::Scalar OBSTACLE_COLOR = cv::Scalar(19, 69, 139);     //brown
const cv::Scalar OBSTACLE_RAY = cv::Scalar(20, 69, 139);       //brown ray

class Screen
{
private:
    cv::Mat map; //ópecv image

    std::string windowName;

    uchar **imgMatrix; //matrix to faster randon access

    //map parameters
    int rows;
    int cols;

public:
    Screen();
    ~Screen();

    void setScreenParam(std::string name);

    int colorToId(cv::Scalar color); //transforms color to id

    void matToMatrix(); //set ptrs of matrix
    void resetImage();  //set BACKGROUND_COLOR

    void updateMap();      //shows map
    void createObstacle(); //draw obstacles

    cv::Scalar getColor(cv::Point pt);       //get point color
    cv::Scalar colorToRay(cv::Scalar color); //transforms color to ray color
    cv::Scalar idToRay(int rayId);           //transforms id to ray color

    //gets:
    cv::Mat getMap();

    uchar **getMatrix();
};

#endif