#include "headers/Player.h"
#include "headers/Inocent.h"
#include "headers/Traitor.h"
#include "headers/Detective.h"
#include "string.h"

Player::Player()
{
    double currentAngle;
    double nextCurrentAngle;

    alive = true;

    aux.x = -RADIUS / 2;
    aux.y = RADIUS / 2;

    separationAngle = _RADIUS_TOTAL_DISTANCE * 2.0 / VISION_DIST;
    numberOfRays = round(2 * M_PI / separationAngle);

    angleCorrection = M_PI * 2 / numberOfRays - separationAngle;

    separationAngle += angleCorrection;

    raysID = new int[numberOfRays];
    raysDist = new int[numberOfRays];
}

Player::~Player()
{
}

bool Player::isAlive()
{
    return alive;
}

void Player::setPlayerValues(Screen *screen, int playerNumber)
{
    this->playerType = playerType;
    this->playerColor = playerColor;
    this->playerNumber = playerNumber;
    playerNumberStr = std::to_string(playerNumber);
    this->screen = screen;

    setPosition();
}

void Player::setPosition()
{
    int safeDist = 2 * RADIUS + RADIUS_OFFSET;

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
        pt.x = cos(i) * (_RADIUS_TOTAL_DISTANCE + 1);
        pt.y = sin(i) * (_RADIUS_TOTAL_DISTANCE + 1);

        pt += center;

        //cv::circle(screen->getMap(), pt, 1, TRAITOR_RAY, cv::FILLED);

        //std::cout << pt << std::endl;
        //std::cout << i << std::endl;

        if (pt.x >= LENGTH || pt.x < 0 || pt.y >= HEIGHT || pt.y < 0 || screen->colorToId(screen->getColor(pt)) != NOTHING)
            return 0;
    }

    return 1;
}

void Player::drawPlayer()
{
    cv::circle(screen->getMap(), center, RADIUS, playerColor, cv::FILLED);
    cv::putText(screen->getMap(), playerNumberStr, center + aux, cv::FONT_HERSHEY_SIMPLEX, 0.35, cv::Scalar(0, 0, 0), 2);
}

cv::Point *Player::updateVision()
{
    double currentAngle;

    for (int i = 0; i < numberOfRays; i++)
    {
        currentAngle = separationAngle * i;
        drawVisionLines(currentAngle, i);
    }
}

void Player::drawVisionLines(double currentAngle, int id)
{

    cv::Point pt;
    cv::Point finalPt;
    cv::Point offset;

    cv::Scalar color;

    uchar **matrix;

    int i;

    for (i = 0; i < VISION_DIST; i++)
    {
        offset.x = _RADIUS_TOTAL_DISTANCE * cos(currentAngle);
        offset.y = _RADIUS_TOTAL_DISTANCE * sin(currentAngle);

        pt.x = i * cos(currentAngle);
        pt.y = i * sin(currentAngle);

        finalPt = pt + center + offset;

        raysID[id] = screen->colorToId(screen->getColor(finalPt));

        if (raysID[id] != NOTHING)
        {
            break;
        }
    }

    raysDist[id] = i;

    if (raysID[id] == TRAITOR && playerType != TRAITOR)
    {
        raysID[id] = INOCENT;
        color = INOCENT_RAY;
    }
    else
    {
        color = screen->idToRay(raysID[id]);
    }

    cv::line(screen->getMap(), center + offset, finalPt, color);
}

int Player::checkMove(cv::Point offset)
{
    cv::Point pt;

    float angle = atan2(offset.y, offset.x);

    for (float i = angle - M_PI_2; i < M_PI_2 + angle; i += 0.1)
    {
        pt.x = cos(i) * _RADIUS_TOTAL_DISTANCE;
        pt.y = sin(i) * _RADIUS_TOTAL_DISTANCE;

        pt += center + offset;

        //std::cout << pt << std::endl;
        //std::cout << i << std::endl;

        if (pt.x >= LENGTH || pt.x < 0 || pt.y >= HEIGHT || pt.y < 0 || screen->colorToId(screen->getColor(pt)) != NOTHING)
            return 0;

        //cv::circle(screen->getMap(), pt, 1, cv::Scalar(0, 1, 255), cv::FILLED);
    }

    return 1;
}

void Player::move(cv::Point offset)
{
    center += offset;

    if (!checkMove(offset))
    {
        center -= offset;
        std::cout << "posisao invalida" << std::endl;
    }
}

void Player::killPlayer(int rayNumber, Player *inocentArray, Player *traitorArray, Player *detectiveArray)
{
    double currentAngle;

    cv::Point enemyPoint;

    currentAngle = separationAngle * rayNumber;

    enemyPoint.x = (_RADIUS_TOTAL_DISTANCE + raysDist[rayNumber]) * cos(currentAngle);
    enemyPoint.y = (_RADIUS_TOTAL_DISTANCE + raysDist[rayNumber]) * sin(currentAngle);

    enemyPoint += center;

    //cv::circle(screen->getMap(), enemyPoint, 3, cv::Scalar(0, 1, 255), cv::FILLED);

    getPlayerNumber(enemyPoint, inocentArray, traitorArray, detectiveArray);
}

int Player::getPlayerNumber(cv::Point enemyPoint, Player *inocentArray, Player *traitorArray, Player *detectiveArray)
{
    int i;
    float distance;

    for (i = 0; i < NUMBER_OF_INOCENTS; i++)
    {
        distance = cv::norm(enemyPoint - inocentArray[i].center);

        if (distance <= RADIUS)
        {
            inocentArray[i].alive = false;
            //std::cout << playerNumber << "matou " << inocentArray[i].playerNumber << std::endl;
            return inocentArray[i].playerNumber;
        }
    }

    for (i = 0; i < NUMBER_OF_TRAITORS; i++)
    {
        distance = cv::norm(enemyPoint - traitorArray[i].center);

        if (distance <= RADIUS)
        {
            traitorArray[i].alive = false;
            //std::cout << playerNumber << "matou " << traitorArray[i].playerNumber << std::endl;
            return traitorArray[i].playerNumber;
        }
    }

    //nao tinha ninguem
    return -1;
}