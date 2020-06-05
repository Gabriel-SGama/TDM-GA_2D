#include <iostream>
#include <vector>

#include "headers/Moderator.h"
#include "headers/Evolution.h"

Moderator::Moderator() {
}

Moderator::~Moderator() {
}

void Moderator::setModerator(int NUMBER_OF_LIGHT_ASSAULT_TRAIN, int NUMBER_OF_SNIPER_TRAIN, int NUMBER_OF_ASSAULT_TRAIN) {
    turn = 0;
    lightAssaultScore = 0;
    sniperScore = 0;
    assaultScore = 0;

    this->NUMBER_OF_LIGHT_ASSAULT_TRAIN = NUMBER_OF_LIGHT_ASSAULT_TRAIN;
    this->NUMBER_OF_SNIPER_TRAIN = NUMBER_OF_SNIPER_TRAIN;
    this->NUMBER_OF_ASSAULT_TRAIN = NUMBER_OF_ASSAULT_TRAIN;

    //allocs all memory
    lightAssaults = new LightAssault[NUMBER_OF_LIGHT_ASSAULT_TRAIN];
    snipers = new Sniper[NUMBER_OF_SNIPER_TRAIN];
    assaults = new Assault[NUMBER_OF_ASSAULT_TRAIN];

    bestLightAssault = new dataOfBestPlayers_t;
    bestSniper = new dataOfBestPlayers_t;
    bestAssault = new dataOfBestPlayers_t;

    bestLightAssault->player = lightAssaults;
    bestSniper->player = snipers;
    bestAssault->player = assaults;
    bestLightAssault->score = INICIAL_SCORE;
    bestSniper->score = INICIAL_SCORE;
    bestAssault->score = INICIAL_SCORE;

    // playersCenter = new cv::Point *[NUMBER_OF_TOTAL_PLAYERS];

    lightAssaultCenter = new cv::Point *[NUMBER_OF_LIGHT_ASSAULTS];
    sniperCenter = new cv::Point *[NUMBER_OF_SNIPERS];
    assaultCenter = new cv::Point *[NUMBER_OF_ASSAULTS];

    //neuron networks for each player
    lightAssaultANN = new ANN;
    sniperANN = new ANN;
    assaultANN = new ANN;

    //set initial values of the neuron network
    lightAssaultANN->setANNParameters(lightAssaults->ANNInputSize, lightAssaults->ANNOutputSize);
    sniperANN->setANNParameters(snipers->ANNInputSize, snipers->ANNOutputSize);
    assaultANN->setANNParameters(assaults->ANNInputSize, assaults->ANNOutputSize);

    //difines each to players
    for (int i = 0; i < NUMBER_OF_LIGHT_ASSAULTS; i++) {
        lightAssaults[i].setANN(lightAssaultANN);
        snipers[i].setANN(sniperANN);
        assaults[i].setANN(assaultANN);
    }

    setPlayerCenterPtr(lightAssaults, NUMBER_OF_LIGHT_ASSAULT_TRAIN, lightAssaultCenter);
    setPlayerCenterPtr(snipers, NUMBER_OF_SNIPER_TRAIN, sniperCenter);
    setPlayerCenterPtr(assaults, NUMBER_OF_ASSAULT_TRAIN, assaultCenter);
}

void Moderator::setPlayerCenterPtr(Player *players, int NUMBER_OF_PLAYERS, cv::Point **pointPtr) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
        pointPtr[i] = players[i].getCenterPtr();
}

void Moderator::setScreen(Screen *screen) {
    this->screen = screen;

    screen->resetImage();
    screen->createObstacle();
}

void Moderator::setInicialPosAll(cv::Point *inicialPos, int start) {
    setInicialPos(lightAssaults, NUMBER_OF_LIGHT_ASSAULT_TRAIN, inicialPos[start]);
    start++;
    setInicialPos(snipers, NUMBER_OF_SNIPER_TRAIN, inicialPos[start%3]);
    start++;
    setInicialPos(assaults, NUMBER_OF_ASSAULT_TRAIN, inicialPos[start%3]);
}

void Moderator::setInicialPos(Player *players, int NUMBER_OF_PLAYERS, cv::Point initialPos) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        players[i].initialPos = initialPos;
    }
}

void Moderator::setAllPlayersValues() {
    setPlayersValues(lightAssaults, NUMBER_OF_LIGHT_ASSAULT_TRAIN, lightAssaultCenter);
    setPlayersValues(snipers, NUMBER_OF_SNIPER_TRAIN, sniperCenter);
    setPlayersValues(assaults, NUMBER_OF_ASSAULT_TRAIN, assaultCenter);
}

void Moderator::setPlayersValues(Player *players, int NUMBER_OF_PLAYERS, cv::Point **centerPtr) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
        players[i].setPlayerValues(screen, i, centerPtr);
}

void Moderator::drawAllPlayers() {
    screen->resetImage();

    cv::rectangle(screen->getMap(), cv::Point(0, 150), cv::Point(300, 300), cv::Scalar(255, 1, 0), 1);
    cv::rectangle(screen->getMap(), cv::Point(LENGTH - 300, HEIGHT - 150), cv::Point(LENGTH, HEIGHT), cv::Scalar(0, 255, 1), 1);
    cv::rectangle(screen->getMap(), cv::Point(LENGTH - 400, 150), cv::Point(LENGTH - 100, 300), cv::Scalar(0, 1, 255), 1);

    screen->createObstacle();

    drawPlayers(lightAssaults, NUMBER_OF_LIGHT_ASSAULT_TRAIN);
    drawPlayers(snipers, NUMBER_OF_SNIPER_TRAIN);
    drawPlayers(assaults, NUMBER_OF_ASSAULT_TRAIN);
}

void Moderator::drawPlayers(Player *players, int NUMBER_OF_PLAYERS) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive())
            players[i].drawPlayer();
    }
}

void Moderator::updateAllPlayersVision() {
    updatePlayersVision(lightAssaults, NUMBER_OF_LIGHT_ASSAULT_TRAIN);
    updatePlayersVision(snipers, NUMBER_OF_SNIPER_TRAIN);
    updatePlayersVision(assaults, NUMBER_OF_ASSAULT_TRAIN);
}

void Moderator::updatePlayersVision(Player *players, int NUMBER_OF_PLAYERS) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive())
            players[i].updateVision();
    }
}

void Moderator::conflictsAllPlayers() {
    conflictsPlayers(lightAssaults, NUMBER_OF_LIGHT_ASSAULT_TRAIN, lightAssaults->numberOfRays);
    conflictsPlayers(snipers, NUMBER_OF_SNIPER_TRAIN, snipers->numberOfRays);
    conflictsPlayers(assaults, NUMBER_OF_ASSAULT_TRAIN, assaults->numberOfRays);
}

void Moderator::conflictsPlayers(Player *players, int NUMBER_OF_PLAYERS, int numberOfRays) {
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
    if (enemyInfo.playerType == LIGHT_ASSAULT && findPlayer(shooter, lightAssaults, NUMBER_OF_LIGHT_ASSAULT_TRAIN, enemyInfo.posiAprox)) {
        if (shooter->getPlayerType() != LIGHT_ASSAULT)
            shooter->updateScore(LIGHT_ASSAULT_SHOT_REWARD);
        else
            shooter->updateScore(-2.5 * LIGHT_ASSAULT_SHOT_REWARD);
    }

    else if (enemyInfo.playerType == SNIPER && findPlayer(shooter, snipers, NUMBER_OF_SNIPER_TRAIN, enemyInfo.posiAprox)) {
        if (shooter->getPlayerType() != SNIPER)
            shooter->updateScore(SNIPER_SHOT_REWARD);
        else
            shooter->updateScore(-2.5 * SNIPER_SHOT_REWARD);

    } else if (enemyInfo.playerType == ASSAULT && findPlayer(shooter, assaults, NUMBER_OF_ASSAULT_TRAIN, enemyInfo.posiAprox)) {
        if (shooter->getPlayerType() != ASSAULT)
            shooter->updateScore(ASSAULT_SHOT_REWARD);
        else
            shooter->updateScore(-2.5 * ASSAULT_SHOT_REWARD);
    }
}

int Moderator::findPlayer(Player *shooter, Player *players, int NUMBER_OF_PLAYERS, cv::Point enemyPoint) {
    float distance;

    //find the player that was shot
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (!players[i].isAlive() /* || players[i].getPlayerID() == shooter[i].getPlayerID()*/)
            continue;

        distance = cv::norm(players[i].getCenter() - enemyPoint);

        if (distance <= RADIUS + 1) {
            players[i].takeDamage(shooter->getDamage());
            int shooterType = shooter->getPlayerType();

            if (shooterType != players[i].getPlayerType()) {
                if (shooterType == LIGHT_ASSAULT)
                    lightAssaultScore++;
                else if (shooterType == SNIPER)
                    sniperScore++;
                else
                    assaultScore++;
            } else {
                if (shooterType == LIGHT_ASSAULT)
                    lightAssaultScore--;
                else if (shooterType == SNIPER)
                    sniperScore--;
                else
                    assaultScore--;
            }
            return 1;
        }
    }
    return 0;
}

bool Moderator::checkAllPlayersLife() {
    bool LAL = checkPlayersLife(lightAssaults, NUMBER_OF_LIGHT_ASSAULT_TRAIN);
    bool SL = checkPlayersLife(snipers, NUMBER_OF_SNIPER_TRAIN);
    bool AL = checkPlayersLife(assaults, NUMBER_OF_ASSAULT_TRAIN);

    if ((LAL && SL) || (LAL && AL) || (SL && AL))
        return true;

    return false;
}

bool Moderator::checkPlayersLife(Player *players, int NUMBER_OF_PLAYERS) {
    bool match = false;

    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive()) {
            match = true;
            if (players[i].getLife() <= 0)
                players[i].setAlive(false, turn);
        }
    }

    return match;
}

void Moderator::moveAllPlayers() {
    movePlayers(lightAssaults, NUMBER_OF_LIGHT_ASSAULT_TRAIN);
    movePlayers(snipers, NUMBER_OF_SNIPER_TRAIN);
    movePlayers(assaults, NUMBER_OF_ASSAULT_TRAIN);
}

void Moderator::movePlayers(Player *players, int NUMBER_OF_PLAYERS) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive())
            players[i].move();
    }
}

void Moderator::multiplyAllPlayers() {
    multiplyPlayers(lightAssaultANN, lightAssaults, NUMBER_OF_LIGHT_ASSAULT_TRAIN);
    multiplyPlayers(sniperANN, snipers, NUMBER_OF_SNIPER_TRAIN);
    multiplyPlayers(assaultANN, assaults, NUMBER_OF_ASSAULT_TRAIN);
}

void Moderator::multiplyPlayers(ANN *ann, Player *players, int NUMBER_OF_PLAYERS) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive()) {
            players[i].setComunInput();
            players[i].copyOutput(ann->multiply());
        }
    }
}

void Moderator::defineAllPlayersInput() {
    definePlayersInput(lightAssaults, NUMBER_OF_LIGHT_ASSAULT_TRAIN);
    definePlayersInput(snipers, NUMBER_OF_SNIPER_TRAIN);
    definePlayersInput(assaults, NUMBER_OF_ASSAULT_TRAIN);
}

void Moderator::definePlayersInput(Player *players, int NUMBER_OF_PLAYERS) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive())
            players[i].setComunInput();
    }
}

void Moderator::calculateScore() {
    int i;
    float indvScore;

    for (i = 0; i < NUMBER_OF_LIGHT_ASSAULT_TRAIN; i++) {
        indvScore = lightAssaults[i].getScore();
        // lightAssaultScore += indvScore;

        if (indvScore > bestLightAssault->score) {
            bestLightAssault->score = indvScore;
            bestLightAssault->player = &lightAssaults[i];
            bestLightAssault->index = i;
        }
    }

    for (i = 0; i < NUMBER_OF_SNIPER_TRAIN; i++) {
        indvScore = snipers[i].getScore();
        // sniperScore += indvScore;

        if (indvScore > bestSniper->score) {
            bestSniper->score = indvScore;
            bestSniper->player = &snipers[i];
            bestSniper->index = i;
        }
    }

    for (i = 0; i < NUMBER_OF_ASSAULT_TRAIN; i++) {
        indvScore = assaults[i].getScore();
        // assaultScore += indvScore;

        if (indvScore > bestAssault->score) {
            bestAssault->score = indvScore;
            bestAssault->player = &assaults[i];
            bestAssault->index = i;
        }
    }
}

void Moderator::resetAllPlayers(bool resetScore) {
    screen->resetImage();
    screen->createObstacle();

    if (resetScore) {
        bestLightAssault->score = INICIAL_SCORE;
        bestSniper->score = INICIAL_SCORE;
        bestAssault->score = INICIAL_SCORE;
    }

    lightAssaultScore = 0;
    sniperScore = 0;
    assaultScore = 0;

    resetPlayers(lightAssaults, NUMBER_OF_LIGHT_ASSAULT_TRAIN, LIGHT_ASSAULT_HEALTH, resetScore);
    resetPlayers(snipers, NUMBER_OF_SNIPER_TRAIN, SNIPER_HEALTH, resetScore);
    resetPlayers(assaults, NUMBER_OF_ASSAULT_TRAIN, ASSAULT_HEALTH, resetScore);
}

void Moderator::resetPlayers(Player *players, int NUMBER_OF_PLAYERS, int life, bool resetScore) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
        players[i].reset(life, resetScore);
}

void Moderator::setAllWeightsMod(ANN *lightAssaltMatrix, ANN *sniperMatrix, ANN *assaultMatrix) {
    if (lightAssaltMatrix != nullptr)
        this->lightAssaultANN->copyWheights(lightAssaltMatrix->getMatrixPtr());
    if (sniperMatrix != nullptr)
        this->sniperANN->copyWheights(sniperMatrix->getMatrixPtr());
    if (assaultMatrix != nullptr)
        this->assaultANN->copyWheights(assaultMatrix->getMatrixPtr());
}

void Moderator::game() {
    for (turn = 0; turn < DURATION; turn++) {
        drawAllPlayers();
        updateAllPlayersVision();

        multiplyAllPlayers();

        conflictsAllPlayers();
        if (!checkAllPlayersLife())
            break;

        moveAllPlayers();
    }
    calculateScore();
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
    calculateScore();
}

float Moderator::getScore(int id) {
    if (id == LIGHT_ASSAULT)
        return lightAssaultScore;

    if (id == SNIPER)
        return sniperScore;

    return assaultScore;
}

void Moderator::setScore(float score, int id) {
    if (id == LIGHT_ASSAULT)
        lightAssaultScore = score;

    if (id == SNIPER)
        sniperScore = score;

    assaultScore = score;
}

MatrixXf *Moderator::getMatrixPtr(int id) {
    if (id == LIGHT_ASSAULT)
        return lightAssaultANN->getMatrixPtr();
    if (id == SNIPER)
        return sniperANN->getMatrixPtr();

    return assaultANN->getMatrixPtr();
}

ANN *Moderator::getANN(int id) {
    if (id == LIGHT_ASSAULT)
        return lightAssaultANN;
    if (id == SNIPER)
        return sniperANN;

    return assaultANN;
}

MatrixXf *Moderator::setMatrix(MatrixXf *matrixs, int id) {
    if (id == LIGHT_ASSAULT)
        return lightAssaultANN->setMatrix(matrixs);

    if (id == SNIPER)
        return sniperANN->setMatrix(matrixs);

    return assaultANN->setMatrix(matrixs);
}