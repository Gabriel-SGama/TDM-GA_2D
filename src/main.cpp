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
Evolution *evolution;

void runModerator(Moderator *copyModerator)
{
    copyModerator->gameOfBest();
    copyModerator->resetAllPlayers(true);
}

void copyModerator()
{
    cv::waitKey(1000);

    Moderator *copyModerator = new Moderator;
    //Screen *screen = new Screen;

    Inocent *inocents = evolution->bestTeams->getInocents();
    Traitor *traitors = evolution->bestTeams->getTraitors();
    Detective *detectives = evolution->bestTeams->getDetectives();

    copyModerator->setScreen(new Screen);
    copyModerator->screen->setScreenParam("best teams");
    copyModerator->setAllPlayersValues();

    Moderator *bestIndvsCopy = new Moderator;

    bestIndvsCopy->setScreen(new Screen);
    bestIndvsCopy->screen->setScreenParam("best indvs");
    bestIndvsCopy->setAllPlayersValues();

    //mtx.lock();
    // evolution->setBestIndvs();
    ANN *bestInocentMatrix = evolution->bestInocentANN;
    ANN *bestTraitorMatrix = evolution->bestTraitorANN;
    ANN *bestDetectiveMatrix = evolution->bestDetectiveANN;

    bestIndvsCopy->setAllWeightsOneMatrix(bestInocentMatrix->getMatrixPtr(), bestTraitorMatrix->getMatrixPtr(), bestDetectiveMatrix->getMatrixPtr());

    //mtx.unlock();

    //Inocent *bestInocent = (Inocent*) evolution->bestIndvs->bestInocent->player;
    //Traitor *bestTraitor = (Traitor*) evolution->bestIndvs->bestTraitor->player;
    //Detective *bestDetective = (Detective*) evolution->bestIndvs->bestDetective->player;

    //mtx.lock();
    //evolution->bestIndvs->screen->setScreenParam("best Indvs");
    //mtx.unlock();

    while (true)
    {
        mtx.lock();
        //evolution->setBestIndvs();
        std::cout << "copying " << std::endl;
        copyModerator->copyAllWeights(inocents, traitors, detectives);
        std::cout << "finish copy " << std::endl;

        //std::thread th2(runModerator, bestIndvsCopy);
        mtx.unlock();

        //std::thread th1(runModerator, copyModerator);

        copyModerator->gameOfBest();
        copyModerator->resetAllPlayers(true);

        mtx.lock();
        evolution->setBestIndvs();
        bestIndvsCopy->setAllWeightsOneMatrix(bestInocentMatrix->getMatrixPtr(), bestTraitorMatrix->getMatrixPtr(), bestDetectiveMatrix->getMatrixPtr());
        mtx.unlock();

        bestIndvsCopy->gameOfBest();
        bestIndvsCopy->resetAllPlayers(true);

        //th1.join();
        //th2.join();
    }
}

int main()
{
    int gen = 1;

    srand(time(0));

    //Screen *screenOfBest = new Screen;

    /*
    if (!screen->getMap().isContinuous())
    {
        std::cout << "Is not continuous, need to change code" << std::endl;
        return -1;
    }
    */

    //screenOfBest->createObstacle();

    //bestModerator = new Moderator;
    //bestModerator->setScreen(new Screen);
    //bestModerator->setAllPlayersValues();
    //bestModerator->screen->setScreenParam("best indvs");

    //Evolution *evolution = new Evolution;

    evolution = new Evolution;

    //evolution->setParam(bestModerator);

    std::thread th(copyModerator);

    while (1)
    {
        std::cout << "gen: " << gen << std::endl;

        mtx.lock();
        evolution->game();
        evolution->tournamentAll();

        std::cout << "best inocent team score: " << evolution->bestInocentTeamScore << std::endl;
        std::cout << "best traitor team score: " << evolution->bestTraitorTeamScore << std::endl;
        std::cout << "best detective team score: " << evolution->bestDetectiveTeamScore << std::endl;

        //evolution->setBestIndvs();
        evolution->reset();
        mtx.unlock();

        cv::waitKey(1); //time to copy

        // if (!(gen % 10))
        //     evolution->bestPlayers->gameOfBest();

        //evolution->tournamentAll();
        // mtx.lock();
        // bestModerator->game();
        // bestModerator->resetAllPlayers(false);
        // bestModerator->game();

        // bestModerator->calculateScore();
        // evolution->tournamentAll();
        // evolution->eletismAll();

        // mtx.unlock();

        // std::cout << "best inocent: " << bestModerator->bestInocent->player->getPlayerID()
        //           << " | score: " << bestModerator->bestInocent->score << std::endl;

        // std::cout << "best traitor: " << bestModerator->bestTraitor->player->getPlayerID()
        //           << " | score: " << bestModerator->bestTraitor->score << std::endl;

        // std::cout << "best detectives: " << bestModerator->bestDetective->player->getPlayerID()
        //           << " | score: " << bestModerator->bestDetective->score << std::endl;

        // bestModerator->resetAllPlayers(true);

        gen++;
    }

    return 0;
}