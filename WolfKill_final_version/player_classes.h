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

#include "f_in_integer.h"
using namespace std;

const int playerCount = 8;

class Character {
public:


string identity = ""; // Good or Bad


int index;
int remainingPeople;
bool alive;
bool semiAlive;
bool hasAbility1;
bool hasAbility2;
bool canVote;
int getVote;
bool targeted;
friend class GameState;


public:


string name = "";
string occupation = "";
Character(int idx) {
   index = idx;
   alive = true;
   hasAbility1 = false;
   hasAbility2 = false;
   canVote = false;
   getVote = 0;
   targeted = false;
}


virtual ~Character() {}


virtual void useAbility1(Character* playerList[]) = 0;
virtual void useAbility2(Character* playerList[], int killIndex) = 0;


void semiKilled() { alive = false;}
void killed() {
   alive = false;
   hasAbility1 = false;
   hasAbility2 = false;
   canVote = false;
   getVote = 0;
   targeted = false;
}
void lifesaved() { alive = true; }
bool isGood() { return identity == "Good"; }
void targetPlayer() { targeted = true; }
void vote(Character* playerList[]) {
   cout << "玩家列表:\n";
   for (int j = 0; j < playerCount; j++) {
       cout << j + 1 << ". " << playerList[j]->name;
       if (!playerList[j]->alive) cout << " (已死亡)";
       cout << " | ";
   }
   cout << "\n";
   cout << "請輸入要投的人 (index 1-8): ";
   int voteIndex = -1;
   voteIndex = force_input_integer(1, 8, index+1);
   
   playerList[voteIndex - 1]->getVote++;
}
};


class Villager : public Character {
public:
Villager(int idx) : Character(idx) {
   occupation = "Villager";
   identity = "Good";
   remainingPeople = 3;
   hasAbility1 = false;
   hasAbility2 = false;
   canVote = true;
}


bool taskConduct(){
   //srand(time(NULL));
   int randomNum1 = rand() % 11;
   int randomNum2 = rand() % 11;
   cout<<"村民"<<" "<<name<<"你好，"<<"請回答下面問題："<<endl;
   cout<<randomNum1<<" + "<<randomNum2<<" = ";
   int ans = 0;
    ans = force_input_integer(0, 9999);
   if(ans == randomNum1 + randomNum2){
           //task += 10
      
       return true;
   }
   else{
       cout<<"答案不正確，回去睡覺"<<endl;
       return false;
   }
}


void useAbility1(Character* playerList[]) override {}
void useAbility2(Character* playerList[], int killIndex) override {}
};


class Werewolves : public Character {
public:
Werewolves(int idx) : Character(idx) {
   occupation = "Wolf";
   identity = "Bad";
   remainingPeople = 2;
   hasAbility1 = true;
   hasAbility2 = false;
   canVote = true;
}


void useAbility1(Character* playerList[]) override {
   int killIndex;
   cout << "狼人 " << name << " 請輸入要殺的人 (index 1-8): ";
   killIndex = force_input_integer(1, 8);
   while(playerList[killIndex - 1]->alive == false){
    cout << "他已經，死了；目標換人";
    killIndex = force_input_integer(1, 8);
    
   }
   
   playerList[killIndex - 1]->semiKilled();
}


void useAbility2(Character* playerList[], int killIndex) override {}
};


//------------------------
// Witch
//------------------------
class Witch : public Character {
public:
Witch(int idx) : Character(idx) {
   occupation = "Witch";
   identity = "Good";
   remainingPeople = 1;
   hasAbility1 = true; // rescue
   hasAbility2 = true; // poison
   canVote = true;
}


void useAbility1(Character* playerList[]) override {
   // Poison
   int killIndex;
   cout << "女巫 " << name << " 請輸入要毒殺的人 (index 1-8): ";
   killIndex = force_input_integer(1, 8);
   while(playerList[killIndex - 1]->alive == false){
    cout << "他死了，目標換人";
   }
   playerList[killIndex - 1]->killed();
   hasAbility2 = false;
}


void useAbility2(Character* playerList[], int killIndex) override {
   // Rescue
   playerList[killIndex - 1]->lifesaved();
   hasAbility1 = false;
}


bool taskConduct(){
   //srand(time(NULL));
   int randomNum1 = rand() % 11;
   int randomNum2 = rand() % 11;
   cout<<"女巫"<<" "<<name<<"你好，"<<"請回答下面問題："<<endl;
   cout<<randomNum1<<" + "<<randomNum2<<" = ";
   int ans = force_input_integer(0, 9999);
   
   if(ans == randomNum1 + randomNum2){
           //task += 10
      
       return true;
   }
   else{
       cout<<"答案不正確，回去睡覺"<<endl;
       return false;
   }
}


};


//------------------------
// Prophet
//------------------------
class Prophet : public Character {
public:
Prophet(int idx) : Character(idx) {
   occupation = "Prophet";
   identity = "Good";
   remainingPeople = 1;
   hasAbility1 = true;
   hasAbility2 = false;
   canVote = true;
}
void useAbility1(Character* playerList[]) override {
   int target;
   cout << "預言家 " << name << " 請輸入要查驗的人 (index 1-8): ";
   target = force_input_integer(1, 8);
   
   cout << "結果: " << (playerList[target - 1]->isGood() ? "好人" : "壞人") << endl;
}


void useAbility2(Character* playerList[], int killIndex) override {}




bool taskConduct(){
   //srand(time(NULL));
   int randomNum1 = rand() % 11;
   int randomNum2 = rand() % 11;
   cout<<"村民"<<" "<<name<<"你好，"<<"請回答下面問題："<<endl;
   cout<<randomNum1<<" + "<<randomNum2<<" = ";
   int ans = force_input_integer(0, 9999);
   if(ans == randomNum1 + randomNum2){
           //task += 10
      
       return true;
   }
   else{
       cout<<"答案不正確，回去睡覺"<<endl;
       return false;
   }
}


};
//------------------------
// Hunter
//------------------------
class Hunters : public Character {
public:
Hunters(int idx) : Character(idx) {
   occupation = "Hunter";
   identity = "Good";
   remainingPeople = 1;
   hasAbility1 = true;
   hasAbility2 = false;
   canVote = true;
}
void useAbility1(Character* playerList[]) override {
   int target;
   cout << "獵人 " << name << " 請輸入要標記的人 (index 1-8): ";
   target = force_input_integer(1, 8);
  while(playerList[target - 1]->alive == false){
    cout << "bro he's dead, change your target";
    target = force_input_integer(1, 8);
  }
   playerList[target - 1]->targetPlayer();
   hasAbility1 = false;
}
void useAbility2(Character* playerList[], int killIndex) override {}
bool taskConduct(){
   //srand(time(NULL));
   int randomNum1 = rand() % 11;
   int randomNum2 = rand() % 11;
   cout<<"獵人"<<" "<<name<<"你好，"<<"請回答下面問題："<<endl;
   cout<<randomNum1<<" + "<<randomNum2<<" = ";
   int ans = force_input_integer(0, 9999);
  
   if(ans == randomNum1 + randomNum2){
           //task += 10
      
       return true;
   }
   else{
       cout<<"答案不正確，回去睡覺"<<endl;
       return false;
   }
}
};