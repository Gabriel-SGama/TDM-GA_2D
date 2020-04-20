#include <eigen3/Eigen/Dense>

#include "headers/Evolution.h"

using namespace Eigen;

Evolution::Evolution(/* args */)
{
}

Evolution::~Evolution()
{
}

void Evolution::setParam(Moderator *moderator)
{
    this->moderator = moderator;
}

void Evolution::eletism()
{
    int i;

    Inocent *inocents;
    Traitor *traitors;
    Detective *detectives;

    MatrixXf *matrixOfBest;

    inocents = moderator->getInocents();
    traitors = moderator->getTraitors();
    detectives = moderator->getDetectives();

    std::cout << "best inocent: " << moderator->bestInocent->player->getPlayerID()
              << " | score: " << moderator->bestInocent->score << std::endl;

    std::cout << "best traitor: " << moderator->bestTraitor->player->getPlayerID()
              << " | score: " << moderator->bestTraitor->score << std::endl;

    matrixOfBest = moderator->bestInocent->player->ann->getMatrixPtr();
    for (i = 0; i < NUMBER_OF_INOCENTS; i++)
    {
        inocents[i].ann->simpleBreeding(matrixOfBest);
    }

    matrixOfBest = moderator->bestTraitor->player->ann->getMatrixPtr();
    for (i = 0; i < NUMBER_OF_TRAITORS; i++)
    {
        traitors[i].ann->simpleBreeding(matrixOfBest);
    }
}