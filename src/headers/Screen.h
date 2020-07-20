#ifndef SCREEN_H
#define SCREEN_H

class Screen;

//screen dimensions
#define LENGTH 801
#define HEIGHT 800
#define LIMIT_SIZE 2
const int OFFSET_LIMIT = LIMIT_SIZE; 

#include "Player.h"

const int _OBSTACLE_LENGTH = RADIUS * 2;  //size of obstacles

const cv::Scalar BACKGROUND_COLOR = cv::Scalar(255, 255, 255);  //white
const cv::Scalar OBSTACLE_COLOR = cv::Scalar(19, 69, 139);      //brown
const cv::Scalar OBSTACLE_RAY = cv::Scalar(20, 69, 139);        //brown ray

class Screen {
   private:
    cv::Mat map;  //ópecv image

    uchar **imgMatrix;  //matrix to faster randon access
    uchar **resetMatrix;  //matrix to faster randon access

    //map parameters
    int rows;
    int cols;

   public:
    std::string windowName;
    Screen();
    ~Screen();

    void setScreenParam(std::string name, int xPos, int yPos);

    int colorToId(cv::Scalar color);  //transforms color to id

    void matToMatrix();  //set ptrs of matrix
    void resetImage();   //set BACKGROUND_COLOR

    void updateMap();       //shows map
    void createObstacle();  //draw obstacles

    cv::Scalar getColor(cv::Point pt);        //get point color
    cv::Scalar colorToRay(cv::Scalar color);  //transforms color to ray color
    cv::Scalar idToRay(int rayId);            //transforms id to ray color

    //gets:
    cv::Mat getMap();

    uchar **getMatrix();

    //----------------PRIMITIVE FUNCTIONS----------------
    void drawRect(cv::Point pt1, cv::Point pt2, const cv::Scalar color);
    void drawLine(cv::Point pt1, cv::Point pt2, const cv::Scalar color);
    void drawLine(cv::Point pt1, cv::Point pt2, const cv::Scalar color, const int size);
    void drawCircle(cv::Point center, const int radius, const cv::Scalar color);

};

#endif
