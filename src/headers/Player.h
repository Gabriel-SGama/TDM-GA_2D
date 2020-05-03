#pragma once

#include <iostream>
#include "math.h"
#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Dense>

class Player;

//Player ID:
#define NOTHING 1
#define LIGHT_ASSAULT 2
#define ASSAULT 3
#define SNIPER 4
#define OBSTACLE 5

//Vision and player paramters
#define RADIUS 10
#define RADIUS_OFFSET 3

//shot
#define SHOT_INTERVAL 0

#define MIN_DIST_TO_MOVE 40

const int _RADIUS_TOTAL_DISTANCE = RADIUS + RADIUS_OFFSET;
const int safeDist = 2 * RADIUS + RADIUS_OFFSET;

//Colors
const cv::Scalar LIGHT_ASSAULT_COLOR = cv::Scalar(0, 255, 0); //green
const cv::Scalar SNIPER_COLOR = cv::Scalar(0, 0, 255);        //red
const cv::Scalar ASSAULT_COLOR = cv::Scalar(255, 0, 0);       //blue

//Ray colors
const cv::Scalar LIGHT_ASSAULT_RAY = cv::Scalar(1, 255, 0); //green ray
const cv::Scalar ASSAULT_RAY = cv::Scalar(255, 1, 0);       //blue ray
const cv::Scalar SNIPER_RAY = cv::Scalar(1, 0, 255);        //red ray

const cv::Point aux = cv::Point(-RADIUS / 2, RADIUS / 2); //offset to print text

#include "ANN.h"
#include "Screen.h"

const int MAX_TIME_STAND = 20;

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
    cv::Scalar playerRay; //color

    std::string playerIDStr; //id->str

    bool alive; //player status

    int playerType;  //type of player
    int playerID;    //number of player
    int life;        //current life
    //int radiusOffset;
    int damage; //player damage
    int visionDist;
    int timeStand;
    int timeShot;
    int distSumX;
    int distSumY;
    int turn;

    //ray info
    int *raysID;
    int *raysDist;

    float score; //player score

    float visionAngle;
    float direction;
    float speedLimit;
    float angularSpeedLimit;

    double separationAngle; //offset to next ray
    double angleCorrection; //corrects rays positions

    Screen *screen;

    //Moderator* moderator;
    cv::Point **playersCenter;

    VectorXf *input;

public:
    int numberOfRays;

    ANN *ann;

    int ANNInputSize;
    int ANNOutputSize;

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
    inline cv::Point *getCenterPtr() { return &center; }
    inline float getScore() { return score; }

    inline void updateScore(float change) { score += change; }
    inline void setScore(float newScore) { score = newScore; }

    void setAlive(bool alive);

    //initial values
    void setPlayerValues(Screen *screen, int playerID, int life, cv::Point **playersCenter); //inicial values
    void setPosition();                                                                      //initial position
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

    //reset
    void reset(int life, bool resetScore);
};

#include "Moderator.h"
