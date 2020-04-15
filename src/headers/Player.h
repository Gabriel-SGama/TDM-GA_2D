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
const int safeDist = 2 * RADIUS + RADIUS_OFFSET;

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
    cv::Point center;       //position
    cv::Scalar playerColor; //color
    cv::Point aux;          //param to print text

    std::string playerIDStr; //id->str

    bool alive; //player status

    int playerType; //type of player
    int playerID;   //number of player
    int life;       //current life
    //int radiusOffset;
    int numberOfRays;
    int killPotential; //player damage

    //ray info
    int *raysID;
    int *raysDist;

    float score; //player score

    double separationAngle; //offset to next ray
    double angleCorrection; //corrects rays positions

    Screen *screen;

public:
    explicit Player();
    ~Player();

    //gets:
    bool isAlive();

    inline int getPlayerID() { return playerID; }
    inline cv::Point getCenter() { return center; }
    inline int getLife() { return life; }
    //int getRadius();

    inline void setAlive(bool alive) { this->alive = alive; }

    int checkPosition();
    int checkMove(cv::Point offset);

    void setPlayerValues(Screen *screen, int playerID, int life); //inicial values
    void setPosition();                                           //initial position
    void drawPlayer();                                            //draws player
    void drawVisionLines(double currentAngle, int id);            //draw vision lines
    void move(cv::Point offset);                                  //move player
    void updateVision();                                          //updates vision info
    void takeDamage(int damage);                                  //get shot

    cv::Point killPlayer(int rayNumber); //shot playe
};

#endif