#pragma once

#include <iostream>
#include "math.h"
#include <opencv2/opencv.hpp>

class Player;

#define NUMBER_OF_PLAYERS 5
#define PLAYER_HEALTH 100
#define PLAYER_DAMAGE 35
#define PLAYER_NUMBER_OF_RAYS 8
#define PLAYER_SPEED_LIMIT 7.0
#define PLAYER_VISION_DIST 120
#define PLAYER_SHOT_INTERVAL 6

const float PLAYER_ANGULAR_SPEED_LIMIT = M_PI / 45;
const float PLAYER_VISION_ANGLE = M_PI / 2.5;

//Player ID:
#define NOTHING 10
#define OBSTACLE 50
#define TRAINING_PLAYER 1
#define BEST_PLAYER 2
#define ALLY 20
#define ENEMY -20

//Vision and player paramters
#define RADIUS 10
#define RADIUS_OFFSET 3

const int _RADIUS_TOTAL_DISTANCE = RADIUS + RADIUS_OFFSET;
const int safeDist = 2 * RADIUS + RADIUS_OFFSET;

//Colors
const cv::Scalar TRAINING_COLOR = cv::Scalar(0, 255, 0);  //green
const cv::Scalar BEST_COLOR = cv::Scalar(0, 0, 255);  //red

//Ray colors
const cv::Scalar TRAINING_RAY = cv::Scalar(1, 255, 0);  //green ray
const cv::Scalar BEST_RAY = cv::Scalar(1, 0, 255);  //red ray

const cv::Point aux = cv::Point(-RADIUS / 2, RADIUS / 2);  //offset to print text

#include "ANN.h"
#include "Screen.h"

const int MAX_TIME_STAND = 20;

using namespace Eigen;

typedef struct enemyInfo_t {
    cv::Point posiAprox;
    int playerType;

} enemyInfo_t;

class Player {
   protected:
    cv::Point center;  //position
    cv::Point movePt;  //position

    cv::Scalar playerColor;  //color
    cv::Scalar playerRay;    //color

    std::string playerIDStr;  //id->str

    bool alive;  //player status

    int playerType;  //type of player
    int playerID;    //number of player
    int life;        //current life
    //int radiusOffset;
    int damage;  //player damage
    int visionDist;

    int shotInterval;
    int timeShot;

    //ray info
    int *raysID;
    int *raysDist;

    float score;  //player score

    float visionAngle;
    float direction;
    float speedLimit;
    float angularSpeedLimit;

    double separationAngle;  //offset to next ray
    double angleCorrection;  //corrects rays positions

    Screen *screen;


    cv::Point **playersCenter;

    VectorXf *input;
    // VectorXf *inputTest;
    
   public:
    cv::Point initialPos;

    VectorXf outputTest;
    int numberOfRays;

    ANN *ann;

    int ANNInputSize;
    int ANNOutputSize;

    // VectorXf *output;

    Player();
    ~Player();

    //gets:
    bool isAlive();

    inline int getPlayerID() {
        return playerID;
    }
    inline int getPlayerType() {
        return playerType;
    }
    inline int getLife() {
        return life;
    }
    inline int getDamage() {
        return damage;
    }
    inline cv::Point getCenter() {
        return center;
    }
    inline cv::Point *getCenterPtr() {
        return &center;
    }
    inline float getScore() {
        return score;
    }

    inline void updateScore(float change) {
        score += change;
    }
    inline void setScore(float newScore) {
        score = newScore;
    }

    void setAlive(bool alive);

    //initial values
    void setPlayerValues(Screen *screen, int playerID, cv::Point **playersCenter, int playerType, cv::Scalar playerColor, cv::Scalar playerRay);  //inicial values
    void setPosition();                                                                       //initial position
    int checkPosition();

    //draw
    void drawPlayer();                                  //draws player
    void drawVisionLines(double currentAngle, int id);  //draw vision lines

    //vision
    void updateVision();  //updates vision info

    //movement
    void move();  //move player
    int checkMove(cv::Point offset);

    //conflicts
    void takeDamage(int damage);            //get shot
    enemyInfo_t killPlayer(int rayNumber);  //shot playe

    //ANN:
    void setComunInput();
    // inline VectorXf* getInput() {return inputTest;}

    //reset
    void reset(bool resetScore);

    void setANN(ANN* ann);
    inline void copyOutput(VectorXf *output) {outputTest = *output;}
};

#include "Moderator.h"
