//============================================================================
// Name        : .cpp
// Author      : Dr. Sibt Ul Hussain
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game...
//============================================================================

#ifndef RushHour_CPP_
#define RushHour_CPP_
#include "util.h"
#include <iostream>
#include<string>
#include<cmath> // for basic math functions such as cos, sin, sqrt
using namespace std;


int secondsPassed = 0;
int minutesPassed = 0;

// seed the random numbers generator by current time (see the documentation of srand for further help)...

/* Function sets canvas size (drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */
void SetCanvasSize(int width, int height) {
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}


int xI = 400, yI = 400;

void drawCar(){
	DrawSquare(xI, yI, 20, colors[BLUE]);
	glutPostRedisplay();
}
bool flag = true;
void moveCar() {
	if (xI > 10 && flag) {
		xI -= 10;
		cout << "going left";
		if(xI < 100)
			
			flag = false;

	}
	else if (xI < 1010 && !flag) {
		cout << "go right";
		xI += 10;
		if (xI > 900)
			flag = true;
	}
}




// Forward declarations
class Board;
class Game;
class NPC_cars;
class Package;
class Passenger;
class Vehicle;

// Class for checking if a cell is blocked (to break circular dependency)
class occupied_check {
public:
    virtual bool occupied(int x, int y) const = 0;
    virtual ~occupied_check() {}
};

class Vehicle {
protected:
    int position_x, position_y;
    float fuel;
    std::string direction;
    std::string color;
    const int draw_start = 100;
    const int box_gap = 30;

public:
    Vehicle(int x = 0, int y = 0, float f = 100.0f, std::string dir = "UP", std::string clr = "WHITE")
        : position_x(x), position_y(y), fuel(f), direction(dir), color(clr) {}

    Vehicle& operator=(const Vehicle& other) {
        if (this != &other) {
            position_x = other.position_x;
            position_y = other.position_y;
            fuel = other.fuel;
            direction = other.direction;
            color = other.color;
        }
        return *this;
    }

    virtual void draw() const {
        int px = draw_start + position_x * box_gap;
        int py = draw_start + position_y * box_gap;
        DrawSquare(px, py, box_gap, colors[WHITE]);
    }

    virtual void move(int dx, int dy, Board& board);

    void use_fuel(float fuel_amount) {
        fuel -= fuel_amount;
        if (fuel < 0) fuel = 0;
    }

    bool collided(int x, int y) const {
        return (position_x == x && position_y == y);
    }

    int getter_posX() const {
         return position_x; 
        }
    int getter_posY() const { 
        return position_y; 
    }
    float getter_fuel() const { 
        return fuel; }
};

class NPC_cars : public Vehicle {
private:
    int direction;

public:
    NPC_cars() : Vehicle(), direction(0) {}
    void init_random(Board& board);
    void random_move(Board& board);
    void draw() const override;
};


class Passenger {
private:
    int pickup_X, pickup_Y;
    int dropoff_X, dropoff_Y;
    bool waiting;
    bool picked_up;

public:
    Passenger(int px = 0, int py = 0)
        : pickup_X(px), pickup_Y(py), dropoff_X(0), dropoff_Y(0), waiting(true), picked_up(false) {}

    void make_destination(const occupied_check& checker, int grid_size = 20) {
        do {
            dropoff_X = GetRandInRange(0, grid_size - 1);
            dropoff_Y = GetRandInRange(0, grid_size - 1);
        } while ((dropoff_X == pickup_X && dropoff_Y == pickup_Y) || checker.occupied(dropoff_X, dropoff_Y));
    }

    int getter_pickupX() const { 
        return pickup_X; }
    int getter_pickupY() const { return pickup_Y; }
    int getter_dropoffX() const { 
        return dropoff_X; 
    }
    int getter_dropoffY() const { 
        return dropoff_Y;
     }
    bool is_waiting() const { return waiting; }
    bool is_in_taxi() const { return picked_up; }
    void set_waiting(bool w) { 
        waiting = w; 
    }
    void set_in_taxi(bool t) { 
        picked_up = t; 
    }

    void respawn(const occupied_check& checker, int grid_size = 20) {
        do {
            pickup_X = GetRandInRange(0, grid_size - 1);
            pickup_Y = GetRandInRange(0, grid_size - 1);
        } while (checker.occupied(pickup_X, pickup_Y));

        make_destination(checker);
        waiting = true;
        picked_up = false;
    }
};

class Package {
private:
    int pickup_X, pickup_Y;
    int dropoff_X, dropoff_Y;
    bool ready;
    bool picked_up;

public:
    Package(int px = 0, int py = 0)
        : pickup_X(px), pickup_Y(py), dropoff_X(0), dropoff_Y(0), ready(true), picked_up(false) {}

    void make_destination(const occupied_check& checker, int grid_size = 20) {
        do {
            dropoff_X = GetRandInRange(0, grid_size - 1);
            dropoff_Y = GetRandInRange(0, grid_size - 1);
        } while ((dropoff_X == pickup_X && dropoff_Y == pickup_Y) || checker.occupied(dropoff_X, dropoff_Y));
    }

    int getter_pickupX() const { 
        return pickup_X; 
    }
    int getter_pickupY() const { 
        return pickup_Y;
     }
    int getter_dropoffX() const { 
        return dropoff_X; 
    }
    int getter_dropoffY() const { 
        return dropoff_Y; 
    }
    bool is_ready() const { 
        return ready; }
    bool is_in_delivery() const { 
        return picked_up;
     }
    void set_ready(bool r) { ready = r; }
    void set_in_delivery(bool d) { picked_up = d; }

    void respawn(const occupied_check& checker, int grid_size = 20) {
        do {
            pickup_X = GetRandInRange(0, grid_size - 1);
            pickup_Y = GetRandInRange(0, grid_size - 1);
        } while (checker.occupied(pickup_X, pickup_Y));

        make_destination(checker);
        ready = true;
        picked_up = false;
    }
};



class Board : public occupied_check {
private:
    char grid[20][20];
    const int draw_start = 100;
    const int box_gap = 30;
    char role = 'T';
    NPC_cars npcs[3];

    static const int max_passengers = 4;
    Passenger passengers[max_passengers];
    int active_passengers;

 
    static const int max_packages = 3;
    Package packages[max_packages];
    int active_packages;


    int dest_x[max_passengers], dest_y[max_passengers];
    bool dest_current[max_passengers];


    void place_switch_station() {
        grid[0][0] = 'S';
    }

    void build_grid() {
        for (int i = 0; i < 20; i++)
            for (int j = 0; j < 20; j++)
                grid[i][j] = 'R';
    }

    bool is_extreme_corner(int i, int j) {
        return (i == 0 && j == 0) || (i == 0 && j == 19) || 
               (i == 19 && j == 0) || (i == 19 && j == 19);
    }

    bool is_empty(int i, int j) const {
        return grid[i][j] == 'R';
    }

    bool is_corner(int i, int j) const {
        return (i == 0 && j == 0) || (i == 0 && j == 19) || 
               (i == 19 && j == 0) || (i == 19 && j == 19);
    }

    bool is_blocked(int i, int j) const {
        return occupied(i, j);
    }

    void place_random(char symbol, int count) {
        int placed = 0;
        while (placed < count) {
            int i = GetRandInRange(0, 19);
            int j = GetRandInRange(0, 19);
            if (is_empty(i, j) && !is_corner(i, j)) {
                grid[i][j] = symbol;
                placed++;
            }
        }
    }

    void spawn_passengers() {
        active_passengers = GetRandInRange(2, max_passengers);
        for (int i = 0; i < active_passengers; i++) {
            int px, py;
            do {
                px = GetRandInRange(0, 19);
                py = GetRandInRange(0, 19);
            } while (is_blocked(px, py));
            
            passengers[i] = Passenger(px, py);
            passengers[i].make_destination(*this);
            
            dest_x[i] = passengers[i].getter_dropoffX();
            dest_y[i] = passengers[i].getter_dropoffY();
            dest_current[i] = false;
        }
    }

    void spawn_packages() {
        active_packages = GetRandInRange(1, max_packages);
        for (int i = 0; i < active_packages; i++) {
            int px = -1, py = -1;
            for (int r = 0; r < 20 && px == -1; r++) {
                for (int c = 0; c < 20 && px == -1; c++) {
                    if (grid[r][c] == 'K') {
                        px = r;
                        py = c;
                        break;
                    }
                }
            }
            
            if (px != -1) {
                packages[i] = Package(px, py);
                packages[i].make_destination(*this);
            }
        }
    }

    void clear_grid() {
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 20; j++) {
                grid[i][j] = 'R';
            }
        }
    }

    void place_fixed_elements() {
        place_switch_station();
    }

    void init_passengers() {
        spawn_passengers();
    }

    void init_packages() {
        spawn_packages();
    }

    void init_board() {
        generate();
    }

public:
    void set_role(char r) { role = r; }
    void update_board(Game& game);

    int getter_npc_x(int index) const { 
        return npcs[index].getter_posX(); 
    }
    int getter_npc_y(int index) const { 
        return npcs[index].getter_posY(); 
    }

    Board() {
        generate();
    }

    void generate() {
        clear_grid();
        place_fixed_elements();
        place_random('B', (20 * 20 * 35) / 100);
        place_random('T', 10);
        place_random('F', 3);
        place_random('P', 3);
        place_random('K', 3);
        
        for (int i = 0; i < 3; i++) {
            npcs[i].init_random(*this);
        }
        
        init_passengers();
        init_packages();
    }

    void draw_objects() {
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 20; j++) {
                int x = draw_start + i * box_gap;
                int y = draw_start + j * box_gap;

                char type = grid[i][j];
                if (role == 'D' && type == 'P') 
                continue;
                if (role == 'T' && type == 'K') 
                continue;

                switch (type) {
                    case 'B': DrawSquare(x, y, box_gap, colors[BLACK]); break;
                    case 'T': DrawRoundRect(x, y, box_gap, box_gap, colors[DARK_OLIVE_GREEN], 20); break;
                    case 'F': DrawRoundRect(x, y, box_gap, box_gap, colors[RED], 10); break;
                    case 'P': DrawCircle(x + 15, y + 15, 10, colors[MAGENTA]); break;
                    case 'K': DrawSquare(x, y, box_gap, colors[BROWN]); break;
                    case 'S': DrawRoundRect(x, y, box_gap, box_gap, colors[GRAY], 15); break;
                    default: break;
                }
            }
        }

        if (role == 'T') {
            for (int i = 0; i < active_passengers; i++) {
                if (passengers[i].is_waiting()) {
                    int px = draw_start + passengers[i].getter_pickupX() * box_gap;
                    int py = draw_start + passengers[i].getter_pickupY() * box_gap;
                    DrawCircle(px + 15, py + 25, 5, colors[MAGENTA]);
                    DrawLine(px + 15, py + 20, px + 15, py + 10, 2, colors[MAGENTA]);
                    DrawLine(px + 15, py + 15, px + 10, py + 10, 2, colors[MAGENTA]);
                    DrawLine(px + 15, py + 15, px + 20, py + 10, 2, colors[MAGENTA]);
                    DrawLine(px + 15, py + 10, px + 10, py + 5, 2, colors[MAGENTA]);
                    DrawLine(px + 15, py + 10, px + 20, py + 5, 2, colors[MAGENTA]);
                }
            }
        }
        else if (role == 'D') {
            for (int i = 0; i < active_packages; i++) {
                if (packages[i].is_ready()) {
                    int px = draw_start + packages[i].getter_pickupX() * box_gap;
                    int py = draw_start + packages[i].getter_pickupY() * box_gap;
                    DrawSquare(px, py, box_gap, colors[BROWN]);
                }
            }
        }

        if (role == 'T') {
            for (int i = 0; i < active_passengers; i++) {
                if (passengers[i].is_in_taxi()) {
                    int dx = draw_start + passengers[i].getter_dropoffX() * box_gap;
                    int dy = draw_start + passengers[i].getter_dropoffY() * box_gap;
                    DrawSquare(dx, dy, box_gap, colors[GREEN]);
                }
            }
        } 
        else {
            for (int i = 0; i < active_packages; i++) {
                if (packages[i].is_in_delivery()) {
                    int dx = draw_start + packages[i].getter_dropoffX() * box_gap;
                    int dy = draw_start + packages[i].getter_dropoffY() * box_gap;
                    DrawSquare(dx, dy, box_gap, colors[GREEN]);
                }
            }
        }

        for (int i = 0; i < 3; i++) {
            npcs[i].draw();
        }
    }

    Passenger* get_passengers() { 
        return passengers; }
    int get_passenger_count() const { 
        return active_passengers; 
    
    }

    Package* get_packages() {
         return packages; }
    int get_package_count() const {
         return active_packages; }

    bool check_collision(int player_x, int player_y) {
        for (int i = 0; i < 3; i++) {
            if (player_x == npcs[i].getter_posX() && player_y == npcs[i].getter_posY())
                return true;
        }
        return false;
    }

    bool occupied(int x, int y) const override {
        if (x < 0 || x >= 20 || y < 0 || y >= 20) return true;
        return grid[x][y] == 'B' || grid[x][y] == 'T';
    }

    char get_cell_type(int i, int j) {
        if (i >= 0 && i < 20 && j >= 0 && j < 20)
            return grid[i][j];
        return 'X';
    }

    void reset() {
        init_board();
    }
};



class Taxi_driver : public Vehicle {
private:
    Passenger* m_passenger;
    double m_fare;
    bool m_pickup;
    bool m_dropoff;

public:
    Taxi_driver(int x = 0, int y = 19)
        : Vehicle(x, y, 100.0f, "UP", "YELLOW"),
          m_passenger(nullptr), m_fare(0.0), m_pickup(false), m_dropoff(false) {}

    void draw() const override {
        int px = draw_start + position_x * box_gap;
        int py = draw_start + position_y * box_gap;
        DrawSquare(px, py, box_gap, colors[YELLOW]);
    }

    bool calculate_fare() {
        if (m_passenger && m_dropoff) {
            m_fare = 20.0;
            return true;
        }
        return false;
    }

    bool pickup_passenger(Passenger* passenger, int player_x, int player_y) {
        if (passenger && !m_passenger && passenger->is_waiting() &&
            abs(player_x - passenger->getter_pickupX()) <= 1 &&
            abs(player_y - passenger->getter_pickupY()) <= 1) {

            m_passenger = passenger;
            passenger->set_waiting(false);
            passenger->set_in_taxi(true);
            m_pickup = true;
            return true;
        }
        return false;
    }

    bool dropoff_passenger(int player_x, int player_y) {
        if (m_passenger && m_passenger->is_in_taxi() &&
            player_x == m_passenger->getter_dropoffX() &&
            player_y == m_passenger->getter_dropoffY()) {

            m_passenger->set_in_taxi(false);
            m_dropoff = true;
            m_passenger = nullptr;
            return true;
        }
        return false;
    }

    double getter_fare() const { 
        return m_fare; 
    }
    bool has_picked_up() const { return m_pickup; 
    }
    bool has_dropped_off() const { 
        return m_dropoff; }
    Passenger* get_passenger() { 
        return m_passenger; 
    }
};


class Delivery_driver : public Vehicle {
private:
    Package* m_package;
    double m_fare;
    bool m_pickup;
    bool m_dropoff;

public:
    Delivery_driver(int x = 0, int y = 19)
        : Vehicle(x, y, 100.0f, "UP", "CYAN"),
          m_package(nullptr), m_fare(0.0), m_pickup(false), m_dropoff(false) {}

    void draw() const override {
        int px = draw_start + position_x * box_gap;
        int py = draw_start + position_y * box_gap;
        DrawSquare(px, py, box_gap, colors[CYAN]);
    }

    bool calculate_fare() {
        if (m_package && m_dropoff) {
            m_fare = 30.0;
            return true;
        }
        return false;
    }

    bool pickup_package(Package* package, int player_x, int player_y) {
        if (package && !m_package && package->is_ready() &&
            abs(player_x - package->getter_pickupX()) <= 1 &&
            abs(player_y - package->getter_pickupY()) <= 1) {

            m_package = package;
            package->set_ready(false);
            package->set_in_delivery(true);
            m_pickup = true;
            return true;
        }
        return false;
    }

    bool dropoff_package(int player_x, int player_y)
     {
        if (m_package && m_package->is_in_delivery() &&
            player_x == m_package->getter_dropoffX() &&
            player_y == m_package->getter_dropoffY()) {

            m_package->set_in_delivery(false);
            m_dropoff = true;
            m_package = nullptr;
            m_pickup = false;
            m_dropoff = false;
            return true;
        }
        return false;
    }

    double getter_fare() const {
         return m_fare; 
        }
    bool has_picked_up() const {
         return m_pickup;
         }
    bool has_dropped_off() const { 
        return m_dropoff; 
    }
    Package* get_package() { 
        return m_package; }
};


class Player : public Vehicle {
private:
    Taxi_driver m_taxi_driver;
    Delivery_driver m_delivery_driver;
    char m_role;
    bool m_picked = false;
    int m_score = 0;
    int m_money = 0;
    const int draw_start = 100;
    const int box_gap = 30;

public:
    Player() : m_taxi_driver(0, 19), m_delivery_driver(0, 19), m_role('T') {}

    void draw() const override {
        if (m_role == 'T') {
            m_taxi_driver.draw();
        } 
        else {
            m_delivery_driver.draw();
        }
    }

    void move(int dx, int dy, Board& board) override
     {
        if (m_role == 'T') {
            m_taxi_driver.move(dx, dy, board);
        } 
        else {
            m_delivery_driver.move(dx, dy, board);
        }
    }

    void role_switch(Board& board) 
    {
        int current_x = getter_X();
        int current_y = getter_Y();

        if (board.get_cell_type(current_x, current_y) == 'S') {
            if (m_role == 'T') {
                m_role = 'D';
            } 

            else {
                m_role = 'T';
            }

            board.set_role(m_role);
            m_picked = false;

            if (m_role == 'T') 
            {
                m_taxi_driver = Taxi_driver(current_x, current_y);
                m_taxi_driver.use_fuel(100.0f - m_delivery_driver.getter_fuel());
            }
             else {
                m_delivery_driver = Delivery_driver(current_x, current_y);
                m_delivery_driver.use_fuel(100.0f - m_taxi_driver.getter_fuel());
            }
        }
    }

    void interact(Board& board, Passenger* passengers, int passenger_count,
              Package* packages, int package_count){
        int x = getter_X();
        int y = getter_Y();

        if (m_role == 'T') {
            if (!m_picked) {
                for (int i = 0; i < passenger_count; i++) 
                {
                    if (m_taxi_driver.pickup_passenger(&passengers[i], x, y)) {
                        m_picked = true;
                        break;
                    }
                }
            } 
            else {
                Passenger* p = m_taxi_driver.get_passenger();
                if (p && m_taxi_driver.dropoff_passenger(x, y)) {
                    m_score += 10;
                    m_money += 10;
                    m_picked = false;
                    p->respawn(board);
                }
            }
        } 
        else {
            if (!m_picked) 
            {
                for (int i = 0; i < package_count; i++) {
                    if (m_delivery_driver.pickup_package(&packages[i], x, y)) {
                        m_picked = true;
                        break;
                    }
                }
            } 
            else {
                Package* p = m_delivery_driver.get_package();
                if (p && m_delivery_driver.dropoff_package(x, y)) {
                    m_score += 20;
                    m_money += 20;
                    m_picked = false;
                    p->respawn(board);
                }
            }
        }
    }

    void reset(){
        m_taxi_driver = Taxi_driver(0, 19);
        m_delivery_driver = Delivery_driver(0, 19);
        m_role = 'T';

        m_picked = false;
        m_score = 0;
        m_money = 0;
    }

    void use_fuel(float f1) {
        if (m_role == 'T') {
            m_taxi_driver.use_fuel(f1);
        } 
        else {
            m_delivery_driver.use_fuel(f1);
        }
    }

    void refuel(int amount){
        if (m_money >= amount){
            m_money -= amount;
            if (m_role == 'T'){
                m_taxi_driver.use_fuel(-50);
            } else {
                m_delivery_driver.use_fuel(-50);
            }
        }
    }

    int getter_X() const {
        if (m_role == 'T') {
            return m_taxi_driver.getter_posX();
        } 
        else {
            return m_delivery_driver.getter_posX();
        }
    }

    int getter_Y() const {
        if (m_role == 'T') {
            return m_taxi_driver.getter_posY();
        } 
        else {
            return m_delivery_driver.getter_posY();
        }
    }

    float getter_fuel() const {
        if (m_role == 'T') {
            return m_taxi_driver.getter_fuel();
        } 
        else 
        {
            return m_delivery_driver.getter_fuel();
        }
    }

    void score_add(int s1) { 
        m_score += s1; 
    }
    void money_add(int m1) { 
        m_money += m1; 
    }

    char getter_role() const { 
        return m_role; 
    }
    bool picked_up() const { 
        return m_picked; 
    }
    int getter_score() const { 
        return m_score; 
    }
    int getter_money() const { return m_money; 
    }

    bool has_picked_up() const 
    {
        if (m_role == 'T') {
            return m_taxi_driver.has_picked_up();
        } 
        else {
            return m_delivery_driver.has_picked_up();
        }
    }

    bool has_dropped_off() const {
        if (m_role == 'T') {
            return m_taxi_driver.has_dropped_off();
        } 
        else 
        {
            return m_delivery_driver.has_dropped_off();
        }
    }
};



class Game {
private:
    Board board;
    Player player;
    int seconds_elapsed;
    int minutes_elapsed;
    int state; // 0 = MENU, 1 = PLAYING, 2 = GAME_OVER
    char selected_role; 
    int tasks_done; // tracks completed jobs

public:
    Player& getter_player() { 
        
        return player; 
    }
    const Player& getter_player() const { return player; }

    int getter_tasks() const {
         return tasks_done; 
        }
    void tasks_plus() { 
        tasks_done++; 
    }

    Game() {
        state = 0;
        seconds_elapsed = 0;
        minutes_elapsed = 0;
        selected_role = 'T';
        tasks_done = 0;
        board.set_role('T');
    }

    int getter_player_x() const { 
        return player.getter_X(); 
    }
    int getter_player_y() const { 
        return player.getter_Y(); 
    }
    void reset_player() { player.reset(); 
    }

    void reset() {
        board.generate();
        player.reset();
        board.set_role(selected_role);

        if (selected_role == 'D') {
            player.role_switch(board);
        }

        seconds_elapsed = 0;
        minutes_elapsed = 0;
        state = 1;
        tasks_done = 0;
    }

    void update() {
        if (state == 1) {
            player.use_fuel(0.5f);
            seconds_elapsed++;
            if (seconds_elapsed >= 60){
                minutes_elapsed++;
                seconds_elapsed = 0;
            }

            if (player.getter_fuel() <= 0 || player.getter_score() < 0 || (minutes_elapsed == 3 && seconds_elapsed == 0)) {
                state = 2;
            }

            char cell = board.get_cell_type(player.getter_X(), player.getter_Y());
            if (cell == 'F' && player.getter_money() >= 20) {
                player.refuel(20);
            }
        }

        if (player.getter_score() >= 100) {
            state = 2;
        }
    }

    void menu() {
        DrawString(280, 500, "RUSH HOUR", colors[RED]);
        DrawString(220, 460, "Press 'T' to play as Taxi Driver", colors[YELLOW]);
        DrawString(220, 430, "Press 'D' to play as Delivery Driver", colors[CYAN]);
    }

    void game_UI() {
        string role_ui;
        if (player.getter_role() == 'T') {
            role_ui = "Taxi Driver";
        } else {
            role_ui = "Delivery Driver";
        }

        string time_ui = "Time: " + to_string(2 - minutes_elapsed) + ":";
        if (59 - seconds_elapsed < 10) {
            time_ui += "0";
        }
        time_ui += to_string(59 - seconds_elapsed);

        string fuel_ui = "Fuel: " + to_string((int)player.getter_fuel()) + "%";
        string score_ui = "Score: " + to_string(player.getter_score());
        string money_ui = "$" + to_string(player.getter_money());

        string status_ui = "";
        if (player.has_picked_up()) {
            if (player.getter_role() == 'T') {
                status_ui = "Passenger Picked Up!";
            } 
            else {
                status_ui = "Package Picked Up!";
            }
        }
         else if (player.has_dropped_off()) {
            if (player.getter_role() == 'T') {
                status_ui = "Passenger Dropped Off! +$10";
            } 
            else {
                status_ui = "Package Delivered! +$20";
            }
        }

        int y = 780;
        DrawString(20, y, role_ui, colors[WHITE]);
        DrawString(150, y, time_ui, colors[WHITE]);
        DrawString(280, y, fuel_ui, colors[WHITE]);
        DrawString(410, y, score_ui, colors[WHITE]);
        DrawString(540, y, money_ui, colors[WHITE]);

        if (!status_ui.empty()) {
            DrawString(670, y, status_ui, colors[GREEN]);
        }
    }

/*
 * Main Canvas drawing function.
 * */

    void display() {
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        const int draw_start = 100;
        const int draw_end = 700;
        const int box_gap = 30;

        if (state == 0) {
            menu();
        } else if (state == 1) {
            for (int x = draw_start; x < draw_end; x += box_gap) {

                for (int y = draw_start; y < draw_end; y += box_gap) {
                    DrawSquare(x, y, box_gap, colors[WHITE]);
                }
            }

            game_UI();
            player.draw();
            board.draw_objects();
            board.update_board(*this);
        } 
        else if (state == 2) {
            if (player.getter_score() >= 100) {
                DrawString(300, 500, "You Won!", colors[GREEN]);
                DrawString(270, 460, "Score: " + to_string(player.getter_score()), colors[WHITE]);
            } 
            else {
                DrawString(300, 500, "Game Over", colors[RED]);
                DrawString(270, 460, "Final Score: " + to_string(player.getter_score()), colors[WHITE]);
            }
            DrawString(250, 420, "Press 'T' or 'D' to Play Again", colors[WHITE]);
        }

        DrawLine(100, 100, 700, 100, 10, colors[BLUE]);
        DrawLine(100, 700, 700, 700, 10, colors[BLUE]);
        DrawLine(100, 100, 100, 700, 10, colors[BLUE]);
        DrawLine(700, 100, 700, 700, 10, colors[BLUE]);

        glutSwapBuffers(); // do not modify this line..
    }

    /*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 * You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 *
 * */
    void NonPrintableKeys(int key, int x, int y){
        if (state == 1){ 
            if (key == GLUT_KEY_LEFT) player.move(-1, 0, board);
            else if (key == GLUT_KEY_RIGHT) player.move(1, 0, board);
            else if (key == GLUT_KEY_UP) player.move(0, 1, board);
            else if (key == GLUT_KEY_DOWN) player.move(0, -1, board);
        }
        glutPostRedisplay();
    }

    // /*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
//  * is pressed from the keyboard
//  * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
//  * program coordinates of mouse pointer when key was pressed.
//  * */
// Global wrappers with same names that delegate to Game instance

    void PrintableKeys(unsigned char key, int x, int y){
        if (key == 27) exit(1); // Escape
        
        if (state == 0 || state == 2) {  // Menu or Game Over
            if (key == 't' || key == 'T'){
                selected_role = 'T';
                reset();
            } else if (key == 'd' ||key == 'D') {
                selected_role = 'D';
                reset();
            }
        } 
        else if (state == 1){  // Playing
            if (key == 'p'|| key == 'P') player.role_switch(board);
            if (key == 32) {  // Spacebar
                player.interact(board,
                              board.get_passengers(),
                              board.get_passenger_count(),
                              board.get_packages(),
                              board.get_package_count());
            }
        }
        glutPostRedisplay();
    }

  
/*
 * This function is called after every 1000.0/FPS milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */

    void Timer(int m) {
        moveCar(); 
        glutTimerFunc(1000, TimerWrapper, m);
        glutPostRedisplay();
    }

    static void TimerWrapper(int m) {
        if (instance) instance->Timer(m);
    }

   
    void MouseMoved(int x, int y) {
        cout << x << " " << y << endl;
        glutPostRedisplay();
    }

  /*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
 *
 * You will have to add the necessary code here for shooting, etc.
 *
 * This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
 * x & y that tells the coordinate of current position of move mouse
 *
 * */
    void MouseClicked(int button, int state, int x, int y) {
        if (button == GLUT_LEFT_BUTTON) {
            cout << GLUT_DOWN << " " << GLUT_UP << endl;
        } else if (button == GLUT_RIGHT_BUTTON) {
            cout << "Right Button Pressed" << endl;
        }
        glutPostRedisplay();
    }

    static Game* instance;
};


void NPC_cars::draw() const {
    int px = draw_start + position_x * box_gap;
    int py = draw_start + position_y * box_gap;
    DrawSquare(px, py, box_gap, colors[RED]);}

void Vehicle::move(int dx, int dy, Board& board) {
    int new_x = position_x + dx;
    int new_y = position_y + dy;
    if (new_x >= 0 && new_x < 20 && new_y >= 0 && new_y < 20 && !board.occupied(new_x, new_y)) {
        position_x = new_x;
        position_y = new_y;
    }
}

void Board::update_board(Game& game){
    set_role(game.getter_player().getter_role());
    
    // Move NPCs
    for (int i = 0; i < 3; i++) {
        npcs[i].random_move(*this);
    }

    // Check collisions
    for (int i = 0; i < 3; i++) {
        if (game.getter_player_x() == npcs[i].getter_posX() && 
            game.getter_player_y() == npcs[i].getter_posY()){
            game.reset_player();
            break;
        }
    }
}


void NPC_cars::init_random(Board& board){
    do {
        position_x = GetRandInRange(0, 19);
        position_y = GetRandInRange(0, 19);
    } while (board.occupied(position_x, position_y) || (position_x == 0 && position_y == 0));
    direction = GetRandInRange(0, 3);
}

void NPC_cars::random_move(Board& board){
    if (GetRandInRange(0, 3) == 0) {
        direction = GetRandInRange(0, 3);
    }

    int dx = 0, dy = 0;
    switch (direction) {
        case 0: dx = -1; break;
        case 1: dx = 1; break;
        case 2: dy = 1; break;
        case 3: dy = -1; break;
    }

    int new_x = position_x + dx;
    int new_y = position_y + dy;

    if (new_x >= 0 && new_x < 20 && new_y >= 0 && new_y < 20 && !board.occupied(new_x, new_y)){
        position_x = new_x;
        position_y = new_y;
    } else {
        direction = GetRandInRange(0, 3);
    }
}


Game* Game::instance = nullptr;
Game game;
void GameDisplay() {
    game.display();
}



void NonPrintableKeys(int key, int x, int y){
    if (Game::instance) Game::instance->NonPrintableKeys(key, x, y);
}

void PrintableKeys(unsigned char key, int x, int y) {
    if (Game::instance) Game::instance->PrintableKeys(key, x, y);
}

void Timer(int m){
    if (Game::instance) Game::instance->Timer(m);
}

void MouseMoved(int x, int y){
    if (Game::instance) Game::instance->MouseMoved(x, y);
}

void MouseClicked(int button, int state, int x, int y){
    if (Game::instance) Game::instance->MouseClicked(button, state, x, y);
}

/*This function is called (automatically) whenever your mouse moves witin inside the game window
 *
 * You will have to add the necessary code here for finding the direction of shooting
 *
 * This function has two arguments: x & y that tells the coordinate of current position of move mouse
 *
 * */
void MousePressedAndMoved(int x, int y){
	cout << x << " " << y << endl;
	glutPostRedisplay();
}


/*
 * our gateway main function
 * */
int main(int argc, char*argv[]) {
	
Game::instance = &game; // required for timer redirection

	int width = 800, height = 800; // i have set my window size to be 800 x 600
	InitRandomizer(); // seed the random number generator...
   // gameBoard.initializeBoard();
	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("Wasil's OOP Project"); // set the title of our game window
	SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	//glutDisplayFunc(display); // tell library which function to call for drawing Canvas.

	glutDisplayFunc(GameDisplay); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0, Timer, 0);

	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse
	glutMotionFunc(MousePressedAndMoved); // Mouse

	// now handle the control to library and it will call our registered functions when
	// it deems necessary...
    glutPostRedisplay(); // for menu

	glutMainLoop();
	return 1;
}
#endif /* RushHour_CPP_ */
