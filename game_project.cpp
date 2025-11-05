#include <iostream>
#include <vector>
using namespace std;
 class Entity{
    public:
    string name;
    int atk;
    int def;
    int life;
    int curr_life;
    int wisdom;

    int hunger; int hunger_max;

    int level;
    int atk_growth;
    int def_growth;
    int life_growth;
    int wisdom_growth;

    int experience;

    void level_up(){
        atk += atk_growth;
        def += def_growth;
        life += life_growth;
        wisdom += wisdom_growth;

        curr_life = life;
    }
    
};
class Player : public Entity{
    void status_show(){
        cout << "atk: " << atk << endl;
        cout << "def: " << def << endl;
        cout << "life: " << life << endl;
        cout << "hunger:" << hunger << endl;
        
    }
    Player {
        atk = 10;
        def = 10;
        life = 100;
        curr_life = 100;
        wisdom = 0;
        hunger = 0;

        level = 0;
        atk_growth = 1;
        def_growth = 1;
        life_growth = 1;
        wisdom_growth = 1;
    }
    void hunger(int phase){
        if(phase == 1){
            cout << "You feel something gently pressing on the back of you head\n";
            cout << "Without my reminder, you barely notice that you are sluggish.";
        }
        else if(phase == 2){
            cout << "You explicitly feel your hunger";
            cout << "It is socially acceptable for you to randomly... ";
            cout << "Like, Eat a horse.";
        }
        else if(phase == 3){
            cout << "you are hungry...";
        }
        else if(phase == 4){
            //player collapses;
        }
    }
};
class Enemy : public Entity{

};
class Location{
    public:
    string name;
    string description;
    int* monster_id = new int[100];
    int monster_types;

    int* item_id = new int[100];
    int item_types;
    void observe(){
        cout << endl;
        cout << "You are in " << name << ".\n";
        cout << description;
    }
    Location** adjacent;
};
class Item{
    public:
    int id;
    string effect_type;
    string increment_type;
    string increment_value;


};
class Inventory{
    Item **item_types = {};
    int item_cnt[100] = {};
    int item_type_cnt = 0;
    void add_item(Item *thing){
        bool is_in = 0;
        int index = -1;
        for(int i = 0; i < item_type_cnt; i++){
            if(item_types[i] == thing){
                is_in = 1; index = i; break;}
        }
        if(index != -1){
            item_cnt[i]++;
        }
        else{
            item_type_cnt++;
            item_types[item_type_cnt-1] = thing;
        }
        
    }
}


int main(){
    
    Location forest;
    forest.name = "Horrendous Forest";
    forest.description = "You smell mold and iron rust. \n";
    forest.description += "The echos of some animals roaring ravages your eardrums.\n";
    forest.description += "A sweet fragrance comes from the bush next to you.\n";
    forest.description += "You feel restless, as if you have an obligation to do something.";
    forest.observe();

    Item berry;
    berry.id = 1;
    berry.effect_type = "food";
    berry.increment_value = 120;

    Player player = Player();
    
    
    string action;

}