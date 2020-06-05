#include "headers/Evolution.h"

#include <omp.h>
#include <time.h>

#include <iostream>

using namespace Eigen;

Evolution::Evolution() {
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

    for (int i = 0; i < POP_SIZE; i++) {
        lightAssaultTraining[i].setModerator(NUMBER_OF_LIGHT_ASSAULTS, NUMBER_OF_SNIPERS, NUMBER_OF_ASSAULTS);
        lightAssaultTraining[i].setScreen(new Screen);
        lightAssaultTraining[i].setAllPlayersValues();

        snipersTraining[i].setModerator(NUMBER_OF_LIGHT_ASSAULTS, NUMBER_OF_SNIPERS, NUMBER_OF_ASSAULTS);
        snipersTraining[i].setScreen(new Screen);
        snipersTraining[i].setAllPlayersValues();

        assaultsTraining[i].setModerator(NUMBER_OF_LIGHT_ASSAULTS, NUMBER_OF_SNIPERS, NUMBER_OF_ASSAULTS);
        assaultsTraining[i].setScreen(new Screen);
        assaultsTraining[i].setAllPlayersValues();

        lightAssaultTraining[i].setAllWeightsMod(nullptr, bestTeams->sniperANN, bestTeams->assaultANN);
        snipersTraining[i].setAllWeightsMod(bestTeams->lightAssaultANN, nullptr, bestTeams->assaultANN);
        assaultsTraining[i].setAllWeightsMod(bestTeams->lightAssaultANN, bestTeams->sniperANN, nullptr);
    }

    lightAssaultChilds = new ANN[POP_SIZE];
    snipersChilds = new ANN[POP_SIZE];
    assaultsChilds = new ANN[POP_SIZE];

    //create the matrixs for the childs
    createANN(lightAssaultChilds, POP_SIZE, bestTeams->getLightAssaults()->ANNInputSize, bestTeams->getLightAssaults()->ANNOutputSize);
    createANN(snipersChilds, POP_SIZE, bestTeams->getSnipers()->ANNInputSize, bestTeams->getSnipers()->ANNOutputSize);
    createANN(assaultsChilds, POP_SIZE, bestTeams->getAssaults()->ANNInputSize, bestTeams->getAssaults()->ANNOutputSize);

    allLightAssaults = new Player *[TOTAL_NUMBER_OF_LIGHT_ASSAULTS];
    allSnipers = new Player *[TOTAL_NUMBER_OF_SNIPERS];
    allAssaults = new Player *[TOTAL_NUMER_OF_ASSAULTS];

    //sets players ptr
    setPlayersPtr();
}

Evolution::~Evolution() {
}

void Evolution::createANN(ANN *childs, int NUMBER_OF_PLAYERS, int inputSize, int outputSize) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
        childs[i].setANNParameters(inputSize, outputSize);
}

void Evolution::setPlayersPtr() {
    int i;
    int j;

    int numberOfLightAssaults = 0;
    int numberOfSnipers = 0;
    int numberOfAssaults = 0;

    Player *playersPtr;

    for (i = 0; i < POP_SIZE; i++) {
        playersPtr = lightAssaultTraining[i].getLightAssaults();
        for (j = 0; j < lightAssaultTraining[i].NUMBER_OF_LIGHT_ASSAULT_TRAIN; j++) {
            allLightAssaults[numberOfLightAssaults + j] = &playersPtr[j];
        }

        numberOfLightAssaults += lightAssaultTraining[i].NUMBER_OF_LIGHT_ASSAULT_TRAIN;
    }

    for (i = 0; i < POP_SIZE; i++) {
        playersPtr = snipersTraining[i].getSnipers();

        for (j = 0; j < snipersTraining[i].NUMBER_OF_SNIPER_TRAIN; j++) {
            allSnipers[numberOfSnipers + j] = &playersPtr[j];
        }

        numberOfSnipers += snipersTraining[i].NUMBER_OF_SNIPER_TRAIN;
    }

    for (i = 0; i < POP_SIZE; i++) {
        playersPtr = assaultsTraining[i].getAssaults();

        for (j = 0; j < assaultsTraining[i].NUMBER_OF_ASSAULT_TRAIN; j++) {
            allAssaults[numberOfAssaults + j] = &playersPtr[j];
        }
        numberOfAssaults += assaultsTraining[i].NUMBER_OF_ASSAULT_TRAIN;
    }
}

void Evolution::game() {
    int i;

#pragma omp parallel for
    //game
    for (i = 0; i < POP_SIZE; i++) {
        lightAssaultTraining[i].game();
        snipersTraining[i].game();
        assaultsTraining[i].game();
    }

    //select best team
    for (i = 0; i < POP_SIZE; i++) {
        lightAssaultTraining[i].calculateScore();
        snipersTraining[i].calculateScore();
        assaultsTraining[i].calculateScore();

        if (lightAssaultTraining[i].lightAssaultScore > bestLightAssaultTeamScore) {
            bestLightAssaultTeamScore = lightAssaultTraining[i].lightAssaultScore;
            bestLightAssaults = &lightAssaultTraining[i];
        }

        if (assaultsTraining[i].assaultScore > bestAssaultTeamScore) {
            bestAssaultTeamScore = assaultsTraining[i].assaultScore;
            bestAssaults = &assaultsTraining[i];
        }

        if (snipersTraining[i].sniperScore > bestSniperTeamScore) {
            bestSniperTeamScore = snipersTraining[i].sniperScore;
            bestSnipers = &snipersTraining[i];
        }
    }
}

void Evolution::reset() {
    bestLightAssaultTeamScore = INICIAL_SCORE;
    bestSniperTeamScore = INICIAL_SCORE;
    bestAssaultTeamScore = INICIAL_SCORE;

    //sets weigths
    bestTeams->setAllWeightsMod(bestLightAssaults->lightAssaultANN, bestSnipers->sniperANN, bestAssaults->assaultANN);

    cv::Point initialPos[] = {cv::Point(LENGTH-300,HEIGHT-250), cv::Point(LENGTH-400,150), cv::Point(0,150)};
    int posIndex = rand()%3;

#pragma omp parallel for
    for (int i = 0; i < POP_SIZE; i++) {
        lightAssaultTraining[i].setInicialPosAll(initialPos, posIndex);
        snipersTraining[i].setInicialPosAll(initialPos, posIndex);
        assaultsTraining[i].setInicialPosAll(initialPos, posIndex);

        lightAssaultTraining[i].setAllWeightsMod(nullptr, bestSnipers->sniperANN, bestAssaults->assaultANN);
        snipersTraining[i].setAllWeightsMod(bestLightAssaults->lightAssaultANN, nullptr, bestAssaults->assaultANN);
        assaultsTraining[i].setAllWeightsMod(bestLightAssaults->lightAssaultANN, bestSnipers->sniperANN, nullptr);

        lightAssaultTraining[i].resetAllPlayers(true);
        snipersTraining[i].resetAllPlayers(true);
        assaultsTraining[i].resetAllPlayers(true);
    }

    bestTeams->resetAllPlayers(true);
}

void Evolution::tournamentAllMod() {
    tournamentMod(lightAssaultTraining, lightAssaultChilds, TOURNAMENT_K_LIGHT_ASSAULTS, LIGHT_ASSAULT);
    tournamentMod(snipersTraining, snipersChilds, TOURNAMENT_K_SNIPERS, SNIPER);
    tournamentMod(assaultsTraining, assaultsChilds, TOURNAMENT_K_ASSAULTS, ASSAULT);

}

void Evolution::tournamentMod(Moderator *training, ANN *childs, int TOURNAMENT_K, int id) {
    int i;
    int j;

    Moderator *father1;
    Moderator *father2;

    Moderator *best1;
    Moderator *best2;

    MatrixXf *matrixArray;
    MatrixXf *matrixArrayBest1;
    MatrixXf *matrixArrayBest2;

    int bestIndex = 0;
    float score = INICIAL_SCORE;

    //get the top 2 and the worst
    for (i = 0; i < POP_SIZE; i++) {
        if (training[i].getScore(id) > score) {
            score = training[i].getScore(id);
            bestIndex = i;
        }
    }

#pragma omp parallel for
    for (i = 0; i < POP_SIZE; i++) {
        if (i == bestIndex)
            continue;

        matrixArray = childs[i].getMatrixPtr();

        father1 = &training[rand() % POP_SIZE];

        for (j = 0; j < TOURNAMENT_K; j++) {
            father2 = &training[rand() % POP_SIZE];
            if (father2->getScore(id) > father1->getScore(id))
                father1 = father2;
        }
        best1 = father1;

        father1 = &training[rand() % POP_SIZE];

        for (j = 0; j < TOURNAMENT_K; j++) {
            father2 = &training[rand() % POP_SIZE];
            if (father2->getScore(id) > father1->getScore(id))
                father1 = father2;
        }
        best2 = father1;

        matrixArrayBest1 = best1->getMatrixPtr(id);
        matrixArrayBest2 = best2->getMatrixPtr(id);

        // matrixArrayBest1 = training[bestIndex].getMatrixPtr(id);
        // matrixArrayBest2 = training[i].getMatrixPtr(id);

        for (j = 0; j < layerSize + 1; j++) {
            matrixArray[j] = (matrixArrayBest1[j] + matrixArrayBest2[j]) / 2;
        }

        mutation(matrixArray);
    }

    if (id == LIGHT_ASSAULT) {
#pragma omp parallel for
        for (i = 0; i < POP_SIZE; i++) {
            if (i == bestIndex)
                continue;
            //changes ptr
            childs[i].setMatrix(lightAssaultTraining[i].lightAssaultANN->setMatrix(childs[i].getMatrixPtr()));
        }
    } else if (id == SNIPER) {
#pragma omp parallel for
        for (i = 0; i < POP_SIZE; i++) {
            if (i == bestIndex)
                continue;
            //changes ptr
            childs[i].setMatrix(snipersTraining[i].sniperANN->setMatrix(childs[i].getMatrixPtr()));
        }
    } else {
#pragma omp parallel for
        for (i = 0; i < POP_SIZE; i++) {
            if (i == bestIndex)
                continue;
            //changes ptr
            childs[i].setMatrix(assaultsTraining[i].assaultANN->setMatrix(childs[i].getMatrixPtr()));
        }
    }
}

void Evolution::mutation(MatrixXf *matrixArray) {
    int quant;
    int line;
    int colun;

    //simple mutation
    for (int i = 0; i < layerSize + 1; i++) {
        for (quant = 0; quant < 40; quant++) {
            line = rand() % matrixArray[i].rows();
            colun = rand() % matrixArray[i].cols();

            matrixArray[i](line, colun) += (rand() % (2 * 75) - 75) / 1000.0;
        }
    }
}

void quickSort(Moderator *training, int low, int high, int id) {
    int i = low;
    int j = high;
    float pivot = training[(i + j) / 2].getScore(id);
    float temp;

    while (i <= j) {
        while (training[i].getScore(id) < pivot)
            i++;

        while (training[j].getScore(id) > pivot)
            j--;

        if (i <= j) {
            //swap
            training[j].setMatrix(training[i].setMatrix(training[j].getMatrixPtr(id), id), id);
            // training[j].ann->setMatrix(training[i].ann->setMatrix(training[j].ann->getMatrixPtr()));

            temp = training[j].getScore(id);
            training[j].setScore(training[i].getScore(id), id);
            training[i].setScore(temp, id);

            i++;
            j--;
        }
    }

    if (j > low)
        quickSort(training, low, j, id);
    if (i < high)
        quickSort(training, i, high, id);
}

void Evolution::genocideAll() {
    genocide(lightAssaultTraining, LIGHT_ASSAULT);
    genocide(snipersTraining, SNIPER);
    genocide(assaultsTraining, ASSAULT);
}

void Evolution::genocide(Moderator *training, int id) {
    quickSort(training, 0, POP_SIZE - 1, id);

    for (int i = 0; i < POP_SIZE / 6; i++) {
        training[i].getANN(id)->reset();
    }
}