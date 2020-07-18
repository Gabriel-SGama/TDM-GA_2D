#include "headers/Screen.h"

Screen::Screen() {
    map = cv::Mat(HEIGHT, LENGTH, CV_8UC3, BACKGROUND_COLOR);

    rows = map.rows;
    cols = map.cols * map.channels();

    matToMatrix();
}

void Screen::setScreenParam(std::string name, int xPos, int yPos) {
    windowName = name;
    cv::namedWindow(windowName);
    cv::moveWindow(windowName, xPos, yPos);
}

uchar **Screen::getMatrix() {
    return imgMatrix;
}

void Screen::matToMatrix() {
    imgMatrix = new uchar *[rows];

    for (int i = 0; i < rows; i++) {
        imgMatrix[i] = map.ptr(i);
    }
}

cv::Scalar Screen::getColor(cv::Point pt) {
    cv::Scalar color;

    if (pt.x >= LENGTH || pt.x < 0 || pt.y >= HEIGHT || pt.y < 0)
        return cv::Scalar(BACKGROUND_COLOR);

    //BGR
    color.val[0] = imgMatrix[pt.y][pt.x * 3];
    color.val[1] = imgMatrix[pt.y][pt.x * 3 + 1];
    color.val[2] = imgMatrix[pt.y][pt.x * 3 + 2];

    return color;
}

int Screen::colorToId(cv::Scalar color) {
    if (color == BACKGROUND_COLOR)
        return NOTHING;

    else if (color == LIGHT_ASSAULT_COLOR)
        return LIGHT_ASSAULT;

    else if (color == ASSAULT_COLOR)
        return ASSAULT;

    else if (color == SNIPER_COLOR)
        return SNIPER;

    else if (color == OBSTACLE_COLOR)
        return OBSTACLE;

    return NOTHING;  //vision ray
}

cv::Scalar Screen::colorToRay(cv::Scalar color) {
    if (color == LIGHT_ASSAULT_COLOR)
        return LIGHT_ASSAULT_RAY;
    else if (color == SNIPER_COLOR)
        return SNIPER_RAY;
    else if (color == ASSAULT_COLOR)
        return ASSAULT_RAY;
    else if (color == OBSTACLE_COLOR)
        return OBSTACLE_RAY;

    return color;  //vision ray color
}

cv::Scalar Screen::idToRay(int rayId) {
    if (rayId == NOTHING)
        return BACKGROUND_COLOR;
    else if (rayId == OBSTACLE)
        return OBSTACLE_RAY;
    else if (rayId == LIGHT_ASSAULT)
        return LIGHT_ASSAULT_RAY;
    else if (rayId == SNIPER)
        return SNIPER_RAY;
    else
        return ASSAULT_RAY;
}

void Screen::resetImage() {
    map.setTo(BACKGROUND_COLOR);
}

void Screen::updateMap() {
    cv::imshow(windowName, map);
    cv::waitKey(30);
}

cv::Mat Screen::getMap() {
    return map;
}

void Screen::createObstacle() {
    cv::Point pt1;
    cv::Point pt2;

/*
    //----------------L----------------
    pt1.x = LENGTH / 5;
    pt1.y = 0;

    pt2.x = pt1.x + _OBSTACLE_LENGTH;
    pt2.y = LENGTH / 5;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);

    pt1 = pt2;

    pt2.x += LENGTH / 8;
    pt2.y -= _OBSTACLE_LENGTH;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);

    //----------------TOP RECT----------------
    pt1.x = LENGTH / 2;
    pt1.y = HEIGHT / 6;

    pt2.x = pt1.x + LENGTH / 2;
    pt2.y = pt1.y - _OBSTACLE_LENGTH;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);

    //----------------RIGHT SIDE RECT----------------
    pt1.x = LENGTH - RADIUS * 12;
    pt1.y = HEIGHT / 3;

    pt2.x = pt1.x + _OBSTACLE_LENGTH;
    pt2.y = pt1.y + HEIGHT / 3;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);

    //----------------MIDDLE RECT----------------
    pt1.x = LENGTH / 2 - RADIUS * 20;
    pt1.y = HEIGHT / 2;

    pt2.x = pt1.x + LENGTH / 4;
    pt2.y = pt1.y + _OBSTACLE_LENGTH;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);

    //----------------LOWER VERT RECT----------------
    pt1.x = LENGTH / 1.5;
    pt1.y = HEIGHT;

    pt2.x = pt1.x + _OBSTACLE_LENGTH;
    pt2.y = pt1.y - HEIGHT / 4;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);

    //----------------LOWER HORIZ RECT----------------
    pt1.x = LENGTH / 6;
    pt1.y = 3 * HEIGHT / 4;

    pt2.x = LENGTH / 2;
    pt2.y = pt1.y + _OBSTACLE_LENGTH;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);
//*/

    //----------------MAP FOR MULTI PLAYER TYPES----------------
    ///*
    pt1.x = 100;
    pt1.y = 0;

    pt2.x = pt1.x + _OBSTACLE_LENGTH;
    pt2.y = 125;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);

    pt1.x = pt2.x;
    pt1.y = 125 - _OBSTACLE_LENGTH;

    pt2.x = 250;
    pt2.y = 125;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);

    pt1.x = 0;
    pt1.y = 250;

    pt2.x = 130;
    pt2.y = 250 +_OBSTACLE_LENGTH;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);

    pt1.x = 250;
    pt1.y = 220;

    pt2.x = 250 + _OBSTACLE_LENGTH;
    pt2.y = 350;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);

    pt1.x = 100;
    pt1.y = 410;

    pt2.x = 250;
    pt2.y = 410 + _OBSTACLE_LENGTH;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);

    pt1.x = 165;
    pt1.y = 410;

    pt2.x = 165 + _OBSTACLE_LENGTH;
    pt2.y = 550;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);

    pt1.x = 0;
    pt1.y = 650;

    pt2.x = 150;
    pt2.y = 650 + _OBSTACLE_LENGTH;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);

    pt1.x = 380;
    pt1.y = HEIGHT - 1;

    pt2.x = 380 + _OBSTACLE_LENGTH;
    pt2.y = 600;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);

    pt1.x = 300;
    pt1.y = 500;

    pt2.x = 450;
    pt2.y = 500 + _OBSTACLE_LENGTH;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);

    pt1.x = 400;
    pt1.y = 0;

    pt2.x = 400 + _OBSTACLE_LENGTH;
    pt2.y = 150;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);

    pt1.x = 650;
    pt1.y = 400;

    pt2.x = 800;
    pt2.y = 400 + _OBSTACLE_LENGTH;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);
    //*/
    //----------------LIMITS----------------
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
