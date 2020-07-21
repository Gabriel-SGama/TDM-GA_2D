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

        std::cout << "start game of best team" << std::endl;
        copyModerator->gameOfBest();
        std::cout << "end game of best team" << std::endl;

        copyModerator->resetAllPlayers(true);
        plot->plotData();

        //----------------BEST PLAYER MATCH----------------
        mtx.lock();
        bestLightAssaultMatrix->copyWheights(evolution->bestLightAssaultANN->getMatrixPtr(), evolution->bestLightAssaultANN->getBiasPtr());
        bestSniperMatrix->copyWheights(evolution->bestSniperANN->getMatrixPtr(), evolution->bestSniperANN->getBiasPtr());
        bestAssaultMatrix->copyWheights(evolution->bestAssaultANN->getMatrixPtr(), evolution->bestAssaultANN->getBiasPtr());

        bestIndvsCopy->setInicialPosAll(initialPos, rand() % 3);
        bestIndvsCopy->setAllWeightsOneMatrix(bestLightAssaultMatrix, bestSniperMatrix, bestAssaultMatrix);
        mtx.unlock();

        std::cout << "start game of best indv" << std::endl;
        bestIndvsCopy->gameOfBest();
        std::cout << "end game of best indv" << std::endl;
        bestIndvsCopy->resetAllPlayers(true);

        //----------------PLOT----------------
        
        std::cout << "plot" << std::endl;
        plot->plotData();
    }
}

int main() {
    int gen = 1;

    srand(time(0));

    evolution = new Evolution;
    scoreData_t scoreData;
    plot = new Plot;
    
    std::thread th(copyModerator);
    /*
    evolution->readANNAll("firstTry/matrix640.txt");
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
    //*/

    // std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    // std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    // std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;

    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;

    long double totalTime = 0;
    
    // Screen test;
    // test.setScreenParam("test", 0, 0);
    // test.createObstacle();
    // test.updateMap();
    while (1) {
        // test.updateMap();
        //----------------EVOLUTION----------------
        ///*
        mtx.lock();

        begin = std::chrono::steady_clock::now();
        std::cout << "start game" << std::endl;
        evolution->game();
        std::cout << "end game" << std::endl;
        end = std::chrono::steady_clock::now();
        totalTime += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

        std::cout << "time: " <<  std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000.0 << "s" << std::endl;
        std::cout << "med time: " <<  totalTime / (gen*1000.0) << "s" << std::endl;
        
        std::cout << "start tournament" << std::endl;
        evolution->tournamentAll();
        std::cout << "end tournament" << std::endl;

        std::cout << "-------------GEN " << gen << " -------------" << std::endl;
        std::cout << "best light assault team score: " << evolution->bestLightAssaultTeamScore << std::endl;
        std::cout << "best sniper team score: " << evolution->bestSniperTeamScore << std::endl;
        std::cout << "best assault team score: " << evolution->bestAssaultTeamScore << std::endl;

        std::cout << "getScore data" << std::endl;
        scoreData = evolution->setBestIndvs();

        if (!(gen % 20)) {
            std::cout << "-------------genocide-------------" << std::endl;
            evolution->genocideAll();
        }

        std::cout << "reset" << std::endl;
        evolution->reset();

        if((gen % 20) < 3){
            std::cout << "-------------write to file-------------" << std::endl;
            evolution->saveANNAll(("try2/matrix" + std::to_string(gen) + ".txt").c_str());
        }

        mtx.unlock();

        std::cout << "add data" << std::endl;
        plot->addData(scoreData);
        std::this_thread::sleep_for(1ms); //time to change threads

        gen++;
        //*/
       
    }

    return 0;
}