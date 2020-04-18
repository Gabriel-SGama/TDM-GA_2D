#ifndef PLAYER_H
#define PLAYER_H

class Player;

#include <iostream>
#include "math.h"
#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Dense>

//Player ID:
#define NOTHING 1
#define INOCENT 2
#define DETECTIVE 3
#define TRAITOR 4
#define OBSTACLE 5

//Vision and player paramters
#define VISION_DIST 100
#define RADIUS 10
#define RADIUS_OFFSET 2

const int _RADIUS_TOTAL_DISTANCE = RADIUS + RADIUS_OFFSET;
const int safeDist = 2 * RADIUS + RADIUS_OFFSET;

//Colors
const cv::Scalar INOCENT_COLOR = cv::Scalar(0, 255, 0);   //green
const cv::Scalar DETECTIVE_COLOR = cv::Scalar(255, 0, 0); //blue
const cv::Scalar TRAITOR_COLOR = cv::Scalar(0, 0, 255);   //red

//Ray colors
const cv::Scalar INOCENT_RAY = cv::Scalar(1, 255, 0);   //green ray
const cv::Scalar DETECTIVE_RAY = cv::Scalar(255, 1, 0); //blue ray
const cv::Scalar TRAITOR_RAY = cv::Scalar(1, 0, 255);   //red ray

const cv::Point aux = cv::Point(-RADIUS / 2, RADIUS / 2); //offset to print text

#include "Screen.h"
#include "ANN.h"

using namespace Eigen;

typedef struct enemyInfo_t
{
    cv::Point posiAprox;
    int playerType;
} enemyInfo_t;

class Player
{
protected:
    cv::Point center; //position
    cv::Point movePt; //position

    cv::Scalar playerColor; //color

    std::string playerIDStr; //id->str

    bool alive; //player status

    int playerType;  //type of player
    int playerID;    //number of player
    int life;        //current life
    int damageTaken; //demage taken on that turn
    //int radiusOffset;
    int numberOfRays;
    int damage; //player damage
    int ANNInputSize;
    int ANNOutputSize;

    //ray info
    int *raysID;
    int *raysDist;

    float score; //player score

    double separationAngle; //offset to next ray
    double angleCorrection; //corrects rays positions

    Screen *screen;

    VectorXf *input;

public:
    ANN *ann;
    VectorXf *output;

    explicit Player();
    ~Player();

    //gets:
    bool isAlive();

    inline int getPlayerID() { return playerID; }
    inline int getPlayerType() { return playerType; }
    inline int getLife() { return life; }
    inline int getDamage() { return damage; }
    inline cv::Point getCenter() { return center; }
    inline float getScore() { return score; }
    inline void updateScore(float change) { score += change; }

    //int getRadius();

    inline void setAlive(bool alive) { this->alive = alive; }

    //initial values
    void setPlayerValues(Screen *screen, int playerID, int life); //inicial values
    void setPosition();                                           //initial position
    int checkPosition();

    //draw
    void drawPlayer();                                 //draws player
    void drawVisionLines(double currentAngle, int id); //draw vision lines

    //vision
    void updateVision(); //updates vision info

    //movement
    void move(); //move player
    int checkMove(cv::Point offset);

    //conflicts
    void takeDamage(int damage);           //get shot
    enemyInfo_t killPlayer(int rayNumber); //shot playe

    //ANN:
    void setComunInput();
};

#endif