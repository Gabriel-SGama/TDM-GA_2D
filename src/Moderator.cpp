#include "headers/Moderator.h"

Moderator::Moderator(Screen *screen)
{
    this->screen = screen;

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
    {
        players[i].setPlayerValues(screen, playerNumber, INOCENT_HEALTH);
        players[i].drawPlayer();
    }
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
            if (enemyInfo.playerType == INOCENT)
            {
            }

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

    else if (enemyInfo.playerType == TRAITOR && findPlayer(shooter, traitors, NUMBER_OF_TRAITORS, enemyInfo.posiAprox))
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
        distance = cv::norm(enemyPoint - players[i].getCenter());

        if (distance <= RADIUS)
        {
            players[i].takeDamage(shooter->getDamage());
            std::cout << shooter->getPlayerID() << "matou " << players[i].getPlayerID() << std::endl;
            std::cout << "vida: " << players[i].getLife() << std::endl;
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