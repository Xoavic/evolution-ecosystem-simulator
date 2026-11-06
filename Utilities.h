#ifndef UTILITIES_H
#define UTILITIES_H

#include "Types.h"
#include"splashkit.h"
#include <vector>

using std::vector;

// tool functions for the simulator
Position random_position(int width, int height);
void draw_info_display(const SimulationStats& stats,int x, int y, bool paused);
void draw_icons(int x,int y);
void draw_controls (int x, int y);

//output functions
void display_initial_info (int herbivore_count, int predator_count, int omnivore_count, int food_count);
void display_final_statistics(int final_herbivores, int final_predators, int final_omnivores, int total_born, int total_died, int total_eaten, int frames );

//evolution analysis
void display_evolution_analysis(const vector<double>& speed_history);  
#endif
