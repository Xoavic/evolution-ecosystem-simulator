#ifndef HERBIVORE_H
#define HERBIVORE_H

#include "Creature.h"
#include "Food.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

// Herbivore class which inherits from Creature
class Herbivore : public Creature
{
public:
    // Constructors
    Herbivore(double start_x, double start_y);
    Herbivore(double start_x, double start_y, double parent_speed, double parent_size, double parent_sense_radius, int parent_gen);

    // Implement basic virtual functions from Creature
    void update(vector<Food>& foods) override;
    void draw() const override;
    color get_color() const override;
    CreatureType get_type() const override;
    string get_type_name() const override; 
    
    // Implement virtual reproduction methods
    bool can_reproduce() const override;
    Creature* reproduce() override;
    double get_reproduction_cost() const override;
    
    // Override virtual methods with default implementation
    double get_movement_energy_cost() const override;

    bool try_eat_food(Food& food) override;

    // specific methods
    bool can_eat(const Food& food) const;
    void eat(Food& food);
};

#endif