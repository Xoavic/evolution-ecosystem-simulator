#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "splashkit.h"
#include "Creature.h"
#include "Food.h"
#include <vector>

using std::vector;

class InputHandler
{
private:
    bool& paused;
    bool& show_debug;
    int& simulation_speed;
    
    const int SCREEN_WIDTH;
    const int SCREEN_HEIGHT;
    
public:
    // Constructor
    InputHandler(bool& pause_state, bool& debug_state, int& sim_speed,int screen_w = 800, int screen_h = 600);
    
    // Process keyboard input
    void process_keyboard(vector<Creature*>& creatures,vector<Food>& foods,int& total_born,int& total_died,int& total_eaten,vector<double>& speed_history);
    
    // Process mouse input
    void process_mouse(vector<Creature*>& creatures,vector<Food>& foods);
    
private:
   //Reset simulation to initial state
    void reset_simulation(vector<Creature*>& creatures,vector<Food>& foods,int& total_born,int& total_died,int& total_eaten,vector<double>& speed_history);
};

#endif