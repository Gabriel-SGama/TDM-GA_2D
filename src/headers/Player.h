#ifndef PLAYER_H
#define PLAYER_H

class Player;

#include "math.h"
#include <opencv2/opencv.hpp>

//Player ID:
#define NOTHING 1
#define INOCENT 2
#define DETECTIVE 3
#define TRAITOR 4
#define OBSTACLE 5

//Colors

//Vision and player paramters
#define VISION_DIST 100
#define RADIUS 10
#define RADIUS_OFFSET 2

const int _RADIUS_TOTAL_DISTANCE = RADIUS + RADIUS_OFFSET;

const cv::Scalar INOCENT_COLOR = cv::Scalar(0, 255, 0);   //green
const cv::Scalar DETECTIVE_COLOR = cv::Scalar(255, 0, 0); //blue
const cv::Scalar TRAITOR_COLOR = cv::Scalar(0, 0, 255);   //red

const cv::Scalar INOCENT_RAY = cv::Scalar(1, 255, 0);   //green ray
const cv::Scalar DETECTIVE_RAY = cv::Scalar(255, 1, 0); //blue ray
const cv::Scalar TRAITOR_RAY = cv::Scalar(1, 0, 255);   //red ray

#include "Screen.h"

class Player
{
protected:
    cv::Point center;
    cv::Scalar playerColor;
    cv::Point aux;

    std::string playerIDStr;

    bool alive;

    int playerType;
    int playerID;
    int life;
    int radiusOffset;
    int numberOfRays;
    int killPotential;

    int *raysID;
    int *raysDist;

    float score;

    double separationAngle;
    double angleCorrection;

    Screen *screen;

public:
    explicit Player();
    ~Player();

    bool isAlive();

    inline int getPlayerID() { return playerID; }
    inline cv::Point getCenter() { return center; }
    inline int getLife() { return life; }

    inline void setAlive(bool alive) { this->alive = alive; }

    int getRadius();
    int checkPosition();
    int checkMove(cv::Point offset);

    void setPlayerValues(Screen *screen, int playerID, int life);
    void setPosition();
    void drawPlayer();
    void drawVisionLines(double currentAngle, int id);
    void move(cv::Point offset);
    void updateVision();
    void takeDamage(int damage);

    cv::Point killPlayer(int rayNumber);
};

#endif