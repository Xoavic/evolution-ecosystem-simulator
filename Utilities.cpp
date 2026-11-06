#include "Utilities.h"
#include <string>

using std::to_string;

// Generate the random position within screen bounds
Position random_position(int width, int height)
{
    return Position(rnd(width), rnd(height));
}

// Draw information display
void draw_info_display(const SimulationStats &stats, int x, int y, bool paused)
{
    int line_height = 20;
    int current_y = y;

    // Title
    draw_text("===Evolution Ecosystem Simulator===", COLOR_BLACK, x, current_y);
    current_y += line_height;

    // Population and food stats
    draw_text("Herbivores: " + to_string(stats.herbivore_count), COLOR_DARK_GREEN, x, current_y);
    current_y += line_height;

    draw_text("Predators: " + to_string(stats.predator_count), COLOR_DARK_RED, x, current_y);
    current_y += line_height;

    draw_text("Omnivores: " + to_string(stats.omnivore_count), COLOR_BLUE, x, current_y);
    current_y += line_height;

    draw_text("Food Available: " + to_string(stats.food_count), COLOR_ORANGE, x, current_y);
    current_y += line_height;

    // Birth and death stats
    draw_text("Born: " + to_string(stats.total_born), COLOR_BLUE, x, current_y);
    current_y += line_height;

    draw_text("Died: " + to_string(stats.total_died), COLOR_RED, x, current_y);
    current_y += line_height;

    draw_text("Eaten: " + to_string(stats.total_eaten), COLOR_PURPLE, x, current_y);
    current_y += line_height;

    // evolution stats
    draw_text("Herbivore Avg Speed: " + to_string(stats.average_herbivore_speed), COLOR_GREEN, x, current_y);
    current_y += line_height;

    draw_text("Predator Avg Speed: " + to_string(stats.average_predator_speed), COLOR_RED, x, current_y);
    current_y += line_height;

    draw_text("Omnivore Avg Speed: " + to_string(stats.average_omnivore_speed), COLOR_BLUE, x, current_y);
    current_y += line_height;

    draw_text("Max Generation: " + to_string(stats.max_generation), COLOR_BROWN, x, current_y);
    current_y += line_height + 5;

    // Paused status
    if (paused)
    {
        draw_text("=== PAUSED ===", COLOR_YELLOW, x, current_y);
        current_y += line_height;
    }
}

// Draw icons
void draw_icons(int x, int y)
{
    int line_height = 20;
    int current_y = y;

    draw_text("=== Icons ===", COLOR_BLACK, x, current_y);
    current_y += line_height;

    // Herbivore example
    fill_circle(rgb_color(100, 200, 100), x + 10, current_y + 5, 8);
    draw_text(" = Herbivore", COLOR_BLACK, x + 25, current_y);
    current_y += line_height;

    // Predator example
    fill_triangle(rgb_color(220, 50, 50), x + 10, current_y, x + 5, current_y + 10, x + 15, current_y + 10);
    draw_text(" = Predator", COLOR_BLACK, x + 25, current_y);
    current_y += line_height;

    // Omnivore example
    fill_rectangle(rgb_color(50, 100, 200), x + 5, current_y, 10, 10);
    draw_text(" = Omnivore", COLOR_BLACK, x + 25, current_y);
    current_y += line_height;

    // Food example
    fill_circle(rgb_color(50, 200, 50), x + 10, current_y + 5, 5);
    draw_text(" = Food", COLOR_BLACK, x + 25, current_y);
}

// Draw controls
void draw_controls(int x, int y)
{
    draw_text("Controls: SPACE=Pause  +/-=Speed  D=Debug  R=Reset G=GA mode", COLOR_GRAY, x, y);
    draw_text("Mouse: Left=Herbivore  Shift+Left=Predator  Ctrl+Left=Omnivore  Right=Food", COLOR_GRAY, x, y + 15);
}

// Display initial simulation information
void display_initial_info(int herbivore_count, int predator_count, int omnivore_count, int food_count)
{
    write_line("========================================");
    write_line("Evolution Ecosystem Simulator");
    write_line("========================================");
    write_line("Initial Herbivores: " + to_string(herbivore_count));
    write_line("Initial Predators: " + to_string(predator_count));
    write_line("Initial Omnivores: " + to_string(omnivore_count));
    write_line("Food Sources: " + to_string(food_count));
    write_line();
    write_line("Features:");
    write_line("- Collision detection (judge if creatures eat food)");
    write_line("- Death system");
    write_line("- Simple reproduction");
    write_line("- Natural food respawning (with random probability)");
    write_line("- Mutation with 45 percent chance on offspring traits");
    write_line("- Autonomous creature behavior (chase food or wander randomly)");
    write_line("- Evolution tracking");
    write_line("- User interaction (keyboard + mouse)");
    write_line("========================================");
    write_line("Controls:");
    write_line("SPACE - Pause/Resume");
    write_line("+/- - Adjust speed");
    write_line("D - Debug mode");
    write_line("R - Reset simulation");
    write_line("G - GA mode");
    write_line("Left Click - Spawn Herbivore");
    write_line("Shift+Left Click - Spawn Predator");
    write_line("Ctrl+Left Click - Spawn Omnivore");
    write_line("Right Click - Spawn Food");
    write_line("========================================");
}

// Display final simulation statistics
void display_final_statistics(int final_herbivores, int final_predators, int final_omnivores, int total_born, int total_died, int total_eaten, int frames)
{
    write_line();
    write_line("=======================================");
    write_line("Simulation Ended");
    write_line("Final Herbivores: " + to_string(final_herbivores));
    write_line("Final Predators: " + to_string(final_predators));
    write_line("Final Omnivores: " + to_string(final_omnivores));
    write_line("Total Born: " + to_string(total_born));
    write_line("Total Died: " + to_string(total_died));
    write_line("Total Eaten: " + to_string(total_eaten));
    write_line("Total Frames: " + to_string(frames));
    write_line("========================================");
}

// display evolution analysis
void display_evolution_analysis(const vector<double> &speed_history)
{

    if (speed_history.size() > 1)
    {
        write_line();

        write_line("=== Evolution Analysis ===");
        write_line("Initial Avg Speed: " + to_string(speed_history[0]));
        write_line("Final Avg Speed: " + to_string(speed_history[speed_history.size() - 1]));

        double speed_change = speed_history[speed_history.size() - 1] - speed_history[0];

        double percent_change = (speed_change / speed_history[0]) * 100.0;

        write_line("Speed Evolution: " + to_string(percent_change) + "%");

        if (speed_change > 0.3)
        {
            write_line("Result: POSITIVE EVOLUTION - Population became faster!");
        }
        else if (speed_change < -0.3)
        {
            write_line("Result: NEGATIVE EVOLUTION - Population became slower!");
        }
        else
        {
            write_line("Result: STABLE - No significant evolution detected.");
        }
    }
}