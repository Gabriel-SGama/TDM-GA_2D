#include <iostream>
#include <vector>

#include "headers/Moderator.h"

Moderator::Moderator() {
    //----------------INICIAL VALUES----------------
    turn = 0;
    lightAssaultScore = 0;
    sniperScore = 0;
    assaultScore = 0;

    lightAssaultScoreSum = 0;
    sniperScoreSum = 0;
    assaultScoreSum = 0;

    //----------------ALLOCS ALL PLAYERS MEMORY----------------
    lightAssaults = new LightAssault[NUMBER_OF_PLAYERS];
    snipers = new Sniper[NUMBER_OF_PLAYERS];
    assaults = new Assault[NUMBER_OF_PLAYERS];

    bestLightAssault = new dataOfBestPlayers_t;
    bestSniper = new dataOfBestPlayers_t;
    bestAssault = new dataOfBestPlayers_t;

    //----------------PLAYER INICIAL VALUES----------------
    bestLightAssault->player = lightAssaults;
    bestSniper->player = snipers;
    bestAssault->player = assaults;
    bestLightAssault->score = INICIAL_SCORE;
    bestSniper->score = INICIAL_SCORE;
    bestAssault->score = INICIAL_SCORE;

    //----------------PLAYERS CENTER----------------
    playersCenter = new cv::Point *[NUMBER_OF_TOTAL_PLAYERS];

    LACenter = new cv::Point *[NUMBER_OF_PLAYERS];
    SCenter = new cv::Point *[NUMBER_OF_PLAYERS];
    ACenter = new cv::Point *[NUMBER_OF_PLAYERS];

    setPlayerCenterPtr(lightAssaults, LACenter);
    setPlayerCenterPtr(snipers, SCenter);
    setPlayerCenterPtr(assaults, ACenter);

}

Moderator::~Moderator() {
}

void Moderator::setInicialPosAll(const cv::Point *inicialPos, int start) {
    setInicialPos(lightAssaults, inicialPos[start]);
    start++;
    setInicialPos(snipers, inicialPos[start % 3]);
    start++;
    setInicialPos(assaults, inicialPos[start % 3]);
}

void Moderator::setInicialPos(Player *players, cv::Point initialPos) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        players[i].initialPos = initialPos;
    }
}

void Moderator::setPlayerCenterPtr(Player *players, cv::Point **centerPtr) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
        centerPtr[i] = players[i].getCenterPtr();
}

void Moderator::setPlayerCenterPtr(Player *players, int offset) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
        playersCenter[i + offset] = players[i].getCenterPtr();
}

void Moderator::setScreen(Screen *screen) {
    this->screen = screen;

    screen->resetImage();
    screen->createObstacle();
}

void Moderator::setAllPlayersValues() {
    int playerNumber = 0;

    setPlayersValues(playerNumber, lightAssaults, LACenter);
    setPlayersValues(playerNumber, snipers, SCenter);
    setPlayersValues(playerNumber, assaults, ACenter);

}

void Moderator::setPlayersValues(int &playerNumber, Player *players, cv::Point **centerPtr) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++, playerNumber++)
        players[i].setPlayerValues(screen, playerNumber, centerPtr);
}

void Moderator::drawAllPlayers() {
    screen->resetImage();

    screen->createObstacle();

    drawPlayers(lightAssaults);
    drawPlayers(snipers);
    drawPlayers(assaults);
}

void Moderator::drawPlayers(Player *players) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive())
            players[i].drawPlayer();
    }
}

void Moderator::updateAllPlayersVision() {
    updatePlayersVision(lightAssaults);
    updatePlayersVision(snipers);
    updatePlayersVision(assaults);
}

void Moderator::updatePlayersVision(Player *players) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive())
            players[i].updateVision();
    }
}

void Moderator::conflictsAllPlayers() {
    conflictsPlayers(lightAssaults);
    conflictsPlayers(snipers);
    conflictsPlayers(assaults);
}

void Moderator::conflictsPlayers(Player *players) {
    enemyInfo_t enemyInfo;

    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive() && players[i].output[0][INDEX_SHOT] > 0.5f) {
            enemyInfo = players[i].killPlayer();
            if (enemyInfo.playerType == NOTHING || enemyInfo.playerType == OBSTACLE)
                continue;

            shotPlayer(&players[i], enemyInfo);
        }
    }
}

void Moderator::shotPlayer(Player *shooter, enemyInfo_t enemyInfo) {
    //----------------LIGHT ASSAULT----------------
    if (enemyInfo.playerType == LIGHT_ASSAULT && findPlayer(shooter, lightAssaults, enemyInfo.posiAprox)) {
        if (shooter->getPlayerType() != LIGHT_ASSAULT)
            shooter->updateScore(shooter->getDamage());
        else
            shooter->updateScore(-2.5 * shooter->getDamage());

        //----------------SNIPER----------------
    } else if (enemyInfo.playerType == SNIPER && findPlayer(shooter, snipers, enemyInfo.posiAprox)) {
        if (shooter->getPlayerType() != SNIPER)
            shooter->updateScore(shooter->getDamage());
        else
            shooter->updateScore(-2.5 * shooter->getDamage());

        //----------------ASSAULT----------------
    } else if (enemyInfo.playerType == ASSAULT && findPlayer(shooter, assaults, enemyInfo.posiAprox)) {
        if (shooter->getPlayerType() != ASSAULT)
            shooter->updateScore(shooter->getDamage());
        else
            shooter->updateScore(-2.5 * shooter->getDamage());
    }
}

int Moderator::findPlayer(Player *shooter, Player *players, cv::Point enemyPoint) {
    float distance;

    //----------------CHECK ALL PLAYERS----------------
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (!players[i].isAlive() || players[i].getPlayerID() == shooter[i].getPlayerID())
            continue;

        distance = cv::norm(players[i].getCenter() - enemyPoint);
        
        //----------------FOUND PLAYER----------------
        if (distance <= RADIUS + 1) {
            players[i].takeDamage(shooter->getDamage());

            if (players[i].getLife() <= 0) {
                //friend fire doesnt count to score
                if (shooter->getPlayerType() == players[i].getPlayerType())
                    players[i].updateScore(12);
                else if (shooter->getPlayerType() == LIGHT_ASSAULT)
                    lightAssaultScore += 1;
                else if (shooter->getPlayerType() == SNIPER)
                    sniperScore += 1;
                else
                    assaultScore += 1;
            }
            return 1;
        }
    }

    return 0;
}

void Moderator::checkAllPlayersLife() {
    checkPlayersLife(lightAssaults);
    checkPlayersLife(snipers);
    checkPlayersLife(assaults);
}

void Moderator::checkPlayersLife(Player *players) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].getLife() <= 0 && players[i].isAlive())
            players[i].setAlive(false);
    }
}

void Moderator::moveAllPlayers() {
    movePlayers(lightAssaults);
    movePlayers(snipers);
    movePlayers(assaults);
}

void Moderator::movePlayers(Player *players) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive())
            players[i].move();
    }
}

void Moderator::multiplyAllPlayers() {
    multiplyPlayers(lightAssaults);
    multiplyPlayers(snipers);
    multiplyPlayers(assaults);
}

void Moderator::multiplyPlayers(Player *players) {
    #pragma omp parallel for
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive())
            players[i].ann->multiply();
    }
}

void Moderator::defineAllPlayersInput() {
    definePlayersInput(lightAssaults);
    definePlayersInput(snipers);
    definePlayersInput(assaults);
}

void Moderator::definePlayersInput(Player *players) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive())
            players[i].setComunInput();
    }
}

void Moderator::calculateScore() {
    int i;
    float indvScore;

    //----------------LIGHT ASSAULT SCORE----------------
    for (i = 0; i < NUMBER_OF_PLAYERS; i++) {
        indvScore = lightAssaults[i].getScore();
        lightAssaultScoreSum += indvScore;

        if (indvScore > bestLightAssault->score) {
            bestLightAssault->score = indvScore;
            bestLightAssault->player = &lightAssaults[i];
            bestLightAssault->index = i;
        }
    }

    //----------------SNIPER SCORE----------------
    for (i = 0; i < NUMBER_OF_PLAYERS; i++) {
        indvScore = snipers[i].getScore();
        sniperScoreSum += indvScore;

        if (indvScore > bestSniper->score) {
            bestSniper->score = indvScore;
            bestSniper->player = &snipers[i];
            bestSniper->index = i;
        }
    }

    //----------------ASSAULT SCORE----------------
    for (i = 0; i < NUMBER_OF_PLAYERS; i++) {
        indvScore = assaults[i].getScore();
        assaultScoreSum += indvScore;

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

    //----------------RESET SCORE----------------

    bestLightAssault->score = INICIAL_SCORE;
    bestSniper->score = INICIAL_SCORE;
    bestAssault->score = INICIAL_SCORE;
    
    if (resetScore) {
        lightAssaultScore = 0;
        sniperScore = 0;
        assaultScore = 0;
        
        lightAssaultScoreSum = 0;
        sniperScoreSum = 0;
        assaultScoreSum = 0;
    }

    //----------------RESET PLAYERS----------------
    resetPlayers(lightAssaults, LIGHT_ASSAULT_HEALTH, resetScore);
    resetPlayers(snipers, SNIPER_HEALTH, resetScore);
    resetPlayers(assaults, ASSAULT_HEALTH, resetScore);
}

void Moderator::resetPlayers(Player *players, int life, bool resetScore) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
        players[i].reset(life, resetScore);
}

void Moderator::setAllWeights(LightAssault *bestLightAssaults, Sniper *bestSnipers, Assault *bestAssaults) {
    if (bestLightAssaults != nullptr)
        setWeights(bestLightAssaults, lightAssaults);

    if (bestSnipers != nullptr)
        setWeights(bestSnipers, snipers);

    if (bestAssaults != nullptr)
        setWeights(bestAssaults, assaults);
}

void Moderator::setWeights(Player *bestPlayers, Player *players) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++){
        players[i].ann->setBias(bestPlayers[i].ann->getBiasPtr());
        players[i].ann->setMatrix(bestPlayers[i].ann->getMatrixPtr());
    }
}

void Moderator::copyAllWeights(LightAssault *bestLightAssaults, Sniper *bestSnipers, Assault *bestAssaults) {
    if (bestLightAssaults != nullptr)
        copyWeights(bestLightAssaults, lightAssaults);

    if (bestSnipers != nullptr)
        copyWeights(bestSnipers, snipers);

    if (bestAssaults != nullptr)
        copyWeights(bestAssaults, assaults);
}

void Moderator::copyWeights(Player *bestPlayers, Player *players) {
    int j;

    vectorF *newBiasArray;
    vectorF *biasArray;

    MatrixF *newMatrixArray;
    MatrixF *matrixArray;

    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        newBiasArray = players[i].ann->getBiasPtr();
        biasArray = bestPlayers[i].ann->getBiasPtr();

        newMatrixArray = players[i].ann->getMatrixPtr();
        matrixArray = bestPlayers[i].ann->getMatrixPtr();

        for (j = 0; j < layerSize + 1; j++){
            newBiasArray[j] = biasArray[j];
            newMatrixArray[j] = matrixArray[j];
        }
    }
}

void Moderator::setAllWeightsOneMatrix(ANN *lightAssaultANN, ANN *sniperANN, ANN *assaultANN) {
    int i;

    for (i = 0; i < NUMBER_OF_PLAYERS; i++){
        lightAssaults[i].ann->setBias(lightAssaultANN->getBiasPtr());
        lightAssaults[i].ann->setMatrix(lightAssaultANN->getMatrixPtr());
    }
            
    for (i = 0; i < NUMBER_OF_PLAYERS; i++){
        snipers[i].ann->setBias(sniperANN->getBiasPtr());
        snipers[i].ann->setMatrix(sniperANN->getMatrixPtr());
    }

    for (i = 0; i < NUMBER_OF_PLAYERS; i++){
        assaults[i].ann->setBias(assaultANN->getBiasPtr());
        assaults[i].ann->setMatrix(assaultANN->getMatrixPtr());
    }
}

void Moderator::game() {
    for (turn = 0; turn < DURATION; turn++) {
        drawAllPlayers();
        updateAllPlayersVision();

        defineAllPlayersInput();
        multiplyAllPlayers();

        conflictsAllPlayers();
        checkAllPlayersLife();

        moveAllPlayers();
    }
    calculateScore();
}

void Moderator::gameOfBest() {
    for (int i = 0; i < DURATION; i++) {
        drawAllPlayers();
        updateAllPlayersVision();

        defineAllPlayersInput();
        multiplyAllPlayers();

        conflictsAllPlayers();
        checkAllPlayersLife();

        moveAllPlayers();
        screen->updateMap();
    }
    calculateScore();
}