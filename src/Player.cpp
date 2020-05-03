#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "string.h"
#include "headers/Player.h"

Player::Player()
{
    turn = 1;
    score = 0;
    alive = true;

    distSumX = 0;
    distSumY = 0;
    timeStand = MAX_TIME_STAND;
    timeShot = SHOT_INTERVAL;
}

Player::~Player()
{
}

bool Player::isAlive()
{
    return alive;
}

void Player::setPlayerValues(Screen *screen, int playerID, int life, cv::Point **playersCenter)
{
    this->playerType = playerType;
    this->playerID = playerID;
    playerIDStr = std::to_string(playerID);
    this->screen = screen;
    this->life = life;
    this->playersCenter = playersCenter;

    setPosition();
    drawPlayer();
}

void Player::setPosition()
{
    //int safeDist = 2 * RADIUS + RADIUS_OFFSET;

    center.x = LENGTH / 2;
    center.y = HEIGHT / 2;

    do
    {
        center.x = rand() % (LENGTH - safeDist);
        center.y = rand() % (HEIGHT - safeDist);

    } while (!checkPosition());
}

int Player::checkPosition()
{
    cv::Point pt;

    for (float i = 0; i < M_PI * 2; i += 0.1)
    {
        pt.x = cos(i) * _RADIUS_TOTAL_DISTANCE;
        pt.y = sin(i) * _RADIUS_TOTAL_DISTANCE;

        pt += center;

        if (pt.x >= LENGTH || pt.x < 0 || pt.y >= HEIGHT || pt.y < 0 || screen->colorToId(screen->getColor(pt)) != NOTHING)
            return 0;
    }

    return 1;
}

void Player::drawPlayer()
{
    cv::circle(screen->getMap(), center, RADIUS, playerColor, cv::FILLED);
    //cv::line(screen->getMap(), center, cv::Point(cos(direction) * (RADIUS + 5), sin(direction) * (RADIUS + 5)) + center, playerRay, 5);
    //cv::rectangle(screen->getMap(), center + cv::Point(sin(direction),cos(direction)), center + cv::Point(sin(direction) + 20,cos(direction) + 20), playerRay, cv::FILLED);
    
    //cv::putText(screen->getMap(), playerIDStr, center + aux, cv::FONT_HERSHEY_SIMPLEX, 0.35, cv::Scalar(0, 0, 0), 2);
}

void Player::updateVision()
{
    double currentAngle;

    currentAngle = direction - visionAngle / 2;
    for (int i = 0; i < numberOfRays; i++)
    {
        currentAngle += separationAngle;
        drawVisionLines(currentAngle, i);
    }
}

void Player::drawVisionLines(double currentAngle, int id)
{

    cv::Point pt;
    cv::Point finalPt;
    cv::Point offset;

    cv::Scalar color;

    int i;

    for (i = 0; i < visionDist; i++)
    {
        offset.x = _RADIUS_TOTAL_DISTANCE * cos(currentAngle);
        offset.y = _RADIUS_TOTAL_DISTANCE * sin(currentAngle);

        pt.x = i * cos(currentAngle);
        pt.y = i * sin(currentAngle);

        finalPt = pt + center + offset;

        raysID[id] = screen->colorToId(screen->getColor(finalPt));

        if (raysID[id] != NOTHING)
            break;
    }

    raysDist[id] = i;

    if (raysID[id] != NOTHING)
    {
        color = screen->idToRay(raysID[id]);
        cv::line(screen->getMap(), center + offset, finalPt, color);
    }
}

int Player::checkMove(cv::Point offset)
{
    cv::Point pt;

    //checks movement in an angle of 90 degres to each side
    for (float i = direction - M_PI_2; i < M_PI_2 + direction; i += 0.1)
    {
        pt.x = cos(i) * _RADIUS_TOTAL_DISTANCE;
        pt.y = sin(i) * _RADIUS_TOTAL_DISTANCE;

        pt += center + offset;

        if (pt.x >= LENGTH || pt.x < 0 || pt.y >= HEIGHT || pt.y < 0 || screen->colorToId(screen->getColor(pt)) != NOTHING)
            return 0;
    }

    return 1;
}

void Player::move()
{
    turn++;
    ///*
    //limits speed
    if (output[0][INDEX_DIRECTION] > angularSpeedLimit)
        output[0][INDEX_DIRECTION] = angularSpeedLimit;

    else if (output[0][INDEX_DIRECTION] < -angularSpeedLimit)
        output[0][INDEX_DIRECTION] = -angularSpeedLimit;

    if (output[0][INDEX_SPEED] > speedLimit)
        output[0][INDEX_SPEED] = speedLimit;

    else if (output[0][INDEX_SPEED] < 0)
        output[0][INDEX_SPEED] = 0;

    direction += output[0][INDEX_DIRECTION];
    //*/

    //direction += angularSpeedLimit;

    if (direction > 2 * M_PI)
        direction -= 2 * M_PI;
    else if (direction < -2 * M_PI)
        direction += 2 * M_PI;

    int speed = speedLimit * output[0][INDEX_SPEED];

    cv::Point offset = cv::Point(speed * cos(direction), speed * sin(direction));
    //cv::Point offset = cv::Point(speedLimit * cos(direction), speedLimit * sin(direction));

    center += offset;

    if (!checkMove(offset) || offset == cv::Point(0, 0))
        center -= offset;
}

enemyInfo_t Player::killPlayer(int rayNumber)
{
    if (rayNumber < 0 || rayNumber > numberOfRays || timeShot > 0)
    {
        if (timeShot <= 0)
            timeShot = SHOT_INTERVAL;

        return {cv::Point(-1, -1), NOTHING};
    }

    timeShot = SHOT_INTERVAL;

    //score += 1.1;

    double currentAngle;

    cv::Point enemyPoint;

    currentAngle = separationAngle * rayNumber + direction - visionAngle / 2;

    //enemy location
    enemyPoint.x = (_RADIUS_TOTAL_DISTANCE + raysDist[rayNumber]) * cos(currentAngle);
    enemyPoint.y = (_RADIUS_TOTAL_DISTANCE + raysDist[rayNumber]) * sin(currentAngle);

    enemyPoint += center;

    cv::line(screen->getMap(), center, enemyPoint, cv::Scalar(0, 0, 0), 1);
    cv::circle(screen->getMap(), enemyPoint, 2, cv::Scalar(0, 0, 0), cv::FILLED);

    return {enemyPoint, raysID[rayNumber]};
}

void Player::takeDamage(int damage)
{
    life -= damage;
}

void Player::setAlive(bool alive)
{
    this->alive = alive;

    if (!alive)
    {
        updateScore(-3);
        center.x = 0;
        center.y = 0;
    }
}

void Player::setComunInput()
{
    int i;
    //float distance;

    for (i = 0; i < 2 * numberOfRays; i += 2)
    {
        (*input)[i] = raysID[i];
        (*input)[i + 1] = raysDist[i];
    }

    for (int j = 0; i < 2 * (NUMBER_OF_TOTAL_PLAYERS - 1 + numberOfRays); i += 2, j++)
    {
        if (j == playerID)
        {
            i -= 2;
            continue;
        }

        //distance = cv::norm(center - playersCenter[j][0]) * RADIUS * 2;

        //(*input)[i] = playersCenter[j]->x / distance;
        //(*input)[i + 1] = playersCenter[j]->y / distance;
        (*input)[i] = center.x - playersCenter[j]->x;
        (*input)[i + 1] = center.y - playersCenter[j]->y;
    }

    (*input)[i] = center.x;
    (*input)[i + 1] = center.y;

    (*input)[i + 2] = life;

    // for (int i = 0; i < ANNInputSize; i++)
    // {
    //     (*input)[i] = 0;
    // }

    timeShot--;
}

void Player::reset(int life, bool resetScore)
{
    if (resetScore)
        score = 0;

    turn = 1;
    alive = true;

    this->life = life;

    timeStand = MAX_TIME_STAND;
    timeShot = SHOT_INTERVAL;

    setPosition();
    drawPlayer();
    updateVision();
    setComunInput();
}