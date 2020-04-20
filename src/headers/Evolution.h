#pragma once

#include "Moderator.h"

class Evolution
{
private:
    Moderator *moderator;

public:
    Evolution(/* args */);
    ~Evolution();

    void setParam(Moderator *moderator);
    void eletism();
};
