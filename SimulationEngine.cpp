#include "SimulationEngine.h"
#include "Herbivore.h"
#include "Predator.h"
#include "Omnivore.h"
#include "Utilities.h"
#include <vector>
#include <string>


using std::vector;
using std::string;
using std::to_string;

// Constructor
SimulationEngine::SimulationEngine(int screen_w, int screen_h, int max_food)
    : SCREEN_WIDTH(screen_w), SCREEN_HEIGHT(screen_h), MAX_FOOD(max_food)
{
}

void SimulationEngine::update_creatures(vector<Creature *> &creatures, vector<Food> &foods)
{
    for (int i = 0; i < creatures.size(); i++)
    {
        creatures[i]->update(foods);
        creatures[i]->check_bounds(SCREEN_WIDTH, SCREEN_HEIGHT);
    }
}

void SimulationEngine::handle_eating_collisions(vector<Creature *> &creatures, vector<Food> &foods, int &total_eaten)
{
    for (int i = 0; i < creatures.size(); i++)
    {

        for (int j = 0; j < foods.size(); j++)
        {
            if (creatures[i]->try_eat_food(foods[j]))
            {
                total_eaten++;
            }
        }
    }
}

void SimulationEngine::handle_predator_hunting(vector<Creature *> &creatures)
{
    for (int i = 0; i < creatures.size(); i++)
    {
        creatures[i]->try_hunt(creatures);
    }
}

void SimulationEngine::handle_deaths(vector<Creature *> &creatures, int &total_died)
{
    for (int i = creatures.size() - 1; i >= 0; i--)
    {
        if (creatures[i]->is_dead())
        {
            delete creatures[i];
            creatures.erase(creatures.begin() + i);
            total_died++;
        }
    }
}

void SimulationEngine::handle_births(vector<Creature *> &creatures, int &total_born)
{
    vector<Creature *> babies;

    for (int i = 0; i < creatures.size(); i++)
    {
        if (creatures[i]->can_reproduce())
        {
            Creature *baby = creatures[i]->reproduce();
            babies.push_back(baby);
            total_born++;
        }
    }

    // Add babies to population
    for (int i = 0; i < babies.size(); i++)
    {
        creatures.push_back(babies[i]);
    }
}

void SimulationEngine::manage_food_ecosystem(vector<Food> &foods)
{
    // Remove inactive food
    for (int i = foods.size() - 1; i >= 0; i--)
    {
        if (!foods[i].is_active())
        {
            foods.erase(foods.begin() + i);
        }
    }

    // Randomly spawn new food （12% chance）
    if (foods.size() < MAX_FOOD)
    {
        if (rnd(100) < 12)
        {
            Position pos = random_position(SCREEN_WIDTH, SCREEN_HEIGHT);
            foods.push_back(Food(pos.x, pos.y));
        }
    }
}

SimulationStats SimulationEngine::calculate_statistics(const vector<Creature *> &creatures, const vector<Food> &foods, int total_born, int total_died, int total_eaten)
{
    SimulationStats stats;

    stats.herbivore_count = 0;
    stats.predator_count = 0;
    stats.omnivore_count = 0;
    double total_h_speed = 0;
    double total_p_speed = 0;
    double total_o_speed = 0;
    int max_gen = 0;

    for (int i = 0; i < creatures.size(); i++)
    {
        if (creatures[i]->get_type() == CreatureType::HERBIVORE)
        {
            stats.herbivore_count++;
            total_h_speed += creatures[i]->get_speed();
        }
        else if (creatures[i]->get_type() == CreatureType::PREDATOR)
        {
            stats.predator_count++;
            total_p_speed += creatures[i]->get_speed();
        }
        else if (creatures[i]->get_type() == CreatureType::OMNIVORE)
        {
            stats.omnivore_count++;
            total_o_speed += creatures[i]->get_speed();
        }

        if (creatures[i]->get_generation() > max_gen)
        {
            max_gen = creatures[i]->get_generation();
        }
    }

    stats.population_count = creatures.size();
    stats.total_born = total_born;
    stats.total_died = total_died;
    stats.total_eaten = total_eaten;
    stats.max_generation = max_gen;

    // calculate averages
    if (stats.herbivore_count > 0)
    {
        stats.average_herbivore_speed = total_h_speed / stats.herbivore_count;
    }
    else
    {
        stats.average_herbivore_speed = 0.0;
    }

    if (stats.predator_count > 0)
    {
        stats.average_predator_speed = total_p_speed / stats.predator_count;
    }
    else
    {
        stats.average_predator_speed = 0.0;
    }

    if (stats.omnivore_count > 0)
    {
        stats.average_omnivore_speed = total_o_speed / stats.omnivore_count;
    }
    else
    {
        stats.average_omnivore_speed = 0.0;
    }

    if (stats.population_count > 0)
    {
        stats.average_speed = (total_h_speed + total_p_speed + total_o_speed) / stats.population_count;
    }
    else
    {
        stats.average_speed = 0.0;
    }

    // Count active food
    stats.food_count = 0;
    for (int i = 0; i < foods.size(); i++)
    {
        if (foods[i].is_active())
        {
            stats.food_count++;
        }
    }

    return stats;
}

void SimulationEngine::display_debug_info(const vector<Creature *> &creatures,bool show_debug)
{
    if (!show_debug)
        return;

    int y_coordinate = 400;
    draw_text("=== DEBUG MODE ===", COLOR_BLUE, 10, y_coordinate);
    y_coordinate += 20;

    int h_count = 0;
    int p_count = 0;
    int o_count = 0;

    // Show info for first 3 herbivores and 2 predators 2 omnivores
    for (int i = 0; i < creatures.size() && (h_count < 3 || p_count < 2 || o_count < 2); i++)
    {
        if (creatures[i]->get_type() == CreatureType::HERBIVORE && h_count < 3)
        {
            draw_circle(COLOR_GREEN,creatures[i]->get_x(),creatures[i]->get_y(),creatures[i]->get_sense_radius());

            draw_text("H" + to_string(h_count) +" Energy:" + to_string((int)creatures[i]->get_energy()) + " Speed:" + to_string(creatures[i]->get_speed()),COLOR_BLUE, 10, y_coordinate);
            y_coordinate += 18;
            h_count++;
        }
        else if (creatures[i]->get_type() == CreatureType::PREDATOR && p_count < 2)
        {
            draw_circle(COLOR_RED, creatures[i]->get_x(), creatures[i]->get_y(), creatures[i]->get_sense_radius());

            draw_text("P" + to_string(p_count) + " Energy:" + to_string((int)creatures[i]->get_energy()) + " Speed:" + to_string(creatures[i]->get_speed()),COLOR_BLUE, 10, y_coordinate);
            y_coordinate += 18;
            p_count++;
        }

        else if (creatures[i]->get_type() == CreatureType::OMNIVORE && o_count < 2)
        {
            draw_circle(COLOR_BLUE, creatures[i]->get_x(), creatures[i]->get_y(), creatures[i]->get_sense_radius());

            draw_text("O" + to_string(o_count) + " Energy:" + to_string((int)creatures[i]->get_energy()) + " Speed:" + to_string(creatures[i]->get_speed()),COLOR_BLUE, 10, y_coordinate);
            y_coordinate += 18;
            o_count++;
        }
    }
}