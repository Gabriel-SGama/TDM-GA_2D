#include "headers/Evolution.h"

#include <omp.h>
#include <time.h>

#include <iostream>

using namespace Eigen;

Evolution::Evolution() {
    bestTeamScore = INICIAL_SCORE;

    //best individual players
    
    bestTrainingANN = new ANN;

    //creates the matrix

    //best team players
    bestTeams = new Moderator;
    bestTeams->setModerator();
    bestTeams->setScreen(new Screen);
    bestTeams->setAllPlayersValues();
    
    bestTrainingANN->setANNParameters(bestTeams->getBestTeam()->ANNInputSize, bestTeams->getBestTeam()->ANNOutputSize);

    TOTAL_NUMBER_PLAYERS = 2 * POP_SIZE * NUMBER_OF_PLAYERS;
    //training moderators
    training = new Moderator[POP_SIZE];

    for (int i = 0; i < POP_SIZE; i++) {
        training[i].setModerator();
        training[i].setScreen(new Screen);
        training[i].setAllPlayersValues();

        training[i].setAllWeightsMod(nullptr, bestTeams->bestANN);
    }

    trainingChilds = new ANN[POP_SIZE];

    //create the matrixs for the childs
    createANN(bestTeams->getBestTeam()->ANNInputSize, bestTeams->getBestTeam()->ANNOutputSize);
}

Evolution::~Evolution() {
}

void Evolution::createANN(int inputSize, int outputSize) {
    for (int i = 0; i < POP_SIZE; i++)
        trainingChilds[i].setANNParameters(inputSize, outputSize);
}

void Evolution::game() {
    int i;

#pragma omp parallel for
    //game
    for (i = 0; i < POP_SIZE; i++) {
        training[i].game();
    }

    //select best team
    for (i = 0; i < POP_SIZE; i++) {
        
        if (training[i].trainingScore > bestTeamScore) {
            bestTeamScore = training[i].trainingScore;
            bestTrainingTeam = &training[i];
        }
    }
    
    bestTrainingANN = bestTrainingTeam->trainingANN;
}

void Evolution::reset() {
    bestTeamScore = INICIAL_SCORE;

    //sets weigths
    //swap
    bestTeams->setAllWeightsMod(bestTeams->bestANN, nullptr);
    bestTeams->setAllWeightsMod(nullptr, bestTrainingTeam->bestANN);

    cv::Point initialPos[] = {cv::Point(LENGTH-300,HEIGHT-250), cv::Point(LENGTH-400,150), cv::Point(0,150)};
    int posIndex = rand()%3;

#pragma omp parallel for
    for (int i = 0; i < POP_SIZE; i++) {
        training[i].setInicialPosAll(initialPos, posIndex);

        training[i].setAllWeightsMod(nullptr, bestTeams->bestANN);

        training[i].resetAllPlayers(true);
    }

    bestTeams->resetAllPlayers(true);
}

void Evolution::tournamentMod() {
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
        if (training[i].getScore() > score) {
            score = training[i].getScore();
            bestIndex = i;
        }
    }

#pragma omp parallel for
    for (i = 0; i < POP_SIZE; i++) {
        if (i == bestIndex)
            continue;

        matrixArray = trainingChilds[i].getMatrixPtr();

        father1 = &training[rand() % POP_SIZE];

        for (j = 0; j < TOURNAMENT_K; j++) {
            father2 = &training[rand() % POP_SIZE];
            if (father2->getScore() > father1->getScore())
                father1 = father2;
        }
        best1 = father1;

        father1 = &training[rand() % POP_SIZE];

        for (j = 0; j < TOURNAMENT_K; j++) {
            father2 = &training[rand() % POP_SIZE];
            if (father2->getScore() > father1->getScore())
                father1 = father2;
        }
        best2 = father1;

        matrixArrayBest1 = best1->getMatrixPtr(TRAINING_PLAYER);
        matrixArrayBest2 = best2->getMatrixPtr(TRAINING_PLAYER);

        // matrixArrayBest1 = training[bestIndex].getMatrixPtr(id);
        // matrixArrayBest2 = training[i].getMatrixPtr(id);

        for (j = 0; j < layerSize + 1; j++) {
            matrixArray[j] = (matrixArrayBest1[j] + matrixArrayBest2[j]) / 2;
        }

        mutation(matrixArray);
    }

#pragma omp parallel for
    for (i = 0; i < POP_SIZE; i++) {
        if (i == bestIndex)
            continue;
        //changes ptr
        trainingChilds[i].setMatrix(training[i].trainingANN->setMatrix(trainingChilds[i].getMatrixPtr()));
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

void quickSort(Moderator *training, int low, int high) {
    int i = low;
    int j = high;
    float pivot = training[(i + j) / 2].getScore();
    float temp;

    while (i <= j) {
        while (training[i].getScore() < pivot)
            i++;

        while (training[j].getScore() > pivot)
            j--;

        if (i <= j) {
            //swap
            training[j].setMatrix(training[i].setMatrix(training[j].getMatrixPtr(TRAINING_PLAYER), TRAINING_PLAYER), TRAINING_PLAYER);
            // training[j].ann->setMatrix(training[i].ann->setMatrix(training[j].ann->getMatrixPtr()));

            temp = training[j].getScore();
            training[j].setScore(training[i].getScore());
            training[i].setScore(temp);

            i++;
            j--;
        }
    }

    if (j > low)
        quickSort(training, low, j);
    if (i < high)
        quickSort(training, i, high);
}


void Evolution::genocide() {
    quickSort(training, 0, POP_SIZE - 1);

    for (int i = 0; i < POP_SIZE / 6; i++) {
        training[i].getANN(TRAINING_PLAYER)->reset();
    }
}