#include <iostream>

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
    srand(time(0));

    Screen *screen = new Screen;

    if (!screen->getMap().isContinuous())
    {
        std::cout << "Is not continuous, need to change code" << std::endl;
        return -1;
    }

    screen->createObstacle();

    Moderator *moderator = new Moderator(screen);

    moderator->setAllPlayersValues();

    while (1)
    {

        screen->resetImage();
        screen->createObstacle();

        moderator->drawAllPlayers();
        moderator->updateAllPlayersVision();
        moderator->conflicts();
        moderator->checkAllPlayersLife();

        screen->updateMap();

        moderator->multiplyAllPlayers();
    }

    return 0;
}