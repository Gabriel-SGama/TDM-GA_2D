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

void Moderator::setPlayersValues()
{
    int i;
    int playerNumber = 0;

    for (i = 0; i < NUMBER_OF_INOCENTS; i++, playerNumber++)
    {
        inocents[i].setPlayerValues(screen, playerNumber, INOCENT_HEALTH);
        inocents[i].drawPlayer();
    }

    for (i = 0; i < NUMBER_OF_TRAITORS; i++, playerNumber++)
    {
        traitors[i].setPlayerValues(screen, playerNumber, TRAITOR_HEALTH);
        traitors[i].drawPlayer();
    }

    for (i = 0; i < NUMBER_OF_DETECTIVES; i++, playerNumber++)
    {
        detectives[i].setPlayerValues(screen, playerNumber, DETECTIVE_HEALTH);
        detectives[i].drawPlayer();
    }
}

void Moderator::drawPlayers()
{

    int i;

    for (i = 0; i < NUMBER_OF_INOCENTS; i++)
    {
        if (inocents[i].isAlive())
            inocents[i].drawPlayer();
    }

    for (i = 0; i < NUMBER_OF_TRAITORS; i++)
    {
        if (traitors[i].isAlive())
            traitors[i].drawPlayer();
    }

    for (i = 0; i < NUMBER_OF_DETECTIVES; i++)
    {
        if (detectives[i].isAlive())
            detectives[i].drawPlayer();
    }
}

void Moderator::updatePlayersVision()
{
    int i;

    for (i = 0; i < NUMBER_OF_INOCENTS; i++)
    {
        if (inocents[i].isAlive())
            inocents[i].updateVision();
    }

    for (i = 0; i < NUMBER_OF_TRAITORS; i++)
    {
        if (traitors[i].isAlive())
            traitors[i].updateVision();
    }

    for (i = 0; i < NUMBER_OF_DETECTIVES; i++)
    {
        if (detectives[i].isAlive())
            detectives[i].updateVision();
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

void Moderator::checkLife()
{
    int i;

    //update status
    for (i = 0; i < NUMBER_OF_INOCENTS; i++)
    {
        if (inocents[i].getLife() <= 0)
            inocents[i].setAlive(false);
    }

    for (i = 0; i < NUMBER_OF_TRAITORS; i++)
    {
        if (traitors[i].getLife() <= 0)
            traitors[i].setAlive(false);
    }
    for (i = 0; i < NUMBER_OF_DETECTIVES; i++)
    {
        if (detectives[i].getLife() <= 0)
            detectives[i].setAlive(false);
    }
}

void Moderator::multiply()
{
    int i;
    //update status
    for (i = 0; i < NUMBER_OF_INOCENTS; i++)
    {
        if (inocents[i].isAlive())
        {
            inocents[i].ann->multiply();
            inocents[i].move();
        }
    }

    for (i = 0; i < NUMBER_OF_TRAITORS; i++)
    {
        if (traitors[i].isAlive())
        {
            traitors[i].ann->multiply();
            traitors[i].move();
        }
    }
    for (i = 0; i < NUMBER_OF_DETECTIVES; i++)
    {

        if (detectives[i].isAlive())
        {
            detectives[i].ann->multiply();
            detectives[i].move();
        }
    }
}