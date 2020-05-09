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

#pragma omp parallel for num_threads(4)
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

#pragma omp parallel for num_threads(4)
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
    int bestIndex = 0;
    int secondBestIndex = 0;
    int worstIndex = 0;

    int j;

    float bestScore = INICIAL_SCORE;
    float secondBestScore = INICIAL_SCORE;
    float worstScore = -INICIAL_SCORE;

    Player *father1;
    Player *father2;

    Player *best1;
    Player *best2;

    MatrixXf *matrixArray;
    MatrixXf *matrixArrayBest1;
    MatrixXf *matrixArrayBest2;

    //get the top 2 and the worst
    for (i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        if (players[i]->getScore() > bestScore)
        {
            secondBestScore = bestScore;
            secondBestIndex = i;

            bestScore = players[i]->getScore();
            bestIndex = i;
        }
        else if (players[i]->getScore() > secondBestScore)
        {
            secondBestScore = players[i]->getScore();
            secondBestIndex = i;
        }

        if (players[i]->getScore() < worstScore)
        {
            worstScore = players[i]->getScore();
            worstIndex = i;
        }
    }

#pragma omp parallel for
    for (i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        if (i == bestIndex || i == secondBestIndex)
            continue;

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

        for (j = 0; j < layerSize + 1; j++)
        {
            matrixArray[j] = (matrixArrayBest1[j] + matrixArrayBest2[j]) / 2;
        }

        mutation(matrixArray);
    }

#pragma omp parallel for
    for (i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        if (i == bestIndex || i == secondBestIndex)
            continue;

        if (i == worstIndex)
        {
            matrixArray = childs[worstIndex].getMatrixPtr();
            matrixArrayBest1 = players[bestIndex]->ann->getMatrixPtr();
            matrixArrayBest2 = players[secondBestIndex]->ann->getMatrixPtr();

            for (j = 0; j < layerSize + 1; j++)
            {
                matrixArray[j] = (matrixArrayBest1[j] + matrixArrayBest2[j]) / 2;
            }
        }

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
    for (int i = 0; i < layerSize + 1; i++)
    {
        for (quant = 0; quant < 7; quant++)
        {
            line = rand() % matrixArray[i].rows();
            colun = rand() % matrixArray[i].cols();

            matrixArray[i](line, colun) += (rand() % (2 * 75) - 75) / 10000.0;
        }
    }
}

void quickSort(Player **players, int low, int high)
{
    int i = low;
    int j = high;
    float pivot = players[(i + j) / 2]->getScore();
    float temp;

    while (i <= j)
    {
        while (players[i]->getScore() < pivot)
            i++;

        while (players[j]->getScore() > pivot)
            j--;

        if (i <= j)
        {
            //swap
            players[j]->ann->setMatrix(players[i]->ann->setMatrix(players[j]->ann->getMatrixPtr()));

            temp = players[j]->getScore();
            players[j]->setScore(players[i]->getScore());
            players[i]->setScore(temp);

            i++;
            j--;
        }
    }

    if (j > low)
        quickSort(players, low, j);
    if (i < high)
        quickSort(players, i, high);
}

void Evolution::genocideAll()
{

    genocide(allLightAssaults, NUMBER_OF_LIGHT_ASSAULTS);
    genocide(allSnipers, NUMBER_OF_SNIPERS);
    genocide(allAssaults, NUMBER_OF_ASSAULTS);
}

void Evolution::genocide(Player **players, int NUMBER_OF_PLAYERS)
{
    quickSort(players, 0, NUMBER_OF_PLAYERS - 1);

    for (int i = 0; i < NUMBER_OF_PLAYERS / 6; i++)
    {
        players[i]->ann->reset();
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