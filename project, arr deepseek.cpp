#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Entity {
public:
    string name;
    int atk;
    int def;
    int life;
    int curr_life;
    int wisdom;
    int hunger;
    int hunger_max;
    int level;
    int atk_growth;
    int def_growth;
    int life_growth;
    int wisdom_growth;
    int experience;

    Entity() {
        // Default constructor
        atk = 0;
        def = 0;
        life = 0;
        curr_life = 0;
        wisdom = 0;
        hunger = 0;
        hunger_max = 100;
        level = 1;
        atk_growth = 0;
        def_growth = 0;
        life_growth = 0;
        wisdom_growth = 0;
        experience = 0;
    }

    void level_up() {
        level++;
        atk += atk_growth;
        def += def_growth;
        life += life_growth;
        wisdom += wisdom_growth;
        curr_life = life; // Heal on level up
        cout << name << " leveled up to level " << level << "!\n";
    }

    bool is_alive() {
        return curr_life > 0;
    }

    void take_damage(int damage) {
        curr_life -= damage;
        if (curr_life < 0) curr_life = 0;
    }

    void heal(int amount) {
        curr_life += amount;
        if (curr_life > life) curr_life = life;
    }
};

class Player : public Entity {
public:
    Player() {
        name = "Hero";
        atk = 10;
        def = 10;
        life = 100;
        curr_life = 100;
        wisdom = 5;
        hunger = 0;
        hunger_max = 100;
        
        level = 1;
        atk_growth = 2;
        def_growth = 1;
        life_growth = 10;
        wisdom_growth = 1;
        experience = 0;
    }

    void status_show() {
        cout << "\n=== " << name << " Status ===\n";
        cout << "Level: " << level << endl;
        cout << "HP: " << curr_life << "/" << life << endl;
        cout << "ATK: " << atk << endl;
        cout << "DEF: " << def << endl;
        cout << "Wisdom: " << wisdom << endl;
        cout << "Hunger: " << hunger << "/" << hunger_max << endl;
        cout << "EXP: " << experience << endl;
    }

    void check_hunger() {
        if (hunger < 25) {
            cout << "You feel well-fed and energetic.\n";
        } else if (hunger < 50) {
            cout << "You feel something gently pressing on the back of your head.\n";
            cout << "Without my reminder, you barely notice that you are sluggish.\n";
        } else if (hunger < 75) {
            cout << "You explicitly feel your hunger.\n";
            cout << "It is socially acceptable for you to randomly eat a horse.\n";
        } else if (hunger < 100) {
            cout << "You are extremely hungry...\n";
            // Apply penalties
            atk -= 2;
            def -= 2;
        } else {
            cout << "You collapse from hunger!\n";
            take_damage(10);
        }
    }

    void eat(int hunger_reduction) {
        hunger -= hunger_reduction;
        if (hunger < 0) hunger = 0;
        cout << "You feel less hungry.\n";
    }

    void gain_exp(int exp) {
        experience += exp;
        cout << "Gained " << exp << " experience points!\n";
        
        // Simple level up system
        if (experience >= level * 100) {
            level_up();
            experience = 0;
        }
    }
};

class Enemy : public Entity {
public:
    int exp_reward;
    
    Enemy(string n, int a, int d, int l, int exp) {
        name = n;
        atk = a;
        def = d;
        life = l;
        curr_life = l;
        exp_reward = exp;
    }
};

class Item {
public:
    int id;
    string name;
    string effect_type;
    int increment_value;

    Item(int i, string n, string effect, int value) {
        id = i;
        name = n;
        effect_type = effect;
        increment_value = value;
    }

    void use(Player& player) {
        if (effect_type == "heal") {
            player.heal(increment_value);
            cout << "Used " << name << "! Healed " << increment_value << " HP.\n";
        } else if (effect_type == "food") {
            player.eat(increment_value);
            cout << "Ate " << name << "! Reduced hunger by " << increment_value << ".\n";
        } else if (effect_type == "atk") {
            player.atk += increment_value;
            cout << "Used " << name << "! ATK increased by " << increment_value << ".\n";
        }
    }
};

class Inventory {
public:
    vector<pair<Item*, int>> items; // Item pointer and count

public:
    void add_item(Item* thing) {
        for (auto& pair : items) {
            if (pair.first->id == thing->id) {
                pair.second++;
                return;
            }
        }
        items.push_back(make_pair(thing, 1));
    }

    void show_inventory() {
        cout << "\n=== Inventory ===\n";
        if (items.empty()) {
            cout << "Empty\n";
            return;
        }
        
        for (size_t i = 0; i < items.size(); i++) {
            cout << i + 1 << ". " << items[i].first->name << " x" << items[i].second << endl;
        }
    }

    bool use_item(int index, Player& player) {
        if (index < 1 || index > (int)items.size()) {
            cout << "Invalid item selection!\n";
            return false;
        }
        
        index--; // Convert to 0-based
        items[index].first->use(player);
        items[index].second--;
        
        if (items[index].second <= 0) {
            delete items[index].first;
            items.erase(items.begin() + index);
        }
        
        return true;
    }
};

class Location {
public:
    string name;
    string description;
    vector<Enemy*> enemies;
    vector<Item*> items;

    Location(string n, string desc) {
        name = n;
        description = desc;
    }

    void observe() {
        cout << "\nYou are in " << name << ".\n";
        cout << description << endl;
        
        if (!enemies.empty()) {
            cout << "You see " << enemies.size() << " enemy(ies) here!\n";
        }
        if (!items.empty()) {
            cout << "You find some items here!\n";
        }
    }

    void add_enemy(Enemy* enemy) {
        enemies.push_back(enemy);
    }

    void add_item(Item* item) {
        items.push_back(item);
    }
};

// Combat function
void combat(Player& player, Enemy& enemy) {
    cout << "\n=== Combat Start ===\n";
    cout << "You encounter a " << enemy.name << "!\n";
    
    while (player.is_alive() && enemy.is_alive()) {
        // Player turn
        cout << "\nYour HP: " << player.curr_life << "/" << player.life << endl;
        cout << enemy.name << " HP: " << enemy.curr_life << "/" << enemy.life << endl;
        
        cout << "\nChoose action:\n";
        cout << "1. Attack\n";
        cout << "2. Use Item\n";
        cout << "3. Flee\n";
        
        int choice;
        cin >> choice;
        
        if (choice == 1) {
            int damage = player.atk - enemy.def / 2;
            if (damage < 1) damage = 1;
            enemy.take_damage(damage);
            cout << "You deal " << damage << " damage to " << enemy.name << "!\n";
        } else if (choice == 2) {
            // Item usage would go here
            cout << "Item system not fully implemented in combat yet.\n";
            continue;
        } else if (choice == 3) {
            cout << "You fled from combat!\n";
            return;
        } else {
            cout << "Invalid choice!\n";
            continue;
        }
        
        // Check if enemy died
        if (!enemy.is_alive()) {
            cout << "You defeated the " << enemy.name << "!\n";
            player.gain_exp(enemy.exp_reward);
            return;
        }
        
        // Enemy turn
        int damage = enemy.atk - player.def / 2;
        if (damage < 1) damage = 1;
        player.take_damage(damage);
        cout << enemy.name << " deals " << damage << " damage to you!\n";
        
        if (!player.is_alive()) {
            cout << "You were defeated...\n";
            return;
        }
    }
}

int main() {
    // Create player
    Player player;
    
    // Create locations
    Location forest("Horrendous Forest", 
        "You smell mold and iron rust.\n"
        "The echoes of animals roaring ravage your eardrums.\n"
        "A sweet fragrance comes from the bush next to you.");
    
    Location cave("Dark Cave",
        "A damp, dark cave with mysterious sounds echoing.\n"
        "You can barely see anything without a light source.");
    
    // Create items
    Item berry(1, "Mysterious Berry", "food", 30);
    Item potion(2, "Health Potion", "heal", 25);
    
    // Create enemies
    Enemy wolf("Forest Wolf", 8, 5, 30, 50);
    Enemy goblin("Cave Goblin", 12, 8, 40, 75);
    
    // Populate locations
    forest.add_item(&berry);
    forest.add_enemy(&wolf);
    cave.add_item(&potion);
    cave.add_enemy(&goblin);
    
    // Create inventory
    Inventory inventory;
    
    // Game loop
    bool game_running = true;
    Location* current_location = &forest;
    
    cout << "=== Welcome to the Text Adventure Game ===\n";
    
    while (game_running && player.is_alive()) {
        current_location->observe();
        player.status_show();
        
        cout << "\nWhat would you like to do?\n";
        cout << "1. Explore\n";
        cout << "2. Check Status\n";
        cout << "3. Use Item\n";
        cout << "4. Check Inventory\n";
        cout << "5. Travel to Cave\n";
        cout << "6. Travel to Forest\n";
        cout << "7. Quit\n";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1: // Explore
                if (!current_location->enemies.empty()) {
                    combat(player, *current_location->enemies[0]);
                } else if (!current_location->items.empty()) {
                    Item* found_item = current_location->items[0];
                    cout << "You found: " << found_item->name << "!\n";
                    inventory.add_item(new Item(*found_item)); // Add copy to inventory
                    current_location->items.erase(current_location->items.begin());
                } else {
                    cout << "You explore but find nothing of interest.\n";
                    player.hunger += 10; // Increase hunger when exploring
                    player.check_hunger();
                }
                break;
                
            case 2: // Check Status
                player.status_show();
                break;
                
            case 3: // Use Item
                inventory.show_inventory();
                if (!inventory.items.empty()) {
                    cout << "Select item to use (0 to cancel): ";
                    int item_choice;
                    cin >> item_choice;
                    if (item_choice != 0) {
                        inventory.use_item(item_choice, player);
                    }
                }
                break;
                
            case 4: // Check Inventory
                inventory.show_inventory();
                break;
                
            case 5: // Travel to Cave
                current_location = &cave;
                cout << "You travel to the Dark Cave.\n";
                current_location->observe();
                player.hunger += 5;
                break;
                
            case 6: // Travel to Forest
                current_location = &forest;
                cout << "You travel to the Horrendous Forest.\n";
                current_location->observe();
                player.hunger += 5;
                break;
                
            case 7: // Quit
                game_running = false;
                cout << "Thanks for playing!\n";
                break;
                
            default:
                cout << "Invalid choice!\n";
                break;
        }
        
        // Check hunger every turn
        player.check_hunger();
        
        // Check for game over
        if (!player.is_alive()) {
            cout << "Game Over!\n";
        }
    }
    
    return 0;
}