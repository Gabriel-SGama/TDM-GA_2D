#include <iostream>
#include <vector>

#include "headers/Moderator.h"
#include "headers/ANN.h"

Moderator::Moderator()
{
    inocentsScore = 0;
    traitorScore = 0;

    //allocs all memory
    inocents = new Inocent[NUMBER_OF_INOCENTS];
    traitors = new Traitor[NUMBER_OF_TRAITORS];
    detectives = new Detective[NUMBER_OF_DETECTIVES];
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
            shooter->updateScore(shooter->getDamage() / INOCENT_DAMAGE);
        else
            shooter->updateScore(-shooter->getDamage() / INOCENT_DAMAGE);
    }

    else if ((enemyInfo.playerType == TRAITOR || enemyInfo.playerType == INOCENT) && findPlayer(shooter, traitors, NUMBER_OF_TRAITORS, enemyInfo.posiAprox))
    {
        if (shooter->getPlayerType() == TRAITOR)
            shooter->updateScore(-shooter->getDamage() / INOCENT_DAMAGE);
        else
            shooter->updateScore(shooter->getDamage() / INOCENT_DAMAGE);
    }
    else if (enemyInfo.playerType == DETECTIVE && findPlayer(shooter, detectives, NUMBER_OF_DETECTIVES, enemyInfo.posiAprox))
    {
        if (shooter->getPlayerType() == TRAITOR)
            shooter->updateScore(shooter->getDamage() / INOCENT_DAMAGE);
        else
            shooter->updateScore(-shooter->getDamage() / INOCENT_DAMAGE);
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

        distance = cv::norm(enemyPoint - shooter->getCenter());

        if (distance <= RADIUS + 2)
        {
            std::cout << "distance:" << distance << std::endl;
            std::cout << "center: " << shooter->getCenter() << std::endl;
            std::cout << "enemy: " << enemyPoint << std::endl;

            players[i].takeDamage(shooter->getDamage());
            std::cout << shooter->getPlayerID() << "matou " << players[i].getPlayerID() << std::endl;
            std::cout << "vida: " << players[i].getLife() << std::endl;
            cv::waitKey(0);
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

void Moderator::multiplyAllPlayers()
{
    multiplyPlayers(inocents, NUMBER_OF_INOCENTS);
    multiplyPlayers(traitors, NUMBER_OF_TRAITORS);
    multiplyPlayers(detectives, NUMBER_OF_DETECTIVES);

    screen->resetImage();
    screen->createObstacle();
}

void Moderator::multiplyPlayers(Player *players, int NUMBER_OF_PLAYERS)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        if (players[i].isAlive())
        {
            players[i].ann->multiply();
            players[i].move();
        }
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

    for (i = 0; i < NUMBER_OF_INOCENTS; i++)
    {
        inocentsScore += inocents[i].getScore();
        if (inocents[i].isAlive())
            inocentsScore++;
        else
            traitorScore += 2;
    }

    for (i = 0; i < NUMBER_OF_TRAITORS; i++)
    {
        traitorScore += traitors[i].getScore();
        if (traitors[i].isAlive())
            traitorScore++;
        else
            inocentsScore += 2;
    }

    for (i = 0; i < NUMBER_OF_DETECTIVES; i++)
    {
        inocentsScore += detectives[i].getScore();
        if (detectives[i].isAlive())
            inocentsScore++;
        else
            traitorScore += 2;
    }

    //std::cout << "inocent Score: " << inocentsScore << std::endl;
    //std::cout << "traitor Score: " << traitorScore << std::endl;
}

void Moderator::resetAllPlayers()
{
    screen->resetImage();
    screen->createObstacle();

    resetPlayers(inocents, NUMBER_OF_INOCENTS, INOCENT_HEALTH);
    resetPlayers(traitors, NUMBER_OF_TRAITORS, TRAITOR_HEALTH);
    resetPlayers(detectives, NUMBER_OF_DETECTIVES, DETECTIVE_HEALTH);
}

void Moderator::resetPlayers(Player *players, int NUMBER_OF_PLAYERS, int life)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
        players[i].reset(life);
}

void Moderator::setAllWeights(Inocent *bestInocents, Traitor *bestTraitors, Detective *bestDetectives)
{
    setWeights(bestInocents, inocents, NUMBER_OF_INOCENTS);
    //setWeights(bestTraitors, traitors, NUMBER_OF_TRAITORS);
    //setWeights(bestDetectives, detectives, NUMBER_OF_DETECTIVES);
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
    copyWeights(bestInocents, inocents, NUMBER_OF_INOCENTS);
    //copyWeights(bestTraitors, traitors, NUMBER_OF_TRAITORS);
    //copyWeights(bestDetectives, detectives, NUMBER_OF_DETECTIVES);
}

void Moderator::copyWeights(Player *bestPlayers, Player *players, int NUMBER_OF_PLAYERS)
{
    unsigned int j;

    MatrixXf *newMatrixArray;
    MatrixXf *matrixArray;

    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        newMatrixArray = new MatrixXf[layers.size() + 1];
        //std::cout << players[i].ann->getMatrixPtr() << std::endl;
        delete[] players[i].ann->getMatrixPtr();
        matrixArray = bestPlayers[i].ann->getMatrixPtr();
        for (j = 0; j < layers.size() + 1; j++)
        {
            newMatrixArray[j] = matrixArray[j];
        }
        players[i].ann->setMatrix(newMatrixArray);

        //std::cout << players[i].ann->getMatrixPtr()[2] << std::endl;
    }
}