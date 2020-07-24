#include "headers/Screen.h"

Screen::Screen() {
    map = cv::Mat(HEIGHT, LENGTH, CV_8UC3, BACKGROUND_COLOR);

    rows = map.rows;
    cols = map.cols * map.channels();

    matToMatrix();

    // resetMatrix = new uchar*[rows];
    // for (int i = 0; i < rows; i++) {
    //     resetMatrix[i] = new uchar[cols];
    // }    
    createObstacle();
}

Screen::~Screen(){

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
        // imgMatrix[i] = new uchar[cols];
        // for (int j = 0; j < cols; j++) {
        //     imgMatrix[i][j] = 255;
        // }
        // std::memcpy(imgMatrix[i], map.ptr(i), sizeof(uchar)*cols);
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

    // for (int i = 0; i < rows; i++) {
    //     memcpy(imgMatrix[i], resetMatrix[i], sizeof(uchar)*cols);
    // }
}

void Screen::updateMap() {
    // uchar* imgMatrixPtr;

    // for (int i = 0; i < rows; i++) {
    //     std::memcpy(map.ptr(i), imgMatrix[i], sizeof(uchar)*cols);
    // }

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
    // drawRect(pt1, pt2, OBSTACLE_COLOR);

    pt1.x = pt2.x;
    pt1.y = 125 - _OBSTACLE_LENGTH;

    pt2.x = 250;
    pt2.y = 125;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);
    // drawRect(pt1, pt2, OBSTACLE_COLOR);

    pt1.x = 0;
    pt1.y = 250;

    pt2.x = 130;
    pt2.y = 250 +_OBSTACLE_LENGTH;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);
    // drawRect(pt1, pt2, OBSTACLE_COLOR);

    pt1.x = 250;
    pt1.y = 220;

    pt2.x = 250 + _OBSTACLE_LENGTH;
    pt2.y = 350;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);
    // drawRect(pt1, pt2, OBSTACLE_COLOR);

    pt1.x = 100;
    pt1.y = 410;

    pt2.x = 250;
    pt2.y = 410 + _OBSTACLE_LENGTH;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);
    // drawRect(pt1, pt2, OBSTACLE_COLOR);

    pt1.x = 165;
    pt1.y = 410;

    pt2.x = 165 + _OBSTACLE_LENGTH;
    pt2.y = 550;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);
    // drawRect(pt1, pt2, OBSTACLE_COLOR);

    pt1.x = 0;
    pt1.y = 650;

    pt2.x = 150;
    pt2.y = 650 + _OBSTACLE_LENGTH;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);
    // drawRect(pt1, pt2, OBSTACLE_COLOR);

    pt1.x = 380;
    pt1.y = HEIGHT - 1;

    pt2.x = 380 + _OBSTACLE_LENGTH;
    pt2.y = 600;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);
    // drawRect(pt1, pt2, OBSTACLE_COLOR);

    pt1.x = 300;
    pt1.y = 500;

    pt2.x = 450;
    pt2.y = 500 + _OBSTACLE_LENGTH;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);
    // drawRect(pt1, pt2, OBSTACLE_COLOR);

    pt1.x = 400;
    pt1.y = 0;

    pt2.x = 400 + _OBSTACLE_LENGTH;
    pt2.y = 150;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);
    // drawRect(pt1, pt2, OBSTACLE_COLOR);

    pt1.x = 650;
    pt1.y = 400;

    pt2.x = LENGTH - 1;
    pt2.y = 400 + _OBSTACLE_LENGTH;

    cv::rectangle(map, pt1, pt2, OBSTACLE_COLOR, cv::FILLED);
    // drawRect(pt1, pt2, OBSTACLE_COLOR);
    //*/

    //----------------LIMITS----------------

    pt1.x = 0;
    pt1.y = 0;

    pt2.x = LENGTH;
    pt2.y = 0;

    // drawRect(pt1, pt2, OBSTACLE_COLOR);
    cv::line(map, pt1, pt2, OBSTACLE_COLOR, 5);

    pt1.x = 0;
    pt1.y = HEIGHT;

    pt2.x = LENGTH - 1;
    pt2.y = HEIGHT;

    // drawRect(pt1, pt2, OBSTACLE_COLOR);
    cv::line(map, pt1, pt2, OBSTACLE_COLOR, 5);

    pt1.x = 0;
    pt1.y = 0;

    pt2.x = 0;
    pt2.y = HEIGHT - 1;

    // drawRect(pt1, pt2, OBSTACLE_COLOR);
    cv::line(map, pt1, pt2, OBSTACLE_COLOR, 5);

    pt1.x = LENGTH - 1;
    pt1.y = 0;

    pt2.x = LENGTH - 1;
    pt2.y = HEIGHT - 1;

    // drawRect(pt1, pt2, OBSTACLE_COLOR);
    cv::line(map, pt1, pt2, OBSTACLE_COLOR, 5);
   
    // for (int i = 0; i < rows; i++) {
    //     memcpy(resetMatrix[i], imgMatrix[i], sizeof(uchar)*cols);
    // }
}

/*
void Screen::drawRect(cv::Point pt1, cv::Point pt2, const cv::Scalar color){
    int line;
    int colun;

    int swap;

    if(pt1.x  > pt2.x){
        swap = pt1.x;
        pt1.x = pt2.x;
        pt2.x = swap;
    }

    if(pt1.y  > pt2.y){
        swap = pt1.y;
        pt1.y = pt2.y;
        pt2.y = swap;
    }

    pt1.x *= 3;
    pt2.x *= 3;

    for (line = pt1.y; line < pt2.y; line++) {
        for (colun = pt1.x; colun < pt2.x; colun += 3) {
            imgMatrix[line][colun] = color.val[0];
            imgMatrix[line][colun + 1] = color.val[1];
            imgMatrix[line][colun + 2] = color.val[2];
        }
    }
    
}

void Screen::drawLine(cv::Point pt1, cv::Point pt2, const cv::Scalar color){
    int line;
    int colun;

    float angle;
    int size = cv::norm(pt2 - pt1);

    float sinA;
    float cosA;


    if(pt1.y == pt2.y)
        if(pt2.x - pt1.x > 0){
            angle = 0;
            cosA = 1;
            sinA = 0;
        }
        else{
            angle = M_PI;
            cosA = -1;
            sinA = 0;
        }
    else if(pt1.x == pt2.x)
        if(pt2.y - pt1.y > 0){
            angle = M_PI_2;
            cosA = 0;
            sinA = 1;
        }else{
            angle = M_PI + M_PI_2; //pi*3/2
            cosA = 0;
            sinA = -1;
        }
    else{
        angle = atan2(pt2.y - pt1.y, pt2.x - pt1.x);
        sinA =  sin(angle);
        cosA =  cos(angle);
    }
    
    for (int i = 0; i < size; i++) {
        line = sinA*i + pt1.y;
        colun = cosA*i + pt1.x;
        if(line < 0 || line > HEIGHT - 1 || colun < 0 || colun > LENGTH - 1)
            break;

        colun *= 3;

        imgMatrix[line][colun] = color[0];
        imgMatrix[line][colun + 1] = color[1];
        imgMatrix[line][colun + 2] = color[2];
    }
}

void Screen::drawLine(cv::Point pt1, cv::Point pt2, const cv::Scalar color, const int thiknees){
    int line;
    int colun;

    float angle;

    float sinA;
    float cosA;


    if(pt1.y == pt2.y)
        if(pt2.x - pt1.x > 0){
            angle = 0;
            cosA = 1;
            sinA = 0;
        }
        else{
            angle = M_PI;
            cosA = -1;
            sinA = 0;
        }
    else if(pt1.x == pt2.x)
        if(pt2.y - pt1.y > 0){
            angle = M_PI_2;
            cosA = 0;
            sinA = 1;
        }else{
            angle = M_PI + M_PI_2; //pi*3/2
            cosA = 0;
            sinA = -1;
        }
    else{
        angle = atan2(pt2.y - pt1.y, pt2.x - pt1.x);
        sinA =  sin(angle);
        cosA =  cos(angle);
    }

    cv::Point circlePt;
    cv::Point offset;

    offset.x = cosA * thiknees;
    offset.y = sinA * thiknees;

    float dist;
    int j;

    for (float i = -M_PI_2 - angle; i < M_PI_2 - angle; i += 0.1) {
        circlePt.x = -round(cos(i) * thiknees);
        circlePt.y = round(sin(i) * thiknees);
        line = pt1.y + circlePt.y + offset.y;
        colun = 3*(pt1.x + circlePt.x + offset.x);
        
        dist = cv::norm(pt2 - pt1 -2*circlePt);

        for (j = 0; j < dist; j++) {
            line = sinA*j + pt1.y + circlePt.y;
            colun = cosA*j + pt1.x + circlePt.x;
            
            if(line < 0 || line > HEIGHT - 1 || colun < 0 || colun > LENGTH - 1)
                break;
            
            colun *= 3;
            
            imgMatrix[line][colun] = color[0];
            imgMatrix[line][colun + 1] = color[1];
            imgMatrix[line][colun + 2] = color[2];
        }
    }
}

void Screen::drawCircle(cv::Point center, const int radius, const cv::Scalar color){

    cv::Point circlePt;

    int i;
    int line;

    int limit = 0;

    for (float angle = 0; angle < M_PI_2 / 2.0; angle += 0.09) {
        circlePt.x = round(radius * cos(angle));
        circlePt.y = round(radius * sin(angle));
        
        line = center.y + circlePt.y;
        limit = 3*(circlePt.x + center.x);

        for (i = 3*(-circlePt.x + center.x); i <= limit; i += 3) {

            if(line < 0 || line > HEIGHT - 1 || i < 0 || i > 3*LENGTH - 1)
                break;

            imgMatrix[line][i] = color.val[0];
            imgMatrix[line][i + 1] = color.val[1];
            imgMatrix[line][i + 2] = color.val[2];
        }

        line = center.y - circlePt.y;
        limit = 3*(circlePt.x + center.x);

        for (i = 3*(-circlePt.x + center.x); i <= limit; i += 3) {

            if(line < 0 || line > HEIGHT - 1 || i < 0 || i > 3*LENGTH - 1)
                break;

            imgMatrix[line][i] = color.val[0];
            imgMatrix[line][i + 1] = color.val[1];
            imgMatrix[line][i + 2] = color.val[2];
        }

        line = center.y + circlePt.x;
        limit = 3*(circlePt.y + center.x);
        
        for (i = 3*(-circlePt.y + center.x); i <= limit; i += 3) {

            if(line < 0 || line > HEIGHT - 1 || i < 0 || i > 3*LENGTH - 1)
                break;

            imgMatrix[line][i] = color.val[0];
            imgMatrix[line][i + 1] = color.val[1];
            imgMatrix[line][i + 2] = color.val[2];
        }

        line = center.y - circlePt.x;
        limit = 3*(circlePt.y + center.x);
        
        for (i = 3*(-circlePt.y + center.x); i <= limit; i += 3) {

            if(line < 0 || line > HEIGHT - 1 || i < 0 || i > 3*LENGTH - 1)
                break;

            imgMatrix[line][i] = color.val[0];
            imgMatrix[line][i + 1] = color.val[1];
            imgMatrix[line][i + 2] = color.val[2];
        }
    }
}
*/