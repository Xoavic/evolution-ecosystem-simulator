#include "Food.h"

// Constructor to initialize food with given position
Food::Food(double pos_x, double pos_y)
{
    position.x = pos_x;
    position.y = pos_y;
    
    // food properties 
    // fixed size for simplicity
    size = 5;  

    // random nutrition value between 20 - 30
    nutrition = 20 + rnd(10);  

    // initially active (available to be eaten)
    active = true; 
    
    // visual properties 
    food_color = rgb_color(50 + rnd(100), 150 + rnd(100), 50 + rnd(50));
}

// Draw: Draw the food as a small circle
void Food::draw()
{
    // Only draw if food is active (not eaten yet)
    if (active)
    {
        fill_circle(food_color, position.x, position.y, size);
    }
}

  // state management

bool Food::is_active() const
{
    return active;
}

void Food::set_active(bool state)
{
    active = state;
}

// getters for collision detection in the future

double Food::get_x() const
{
    return position.x;
}

double Food::get_y() const
{
    return position.y;
}

double Food::get_size() const
{
    return size;
}

double Food::get_nutrition() const
{
    return nutrition;
}

Position Food::get_position() const
{
    return position;
}

