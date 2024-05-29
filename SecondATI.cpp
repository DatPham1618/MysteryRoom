#include <iostream>
#include <cstdlib>
#include <vector>
#include <random>
using namespace std;
class Room;
class Hole;
class Dragon;
class Human;
class Monster;
class Stones;

//Class Entities
// Abstract base class for all entities in the game
class Entities {
protected:
    Room& _room;
    string _type;
    char _sign;
    bool _canMove;
    int x_position;
    int y_position;

public:
    Entities(Room& room, string typeZ, char _signZ, bool canMove, int x, int y) : _room(room), _type(typeZ), _sign(_signZ), _canMove(canMove), x_position(x), y_position(y) {};

     // Getter and setter methods for entity properties
    string typeGetter() { return _type; }
    char _signGetter() { return _sign; }
    bool statusGetter() { return _canMove; }
    int getX() { return x_position; }
    int getY() { return y_position; }
    void typeSetter(string typeZ) { _type = typeZ; }
    void _signSetter(char _signNew) { _sign = _signNew; }
    void setAnimatedOrInanimate(bool canMove) { _canMove = canMove; }

    // Pure virtual function for entity movement
    virtual void Movement() = 0;
    // Virtual function for displaying entity information
    virtual void showInfo() {};
};
// Class Room
// Class representing the game room

class Room{

    private:

    vector<vector<Entities*>> room;

    public:

    Room(){
        room = vector<vector<Entities*>>(10, vector<Entities*>(10, nullptr));
    }

     // Method to set the position of an entity in the room
    void setPositionEntity(Entities* entity, int x, int y){
        room[x][y] = entity;
    }

    // Method to check if a position is valid within the room boundaries
    bool isPositionValid(int x, int y) {
        return x >= 0 && x < room.size() && y >= 0 && y < room[0].size();
    }
    
    // Method to check if a position is free (no entity occupying it)
    bool isPositionFree(int x, int y) {
        return isPositionValid(x, y) && room[x][y] == nullptr;
    }

    // Method to move an entity to a new position in the room
    void moveEntityOneMoreStep(Entities* entity, int newX, int newY) {
    if(!entity->statusGetter()){
        return;
    }
    else{
    if (isPositionValid(newX, newY) && isPositionFree(newX, newY)) {
        room[entity->getX()][entity->getY()] = nullptr;

        room[newX][newY] = entity;
    }}
    }


    // Method to remove an entity from the room
    void removeEntity(Entities* entity){
    int entityX = entity->getX();
    int entityY = entity->getY();
    delete entity;
    room[entityX][entityY] = nullptr;
    }
    
    
    // Method to display the current state of the room
    void showMap() {
    cout << "  ";
    for (int i = 0; i < room[0].size(); i++) {
        cout << i << " ";
    }
    cout << endl;

    for (int i = 0; i < room.size(); i++) {
        cout << i << " ";
        for (int j = 0; j < room[i].size(); j++) {
            if (room[i][j] == nullptr)
                cout << ". ";
            else
                cout << room[i][j]->_signGetter() << " ";
        }
        cout << endl;
    }
}
    // Method to display information about an entity at a specific position
    void showInfo() {
        int x, y;
        cout << "Enter a row please: ";
        cin >> x;
        cout << "Enter a column please: ";
        cin >> y;

        if (x < 0 || x >= room.size() || y < 0 || y >= room[0].size()) {
            cout << "Invalid coordinates. Please enter values between 0 and 9." << endl;
        } else {
            if (room[x][y] == nullptr) {
                cout << "Nothing here" << endl;
            } else {
                room[x][y]->showInfo();
            }
        }
    }

    //Method declaration
    void resetRoom();

    void setInitialRoom();

    void moveAllEntities();
};

// Class Hole
// Class representing a hole entity
class Hole : public Entities {
    int depth;

public:
    Hole(Room& room, int x, int y) : Entities(room,"Hole", 'O', false, x, y), depth(rand() % 21) {
    _room.setPositionEntity(this, x_position, y_position); }
    
    void Movement(){
        return;
    }
    
    void showInfo(){
        cout << "Type: " << _type << endl;
        cout << "Depth: " << depth << endl;
    }

    int depthGetter() { return depth; }
};

// Class Stone
// Class representing a stone entity
class Stones : public Entities {
    public:
    Stones(Room& room, int x, int y) : Entities(room, "Stone", 'S', false, x, y) {
    _room.setPositionEntity(this, x_position, y_position); }
    
    void Movement(){
        return;
    }
    
    void showInfo(){
        cout << "Type: " << _type << endl; 
    }
};

// Class Dragon
// Class representing a dragon entity
class Dragon : public Entities {
    

    int health;

    

    public:

    Dragon(Room& room, int x, int y) : Entities(room,"Dragon", '#', true, x, y), health(100) {
    _room.setPositionEntity(this, x_position, y_position);
    }

    void healthSetter(int healthZ){
        health = healthZ;
    }
    int healthGetter(){
        return health;
    }
    bool checkHealth(){
        return health > 0;
    }
    void Movement() {
    vector<pair<int, int>> directions {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    vector<pair<int, int>> validMoves;
    if (checkHealth()) {
        for (auto dir : directions) {
            int newX = x_position + dir.first;
            int newY = y_position + dir.second;
            if (_room.isPositionFree(newX, newY) && _room.isPositionValid(newX, newY)) {
                validMoves.push_back(make_pair(newX, newY));
            }
        }
        if (!validMoves.empty()) {
            auto randomMove = validMoves[rand() % validMoves.size()];
            _room.moveEntityOneMoreStep(this, randomMove.first, randomMove.second);
            x_position = randomMove.first;
            y_position = randomMove.second;
            int damageAfterMove = rand() % 6;
            health -= damageAfterMove;
        }
    } else {
        _room.removeEntity(this);
    }
}
    void showInfo(){
        cout << "Type: " << _type << endl;
        cout << "Health: " << health << endl;
    }
};

// Class Human
// Class representing a human entity
class Human : public Entities{
    int health;

    int damageAfterMove;

    string name;

    public:

    Human(Room& room, int x, int y, string nameZ)
    : Entities(room,"Human", '@', true, x, y), health(100), name(nameZ) {
    damageAfterMove = 1;
    _room.setPositionEntity(this, x_position, y_position);
    }   

    string nameGetter(){
        return name;
    }
    void nameSetter(string nameZ){
        name = nameZ;
    }
    void healthSetter(int healthZ){
        health = healthZ;
    }
    int healthGetter(){
        return health;
    }
    bool checkHealth(){
        return health > 0;
    }
void Movement() {
    if (checkHealth()) {
        int newX = x_position; 
        int newY = y_position + 1;
        if (_room.isPositionFree(newX, newY) && _room.isPositionValid(newX, newY)) {
            _room.moveEntityOneMoreStep(this, newX, newY);
            x_position = newX;
            y_position = newY;
            health -= damageAfterMove;
        } else {
            vector<pair<int, int>> directions {{1, 0}, {-1, 0}, {0, -1}};
            vector<pair<int, int>> validMoves;
            for (auto dir : directions) {
                newX = x_position + dir.first;
                newY = y_position + dir.second;
                if (_room.isPositionFree(newX, newY) && _room.isPositionValid(newX, newY)) {
                    validMoves.push_back(make_pair(newX, newY));
                }
            }
            if (!validMoves.empty()) {
                auto randomMove = validMoves[rand() % validMoves.size()];
                _room.moveEntityOneMoreStep(this, randomMove.first, randomMove.second);
                x_position = randomMove.first;
                y_position = randomMove.second;
                health -= damageAfterMove;
            }
        }
    } else {
        _room.removeEntity(this);
    }
}
    
    void showInfo(){
        cout << "Type: " << _type << endl;
        cout << "Name: " << name << endl;
        cout << "Health: " << health << endl;
    }
};

// Class Monster
// Class representing a monster entity
class Monster : public Entities{
    int health;

    int strength;

    public:
    Monster(Room& room, int x, int y) : Entities(room, "Monster", '*', true, x, y), health(100), strength(rand() % 6) {
    _room.setPositionEntity(this, x_position, y_position);}
    
    void healthSetter(int healthZ){
        health = healthZ;
    }
    int healthGetter(){
        return health;
    }

    bool checkHealth(){
        return health > 0;
    }

    void strengthSetter(int strengthZ){
        strength = strengthZ;
    }
    int strengthGetter(){
        return strength;
    }

void Movement() {
    vector<pair<int, int>> directions {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    bool checkMove = false;

    if (checkHealth()) {
        int newX, newY;
        for (auto dir : directions) {
            newX = x_position + dir.first * strength;
            newY = y_position + dir.second * strength;
            if (_room.isPositionValid(newX, newY) && _room.isPositionFree(newX, newY)) {
                _room.moveEntityOneMoreStep(this, newX, newY);
                x_position = newX;
                y_position = newY;
                health -= strength;
                checkMove = true;
                break;
            }
        }
    if (!checkMove) {
        vector<pair<int, int>> validMoves;
        for (auto dir : directions) {
            newX = x_position + dir.first;
            newY = y_position + dir.second;
            if (_room.isPositionValid(newX, newY) && _room.isPositionFree(newX, newY)) {
                validMoves.push_back(make_pair(newX, newY));
            }
        }
        if (!validMoves.empty()) {
            auto randomMove = validMoves[rand() % validMoves.size()];
            _room.moveEntityOneMoreStep(this, randomMove.first, randomMove.second);
            x_position = randomMove.first;
            y_position = randomMove.second;
            health -= strength;
        }
    }
    } else {
        _room.removeEntity(this);
    }
}

    void showInfo(){
        cout << "Type: " << _type << endl;
        cout << "Health: " << health << endl;
        cout << "Strength: " << strength << endl;
    }
};

// Method to set up the initial state of the room
void Room::setInitialRoom() {

    srand(time(0));

    // Create stones
    for (int i = 0; i < 2; i++) {
        int stoneX = rand() % 10;
        int stoneY = rand() % 10;
        while (!isPositionFree(stoneX, stoneY)) {
            stoneX = rand() % 10;
            stoneY = rand() % 10;
        }
        new Stones(*this, stoneX, stoneY);
    }

    // Create holes
    for (int i = 0; i < 2; i++) {
        int holeX = rand() % 10;
        int holeY = rand() % 10;
        while (!isPositionFree(holeX, holeY)) {
            holeX = rand() % 10;
            holeY = rand() % 10;
        }
        new Hole(*this, holeX, holeY);
    }

    // Create humans
    string names[] = {"Harold", "David", "Clare"};
    for (int i = 0; i < 3; i++) {
        int humanX = rand() % 10;
        int humanY = rand() % 10;
        while (!isPositionFree(humanX, humanY)) {
            humanX = rand() % 10;
            humanY = rand() % 10;
        }
        new Human(*this, humanX, humanY, names[i]);
    }
    
    // Create dragons
    for (int i = 0; i < 2; i++) {
        int dragonX = rand() % 10;
        int dragonY = rand() % 10;
        while (!isPositionFree(dragonX, dragonY)) {
            dragonX = rand() % 10;
            dragonY = rand() % 10;
        }
        new Dragon(*this, dragonX, dragonY);
    }

    //Create monsters
    for (int i = 0; i < 3; i++) {
        int monsterX = rand() % 10;
        int monsterY = rand() % 10;
        while (!isPositionFree(monsterX, monsterY)) {
            monsterX = rand() % 10;
            monsterY = rand() % 10;
        }
        Monster* monster = new Monster(*this, monsterX, monsterY);
        monster->strengthSetter(rand() % 6);
        
    }
}

// Method to move all animated entities in the room
void Room::moveAllEntities() {
    vector<Entities*> entitiesToMove;


    // Collect all entities that can move
    for (auto& row : room) {
        for (auto& entity : row) {
            if (entity != nullptr && entity->statusGetter()) {
                entitiesToMove.push_back(entity);
            }
        }
    }

    // Move each entity
    for (auto& entity : entitiesToMove) {
        entity->Movement();
    }
}

// Method to reset the room to its initial state
void Room::resetRoom(){
    for (int row = 0; row < room.size(); row++) {
        for (int column = 0; column < room[row].size(); column++) {
            if (room[row][column] != nullptr) {
                delete room[row][column];
                room[row][column] = nullptr;
            }
        }
    }
    room.clear();
    room = vector<vector<Entities*>>(10, vector<Entities*>(10, nullptr));
    setInitialRoom();
}


int main() {
    srand(time(0));
    Room mainRoom;
    mainRoom.setInitialRoom();

    // Menu user
    while (true) {
        int choice;
        cout << "**************WELCOME**************" << endl;
        cout << "**********MYSTERY ROOOM**********" << endl;
        cout << "1. Display room" << endl;
        cout << "2. Move all the animated entities" << endl;
        cout << "3. Display entity state by coordinates" << endl;
        cout << "4. Reset the room" << endl;
        cout << "5. Finish" << endl;
        cout << "Enter your option: ";
        cin >> choice;
        if (choice == 1) {
            mainRoom.showMap();
        } else if (choice == 2) {
            mainRoom.moveAllEntities();
        } else if (choice == 3) {
            mainRoom.showInfo();
        } else if (choice == 4) {
            mainRoom.resetRoom();
        } else if (choice == 5) {
            cout << "See you next time" << endl;
            break;
        }
    }
    return 0;
}