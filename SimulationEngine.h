#ifndef SIMULATION_ENGINE_H
#define SIMULATION_ENGINE_H

#include "Creature.h"
#include "Food.h"
#include "Types.h"
#include <vector>

using std::vector;

class SimulationEngine
{
private:
    const int SCREEN_WIDTH;
    const int SCREEN_HEIGHT;
    const int MAX_FOOD;
    
public:
    // Constructor
    SimulationEngine(int screen_w = 800, int screen_h = 600, int max_food = 120);
    
    // Update all creatures
    void update_creatures(vector<Creature*>& creatures,vector<Food>& foods);
    
    // Handle eating collisions (herbivores eat food)
    void handle_eating_collisions(vector<Creature*>& creatures,vector<Food>& foods,int& total_eaten);
    
    // Handle predator hunting (predators hunt herbivores)
    void handle_predator_hunting(vector<Creature*>& creatures);
    
    // Handle creature deaths
    void handle_deaths(vector<Creature*>& creatures, int& total_died);
    
    // Handle births (reproduction)
    void handle_births(vector<Creature*>& creatures, int& total_born);
    
    // Manage food ecosystem (respawning)
    void manage_food_ecosystem(vector<Food>& foods);
    
    // Calculate statistics
    SimulationStats calculate_statistics(const vector<Creature*>& creatures,const vector<Food>& foods,int total_born,int total_died, int total_eaten);
    
    // Display debug information
    void display_debug_info(const vector<Creature*>& creatures, bool show_debug);
};

#endif