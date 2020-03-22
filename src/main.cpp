#include <iostream>
#include "headers/Player.h"
#include "headers/Screen.h"
#include "headers/Inocent.h"
#include "headers/Traitor.h"

// g++ *.cpp -o main `pkg-config --cflags --libs opencv4`
//https://stackoverflow.com/questions/23683023/how-to-store-a-matrix-of-custom-objects-in-c
int main()
{
    srand(time(0));

    int i;
    int playerNumber = 0;

    Screen *screen = new Screen;

    if (!screen->getMap().isContinuous())
    {
        std::cout << "Is not continuous, need to change code" << std::endl;
        return -1;
    }

    screen->createObstacle();

    Inocent *inocentArray = new Inocent[NUMBER_OF_INOCENTS];
    Traitor *traitorArray = new Traitor[NUMBER_OF_TRAITORS];

    for (i = 0; i < NUMBER_OF_INOCENTS; i++)
    {
        inocentArray[i].setPlayerValues(screen, playerNumber);
        inocentArray[i].drawPlayer();
        playerNumber++;
    }

    for (i = 0; i < NUMBER_OF_TRAITORS; i++)
    {
        traitorArray[i].setPlayerValues(screen, playerNumber);
        traitorArray[i].drawPlayer();
        playerNumber++;
    }
    //cv::circle(screen->getMap(), cv::Point(LENGTH / 2, HEIGHT / 2), 50, cv::Scalar(DETECTIVE_COLOR), -1);

    while (1)
    {
        screen->resetImage();
        screen->createObstacle();

        for (i = 0; i < NUMBER_OF_INOCENTS; i++)
        {
            if (inocentArray[i].isAlive())
                inocentArray[i].drawPlayer();
        }
        for (i = 0; i < NUMBER_OF_TRAITORS; i++)
        {
            if (traitorArray[i].isAlive())
                traitorArray[i].drawPlayer();
        }
        for (i = 0; i < NUMBER_OF_INOCENTS; i++)
        {
            if (inocentArray[i].isAlive())
            {
                inocentArray[i].updateVision();
                inocentArray[i].killPlayer(0, inocentArray, traitorArray, nullptr);
            }
        }

        for (i = 0; i < NUMBER_OF_TRAITORS; i++)
        {
            if (traitorArray[i].isAlive())
                traitorArray[i].updateVision();
        }
        screen->updateMap();
    }

    return 0;
}