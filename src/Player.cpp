#include <cmath>
#include "string.h"
#include "headers/Player.h"

Player::Player() {
    score = 0;
    alive = true;

    timeShot = shotInterval;
}

Player::~Player() {
}

bool Player::isAlive() {
    return alive;
}

void Player::setPlayerValues(Screen *screen, int playerID, cv::Point **playersCenter) {
    this->playerType = playerType;
    this->playerID = playerID;
    playerIDStr = std::to_string(playerID);
    this->screen = screen;
    this->playersCenter = playersCenter;

    setPosition();
    drawPlayer();
}

void Player::setANN(ANN *ann) {
    this->ann = ann;

    input = ann->getInputPtr();
    // output = ann->getOutputPtr();
}

void Player::setPosition() {

    do {
        center.x = rand() % (300) + initialPos.x;
        center.y = rand() % (150) + initialPos.y;

    } while (!checkPosition());
}

int Player::checkPosition() {
    cv::Point pt;

    for (float i = 0; i < M_PI * 2; i += 0.1) {
        pt.x = cos(i) * _RADIUS_TOTAL_DISTANCE;
        pt.y = sin(i) * _RADIUS_TOTAL_DISTANCE;

        pt += center;

        if (pt.x >= LENGTH || pt.x < 0 || pt.y >= HEIGHT || pt.y < 0 || screen->colorToId(screen->getColor(pt)) != NOTHING)
            return 0;
    }

    return 1;
}

void Player::drawPlayer() {
    cv::circle(screen->getMap(), center, RADIUS, playerColor, cv::FILLED);
    cv::line(screen->getMap(), center, cv::Point(cos(direction) * (RADIUS + 4), sin(direction) * (RADIUS + 4)) + center, playerRay, 3);
    // cv::putText(screen->getMap(), playerIDStr, center + aux, cv::FONT_HERSHEY_SIMPLEX, 0.35, cv::Scalar(0, 0, 0), 2);
}

void Player::updateVision() {
    double currentAngle;

    currentAngle = direction - visionAngle / 2;
    for (int i = 0; i < numberOfRays; i++) {
        drawVisionLines(currentAngle, i);
        currentAngle += separationAngle;
    }
}

void Player::drawVisionLines(double currentAngle, int id) {
    cv::Point pt;
    cv::Point finalPt;
    cv::Point offset;

    cv::Scalar color;

    int i;

    for (i = 0; i < visionDist; i++) {
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

    if (raysID[id] != NOTHING) {
        color = screen->idToRay(raysID[id]);
        cv::line(screen->getMap(), center + offset, finalPt, color);
    }
}

int Player::checkMove(cv::Point offset) {
    cv::Point pt;

    float angle = atan2(offset.y, offset.x);

    for (float i = angle - M_PI_2; i < M_PI_2 + angle; i += 0.1) {
        pt.x = cos(i) * _RADIUS_TOTAL_DISTANCE;
        pt.y = sin(i) * _RADIUS_TOTAL_DISTANCE;

        pt += center + offset;

        if (pt.x >= LENGTH || pt.x < 0 || pt.y >= HEIGHT || pt.y < 0 || screen->colorToId(screen->getColor(pt)) != NOTHING)
            return 0;

        //cv::circle(screen->getMap(), pt, 1, cv::Scalar(0, 1, 255), cv::FILLED);
    }
    return 1;
}

void Player::move() {
    direction += outputTest[INDEX_DIRECTION] * angularSpeedLimit;

    if (direction > 2 * M_PI)
        direction -= 2 * M_PI;
    else if (direction < -2 * M_PI)
        direction += 2 * M_PI;

    int frontSpeed = speedLimit * outputTest[INDEX_FRONT_SPEED];
    int sideSpeed = speedLimit * outputTest[INDEX_SIDE_SPEED];

    cv::Point offset = cv::Point((int)(frontSpeed * cos(direction) + sideSpeed * cos(direction + M_PI_2)), (int)(frontSpeed * sin(direction) + sideSpeed * sin(direction + M_PI_2)));

    if (checkMove(offset))
        center += offset;
}

enemyInfo_t Player::killPlayer(int rayNumber) {
    if (rayNumber < 0 || rayNumber > numberOfRays || timeShot > 0) {
        if (timeShot <= 0)
            timeShot = shotInterval;

        return {cv::Point(-1, -1), NOTHING};
    }

    timeShot = shotInterval;

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

void Player::takeDamage(int damage) {
    life -= damage;
}

void Player::setAlive(bool alive, int turn) {
    this->alive = alive;

    if (!alive) {
        updateScore(-3);
        center.x = 0;
        center.y = 0;
    }
}

void Player::setComunInput() {
    int i = 0;
    int j;
    ///*
    for (i = 0, j = 0; i < 2 * numberOfRays; i += 2, j++) {
        if (raysID[j] == playerType)
            (*input)[i] = ALLY;
        else if (raysID[j] == NOTHING || raysID[j] == OBSTACLE)
            (*input)[i] = raysID[j];
        else
            (*input)[i] = ENEMY;

        (*input)[i + 1] = raysDist[j] / 10.0;
        // (*input)[i + 1] = raysDist[j];
    }
    //*/

    ///*
    /*
    i = 2 * numberOfRays;
    for (j = 0; j < NUMBER_OF_LIGHT_ASSAULTS; i += 2, j++) {
        if (j == playerID) {
            i -= 2;
            continue;
        }

        (*input)[i] = (center.x - playersCenter[j]->x) / 50.0;
        (*input)[i + 1] = (center.y - playersCenter[j]->y) / 50.0;
    }
    */
    //*/

    // i = 2 * (numberOfRays + NUMBER_OF_LIGHT_ASSAULTS - 1);
    i = 2 * numberOfRays;

    (*input)[i] = center.x / 50.0;
    (*input)[i + 1] = center.y / 50.0;
    (*input)[i + 2] = direction;
    (*input)[i + 3] = life / 10.0;

    // (*input)[i] = center.x;
    // (*input)[i + 1] = center.y;
    // (*input)[i + 2] = direction;
    // (*input)[i + 3] = life;

    timeShot--;
}

void Player::reset(int life, bool resetScore) {
    if (resetScore)
        score = 0;

    alive = true;
    this->life = life;
    timeShot = shotInterval;

    setPosition();
    drawPlayer();
    updateVision();
    setComunInput();
}