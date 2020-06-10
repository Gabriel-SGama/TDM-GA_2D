#include <iostream>
#include <string.h>
#include <thread>
#include <mutex>

#include "headers/Moderator.h"
#include "headers/ANN.h"
#include "headers/Evolution.h"

std::mutex mtx;
Evolution *evolution;

void copyModerator() {
    Moderator *copyModerator = new Moderator;
    //Screen *screen = new Screen;

    LightAssault *lightAssaults = evolution->bestTeams->getLightAssaults();
    Sniper *snipers = evolution->bestTeams->getSnipers();
    Assault *assaults = evolution->bestTeams->getAssaults();

    copyModerator->setModerator();
    copyModerator->setScreen(new Screen);
    copyModerator->screen->setScreenParam("best teams", 0, 0);
    copyModerator->setAllPlayersValues();

    Moderator *bestIndvsCopy = new Moderator;

    bestIndvsCopy->setModerator();
    bestIndvsCopy->setScreen(new Screen);
    bestIndvsCopy->screen->setScreenParam("best indvs", LENGTH + 67, 0);
    bestIndvsCopy->setAllPlayersValues();

    ANN *bestLightAssaultMatrix = new ANN;
    ANN *bestSniperMatrix = new ANN;
    ANN *bestAssaultMatrix = new ANN;

    bestLightAssaultMatrix->setANNParameters(lightAssaults->ANNInputSize, lightAssaults->ANNOutputSize);
    bestSniperMatrix->setANNParameters(snipers->ANNInputSize, snipers->ANNOutputSize);
    bestAssaultMatrix->setANNParameters(assaults->ANNInputSize, assaults->ANNOutputSize);
    cv::Point initialPos[] = {cv::Point(LENGTH-300,HEIGHT-250), cv::Point(LENGTH-400,150), cv::Point(0,150)};

    while (true) {
        mtx.lock();
        // evolution->setBestIndvs();
        // std::cout << "copying " << std::endl;
        copyModerator->setInicialPosAll(initialPos, rand()%3);
        copyModerator->copyAllWeights(lightAssaults, snipers, assaults);
        // std::cout << "finish copy " << std::endl;

        mtx.unlock();

        copyModerator->gameOfBest();
        copyModerator->resetAllPlayers(true);

        mtx.lock();
        bestLightAssaultMatrix->copyWheights(evolution->bestLightAssaultANN->getMatrixPtr());
        bestSniperMatrix->copyWheights(evolution->bestSniperANN->getMatrixPtr());
        bestAssaultMatrix->copyWheights(evolution->bestAssaultANN->getMatrixPtr());

        bestIndvsCopy->setInicialPosAll(initialPos, rand() % 3);
        bestIndvsCopy->setAllWeightsOneMatrix(bestLightAssaultMatrix->getMatrixPtr(), bestSniperMatrix->getMatrixPtr(), bestAssaultMatrix->getMatrixPtr());
        mtx.unlock();
        
        bestIndvsCopy->gameOfBest();
        bestIndvsCopy->resetAllPlayers(true);
    }
}

int main() {
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
  
    topScore_t bestIndvScores;

    while (1) {
      
        mtx.lock();
        evolution->game();
        evolution->tournamentAll();
        std::cout << "-------------GEN " << gen << " -------------" << std::endl;
        std::cout << "best light assault team score: " << evolution->bestLightAssaultTeamScore << std::endl;
        std::cout << "best sniper team score: " << evolution->bestSniperTeamScore << std::endl;
        std::cout << "best assault team score: " << evolution->bestAssaultTeamScore << std::endl;

        bestIndvScores = evolution->setBestIndvs();

        if (!(gen % 20)) {
            evolution->genocideAll();
            std::cout << "-------------genocide-------------" << std::endl;
        }

        evolution->reset();
        mtx.unlock();

        cv::waitKey(1);  //time to copy
        gen++;
    }

    return 0;
}