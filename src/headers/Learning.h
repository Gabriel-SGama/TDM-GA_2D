#pragma once

typedef struct dataOfBest_t
{
    int bestInocentScore;
    int bestTraitorScore;
    int bestInocentIndex;
    int bestTraitorIndex;

    dataOfBest_t() : bestInocentScore(-100),
                     bestTraitorScore(-100),
                     bestInocentIndex(0),
                     bestTraitorIndex(0) {}

} dataOfBest_t;

#include "Moderator.h"

class Learning
{
private:
    dataOfBest_t *dataOfBest;

    Moderator *inocentsTraining;
    Moderator *traitorsTraining;

public:
    Learning(/* args */);
    ~Learning();

    void setData(Moderator *inocentsTraining, Moderator *traitorsTraining, dataOfBest_t *dataOfBest);
    void eletism();
    void tournament();
};
