//SomeGame
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

class Creature
{
protected:
    std::string m_name;
    char m_symbol;
    int m_health;
    int m_damage;
    int m_gold;

public:
    Creature(std::string name = "", char symbol = '?', int health = 0, int damage = 0, int gold = 0)
        : m_name(name), m_symbol(symbol), m_health(health), m_damage(damage), m_gold(gold)
    {
    }

    std::string getName() const { return m_name; }
    char getSymbol() const { return m_symbol; }
    int getHealth() const { return m_health; }
    int getDamage() const { return m_damage; }
    int getGold() const { return m_gold; }

    void reduceHealth(int health) { m_health -= health; }
    bool isDead() const { return m_health <= 0; }
    void addGold(int gold) { m_gold += gold; }
};

class Player : public Creature
{
private:
    int m_level = 1;

public:
    Player(std::string name) : Creature(name, '@', 10, 1, 0) {}

    void levelUp() { ++m_level; ++m_damage; }
    int getLevel() const { return m_level; }
    bool hasWon() const { return m_level >= 20; }
};

class Monster : public Creature
{
public:
    enum class MonsterType
    {
        DRAGON,
        ORC,
        SLIME,
        MAX_TYPES
    };

    Monster(MonsterType type)
        : Creature(monsterData[static_cast<int>(type)].m_name,
            monsterData[static_cast<int>(type)].m_symbol,
            monsterData[static_cast<int>(type)].m_health,
            monsterData[static_cast<int>(type)].m_damage,
            monsterData[static_cast<int>(type)].m_gold) {}

    static Monster getRandomMonster();

private:
    struct MonsterData
    {
    public:
        std::string m_name;
        char m_symbol;
        int m_health;
        int m_damage;
        int m_gold;
    };

    static MonsterData monsterData[static_cast<int>(MonsterType::MAX_TYPES)];
};

Monster::MonsterData Monster::monsterData[static_cast<int>(MonsterType::MAX_TYPES)]
{
    { "dragon", 'D', 20, 4, 100 },
    { "orc", 'o', 4, 2, 25 },
    { "slime", 's', 1, 1, 10 }
};

Monster Monster::getRandomMonster()
{
    return Monster(static_cast<Monster::MonsterType>(rand() % static_cast<int>(Monster::MonsterType::MAX_TYPES)));
}

void attackMonster(Player& player, Monster& monster)
{
    monster.reduceHealth(player.getDamage());
    std::cout << "You hit the " << monster.getName() << " for " << player.getDamage() << " damage.\n";

    if (monster.isDead())
    {
        std::cout << "You killed the " << monster.getName() << ".\n";
        player.levelUp();
        std::cout << "You are now level " << player.getLevel() << ".\n";
        player.addGold(monster.getGold());
        std::cout << "You found " << monster.getGold() << " gold.\n";
    }
}

void attackPlayer(Player& player, Monster& monster)
{
    player.reduceHealth(monster.getDamage());
    std::cout << "The " << monster.getName() << " hit you for " << monster.getDamage() << " damage.\n";
}

void fightMonster(Player& player)
{
    Monster monster(static_cast<Monster::MonsterType>(rand() % static_cast<int>(Monster::MonsterType::MAX_TYPES)));
    std::cout << "You have encountered a " << monster.getName() << " (" << monster.getSymbol() << ").\n";
    char chose;

    while (true)
    {
        std::cout << "(R)un or (F)ight: ";
        std::cin >> chose;

        if (chose == 'F' || chose == 'f')
        {
            attackMonster(player, monster);
            if (monster.isDead())
            {
                break;
            }

            attackPlayer(player, monster);
            if (player.isDead())
            {
                break;
            }
        }
        else if (chose == 'R' || chose == 'r')
        {
            int trying;
            trying = rand() % 2;

            if (trying)
            {
                std::cout << "You successfully fled.\n";
                break;
            }

            std::cout << "You didn't flee.\n";

            attackPlayer(player, monster);
            if (player.isDead())
            {
                break;
            }
        }
    }

    return;
}

int main()
{
    srand(time(0));

    std::cout << "Enter you name: ";
    std::string nameOfPlayer;
    std::cin >> nameOfPlayer;
    Player player(nameOfPlayer);
    std::cout << "Welcome, " << player.getName() << '\n';

    bool isWin = false;

    while (true)
    {
        fightMonster(player);

        if (player.isDead())
        {
            break;
        }

        if (player.getLevel() >= 20)
        {
            isWin = true;
            break;
        }
    }

    if (isWin)
    {
        std::cout << "Congratulation! You win with " << player.getGold() << " gold.\n";
        std::cout << "Good luck!\n";
    }
    else
    {
        std::cout << "You died at level " << player.getLevel() << " and with " << player.getGold() << " gold.\n";
        std::cout << "Too bad you can't take it with you!\n";
    }

    return 0;
}