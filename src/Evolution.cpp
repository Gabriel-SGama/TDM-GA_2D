#include <eigen3/Eigen/Dense>
#include <iostream>
#include <omp.h>
#include <time.h>
#include "headers/Evolution.h"

using namespace Eigen;

Evolution::Evolution()
{
    bestLightAssaultTeamScore = INICIAL_SCORE;
    bestSniperTeamScore = INICIAL_SCORE;
    bestAssaultTeamScore = INICIAL_SCORE;

    //best individual players
    bestIndvs = new Moderator;
    bestIndvs->setModerator(NUMBER_OF_LIGHT_ASSAULTS, NUMBER_OF_SNIPERS, NUMBER_OF_ASSAULTS);
    bestIndvs->setScreen(new Screen);
    bestIndvs->setAllPlayersValues();

    bestLightAssaultANN = new ANN;
    bestSniperANN = new ANN;
    bestAssaultANN = new ANN;

    //creates the matrix
    bestLightAssaultANN->setANNParameters(bestIndvs->getLightAssaults()->ANNInputSize, bestIndvs->getLightAssaults()->ANNOutputSize);
    bestSniperANN->setANNParameters(bestIndvs->getSnipers()->ANNInputSize, bestIndvs->getSnipers()->ANNOutputSize);
    bestAssaultANN->setANNParameters(bestIndvs->getAssaults()->ANNInputSize, bestIndvs->getAssaults()->ANNOutputSize);

    //best team players
    bestTeams = new Moderator;
    bestTeams->setModerator(NUMBER_OF_LIGHT_ASSAULTS, NUMBER_OF_SNIPERS, NUMBER_OF_ASSAULTS);
    bestTeams->setScreen(new Screen);
    bestTeams->setAllPlayersValues();

    TOTAL_NUMBER_OF_LIGHT_ASSAULTS = NUMBER_OF_LIGHT_ASSAULTS * POP_SIZE;
    TOTAL_NUMBER_OF_SNIPERS = NUMBER_OF_SNIPERS * POP_SIZE;
    TOTAL_NUMER_OF_ASSAULTS = NUMBER_OF_ASSAULTS * POP_SIZE;

    //training moderators
    lightAssaultTraining = new Moderator[POP_SIZE];
    snipersTraining = new Moderator[POP_SIZE];
    assaultsTraining = new Moderator[POP_SIZE];

    for (int i = 0; i < POP_SIZE; i++)
    {
        lightAssaultTraining[i].setModerator(NUMBER_OF_LIGHT_ASSAULTS, NUMBER_OF_SNIPERS, NUMBER_OF_ASSAULTS);
        lightAssaultTraining[i].setScreen(new Screen);
        lightAssaultTraining[i].setAllPlayersValues();

        snipersTraining[i].setModerator(NUMBER_OF_LIGHT_ASSAULTS, NUMBER_OF_SNIPERS, NUMBER_OF_ASSAULTS);
        snipersTraining[i].setScreen(new Screen);
        snipersTraining[i].setAllPlayersValues();

        assaultsTraining[i].setModerator(NUMBER_OF_LIGHT_ASSAULTS, NUMBER_OF_SNIPERS, NUMBER_OF_ASSAULTS);
        assaultsTraining[i].setScreen(new Screen);
        assaultsTraining[i].setAllPlayersValues();

        lightAssaultTraining[i].setAllWeights(nullptr, bestTeams->getSnipers(), bestTeams->getAssaults());
        snipersTraining[i].setAllWeights(bestTeams->getLightAssaults(), nullptr, bestTeams->getAssaults());
        assaultsTraining[i].setAllWeights(bestTeams->getLightAssaults(), bestTeams->getSnipers(), nullptr);
    }

    //stores relation results
    lightAssaultChilds = new ANN[TOTAL_NUMBER_OF_LIGHT_ASSAULTS];
    snipersChilds = new ANN[TOTAL_NUMBER_OF_SNIPERS];
    assaultsChilds = new ANN[TOTAL_NUMER_OF_ASSAULTS];

    //create the matrixs
    createANN(lightAssaultChilds, TOTAL_NUMBER_OF_LIGHT_ASSAULTS, bestTeams->getLightAssaults()->ANNInputSize, bestTeams->getLightAssaults()->ANNOutputSize);
    createANN(snipersChilds, TOTAL_NUMBER_OF_SNIPERS, bestTeams->getSnipers()->ANNInputSize, bestTeams->getSnipers()->ANNOutputSize);
    createANN(assaultsChilds, TOTAL_NUMER_OF_ASSAULTS, bestTeams->getAssaults()->ANNInputSize, bestTeams->getAssaults()->ANNOutputSize);

    allLightAssaults = new Player *[TOTAL_NUMBER_OF_LIGHT_ASSAULTS];
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

    int numberOfLightAssaults = 0;
    int numberOfSnipers = 0;
    int numberOfAssaults = 0;

    Player *playersPtr;

    for (i = 0; i < POP_SIZE; i++)
    {
        playersPtr = lightAssaultTraining[i].getLightAssaults();
        for (j = 0; j < lightAssaultTraining[i].NUMBER_OF_LIGHT_ASSAULT_TRAIN; j++)
        {
            allLightAssaults[numberOfLightAssaults + j] = &playersPtr[j];
        }

        numberOfLightAssaults += lightAssaultTraining[i].NUMBER_OF_LIGHT_ASSAULT_TRAIN;
    }

    for (i = 0; i < POP_SIZE; i++)
    {
        playersPtr = snipersTraining[i].getSnipers();

        for (j = 0; j < snipersTraining[i].NUMBER_OF_SNIPER_TRAIN; j++)
        {
            allSnipers[numberOfSnipers + j] = &playersPtr[j];
        }

        numberOfSnipers += snipersTraining[i].NUMBER_OF_SNIPER_TRAIN;
    }

    for (i = 0; i < POP_SIZE; i++)
    {
        playersPtr = assaultsTraining[i].getAssaults();

        for (j = 0; j < assaultsTraining[i].NUMBER_OF_ASSAULT_TRAIN; j++)
        {
            allAssaults[numberOfAssaults + j] = &playersPtr[j];
        }

        numberOfAssaults += assaultsTraining[i].NUMBER_OF_ASSAULT_TRAIN;
    }
}

void Evolution::game()
{
    int i;

#pragma omp parallel for
    for (i = 0; i < POP_SIZE; i++)
    {
        lightAssaultTraining[i].game();
        snipersTraining[i].game();
        assaultsTraining[i].game();
    }
    //select best team
    for (i = 0; i < POP_SIZE; i++)
    {
        lightAssaultTraining[i].calculateScore();
        snipersTraining[i].calculateScore();
        assaultsTraining[i].calculateScore();

        /* code */
        if (lightAssaultTraining[i].lightAssaultScore > bestLightAssaultTeamScore)
        {
            bestLightAssaultTeamScore = lightAssaultTraining[i].lightAssaultScore;
            bestLightAssaults = &lightAssaultTraining[i];
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

    bestLightAssaultTeamScore = INICIAL_SCORE;
    bestSniperTeamScore = INICIAL_SCORE;
    bestAssaultTeamScore = INICIAL_SCORE;

    //sets weigths
    bestTeams->setAllWeights(bestLightAssaults->getLightAssaults(), bestSnipers->getSnipers(), bestAssaults->getAssaults());

#pragma omp parallel for
    for (int i = 0; i < POP_SIZE; i++)
    {
        lightAssaultTraining[i].setAllWeights(nullptr, bestTeams->getSnipers(), bestTeams->getAssaults());
        snipersTraining[i].setAllWeights(bestTeams->getLightAssaults(), nullptr, bestTeams->getAssaults());
        assaultsTraining[i].setAllWeights(bestTeams->getLightAssaults(), bestTeams->getSnipers(), nullptr);

        lightAssaultTraining[i].resetAllPlayers(true);
        snipersTraining[i].resetAllPlayers(true);
        assaultsTraining[i].resetAllPlayers(true);
    }

    bestTeams->resetAllPlayers(true);
}

void Evolution::tournamentAll()
{
    tournament(allLightAssaults, TOTAL_NUMBER_OF_LIGHT_ASSAULTS, lightAssaultChilds, TOURNAMENT_K_LIGHT_ASSAULTS);
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
        for (quant = 0; quant < 20; quant++)
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
        if (lightAssaultTraining[i].bestLightAssault->score > BIS)
        {
            BIS = lightAssaultTraining[i].bestLightAssault->score;
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

    std::cout << "best light assault score: " << BIS << std::endl;
    std::cout << "best sniper score: " << BTS << std::endl;
    std::cout << "best assault score: " << BDS << std::endl;

    bestLightAssaultANN->copyWheights(lightAssaultTraining[BII].bestLightAssault->player->ann->getMatrixPtr());
    bestSniperANN->copyWheights(snipersTraining[BTI].bestSniper->player->ann->getMatrixPtr());
    bestAssaultANN->copyWheights(assaultsTraining[BDI].bestAssault->player->ann->getMatrixPtr());

    bestIndvs->setAllWeightsOneMatrix(bestLightAssaultANN->getMatrixPtr(), bestSniperANN->getMatrixPtr(), bestAssaultANN->getMatrixPtr());
}