#ifndef OMNIVORE_H
#define OMNIVORE_H

#include "Creature.h"
#include "Food.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

// Omnivore class which inherits from Creature and can eat both food and hunt creatures (herbivores)
class Omnivore : public Creature
{
public:
    // Constructors
    Omnivore(double start_x, double start_y);
    Omnivore(double start_x, double start_y, double parent_speed,  double parent_size, double parent_sense_radius, int parent_gen);

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
    bool try_eat_food(Food& food) override;
    void try_hunt(vector<Creature*>& creatures) override;

private:
    // Food eating methods (from Herbivore)
    bool can_eat(const Food& food) const;
    void eat(Food& food);
    
    // Hunting methods (from Predator)
    void hunt(std::vector<Creature*>& creatures);
    bool can_catch(const Creature* creature) const;
    void catch_prey(Creature* creature);
    Creature* find_nearest_prey(std::vector<Creature*>& creatures);
};

#endif