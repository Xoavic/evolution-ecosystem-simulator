#include "Herbivore.h"

using namespace std;

// Constructor
Herbivore::Herbivore(double start_x, double start_y): Creature(start_x, start_y)
{
    // Herbivores（colour mainly green）
    creature_color = rgb_color(50 + rnd(100), 150 + rnd(100), 50 + rnd(100));
}

// Constructor for offspring
Herbivore::Herbivore(double start_x, double start_y, double parent_speed, double parent_size, double parent_sense_radius, int parent_gen): Creature(start_x, start_y, parent_speed, parent_size, parent_sense_radius, parent_gen)
{
    // Similar green to parent
    creature_color = rgb_color(50 + rnd(100), 150 + rnd(100), 50 + rnd(100));
}

// autonomous behavior - chase food or wander
void Herbivore::update(vector<Food>& foods)
{
    Food* target = find_nearest_food(foods);

    if (target != nullptr)
    {
        Position target_pos = target->get_position();
        move_towards(target_pos);
    }
    else
    {
        wander();
    }

    // move
    position.x += velocity_x;
    position.y += velocity_y;
    
    // energy consumption
    consume_energy(get_movement_energy_cost());
}

//  Draw the herbivore as a circle （mainly green）with energy bar
void Herbivore::draw() const
{
    fill_circle(creature_color, position.x, position.y, size);
    draw_energy_bar();
}

color Herbivore::get_color() const
{
    return creature_color;
}

CreatureType Herbivore::get_type() const
{
    return CreatureType::HERBIVORE;
}

string Herbivore::get_type_name() const
{
    return "Herbivore";
}

// Get movement energy cost (virtual method)
double Herbivore::get_movement_energy_cost() const
{
    return speed * 0.03; 
}


// Check if herbivore has enough energy to reproduce (virtual method)
bool Herbivore::can_reproduce() const
{
    return energy > 100;
}

// Get reproduction cost (virtual method)
double Herbivore::get_reproduction_cost() const
{
    return 40.0;
}

// Create offspring (virtual method)
Creature* Herbivore::reproduce()
{
    // Create the baby at its parent's position with inherited traits
    Herbivore* baby = new Herbivore(position.x, position.y, speed, size, sense_radius, generation);
    consume_energy(get_reproduction_cost());
    return baby; 
}

// Try to eat food ( virtual method)
bool Herbivore::try_eat_food(Food& food)
{
    if (can_eat(food))
    {
        eat(food);
        return true;
    }
    return false;
}

// check if herbivore can eat the food (collision detection)
bool Herbivore::can_eat(const Food& food) const
{
    if (!food.is_active())
    {
        return false;
    }

    Position food_pos = food.get_position();
    double dist = position.distance(food_pos);

    return dist < (size + food.get_size());
}

// Herbivore eat food and get energy
void Herbivore::eat(Food& food)
{
    if (can_eat(food))
    {
        add_energy(food.get_nutrition());
        food.set_active(false);
    }
}