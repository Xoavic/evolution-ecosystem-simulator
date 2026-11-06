#include "InputHandler.h"
#include "Herbivore.h"
#include "Predator.h"
#include "Omnivore.h"
#include "Utilities.h"
#include <vector>

using std::vector;

// Constructor
InputHandler::InputHandler(bool& pause_state, bool& debug_state, int& sim_speed, int screen_w, int screen_h)
: paused(pause_state), show_debug(debug_state), simulation_speed(sim_speed),SCREEN_WIDTH(screen_w), SCREEN_HEIGHT(screen_h)
{

}

void InputHandler::process_keyboard(vector<Creature*>& creatures,vector<Food>& foods,int& total_born,int& total_died,int& total_eaten,vector<double>& speed_history)
{
    // Pause/Resume
    if (key_typed(SPACE_KEY))
    {
        paused = !paused;
    }
    
    // Speed control
    if (key_typed(PLUS_KEY) || key_typed(EQUALS_KEY))
    {
        if (simulation_speed < 5)
        {
            simulation_speed++;
        }
    }
    if (key_typed(MINUS_KEY))
    {
        if (simulation_speed > 1)
        {
            simulation_speed--;
        }
    }
    
    // Debug mode
    if (key_typed(D_KEY))
    {
        show_debug = !show_debug;
    }
    
    // Reset simulation
    if (key_typed(R_KEY))
    {
        reset_simulation(creatures, foods, total_born, total_died, total_eaten, speed_history);
    }
}

void InputHandler::process_mouse(vector<Creature*>& creatures,vector<Food>& foods)
{
    // Left click  willspawn herbivore or predator or omnivore
    if (mouse_clicked(LEFT_BUTTON))
    {
        double x = mouse_x();
        double y = mouse_y();

        if (key_down(LEFT_CTRL_KEY) || key_down(RIGHT_CTRL_KEY))
        {
            creatures.push_back(new Omnivore(x, y));
        }

        else if (key_down(LEFT_SHIFT_KEY) || key_down(RIGHT_SHIFT_KEY))
        {
            creatures.push_back(new Predator(x, y));
        }
        else
        {
            creatures.push_back(new Herbivore(x, y));
        }
    }

    // Right click willspawn food
    if (mouse_clicked(RIGHT_BUTTON))
    {
        double x = mouse_x();
        double y = mouse_y();
        foods.push_back(Food(x, y));
    }
}

void InputHandler::reset_simulation(vector<Creature*>& creatures,vector<Food>& foods,int& total_born,int& total_died,int& total_eaten,vector<double>& speed_history)
{
    // Clean up old creatures 
    for (int i = 0; i < creatures.size(); i++)
    {
        delete creatures[i];
    }
    creatures.clear();
    foods.clear();
    speed_history.clear();

    // Respawn initial population
    for (int i = 0; i < 20; i++)
    {
        Position pos = random_position(SCREEN_WIDTH, SCREEN_HEIGHT);
        creatures.push_back(new Herbivore(pos.x, pos.y));
    }

    for (int i = 0; i < 5; i++)
    {
        Position pos = random_position(SCREEN_WIDTH, SCREEN_HEIGHT);
        creatures.push_back(new Predator(pos.x, pos.y));
    }

    for (int i = 0; i < 3; i++)
    {
        Position pos = random_position(SCREEN_WIDTH, SCREEN_HEIGHT);
        creatures.push_back(new Omnivore(pos.x, pos.y));
    }

    for (int i = 0; i < 60; i++)
    {
        Position pos = random_position(SCREEN_WIDTH, SCREEN_HEIGHT);
        foods.push_back(Food(pos.x, pos.y));
    }

    // Reset statistics
    total_born = 28;
    total_died = 0;
    total_eaten = 0;
    paused = false;
}