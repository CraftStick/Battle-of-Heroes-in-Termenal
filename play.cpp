#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

#define COLOR_RESET   "\033[00m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[37m"

#define COLOR_BRIGHT_RED     "\033[91m"
#define COLOR_BRIGHT_GREEN   "\033[92m"
#define COLOR_BRIGHT_YELLOW  "\033[93m"
#define COLOR_BRIGHT_BLUE    "\033[94m"
#define COLOR_BRIGHT_MAGENTA "\033[95m"
#define COLOR_BRIGHT_CYAN    "\033[96m"
#define COLOR_BRIGHT_WHITE   "\033[97m"

#define BG_BLACK   "\033[40m"
#define BG_RED     "\033[41m"
#define BG_GREEN   "\033[42m"
#define BG_YELLOW  "\033[43m"
#define BG_BLUE    "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN    "\033[46m"
#define BG_WHITE   "\033[47m"

#define BOLD "\033[1m"

void setColor(int text_color, int back_color) {
    printf("\033[3%d;4%dm", text_color, back_color);
}

void gotoXY(int line, int column) {
    printf("\033[%d;%dH", line, column);
}

class Character {
protected:
    string name;
    int health;
    int maxHealth;
    int damage;

public:
    Character(string n, int h, int d) : name(n), health(h), maxHealth(h), damage(d) {}

    void attack(Character& obj) {
        obj.health -= damage;
        if (obj.health < 0) obj.health = 0;
        cout << COLOR_YELLOW << name << COLOR_RESET << " attacks " 
             << COLOR_RED << obj.name << COLOR_RESET 
             << " and deals " << COLOR_BRIGHT_RED << damage << COLOR_RESET << " damage!\n";
    }

    bool isAlive(){
        return health > 0;
    }

    void displayStatus(){
        cout << "Name: " << name << endl;
    }

    int get_health(){
        return health;
    }

    int get_maxHealth(){
        return maxHealth;
    }

    string get_name(){
        return name;
    }

    void takeDamage(int d) {
        health -= d;
        if (health < 0) health = 0;
    }

    void heal(int amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
        cout << COLOR_GREEN << name << COLOR_RESET << " heals for " 
             << COLOR_BRIGHT_GREEN << amount << COLOR_RESET << " health! Now " 
             << COLOR_GREEN << health << "/" << maxHealth << COLOR_RESET << endl;
    }
};

class Player : public Character {
private:
    vector<string> inventory;
    int potions;

public:
    Player(string n, int h, int d) : Character(n, h, d), potions(3) {
        inventory.push_back("Health Potion");
        inventory.push_back("Health Potion");
        inventory.push_back("Health Potion");
    }

    void victory() {
        cout << COLOR_BRIGHT_YELLOW << BOLD << name << " WON! ✨" << COLOR_RESET << endl;
    }

    void defeat() {
        cout << COLOR_BRIGHT_RED << BOLD << name << " was defeated! 💀" << COLOR_RESET << endl;
    }

    void displayPlayer() {
        setColor(3, 0);
        cout << "[Player O_O]";
        setColor(7, 0);
        cout << endl;
        displayStatus();
        cout << COLOR_CYAN << "HP: " << COLOR_RESET;
        if (health < maxHealth / 3) {
            cout << COLOR_RED;
        } else if (health < maxHealth / 2) {
            cout << COLOR_YELLOW;
        } else {
            cout << COLOR_GREEN;
        }
        cout << health << "/" << maxHealth << COLOR_RESET << endl;
        cout << COLOR_MAGENTA << "Potions: " << potions << COLOR_RESET << endl;
    }

    void usePotion() {
        if (potions > 0 && health < maxHealth) {
            potions--;
            heal(15);
        } else if (potions <= 0) {
            cout << COLOR_RED << "No potions left!" << COLOR_RESET << endl;
        } else {
            cout << COLOR_YELLOW << "Health is already full!" << COLOR_RESET << endl;
        }
    }

    bool hasPotions() {
        return potions > 0;
    }
};

class Enemy : public Character {
private:
    int healingBoxUses;

public:
    Enemy(string n, int h, int d) : Character(n, h, d), healingBoxUses(2) {}

    void displayEnemy() {
        setColor(1, 0);
        cout << "[Enemy X_X]";
        setColor(7, 0);
        cout << endl;
        displayStatus();
        cout << COLOR_CYAN << "HP: " << COLOR_RESET;
        if (health < maxHealth / 3) {
            cout << COLOR_RED;
        } else if (health < maxHealth / 2) {
            cout << COLOR_YELLOW;
        } else {
            cout << COLOR_GREEN;
        }
        cout << health << "/" << maxHealth << COLOR_RESET << endl;
    }

    void healingBox() {
        if (healingBoxUses > 0 && health < maxHealth && health > 0) {
            health += 8;
            if (health > maxHealth) health = maxHealth;
            healingBoxUses--;
            cout << COLOR_BRIGHT_MAGENTA << name << " uses healingbox! " << COLOR_RESET
                 << "Health restored to " << COLOR_GREEN << health << "/" << maxHealth << COLOR_RESET << endl;
        } else if (healingBoxUses <= 0) {
            cout << COLOR_RED << name << " has no healingboxes left!" << COLOR_RESET << endl;
        } else if (health <= 0) {
            cout << COLOR_RED << name << " is dead and cannot heal!" << COLOR_RESET << endl;
        } else {
            cout << COLOR_YELLOW << name << " doesn't need healing!" << COLOR_RESET << endl;
        }
    }

    int getHealingUses() {
        return healingBoxUses;
    }
};

class Pet {
private:
    string name;
    int bites;

public:
    Pet(string n, int b) : name(n), bites(b) {}

    void displayStatus() {
        setColor(5, 0);
        cout << "[Pet ^_^]";
        setColor(7, 0);
        cout << endl;
        cout << COLOR_BRIGHT_CYAN << name << COLOR_RESET 
             << " can bite " << COLOR_BRIGHT_YELLOW << bites << COLOR_RESET 
             << " more times" << endl;
    }

    void attackPet(Enemy& enemy) {
        if (bites > 0) {
            enemy.takeDamage(2);
            bites--;
            cout << COLOR_BRIGHT_CYAN << name << COLOR_RESET << " bites " 
                 << COLOR_RED << enemy.get_name() << COLOR_RESET 
                 << " and deals " << COLOR_BRIGHT_RED << "2" << COLOR_RESET << " damage!" << endl;
        } else {
            cout << COLOR_YELLOW << name << " is too tired to bite!" << COLOR_RESET << endl;
        }
    }

    bool canBite() {
        return bites > 0;
    }
};

void drawHealth(int health, int maxHealth) {
    cout << COLOR_CYAN << "Health: " << COLOR_RESET;
    int healthBars = (health * 20) / maxHealth;
    
    if (health < maxHealth / 3) {
        cout << COLOR_RED;
    } else if (health < maxHealth / 2) {
        cout << COLOR_YELLOW;
    } else {
        cout << COLOR_GREEN;
    }
    
    for (int i = 0; i < 20; i++) {
        if (i < healthBars) {
            cout << "█";
        } else {
            cout << "░";
        }
    }
    cout << COLOR_RESET << " " << health << "/" << maxHealth << endl;
}

void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

int main() {
    srand(time(0));
    
    string name; 
    int damage, health, b, n = 0;
    
    clearScreen();
    
    setColor(3, 0);
    cout << "╔══════════════════════════════════╗" << endl;
    cout << "║    CHARACTER CREATION MENU       ║" << endl;
    cout << "╚══════════════════════════════════╝" << endl;
    setColor(7, 0);
    
    cout << COLOR_GREEN << "Enter player name, health, damage: " << COLOR_RESET;
    cin >> name >> health >> damage;
    Player player(name, health, damage);
    
    cout << COLOR_RED << "Enter enemy name, health, damage: " << COLOR_RESET;
    cin >> name >> health >> damage;
    Enemy enemy(name, health, damage);
    
    cout << COLOR_MAGENTA << "Enter pet name and bite count: " << COLOR_RESET;
    cin >> name >> b;
    Pet pet(name, b);

    cout << "\n" << COLOR_BRIGHT_YELLOW << BOLD << "=== BATTLE BEGINS ===" << COLOR_RESET << endl;
    
    while (player.isAlive() && enemy.isAlive()) {
        n++;
        
        setColor(3, 0);
        printf("\n══════ ROUND %d ══════\n", n);
        setColor(7, 0);
        
        player.displayPlayer();
        drawHealth(player.get_health(), player.get_maxHealth());
        
        enemy.displayEnemy();
        drawHealth(enemy.get_health(), enemy.get_maxHealth());
        
        pet.displayStatus();
        
        cout << "\n" << COLOR_BRIGHT_GREEN << "Your turn! Choose action:" << COLOR_RESET << endl;
        cout << COLOR_YELLOW << "1. Attack" << COLOR_RESET << endl;
        cout << COLOR_BLUE << "2. Use Health Potion (" 
             << (player.hasPotions() ? COLOR_GREEN "available" : COLOR_RED "none left") 
             << COLOR_RESET << ")" << endl;
        
        int choice;
        cin >> choice;
        
        if (choice == 2 && player.hasPotions()) {
            player.usePotion();
        } else {
            player.attack(enemy);
            
            if (enemy.isAlive() && pet.canBite()) {
                pet.attackPet(enemy);
            }
        }
    
        if (enemy.isAlive()) {
            cout << "\n" << COLOR_RED << BOLD << "Enemy's turn:" << COLOR_RESET << endl;
         
            if (enemy.get_health() < enemy.get_maxHealth() / 3 && enemy.getHealingUses() > 0) {
                enemy.healingBox();
            }

            if (enemy.isAlive()) {
                enemy.attack(player);
            }
        }
    }

    cout << "\n" << COLOR_BRIGHT_YELLOW << BOLD << "=== BATTLE ENDED ===" << COLOR_RESET << endl;
    
    if (player.isAlive()) {
        player.victory();
    } else {
        player.defeat();
    }
    
    cout << COLOR_RESET;
    
    return 0;
}
