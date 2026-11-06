#include "Omnivore.h"
#include "Herbivore.h"
#include <vector>
#include <string>

using namespace std;

// Constructor
Omnivore::Omnivore(double start_x, double start_y): Creature(start_x, start_y)
{
    // Omnivores have blue color 
    creature_color = rgb_color(50, 100, 200); 
}

// Constructor for offspring
Omnivore::Omnivore(double start_x, double start_y, double parent_speed, double parent_size, double parent_sense_radius, int parent_gen): Creature(start_x, start_y, parent_speed, parent_size, parent_sense_radius, parent_gen)
{
    // Similar blue to parent
    creature_color = rgb_color(50 + rnd(50), 100 + rnd(50), 200);
}

//try to eat food and hunt
void Omnivore::update(vector<Food>& foods)
{
    // first try to find food 
    Food* target_food = find_nearest_food(foods);
    
    if (target_food)
    {
        //move towards it
        move_towards(target_food->get_position());
    }
    else
    {
        wander();
    }

    //move
    position.x += velocity_x;
    position.y += velocity_y;

    //  energy consumption 
    consume_energy(get_movement_energy_cost());
}

// Draw the omnivore as a blue square with energy bar
void Omnivore::draw() const
{
    double half_size = size;
    fill_rectangle(creature_color, position.x - half_size,position.y - half_size,half_size * 2,half_size * 2);
    
    draw_energy_bar();
}

color Omnivore::get_color() const
{
    return creature_color;
}

CreatureType Omnivore::get_type() const
{
    return CreatureType::OMNIVORE;
}

string Omnivore::get_type_name() const
{
    return "Omnivore";
}

// Check if omnivore has enough energy to reproduce (virtual method) (between Herbivore and Predator)
bool Omnivore::can_reproduce() const
{
    return energy >= 110;  
}

// Create offspring  (virtual method)
Creature* Omnivore::reproduce()
{
    Omnivore* baby = new Omnivore(position.x, position.y, speed, size, sense_radius, generation);
    consume_energy(get_reproduction_cost());
    return baby;
}

// Get reproduction cost (virtual method) (between Herbivore and Predator)
double Omnivore::get_reproduction_cost() const
{
    return 45; 
}

//Get movement energy cost ,   consume  energy between Herbivore and Predator
double Omnivore::get_movement_energy_cost() const
{
    return speed * 0.04; 
}

// Try to eat food ( virtual method)
bool Omnivore::try_eat_food(Food& food)
{
    if (can_eat(food))
    {
        eat(food);
        return true;
    }
    return false;
}

// Try to hunt prey ( virtual method)
void Omnivore::try_hunt(vector<Creature*>& creatures)
{
    hunt(creatures);
}


// food eating methods (from Herbivore)

bool Omnivore::can_eat(const Food& food) const
{
    if (!food.is_active())
    {
        return false;
    }
    
    Position food_pos = food.get_position();
    double dist = position.distance(food_pos);
    return dist < (size + food.get_size());
}

void Omnivore::eat(Food& food)
{
    if (can_eat(food))
    {
        food.set_active(false);
        add_energy(25); 
    }
}

//hunting methods (from Predator) 

void Omnivore::hunt(vector<Creature*>& creatures)
{
    Creature* prey = find_nearest_prey(creatures);
    
    if (prey != nullptr)
    {
        if (can_catch(prey))
        {
            catch_prey(prey);
        }
        else
        {
            move_towards(prey->get_position());
        }
    }
}

bool Omnivore::can_catch(const Creature* creature) const
{
    if (creature == this) 
    {
        return false;
    }
    if (creature->is_dead()) 
    {
        return false;
    }

    //  hunt herbivores  only
    if (creature->get_type() != CreatureType::HERBIVORE)
    {
        return false;
    }
    
    Position prey_pos = creature->get_position();
    double dist = position.distance(prey_pos);
    return dist < (size + creature->get_size());
}

void Omnivore::catch_prey(Creature* creature)
{
    if (can_catch(creature))
    {
        double prey_energy = creature->get_energy();
        creature->consume_energy(prey_energy); 
        add_energy(prey_energy * 0.4);
    }
}

Creature* Omnivore::find_nearest_prey(vector<Creature*>& creatures)
{
    Creature* nearest = nullptr;
    double nearest_dist = sense_radius;
    
    for (int i = 0; i < creatures.size(); i++)
    {
        if (creatures[i] != this && !creatures[i]->is_dead() && creatures[i]->get_type() == CreatureType::HERBIVORE)
        {
            Position prey_pos = creatures[i]->get_position();
            double dist = position.distance(prey_pos);
            
            if (dist < nearest_dist)
            {
                nearest_dist = dist;
                nearest = creatures[i];
            }
        }
    }
    
    return nearest;
}