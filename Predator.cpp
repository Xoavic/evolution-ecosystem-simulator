#include "Predator.h"
#include <cmath>
#include <vector>
#include <string>

using namespace std;

// Constructor
Predator::Predator(double start_x, double start_y): Creature(start_x, start_y)
{
   // Predators（colour mainly red）
    creature_color = rgb_color(150 + rnd(100), 50 + rnd(100), 50 + rnd(100));
    
    // Predators with bigger size, faster speed, better sense radius , more initial energy
    size = 12 + rnd(8);
    speed = (1.5 + rnd(3)) * 1.2; 
    sense_radius = 60 + rnd(60); 
    energy = 80 + rnd(40);
}

// Constructor for offspring
Predator::Predator(double start_x, double start_y, double parent_speed, double parent_size, double parent_sense_radius, int parent_gen): Creature(start_x, start_y, parent_speed, parent_size, parent_sense_radius, parent_gen)
{
    // Similar red   to parent
    creature_color = rgb_color(150 + rnd(100), 50 + rnd(100), 50 + rnd(100));
    energy = 80;
}

// find nearest prey (herbivore) within sense radius
Creature* Predator::find_nearest_prey(vector<Creature*>& creatures)
{
    Creature* nearest_prey = nullptr;
    double nearest_distance = sense_radius;

    for (int i = 0; i < creatures.size(); i++)
    {
        // hunt herbivores only  ,but not other predators or dead creatures
        if (creatures[i]->get_type() == CreatureType::HERBIVORE && !creatures[i]->is_dead())
        {
            Position prey_pos = creatures[i]->get_position();
            double dist = position.distance(prey_pos);

            if (dist < nearest_distance)
            {
                nearest_distance = dist;
                nearest_prey = creatures[i];
            }
        }
    }
    return nearest_prey;
}

// autonomous behavior - hunt or wander
void Predator::update(vector<Food>& foods)
{
    wander();

    // move
    position.x += velocity_x;
    position.y += velocity_y;
    
    // energy consumption 
    consume_energy(get_movement_energy_cost());
}

// hunt prey
void Predator::hunt(vector<Creature*>& creatures)
{
    Creature* prey = find_nearest_prey(creatures);
    
    if (prey != nullptr)
    {
        // Chase the prey
        Position target_pos = prey->get_position();
        move_towards(target_pos);
        
        // Try to catch
        if (can_catch(prey))
        {
            catch_prey(prey);
        }
    }
}

// Draw the predator as a triangle (mainly red) with energy bar
void Predator::draw() const
{
    double half_size = size;
    double height = half_size * 1.732;

    // Top point 
    double top_x = position.x;
    double top_y = position.y - height * 2.0 / 3.0;

    // Bottom left
    double left_x = position.x - half_size;
    double left_y = position.y + height * 1.0 / 3.0;

    // Bottom right
    double right_x = position.x + half_size;
    double right_y = position.y + height * 1.0 / 3.0;

    fill_triangle(creature_color,top_x, top_y, left_x, left_y,right_x, right_y);

    draw_energy_bar();
}

color Predator::get_color() const
{
    return creature_color;
}

CreatureType Predator::get_type() const
{
    return CreatureType::PREDATOR;
}

string Predator::get_type_name() const
{
    return "Predator";
}

// Get movement energy cost ,  predators consume more energy
double Predator::get_movement_energy_cost() const
{
    return speed * 0.05; 
}

// Try to hunt prey
void Predator::try_hunt(vector<Creature*>& creatures)
{
    hunt(creatures);
}

// Check if predator can catch the creature 
bool Predator::can_catch(const Creature* creature) const
{
    if (creature->is_dead() || creature->get_type() != CreatureType::HERBIVORE)
    {
        return false;
    }

    Position prey_pos = creature->get_position();
    double dist = position.distance(prey_pos);

    return dist < (size + creature->get_size());
}

// Predator catch and eat prey
void Predator::catch_prey(Creature* creature)
{
    if (can_catch(creature))
    {
        double gained_energy = creature->get_energy() * 0.5;
        add_energy(gained_energy);
        
        creature->consume_energy(creature->get_energy());
    }
}

// Check if predator has enough energy to reproduce (virtual method) (more than herbivores)
bool Predator::can_reproduce() const
{
    return energy > 120; 
}

// Get reproduction cost (virtual method) (more than herbivores)
double Predator::get_reproduction_cost() const
{
    return 50.0; 
}

// Create offspring  (virtual method)
Creature* Predator::reproduce()
{
    // Create the baby at its parent's position with inherited traits
    Predator* baby = new Predator(position.x, position.y, speed, size, sense_radius, generation);
    consume_energy(get_reproduction_cost());
    return baby; 
}