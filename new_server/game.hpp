#include "socket_api.hpp"
#include <vector>
#include <string>
struct BoardState
{
    char state[9];
};
struct IndividualGame
{
    SOCKET player1;
    SOCKET player2;
    BoardState state;
};
