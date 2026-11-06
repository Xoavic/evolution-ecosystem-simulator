#include "Creature.h"
#include "Food.h"
#include <cmath>
#include <vector>

using namespace std;

// Constructor to initialize a creature with given position
Creature::Creature(double start_x, double start_y)
{
    position.x = start_x;
    position.y = start_y;

    // random size between 8 - 15
    size = 8 + rnd(7);

    // random speed between 1 - 3
    speed = 1 + rnd(3);

    // random sense radius between 50 - 100
    sense_radius = 50 + rnd(50);

    // random velocity direction
    Position random_direction ((rnd(200) - 100)/100.0, (rnd(200) - 100)/100.0);
    Position normalized = random_direction.normalized();
    velocity_x = normalized.x * speed;
    velocity_y = normalized.y * speed;

    // random color
    creature_color = rgb_color(rnd(255), rnd(255), rnd(255));

    // initial energy
    energy = 50 + rnd(50);

    // first generation
    generation = 1;
}

// Constructor for the offspring for my iteration 2 (inherits parent traits with possible mutation)
Creature::Creature(double start_x, double start_y, double parent_speed, double parent_size, double parent_sense_radius, int parent_gen)
{
    position.x = start_x;
    position.y = start_y;

    // mutation for my iteration 2 ( 45% chance of mutation in traits)
    const double MUTATION_RATE = 45.0;

    // mutation for size
    if (rnd(100) < MUTATION_RATE)
    {
        // mutate size slightly ( random change between -2.5 to +2.5)
        size = parent_size + (rnd(100) - 50) / 20.0;
        if (size < 5)
        {
            size = 5; // minimum size
        }
        if (size > 20)
        {
            size = 20; // maximum size
        }
    }
    else
    {
        size = parent_size;
    }

    // mutation for speed
    if (rnd(100) < MUTATION_RATE)
    {
        // mutate speed slightly ( random change between -0.5 to +0.5)
        speed = parent_speed + (rnd(100) - 50) / 100.0;
        if (speed < 0.5)
        {
            speed = 0.5; // minimum speed
        }
        if (speed > 5.0)
        {
            speed = 5.0; // maximum speed
        } 
    }
    else
    {
        speed = parent_speed;
    }

    // mutation for sense radius
    if (rnd(100) < MUTATION_RATE)
    {
        // mutate sense radius slightly ( random change between -10 to +10)
        sense_radius = parent_sense_radius + (rnd(200) - 100) / 10.0;
        if (sense_radius < 30)
        {
            sense_radius = 30; // minimum sense radius
        }
        if (sense_radius > 150)
        {
            sense_radius = 150; // maximum sense radius
        }
    }
    else
    {
        sense_radius = parent_sense_radius;
    }

    // random velocity direction (offspring  doesn't inherit parent's direction)
    Position random_direction ((rnd(200) - 100)/100.0, (rnd(200) - 100)/100.0);
    Position normalized = random_direction.normalized();
    velocity_x = normalized.x * speed;
    velocity_y = normalized.y * speed;

    // Similar color to parent (slightly different from the parent)
    creature_color = rgb_color(100 + rnd(155), 100 + rnd(155), 100 + rnd(155));

    // Offspring starts with moderate energy
    energy = 50;

    // Next generation
    generation = parent_gen + 1;
}

// find nearest food within sense radius
Food *Creature::find_nearest_food(vector<Food> &foods)
{
    Food *nearest_food = nullptr;
    double nearest_distance = sense_radius;

    for (int i = 0; i < foods.size(); i++)
    {
        if (foods[i].is_active())
        {
            Position food_pos = foods[i].get_position();
            double dist = position.distance(food_pos);

            if (dist < nearest_distance)
            {
                nearest_distance = dist;
                nearest_food = &foods[i];
            }
        }
    }
    return nearest_food;
}

// move towards target position (chasing food)
void Creature::move_towards(const Position &target)
{
    // Calculate direction vector
    Position direction = target - position;

    double dist = direction.length();

    double final_speed = speed;

    // start to decelarate when within 40 pixels of the target; when very close, move at minimum 50% speed
    if (dist > 40.0)
    {
        final_speed = speed;
    }
    else
    { 
       double factor = dist / 40.0;
    
       if (factor < 0.5)
       {
        factor = 0.5;
       }

        final_speed = speed * factor;
    }

    // Normalize direction and set velocity
    Position normalized = direction.normalized();
    velocity_x = normalized.x * final_speed;
    velocity_y = normalized.y * final_speed;
}

// wandering randomly behavior ( no target)
void Creature::wander()
{
    // Continuous smooth turning by using jitter strength

    double jitter = 0.15;
    
    // Add small random changes to current velocity
    velocity_x += (rnd(200) - 100) / 100.0 * jitter;
    velocity_y += (rnd(200) - 100) / 100.0 * jitter;
    
    Position velocity_vector(velocity_x, velocity_y);
    Position normalized = velocity_vector.normalized();

    velocity_x = normalized.x * speed;
    velocity_y = normalized.y * speed;
}

void Creature::draw_energy_bar() const
{
    // Draw the energy bar above creature , and green bar  represents the current energy
    double bar_width = size * 2;
    double bar_height = 3;
    double bar_x = position.x - bar_width / 2;
    double bar_y = position.y - size - 8;

    // Background (gray)
    fill_rectangle(COLOR_GRAY, bar_x, bar_y, bar_width, bar_height);

    // Energy bar (green to yellow to red based on energy level)
    double energy_percent = energy / 100.0;
    if (energy_percent > 1.0)
    {
        energy_percent = 1.0;
    }
    if (energy_percent < 0.0)
    {
        energy_percent = 0.0;
    }

    color bar_color;

    if (energy_percent > 0.5)
        bar_color = COLOR_GREEN;
    else if (energy_percent > 0.25)
        bar_color = COLOR_YELLOW;
    else
        bar_color = COLOR_RED;

    fill_rectangle(bar_color, bar_x, bar_y, bar_width * energy_percent, bar_height);
}

// Check bounds: to check and set to opposite if going beyond screen edges
void Creature::check_bounds(int screen_width, int screen_height)
{
    // touch the left or right edge
    if (position.x - size < 0 || position.x + size > screen_width)
    {
        velocity_x = -velocity_x;
    }

    // touch the  top or bottom edge
    if (position.y - size < 0 || position.y + size > screen_height)
    {
        velocity_y = -velocity_y;
    }

    // keep creature inside bounds
    if (position.x - size < 0)
    {
        position.x = size;
    }
    if (position.x + size > screen_width)
    {
        position.x = screen_width - size;
    }
    if (position.y - size < 0)
    {
        position.y = size;
    }
    if (position.y + size > screen_height)
    {
        position.y = screen_height - size;
    }
}

// virtual methods with default implementation
bool Creature::is_dead() const
{
    return energy <= 0;
}

double Creature::get_movement_energy_cost() const
{
// Movement cost proportional to speed
    return speed * 0.03;
}


// energy management

double Creature::get_energy() const
{
    return energy;
}

void Creature::consume_energy(double amount)
{
    energy -= amount;
    if (energy < 0)
    {
        energy = 0;
    }
}

void Creature::add_energy(double amount)
{
    energy += amount;
    if (energy > 200)
    {
        energy = 200;
    }
}

// getters for collision detection in the future

double Creature::get_x() const
{
    return position.x;
}

double Creature::get_y() const
{
    return position.y;
}

double Creature::get_size() const
{
    return size;
}

double Creature::get_speed() const
{
    return speed;
}

double Creature::get_sense_radius() const
{
    return sense_radius;
}
int Creature::get_generation() const
{
    return generation;
}

Position Creature::get_position() const
{
    return position;
}