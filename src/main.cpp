#include <iostream>
#include <string.h>
#include <thread>
#include <mutex>

#include "headers/Moderator.h"
#include "headers/ANN.h"
#include "headers/Evolution.h"

// g++ *.cpp -o main `pkg-config --cflags --libs opencv4`
//https://stackoverflow.com/questions/23683023/how-to-store-a-matrix-of-custom-objects-in-c

std::mutex mtx;
Moderator *bestModerator;

int gen = 1;

void runModerator(Moderator *copyModerator)
{
    copyModerator->gameOfBest();
    copyModerator->resetAllPlayers(true);
}

void copyModerator()
{
    Moderator *copyModerator = new Moderator;
    Screen *screen = new Screen;
    Inocent *inocents = bestModerator->getInocents();
    Traitor *traitors = bestModerator->getTraitors();
    Detective *detectives = bestModerator->getDetectives();

    copyModerator->setScreen(screen);
    copyModerator->setAllPlayersValues();
    copyModerator->screen->setScreenParam("indvs");

    while (true)
    {
        mtx.lock();
        std::cout << "copying " << std::endl;
        copyModerator->copyAllWeights(inocents, traitors, detectives);
        std::cout << "finish copy " << std::endl;
        mtx.unlock();
        std::thread th(runModerator, copyModerator);
        th.join();
    }
}

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

    bestModerator = new Moderator;
    bestModerator->setScreen(screenOfBest);
    bestModerator->setAllPlayersValues();
    //bestModerator->screen->setScreenParam("best indvs");

    Evolution *evolution = new Evolution;

    evolution->setParam(bestModerator);

    std::thread th(copyModerator);

    while (1)
    {
        std::cout << "gen: " << gen << std::endl;

        mtx.lock();
        bestModerator->game();
        bestModerator->resetAllPlayers(false);
        bestModerator->game();

        bestModerator->calculateScore();
        //evolution->tournamentAll();
        evolution->eletismAll();

        mtx.unlock();

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