//==============EvolutionEngineGA.h (created for H1) it manages advanced genetic algorithm-based reproduction
#ifndef EVOLUTION_ENGINE_GA_H
#define EVOLUTION_ENGINE_GA_H

#include "GeneticAlgorithm.h"
#include "Creature.h"
#include "Food.h"
#include <vector>

using std::vector;

class EvolutionEngineGA
{
private:
    int generation_number;
    bool use_ga_reproduction;
    
//statistics tracking
    vector<double> fitness_history;
    vector<double> max_fitness_history;
    
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    
public:
    //constructor
    EvolutionEngineGA(int screen_w = 800, int screen_h = 600);
    
    // enable or disable genetic algorithm reproduction
    void set_ga_mode(bool enabled);
    bool is_ga_enabled() const;
    
    // Handle reproduction using genetic algorithms
    void handle_ga_reproduction(vector<Creature*>& creatures, int& total_born);
    
    //handle simple mutation-only reproduction
    void handle_simple_reproduction(vector<Creature*>& creatures, int& total_born);
    
    //reproduction handler
    void handle_reproduction(vector<Creature*>& creatures, int& total_born);
    
    //getters
    int get_generation() const;
    double get_fitness_improvement() const;
    double get_current_fitness() const;
    double get_best_fitness() const;
    
    //display evolution report 
    void print_evolution_report() const;
};

#endif