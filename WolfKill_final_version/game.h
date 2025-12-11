#include "player_classes.h"
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


using namespace std;
const float Task_coef = 0.5;
bool testing = 0;
void switchPlayerScreen() {
cout << "\n\n>>> 請下一位玩家準備，按 Enter 開始 <<<";
cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 忽略緩衝區多餘輸入
cin.get(); // 等玩家按 Enter
for(int i = 0; i < 500; i++){
   cout<<"\n";
}


#ifdef _WIN32
   system("cls"); // Windows
#else
   system("clear"); // Linux / macOS
#endif
}







class GameState {
public:
int round;
bool isNight;
int alivePeople;
Character* playerList[playerCount];
int task;
int eveningWolfTarget = -1; // 用來記錄狼人晚間選的刀

void revealAllRoles() {
    cout << "\n==================================\n";
    cout << "          ✨ 角色揭示 (DEBUG) ✨\n";
    cout << "==================================\n";

    for (int i = 0; i < playerCount; i++) {
        cout << i + 1 << ". 玩家: " << playerList[i]->name 
             << " | 職業: " << playerList[i]->occupation 
             << " | 陣營: " << playerList[i]->identity
             << (playerList[i]->alive ? " (存活)" : " (已淘汰)") << "\n";
    }

    cout << "==================================\n";
    cout << "當前任務進度: " << task << " / 200\n";
    cout << "==================================\n\n";
}
GameState() {
   round = 0;
   alivePeople = playerCount;
   task = 0;
   isNight = false;
   for (int i = 0; i < playerCount; i++) playerList[i] = nullptr;
}
~GameState() {
   for (int i = 0; i < playerCount; i++)
       delete playerList[i];
}


int checkWinCondition() {
   int aliveWolves = 0;
   int aliveVillagers = 0;
   int aliveSpecial = 0;  // prophet + hunter + witch


   for (int i = 0; i < playerCount; i++) {
       if (!playerList[i]->alive) continue;


       if (dynamic_cast<Werewolves*>(playerList[i])) {
           aliveWolves++;
       }
       else if (dynamic_cast<Villager*>(playerList[i])) {
           aliveVillagers++;
       }
       else if (dynamic_cast<Prophet*>(playerList[i]) ||
                   dynamic_cast<Hunters*>(playerList[i]) ||
                   dynamic_cast<Witch*>(playerList[i])) {
           aliveSpecial++;
       }
   }


   // -----------------------------
   //   好人勝利條件
   // -----------------------------
   if (aliveWolves == 0 || task >= 100) {
       cout << "好人勝利！\n";
       return 1;
   }


   // -----------------------------
   //   狼人勝利條件
   // -----------------------------
   if (aliveVillagers == 0 || aliveSpecial == 0) {
       cout << "狼人勝利！\n";
       return 2;
   }
   // 尚未勝負
   return 0;
}
// ------------------------
// 新增 Evening Phase
// ------------------------
/*void dayPhase()//what to do during the day
   {
  
  
  
       for(int i=0;i<8;i++)
       {
            if (playerList[i]->canVote&&playerList[i]->alive)
       {
                cout << "Now you can choose who you want to kill and vote by input their name";
                playerList[i]->vote(playerList);
       }
           else
                cout << "Go back to your seat and watch the game proceed";
       }




       int highestVote = -1;
       int highestVoteIndex = -1;
       for(int i = 0; i < 8; i++){
            if(playerList[i]->getVote > highestVote){
                highestVote = playerList[i]->getVote;
                highestVoteIndex = playerList[i]->index;
            }
       }




       playerList[highestVoteIndex]->killed();
       cout<<"玩家"<<playerList[highestVoteIndex]->name<<"得到最高票 "<<highestVote<<"票，遭到淘汰！"<<endl;




}*/




void dayPhase()
{




    cout << "\n====== 第一輪投票開始 ======\n";




    // 清票
    for(int i = 0; i < 8; i++) playerList[i]->getVote = 0;








    for(int i = 0; i < 8; i++)
    {
        if (playerList[i]->canVote && playerList[i]->alive)
        {
            cout << "玩家 " << playerList[i]->name << " 請投票：";
            playerList[i]->vote(playerList);
        }
        else
        {
            cout << "玩家 " << playerList[i]->name << " 不能投票\n";
        }
    }








    int highestVote = -1;
    vector<int> tiedPlayers;




    for(int i = 0; i < 8; i++)
    {
        if (playerList[i]->getVote > highestVote)
        {
            highestVote = playerList[i]->getVote;
            tiedPlayers.clear();
            tiedPlayers.push_back(i);
        }
        else if (playerList[i]->getVote == highestVote)
        {
            tiedPlayers.push_back(i);
        }
    }




    // ======== 判斷票數結果 ========
    if (tiedPlayers.size() >= 3)
    {
        cout << "\n====== 三人以上同票，今日無人淘汰 ======\n";
        return;
    }
    else if (tiedPlayers.size() == 1)
    {
        // 直接淘汰
        int killIndex = tiedPlayers[0];
        playerList[killIndex]->killed();
        cout << "\n====== 玩家 " << playerList[killIndex - 1]->name 
             << " 以 " << highestVote << " 票遭淘汰！ ======\n";
        return;
    }




    // ======== 兩人平票 → 第二輪投票 ========
    cout << "\n====== 兩人同票，進入 PK 第二輪投票 ======\n";




    int p1 = tiedPlayers[0];
    int p2 = tiedPlayers[1];




    cout << "第二輪候選人：\n";
    cout << "1. " << playerList[p1]->name << "\n";
    cout << "2. " << playerList[p2]->name << "\n";




    // 清空票
    for(int i = 0; i < 8; i++) playerList[i]->getVote = 0;




    // 第二輪投票：所有可投票者只能投 p1 或 p2
    for(int i = 0; i < 8; i++)
    {
        if (playerList[i]->canVote && playerList[i]->alive)
        {
            cout << "玩家 " << playerList[i]->name 
                 << " 請在 " << playerList[p1]->name 
                 << " 和 " << playerList[p2]->name << " 中選一人：\n";




            int target = force_input_integer(1, 2);
            




            if (target == 1) playerList[p1]->getVote++;
            else playerList[p2]->getVote++;
        }
    }




    // 判斷第二輪結果
    if (playerList[p1]->getVote > playerList[p2]->getVote)
    {
        playerList[p1]->killed();
        cout << "\n====== 玩家 " << playerList[p1]->name 
             << " 在 PK 中遭淘汰！ ======\n";
    }
    else if (playerList[p2]->getVote > playerList[p1]->getVote)
    {
        playerList[p2]->killed();
        cout << "\n====== 玩家 " << playerList[p2]->name 
             << " 在 PK 中遭淘汰！ ======\n";
    }
    else
    {
        // 如果 PK 又平手 → 無人淘汰
        cout << "\n====== PK 平手，今日無人淘汰 ======\n";
    }
}




















void eveningPhase() {
cout << "黃昏時分，玩家開始各自行動...\n";
















auto printPlayerList = [this]() {
   cout << "玩家列表:\n";
   for (int j = 0; j < playerCount; j++) {
       cout << j + 1 << ". " << playerList[j]->name;
       if (!playerList[j]->alive) cout << " (已死亡)";
       cout << " | ";
   }
   cout << "\n";
};
















// 先找出所有狼人，紀錄 index
vector<int> wolfIndices;
for (int i = 0; i < playerCount; i++)
   if (dynamic_cast<Werewolves*>(playerList[i])) wolfIndices.push_back(i);
















int chosenTarget = -1; // 這晚狼人的目標
















for (int i = 0; i < playerCount; i++) {
   if (!playerList[i]->alive) continue;
















   // 狼人輪到
   if (Werewolves* wolf = dynamic_cast<Werewolves*>(playerList[i])) {
       switchPlayerScreen();
















       // 顯示隊友名字
       cout << "狼人 " << wolf->name << " 起床，這是你的狼人隊友：";
       for (int idx : wolfIndices) {
           if (idx != i) cout << playerList[idx]->name << " ";
       }
       cout << "\n";
















       // 如果還沒選刀
       if (chosenTarget == -1) {
           int target = -1;
           printPlayerList();
           do {
    cout << "你來選今晚要攻擊的玩家 (1-" << playerCount << "): ";
    
    // 改用 getline
    target = force_input_integer(1, 8);
    
    if (target < 1 || target > playerCount) {
        cout << "編號必須在 1 到 " << playerCount << " 之間！" << endl;
        continue;
    }
    
    if (!playerList[target - 1]->alive) {
        cout << "該玩家已死亡，請選擇其他玩家！" << endl;
        continue;
    }
    
    break;  // 有效輸入，跳出循環
    
} while (true);















           chosenTarget = target - 1;
           cout << "狼人已選擇目標: " << playerList[chosenTarget]->name << "\n";
       } else {
           // 如果是後面的狼人，詢問是否同意
           string agree;
           do {
               cout << "前一個狼人已選目標: " << playerList[chosenTarget]->name
                       << "，你同意這個目標嗎? (yes/no): ";
               cin >> agree;
           } while (agree != "yes" && agree != "no");
















           if (agree == "no") {
               int target = -1;
               printPlayerList();
               do {
                   cout << "你重新選擇今晚要攻擊的玩家 (1-" << playerCount << "): ";
                   cin >> target;
               } while (target < 1 || target > playerCount || !playerList[target - 1]->alive);


               chosenTarget = target - 1;
               cout << "狼人重新選擇目標: " << playerList[chosenTarget]->name << "\n";
           } else {
               cout << "狼人同意繼續使用前一個目標: " << playerList[chosenTarget]->name << "\n";
           }
       }
   }




   // 女巫、預言家、獵人、村民做任務
   else if (Witch* witch = dynamic_cast<Witch*>(playerList[i])) {
       switchPlayerScreen();
       int doneTask = witch->taskConduct();
       if(doneTask){
           this->task += 5 * Task_coef;
           cout << "任務完成，目前進度：" << task << " / 100" << endl;
       }
   } else if (Prophet* prophet = dynamic_cast<Prophet*>(playerList[i])) {
       switchPlayerScreen();
       int doneTask = prophet->taskConduct();
       if(doneTask){
           this->task += 5 * Task_coef;
           cout << "任務完成，目前進度：" << task << " / 100" << endl;
       }
   } else if (Hunters* hunter = dynamic_cast<Hunters*>(playerList[i])) {
       switchPlayerScreen();
       int doneTask = hunter->taskConduct();
       if(doneTask){
           this->task += 5 * Task_coef;
           cout << "任務完成，目前進度：" << task << " / 100" << endl;
       }
   } else if (Villager* villager = dynamic_cast<Villager*>(playerList[i])) {
       switchPlayerScreen();
       int doneTask = villager->taskConduct();
       if(doneTask){
           this->task += 10 * Task_coef;
           cout << "任務完成，目前進度：" << task << " / 100" << endl;
       }
   }
}


eveningWolfTarget = chosenTarget; // 最後設定全局狼人目標
cout << "黃昏結束，夜晚即將來臨...\n";
}


void nightPhase() {
    if(testing){
        revealAllRoles();
    }
   cout << "夜晚降臨...\n";
   vector<string> deaths;
  
   auto printPlayerList = [this]() {
       cout << "玩家列表:\n";
       for (int j = 0; j < playerCount; j++) {
           cout << j + 1 << ". " << playerList[j]->name;
           if (!playerList[j]->alive) cout << " (已死亡)";
           cout << " | ";
       }
       cout << "\n";
   };




   // 先處理狼人晚間選的目標
   int finalTarget = eveningWolfTarget;


   if (finalTarget != -1) {
       playerList[finalTarget]->semiKilled();
   }




   // 女巫、預言家、獵人、村民輪流操作
   for (int i = 0; i < playerCount; i++) {
       if (!playerList[i]->alive){
               continue;
       };


       // 狼人跳過
       if (dynamic_cast<Werewolves*>(playerList[i])) {
           switchPlayerScreen();
           cout << "狼人 " << playerList[i]->name << "你知道你做了什麼， 按 Enter 休息...\n";
           cin.ignore(numeric_limits<streamsize>::max(), '\n');
           cin.get();
           continue;
       }


       else if (Witch* witch = dynamic_cast<Witch*>(playerList[i])) {
           switchPlayerScreen();
           cout << "女巫 " << witch->name << " 起床\n";




           // 顯示黃昏狼人選的目標
           bool cureUsed = false;
           if (eveningWolfTarget != -1)
               cout << "今晚狼人選擇的目標是: " << playerList[eveningWolfTarget]->name
                       << " (編號 " << eveningWolfTarget + 1 << ")\n";




           // 先判斷是否使用解藥
           if (witch->hasAbility1 && eveningWolfTarget != -1) {
               string choice;
               do {
                   cout << "你是否要使用解藥救 " << playerList[eveningWolfTarget]->name << "? (yes/no): ";
                   cin >> choice;
               } while (choice != "yes" && choice != "no");






               if (choice == "yes") {
                   witch->useAbility2(playerList, eveningWolfTarget); // 使用解藥
                   eveningWolfTarget = -1; // 已救，不再被狼人攻擊
                   cureUsed = true;
               }
           }




           // 如果沒有用解藥，判斷是否使用毒藥
           if (witch->hasAbility2 && cureUsed == false) {
               string poisonChoice;
               do {
                   cout << "你是否要使用毒藥? (yes/no): ";
                   cin >> poisonChoice;
               } while (poisonChoice != "yes" && poisonChoice != "no");




               if (poisonChoice == "yes") {
                  
                  
                   // Poison
                   int killIndex = -1;
                   cout << "女巫 " << witch->name << " 請輸入要毒殺的人 (index 1-8): ";
                   killIndex = force_input_integer(1, 8);
                   
                   playerList[killIndex -1]->killed();
                   witch->hasAbility2 = false;
                   deaths.push_back(playerList[killIndex - 1]->name);
                   //witch->useAbility1(playerList); // 執行毒藥
                   // 注意：使用毒藥後 witch->hasAbility2 = false 已在 useAbility1 裡面處理






               }
           }
       }




           else if (Prophet* prophet = dynamic_cast<Prophet*>(playerList[i])) {
           switchPlayerScreen();
           printPlayerList();
           prophet->useAbility1(playerList);
           switchPlayerScreen();
       } else if (Hunters* hunter = dynamic_cast<Hunters*>(playerList[i])) {
           switchPlayerScreen();
           int doneTask = hunter->taskConduct();
           if(doneTask){
               this->task += 5 * Task_coef;
               cout<<"任務完成，目前進度："<<task<<" / 100"<<endl;
           }
           switchPlayerScreen();
       } else if (Villager* villager = dynamic_cast<Villager*>(playerList[i])) {
           switchPlayerScreen();
           int doneTask = villager->taskConduct();
           if(doneTask){
               this->task += 10 * Task_coef;
               cout<<"任務完成，目前進度："<<task<<" / 100"<<endl;
           }
           switchPlayerScreen();
       }
   }


   // 處理死亡
   /*int poisonedTarget = -1;
   for (int i = 0; i < playerCount; i++) {
       if (Witch* witch = dynamic_cast<Witch*>(playerList[i])) {
           if (witch->hasAbility2) {
               string word;
               do {
                   printPlayerList();
                   cout << "女巫 " << witch->name << " 是否要使用毒藥? (yes/no): ";
                   cin >> word;
               } while (word != "yes" && word != "no");


               if (word == "yes") {
                   int killIndex = -1;
                   do {
                       printPlayerList();
                       cout << "女巫 " << witch->name << " 請輸入要毒殺的玩家 index (1-" << playerCount << "): ";
                       cin >> killIndex;
                   } while (killIndex < 1 || killIndex > playerCount || !playerList[killIndex - 1]->alive);
                   poisonedTarget = killIndex - 1;
                   cout << "女巫已毒殺 " << playerList[poisonedTarget]->name << " (編號 " << poisonedTarget + 1 << ")\n";
               }
           }
       }
   }*/


 
   if (finalTarget != -1 && playerList[finalTarget]->alive == false) {
       playerList[finalTarget]->killed();
       deaths.push_back(playerList[finalTarget]->name);
   }
   /*if (poisonedTarget != -1 && playerList[poisonedTarget]->alive) {
       playerList[poisonedTarget]->killed();
       deaths.push_back(playerList[poisonedTarget]->name);
   }*/




   if (!deaths.empty()) {
       cout << "\n今晚死亡的玩家有: ";
       for (auto& n : deaths) cout << n << " ";
       cout << "\n";
   } else {
       cout << "\n今天是平安夜，沒有人死亡\n";
   }


   cout << "黎明到來...\n";
}



};




void setupPlayers(GameState& game) {
vector<string> roles = { "Wolf","Wolf","Hunter","Villager","Villager","Villager","Witch","Prophet" };
random_device rd;
mt19937 g(rd());
shuffle(roles.begin(), roles.end(), g);


   for (int i = 0; i < playerCount; i++) {
       string tempName;
       cout << "請玩家輸入名字: ";
       cin >> tempName;


       if (roles[i] == "Wolf") game.playerList[i] = new Werewolves(i);
       else if (roles[i] == "Villager") game.playerList[i] = new Villager(i);
       else if (roles[i] == "Hunter") game.playerList[i] = new Hunters(i);
       else if (roles[i] == "Witch") game.playerList[i] = new Witch(i);
       else game.playerList[i] = new Prophet(i);


       game.playerList[i]->name = tempName;        cout << "<" << tempName << "> 你的職業是: " << game.playerList[i]->occupation << "\n";
       switchPlayerScreen();
   }
}







