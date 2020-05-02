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

void Moderator::setModerator(int NUMBER_OF_INOCENT_TRAIN, int NUMBER_OF_SNIPER_TRAIN, int NUMBER_OF_DETECTIVE_TRAIN)
{
    inocentScore = 0;
    sniperScore = 0;
    detectiveScore = 0;

    this->NUMBER_OF_INOCENT_TRAIN = NUMBER_OF_INOCENT_TRAIN;
    this->NUMBER_OF_SNIPER_TRAIN = NUMBER_OF_SNIPER_TRAIN;
    this->NUMBER_OF_DETECTIVE_TRAIN = NUMBER_OF_DETECTIVE_TRAIN;

    //allocs all memory
    inocents = new Inocent[NUMBER_OF_INOCENT_TRAIN];
    snipers = new Sniper[NUMBER_OF_SNIPER_TRAIN];
    detectives = new Detective[NUMBER_OF_DETECTIVE_TRAIN];

    bestInocent = new dataOfBestPlayers_t;
    bestSniper = new dataOfBestPlayers_t;
    bestDetective = new dataOfBestPlayers_t;

    bestInocent->player = inocents;
    bestSniper->player = snipers;
    bestDetective->player = detectives;
    bestInocent->score = INICIAL_SCORE;
    bestSniper->score = INICIAL_SCORE;
    bestDetective->score = INICIAL_SCORE;

    playersCenter = new cv::Point *[NUMBER_OF_TOTAL_PLAYERS];

    setPlayerCenterPtr(inocents, NUMBER_OF_INOCENT_TRAIN, 0);
    setPlayerCenterPtr(snipers, NUMBER_OF_SNIPER_TRAIN, NUMBER_OF_INOCENT_TRAIN);
    setPlayerCenterPtr(detectives, NUMBER_OF_DETECTIVE_TRAIN, NUMBER_OF_SNIPER_TRAIN + NUMBER_OF_INOCENT_TRAIN);
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

    setPlayersValues(playerNumber, inocents, NUMBER_OF_INOCENT_TRAIN);
    setPlayersValues(playerNumber, snipers, NUMBER_OF_SNIPER_TRAIN);
    setPlayersValues(playerNumber, detectives, NUMBER_OF_DETECTIVE_TRAIN);
}

void Moderator::setPlayersValues(int &playerNumber, Player *players, int NUMBER_OF_PLAYERS)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++, playerNumber++)
        players[i].setPlayerValues(screen, playerNumber, INOCENT_HEALTH, playersCenter);
}

void Moderator::drawAllPlayers()
{
    screen->resetImage();
    screen->createObstacle();

    drawPlayers(inocents, NUMBER_OF_INOCENT_TRAIN);
    drawPlayers(snipers, NUMBER_OF_SNIPER_TRAIN);
    drawPlayers(detectives, NUMBER_OF_DETECTIVE_TRAIN);
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
    updatePlayersVision(inocents, NUMBER_OF_INOCENT_TRAIN);
    updatePlayersVision(snipers, NUMBER_OF_SNIPER_TRAIN);
    updatePlayersVision(detectives, NUMBER_OF_DETECTIVE_TRAIN);
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
    conflictsPlayers(inocents, NUMBER_OF_INOCENT_TRAIN);
    conflictsPlayers(snipers, NUMBER_OF_SNIPER_TRAIN);
    conflictsPlayers(detectives, NUMBER_OF_DETECTIVE_TRAIN);
}

void Moderator::conflictsPlayers(Player *players, int NUMBER_OF_PLAYERS)
{
    enemyInfo_t enemyInfo;

    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        if (players[i].isAlive())
        {
            enemyInfo = players[i].killPlayer((int)players[i].output[0][INDEX_SHOT]);

            if (enemyInfo.playerType == NOTHING || enemyInfo.playerType == OBSTACLE)
                continue;

            shotPlayer(&players[i], enemyInfo);
        }
    }
}

void Moderator::shotPlayer(Player *shooter, enemyInfo_t enemyInfo)
{
    //find the player that was shot
    if (enemyInfo.playerType == LIGHT_ASSAULT && findPlayer(shooter, inocents, NUMBER_OF_INOCENT_TRAIN, enemyInfo.posiAprox))
    {
        if (shooter->getPlayerType() == SNIPER)
        {
            shooter->updateScore(1.75 * shooter->getDamage() / INOCENT_DAMAGE);
            sniperScore += 2;
        }
        else
        {
            shooter->updateScore(-4.0 * shooter->getDamage() / INOCENT_DAMAGE);

            if (shooter->getPlayerType() == LIGHT_ASSAULT)
                inocentScore -= 3;
            else
                detectiveScore -= 3;
        }
    }

    else if ((enemyInfo.playerType == SNIPER || enemyInfo.playerType == LIGHT_ASSAULT) && findPlayer(shooter, snipers, NUMBER_OF_SNIPER_TRAIN, enemyInfo.posiAprox))
    {
        if (shooter->getPlayerType() == SNIPER)
        {
            shooter->updateScore(-4.0 * shooter->getDamage() / INOCENT_DAMAGE);
            sniperScore -= 4;
        }
        else
        {
            shooter->updateScore(2 * shooter->getDamage() / INOCENT_DAMAGE);

            if (shooter->getPlayerType() == LIGHT_ASSAULT)
                inocentScore += 3;
            else
                detectiveScore += 3;
        }
    }
    else if (enemyInfo.playerType == ASSAULT && findPlayer(shooter, detectives, NUMBER_OF_DETECTIVE_TRAIN, enemyInfo.posiAprox))
    {
        if (shooter->getPlayerType() == SNIPER)
        {
            shooter->updateScore(2.5 * shooter->getDamage() / INOCENT_DAMAGE);
            sniperScore += 5;
        }
        else
        {
            shooter->updateScore(-6.0 * shooter->getDamage() / INOCENT_DAMAGE);
            if (shooter->getPlayerType() == LIGHT_ASSAULT)
                inocentScore -= 6;
            else
                detectiveScore -= 6;
        }
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
            if (shooter->getPlayerType() == ASSAULT && players[i].getPlayerType() == ASSAULT && players[i].getLife() <= 0)
                players[i].updateScore(3);

            if (shooter->getPlayerType() != ASSAULT && players[i].getPlayerType() != ASSAULT && players[i].getLife() <= 0)
                players[i].updateScore(3);

            return 1;
        }
    }
    return 0;
}

void Moderator::checkAllPlayersLife()
{
    checkPlayersLife(inocents, NUMBER_OF_INOCENT_TRAIN);
    checkPlayersLife(snipers, NUMBER_OF_SNIPER_TRAIN);
    checkPlayersLife(detectives, NUMBER_OF_DETECTIVE_TRAIN);
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
    movePlayers(inocents, NUMBER_OF_INOCENT_TRAIN);
    movePlayers(snipers, NUMBER_OF_SNIPER_TRAIN);
    movePlayers(detectives, NUMBER_OF_DETECTIVE_TRAIN);
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
    multiplyPlayers(inocents, NUMBER_OF_INOCENT_TRAIN);
    multiplyPlayers(snipers, NUMBER_OF_SNIPER_TRAIN);
    multiplyPlayers(detectives, NUMBER_OF_DETECTIVE_TRAIN);
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
    definePlayersInput(inocents, NUMBER_OF_INOCENT_TRAIN);
    definePlayersInput(snipers, NUMBER_OF_SNIPER_TRAIN);
    definePlayersInput(detectives, NUMBER_OF_DETECTIVE_TRAIN);
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

    for (i = 0; i < NUMBER_OF_INOCENT_TRAIN; i++)
    {
        indvScore = inocents[i].getScore();

        if (indvScore > bestInocent->score)
        {
            bestInocent->score = indvScore;
            bestInocent->player = &inocents[i];
            bestInocent->index = i;
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

    for (i = 0; i < NUMBER_OF_DETECTIVE_TRAIN; i++)
    {
        indvScore = detectives[i].getScore();

        if (indvScore > bestDetective->score)
        {
            bestDetective->score = indvScore;
            bestDetective->player = &detectives[i];
            bestDetective->index = i;
        }
    }
}

void Moderator::resetAllPlayers(bool resetScore)
{
    screen->resetImage();
    screen->createObstacle();

    if (resetScore)
    {
        bestInocent->score = INICIAL_SCORE;
        bestSniper->score = INICIAL_SCORE;
        bestDetective->score = INICIAL_SCORE;
    }

    inocentScore = 0;
    sniperScore = 0;
    detectiveScore = 0;

    resetPlayers(inocents, NUMBER_OF_INOCENT_TRAIN, INOCENT_HEALTH, resetScore);
    resetPlayers(snipers, NUMBER_OF_SNIPER_TRAIN, SNIPER_HEALTH, resetScore);
    resetPlayers(detectives, NUMBER_OF_DETECTIVE_TRAIN, DETECTIVE_HEALTH, resetScore);
}

void Moderator::resetPlayers(Player *players, int NUMBER_OF_PLAYERS, int life, bool resetScore)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
        players[i].reset(life, resetScore);
}

Detective *Moderator::getDetectives()
{
    int i;
    int j;

    Detective *best;

    float bestScore;

    //put the top 'NUMBER_OF_DETECTIVES'(2) in the start
    //faster for small numbers of detectives
    for (i = 0; i < NUMBER_OF_DETECTIVES; i++)
    {
        best = &detectives[i];
        bestScore = best->getScore();

        for (j = i + 1; j < NUMBER_OF_DETECTIVE_TRAIN; j++)
        {
            if (detectives[j].getScore() > bestScore)
            {
                best = &detectives[j];
                bestScore = best->getScore();
            }
        }

        //changes matrix ptr
        best->ann->setMatrix(detectives[i].ann->setMatrix(best->ann->getMatrixPtr()));
        //changes the best
        best->setScore(INICIAL_SCORE);
    }

    return detectives;
}

void Moderator::setAllWeights(Inocent *bestInocents, Sniper *bestSnipers, Detective *bestDetectives)
{
    if (bestInocents != nullptr)
        setWeights(bestInocents, inocents, NUMBER_OF_INOCENT_TRAIN);

    if (bestSnipers != nullptr)
        setWeights(bestSnipers, snipers, NUMBER_OF_SNIPER_TRAIN);

    if (bestDetectives != nullptr)
        setWeights(bestDetectives, detectives, NUMBER_OF_DETECTIVE_TRAIN);
}

void Moderator::setWeights(Player *bestPlayers, Player *players, int NUMBER_OF_PLAYERS)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
        players[i].ann->setMatrix(bestPlayers[i].ann->getMatrixPtr());
}

void Moderator::copyAllWeights(Inocent *bestInocents, Sniper *bestSnipers, Detective *bestDetectives)
{
    if (bestInocents != nullptr)
        copyWeights(bestInocents, inocents, NUMBER_OF_INOCENT_TRAIN);

    if (bestSnipers != nullptr)
        copyWeights(bestSnipers, snipers, NUMBER_OF_SNIPER_TRAIN);

    if (bestDetectives != nullptr)
        copyWeights(bestDetectives, detectives, NUMBER_OF_DETECTIVE_TRAIN);
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

    for (i = 0; i < NUMBER_OF_INOCENT_TRAIN; i++)
    {
        inocents[i].ann->setMatrix(inocentMatrix);
    }

    for (i = 0; i < NUMBER_OF_SNIPER_TRAIN; i++)
    {
        snipers[i].ann->setMatrix(traitorMatrix);
    }
    for (i = 0; i < NUMBER_OF_DETECTIVE_TRAIN; i++)
    {
        detectives[i].ann->setMatrix(detectiveMatrix);
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
        moveAllPlayers();
        defineAllPlayersInput();
        multiplyAllPlayers();
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
        moveAllPlayers();
        defineAllPlayersInput();
        multiplyAllPlayers();
    }
    calculateScore();
}