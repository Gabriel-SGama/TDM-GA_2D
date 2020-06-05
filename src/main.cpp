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

    LightAssault *lightAssaults = evolution->bestTeams->getLightAssaults();
    Sniper *snipers = evolution->bestTeams->getSnipers();
    Assault *assaults = evolution->bestTeams->getAssaults();

    copyModerator->setModerator(NUMBER_OF_LIGHT_ASSAULTS, NUMBER_OF_SNIPERS, NUMBER_OF_ASSAULTS);
    copyModerator->setScreen(new Screen);
    copyModerator->screen->setScreenParam("best teams", 0, 0);
    copyModerator->setAllPlayersValues();

    cv::Point initialPos[] = {cv::Point(LENGTH-300,HEIGHT-250), cv::Point(LENGTH-400,150), cv::Point(0,150)};

    while (true) {
        mtx.lock();
       
        copyModerator->setInicialPosAll(initialPos, rand() % 3);
        copyModerator->setAllWeightsMod(lightAssaults->ann, snipers->ann, assaults->ann);

        mtx.unlock();

        copyModerator->gameOfBest();
        std::cout << "best scores:  "<< copyModerator->lightAssaultScore <<
        " | " << copyModerator->sniperScore << " | " << copyModerator->assaultScore << std::endl;
        
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
        totalMin += elapsed_secs / 60.0;
        evolution->tournamentAllMod();
        std::cout << "-------------GEN " << gen << " -------------" << std::endl;
        std::cout << "elapsed: " << elapsed_secs <<" | total time+-: " << totalMin << "(min)" << std::endl;
        std::cout << "best light assault team score: " << evolution->bestLightAssaultTeamScore << std::endl;
        std::cout << "best sniper team score: " << evolution->bestSniperTeamScore << std::endl;
        std::cout << "best assault team score: " << evolution->bestAssaultTeamScore << std::endl;

        if (!(gen % 15)) {
            evolution->genocideAll();
            std::cout << "-------------genocide-------------" << std::endl;
        }

        evolution->reset();
        mtx.unlock();
        //*/

        cv::waitKey(1);  //time to copy
        gen++;
    }

    return 0;
}