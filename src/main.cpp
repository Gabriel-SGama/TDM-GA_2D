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
    int gen = 0;

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
        std::cout << "gen: " << gen << std::endl;

        if (!(gen % 20))
        {
            bestModerator->gameOfBest();
            bestModerator->resetAllPlayers(false);
            bestModerator->gameOfBest();
        }
        else
        {
            bestModerator->game();
            bestModerator->resetAllPlayers(false);
            bestModerator->game();
        }

        //bestModerator->gameOfBest();
        //bestModerator->resetAllPlayers(false);
        //bestModerator->gameOfBest();

        bestModerator->calculateScore();
        //evolution->tournamentAll();
        evolution->eletismAll();

        std::cout << "best inocent: " << bestModerator->bestInocent->player->getPlayerID()
                  << " | score: " << bestModerator->bestInocent->score << std::endl;

        std::cout << "best traitor: " << bestModerator->bestTraitor->player->getPlayerID()
                  << " | score: " << bestModerator->bestTraitor->score << std::endl;

        std::cout << "best detectives: " << bestModerator->bestDetective->player->getPlayerID()
                  << " | score: " << bestModerator->bestDetective->score << std::endl;

        bestModerator->resetAllPlayers(true);

        gen++;
    }

    return 0;
}