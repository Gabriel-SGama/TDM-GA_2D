#include <iostream>
#include <string.h>

#include "headers/Player.h"
#include "headers/Screen.h"
#include "headers/Inocent.h"
#include "headers/Traitor.h"
#include "headers/Detective.h"
#include "headers/Moderator.h"
#include "headers/ANN.h"
#include "headers/Learning.h"

// g++ *.cpp -o main `pkg-config --cflags --libs opencv4`
//https://stackoverflow.com/questions/23683023/how-to-store-a-matrix-of-custom-objects-in-c

int main()
{
    int i;

    dataOfBest_t dataOfBest;

    std::cout << dataOfBest.bestInocentScore << std::endl;

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

    Moderator *inocentsTraining = new Moderator[POP_SIZE];
    Moderator *traitorsTraining = new Moderator[POP_SIZE];

    Screen *temp;

    for (i = 0; i < POP_SIZE; i++)
    {
        temp = new Screen;

        //temp->setScreenParam(std::to_string(i));
        inocentsTraining[i].setScreen(temp);
        inocentsTraining[i].setAllPlayersValues();
        inocentsTraining[i].drawAllPlayers();
        inocentsTraining[i].setAllWeights(nullptr, bestModerator->getTraitors(), nullptr);

        temp = new Screen;
        //temp->setScreenParam(std::to_string(i + POP_SIZE));

        traitorsTraining[i].setScreen(temp);
        traitorsTraining[i].setAllPlayersValues();
        traitorsTraining[i].drawAllPlayers();
        traitorsTraining[i].setAllWeights(bestModerator->getInocents(), nullptr, bestModerator->getDetectives());
    }

    Learning *learning = new Learning;
    learning->setData(inocentsTraining, traitorsTraining, &dataOfBest);

    while (1)
    {
        for (i = 0; i < POP_SIZE; i++)
        {
            inocentsTraining[i].game();
            traitorsTraining[i].game();
        }

        bestModerator->gameOfBest();

        for (i = 0; i < POP_SIZE; i++)
        {
            //inocentsTraining[i].calculateScore();
            //traitorsTraining[i].calculateScore();
            if (inocentsTraining[i].inocentsScore > dataOfBest.bestInocentScore)
            {
                std::cout << "change" << std::endl;
                dataOfBest.bestInocentScore = inocentsTraining[i].inocentsScore;
                dataOfBest.bestInocentIndex = i;
            }
            if (traitorsTraining[i].traitorScore > dataOfBest.bestTraitorScore)
            {
                std::cout << "change" << std::endl;
                dataOfBest.bestTraitorScore = traitorsTraining[i].traitorScore;
                dataOfBest.bestTraitorIndex = i;
            }

            inocentsTraining[i].resetAllPlayers();
            traitorsTraining[i].resetAllPlayers();
        }

        bestModerator->copyAllWeights(inocentsTraining[dataOfBest.bestInocentIndex].getInocents(), traitorsTraining[dataOfBest.bestTraitorIndex].getTraitors(), inocentsTraining[dataOfBest.bestInocentIndex].getDetectives());
        for (i = 0; i < POP_SIZE; i++)
        {
            inocentsTraining[i].setAllWeights(nullptr, bestModerator->getTraitors(), nullptr);
            traitorsTraining[i].setAllWeights(bestModerator->getInocents(), nullptr, bestModerator->getDetectives());
        }
        bestModerator->calculateScore();
        bestModerator->resetAllPlayers();
    }

    return 0;
}