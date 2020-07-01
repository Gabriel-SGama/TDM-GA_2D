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
    touchRayID = nullptr;

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

void Player::setPlayerValues(Screen *screen, int playerID, cv::Point **playersCenter, float* checkMoveCos, float* checkMoveSin) {
    //----------------VISION----------------
    separationAngle = visionAngle / numberOfRays;
    angleCorrection = visionAngle / numberOfRays - separationAngle;

    separationAngle += angleCorrection;

    this->checkMoveCos = checkMoveCos;
    this->checkMoveSin = checkMoveSin;
    raysID = new int[numberOfRays];
    raysDist = new int[numberOfRays];
    touchRayID = new int[NUMBER_OF_TOUCH_SENSORS];

    //----------------ANN----------------
    // vision + position + life + direction + shot interval + playerTypeDynamic + 
    // number of touch sensors + other players position + memory
    ANNInputSize = numberOfRays * 2 + 2 + 1 + 1 + 1 + 1 + NUMBER_OF_TOUCH_SENSORS + (NUMBER_OF_PLAYERS * 2 - 2) + MEMORY_SIZE;

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

    playerStatus += (*output)[INDEX_PLAYER_TYPE_CHANGE] * 0.5;

    if(playerStatus > PLAYER_STATUS_INTERVAL)
        playerStatus = PLAYER_STATUS_INTERVAL;
    else if(playerStatus < 0)
        playerStatus = 0;

    if(playerStatus >= playerStatusS){
        if(lastPlayerType != SNIPER){
            change = true;

            life = ((float) life/ASSAULT_HEALTH) * SNIPER_HEALTH;

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
            if(lastPlayerType == SNIPER)
                life = ((float) life/SNIPER_HEALTH) * ASSAULT_HEALTH;
            else
                life = ((float) life/LIGHT_ASSAULT_HEALTH) * ASSAULT_HEALTH;

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
            life = ((float) life/ASSAULT_HEALTH) * LIGHT_ASSAULT_HEALTH;
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
    float currentAngle;

    cv::Point pt;
    
    for (int i = 0; i < NUMBER_OF_TOUCH_SENSORS; i++) {
        pt.x = center.x + cos(TOUCH_ANGLE_INTERVAL * i) * TOUCH_SENSOR_DIST;
        pt.y = center.y + sin(TOUCH_ANGLE_INTERVAL * i) * TOUCH_SENSOR_DIST;
        touchRayID[i] = screen->colorToId(screen->getColor(pt));

        if(pt.x > LENGTH || pt.x < 0 || pt.y > HEIGHT || pt.y < 0)
            touchRayID[i] = OBSTACLE;
        else
            touchRayID[i] = screen->colorToId(screen->getColor(pt));
        
        if(touchRayID[i] != NOTHING)
            cv::line(screen->getMap(), center, pt, screen->idToRay(touchRayID[i]));
    }
    

    currentAngle = direction - visionAngle / 2;
    for (int i = 0; i < numberOfRays; i++) {
        drawVisionLines(currentAngle, i);
        currentAngle += separationAngle;
    }
}

void Player::drawVisionLines(float currentAngle, int id) {
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

    // int startIndex = angle/(2.0*M_PI) * NUMBER_OF_ANGLES_INTERVAL;

    // if(startIndex > NUMBER_OF_ANGLES_TO_CHECK / 2){
    //     for (int angleCount = 0, angleIndex = startIndex - NUMBER_OF_ANGLES_TO_CHECK / 2; angleCount < NUMBER_OF_ANGLES_TO_CHECK; angleCount++, angleIndex++) {
    //         pt.x = checkMoveCos[angleIndex] * _RADIUS_TOTAL_DISTANCE;
    //         pt.y = checkMoveSin[angleIndex] * _RADIUS_TOTAL_DISTANCE;

    //         pt += center + offset;

    //         if (pt.x >= LENGTH || pt.x < 0 || pt.y >= HEIGHT || pt.y < 0 || screen->colorToId(screen->getColor(pt)) != NOTHING)
    //             return 0;
   
        
    //     }
    // }else{
    //     int negativeIndex = NUMBER_OF_ANGLES_TO_CHECK / 2 - startIndex;

    //     for (int angleCount = NUMBER_OF_ANGLES_INTERVAL - 1; angleCount > count; angleCount++)
    //     {
    //         /* code */
    //     }
        

    //     for (int angleCount = 0, angleIndex = startIndex - NUMBER_OF_ANGLES_TO_CHECK / 2; angleCount < NUMBER_OF_ANGLES_TO_CHECK; angleCount++, angleIndex++) {
    //         pt.x = checkMoveCos[angleIndex] * _RADIUS_TOTAL_DISTANCE;
    //         pt.y = checkMoveSin[angleIndex] * _RADIUS_TOTAL_DISTANCE;

    //         pt += center + offset;

    //         if (pt.x >= LENGTH || pt.x < 0 || pt.y >= HEIGHT || pt.y < 0 || screen->colorToId(screen->getColor(pt)) != NOTHING)
    //             return 0;
   
        
    //     }
    // }
    

    for (float i = angle - M_PI_2; i < M_PI_2 + angle; i += 0.15) {
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
        score -= 12;
        
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

        (*input).vector[i + 1] = raysDist[j] / 50.0;
    }

    //----------------PLAYERS CENTER----------------
    i = 2 * numberOfRays;
    for (j = 0; j < NUMBER_OF_PLAYERS; j++) {
        if (j == (playerID % NUMBER_OF_PLAYERS))
            continue;

        (*input).vector[i] = playersCenter[j]->x / 200.0;
        (*input).vector[i + 1] = playersCenter[j]->y / 200.0;

        i += 2;
    }
    // i--;
    i = 2 * numberOfRays + 2 * (NUMBER_OF_PLAYERS - 1);
    // i = 2 * numberOfRays;

    (*input).vector[i] = center.x / 200.0;
    (*input).vector[i + 1] = center.y / 200.0;
    (*input).vector[i + 2] = direction;
    (*input).vector[i + 3] = life;
    (*input).vector[i + 4] = timeShot;
    (*input).vector[i + 5] = playerStatus;

    i += 6;

    for (j = 0; j < NUMBER_OF_TOUCH_SENSORS; j++, i++) {
        if (touchRayID[j] == playerType)
            (*input).vector[i] = ALLY;
        else if (touchRayID[j] == NOTHING || touchRayID[j] == OBSTACLE)
            (*input).vector[i] = touchRayID[j];
        else
            (*input).vector[i] = ENEMY;
    }

    //----------------MEMORY----------------
    for (j = 0; j < MEMORY_SIZE; i++, j++) {
        (*input).vector[i] = (*output)[j + INDEX_SHOT + 1];
    }

    if (timeShot > 0)
        timeShot--;
}

void Player::reset(float life, bool resetScore) {
    if (resetScore)
        score = 0;

    alive = true;
    this->life = life;
    timeShot = shotInterval;
    lastPlayerType = playerType;
     
    //----------------RESET PLAYER TYPE----------------
    if(playerType == SNIPER) {
        playerStatus = playerStatusS;
        damage = SNIPER_DAMAGE;
        visionDist = SNIPER_VISION_DIST;

        timeShot = timeShot/shotInterval * SNIPER_SHOT_INTERVAL;
        
        shotInterval = SNIPER_SHOT_INTERVAL;
        speedLimit = SNIPER_SPEED_LIMIT;
        angularSpeedLimit = SNIPER_ANGULAR_SPEED_LIMIT;

        visionAngle = SNIPER_VISION_ANGLE;
        numberOfRays = SNIPER_NUMBER_OF_RAYS;
    
    } else if(lastPlayerType == ASSAULT){
        playerStatus = playerStatusA;
        damage = ASSAULT_DAMAGE;
        visionDist = ASSAULT_VISION_DIST;

        timeShot = timeShot/shotInterval * ASSAULT_SHOT_INTERVAL;
        
        shotInterval = ASSAULT_SHOT_INTERVAL;
        speedLimit = ASSAULT_SPEED_LIMIT;
        angularSpeedLimit = ASSAULT_ANGULAR_SPEED_LIMIT;

        visionAngle = ASSAULT_VISION_ANGLE;
        numberOfRays = ASSAULT_NUMBER_OF_RAYS;

    } else {
        playerStatus = playerStatusLA;
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

    setPosition();
    drawPlayer();
    updateVision();
    setComunInput();
}