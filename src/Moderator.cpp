#include "headers/Moderator.h"

Moderator::Moderator(Screen *screen)
{
    this->screen = screen;

    //allocs all memory
    inocents = new Inocent[NUMBER_OF_INOCENTS];
    traitors = new Traitor[NUMBER_OF_TRAITORS];
    detectives = new Detective[NUMBER_OF_DETECTIVES];
}

Moderator::~Moderator()
{
}

void Moderator::setAllPlayersValues()
{
    int playerNumber = 0;

    setPlayersValues(playerNumber, inocents, NUMBER_OF_INOCENTS);
    setPlayersValues(playerNumber, traitors, NUMBER_OF_TRAITORS);
    setPlayersValues(playerNumber, detectives, NUMBER_OF_DETECTIVES);
}

void Moderator::setPlayersValues(int &playerNumber, Player *players, int NUMBER_OF_PLAYERS)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++, playerNumber++)
    {
        players[i].setPlayerValues(screen, playerNumber, INOCENT_HEALTH);
        players[i].drawPlayer();
    }
}

void Moderator::drawAllPlayers()
{
    drawPlayers(inocents, NUMBER_OF_INOCENTS);
    drawPlayers(traitors, NUMBER_OF_TRAITORS);
    drawPlayers(detectives, NUMBER_OF_DETECTIVES);
}

void Moderator::drawPlayers(Player *players, int NUMBER_OF_PLAYERS)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        if (players[i].isAlive())
            players[i].drawPlayer();
    }
}

void Moderator::updateAllPlayersVision()
{
    updatePlayersVision(inocents, NUMBER_OF_INOCENTS);
    updatePlayersVision(traitors, NUMBER_OF_TRAITORS);
    updatePlayersVision(detectives, NUMBER_OF_DETECTIVES);
}

void Moderator::updatePlayersVision(Player *players, int NUMBER_OF_PLAYERS)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        if (players[i].isAlive())
            players[i].updateVision();
    }
}

void Moderator::conflictsAllPlayers()
{

    conflictsPlayers(inocents, NUMBER_OF_INOCENTS);
    conflictsPlayers(traitors, NUMBER_OF_TRAITORS);
    conflictsPlayers(detectives, NUMBER_OF_DETECTIVES);
}

void Moderator::conflictsPlayers(Player *players, int NUMBER_OF_PLAYERS)
{
    enemyInfo_t enemyInfo;

    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {

        if (players[i].isAlive())
        {
            enemyInfo = players[i].killPlayer((int)players[i].output[0][INDEX_SHOT]);

            if (enemyInfo.playerType == NOTHING || enemyInfo.playerType == OBSTACLE)
                continue;

            shotPlayer(&players[i], INOCENT_DAMAGE, enemyInfo);
        }
    }
}

void Moderator::shotPlayer(Player *shooter, int damage, enemyInfo_t enemyInfo)
{
    //find the player that was shot
    if (enemyInfo.playerType == INOCENT)
        findPlayer(shooter, inocents, NUMBER_OF_INOCENTS, damage, enemyInfo.posiAprox);

    if (enemyInfo.playerType == TRAITOR)
        findPlayer(shooter, traitors, NUMBER_OF_TRAITORS, damage, enemyInfo.posiAprox);

    if (enemyInfo.playerType == DETECTIVE)
        findPlayer(shooter, detectives, NUMBER_OF_DETECTIVES, damage, enemyInfo.posiAprox);
}

void Moderator::findPlayer(Player *shooter, Player *players, int NUMBER_OF_PLAYERS, int damage, cv::Point enemyPoint)
{
    float distance;

    //find the player that was shot
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        distance = cv::norm(enemyPoint - players[i].getCenter());

        if (distance <= RADIUS)
        {
            players[i].takeDamage(damage);
            std::cout << shooter->getPlayerID() << "matou " << players[i].getPlayerID() << std::endl;
            std::cout << "vida: " << players[i].getLife() << std::endl;
            break;
        }
    }
}

void Moderator::checkAllPlayersLife()
{
    checkPlayersLife(inocents, NUMBER_OF_INOCENTS);
    checkPlayersLife(traitors, NUMBER_OF_TRAITORS);
    checkPlayersLife(detectives, NUMBER_OF_DETECTIVES);
}

void Moderator::checkPlayersLife(Player *players, int NUMBER_OF_PLAYERS)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        if (players[i].getLife() <= 0)
            players[i].setAlive(false);
    }
}

void Moderator::multiplyAllPlayers()
{
    multiplyPlayers(inocents, NUMBER_OF_INOCENTS);
    multiplyPlayers(traitors, NUMBER_OF_TRAITORS);
    multiplyPlayers(detectives, NUMBER_OF_DETECTIVES);
}

void Moderator::multiplyPlayers(Player *players, int NUMBER_OF_PLAYERS)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        if (players[i].isAlive())
        {
            players[i].ann->multiply();
            players[i].move();
        }
    }
}

void Moderator::defineAllPlayersInput()
{
    definePlayersInput(inocents, NUMBER_OF_INOCENTS);
    definePlayersInput(traitors, NUMBER_OF_TRAITORS);
    definePlayersInput(detectives, NUMBER_OF_DETECTIVES);
}

void Moderator::definePlayersInput(Player *players, int NUMBER_OF_PLAYERS)
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        if (players[i].isAlive())
            players[i].setComunInput();
    }
}