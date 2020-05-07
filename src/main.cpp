#include <iostream>
#include <string.h>
#include <thread>
#include <mutex>

#include "headers/Moderator.h"
#include "headers/ANN.h"
#include "headers/Evolution.h"

// g++ *.cpp -o main `pkg-config --cflags --libs opencv4`

std::mutex mtx;
Evolution *evolution;

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

    ANN *bestLightAssaultMatrix = new ANN;
    ANN *bestSniperMatrix = new ANN;
    ANN *bestAssaultMatrix = new ANN;

    bestLightAssaultMatrix->setANNParameters(lightAssaults->ANNInputSize, lightAssaults->ANNOutputSize);
    bestSniperMatrix->setANNParameters(snipers->ANNInputSize, snipers->ANNOutputSize);
    bestAssaultMatrix->setANNParameters(assaults->ANNInputSize, assaults->ANNOutputSize);

    //bestIndvsCopy->setAllWeightsOneMatrix(bestLightAssaultMatrix->getMatrixPtr(), bestSniperMatrix->getMatrixPtr(), bestAssaultMatrix->getMatrixPtr());

    while (true)
    {
        mtx.lock();
        //evolution->setBestIndvs();
        std::cout << "copying " << std::endl;
        copyModerator->copyAllWeights(lightAssaults, snipers, assaults);
        std::cout << "finish copy " << std::endl;

        mtx.unlock();

        copyModerator->gameOfBest();
        copyModerator->resetAllPlayers(true);

        mtx.lock();

        bestLightAssaultMatrix->copyWheights(evolution->bestLightAssaultANN->getMatrixPtr());
        bestSniperMatrix->copyWheights(evolution->bestSniperANN->getMatrixPtr());
        bestAssaultMatrix->copyWheights(evolution->bestAssaultANN->getMatrixPtr());

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

    LightAssault *lightAssault1 = new LightAssault;
    LightAssault *lightAssault2 = new LightAssault;
    Sniper *sniper = new Sniper;
    Assault *assault = new Assault;

    lightAssault1->setPlayerValues(screen, 0, 100, nullptr);
    lightAssault2->setPlayerValues(screen, 0, 100, nullptr);
    sniper->setPlayerValues(screen, 1, 100, nullptr);
    assault->setPlayerValues(screen, 2, 100, nullptr);
    
    Eigen::MatrixXf *matrixArray = lightAssault1->ann->getMatrixPtr();    
    //*/

    while (1)
    {
        std::cout << "gen: " << gen << std::endl;
        /*
        screen->resetImage();
        screen->createObstacle();

        lightAssault1->drawPlayer();
        lightAssault2->drawPlayer();
        sniper->drawPlayer();
        assault->drawPlayer();

        lightAssault1->updateVision();
        lightAssault2->updateVision();
        sniper->updateVision();
        assault->updateVision();

        screen->updateMap();
        lightAssault1->setComunInput();
        //lightAssault2->setComunInput();
        lightAssault1->ann->multiply();
        //lightAssault2->ann->multiply();
        lightAssault1->move();
        //lightAssault2->move();
        //sniper->move();
        //assault->move();

        if (gen == 5)
        {
            std::cout << "changing" << std::endl;
            lightAssault1->ann->copyWheights(lightAssault2->ann->getMatrixPtr());
        }

        if (!(gen % 10))
        {
            matrixArray = lightAssault1->ann->getMatrixPtr();
        }
        std::cout << "matrix array:" << matrixArray[0] << std::endl;

        //*/
        ///*
        mtx.lock();
        evolution->game();
        evolution->tournamentAll();

        std::cout << "best light assault team score: " << evolution->bestLightAssaultTeamScore << std::endl;
        std::cout << "best sniper team score: " << evolution->bestSniperTeamScore << std::endl;
        std::cout << "best assault team score: " << evolution->bestAssaultTeamScore << std::endl;

        evolution->setBestIndvs();

        if (!(gen % 20))
        {
            evolution->genocideAll();
            std::cout << "genocide" << std::endl;
        }

        evolution->reset();
        mtx.unlock();
        //*/

        cv::waitKey(1); //time to copy

        gen++;
    }

    return 0;
}