// ============InputHandlerGA.h (modified for H1)============
#ifndef INPUT_HANDLERGA_H
#define INPUT_HANDLERGA_H

#include "splashkit.h"
#include "Creature.h"
#include "Food.h"
#include "EvolutionEngineGA.h"  // new for H1
#include <vector>

using std::vector;

class InputHandlerGA
{
private:
    bool& paused;
    bool& show_debug;
    int& simulation_speed;
    
    //new for H1: reference to EvolutionEngineGA
    EvolutionEngineGA& evolution_engine;
    
    const int SCREEN_WIDTH;
    const int SCREEN_HEIGHT;
    
public:
    //Constructor (now with a new reference parameter )
    InputHandlerGA(bool& pause_state, bool& debug_state, int& sim_speed,EvolutionEngineGA& evo_engine,int screen_w = 800, int screen_h = 600);
    
    // Process keyboard input (now includes G key for GA toggle)
    void process_keyboard(vector<Creature*>& creatures,vector<Food>& foods,int& total_born,int& total_died,int& total_eaten,vector<double>& speed_history);
    
    // Process mouse input
    void process_mouse(vector<Creature*>& creatures, vector<Food>& foods);
    
private:
    // Reset simulation to initial state
    void reset_simulation(vector<Creature*>& creatures,vector<Food>& foods,int& total_born,int& total_died,int& total_eaten,vector<double>& speed_history);
};

#endif