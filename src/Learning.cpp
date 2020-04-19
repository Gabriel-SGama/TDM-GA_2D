#include <eigen3/Eigen/Dense>

#include "headers/Learning.h"

using namespace Eigen;

Learning::Learning()
{
}

Learning::~Learning()
{
}

void Learning::setData(Moderator *inocentsTraining, Moderator *traitorsTraining, dataOfBest_t *dataOfBest)
{
    this->dataOfBest = dataOfBest;
    this->inocentsTraining = inocentsTraining;
    this->traitorsTraining = traitorsTraining;
}

void Learning::eletism()
{
    int i;
    int j;

    Inocent *bestInocent = (Inocent *)inocentsTraining[dataOfBest->bestInocentIndex].bestInocent->player;
    Traitor *bestTraitor = (Traitor *)traitorsTraining[dataOfBest->bestTraitorIndex].bestTraitor->player;
    Detective *bestDetective = (Detective *)inocentsTraining[dataOfBest->bestInocentIndex].bestDetective->player;

    MatrixXf *bestInocentMatrix = bestInocent->ann->getMatrixPtr();
    MatrixXf *bestTraitorMatrix = bestTraitor->ann->getMatrixPtr();
    MatrixXf *bestDetectiveMatrix = bestDetective->ann->getMatrixPtr();

    Player *players;

    for (i = 0; i < POP_SIZE; i++)
    {
        players = inocentsTraining[i].getInocents();
        for (j = 0; j < NUMBER_OF_INOCENTS; j++)
        {
            players[j].ann->simpleBreeding(bestInocentMatrix);
        }

        players = traitorsTraining[i].getTraitors();
        for (j = 0; j < NUMBER_OF_TRAITORS; j++)
        {
            players[j].ann->simpleBreeding(bestTraitorMatrix);
        }

        players = inocentsTraining[i].getDetectives();
        for (j = 0; j < NUMBER_OF_DETECTIVES; j++)
        {
            players[j].ann->simpleBreeding(bestDetectiveMatrix);
        }
    }
}

void Learning::tournament()
{
}
