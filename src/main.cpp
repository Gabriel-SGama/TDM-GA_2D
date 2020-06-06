#include <iostream>
#include <string.h>
#include <thread>
#include <mutex>
#include <ctime>

#include "headers/Evolution.h"

std::mutex mtx;
Evolution *evolution;

void copyModerator() {
    Moderator *copyModerator = new Moderator;
    //Screen *screen = new Screen;

    ANN *bestANN = evolution->bestTrainingANN;

    copyModerator->setModerator();
    copyModerator->setScreen(new Screen);
    copyModerator->screen->setScreenParam("best team", 0, 0);
    copyModerator->setAllPlayersValues();

    cv::Point initialPos[] = {cv::Point(LENGTH-300,HEIGHT-250), cv::Point(LENGTH-400,150), cv::Point(0,150)};

    while (true) {
        mtx.lock();
       
        copyModerator->setInicialPosAll(initialPos, rand() % 3);
        // copyModerator->setAllWeightsMod(lightAssaults->ann, snipers->ann, assaults->ann);
        copyModerator->setAllWeightsMod(copyModerator->bestANN, nullptr);
        copyModerator->setAllWeightsMod(nullptr, evolution->bestTrainingANN);

        mtx.unlock();

        copyModerator->gameOfBest();
        copyModerator->resetAllPlayers(true);

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

    std::clock_t begin;
    std::clock_t end;    
    double elapsed_secs;
    double totalMin = 0;

    evolution = new Evolution;

    std::thread th(copyModerator);

    while (1) {
        mtx.lock();
        begin = clock();
        evolution->game();
        end = clock();
        elapsed_secs = double(end-begin) / (CLOCKS_PER_SEC*4);
        // totalMin += elapsed_secs / 60.0;
        evolution->tournamentMod();
        std::cout << "-------------GEN " << gen << " -------------" << std::endl;
        std::cout << "elapsed: " << elapsed_secs << std::endl;
        std::cout << "best team score: " << evolution->bestTeamScore << std::endl;

        if (!(gen % 15)) {
            evolution->genocide();
            std::cout << "-------------genocide-------------" << std::endl;
        }

        evolution->reset();
        mtx.unlock();

        cv::waitKey(1);  //time to copy
        gen++;
    }

    return 0;
}