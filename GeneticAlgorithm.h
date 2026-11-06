#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include "Creature.h"
#include "Food.h"
#include "Types.h"
#include <vector>

using std::vector;

// used for advanced evolution which can implements tournament selection, crossover, elitism, and fitness calculation
class GeneticAlgorithm
{
public:
    //tournament selection 
    static const int TOURNAMENT_SIZE;
    
    //elitism 
    static const double ELITE_PERCENTAGE;
    
    //calculate“fitness ”combining speed, size, and energy
    static double calculate_fitness(const Creature* creature);
    
    // biased Uniform Crossover : creates offspring from two parents where traits selected independently with probability weighted by fitness
    static Creature* biased_crossover(Creature* parent1, Creature* parent2, double x, double y);
    
    // tournament Selection :local competition for reproduction which randomly pick k creatures, one with greatest fitness wins
    static Creature* tournament_selection(const vector<Creature*>& population);
    
    // elitism : preserve top 10% across generations 
    static void elitism(vector<Creature*>& population);
    
    static double get_average_fitness(const vector<Creature*>& population);

    static double get_max_fitness(const vector<Creature*>& population);
};

#endif