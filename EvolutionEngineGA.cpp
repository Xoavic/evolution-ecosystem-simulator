//==============EvolutionEngineGA.cpp(created for H1)==============
#include "EvolutionEngineGA.h"
#include "Herbivore.h"
#include "Predator.h"
#include "Omnivore.h"
#include "Utilities.h"

using std::to_string;
using std::vector;

EvolutionEngineGA::EvolutionEngineGA(int screen_w, int screen_h)
{
    generation_number = 0;
    use_ga_reproduction = true;
    SCREEN_WIDTH = screen_w;
    SCREEN_HEIGHT = screen_h;
}

void EvolutionEngineGA::set_ga_mode(bool enabled)
{
    use_ga_reproduction = enabled;
}

bool EvolutionEngineGA::is_ga_enabled() const
{
    return use_ga_reproduction;
}

void EvolutionEngineGA::handle_ga_reproduction(vector<Creature *> &creatures, int &total_born)
{
    if (creatures.empty())
    {
        return;
    }

    // record fitness for tracking
    double avg_fitness = GeneticAlgorithm::get_average_fitness(creatures);
    double max_fitness = GeneticAlgorithm::get_max_fitness(creatures);
    fitness_history.push_back(avg_fitness);
    max_fitness_history.push_back(max_fitness);

    // apply elitism
    GeneticAlgorithm::elitism(creatures);

    // calculate how many offspring we need
    int current_alive = 0;
    for (int i = 0; i < creatures.size(); i++)
    {
        if (!creatures[i]->is_dead())
            current_alive++;
    }

    int target_population = 40;
    int offspring_needed = target_population - current_alive;

    if (offspring_needed < 0)
    {
        offspring_needed = 0;
    }
    if (offspring_needed > 15)
    {
        offspring_needed = 15;
    }

    //tournament selection + crossover to create offspring
    vector<Creature *> offspring_list;
    int offspring_created = 0;

    //use consecutive failure detection  and stop if we can't find valid parents after 10 tries 
    int failures = 0;
    int max_failures = 10;

    while (offspring_created < offspring_needed && failures < max_failures)
    {
        //tournament selection for both parents
        Creature *parent1 = GeneticAlgorithm::tournament_selection(creatures);
        Creature *parent2 = GeneticAlgorithm::tournament_selection(creatures);

        //validate parents
        bool valid = true;

        if (parent1 == nullptr || parent2 == nullptr)
        {
            valid = false;
        }

        if (valid)
        {
            if (parent1->get_energy() < 80 || parent2->get_energy() < 80)
            {
                valid = false;
            }
        }

        if (valid)
        {
            if (parent1->get_type() != parent2->get_type())
            {
                valid = false;
            }
        }

        //create offspring if valid
        if (valid)
        {
            Position p1_pos = parent1->get_position();
            double offset_x = rnd(40) - 20;
            double offset_y = rnd(40) - 20;

            // biased crossover 
            Creature *offspring = GeneticAlgorithm::biased_crossover(parent1, parent2,p1_pos.x + offset_x,p1_pos.y + offset_y);

            if (offspring != nullptr)
            {
                offspring_list.push_back(offspring);
                offspring_created++;
                total_born++;

                parent1->consume_energy(40);
                parent2->consume_energy(40);

                failures = 0;
            }
            else
            {
               
                failures++;
            }
        }
        else
        {
            // Invalid parents, increment failure counter
            failures++;
        }
    }

    // Step 5: Add offspring to population
    for (int i = 0; i < offspring_list.size(); i++)
    {
        creatures.push_back(offspring_list[i]);
    }

    generation_number++;
}

void EvolutionEngineGA::handle_simple_reproduction(vector<Creature *> &creatures, int &total_born)
{
    vector<Creature *> offspring;

    for (int i = 0; i < creatures.size(); i++)
    {
        if (creatures[i]->can_reproduce())
        {

            Creature *baby = creatures[i]->reproduce();
            if (baby != nullptr)
            {
                offspring.push_back(baby);
                total_born++;
            }
        }
    }

    for (int i = 0; i < offspring.size(); i++)
    {
        creatures.push_back(offspring[i]);
    }
}

void EvolutionEngineGA::handle_reproduction(vector<Creature *> &creatures, int &total_born)
{
    if (use_ga_reproduction)
    {
        handle_ga_reproduction(creatures, total_born);
    }
    else
    {
        handle_simple_reproduction(creatures, total_born);
    }
}

// Getters
int EvolutionEngineGA::get_generation() const
{
    return generation_number;
}

double EvolutionEngineGA::get_fitness_improvement() const
{
    if (fitness_history.size() < 2)
    {
        return 0.0;
    }

    int last = fitness_history.size() - 1;
    return fitness_history[last] - fitness_history[0];
}

double EvolutionEngineGA::get_current_fitness() const
{
    if (fitness_history.empty())
    {
        return 0.0;
    }

    return fitness_history[fitness_history.size() - 1];
}

double EvolutionEngineGA::get_best_fitness() const
{
    if (max_fitness_history.empty())
    {
        return 0.0;
    }

    return max_fitness_history[max_fitness_history.size() - 1];
}

void EvolutionEngineGA::print_evolution_report() const
{
    write_line("");
    write_line("=== EVOLUTION REPORT ===");

    if (use_ga_reproduction)
    {
        write_line("Mode: GENETIC ALGORITHM");
    }
    else
    {
        write_line("Mode: MUTATION ONLY");
    }

    write_line("Generations: " + to_string(generation_number));

    if (!fitness_history.empty())
    {
        write_line("Initial avg fitness: " + to_string(fitness_history[0]));
        write_line("Final avg fitness: " + to_string(get_current_fitness()));
        write_line("Improvement: " + to_string(get_fitness_improvement()));
        write_line("Best fitness achieved: " + to_string(get_best_fitness()));
    }

    write_line("============================");
    write_line("");
}