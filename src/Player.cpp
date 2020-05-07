#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "string.h"
#include "headers/Player.h"

Player::Player()
{
    score = 0;
    alive = true;

    standInterval = 20;
    timeStand = standInterval;
    timeShot = shotInterval;

    spinInterval = 20;
    timeSpin = spinInterval;
    lastAngularSpeed = 0;
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
    cv::line(screen->getMap(), center, cv::Point(cos(direction) * (RADIUS + 4), sin(direction) * (RADIUS + 4)) + center, playerRay, 3);
    //cv::rectangle(screen->getMap(), center + cv::Point(sin(direction),cos(direction)), center + cv::Point(sin(direction) + 20,cos(direction) + 20), playerRay, cv::FILLED);

    //cv::putText(screen->getMap(), playerIDStr, center + aux, cv::FONT_HERSHEY_SIMPLEX, 0.35, cv::Scalar(0, 0, 0), 2);
}

void Player::updateVision()
{
    double currentAngle;

    currentAngle = direction - visionAngle / 2;
    for (int i = 0; i < numberOfRays; i++)
    {
        drawVisionLines(currentAngle, i);
        currentAngle += separationAngle;
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

    if ((output[0][INDEX_DIRECTION] > 0 && lastAngularSpeed > 0) || (output[0][INDEX_DIRECTION] < 0 && lastAngularSpeed < 0))
    {
        timeSpin--;
        if (timeSpin <= 0)
        {
            score -= 0;
            timeSpin = spinInterval;
        }
    }
    else
    {
        timeSpin = spinInterval;
    }

    cv::Point offset = cv::Point((int)speed * cos(direction), (int)speed * sin(direction));

    center += offset;

    if (!checkMove(offset) || offset == cv::Point(0, 0))
    {
        center -= offset;
        timeStand--;

        if (timeStand <= 0)
        {
            score -= 0;
            timeStand = standInterval;
        }
    }
    else
    {
        timeStand = standInterval;
    }

    lastAngularSpeed = output[0][INDEX_DIRECTION];
}

enemyInfo_t Player::killPlayer(int rayNumber)
{
    if (rayNumber < 0 || rayNumber > numberOfRays || timeShot > 0)
    {
        if (timeShot <= 0)
            timeShot = shotInterval;

        return {cv::Point(-1, -1), NOTHING};
    }

    timeShot = shotInterval;

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

void Player::setAlive(bool alive, int turn)
{
    this->alive = alive;

    float punishes = (DURATION - turn) / timeStand;
    punishes = 0;

    if (!alive)
    {
        updateScore(-punishes * 1 - 3);
        center.x = 0;
        center.y = 0;
    }
}

void Player::setComunInput()
{
    int i = 0;
    int j;
    ///*
    for (i = 0, j = 0; i < 2 * numberOfRays; i += 2, j++)
    {
        if (raysID[j] == playerType)
        {
            (*input)[i] = -raysID[j];
        }
        else
        {
            (*input)[i] = raysID[j];
        }
        (*input)[i + 1] = raysDist[j] / 10.0;
    }
    //*/
    /*
    i = 2 * numberOfRays;
    for (j = 0; i < 2 * (NUMBER_OF_TOTAL_PLAYERS - 1 + numberOfRays); i += 2, j++)
    {
        if (j == playerID)
        {
            i -= 2;
            continue;
        }

        (*input)[i] = playersCenter[j]->x / 50.0;
        (*input)[i + 1] = playersCenter[j]->y / 50.0;
    }
    i--;
    //*/
    i = 2 * numberOfRays;

    (*input)[i] = center.x / 50.0;
    (*input)[i + 1] = center.y / 50.0;
    (*input)[i + 2] = direction;
    (*input)[i + 3] = life / 10.0;

    timeShot--;
}

void Player::reset(int life, bool resetScore)
{
    if (resetScore)
        score = 0;

    alive = true;

    this->life = life;

    timeShot = shotInterval;
    timeSpin = spinInterval;
    timeStand = standInterval;

    setPosition();
    drawPlayer();
    updateVision();
    setComunInput();
}