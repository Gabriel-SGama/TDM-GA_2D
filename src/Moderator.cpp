#include <iostream>
#include <vector>

#include "headers/Moderator.h"
#include "headers/ANN.h"
#include "headers/Evolution.h"

Moderator::Moderator()
{
    inocentsScore = 0;
    traitorScore = 0;

    //allocs all memory
    inocents = new Inocent[NUMBER_OF_INOCENTS];
    traitors = new Traitor[NUMBER_OF_TRAITORS];
    detectives = new Detective[NUMBER_OF_DETECTIVES];

    bestInocent = new dataOfBestPlayers_t;
    bestTraitor = new dataOfBestPlayers_t;
    bestDetective = new dataOfBestPlayers_t;
}

Moderator::~Moderator()
{
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

    setPlayersValues(playerNumber, inocents, NUMBER_OF_INOCENTS);
    setPlayersValues(playerNumber, traitors, NUMBER_OF_TRAITORS);
    setPlayersValues(playerNumber, detectives, NUMBER_OF_DETECTIVES);
}

void Moderator::setPlayersValues(int &playerNumber, Player *players, int NUMBER_OF_PLAYERS)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++, playerNumber++)
        players[i].setPlayerValues(screen, playerNumber, INOCENT_HEALTH);
}

void Moderator::drawAllPlayers()
{
    screen->resetImage();
    screen->createObstacle();

    drawPlayers(inocents, NUMBER_OF_INOCENTS);
    drawPlayers(traitors, NUMBER_OF_TRAITORS);
    drawPlayers(detectives, NUMBER_OF_DETECTIVES);
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
    updatePlayersVision(inocents, NUMBER_OF_INOCENTS);
    updatePlayersVision(traitors, NUMBER_OF_TRAITORS);
    updatePlayersVision(detectives, NUMBER_OF_DETECTIVES);
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
    conflictsPlayers(inocents, NUMBER_OF_INOCENTS);
    conflictsPlayers(traitors, NUMBER_OF_TRAITORS);
    conflictsPlayers(detectives, NUMBER_OF_DETECTIVES);
}

void Moderator::conflictsPlayers(Player *players, int NUMBER_OF_PLAYERS)
{
    enemyInfo_t enemyInfo;

    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        if (players[i].isAlive())
        {
            enemyInfo = players[i].killPlayer((int)players[i].output[0][INDEX_SHOT]);

            //if(players->getPlayerType() == INOCENT || players->getPlayerType()==DETECTIVE)
            if (enemyInfo.playerType == NOTHING || enemyInfo.playerType == OBSTACLE)
                continue;

            shotPlayer(&players[i], enemyInfo);
        }
    }
}

void Moderator::shotPlayer(Player *shooter, enemyInfo_t enemyInfo)
{
    //find the player that was shot
    if (enemyInfo.playerType == INOCENT && findPlayer(shooter, inocents, NUMBER_OF_INOCENTS, enemyInfo.posiAprox))
    {
        if (shooter->getPlayerType() == TRAITOR)
            shooter->updateScore(1.25 * shooter->getDamage() / INOCENT_DAMAGE);
        else
            shooter->updateScore(-1.5 * shooter->getDamage() / INOCENT_DAMAGE);
    }

    else if ((enemyInfo.playerType == TRAITOR || enemyInfo.playerType == INOCENT) && findPlayer(shooter, traitors, NUMBER_OF_TRAITORS, enemyInfo.posiAprox))
    {
        if (shooter->getPlayerType() == TRAITOR)
            shooter->updateScore(-1.5 * shooter->getDamage() / INOCENT_DAMAGE);
        else
            shooter->updateScore(1.5 * shooter->getDamage() / INOCENT_DAMAGE);
    }
    else if (enemyInfo.playerType == DETECTIVE && findPlayer(shooter, detectives, NUMBER_OF_DETECTIVES, enemyInfo.posiAprox))
    {
        if (shooter->getPlayerType() == TRAITOR)
            shooter->updateScore(1.75 * shooter->getDamage() / INOCENT_DAMAGE);
        else
            shooter->updateScore(-1.5 * shooter->getDamage() / INOCENT_DAMAGE);
    }
}

int Moderator::findPlayer(Player *shooter, Player *players, int NUMBER_OF_PLAYERS, cv::Point enemyPoint)
{
    float distance;

    //find the player that was shot
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        if (!players[i].isAlive())
            continue;

        distance = cv::norm(players[i].getCenter() - enemyPoint);

        if (distance <= RADIUS)
        {
            //std::cout << "distance:" << distance << std::endl;
            //std::cout << "center: " << shooter->getCenter() << std::endl;
            //std::cout << "enemy: " << enemyPoint << std::endl;
            players[i].takeDamage(shooter->getDamage());

            if (shooter->getPlayerType() == TRAITOR && players[i].getPlayerType() == TRAITOR)
                players[i].updateScore(3);

            if (shooter->getPlayerType() != TRAITOR && players[i].getPlayerType() != TRAITOR)
                players[i].updateScore(3);
            //std::cout << shooter->getPlayerID() << "matou " << players[i].getPlayerID() << std::endl;
            //std::cout << "vida: " << players[i].getLife() << std::endl;
            //cv::waitKey(0);

            return 1;
        }
    }
    return 0;
}

void Moderator::checkAllPlayersLife()
{
    checkPlayersLife(inocents, NUMBER_OF_INOCENTS);
    checkPlayersLife(traitors, NUMBER_OF_TRAITORS);
    checkPlayersLife(detectives, NUMBER_OF_DETECTIVES);
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
    movePlayers(inocents, NUMBER_OF_INOCENTS);
    movePlayers(traitors, NUMBER_OF_TRAITORS);
    movePlayers(detectives, NUMBER_OF_DETECTIVES);
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
    multiplyPlayers(inocents, NUMBER_OF_INOCENTS);
    multiplyPlayers(traitors, NUMBER_OF_TRAITORS);
    multiplyPlayers(detectives, NUMBER_OF_DETECTIVES);
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
    definePlayersInput(inocents, NUMBER_OF_INOCENTS);
    definePlayersInput(traitors, NUMBER_OF_TRAITORS);
    definePlayersInput(detectives, NUMBER_OF_DETECTIVES);
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

    for (i = 0; i < NUMBER_OF_INOCENTS; i++)
    {
        indvScore = inocents[i].getScore();
        inocentsScore += indvScore;

        if (inocents[i].isAlive())
            inocentsScore++;
        else
            traitorScore += 2;

        if (indvScore > bestInocent->score)
        {
            bestInocent->score = indvScore;
            bestInocent->player = &inocents[i];
            bestInocent->index = i;
        }
    }

    for (i = 0; i < NUMBER_OF_TRAITORS; i++)
    {
        indvScore = traitors[i].getScore();
        traitorScore += indvScore;

        if (traitors[i].isAlive())
            traitorScore++;
        else
            inocentsScore += 3;

        if (indvScore > bestTraitor->score)
        {
            bestTraitor->score = indvScore;
            bestTraitor->player = &traitors[i];
            bestTraitor->index = i;
        }
    }

    for (i = 0; i < NUMBER_OF_DETECTIVES; i++)
    {
        indvScore = detectives[i].getScore();
        inocentsScore += indvScore;

        if (detectives[i].isAlive())
            inocentsScore++;
        else
            traitorScore += 4;

        if (indvScore > bestDetective->score)
        {
            bestDetective->score = indvScore;
            bestDetective->player = &detectives[i];
            bestDetective->index = i;
        }
    }

    //std::cout << "inocent Score: " << inocentsScore << std::endl;
    //std::cout << "traitor Score: " << traitorScore << std::endl;
}

void Moderator::resetAllPlayers(bool resetScore)
{
    screen->resetImage();
    screen->createObstacle();

    if (resetScore)
    {
        bestInocent->score = -100;
        bestTraitor->score = -100;
        bestDetective->score = -100;
    }

    inocentsScore = 0;
    traitorScore = 0;

    resetPlayers(inocents, NUMBER_OF_INOCENTS, INOCENT_HEALTH, resetScore);
    resetPlayers(traitors, NUMBER_OF_TRAITORS, TRAITOR_HEALTH, resetScore);
    resetPlayers(detectives, NUMBER_OF_DETECTIVES, DETECTIVE_HEALTH, resetScore);
}

void Moderator::resetPlayers(Player *players, int NUMBER_OF_PLAYERS, int life, bool resetScore)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
        players[i].reset(life, resetScore);
}

void Moderator::setAllWeights(Inocent *bestInocents, Traitor *bestTraitors, Detective *bestDetectives)
{
    if (bestInocents != nullptr)
        setWeights(bestInocents, inocents, NUMBER_OF_INOCENTS);

    if (bestTraitors != nullptr)
        setWeights(bestTraitors, traitors, NUMBER_OF_TRAITORS);

    if (bestDetectives != nullptr)
        setWeights(bestDetectives, detectives, NUMBER_OF_DETECTIVES);
}

void Moderator::setWeights(Player *bestPlayers, Player *players, int NUMBER_OF_PLAYERS)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        //std::cout << players[i].ann->getMatrixPtr() << std::endl;
        //delete[] players[i].ann->getMatrixPtr();
        players[i].ann->setMatrix(bestPlayers[i].ann->getMatrixPtr());
        //std::cout << players[i].ann->getMatrixPtr()[2] << std::endl;
    }
}

void Moderator::copyAllWeights(Inocent *bestInocents, Traitor *bestTraitors, Detective *bestDetectives)
{
    if (bestInocents != nullptr)
        copyWeights(bestInocents, inocents, NUMBER_OF_INOCENTS);

    if (bestTraitors != nullptr)
        copyWeights(bestTraitors, traitors, NUMBER_OF_TRAITORS);

    if (bestDetectives != nullptr)
        copyWeights(bestDetectives, detectives, NUMBER_OF_DETECTIVES);
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