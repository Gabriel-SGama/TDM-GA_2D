#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "string.h"
#include "headers/Player.h"

Player::Player() {
    score = 0;
    alive = true;

    timeShot = shotInterval;

    raysDist = nullptr;
    raysID = nullptr;

    screen = nullptr;

    playersCenter = nullptr;

    input = nullptr;
    output = nullptr;
    ann = nullptr;
}

Player::~Player() {
}

bool Player::isAlive() {
    return alive;
}

void Player::setPlayerValues(Screen *screen, int playerID, cv::Point **playersCenter) {
    //----------------VISION----------------
    separationAngle = visionAngle / numberOfRays;
    angleCorrection = visionAngle / numberOfRays - separationAngle;

    separationAngle += angleCorrection;

    raysID = new int[numberOfRays];
    raysDist = new int[numberOfRays];

    //----------------ANN----------------
    // vision + position + life + direction + shot interval + playerTypeDynamic + speedLimit 
    // + angulaSpeedLimit + vision dist + other players position + memory
    ANNInputSize = numberOfRays * 2 + 2 + 1 + 1 + 1 + 1 + (NUMBER_OF_PLAYERS * 2 - 2) + MEMORY_SIZE;

    //angle + front speed + shot + memory + playerTypeChange
    ANNOutputSize = 1 + 1 + 1 + 1 + MEMORY_SIZE;

    ann = new ANN;
    ann->setANNParameters(ANNInputSize, ANNOutputSize);

    input = ann->getInputPtr();
    output = ann->getOutputPtr();

    //----------------PLAYER VALUES----------------
    this->playerID = playerID;
    playerIDStr = std::to_string(playerID);
    this->screen = screen;
    this->playersCenter = playersCenter;

    setPosition();
    drawPlayer();
}

void Player::setPosition() {
    do {
        // center.x = rand() % (LENGTH - safeDist);
        // center.y = rand() % (HEIGHT - safeDist);
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
    bool change = false;

    cv::circle(screen->getMap(), center, RADIUS, playerColor, cv::FILLED);
    
    playerStatus += (*output)[INDEX_PLAYER_TYPE_CHANGE] * 0.1;

    if(playerStatus > PLAYER_STATUS_INTERVAL)
        playerStatus = PLAYER_STATUS_INTERVAL;
    else if(playerStatus < 0)
        playerStatus = 0;

    if(playerStatus >= playerStatusS){
        if(lastPlayerType != SNIPER){
            change = true;
            lastPlayerType = SNIPER;
            damage = SNIPER_DAMAGE;
            visionDist = SNIPER_VISION_DIST;

            timeShot = timeShot/shotInterval * SNIPER_SHOT_INTERVAL;
            
            shotInterval = SNIPER_SHOT_INTERVAL;
            speedLimit = SNIPER_SPEED_LIMIT;
            angularSpeedLimit = SNIPER_ANGULAR_SPEED_LIMIT;

            visionAngle = SNIPER_VISION_ANGLE;
            numberOfRays = SNIPER_NUMBER_OF_RAYS;
        }
        
        cv::line(screen->getMap(), center, cv::Point(cos(direction) * (RADIUS + 4), sin(direction) * (RADIUS + 4)) + center, SNIPER_RAY, 3);
    
    }else if(playerStatus >= playerStatusA){
        if(lastPlayerType != ASSAULT){
            change = true;
            lastPlayerType = ASSAULT;
            damage = ASSAULT_DAMAGE;
            visionDist = ASSAULT_VISION_DIST;

            timeShot = timeShot/shotInterval * ASSAULT_SHOT_INTERVAL;
            
            shotInterval = ASSAULT_SHOT_INTERVAL;
            speedLimit = ASSAULT_SPEED_LIMIT;
            angularSpeedLimit = ASSAULT_ANGULAR_SPEED_LIMIT;

            visionAngle = ASSAULT_VISION_ANGLE;
            numberOfRays = ASSAULT_NUMBER_OF_RAYS;
        }
        
        cv::line(screen->getMap(), center, cv::Point(cos(direction) * (RADIUS + 4), sin(direction) * (RADIUS + 4)) + center, ASSAULT_RAY, 3);

    }else {
        if(lastPlayerType != LIGHT_ASSAULT){
            change = true;
            lastPlayerType = LIGHT_ASSAULT;
            damage = LIGHT_ASSAULT_DAMAGE;
            visionDist = LIGHT_ASSAULT_VISION_DIST;

            timeShot = timeShot/shotInterval * LIGHT_ASSAULT_SHOT_INTERVAL;
            
            shotInterval = LIGHT_ASSAULT_SHOT_INTERVAL;
            speedLimit = LIGHT_ASSAULT_SPEED_LIMIT;
            angularSpeedLimit = LIGHT_ASSAULT_ANGULAR_SPEED_LIMIT;

            visionAngle = LIGHT_ASSAULT_VISION_ANGLE;
            numberOfRays = LIGHT_ASSAULT_NUMBER_OF_RAYS;
        }
        cv::line(screen->getMap(), center, cv::Point(cos(direction) * (RADIUS + 4), sin(direction) * (RADIUS + 4)) + center, LIGHT_ASSAULT_RAY, 3);
    }

    if(change){
        separationAngle = visionAngle / numberOfRays;
        angleCorrection = visionAngle / numberOfRays - separationAngle;

        separationAngle += angleCorrection;
    }

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

    const float cosCA = cos(currentAngle);
    const float sinCA = sin(currentAngle);


    offset.x = _RADIUS_TOTAL_DISTANCE * cosCA;
    offset.y = _RADIUS_TOTAL_DISTANCE * sinCA;

    for (i = 0; i < visionDist; i++) {
        pt.x = i * cosCA;
        pt.y = i * sinCA;

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
    direction += output[0][INDEX_DIRECTION] * angularSpeedLimit;

    if (direction > 2 * M_PI)
        direction -= 2 * M_PI;
    else if (direction < -2 * M_PI)
        direction += 2 * M_PI;

    int frontSpeed = speedLimit * output[0][INDEX_FRONT_SPEED];
    if (frontSpeed < 0)
        frontSpeed = 0;

    // int sideSpeed = speedLimit * output[0][INDEX_SIDE_SPEED];

    // cv::Point offset = cv::Point((int)(frontSpeed * cos(direction) + sideSpeed * cos(direction + M_PI_2)), (int)(frontSpeed * sin(direction) + sideSpeed * sin(direction + M_PI_2)));
    cv::Point offset = cv::Point((int)frontSpeed * cos(direction), (int)frontSpeed * sin(direction));

    if (checkMove(offset))
        center += offset;
}

enemyInfo_t Player::killPlayer() {
    if (timeShot > 0) {
        timeShot--;
        return {cv::Point(-1, -1), NOTHING};
    }

    timeShot = shotInterval;

    int i;
    int id;

    cv::Point offset;
    cv::Point finalPt;
    cv::Point pt;
    cv::Point enemyPoint;

    const float cosDir = cos(direction);
    const float sinDir = sin(direction);

    offset.x = _RADIUS_TOTAL_DISTANCE * cosDir;
    offset.y = _RADIUS_TOTAL_DISTANCE * sinDir;

    for (i = 0; i < visionDist; i++) {
        pt.x = i * cosDir;
        pt.y = i * sinDir;

        finalPt = pt + center + offset;

        id = screen->colorToId(screen->getColor(finalPt));

        if (id != NOTHING)
            break;
    }

    enemyPoint = finalPt;

    cv::line(screen->getMap(), center, enemyPoint, cv::Scalar(0, 0, 0), 1);
    cv::circle(screen->getMap(), enemyPoint, 2, cv::Scalar(0, 0, 0), cv::FILLED);

    return {enemyPoint, id};
}

void Player::takeDamage(int damage) {
    life -= damage;
}

void Player::setAlive(bool alive) {
    this->alive = alive;

    if (!alive) {
        score -= 4;
        
        center.x = 0;
        center.y = 0;
    }
}

void Player::setComunInput() {
    int i;
    int j;
    
    //----------------VISION----------------
    for (i = 0, j = 0; i < 2 * numberOfRays; i += 2, j++) {
        if (raysID[j] == playerType)
            (*input).vector[i] = ALLY;
        else if (raysID[j] == NOTHING || raysID[j] == OBSTACLE)
            (*input).vector[i] = raysID[j];
        else
            (*input).vector[i] = ENEMY;

        (*input).vector[i + 1] = raysDist[j] / 10.0;
    }

    // for (i = 0, j = 0; i < 2 * numberOfRays; i += 2, j++) {
    //     if (raysID[j] == playerType)
    //         (*input)[i] = ALLY / 50.0;
    //     else if (raysID[j] == NOTHING || raysID[j] == OBSTACLE)
    //         (*input)[i] = raysID[j] / 50.0;
    //     else
    //         (*input)[i] = ENEMY / 50.0;

    //     (*input)[i + 1] = raysDist[j] / visionDist;
    // }

    //----------------PLAYERS CENTER----------------
    i = 2 * numberOfRays;
    for (j = 0; j < NUMBER_OF_PLAYERS; j++) {
        if (j == (playerID % NUMBER_OF_PLAYERS))
            continue;

        // (*input)[i] = playersCenter[j]->x / 50.0;
        // (*input)[i + 1] = playersCenter[j]->y / 50.0;

        (*input).vector[i] = playersCenter[j]->x / 50.0;
        (*input).vector[i + 1] = playersCenter[j]->y / 50.0;

        i += 2;
    }
    // i--;
    i = 2 * numberOfRays + 2 * (NUMBER_OF_PLAYERS - 1);
    // i = 2 * numberOfRays;

    (*input).vector[i] = center.x / 50.0;
    (*input).vector[i + 1] = center.y / 50.0;
    (*input).vector[i + 2] = direction;
    (*input).vector[i + 3] = life;
    (*input).vector[i + 4] = timeShot;
    (*input).vector[i + 5] = playerStatus;

    // (*input)[i] = center.x / LENGTH;
    // (*input)[i + 1] = center.y / LENGTH;
    // (*input)[i + 2] = direction / (2.0*M_PI);
    // (*input)[i + 3] = life / 100.0;
    // (*input)[i + 4] = 1.0 * timeShot / shotInterval;

    i += 8;

    //----------------MEMORY----------------
    for (j = 0; j < MEMORY_SIZE; i++, j++) {
        (*input).vector[i] = (*output)[j + INDEX_SHOT + 1];
    }

    if (timeShot > 0)
        timeShot--;
}

void Player::reset(int life, bool resetScore) {
    if (resetScore)
        score = 0;

    alive = true;
    this->life = life;
    timeShot = shotInterval;
    lastPlayerType = playerType;
     
    //----------------RESET PLAYER TYPE----------------
    if(playerType == SNIPER) {
        damage = SNIPER_DAMAGE;
        visionDist = SNIPER_VISION_DIST;

        timeShot = timeShot/shotInterval * SNIPER_SHOT_INTERVAL;
        
        shotInterval = SNIPER_SHOT_INTERVAL;
        speedLimit = SNIPER_SPEED_LIMIT;
        angularSpeedLimit = SNIPER_ANGULAR_SPEED_LIMIT;

        visionAngle = SNIPER_VISION_ANGLE;
        numberOfRays = SNIPER_NUMBER_OF_RAYS;
    
    } else if(lastPlayerType == ASSAULT){
        damage = ASSAULT_DAMAGE;
        visionDist = ASSAULT_VISION_DIST;

        timeShot = timeShot/shotInterval * ASSAULT_SHOT_INTERVAL;
        
        shotInterval = ASSAULT_SHOT_INTERVAL;
        speedLimit = ASSAULT_SPEED_LIMIT;
        angularSpeedLimit = ASSAULT_ANGULAR_SPEED_LIMIT;

        visionAngle = ASSAULT_VISION_ANGLE;
        numberOfRays = ASSAULT_NUMBER_OF_RAYS;

    } else {
        damage = LIGHT_ASSAULT_DAMAGE;
        visionDist = LIGHT_ASSAULT_VISION_DIST;

        timeShot = timeShot/shotInterval * LIGHT_ASSAULT_SHOT_INTERVAL;
        
        shotInterval = LIGHT_ASSAULT_SHOT_INTERVAL;
        speedLimit = LIGHT_ASSAULT_SPEED_LIMIT;
        angularSpeedLimit = LIGHT_ASSAULT_ANGULAR_SPEED_LIMIT;

        visionAngle = LIGHT_ASSAULT_VISION_ANGLE;
        numberOfRays = LIGHT_ASSAULT_NUMBER_OF_RAYS;
    }

    separationAngle = visionAngle / numberOfRays;
    angleCorrection = visionAngle / numberOfRays - separationAngle;

    separationAngle += angleCorrection;

    //----------------RESET PLAYER TYPE INPUT VALUES----------------
    int posiStart = 2 * numberOfRays + 2 * (NUMBER_OF_PLAYERS - 1);

    (*input).vector[posiStart + 5] = playerStatus;

    setPosition();
    drawPlayer();
    updateVision();
    setComunInput();
}