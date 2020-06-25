#include <iostream>
#include <string.h>
#include <thread>
#include <chrono>
#include <mutex>

#include "headers/Plot.h"

using namespace std::chrono_literals;

std::mutex mtx;
Evolution *evolution;
Plot *plot;

void copyModerator() {

    Moderator *copyModerator = new Moderator;

    LightAssault *lightAssaults = evolution->bestTeams->getLightAssaults();
    Sniper *snipers = evolution->bestTeams->getSnipers();
    Assault *assaults = evolution->bestTeams->getAssaults();

    copyModerator->setScreen(new Screen);
    copyModerator->screen->setScreenParam("best teams", 0, 0);
    copyModerator->setAllPlayersValues();

    Moderator *bestIndvsCopy = new Moderator;

    bestIndvsCopy->setScreen(new Screen);
    bestIndvsCopy->screen->setScreenParam("best indvs", LENGTH + 67, 0);
    bestIndvsCopy->setAllPlayersValues();

    ANN *bestLightAssaultMatrix = new ANN;
    ANN *bestSniperMatrix = new ANN;
    ANN *bestAssaultMatrix = new ANN;

    bestLightAssaultMatrix->setANNParameters(lightAssaults->ANNInputSize, lightAssaults->ANNOutputSize);
    bestSniperMatrix->setANNParameters(snipers->ANNInputSize, snipers->ANNOutputSize);
    bestAssaultMatrix->setANNParameters(assaults->ANNInputSize, assaults->ANNOutputSize);

    while (true) {
        //----------------BEST TEAM MATCH----------------
        mtx.lock();

        copyModerator->setInicialPosAll(initialPos, rand() % 3);
        copyModerator->copyAllWeights(lightAssaults, snipers, assaults);

        mtx.unlock();

        copyModerator->gameOfBest();
        copyModerator->resetAllPlayers(true);
        plot->plotData();

        //----------------BEST PLAYER MATCH----------------
        mtx.lock();
        bestLightAssaultMatrix->copyWheights(evolution->bestLightAssaultANN->getMatrixPtr());
        bestSniperMatrix->copyWheights(evolution->bestSniperANN->getMatrixPtr());
        bestAssaultMatrix->copyWheights(evolution->bestAssaultANN->getMatrixPtr());

        bestIndvsCopy->setInicialPosAll(initialPos, rand() % 3);
        bestIndvsCopy->setAllWeightsOneMatrix(bestLightAssaultMatrix->getMatrixPtr(), bestSniperMatrix->getMatrixPtr(), bestAssaultMatrix->getMatrixPtr());
        mtx.unlock();

        bestIndvsCopy->gameOfBest();
        bestIndvsCopy->resetAllPlayers(true);

        //----------------PLOT----------------
        plot->plotData();
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
    scoreData_t scoreData;
    plot = new Plot;
    std::thread th(copyModerator);
    
    /*
    evolution->readANNAll();
    Moderator *bestIndvsCopy = new Moderator;

    bestIndvsCopy->setScreen(new Screen);
    bestIndvsCopy->screen->setScreenParam("best indvs", LENGTH + 67, 0);
    bestIndvsCopy->setAllPlayersValues();

    ANN *bestLightAssaultMatrix = new ANN;
    ANN *bestSniperMatrix = new ANN;
    ANN *bestAssaultMatrix = new ANN;

    LightAssault *lightAssaults = evolution->bestTeams->getLightAssaults();
    Sniper *snipers = evolution->bestTeams->getSnipers();
    Assault *assaults = evolution->bestTeams->getAssaults();

    bestLightAssaultMatrix->setANNParameters(lightAssaults->ANNInputSize, lightAssaults->ANNOutputSize);
    bestSniperMatrix->setANNParameters(snipers->ANNInputSize, snipers->ANNOutputSize);
    bestAssaultMatrix->setANNParameters(assaults->ANNInputSize, assaults->ANNOutputSize);

    //----------------BEST PLAYER MATCH----------------
    bestLightAssaultMatrix->copyWheights(evolution->bestLightAssaultANN->getMatrixPtr());
    bestSniperMatrix->copyWheights(evolution->bestSniperANN->getMatrixPtr());
    bestAssaultMatrix->copyWheights(evolution->bestAssaultANN->getMatrixPtr());

    bestIndvsCopy->setInicialPosAll(initialPos, rand() % 3);
    bestIndvsCopy->setAllWeightsOneMatrix(bestLightAssaultMatrix->getMatrixPtr(), bestSniperMatrix->getMatrixPtr(), bestAssaultMatrix->getMatrixPtr());

    bestIndvsCopy->gameOfBest();
    bestIndvsCopy->resetAllPlayers(true);
    return 0;
    */
   
    // Moderator testMod;
    // testMod.setScreen(new Screen);
    // testMod.screen->setScreenParam("teste", 0, 0);
    // testMod.setAllPlayersValues();
    // testMod.gameOfBest();

    while (1) {
        mtx.lock();
        //----------------EVOLUTION----------------
        evolution->game();
        evolution->tournamentAll();

        std::cout << "-------------GEN " << gen << " -------------" << std::endl;
        std::cout << "best light assault team score: " << evolution->bestLightAssaultTeamScore << std::endl;
        std::cout << "best sniper team score: " << evolution->bestSniperTeamScore << std::endl;
        std::cout << "best assault team score: " << evolution->bestAssaultTeamScore << std::endl;

        scoreData = evolution->setBestIndvs();

        if (!(gen % 20)) {
            evolution->genocideAll();
            std::cout << "-------------genocide-------------" << std::endl;
        }

        evolution->reset();

        // if(gen == 50){
        //     evolution->saveANNAll("matrix50.txt");
        // }
        // if(gen == 100){
        //     evolution->saveANNAll("matrix100.txt");
        // }
        // if(gen == 150){
        //     evolution->saveANNAll("matrix150.txt");
        // }
        // if(gen == 200){
        //     evolution->saveANNAll("matrix200.txt");
        // }
        // if(gen == 250){
        //     evolution->saveANNAll("matrix250.txt");
        // }
        // if(gen == 300){
        //     evolution->saveANNAll("matrix300.txt");
        //     return 0;
        // }

        mtx.unlock();

        plot->addData(scoreData);
        std::this_thread::sleep_for(1ms); //time to change threads

        gen++;
    }

    return 0;
}