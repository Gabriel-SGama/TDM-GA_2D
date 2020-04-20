#include <iostream>
#include <string.h>

#include "headers/Player.h"
#include "headers/Screen.h"
#include "headers/Inocent.h"
#include "headers/Traitor.h"
#include "headers/Detective.h"
#include "headers/Moderator.h"
#include "headers/ANN.h"
#include "headers/Evolution.h"

// g++ *.cpp -o main `pkg-config --cflags --libs opencv4`
//https://stackoverflow.com/questions/23683023/how-to-store-a-matrix-of-custom-objects-in-c

int main()
{

    srand(time(0));

    Screen *screenOfBest = new Screen;

    /*
    if (!screen->getMap().isContinuous())
    {
        std::cout << "Is not continuous, need to change code" << std::endl;
        return -1;
    }
    */

    screenOfBest->createObstacle();

    Moderator *bestModerator = new Moderator;
    bestModerator->setScreen(screenOfBest);
    bestModerator->setAllPlayersValues();
    bestModerator->screen->setScreenParam("best indvs");

    Evolution *evolution = new Evolution;

    evolution->setParam(bestModerator);

    while (1)
    {

        bestModerator->gameOfBest();

        bestModerator->calculateScore();
        evolution->eletism();
        bestModerator->resetAllPlayers();
    }

    return 0;
}