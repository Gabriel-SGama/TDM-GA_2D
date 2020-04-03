#include "headers/Moderator.h"

Moderator::Moderator(Screen *screen)
{
    this->screen = screen;

    int i;

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
        inocents[i].setPlayerValues(screen, playerNumber);
        inocents[i].drawPlayer();
    }

    for (i = 0; i < NUMBER_OF_TRAITORS; i++, playerNumber++)
    {
        traitors[i].setPlayerValues(screen, playerNumber);
        traitors[i].drawPlayer();
    }

    for (i = 0; i < NUMBER_OF_DETECTIVES; i++, playerNumber++)
    {
        detectives[i].setPlayerValues(screen, playerNumber);
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
        {
            inocents[i].updateVision();
            inocents[i].killPlayer(0, inocents, traitors, nullptr);
        }
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