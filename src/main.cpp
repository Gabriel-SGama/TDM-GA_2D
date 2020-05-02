#include <iostream>
#include <string.h>
#include <thread>
#include <mutex>

#include "headers/Moderator.h"
#include "headers/ANN.h"
#include "headers/Evolution.h"

// g++ *.cpp -o main `pkg-config --cflags --libs opencv4`

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
    //cv::waitKey(1000);

    Moderator *copyModerator = new Moderator;
    //Screen *screen = new Screen;

    LightAssault *lightAssaults = evolution->bestTeams->getLightAssaults();
    Sniper *snipers = evolution->bestTeams->getSnipers();
    Assault *assaults = evolution->bestTeams->getAssaults();

    copyModerator->setModerator(NUMBER_OF_LIGHT_ASSAULTS, NUMBER_OF_SNIPERS, NUMBER_OF_ASSAULTS);
    copyModerator->setScreen(new Screen);
    copyModerator->screen->setScreenParam("best teams");
    copyModerator->setAllPlayersValues();

    Moderator *bestIndvsCopy = new Moderator;

    bestIndvsCopy->setModerator(NUMBER_OF_LIGHT_ASSAULTS, NUMBER_OF_SNIPERS, NUMBER_OF_ASSAULTS);
    bestIndvsCopy->setScreen(new Screen);
    bestIndvsCopy->screen->setScreenParam("best indvs");
    bestIndvsCopy->setAllPlayersValues();

    //mtx.lock();
    // evolution->setBestIndvs();
    ANN *bestLightAssaultMatrix = evolution->bestLightAssaultANN;
    ANN *bestSniperMatrix = evolution->bestSniperANN;
    ANN *bestAssaultMatrix = evolution->bestAssaultANN;

    bestIndvsCopy->setAllWeightsOneMatrix(bestLightAssaultMatrix->getMatrixPtr(), bestSniperMatrix->getMatrixPtr(), bestAssaultMatrix->getMatrixPtr());

    while (true)
    {
        mtx.lock();
        //evolution->setBestIndvs();
        std::cout << "copying " << std::endl;
        copyModerator->copyAllWeights(lightAssaults, snipers, assaults);
        std::cout << "finish copy " << std::endl;

        //std::thread th2(runModerator, bestIndvsCopy);
        mtx.unlock();

        //std::thread th1(runModerator, copyModerator);

        copyModerator->gameOfBest();
        copyModerator->resetAllPlayers(true);

        mtx.lock();

        bestLightAssaultMatrix = evolution->bestLightAssaultANN;
        bestSniperMatrix = evolution->bestSniperANN;
        bestAssaultMatrix = evolution->bestAssaultANN;

        bestIndvsCopy->setAllWeightsOneMatrix(bestLightAssaultMatrix->getMatrixPtr(), bestSniperMatrix->getMatrixPtr(), bestAssaultMatrix->getMatrixPtr());
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

    /*
    if (!screen->getMap().isContinuous())
    {
        std::cout << "Is not continuous, need to change code" << std::endl;
        return -1;
    }
    */

    evolution = new Evolution;

    std::thread th(copyModerator);
    /*
    Screen *screen = new Screen;

    screen->setScreenParam("test");
    screen->createObstacle();
    
    LightAssault *lightAssault = new LightAssault;
    Sniper *sniper = new Sniper;
    Assault *assault = new Assault;

    lightAssault->setPlayerValues(screen, 0, 100, nullptr);
    sniper->setPlayerValues(screen, 1, 100, nullptr);
    assault->setPlayerValues(screen, 2, 100, nullptr);
    //*/

    while (1)
    {
        std::cout << "gen: " << gen << std::endl;
        /*
        screen->resetImage();
        screen->createObstacle();

        lightAssault->drawPlayer();
        sniper->drawPlayer();
        assault->drawPlayer();
        
        lightAssault->updateVision();
        sniper->updateVision();
        assault->updateVision();
        
        screen->updateMap();
        
        lightAssault->move();
        sniper->move();
        assault->move();
        */
        ///*
        mtx.lock();
        evolution->game();
        evolution->tournamentAll();

        std::cout << "best light assault team score: " << evolution->bestLightAssaultTeamScore << std::endl;
        std::cout << "best sniper team score: " << evolution->bestSniperTeamScore << std::endl;
        std::cout << "best assault team score: " << evolution->bestAssaultTeamScore << std::endl;

        evolution->setBestIndvs();
        evolution->reset();
        mtx.unlock();
        //*/

        cv::waitKey(1); //time to copy

        gen++;
    }

    return 0;
}