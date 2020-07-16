#include <iostream>
#include <vector>

#include "headers/Moderator.h"

Moderator::Moderator() {
    //----------------INICIAL VALUES----------------
    turn = 0;
    lightAssaultScore = 0;
    sniperScore = 0;
    assaultScore = 0;

    lightAssaultScoreSum = 0;
    sniperScoreSum = 0;
    assaultScoreSum = 0;

    //----------------ALLOCS ALL PLAYERS MEMORY----------------
    lightAssaults = new LightAssault[NUMBER_OF_PLAYERS];
    snipers = new Sniper[NUMBER_OF_PLAYERS];
    assaults = new Assault[NUMBER_OF_PLAYERS];

    bestLightAssault = new dataOfBestPlayers_t;
    bestSniper = new dataOfBestPlayers_t;
    bestAssault = new dataOfBestPlayers_t;

    //----------------PLAYER INICIAL VALUES----------------
    bestLightAssault->player = lightAssaults;
    bestSniper->player = snipers;
    bestAssault->player = assaults;
    bestLightAssault->score = INICIAL_SCORE;
    bestSniper->score = INICIAL_SCORE;
    bestAssault->score = INICIAL_SCORE;

    //----------------PLAYERS CENTER----------------
    playersCenter = new cv::Point *[NUMBER_OF_TOTAL_PLAYERS];

    LACenter = new cv::Point *[NUMBER_OF_PLAYERS];
    SCenter = new cv::Point *[NUMBER_OF_PLAYERS];
    ACenter = new cv::Point *[NUMBER_OF_PLAYERS];

    setPlayerCenterPtr(lightAssaults, LACenter);
    setPlayerCenterPtr(snipers, SCenter);
    setPlayerCenterPtr(assaults, ACenter);

    checkMoveCos = new float[NUMBER_OF_ANGLES_INTERVAL];
    checkMoveSin = new float[NUMBER_OF_ANGLES_INTERVAL];

    float angle = 0;

    for (int i = 0; i < NUMBER_OF_ANGLES_INTERVAL; i++) {
        checkMoveCos[i] = cos(angle) + _RADIUS_TOTAL_DISTANCE;
        checkMoveSin[i] = sin(angle) + _RADIUS_TOTAL_DISTANCE;
        angle += ANGLE_INTERVAL;
    }
}

Moderator::~Moderator() {
}

void Moderator::setInicialPosAll(const cv::Point *inicialPos, int start) {
    setInicialPos(lightAssaults, inicialPos[start]);
    start++;
    setInicialPos(snipers, inicialPos[start % 3]);
    start++;
    setInicialPos(assaults, inicialPos[start % 3]);
}

void Moderator::setInicialPos(Player *players, cv::Point initialPos) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        players[i].initialPos = initialPos;
    }
}

void Moderator::setPlayerCenterPtr(Player *players, cv::Point **centerPtr) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
        centerPtr[i] = players[i].getCenterPtr();
}

void Moderator::setPlayerCenterPtr(Player *players, int offset) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
        playersCenter[i + offset] = players[i].getCenterPtr();
}

void Moderator::setScreen(Screen *screen) {
    this->screen = screen;

    screen->resetImage();
    screen->createObstacle();
}

void Moderator::setAllPlayersValues() {
    int playerNumber = 0;

    setPlayersValues(playerNumber, lightAssaults, LACenter);
    setPlayersValues(playerNumber, snipers, SCenter);
    setPlayersValues(playerNumber, assaults, ACenter);

    allocGPUPtr();
    // allocGPUMem();
    // sendMatrixMemToGPU();
}

void Moderator::setPlayersValues(int &playerNumber, Player *players, cv::Point **centerPtr) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++, playerNumber++)
        players[i].setPlayerValues(screen, playerNumber, centerPtr, checkMoveCos, checkMoveSin);
}

void Moderator::drawAllPlayers() {
    screen->resetImage();

    cv::rectangle(screen->getMap(), cv::Point(0, 150), cv::Point(300, 300), cv::Scalar(0, 1, 0), 1);
    cv::rectangle(screen->getMap(), cv::Point(LENGTH - 300, HEIGHT - 150), cv::Point(LENGTH, HEIGHT), cv::Scalar(0, 0, 1), 1);
    cv::rectangle(screen->getMap(), cv::Point(LENGTH - 400, 150), cv::Point(LENGTH - 100, 300), cv::Scalar(0, 1, 0), 1);

    screen->createObstacle();

    drawPlayers(lightAssaults);
    drawPlayers(snipers);
    drawPlayers(assaults);
}

void Moderator::drawPlayers(Player *players) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive())
            players[i].drawPlayer();
    }
}

void Moderator::updateAllPlayersVision() {
    updatePlayersVision(lightAssaults);
    updatePlayersVision(snipers);
    updatePlayersVision(assaults);
}

void Moderator::updatePlayersVision(Player *players) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive())
            players[i].updateVision();
    }
}

void Moderator::conflictsAllPlayers() {
    conflictsPlayers(lightAssaults);
    conflictsPlayers(snipers);
    conflictsPlayers(assaults);
}

void Moderator::conflictsPlayers(Player *players) {
    enemyInfo_t enemyInfo;

    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive() && players[i].output[0][INDEX_SHOT] > 0.5f) {
            enemyInfo = players[i].killPlayer();
            if (enemyInfo.playerType == NOTHING || enemyInfo.playerType == OBSTACLE)
                continue;

            shotPlayer(&players[i], enemyInfo);
        }
    }
}

void Moderator::shotPlayer(Player *shooter, enemyInfo_t enemyInfo) {
    //----------------LIGHT ASSAULT----------------
    if (enemyInfo.playerType == LIGHT_ASSAULT && findPlayer(shooter, lightAssaults, enemyInfo.posiAprox)) {
        if (shooter->getPlayerType() != LIGHT_ASSAULT)
            shooter->updateScore(shooter->getDamage());
        else
            shooter->updateScore(-2.5 * shooter->getDamage());

        //----------------SNIPER----------------
    } else if (enemyInfo.playerType == SNIPER && findPlayer(shooter, snipers, enemyInfo.posiAprox)) {
        if (shooter->getPlayerType() != SNIPER)
            shooter->updateScore(shooter->getDamage());
        else
            shooter->updateScore(-2.5 * shooter->getDamage());

        //----------------ASSAULT----------------
    } else if (enemyInfo.playerType == ASSAULT && findPlayer(shooter, assaults, enemyInfo.posiAprox)) {
        if (shooter->getPlayerType() != ASSAULT)
            shooter->updateScore(shooter->getDamage());
        else
            shooter->updateScore(-2.5 * shooter->getDamage());
    }
}

int Moderator::findPlayer(Player *shooter, Player *players, cv::Point enemyPoint) {
    float distance;

    //----------------CHECK ALL PLAYERS----------------
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (!players[i].isAlive() || players[i].getPlayerID() == shooter[i].getPlayerID())
            continue;

        distance = cv::norm(players[i].getCenter() - enemyPoint);

        //----------------FOUND PLAYER----------------
        if (distance <= RADIUS + 1) {
            players[i].takeDamage(shooter->getDamage());

            if (players[i].getLife() <= 0) {
                //friend fire doesnt count to score
                if (shooter->getPlayerType() == players[i].getPlayerType())
                    players[i].updateScore(12);
                else if (shooter->getPlayerType() == LIGHT_ASSAULT)
                    lightAssaultScore += 1;
                else if (shooter->getPlayerType() == SNIPER)
                    sniperScore += 1;
                else
                    assaultScore += 1;
            }
            return 1;
        }
    }

    return 0;
}

void Moderator::checkAllPlayersLife() {
    checkPlayersLife(lightAssaults);
    checkPlayersLife(snipers);
    checkPlayersLife(assaults);
}

void Moderator::checkPlayersLife(Player *players) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].getLife() <= 0 && players[i].isAlive())
            players[i].setAlive(false);
    }
}

void Moderator::moveAllPlayers() {
    movePlayers(lightAssaults);
    movePlayers(snipers);
    movePlayers(assaults);
}

void Moderator::movePlayers(Player *players) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive())
            players[i].move();
    }
}

void Moderator::multiplyAllPlayers() {
    multiplyPlayers(lightAssaults);
    multiplyPlayers(snipers);
    multiplyPlayers(assaults);
}

void Moderator::multiplyPlayers(Player *players) {
    #pragma omp parallel for
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive())
            players[i].ann->multiply();
    }
}

void Moderator::defineAllPlayersInput() {
    definePlayersInput(lightAssaults);
    definePlayersInput(snipers);
    definePlayersInput(assaults);
}

void Moderator::definePlayersInput(Player *players) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isAlive())
            players[i].setComunInput();
    }
}

void Moderator::calculateScore() {
    int i;
    float indvScore;

    //----------------LIGHT ASSAULT SCORE----------------
    for (i = 0; i < NUMBER_OF_PLAYERS; i++) {
        indvScore = lightAssaults[i].getScore();
        lightAssaultScoreSum += indvScore;

        if (indvScore > bestLightAssault->score) {
            bestLightAssault->score = indvScore;
            bestLightAssault->player = &lightAssaults[i];
            bestLightAssault->index = i;
        }
    }

    //----------------SNIPER SCORE----------------
    for (i = 0; i < NUMBER_OF_PLAYERS; i++) {
        indvScore = snipers[i].getScore();
        sniperScoreSum += indvScore;

        if (indvScore > bestSniper->score) {
            bestSniper->score = indvScore;
            bestSniper->player = &snipers[i];
            bestSniper->index = i;
        }
    }

    //----------------ASSAULT SCORE----------------
    for (i = 0; i < NUMBER_OF_PLAYERS; i++) {
        indvScore = assaults[i].getScore();
        assaultScoreSum += indvScore;

        if (indvScore > bestAssault->score) {
            bestAssault->score = indvScore;
            bestAssault->player = &assaults[i];
            bestAssault->index = i;
        }
    }
}

void Moderator::resetAllPlayers(bool resetScore) {
    screen->resetImage();
    screen->createObstacle();

    //----------------RESET SCORE----------------
    // std::cout << bestLightAssault->score << std::endl;
    // std::cout << bestSniper->score << std::endl;
    // std::cout << bestAssault->score << std::endl;

    bestLightAssault->score = INICIAL_SCORE;
    bestSniper->score = INICIAL_SCORE;
    bestAssault->score = INICIAL_SCORE;

    lightAssaultScoreSum = 0;
    sniperScoreSum = 0;
    assaultScoreSum = 0;

    if (resetScore) {
        lightAssaultScore = 0;
        sniperScore = 0;
        assaultScore = 0;
    }

    //----------------RESET PLAYERS----------------
    resetPlayers(lightAssaults, LIGHT_ASSAULT_HEALTH, resetScore);
    resetPlayers(snipers, SNIPER_HEALTH, resetScore);
    resetPlayers(assaults, ASSAULT_HEALTH, resetScore);
}

void Moderator::resetPlayers(Player *players, int life, bool resetScore) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
        players[i].reset(life, resetScore);
}

void Moderator::setAllWeights(LightAssault *bestLightAssaults, Sniper *bestSnipers, Assault *bestAssaults) {
    if (bestLightAssaults != nullptr)
        setWeights(bestLightAssaults, lightAssaults);

    if (bestSnipers != nullptr)
        setWeights(bestSnipers, snipers);

    if (bestAssaults != nullptr)
        setWeights(bestAssaults, assaults);

    // sendMatrixMemToGPU();
}

void Moderator::setWeights(Player *bestPlayers, Player *players) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
        players[i].ann->setMatrix(bestPlayers[i].ann->getMatrixPtr());
}

void Moderator::copyAllWeights(LightAssault *bestLightAssaults, Sniper *bestSnipers, Assault *bestAssaults) {
    if (bestLightAssaults != nullptr)
        copyWeights(bestLightAssaults, lightAssaults);

    if (bestSnipers != nullptr)
        copyWeights(bestSnipers, snipers);

    if (bestAssaults != nullptr)
        copyWeights(bestAssaults, assaults);
    
    // sendMatrixMemToGPU();
}

void Moderator::copyWeights(Player *bestPlayers, Player *players) {
    int j;

    MatrixF *newMatrixArray;
    MatrixF *matrixArray;

    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        newMatrixArray = players[i].ann->getMatrixPtr();
        matrixArray = bestPlayers[i].ann->getMatrixPtr();

        for (j = 0; j < layerSize + 1; j++)
            newMatrixArray[j] = matrixArray[j];
    }
}

void Moderator::setAllWeightsOneMatrix(MatrixF *lightAssaultMatrix, MatrixF *snioerMatrix, MatrixF *assaultMatrix) {
    int i;

    for (i = 0; i < NUMBER_OF_PLAYERS; i++)
        lightAssaults[i].ann->setMatrix(lightAssaultMatrix);

    for (i = 0; i < NUMBER_OF_PLAYERS; i++)
        snipers[i].ann->setMatrix(snioerMatrix);

    for (i = 0; i < NUMBER_OF_PLAYERS; i++)
        assaults[i].ann->setMatrix(assaultMatrix);

    // sendMatrixMemToGPU();
}

void Moderator::game() {
    for (turn = 0; turn < DURATION; turn++) {
        drawAllPlayers();
        updateAllPlayersVision();

        defineAllPlayersInput();
        multiplyAllPlayers();
        // multplyGPU();

        conflictsAllPlayers();
        checkAllPlayersLife();

        moveAllPlayers();
    }
    calculateScore();
}

void Moderator::gameOfBest() {
    for (int i = 0; i < DURATION; i++) {
        drawAllPlayers();
        updateAllPlayersVision();

        defineAllPlayersInput();
        multiplyAllPlayers();
        // multplyGPU();

        conflictsAllPlayers();
        // screen->updateMap();
        checkAllPlayersLife();

        moveAllPlayers();
        screen->updateMap();
    }
    calculateScore();
}

void Moderator::allocGPUPtr() {
    int playerIdx;
    int layerIdx;

    //----------------MATRIX----------------
    d_laM = new float **[NUMBER_OF_PLAYERS];
    d_sM = new float **[NUMBER_OF_PLAYERS];
    d_aM = new float **[NUMBER_OF_PLAYERS];

    //----------------LAYERS----------------
    d_laL = new float **[NUMBER_OF_PLAYERS];
    d_sL = new float **[NUMBER_OF_PLAYERS];
    d_aL = new float **[NUMBER_OF_PLAYERS];

    for (playerIdx = 0; playerIdx < NUMBER_OF_PLAYERS; playerIdx++) {
        //----------------MATRIXS----------------
        d_laM[playerIdx] = new float *[layerSize + 1];
        d_sM[playerIdx] = new float *[layerSize + 1];
        d_aM[playerIdx] = new float *[layerSize + 1];

        //----------------LAYERS----------------
        d_laL[playerIdx] = new float *[layerSize + 2];
        d_sL[playerIdx] = new float *[layerSize + 2];
        d_aL[playerIdx] = new float *[layerSize + 2];
    }
}

void Moderator::allocGPUMem() {
    int playerIdx;
    int layerIdx;

    std::vector<int> aux;

    aux.push_back(lightAssaults->ANNInputSize);

    for (layerIdx = 0; layerIdx < layerSize; layerIdx++) {
        aux.push_back(layers[layerIdx]);
    }

    aux.push_back(lightAssaults->ANNOutputSize);

    for (playerIdx = 0; playerIdx < NUMBER_OF_PLAYERS; playerIdx++) {
        //----------------INPUT LAYER----------------
        cudaMalloc((void **)&d_laL[playerIdx][0], aux[0] * sizeof(float));
        cudaMalloc((void **)&d_sL[playerIdx][0], aux[0] * sizeof(float));
        cudaMalloc((void **)&d_aL[playerIdx][0], aux[0] * sizeof(float));

        //----------------LAYERS & MATRIXS----------------
        for (layerIdx = 0; layerIdx < layerSize + 1; layerIdx++) {
            cudaMalloc((void **)&d_laM[playerIdx][layerIdx], aux[layerIdx + 1] * aux[layerIdx] * sizeof(float));
            cudaMalloc((void **)&d_sM[playerIdx][layerIdx], aux[layerIdx + 1] * aux[layerIdx] * sizeof(float));
            cudaMalloc((void **)&d_aM[playerIdx][layerIdx], aux[layerIdx + 1] * aux[layerIdx] * sizeof(float));

            cudaMalloc((void **)&d_laL[playerIdx][layerIdx + 1], aux[layerIdx + 1] * sizeof(float));
            cudaMalloc((void **)&d_sL[playerIdx][layerIdx + 1], aux[layerIdx + 1] * sizeof(float));
            cudaMalloc((void **)&d_aL[playerIdx][layerIdx + 1], aux[layerIdx + 1] * sizeof(float));
        }
    }
}

void Moderator::sendMatrixMemToGPU() {
    int playerIdx;
    int layerIdx;

    MatrixF* laMptr;
    MatrixF* sMptr;
    MatrixF* aMptr;

    for (playerIdx = 0; playerIdx < NUMBER_OF_PLAYERS; playerIdx++) {
        //----------------MATRIXS----------------
        laMptr = lightAssaults[playerIdx].ann->getMatrixPtr();
        sMptr = snipers[playerIdx].ann->getMatrixPtr();
        aMptr = assaults[playerIdx].ann->getMatrixPtr();

        for (layerIdx = 0; layerIdx < layerSize + 1; layerIdx++) {
            cudaMemcpyAsync(d_laM[playerIdx][layerIdx], laMptr[layerIdx].matrix, laMptr[layerIdx].memSize, cudaMemcpyHostToDevice);
            cudaMemcpyAsync(d_sM[playerIdx][layerIdx], sMptr[layerIdx].matrix, sMptr[layerIdx].memSize, cudaMemcpyHostToDevice);
            cudaMemcpyAsync(d_aM[playerIdx][layerIdx], aMptr[layerIdx].matrix, aMptr[layerIdx].memSize, cudaMemcpyHostToDevice);
        }
    }

    cudaDeviceSynchronize();
}

void Moderator::sendInputMemToGPU() {
    int playerIdx;

    vectorF* laIptr;
    vectorF* sIptr;
    vectorF* aIptr;

    for (playerIdx = 0; playerIdx < NUMBER_OF_PLAYERS; playerIdx++) {
        //----------------INPUT----------------
        laIptr = lightAssaults[playerIdx].ann->getInputPtr();
        sIptr = snipers[playerIdx].ann->getInputPtr();
        aIptr = assaults[playerIdx].ann->getInputPtr();

        cudaMemcpyAsync(d_laL[playerIdx][0], laIptr[0].vector, laIptr[0].memSize, cudaMemcpyHostToDevice);
        cudaMemcpyAsync(d_sL[playerIdx][0], sIptr[0].vector, sIptr[0].memSize, cudaMemcpyHostToDevice);
        cudaMemcpyAsync(d_aL[playerIdx][0], aIptr[0].vector, aIptr[0].memSize, cudaMemcpyHostToDevice);
    }

    cudaDeviceSynchronize();
}

void Moderator::sendOutputMemToCPU() {
    int playerIdx;

    vectorF* laOptr;
    vectorF* sOptr;
    vectorF* aOptr;

    for (playerIdx = 0; playerIdx < NUMBER_OF_PLAYERS; playerIdx++) {
        laOptr = lightAssaults[playerIdx].output;
        sOptr = snipers[playerIdx].output;
        aOptr = assaults[playerIdx].output;
        
        //----------------OUTPUT----------------
        cudaMemcpyAsync(laOptr->vector, d_laL[playerIdx][layerSize+1], laOptr->memSize, cudaMemcpyDeviceToHost);
        cudaMemcpyAsync(sOptr->vector, d_sL[playerIdx][layerSize+1], sOptr->memSize, cudaMemcpyDeviceToHost);
        cudaMemcpyAsync(aOptr->vector, d_aL[playerIdx][layerSize+1], aOptr->memSize, cudaMemcpyDeviceToHost);
    }

    cudaDeviceSynchronize();
}

void Moderator::multplyGPU(){

    int playerIdx;
    int layerIdx;

    MatrixF* laMptr = lightAssaults->ann->getMatrixPtr();
    MatrixF* sMptr = snipers->ann->getMatrixPtr();
    MatrixF* aMptr = assaults->ann->getMatrixPtr();

    sendInputMemToGPU();

    for(layerIdx = 0; layerIdx < layerSize + 1; layerIdx++){
        for (playerIdx = 0; playerIdx < NUMBER_OF_PLAYERS; playerIdx++) {
            
            CUDA::matrixMultiplication(d_laM[playerIdx][layerIdx], d_laL[playerIdx][layerIdx], d_laL[playerIdx][layerIdx+1],
                                        laMptr[layerSize].lines, laMptr[layerSize].coluns);
            
            CUDA::matrixMultiplication(d_sM[playerIdx][layerIdx], d_sL[playerIdx][layerIdx], d_sL[playerIdx][layerIdx+1],
                                        sMptr[layerSize].lines, sMptr[layerSize].coluns);
            
            CUDA::matrixMultiplication(d_aM[playerIdx][layerIdx], d_aL[playerIdx][layerIdx], d_aL[playerIdx][layerIdx+1],
                                        aMptr[layerSize].lines, aMptr[layerSize].coluns);
        }

        cudaDeviceSynchronize();
    }
    
    sendOutputMemToCPU();
}

void Moderator::freeGPUMem() {
    
    int playerIdx;
    int layerIdx;

    for (playerIdx = 0; playerIdx < NUMBER_OF_PLAYERS; playerIdx++) {
        //----------------INPUT LAYER----------------
        cudaFree(d_laL[playerIdx][0]);
        cudaFree(d_sL[playerIdx][0]);
        cudaFree(d_aL[playerIdx][0]);

        //----------------LAYERS & MATRIXS----------------
        for (layerIdx = 0; layerIdx < layerSize + 1; layerIdx++) {
            cudaFree(d_laM[playerIdx][layerIdx]);
            cudaFree(d_sM[playerIdx][layerIdx]);
            cudaFree(d_aM[playerIdx][layerIdx]);
           
            cudaFree(d_laL[playerIdx][layerIdx + 1]);
            cudaFree(d_sL[playerIdx][layerIdx + 1]);
            cudaFree(d_aL[playerIdx][layerIdx + 1]);
        }
    }
}