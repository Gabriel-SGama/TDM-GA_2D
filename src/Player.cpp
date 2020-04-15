#include "headers/Player.h"
#include "headers/Inocent.h"
#include "headers/Traitor.h"
#include "headers/Detective.h"
#include "string.h"

Player::Player()
{
    score = 0;
    alive = true;

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

void Player::setPlayerValues(Screen *screen, int playerID, int life)
{
    this->playerType = playerType;
    this->playerColor = playerColor;
    this->playerID = playerID;
    playerIDStr = std::to_string(playerID);
    this->screen = screen;
    this->life = life;

    setPosition();
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
        pt.x = cos(i) * (_RADIUS_TOTAL_DISTANCE + 1);
        pt.y = sin(i) * (_RADIUS_TOTAL_DISTANCE + 1);

        pt += center;

        if (pt.x >= LENGTH || pt.x < 0 || pt.y >= HEIGHT || pt.y < 0 || screen->colorToId(screen->getColor(pt)) != NOTHING)
            return 0;
    }

    return 1;
}

void Player::drawPlayer()
{
    cv::circle(screen->getMap(), center, RADIUS, playerColor, cv::FILLED);
    cv::putText(screen->getMap(), playerIDStr, center + aux, cv::FONT_HERSHEY_SIMPLEX, 0.35, cv::Scalar(0, 0, 0), 2);
}

void Player::updateVision()
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

    //only traitors see others traitors
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

    //checks movement in an angle of 90 degres to each side
    for (float i = angle - M_PI_2; i < M_PI_2 + angle; i += 0.1)
    {
        pt.x = cos(i) * _RADIUS_TOTAL_DISTANCE;
        pt.y = sin(i) * _RADIUS_TOTAL_DISTANCE;

        pt += center + offset;

        if (pt.x >= LENGTH || pt.x < 0 || pt.y >= HEIGHT || pt.y < 0 || screen->colorToId(screen->getColor(pt)) != NOTHING)
            return 0;
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

cv::Point Player::killPlayer(int rayNumber)
{
    if (rayNumber < 0 || rayNumber > numberOfRays) //invalid position
        return cv::Point(-1, -1);

    double currentAngle;

    cv::Point enemyPoint;

    currentAngle = separationAngle * rayNumber;

    //enemy location
    enemyPoint.x = (_RADIUS_TOTAL_DISTANCE + raysDist[rayNumber]) * cos(currentAngle);
    enemyPoint.y = (_RADIUS_TOTAL_DISTANCE + raysDist[rayNumber]) * sin(currentAngle);

    enemyPoint += center;

    cv::circle(screen->getMap(), enemyPoint, 2, cv::Scalar(0, 0, 0), cv::FILLED);

    return enemyPoint;
}

void Player::takeDamage(int damage)
{
    life -= damage;
}