#ifndef PREDATOR_H
#define PREDATOR_H

#include "Creature.h"
#include "Food.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

class Herbivore;

// Predator class which inherits from Creature
class Predator : public Creature
{
public:
    // Constructors
    Predator(double start_x, double start_y);
    Predator(double start_x, double start_y, double parent_speed, double parent_size, double parent_sense_radius, int parent_gen);

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
    
    // Override virtual methods with different implementation
    double get_movement_energy_cost() const override; 

    void try_hunt(vector<Creature*>& creatures) override;

    // specific methods 
    void hunt(vector<Creature*>& creatures);
    bool can_catch(const Creature* creature) const;
    void catch_prey(Creature* creature);

private:
    Creature* find_nearest_prey(vector<Creature*>& creatures);
};

#endif