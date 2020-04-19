#include <iostream>
#include <string.h>

#include "headers/Player.h"
#include "headers/Screen.h"
#include "headers/Inocent.h"
#include "headers/Traitor.h"
#include "headers/Detective.h"
#include "headers/Moderator.h"
#include "headers/ANN.h"

// g++ *.cpp -o main `pkg-config --cflags --libs opencv4`
//https://stackoverflow.com/questions/23683023/how-to-store-a-matrix-of-custom-objects-in-c

int main()
{
    int bestInocentScore = -100;
    int bestTraitorScore = -100;
    int bestInocentIndex = 0;
    int bestTraitorIndex = 0;
    int i;
    int move = 0;

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

    std::vector<Moderator *> Moderators;
    Moderators.resize(POP_SIZE);

    Screen *temp;

    for (i = 0; i < POP_SIZE; i++)
    {
        Moderators[i] = new Moderator;
        temp = new Screen;

        //temp->setScreenParam(std::to_string(i));

        Moderators[i]->setScreen(temp);
        Moderators[i]->setAllPlayersValues();
        Moderators[i]->drawAllPlayers();
    }

    while (1)
    {
        for (move = 0; move < DURATION; move++)
        {

            for (i = 0; i < POP_SIZE; i++)
            {
                Moderators[i]->drawAllPlayers();
                Moderators[i]->updateAllPlayersVision();
                Moderators[i]->conflictsAllPlayers();
                Moderators[i]->checkAllPlayersLife();

                //Moderators[i]->screen->updateMap();

                Moderators[i]->defineAllPlayersInput();
                Moderators[i]->multiplyAllPlayers();
            }

            bestModerator->drawAllPlayers();
            bestModerator->updateAllPlayersVision();
            bestModerator->conflictsAllPlayers();
            screenOfBest->updateMap();
            bestModerator->checkAllPlayersLife();

            screenOfBest->updateMap();

            bestModerator->defineAllPlayersInput();
            bestModerator->multiplyAllPlayers();

            if (!(move % 50))
                std::cout << move << std::endl;
        }

        for (i = 0; i < POP_SIZE; i++)
        {
            Moderators[i]->calculateScore();
            if (Moderators[i]->inocentsScore > bestInocentScore)
            {
                bestInocentScore = Moderators[i]->inocentsScore;
                bestInocentIndex = i;
            }
            if (Moderators[i]->traitorScore > bestTraitorScore)
            {
                bestTraitorScore = Moderators[i]->traitorScore;
                bestTraitorIndex = i;
            }
            Moderators[i]->resetAllPlayers();
        }

        bestModerator->copyAllWeights(Moderators[bestInocentIndex]->getInocents(), Moderators[bestTraitorIndex]->getTraitors(), Moderators[bestInocentIndex]->getDetectives());

        bestModerator->calculateScore();
        bestModerator->resetAllPlayers();
    }

    return 0;
}