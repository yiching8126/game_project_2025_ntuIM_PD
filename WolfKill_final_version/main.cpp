#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <locale>
#include <sstream>
#include <cctype>



#include "game.h"

#ifdef _WIN32
#include <windows.h>
#endif


using namespace std;
//this is bad practice, but we're not using other namespaces, so....

int main() {
    srand(time(NULL));
     #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif


    setlocale(LC_ALL, "zh_TW.UTF-8");
   GameState game;
   setupPlayers(game);




   while (game.checkWinCondition() == 0) {
       if (game.checkWinCondition() != 0) break;
       else {
           game.eveningPhase(); // 黃昏階段
           game.isNight = true;
           game.nightPhase();   // 夜晚階段
           game.dayPhase();
       }
   }
}
