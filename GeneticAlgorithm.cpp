#include "GeneticAlgorithm.h"
#include "Herbivore.h"
#include "Predator.h"
#include "Omnivore.h"
#include "Utilities.h"
#include <cmath>

using std::vector;

const int GeneticAlgorithm::TOURNAMENT_SIZE = 5;
const double GeneticAlgorithm::ELITE_PERCENTAGE = 0.10;

// calculate fitness
double GeneticAlgorithm::calculate_fitness(const Creature *creature)
{
    if (creature->is_dead())
    {
        return 0.0;
    }

    // normalize each component to 0-1 range (max speed about 5, max size about 15, max energy about 150)
    double speed_score = creature->get_speed() / 5.0;
    double size_score = creature->get_size() / 15.0;
    double energy_score = creature->get_energy() / 150.0;

    // adjust scores to not exceed 1.0
    if (speed_score > 1.0)
    {
        speed_score = 1.0;
    }

    if (size_score > 1.0)
    {
        size_score = 1.0;
    }

    if (energy_score > 1.0)
    {
        energy_score = 1.0;
    }

    // weighted combination (where speed matters most)
    double fitness = (speed_score * 0.4) + (size_score * 0.3) + (energy_score * 0.3);

    // bonus for surviving longer (for higher generation)
    fitness = fitness * (1.0 + creature->get_generation() * 0.05);

    if (fitness < 0.0)
    {
        fitness = 0.0;
    }

    return fitness;
}

// biased- uniform crossover
Creature *GeneticAlgorithm::biased_crossover(Creature *parent1, Creature *parent2, double x, double y)
{
    // calculate fitness weights
    double fitness1 = calculate_fitness(parent1);
    double fitness2 = calculate_fitness(parent2);
    double total_fitness = fitness1 + fitness2;

    // avoid division by zero
    if (total_fitness < 0.001)
    {
        fitness1 = 0.5;
        fitness2 = 0.5;
        total_fitness = 1.0;
    }

    // calculate probability of selecting from parent1
    double p1 = fitness1 / total_fitness;

    // select each trait independently with fitness-weighted probability
    double child_speed;

    if (rnd() < p1)
    {
        child_speed = parent1->get_speed();
    }

    else
    {
        child_speed = parent2->get_speed();
    }

    double child_size;

    if (rnd() < p1)
    {
        child_size = parent1->get_size();
    }

    else
    {
        child_size = parent2->get_size();
    }

    double child_sense_radius;

    if (rnd() < p1)
    {
        child_sense_radius = parent1->get_sense_radius();
    }

    else
    {
        child_sense_radius = parent2->get_sense_radius();
    }

    // determine creature type based on fitter parent
    CreatureType child_type;

    if (fitness1 >= fitness2)
    {
        child_type = parent1->get_type();
    }
    else
    {
        child_type = parent2->get_type();
    }

    // take average generation and increment
    int parent_gen = (parent1->get_generation() + parent2->get_generation()) / 2;

    // create offspring
    Creature *offspring = nullptr;

    if (child_type == CreatureType::HERBIVORE)
    {
        offspring = new Herbivore(x, y, child_speed, child_size, child_sense_radius, parent_gen);
    }
    else if (child_type == CreatureType::PREDATOR)
    {
        offspring = new Predator(x, y, child_speed, child_size, child_sense_radius, parent_gen);
    }
    else
    {
        offspring = new Omnivore(x, y, child_speed, child_size, child_sense_radius, parent_gen);
    }

    return offspring;
}

// Tournament Selection
Creature *GeneticAlgorithm::tournament_selection(const vector<Creature *> &population)
{
    if (population.empty())
    {
        return nullptr;
    }

    // handle with small populations
    int actual_k = TOURNAMENT_SIZE;

    if (actual_k > population.size())
    {
        actual_k = population.size();
    }

    // create tournament by random sampling comepetitors
    vector<Creature *> tournament;

    for (int i = 0; i < actual_k; i++)
    {
        int random_index = rnd(population.size());
        tournament.push_back(population[random_index]);
    }

    // find fittest in tournament
    Creature *winner = tournament[0];
    double best_fitness = calculate_fitness(winner);

    for (int i = 1; i < tournament.size(); i++)
    {
        double fitness = calculate_fitness(tournament[i]);
        if (fitness > best_fitness)
        {
            best_fitness = fitness;
            winner = tournament[i];
        }
    }

    return winner;
}

// elitism
void GeneticAlgorithm::elitism(vector<Creature *> &population)
{
    if (population.empty())
    {
        return;
    }

    // calculate number of elite members （ if population is less than 10, then just leave one to elite)
    int elite_count = (int)(population.size() * ELITE_PERCENTAGE);
    if (elite_count == 0 && !population.empty())
    {
        elite_count = 1;
    }

    // create pairs of (creature,fitness) for sorting
    vector<int> indices;
    vector<double> fitnesses;

    for (int i = 0; i < population.size(); i++)
    {
        if (!population[i]->is_dead())
        {
            indices.push_back(i);
            fitnesses.push_back(calculate_fitness(population[i]));
        }
    }

    // simple  selection sort to find top elite_count
    for (int i = 0; i < elite_count && i < indices.size(); i++)
    {
        int best_index = i;

        // sort for the remaining creatures to get elite
        for (int j = i + 1; j < indices.size(); j++)
        {
            if (fitnesses[j] > fitnesses[best_index])
            {
                best_index = j;
            }
        }

        // swap to ensure index and fitness are consistent.
        if (best_index != i)
        {
            int temp_index = indices[i];
            indices[i] = indices[best_index];
            indices[best_index] = temp_index;

            double temp_fit = fitnesses[i];
            fitnesses[i] = fitnesses[best_index];
            fitnesses[best_index] = temp_fit;
        }
    }
    // boost elite energy to increase survival rate
    for (int i = 0; i < elite_count && i < indices.size(); i++)
    {
        population[indices[i]]->add_energy(50);
    }
}

// get average fitness
double GeneticAlgorithm::get_average_fitness(const vector<Creature *> &population)
{
    if (population.empty())
    {
        return 0.0;
    }

    double total = 0.0;
    int count = 0;

    for (int i = 0; i < population.size(); i++)
    {
        if (!population[i]->is_dead())
        {
            total += calculate_fitness(population[i]);
            count++;
        }
    }

    if (count > 0)
    {
        return total / count;
    }
    else
    {
        return 0.0;
    }
}

// get maximum fitness
double GeneticAlgorithm::get_max_fitness(const vector<Creature *> &population)
{
    double max_fit = 0.0;

    for (int i = 0; i < population.size(); i++)
    {
        if (!population[i]->is_dead())
        {
            double fit = calculate_fitness(population[i]);
            if (fit > max_fit)
            {
                max_fit = fit;
            }
        }
    }

    return max_fit;
}