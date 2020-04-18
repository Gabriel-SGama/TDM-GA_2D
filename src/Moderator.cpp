#include "headers/Moderator.h"

Moderator::Moderator(Screen *screen)
{
    this->screen = screen;

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

void Moderator::conflicts()
{
    bool shot = false;

    int i;
    //int playerID;

    cv::Point enemyPoint;
    /*
    TODO:
        replace 'true' for the result of the neural network
        replace '0' for the ray choosen
    */

    for (i = 0; i < NUMBER_OF_INOCENTS; i++)
    {
        if (inocents[i].isAlive() && true)
        {
            enemyPoint = inocents[i].killPlayer(0);
            if (enemyPoint == cv::Point(-1, -1))
                continue;

            shot = true;
            shotPlayer(&inocents[i], INOCENT_DAMAGE, enemyPoint);
        }
    }

    for (i = 0; i < NUMBER_OF_TRAITORS && !shot; i++)
    {
        if (traitors[i].isAlive())
            traitors[i].updateVision();
    }

    for (i = 0; i < NUMBER_OF_DETECTIVES && !shot; i++)
    {
        if (detectives[i].isAlive())
            detectives[i].updateVision();
    }
}

void Moderator::shotPlayer(Player *shooter, int damage, cv::Point enemyPoint)
{
    int i;
    float distance;

    //find the player that was shot
    for (i = 0; i < NUMBER_OF_INOCENTS; i++)
    {
        distance = cv::norm(enemyPoint - inocents[i].getCenter());

        if (distance <= RADIUS)
        {
            inocents[i].takeDamage(damage);
            std::cout << shooter->getPlayerID() << "matou " << inocents[i].getPlayerID() << std::endl;
            std::cout << "vida: " << inocents[i].getLife() << std::endl;
            break;
        }
    }

    for (i = 0; i < NUMBER_OF_TRAITORS; i++)
    {
        distance = cv::norm(enemyPoint - traitors[i].getCenter());

        if (distance <= RADIUS)
        {
            traitors[i].takeDamage(damage);
            std::cout << shooter->getPlayerID() << "matou " << traitors[i].getPlayerID() << std::endl;
            std::cout << "vida: " << traitors[i].getLife() << std::endl;
            break;
        }
    }

    for (i = 0; i < NUMBER_OF_DETECTIVES; i++)
    {
        distance = cv::norm(enemyPoint - detectives[i].getCenter());

        if (distance <= RADIUS)
        {
            detectives[i].takeDamage(damage);
            std::cout << shooter->getPlayerID() << "matou " << detectives[i].getPlayerID() << std::endl;
            std::cout << "vida: " << detectives[i].getLife() << std::endl;
            break;
        }
    }
}

void Moderator::checkAllPlayersLife()
{
    int i;

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