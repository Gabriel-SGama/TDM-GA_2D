#include <iostream>
#include <vector>

#include "headers/Moderator.h"
#include "headers/Evolution.h"

Moderator::Moderator() {
}

Moderator::~Moderator() {
}

void Moderator::setModerator() {
    trainingScore = 0;

    //allocs all memory
    trainingTeam = new Player[NUMBER_OF_PLAYERS];
    bestTeam = new Player[NUMBER_OF_PLAYERS];
    // playersCenter = new cv::Point *[NUMBER_OF_TOTAL_PLAYERS];

    trainingCenter = new cv::Point *[NUMBER_OF_PLAYERS];
    bestCenter = new cv::Point *[NUMBER_OF_PLAYERS];

    //neuron networks for each player
    trainingANN = new ANN;
    bestANN = new ANN;

    //set initial values of the neuron network
    trainingANN->setANNParameters(trainingTeam->ANNInputSize, trainingTeam->ANNOutputSize);
    bestANN->setANNParameters(bestTeam->ANNInputSize, bestTeam->ANNOutputSize);

    //difines each to players
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        trainingTeam[i].setANN(trainingANN);
        bestTeam[i].setANN(bestANN);
    }

    setPlayerCenterPtr(trainingTeam, trainingCenter);
    setPlayerCenterPtr(bestTeam, bestCenter);
}

void Moderator::setPlayerCenterPtr(Player *players, cv::Point **pointPtr) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
        pointPtr[i] = players[i].getCenterPtr();
}

void Moderator::setScreen(Screen *screen) {
    this->screen = screen;

    screen->resetImage();
    screen->createObstacle();
}

void Moderator::setInicialPosAll(cv::Point *inicialPos, int start) {
    setInicialPos(trainingTeam, inicialPos[start]);
    start++;
    setInicialPos(bestTeam, inicialPos[start%3]);
}

void Moderator::setInicialPos(Player *players, cv::Point initialPos) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        players[i].initialPos = initialPos;
    }
}

void Moderator::setAllPlayersValues() {
    setPlayersValues(trainingTeam, trainingCenter, TRAINING_PLAYER, TRAINING_COLOR, TRAINING_RAY);
    setPlayersValues(bestTeam, bestCenter, BEST_PLAYER, BEST_COLOR, BEST_RAY);
}

void Moderator::setPlayersValues(Player *players, cv::Point **centerPtr, int playerType, cv::Scalar color, cv::Scalar ray) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
        players[i].setPlayerValues(screen, i, centerPtr, playerType, color, ray);
}

void Moderator::drawAllPlayers() {
    screen->resetImage();

    cv::rectangle(screen->getMap(), cv::Point(0, 150), cv::Point(300, 300), cv::Scalar(0, 0, 0), 1);
    cv::rectangle(screen->getMap(), cv::Point(LENGTH - 300, HEIGHT - 150), cv::Point(LENGTH, HEIGHT), cv::Scalar(0, 0, 0), 1);
    cv::rectangle(screen->getMap(), cv::Point(LENGTH - 400, 150), cv::Point(LENGTH - 100, 300), cv::Scalar(0, 0, 0), 1);

    screen->createObstacle();

    drawPlayers(trainingTeam);
    drawPlayers(bestTeam);
}

void Moderator::drawPlayers(Player *players) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive())
            players[i].drawPlayer();
    }
}

void Moderator::updateAllPlayersVision() {
    updatePlayersVision(trainingTeam);
    updatePlayersVision(bestTeam);
}

void Moderator::updatePlayersVision(Player *players) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive())
            players[i].updateVision();
    }
}

void Moderator::conflictsAllPlayers() {
    conflictsPlayers(trainingTeam, trainingTeam->numberOfRays);
    conflictsPlayers(bestTeam, bestTeam->numberOfRays);
}

void Moderator::conflictsPlayers(Player *players, int numberOfRays) {
    enemyInfo_t enemyInfo;

    int maxIndex = 0;
    int k;
    float max = -1;

    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive()) {
            for (k = INDEX_SHOT; k < numberOfRays + INDEX_SHOT; k++) {
                if (players[i].outputTest[k] > max) {
                    max = players[i].outputTest[k];
                    maxIndex = k;
                }
            }

            if (max > 0.5) {
                enemyInfo = players[i].killPlayer(maxIndex - INDEX_SHOT);
            } else
                continue;

            if (enemyInfo.playerType == NOTHING || enemyInfo.playerType == OBSTACLE)
                continue;

            shotPlayer(&players[i], enemyInfo);
        }
    }
}

void Moderator::shotPlayer(Player *shooter, enemyInfo_t enemyInfo) {
    //find the player that was shot
    if (enemyInfo.playerType == TRAINING_PLAYER && findPlayer(shooter, trainingTeam, enemyInfo.posiAprox)) {
        if (shooter->getPlayerType() != TRAINING_PLAYER)
            shooter->updateScore(SHOT_REWARD);
        else
            shooter->updateScore(-2.5 * SHOT_REWARD);
    }

    else if (findPlayer(shooter, bestTeam, enemyInfo.posiAprox)) {
        if (shooter->getPlayerType() != BEST_PLAYER)
            shooter->updateScore(SHOT_REWARD);
        else
            shooter->updateScore(-2.5 * SHOT_REWARD);
    }
}

int Moderator::findPlayer(Player *shooter, Player *players, cv::Point enemyPoint) {
    float distance;

    //find the player that was shot
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (!players[i].isAlive() /* || players[i].getPlayerID() == shooter->getPlayerID()*/)
            continue;

        distance = cv::norm(players[i].getCenter() - enemyPoint);

        if (distance <= RADIUS + 1) {
            players[i].takeDamage(shooter->getDamage());
            int shooterType = shooter->getPlayerType();

            if (shooterType != players[i].getPlayerType()) {
                if (shooterType == TRAINING_PLAYER)
                    trainingScore++;
                
            } else {
                if (shooterType == TRAINING_PLAYER)
                    trainingScore--;
            }
            return 1;
        }
    }
    return 0;
}

bool Moderator::checkAllPlayersLife() {
    bool TT = checkPlayersLife(trainingTeam);
    bool BT = checkPlayersLife(bestTeam);

    if (TT && BT)
        return true;

    return false;
}

bool Moderator::checkPlayersLife(Player *players) {
    bool match = false;

    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive()) {
            match = true;
            if (players[i].getLife() <= 0)
                players[i].setAlive(false);
        }
    }

    return match;
}

void Moderator::moveAllPlayers() {
    movePlayers(trainingTeam);
    movePlayers(bestTeam);
}

void Moderator::movePlayers(Player *players) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive())
            players[i].move();
    }
}

void Moderator::multiplyAllPlayers() {
    multiplyPlayers(trainingANN, trainingTeam);
    multiplyPlayers(bestANN, bestTeam);
}

void Moderator::multiplyPlayers(ANN *ann, Player *players) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive()) {
            players[i].setComunInput();
            players[i].copyOutput(ann->multiply());
        }
    }
}

void Moderator::defineAllPlayersInput() {
    definePlayersInput(trainingTeam);
    definePlayersInput(bestTeam);
}

void Moderator::definePlayersInput(Player *players) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive())
            players[i].setComunInput();
    }
}

void Moderator::resetAllPlayers(bool resetScore) {
    screen->resetImage();
    screen->createObstacle();

    trainingScore = 0;

    resetPlayers(trainingTeam, resetScore);
    resetPlayers(bestTeam, resetScore);
}

void Moderator::resetPlayers(Player *players, bool resetScore) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
        players[i].reset(resetScore);
}

void Moderator::setAllWeightsMod(ANN *trainingMatrix, ANN *bestMatrix) {
    if (trainingMatrix != nullptr)
        this->trainingANN->copyWheights(trainingMatrix->getMatrixPtr());
    if (bestMatrix != nullptr)
        this->bestANN->copyWheights(bestMatrix->getMatrixPtr());
}

void Moderator::game() {
    for (int turn = 0; turn < DURATION; turn++) {
        drawAllPlayers();
        updateAllPlayersVision();

        multiplyAllPlayers();

        conflictsAllPlayers();
        if (!checkAllPlayersLife())
            break;

        moveAllPlayers();
    }
}

void Moderator::gameOfBest() {
    for (int i = 0; i < DURATION; i++) {
        drawAllPlayers();
        updateAllPlayersVision();

        multiplyAllPlayers();

        conflictsAllPlayers();
        screen->updateMap();

        if (!checkAllPlayersLife())
            break;

        moveAllPlayers();
    }
}

float Moderator::getScore() {
    
    return trainingScore;
}

void Moderator::setScore(float score) {
    
    trainingScore = score;
}

MatrixXf *Moderator::getMatrixPtr(int id) {
    if (id == TRAINING_PLAYER)
        return trainingANN->getMatrixPtr();

    return bestANN->getMatrixPtr();

}

ANN *Moderator::getANN(int id) {
    if (id == TRAINING_PLAYER)
        return trainingANN;

    return bestANN;
}

MatrixXf *Moderator::setMatrix(MatrixXf *matrixs, int id) {
    if (id == TRAINING_PLAYER)
        return trainingANN->setMatrix(matrixs);

    return bestANN->setMatrix(matrixs);
}