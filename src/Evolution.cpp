#include "headers/Evolution.h"

using namespace Eigen;

Evolution::Evolution()
{
    bestInocentTeamScore = -100000;
    bestTraitorTeamScore = -100000;

    //moderators = new Moderator[POP_SIZE];

    bestPlayers = new Moderator[POP_SIZE];
    bestPlayers->setScreen(new Screen);
    //bestPlayers->screen->setScreenParam("best players game");
    bestPlayers->setAllPlayersValues();

    inocentsTraining = new Moderator[POP_SIZE];
    traitorsTraining = new Moderator[POP_SIZE];

    for (int i = 0; i < POP_SIZE; i++)
    {

        inocentsTraining[i].setScreen(new Screen);
        inocentsTraining[i].setAllPlayersValues();

        traitorsTraining[i].setScreen(new Screen);
        traitorsTraining[i].setAllPlayersValues();

        inocentsTraining[i].setAllWeights(bestPlayers->getInocents(), nullptr, bestPlayers->getDetectives());
        traitorsTraining[i].setAllWeights(nullptr, bestPlayers->getTraitors(), nullptr);

        //moderators[i].setScreen(new Screen);
        //moderators[i].setAllPlayersValues();
    }

    inocentsChilds = new ANN[TOTAL_INOCENTS];
    traitorsChilds = new ANN[TOTAL_TRAITORS];
    detectivesChilds = new ANN[TOTAL_DETECTIVES];

    //input and output size are egual to everyone
    createANN(inocentsChilds, TOTAL_INOCENTS, bestPlayers->getInocents()->ANNInputSize, bestPlayers->getInocents()->ANNOutputSize);
    createANN(traitorsChilds, TOTAL_TRAITORS, bestPlayers->getTraitors()->ANNInputSize, bestPlayers->getTraitors()->ANNOutputSize);
    createANN(detectivesChilds, TOTAL_DETECTIVES, bestPlayers->getDetectives()->ANNInputSize, bestPlayers->getDetectives()->ANNOutputSize);

    allInocents = new Player *[TOTAL_INOCENTS];
    allTraitors = new Player *[TOTAL_TRAITORS];
    allDetectives = new Player *[TOTAL_DETECTIVES];

    setPlayersPtr();
}

Evolution::~Evolution()
{
}

/*
void Evolution::setParam(Moderator *moderators)
{
    this->moderators = moderators;

    inocentsChilds = new ANN[NUMBER_OF_INOCENTS];
    traitorsChilds = new ANN[NUMBER_OF_TRAITORS];
    detectivesChilds = new ANN[NUMBER_OF_DETECTIVES];

    createANN(inocentsChilds, NUMBER_OF_INOCENTS, moderators->getInocents()->ANNInputSize, moderators->getInocents()->ANNOutputSize);
    createANN(traitorsChilds, NUMBER_OF_TRAITORS, moderators->getTraitors()->ANNInputSize, moderators->getTraitors()->ANNOutputSize);
    createANN(detectivesChilds, NUMBER_OF_DETECTIVES, moderators->getDetectives()->ANNInputSize, moderators->getDetectives()->ANNOutputSize);
}
*/
void Evolution::createANN(ANN *childs, int NUMBER_OF_PLAYERS, int inputSize, int outputSize)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        childs[i].setANNParameters(inputSize, outputSize);
    }
}

void Evolution::setPlayersPtr()
{

    int i;
    int j;

    Player *playersPtr;

    for (i = 0; i < POP_SIZE; i++)
    {
        playersPtr = inocentsTraining[i].getInocents();
        for (j = 0; j < NUMBER_OF_INOCENTS; j++)
        {
            allInocents[i * NUMBER_OF_INOCENTS + j] = &playersPtr[j];
        }
    }

    for (i = 0; i < POP_SIZE; i++)
    {
        playersPtr = traitorsTraining[i].getTraitors();
        for (j = 0; j < NUMBER_OF_TRAITORS; j++)
        {
            allTraitors[i * NUMBER_OF_TRAITORS + j] = &playersPtr[j];
        }
    }

    for (i = 0; i < POP_SIZE; i++)
    {
        playersPtr = inocentsTraining[i].getDetectives();
        for (j = 0; j < NUMBER_OF_DETECTIVES; j++)
        {
            allDetectives[i * NUMBER_OF_DETECTIVES + j] = &playersPtr[j];
        }
    }
}

void Evolution::game()
{
    int i;

    for (i = 0; i < POP_SIZE; i++)
    {
        //moderators[i].game();
        inocentsTraining[i].game();
        traitorsTraining[i].game();

        inocentsTraining[i].calculateScore();
        traitorsTraining[i].calculateScore();
        //moderators[i].calculateScore();
        if (inocentsTraining[i].inocentsScore > bestInocentTeamScore)
        {
            bestInocentTeamScore = inocentsTraining[i].inocentsScore;
            bestInocents = &inocentsTraining[i];
        }

        if (traitorsTraining[i].traitorScore > bestTraitorTeamScore)
        {
            bestTraitorTeamScore = traitorsTraining[i].traitorScore;
            bestTraitors = &traitorsTraining[i];
        }
    }
}

void Evolution::reset()
{
    bestInocentTeamScore = -100000;
    bestTraitorTeamScore = -100000;

    bestPlayers->setAllWeights(bestInocents->getInocents(), bestTraitors->getTraitors(), bestInocents->getDetectives());

    for (int i = 0; i < POP_SIZE; i++)
    {
        //moderators[i].resetAllPlayers(true);

        inocentsTraining[i].setAllWeights(bestPlayers->getInocents(), nullptr, bestPlayers->getDetectives());
        traitorsTraining[i].setAllWeights(nullptr, bestPlayers->getTraitors(), nullptr);

        inocentsTraining[i].resetAllPlayers(true);
        traitorsTraining[i].resetAllPlayers(true);
    }

    bestPlayers->resetAllPlayers(true);
}
/*
void Evolution::eletismAll()
{
    Inocent *inocents;
    Traitor *traitors;
    Detective *detectives;

    MatrixXf *matrixOfBest;

    inocents = moderators->getInocents();
    traitors = moderators->getTraitors();
    detectives = moderators->getDetectives();

    matrixOfBest = moderators->bestInocent->player->ann->getMatrixPtr();
    eletism(inocents, NUMBER_OF_INOCENTS, matrixOfBest);

    matrixOfBest = moderators->bestTraitor->player->ann->getMatrixPtr();
    eletism(traitors, NUMBER_OF_TRAITORS, matrixOfBest);

    matrixOfBest = moderators->bestDetective->player->ann->getMatrixPtr();
    eletism(detectives, NUMBER_OF_DETECTIVES, matrixOfBest);
}
*/
void Evolution::eletism(Player *players, int NUMBER_OF_PLAYERS, MatrixXf *matrixOfBest)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        players[i].ann->simpleBreeding(matrixOfBest);
        mutation(players[i].ann->getMatrixPtr());
    }
}

void Evolution::tournamentAll()
{
    tournament(allInocents, TOTAL_INOCENTS, inocentsChilds, TOUTNAMENT_K_INOCENTS /*moderators->bestInocent->index*/);
    tournament(allTraitors, TOTAL_TRAITORS, traitorsChilds, TOUTNAMENT_K_TRAITORS /*moderators->bestTraitor->index*/);
    tournament(allDetectives, TOTAL_DETECTIVES, detectivesChilds, TOUTNAMENT_K_DETECTIVES /*moderators->bestDetective->index*/);
}

void Evolution::tournament(Player **players, int NUMBER_OF_PLAYERS, ANN *childs, int TOURNAMENT_K)
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

        father1 = players[rand() % NUMBER_OF_PLAYERS];

        for (j = 0; j < TOURNAMENT_K; j++)
        {
            father2 = players[rand() % NUMBER_OF_PLAYERS];
            if (father2->getScore() > father1->getScore())
                father1 = father2;
        }
        best1 = father1;

        father1 = players[rand() % NUMBER_OF_PLAYERS];

        for (j = 0; j < TOURNAMENT_K; j++)
        {
            father2 = players[rand() % NUMBER_OF_PLAYERS];
            if (father2->getScore() > father1->getScore())
                father1 = father2;
        }
        best2 = father1;

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
        //if (i == indexOfBest)
        //    continue;

        childs[i].setMatrix(players[i]->ann->setMatrix(childs[i].getMatrixPtr()));
    }

    // std::cout << "best inocent: " << moderators->bestInocent->player->getPlayerID()
    //           << " | score: " << moderators->bestInocent->score << std::endl;

    // std::cout << "best traitor: " << moderators->bestTraitor->player->getPlayerID()
    //           << " | score: " << moderators->bestTraitor->score << std::endl;
}

void Evolution::mutation(MatrixXf *matrixArray)
{
    int quant;
    int line;
    int colun;

    for (unsigned int i = 0; i < layers.size() + 1; i++)
    {
        for (quant = 0; quant < 7; quant++)
        {
            line = rand() % matrixArray[i].rows();
            colun = rand() % matrixArray[i].cols();

            matrixArray[i](line, colun) += (rand() % (2 * 750) - 750) / 10000.0;
        }
    }
}