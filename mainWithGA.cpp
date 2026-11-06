//==============main.cpp (modified for H1)==============
#include "splashkit.h"
#include "Herbivore.h"
#include "Predator.h"
#include "Omnivore.h"
#include "Food.h"
#include "Utilities.h"
#include "Types.h"
#include "InputHandlerGA.h"
#include "SimulationEngine.h"
#include "EvolutionEngineGA.h"  //new for H1
#include "GeneticAlgorithm.h"  //new for H1
#include <vector>

using namespace std;

// Screen constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Simulation state 
bool paused = false;
bool show_debug = false;
int simulation_speed = 1;

int main()
{
    // Open window
    open_window("Evolution Ecosystem Simulator H1: Genetic Algorithms",SCREEN_WIDTH, SCREEN_HEIGHT);

    // Create  objects
    SimulationEngine sim_engine(SCREEN_WIDTH, SCREEN_HEIGHT, 120);
    EvolutionEngineGA evolution_engine(SCREEN_WIDTH, SCREEN_HEIGHT); //new for H1
    
    //Create InputHandler (new  with EvolutionEngineGA reference)
    InputHandlerGA input_handler(paused, show_debug, simulation_speed,evolution_engine,SCREEN_WIDTH, SCREEN_HEIGHT);

    // Initialize creatures and food
    vector<Creature*> creatures;
    vector<Food> foods;

    // Spawn initial herbivores
    for (int i = 0; i < 20; i++)
    {
        Position pos = random_position(SCREEN_WIDTH, SCREEN_HEIGHT);
        creatures.push_back(new Herbivore(pos.x, pos.y));
    }

    // Spawn initial predators
    for (int i = 0; i < 5; i++)
    {
        Position pos = random_position(SCREEN_WIDTH, SCREEN_HEIGHT);
        creatures.push_back(new Predator(pos.x, pos.y));
    }

    // Spawn initial omnivores
    for (int i = 0; i < 3; i++)
    {
        Position pos = random_position(SCREEN_WIDTH, SCREEN_HEIGHT);
        creatures.push_back(new Omnivore(pos.x, pos.y));
    }

    // Spawn initial food
    for (int i = 0; i < 60; i++)
    {
        Position pos = random_position(SCREEN_WIDTH, SCREEN_HEIGHT);
        foods.push_back(Food(pos.x, pos.y));
    }

    // Statistics tracking
    int total_born = 28; 
    int total_died = 0;
    int total_eaten = 0;

    // Evolution tracking history
    vector<double> speed_history;
    int record_interval = 300;
    int frame_count = 0;

    // Display initial information
    SimulationStats initial_stats = sim_engine.calculate_statistics(  creatures, foods, total_born, total_died, total_eaten);
    display_initial_info(initial_stats.herbivore_count, initial_stats.predator_count,initial_stats.omnivore_count, foods.size());
    
    //Display GA mode info (new for H1)
    write_line("");
    write_line("=== H1: GENETIC ALGORITHMS ===");
    if (evolution_engine.is_ga_enabled())
    {
        write_line("GA Mode: ENABLED");
        write_line("- Tournament Selection (k=5)");
        write_line("- Biased Uniform Crossover");
        write_line("- Elitism (top 10%)");
    }
    else
    {
        write_line("GA Mode: DISABLED (baseline)");
    }
    write_line("Press 'G' to toggle GA mode");
    write_line("==============================");

    //main loop
    while (!quit_requested())
    {
        //process input
        process_events();
        input_handler.process_keyboard(creatures, foods, total_born,total_died, total_eaten, speed_history);
        input_handler.process_mouse(creatures, foods);
        
        //update simulation (only if not paused)
        if (!paused)
        {
            // Run simulation multiple times per frame based on speed
            for (int speed_step = 0; speed_step < simulation_speed; speed_step++)
            {
                // update all creatures
                sim_engine.update_creatures(creatures, foods);

                // handle predator hunting
                sim_engine.handle_predator_hunting(creatures);

                // handle eating collisions
                sim_engine.handle_eating_collisions(creatures, foods, total_eaten);

                // handle deaths
                sim_engine.handle_deaths(creatures, total_died);

                // new for H1: use EvolutionEngineGA for reproduction
                evolution_engine.handle_reproduction(creatures, total_born);

                // manage food ecosystem
                sim_engine.manage_food_ecosystem(foods);

                // evolution tracking
                if (frame_count % record_interval == 0 && creatures.size() > 0)
                {
                    SimulationStats current_stats = sim_engine.calculate_statistics(creatures, foods, total_born, total_died, total_eaten);
                    speed_history.push_back(current_stats.average_speed);
                    
                    //new for H1: Periodic fitness report (every 30s )
                    if (frame_count % 1800 == 0) 
                    {
                        write_line("GA Updates: " + to_string(evolution_engine.get_generation()) + " | Pop: " + to_string(creatures.size()) + " | Avg Fitness: " + to_string(evolution_engine.get_current_fitness()) + " | Best: " + to_string(evolution_engine.get_best_fitness()));
                    }
                }

                frame_count++;
            }
        }

        // clear screen
        clear_screen(COLOR_WHITE);

        // draw all food
        for (int i = 0; i < foods.size(); i++)
        {
            foods[i].draw();
        }

        // draw all creatures
        for (int i = 0; i < creatures.size(); i++)
        {
            creatures[i]->draw();
        }

        // calculate and display statistics
        SimulationStats stats = sim_engine.calculate_statistics(creatures, foods, total_born, total_died, total_eaten);
        draw_info_display(stats, 10, 10, paused);

        // draw UI 
        draw_icons(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 100);
        draw_controls(10, SCREEN_HEIGHT - 35);

        //new for H1: display GA status on screen
        int ga_y = 10;
        if (evolution_engine.is_ga_enabled())
        {
            draw_text("GA MODE: ACTIVE", rgb_color(0, 100, 255), SCREEN_WIDTH - 200, ga_y);
            ga_y += 20;
            
            draw_text("Fitness: " + to_string(evolution_engine.get_current_fitness()),rgb_color(0, 80, 200), SCREEN_WIDTH - 200, ga_y);
            ga_y += 20;
            
            draw_text("Best: " + to_string(evolution_engine.get_best_fitness()),rgb_color(0, 80, 200), SCREEN_WIDTH - 200, ga_y);
            ga_y += 20;
            
            draw_text("GA Updates: " + to_string(evolution_engine.get_generation()),rgb_color(0, 80, 200), SCREEN_WIDTH - 200, ga_y);
        }
        else
        {
            draw_text("GA MODE: OFF", COLOR_GRAY, SCREEN_WIDTH - 200, ga_y);
        }

        // debug info
        sim_engine.display_debug_info(creatures, show_debug);

        // refresh screen
        refresh_screen(60);
    }

    // display final statistics
    SimulationStats final_stats = sim_engine.calculate_statistics(creatures, foods, total_born, total_died, total_eaten);
    display_final_statistics(final_stats.herbivore_count,final_stats.predator_count,final_stats.omnivore_count, total_born, total_died, total_eaten, frame_count);
    display_evolution_analysis(speed_history);
    
    //new  for H1: Evolution report
    evolution_engine.print_evolution_report();

    // Clean up memory
    for (int i = 0; i < creatures.size(); i++)
    {
        delete creatures[i];
    }

    return 0;
}