#ifndef CREATURE_H
#define CREATURE_H

#include "splashkit.h"
#include "Types.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

class Food;

class Creature
{
protected:
    // position of the creature
    Position position;

    // velocity of the creature
    double velocity_x;
    double velocity_y;

    // Attributes (Genetic  traits for the future generation)
    double size;
    double speed;
    double sense_radius;
    int generation;

    // Energy system
    double energy;

    // color of the creature
    color creature_color;

    // Autonomous behavior state
    Food* find_nearest_food(vector<Food> &foods);
    void move_towards(const Position &target);
    void wander();
    void draw_energy_bar() const;
    
    

public:
    // constructor
    Creature(double start_x, double start_y);
    Creature(double start_x, double start_y, double parent_speed, double parent_size, double parent_sense_radius, int parent_gen);

    // destructor
    virtual ~Creature() {}

    // update methods
    virtual void update(vector <Food> &foods ) = 0;
    virtual void draw() const = 0;
    virtual color get_color() const = 0;
    virtual CreatureType get_type() const = 0;
    virtual string get_type_name() const = 0;

    // reproduction methods
    virtual bool can_reproduce() const = 0;
    virtual Creature* reproduce() = 0;
    virtual double get_reproduction_cost() const = 0;


    virtual bool is_dead() const;
    virtual double get_movement_energy_cost() const ;

    virtual bool try_eat_food(Food& food) { return false; }
    virtual void try_hunt(vector<Creature*>& creatures) {  }

    void check_bounds(int screen_width, int screen_height);

    // energy management
    double get_energy() const;
    void consume_energy(double amount);
    void add_energy(double amount);

    // getters for collision detection in the future
    double get_x() const;
    double get_y() const;
    double get_size() const;
    double get_speed() const;
    double get_sense_radius() const;
    int get_generation() const;
    Position get_position() const;
};

#endif