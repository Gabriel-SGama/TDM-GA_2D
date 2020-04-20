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

    inocentsChilds = new ANN[NUMBER_OF_INOCENTS];
    traitorsChilds = new ANN[NUMBER_OF_TRAITORS];
    detectivesChilds = new ANN[NUMBER_OF_DETECTIVES];

    createANN(inocentsChilds, NUMBER_OF_INOCENTS, moderator->getInocents()->ANNInputSize, moderator->getInocents()->ANNOutputSize);
    createANN(traitorsChilds, NUMBER_OF_TRAITORS, moderator->getTraitors()->ANNInputSize, moderator->getTraitors()->ANNOutputSize);
    createANN(detectivesChilds, NUMBER_OF_DETECTIVES, moderator->getDetectives()->ANNInputSize, moderator->getDetectives()->ANNOutputSize);
}

void Evolution::createANN(ANN *childs, int NUMBER_OF_PLAYERS, int inputSize, int outputSize)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        childs[i].setANNParameters(inputSize, outputSize);
    }
}

void Evolution::eletismAll()
{
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
    eletism(inocents, NUMBER_OF_INOCENTS, matrixOfBest);

    matrixOfBest = moderator->bestTraitor->player->ann->getMatrixPtr();
    eletism(traitors, NUMBER_OF_TRAITORS, matrixOfBest);

    matrixOfBest = moderator->bestDetective->player->ann->getMatrixPtr();
    eletism(detectives, NUMBER_OF_DETECTIVES, matrixOfBest);
}

void Evolution::eletism(Player *players, int NUMBER_OF_PLAYERS, MatrixXf *matrixOfBest)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        players[i].ann->simpleBreeding(matrixOfBest);
    }
}

void Evolution::tournamentAll()
{
    tournament(moderator->getInocents(), NUMBER_OF_INOCENTS, inocentsChilds, moderator->bestInocent->index);
    tournament(moderator->getTraitors(), NUMBER_OF_TRAITORS, traitorsChilds, moderator->bestTraitor->index);
    tournament(moderator->getDetectives(), NUMBER_OF_DETECTIVES, detectivesChilds, moderator->bestDetective->index);
}

void Evolution::tournament(Player *players, int NUMBER_OF_PLAYERS, ANN *childs, int indexOfBest)
{
    int i;
    unsigned int j;

    Player *father1;
    Player *father2;

    Player *best1;
    Player *best2;

    MatrixXf *matrixArray;
    MatrixXf *matrixArrayBest1;
    MatrixXf *matrixArrayBest2;

    for (i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        matrixArray = childs[i].getMatrixPtr();

        father1 = &players[rand() % NUMBER_OF_PLAYERS];
        father2 = &players[rand() % NUMBER_OF_PLAYERS];

        if (father1->getScore() > father2->getScore())
            best1 = father1;
        else
            best1 = father2;

        father1 = &players[rand() % NUMBER_OF_PLAYERS];
        father2 = &players[rand() % NUMBER_OF_PLAYERS];

        if (father1->getScore() > father2->getScore())
            best2 = father1;
        else
            best2 = father2;

        matrixArrayBest1 = best1->ann->getMatrixPtr();
        matrixArrayBest2 = best2->ann->getMatrixPtr();

        for (j = 0; j < layers.size() + 1; j++)
        {
            matrixArray[j] = (matrixArrayBest1[j] + matrixArrayBest2[j]) / 2;
        }

        mutation(matrixArray);
    }

    for (i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        if (i == indexOfBest)
            childs[i].setMatrix(players[i].ann->setMatrix(childs[i].getMatrixPtr()));
    }

    // std::cout << "best inocent: " << moderator->bestInocent->player->getPlayerID()
    //           << " | score: " << moderator->bestInocent->score << std::endl;

    // std::cout << "best traitor: " << moderator->bestTraitor->player->getPlayerID()
    //           << " | score: " << moderator->bestTraitor->score << std::endl;
}

void Evolution::mutation(MatrixXf *matrixArray)
{
    int quant;
    int line;
    int colun;

    for (unsigned int i = 0; i < layers.size() + 1; i++)
    {
        for (quant = 0; quant < 5; quant++)
        {
            line = rand() % matrixArray[i].rows();
            colun = rand() % matrixArray[i].cols();

            matrixArray[i](line, colun) += ((rand() % 2 * 500) - 500) / 10000.0;
        }
    }
}