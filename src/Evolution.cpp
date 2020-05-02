#include <eigen3/Eigen/Dense>
#include <iostream>
#include <omp.h>
#include <time.h>
#include "headers/Evolution.h"

using namespace Eigen;

Evolution::Evolution()
{
    bestInocentTeamScore = INICIAL_SCORE;
    bestSniperTeamScore = INICIAL_SCORE;
    bestAssaultTeamScore = INICIAL_SCORE;

    //best individual players
    bestIndvs = new Moderator;
    bestIndvs->setModerator(NUMBER_OF_INOCENTS, NUMBER_OF_SNIPERS, NUMBER_OF_ASSAULTS);
    bestIndvs->setScreen(new Screen);
    bestIndvs->setAllPlayersValues();

    bestInocentANN = new ANN;
    bestSniperANN = new ANN;
    bestAssaultANN = new ANN;

    //creates the matrix
    bestInocentANN->setANNParameters(bestIndvs->getInocents()->ANNInputSize, bestIndvs->getInocents()->ANNOutputSize);
    bestSniperANN->setANNParameters(bestIndvs->getSnipers()->ANNInputSize, bestIndvs->getSnipers()->ANNOutputSize);
    bestAssaultANN->setANNParameters(bestIndvs->getAssaults()->ANNInputSize, bestIndvs->getAssaults()->ANNOutputSize);

    //best team players
    bestTeams = new Moderator;
    bestTeams->setModerator(NUMBER_OF_INOCENTS, NUMBER_OF_SNIPERS, NUMBER_OF_ASSAULTS);
    bestTeams->setScreen(new Screen);
    bestTeams->setAllPlayersValues();

    TOTAL_NUMBER_OF_INOCENTS = NUMBER_OF_INOCENTS * POP_SIZE;
    TOTAL_NUMBER_OF_SNIPERS = NUMBER_OF_SNIPERS * POP_SIZE;
    TOTAL_NUMER_OF_ASSAULTS = ASSAULT_POP_ASS_TRAIN * POP_SIZE;

    //training moderators
    inocentsTraining = new Moderator[POP_SIZE];
    snipersTraining = new Moderator[POP_SIZE];
    assaultsTraining = new Moderator[POP_SIZE];

    for (int i = 0; i < POP_SIZE; i++)
    {
        inocentsTraining[i].setModerator(NUMBER_OF_INOCENTS, NUMBER_OF_SNIPERS, NUMBER_OF_ASSAULTS);
        inocentsTraining[i].setScreen(new Screen);
        inocentsTraining[i].setAllPlayersValues();

        snipersTraining[i].setModerator(NUMBER_OF_INOCENTS, NUMBER_OF_SNIPERS, NUMBER_OF_ASSAULTS);
        snipersTraining[i].setScreen(new Screen);
        snipersTraining[i].setAllPlayersValues();

        assaultsTraining[i].setModerator(INOCENTS_POP_DET_TRAIN, SNIPER_POP_DET_TRAIN, ASSAULT_POP_ASS_TRAIN);
        assaultsTraining[i].setScreen(new Screen);
        assaultsTraining[i].setAllPlayersValues();

        inocentsTraining[i].setAllWeights(nullptr, bestTeams->getSnipers(), bestTeams->getAssaults());
        snipersTraining[i].setAllWeights(bestTeams->getInocents(), nullptr, bestTeams->getAssaults());
        assaultsTraining[i].setAllWeights(bestTeams->getInocents(), bestTeams->getSnipers(), nullptr);
    }

    //stores relation results
    inocentsChilds = new ANN[TOTAL_NUMBER_OF_INOCENTS];
    snipersChilds = new ANN[TOTAL_NUMBER_OF_SNIPERS];
    assaultsChilds = new ANN[TOTAL_NUMER_OF_ASSAULTS];

    //create the matrixs
    createANN(inocentsChilds, TOTAL_NUMBER_OF_INOCENTS, bestTeams->getInocents()->ANNInputSize, bestTeams->getInocents()->ANNOutputSize);
    createANN(snipersChilds, TOTAL_NUMBER_OF_SNIPERS, bestTeams->getSnipers()->ANNInputSize, bestTeams->getSnipers()->ANNOutputSize);
    createANN(assaultsChilds, TOTAL_NUMER_OF_ASSAULTS, bestTeams->getAssaults()->ANNInputSize, bestTeams->getAssaults()->ANNOutputSize);

    allInocents = new Player *[TOTAL_NUMBER_OF_INOCENTS];
    allSnipers = new Player *[TOTAL_NUMBER_OF_SNIPERS];
    allAssaults = new Player *[TOTAL_NUMER_OF_ASSAULTS];

    //sets players ptr
    setPlayersPtr();
}

Evolution::~Evolution()
{
}

void Evolution::createANN(ANN *childs, int NUMBER_OF_PLAYERS, int inputSize, int outputSize)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
        childs[i].setANNParameters(inputSize, outputSize);
}

void Evolution::setPlayersPtr()
{
    int i;
    int j;

    int numberOfInocents = 0;
    int numberOfTraitors = 0;
    int numberOfDetectives = 0;

    Player *playersPtr;

    for (i = 0; i < POP_SIZE; i++)
    {
        playersPtr = inocentsTraining[i].getInocents();
        for (j = 0; j < inocentsTraining[i].NUMBER_OF_INOCENT_TRAIN; j++)
        {
            allInocents[numberOfInocents + j] = &playersPtr[j];
        }

        numberOfInocents += inocentsTraining[i].NUMBER_OF_INOCENT_TRAIN;
    }

    for (i = 0; i < POP_SIZE; i++)
    {
        playersPtr = snipersTraining[i].getSnipers();

        for (j = 0; j < snipersTraining[i].NUMBER_OF_SNIPER_TRAIN; j++)
        {
            allSnipers[numberOfTraitors + j] = &playersPtr[j];
        }

        numberOfTraitors += snipersTraining[i].NUMBER_OF_SNIPER_TRAIN;
    }

    for (i = 0; i < POP_SIZE; i++)
    {
        playersPtr = assaultsTraining[i].getAssaults();

        for (j = 0; j < assaultsTraining[i].NUMBER_OF_ASSAULT_TRAIN; j++)
        {
            allAssaults[numberOfDetectives + j] = &playersPtr[j];
        }

        numberOfDetectives += assaultsTraining[i].NUMBER_OF_ASSAULT_TRAIN;
    }
}

void Evolution::game()
{
    int i;

#pragma omp parallel for
    for (i = 0; i < POP_SIZE; i++)
    {
        inocentsTraining[i].game();
        snipersTraining[i].game();
        assaultsTraining[i].game();
    }
    //select best team
    for (i = 0; i < POP_SIZE; i++)
    {
        inocentsTraining[i].calculateScore();
        snipersTraining[i].calculateScore();
        assaultsTraining[i].calculateScore();

        /* code */
        if (inocentsTraining[i].inocentScore > bestInocentTeamScore)
        {
            bestInocentTeamScore = inocentsTraining[i].inocentScore;
            bestInocents = &inocentsTraining[i];
        }

        if (assaultsTraining[i].assaultScore > bestAssaultTeamScore)
        {
            bestAssaultTeamScore = assaultsTraining[i].assaultScore;
            bestAssaults = &assaultsTraining[i];
        }

        if (snipersTraining[i].sniperScore > bestSniperTeamScore)
        {
            bestSniperTeamScore = snipersTraining[i].sniperScore;
            bestSnipers = &snipersTraining[i];
        }
    }
}

void Evolution::reset()
{
    if (bestAssaultTeamScore <= INICIAL_SCORE)
        bestAssaults = assaultsTraining;

    bestInocentTeamScore = INICIAL_SCORE;
    bestSniperTeamScore = INICIAL_SCORE;
    bestAssaultTeamScore = INICIAL_SCORE;

    //sets weigths
    bestTeams->setAllWeights(bestInocents->getInocents(), bestSnipers->getSnipers(), bestAssaults->getAssaults());

#pragma omp parallel for
    for (int i = 0; i < POP_SIZE; i++)
    {
        inocentsTraining[i].setAllWeights(nullptr, bestTeams->getSnipers(), bestTeams->getAssaults());
        snipersTraining[i].setAllWeights(bestTeams->getInocents(), nullptr, bestTeams->getAssaults());
        assaultsTraining[i].setAllWeights(bestTeams->getInocents(), bestTeams->getSnipers(), nullptr);

        inocentsTraining[i].resetAllPlayers(true);
        snipersTraining[i].resetAllPlayers(true);
        assaultsTraining[i].resetAllPlayers(true);
    }

    bestTeams->resetAllPlayers(true);
}

void Evolution::tournamentAll()
{
    tournament(allInocents, TOTAL_NUMBER_OF_INOCENTS, inocentsChilds, TOURNAMENT_K_LIGHT_ASSAULTS);
    tournament(allSnipers, TOTAL_NUMBER_OF_SNIPERS, snipersChilds, TOURNAMENT_K_SNIPERS);
    tournament(allAssaults, TOTAL_NUMER_OF_ASSAULTS, assaultsChilds, TOURNAMENT_K_ASSAULTS);
}

void Evolution::tournament(Player **players, int NUMBER_OF_PLAYERS, ANN *childs, int TOURNAMENT_K)
{
    int i;
    unsigned int j;

    float score = INICIAL_SCORE;

    Player *father1;
    Player *father2;

    Player *best1;
    Player *best2;

    MatrixXf *matrixArray;
    MatrixXf *matrixArrayBest1;
    MatrixXf *matrixArrayBest2;

#pragma omp parallel for
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

        if (best1->getScore() > score)
            score = best1->getScore();

        father1 = players[rand() % NUMBER_OF_PLAYERS];

        for (j = 0; j < TOURNAMENT_K; j++)
        {
            father2 = players[rand() % NUMBER_OF_PLAYERS];
            if (father2->getScore() > father1->getScore())
                father1 = father2;
        }
        best2 = father1;

        if (best2->getScore() > score)
            score = best2->getScore();

        matrixArrayBest1 = best1->ann->getMatrixPtr();
        matrixArrayBest2 = best2->ann->getMatrixPtr();

        for (j = 0; j < layers.size() + 1; j++)
        {
            matrixArray[j] = (matrixArrayBest1[j] + matrixArrayBest2[j]) / 2;
        }

        mutation(matrixArray);
    }
    
#pragma omp parallel for
    for (i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        if (players[i]->getScore() >= score)
            continue;

        //changes ptr
        childs[i].setMatrix(players[i]->ann->setMatrix(childs[i].getMatrixPtr()));
    }
}

void Evolution::mutation(MatrixXf *matrixArray)
{
    int quant;
    int line;
    int colun;

    //simple mutation
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

void Evolution::setBestIndvs()
{
    int BIS = INICIAL_SCORE;
    int BII = 0;

    int BTS = INICIAL_SCORE;
    int BTI = 0;

    int BDS = INICIAL_SCORE;
    int BDI = 0;

    //set best individuals players
    for (int i = 0; i < POP_SIZE; i++)
    {
        if (inocentsTraining[i].bestInocent->score > BIS)
        {
            BIS = inocentsTraining[i].bestInocent->score;
            BII = i;
        }

        if (assaultsTraining[i].bestAssault->score > BDS)
        {
            BDS = assaultsTraining[i].bestAssault->score;
            BDI = i;
        }

        if (snipersTraining[i].bestSniper->score > BTS)
        {
            BTS = snipersTraining[i].bestSniper->score;
            BTI = i;
        }
    }

    std::cout << "best inocent score: " << BIS << std::endl;
    std::cout << "best traitor score: " << BTS << std::endl;
    std::cout << "best detective score: " << BDS << std::endl;

    bestInocentANN->copyWheights(inocentsTraining[BII].bestInocent->player->ann->getMatrixPtr());
    bestSniperANN->copyWheights(snipersTraining[BTI].bestSniper->player->ann->getMatrixPtr());
    bestAssaultANN->copyWheights(assaultsTraining[BDI].bestAssault->player->ann->getMatrixPtr());

    bestIndvs->setAllWeightsOneMatrix(bestInocentANN->getMatrixPtr(), bestSniperANN->getMatrixPtr(), bestAssaultANN->getMatrixPtr());
}