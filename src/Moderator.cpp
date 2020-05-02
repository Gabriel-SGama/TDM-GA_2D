#include <iostream>
#include <vector>

#include "headers/Moderator.h"
#include "headers/ANN.h"
#include "headers/Evolution.h"

Moderator::Moderator()
{
}

Moderator::~Moderator()
{
}

void Moderator::setModerator(int NUMBER_OF_LIGHT_ASSAULT_TRAIN, int NUMBER_OF_SNIPER_TRAIN, int NUMBER_OF_ASSAULT_TRAIN)
{
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

    playersCenter = new cv::Point *[NUMBER_OF_TOTAL_PLAYERS];

    setPlayerCenterPtr(lightAssaults, NUMBER_OF_LIGHT_ASSAULT_TRAIN, 0);
    setPlayerCenterPtr(snipers, NUMBER_OF_SNIPER_TRAIN, NUMBER_OF_LIGHT_ASSAULT_TRAIN);
    setPlayerCenterPtr(assaults, NUMBER_OF_ASSAULT_TRAIN, NUMBER_OF_SNIPER_TRAIN + NUMBER_OF_LIGHT_ASSAULT_TRAIN);
}

void Moderator::setPlayerCenterPtr(Player *players, int NUMBER_OF_PLAYERS, int offset)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        playersCenter[i + offset] = players[i].getCenterPtr();
    }
}

void Moderator::setScreen(Screen *screen)
{
    this->screen = screen;

    screen->resetImage();
    screen->createObstacle();
}

void Moderator::setAllPlayersValues()
{
    int playerNumber = 0;

    setPlayersValues(playerNumber, lightAssaults, NUMBER_OF_LIGHT_ASSAULT_TRAIN);
    setPlayersValues(playerNumber, snipers, NUMBER_OF_SNIPER_TRAIN);
    setPlayersValues(playerNumber, assaults, NUMBER_OF_ASSAULT_TRAIN);
}

void Moderator::setPlayersValues(int &playerNumber, Player *players, int NUMBER_OF_PLAYERS)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++, playerNumber++)
        players[i].setPlayerValues(screen, playerNumber, LIGHT_ASSAULT_HEALTH, playersCenter);
}

void Moderator::drawAllPlayers()
{
    screen->resetImage();
    screen->createObstacle();

    drawPlayers(lightAssaults, NUMBER_OF_LIGHT_ASSAULT_TRAIN);
    drawPlayers(snipers, NUMBER_OF_SNIPER_TRAIN);
    drawPlayers(assaults, NUMBER_OF_ASSAULT_TRAIN);
}

void Moderator::drawPlayers(Player *players, int NUMBER_OF_PLAYERS)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        if (players[i].isAlive())
            players[i].drawPlayer();
    }
}

void Moderator::updateAllPlayersVision()
{
    updatePlayersVision(lightAssaults, NUMBER_OF_LIGHT_ASSAULT_TRAIN);
    updatePlayersVision(snipers, NUMBER_OF_SNIPER_TRAIN);
    updatePlayersVision(assaults, NUMBER_OF_ASSAULT_TRAIN);
}

void Moderator::updatePlayersVision(Player *players, int NUMBER_OF_PLAYERS)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        if (players[i].isAlive())
            players[i].updateVision();
    }
}

void Moderator::conflictsAllPlayers()
{
    conflictsPlayers(lightAssaults, NUMBER_OF_LIGHT_ASSAULT_TRAIN, lightAssaults->numberOfRays);
    conflictsPlayers(snipers, NUMBER_OF_SNIPER_TRAIN, snipers->numberOfRays);
    conflictsPlayers(assaults, NUMBER_OF_ASSAULT_TRAIN, assaults->numberOfRays);
}

void Moderator::conflictsPlayers(Player *players, int NUMBER_OF_PLAYERS, int numberOfRays)
{
    enemyInfo_t enemyInfo;

    int maxIndex = 0;
    int k;
    int max = -1;

    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        if (players[i].isAlive())
        {

            for (k = INDEX_SHOT; k < numberOfRays + INDEX_SHOT; k++)
            {
                if (players[i].output[0][k] > max)
                {
                    max = players[i].output[0][k];
                    maxIndex = k;
                }
            }

            if (max > 0)
                enemyInfo = players[i].killPlayer((int)players[i].output[0][maxIndex]);
            else
                continue;

            if (enemyInfo.playerType == NOTHING || enemyInfo.playerType == OBSTACLE)
                continue;

            shotPlayer(&players[i], enemyInfo);
        }
    }
}

void Moderator::shotPlayer(Player *shooter, enemyInfo_t enemyInfo)
{
    //find the player that was shot
    if (enemyInfo.playerType == LIGHT_ASSAULT && findPlayer(shooter, lightAssaults, NUMBER_OF_LIGHT_ASSAULT_TRAIN, enemyInfo.posiAprox))
    {
        if (shooter->getPlayerType() != LIGHT_ASSAULT)
            shooter->updateScore(LIGHT_ASSAULT_SHOT_REWARD);
        else
            shooter->updateScore(-LIGHT_ASSAULT_SHOT_REWARD);
    }

    else if (enemyInfo.playerType == SNIPER && findPlayer(shooter, snipers, NUMBER_OF_SNIPER_TRAIN, enemyInfo.posiAprox))
    {
        if (shooter->getPlayerType() != SNIPER)
            shooter->updateScore(SNIPER_SHOT_REWARD);
        else
            shooter->updateScore(-SNIPER_SHOT_REWARD);
    }
    else if (enemyInfo.playerType == ASSAULT && findPlayer(shooter, assaults, NUMBER_OF_ASSAULT_TRAIN, enemyInfo.posiAprox))
    {
        if (shooter->getPlayerType() != ASSAULT)
            shooter->updateScore(ASSAULT_SHOT_REWARD);
        else
            shooter->updateScore(-ASSAULT_SHOT_REWARD);
    }
}

int Moderator::findPlayer(Player *shooter, Player *players, int NUMBER_OF_PLAYERS, cv::Point enemyPoint)
{
    float distance;

    //find the player that was shot
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        if (!players[i].isAlive() || players[i].getPlayerID() == shooter[i].getPlayerID())
            continue;

        distance = cv::norm(players[i].getCenter() - enemyPoint);

        if (distance <= RADIUS)
        {
            players[i].takeDamage(shooter->getDamage());
            //friend fire doesnt count to score
            if (players[i].getLife() <= 0)
            {
                if (shooter->getPlayerType() == players[i].getPlayerType())
                    players[i].updateScore(3);
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

void Moderator::checkAllPlayersLife()
{
    checkPlayersLife(lightAssaults, NUMBER_OF_LIGHT_ASSAULT_TRAIN);
    checkPlayersLife(snipers, NUMBER_OF_SNIPER_TRAIN);
    checkPlayersLife(assaults, NUMBER_OF_ASSAULT_TRAIN);
}

void Moderator::checkPlayersLife(Player *players, int NUMBER_OF_PLAYERS)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        if (players[i].getLife() <= 0)
            players[i].setAlive(false);
    }
}

void Moderator::moveAllPlayers()
{
    movePlayers(lightAssaults, NUMBER_OF_LIGHT_ASSAULT_TRAIN);
    movePlayers(snipers, NUMBER_OF_SNIPER_TRAIN);
    movePlayers(assaults, NUMBER_OF_ASSAULT_TRAIN);
}

void Moderator::movePlayers(Player *players, int NUMBER_OF_PLAYERS)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        if (players[i].isAlive())
            players[i].move();
    }
}

void Moderator::multiplyAllPlayers()
{
    multiplyPlayers(lightAssaults, NUMBER_OF_LIGHT_ASSAULT_TRAIN);
    multiplyPlayers(snipers, NUMBER_OF_SNIPER_TRAIN);
    multiplyPlayers(assaults, NUMBER_OF_ASSAULT_TRAIN);
}

void Moderator::multiplyPlayers(Player *players, int NUMBER_OF_PLAYERS)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        if (players[i].isAlive())
            players[i].ann->multiply();
    }
}

void Moderator::defineAllPlayersInput()
{
    definePlayersInput(lightAssaults, NUMBER_OF_LIGHT_ASSAULT_TRAIN);
    definePlayersInput(snipers, NUMBER_OF_SNIPER_TRAIN);
    definePlayersInput(assaults, NUMBER_OF_ASSAULT_TRAIN);
}

void Moderator::definePlayersInput(Player *players, int NUMBER_OF_PLAYERS)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        if (players[i].isAlive())
            players[i].setComunInput();
    }
}

void Moderator::calculateScore()
{
    int i;
    float indvScore;

    for (i = 0; i < NUMBER_OF_LIGHT_ASSAULT_TRAIN; i++)
    {
        indvScore = lightAssaults[i].getScore();

        if (indvScore > bestLightAssault->score)
        {
            bestLightAssault->score = indvScore;
            bestLightAssault->player = &lightAssaults[i];
            bestLightAssault->index = i;
        }
    }

    for (i = 0; i < NUMBER_OF_SNIPER_TRAIN; i++)
    {
        indvScore = snipers[i].getScore();

        if (indvScore > bestSniper->score)
        {
            bestSniper->score = indvScore;
            bestSniper->player = &snipers[i];
            bestSniper->index = i;
        }
    }

    for (i = 0; i < NUMBER_OF_ASSAULT_TRAIN; i++)
    {
        indvScore = assaults[i].getScore();

        if (indvScore > bestAssault->score)
        {
            bestAssault->score = indvScore;
            bestAssault->player = &assaults[i];
            bestAssault->index = i;
        }
    }
}

void Moderator::resetAllPlayers(bool resetScore)
{
    screen->resetImage();
    screen->createObstacle();

    if (resetScore)
    {
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

void Moderator::resetPlayers(Player *players, int NUMBER_OF_PLAYERS, int life, bool resetScore)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
        players[i].reset(life, resetScore);
}

/*
Assault *Moderator::getAssaults()
{
    int i;
    int j;

    Assault *best;

    float bestScore;

    //put the top 'NUMBER_OF_ASSAULTS'(2) in the start
    //faster for small numbers of assaults
    for (i = 0; i < NUMBER_OF_ASSAULTS; i++)
    {
        best = &assaults[i];
        bestScore = best->getScore();

        for (j = i + 1; j < NUMBER_OF_ASSAULT_TRAIN; j++)
        {
            if (assaults[j].getScore() > bestScore)
            {
                best = &assaults[j];
                bestScore = best->getScore();
            }
        }

        //changes matrix ptr
        best->ann->setMatrix(assaults[i].ann->setMatrix(best->ann->getMatrixPtr()));
        //changes the best
        best->setScore(INICIAL_SCORE);
    }

    return assaults;
}
*/

void Moderator::setAllWeights(LightAssault *bestLightAssaults, Sniper *bestSnipers, Assault *bestAssaults)
{
    if (bestLightAssaults != nullptr)
        setWeights(bestLightAssaults, lightAssaults, NUMBER_OF_LIGHT_ASSAULT_TRAIN);

    if (bestSnipers != nullptr)
        setWeights(bestSnipers, snipers, NUMBER_OF_SNIPER_TRAIN);

    if (bestAssaults != nullptr)
        setWeights(bestAssaults, assaults, NUMBER_OF_ASSAULT_TRAIN);
}

void Moderator::setWeights(Player *bestPlayers, Player *players, int NUMBER_OF_PLAYERS)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
        players[i].ann->setMatrix(bestPlayers[i].ann->getMatrixPtr());
}

void Moderator::copyAllWeights(LightAssault *bestLightAssaults, Sniper *bestSnipers, Assault *bestAssaults)
{
    if (bestLightAssaults != nullptr)
        copyWeights(bestLightAssaults, lightAssaults, NUMBER_OF_LIGHT_ASSAULT_TRAIN);

    if (bestSnipers != nullptr)
        copyWeights(bestSnipers, snipers, NUMBER_OF_SNIPER_TRAIN);

    if (bestAssaults != nullptr)
        copyWeights(bestAssaults, assaults, NUMBER_OF_ASSAULT_TRAIN);
}

void Moderator::copyWeights(Player *bestPlayers, Player *players, int NUMBER_OF_PLAYERS)
{
    unsigned int j;

    MatrixXf *newMatrixArray;
    MatrixXf *matrixArray;

    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        newMatrixArray = new MatrixXf[layers.size() + 1];
        delete[] players[i].ann->getMatrixPtr();
        matrixArray = bestPlayers[i].ann->getMatrixPtr();
        for (j = 0; j < layers.size() + 1; j++)
        {
            newMatrixArray[j] = matrixArray[j];
        }
        players[i].ann->setMatrix(newMatrixArray);
    }
}

void Moderator::setAllWeightsOneMatrix(MatrixXf *inocentMatrix, MatrixXf *traitorMatrix, MatrixXf *detectiveMatrix)
{
    int i;

    for (i = 0; i < NUMBER_OF_LIGHT_ASSAULT_TRAIN; i++)
    {
        lightAssaults[i].ann->setMatrix(inocentMatrix);
    }

    for (i = 0; i < NUMBER_OF_SNIPER_TRAIN; i++)
    {
        snipers[i].ann->setMatrix(traitorMatrix);
    }
    for (i = 0; i < NUMBER_OF_ASSAULT_TRAIN; i++)
    {
        assaults[i].ann->setMatrix(detectiveMatrix);
    }
}

void Moderator::game()
{
    for (int i = 0; i < DURATION; i++)
    {
        drawAllPlayers();
        updateAllPlayersVision();
        conflictsAllPlayers();
        checkAllPlayersLife();
        defineAllPlayersInput();
        multiplyAllPlayers();
        moveAllPlayers();

    }
    calculateScore();
}

void Moderator::gameOfBest()
{
    for (int i = 0; i < DURATION; i++)
    {
        drawAllPlayers();
        updateAllPlayersVision();
        conflictsAllPlayers();

        screen->updateMap();

        checkAllPlayersLife();
        defineAllPlayersInput();
        multiplyAllPlayers();
        moveAllPlayers();
    }
    calculateScore();
}