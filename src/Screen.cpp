#include "headers/Screen.h"

//defaut constructor
Screen::Screen()
{

    map = cv::Mat(HEIGHT, LENGTH, CV_8UC3, BACKGROUND_COLOR);

    rows = map.rows;
    cols = map.cols * map.channels();

    matToMatrix();
}

void Screen::setScreenParam(std::string name)
{
    windowName = name;
    cv::namedWindow(windowName);
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
        return LIGHT_ASSAULT;

    else if (color == ASSAULT_COLOR)
        return ASSAULT;

    else if (color == SNIPER_COLOR)
        return SNIPER;

    else if (color == OBSTACLE_COLOR)
        return OBSTACLE;

    return NOTHING; //vision ray
}

cv::Scalar Screen::colorToRay(cv::Scalar color)
{

    if (color == INOCENT_COLOR)
        color.val[0] += 1;
    else if (color == ASSAULT_COLOR)
        color.val[1] += 1;
    else if (color == SNIPER_COLOR)
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
    else if (rayId == LIGHT_ASSAULT)
        return INOCENT_RAY;
    else if (rayId == SNIPER)
        return SNIPER_RAY;
    else
        return ASSAULT_RAY;
}

void Screen::resetImage()
{
    map.setTo(BACKGROUND_COLOR);
}

void Screen::updateMap()
{
    cv::imshow(windowName, map);
    cv::waitKey(30);
}

cv::Mat Screen::getMap()
{
    return map;
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

    //limits:
    pt1.x = 0;
    pt1.y = OFFSET_LIMIT;

    pt2.x = LENGTH;
    pt2.y = OFFSET_LIMIT;

    cv::line(map, pt1, pt2, OBSTACLE_COLOR, LIMIT_SIZE);

    pt1.x = 0;
    pt1.y = HEIGHT - 1 - OFFSET_LIMIT;

    pt2.x = LENGTH;
    pt2.y = HEIGHT - 1 - OFFSET_LIMIT;

    cv::line(map, pt1, pt2, OBSTACLE_COLOR, LIMIT_SIZE);

    pt1.x = OFFSET_LIMIT;
    pt1.y = 0;

    pt2.x = OFFSET_LIMIT;
    pt2.y = HEIGHT - 1;

    cv::line(map, pt1, pt2, OBSTACLE_COLOR, LIMIT_SIZE);

    pt1.x = LENGTH - 1 - OFFSET_LIMIT;
    pt1.y = 0;

    pt2.x = LENGTH - 1 - OFFSET_LIMIT;
    pt2.y = HEIGHT - 1;

    cv::line(map, pt1, pt2, OBSTACLE_COLOR, LIMIT_SIZE);
}
