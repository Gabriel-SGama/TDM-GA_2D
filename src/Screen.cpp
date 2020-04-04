#include "headers/Screen.h"

Screen::Screen()
{
    windowName = "map";

    map = cv::Mat(HEIGHT, LENGTH, CV_8UC3, BACKGROUND_COLOR);

    cv::namedWindow(windowName);

    rows = map.rows;
    cols = map.cols * map.channels();

    matToMatrix();
}

uchar **Screen::getMatrix()
{
    return imgMatrix;
}

void Screen::matToMatrix()
{
    imgMatrix = new uchar *[rows];

    for (int i = 0; i < rows; i++)
    {
        imgMatrix[i] = map.ptr(i);
    }
}

cv::Scalar Screen::getColor(cv::Point pt)
{
    //cv::Vec3b color;
    cv::Scalar color;

    //std::cout << pt << std::endl;

    if (pt.x >= LENGTH || pt.x < 0 || pt.y >= HEIGHT || pt.y < 0)
        return cv::Scalar(BACKGROUND_COLOR);

    //BGR
    color.val[0] = imgMatrix[pt.y][pt.x * 3];
    color.val[1] = imgMatrix[pt.y][pt.x * 3 + 1];
    color.val[2] = imgMatrix[pt.y][pt.x * 3 + 2];

    return color;
}

int Screen::colorToId(cv::Scalar color)
{
    if (color == BACKGROUND_COLOR)
        return NOTHING;

    else if (color == INOCENT_COLOR)
        return INOCENT;

    else if (color == DETECTIVE_COLOR)
        return DETECTIVE;

    else if (color == TRAITOR_COLOR)
        return TRAITOR;

    else if (color == OBSTACLE_COLOR)
        return OBSTACLE;

    return NOTHING; //vision ray
}

cv::Scalar Screen::colorToRay(cv::Scalar color)
{

    if (color == INOCENT_COLOR)
        color.val[0] += 1;
    else if (color == DETECTIVE_COLOR)
        color.val[1] += 1;
    else if (color == TRAITOR_COLOR)
        color.val[0] += 1;
    else if (color == OBSTACLE_COLOR)
        color.val[0] += 1;

    return color; //vision ray color
}

cv::Scalar Screen::idToRay(int rayId)
{
    if (rayId == NOTHING)
        return BACKGROUND_COLOR;
    else if (rayId == OBSTACLE)
        return OBSTACLE_RAY;
    else if (rayId == INOCENT)
        return INOCENT_RAY;
    else if (rayId == TRAITOR)
        return TRAITOR_RAY;
    else
        return DETECTIVE_RAY;
}

void Screen::createObstacle()
{
    cv::Point pt1;
    cv::Point pt2;

    pt1.x = LENGTH / 16;
    pt1.y = 0;

    pt2.x = pt1.x + _OBSTACLE_LENGTH;
    pt2.y = LENGTH / 8;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);

    pt1 = pt2;

    pt2.x += LENGTH / 8;
    pt2.y -= _OBSTACLE_LENGTH;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);

    pt1.x = LENGTH / 2;
    pt1.y = HEIGHT / 6;

    pt2.x = pt1.x + LENGTH / 3;
    pt2.y = pt1.y - _OBSTACLE_LENGTH;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);

    pt1.x = LENGTH - RADIUS * 12;
    pt1.y = HEIGHT / 3;

    pt2.x = pt1.x + _OBSTACLE_LENGTH;
    pt2.y = pt1.y + HEIGHT / 3;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);

    pt1.x = LENGTH / 2 - RADIUS * 20;
    pt1.y = HEIGHT / 2;

    pt2.x = pt1.x + LENGTH / 4;
    pt2.y = pt1.y + _OBSTACLE_LENGTH;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);

    pt1.x = LENGTH / 2;
    pt1.y = HEIGHT;

    pt2.x = pt1.x + _OBSTACLE_LENGTH;
    pt2.y = pt1.y - HEIGHT / 4;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);
}

void Screen::resetImage()
{
    int i;
    int j;

    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
            imgMatrix[i][j] = 255;
}

void Screen::updateMap()
{

    cv::imshow(windowName, map);
    cv::waitKey(0);
}

cv::Mat Screen::getMap()
{
    return map;
}