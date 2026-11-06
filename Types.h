#ifndef TYPES_H
#define TYPES_H

#include <cmath>

// enum for two creatures
enum class CreatureType
{
    HERBIVORE,
    PREDATOR,
    OMNIVORE
};

// struct 1 : Position ( 2D coordinates)
struct Position
{
    double x;
    double y;

    // Default constructor
    Position()
    {
        x = 0;
        y = 0;
    }

    // Parameterized constructor
    Position(double x_val, double y_val)
    {
        x = x_val;
        y = y_val;
    }

    // vector addition
    Position operator+(const Position &other) const
    {
        return Position(x + other.x, y + other.y);
    }

    // vector subtraction
    Position operator-(const Position &other) const
    {
        return Position(x - other.x, y - other.y);
    }

    //  multiplied by scalar
    Position operator*(double scalar) const
    {
        return Position(x * scalar, y * scalar);
    }

    // Calculate distance between two position
    double distance(const Position &other) const
    {
        double dx = x - other.x;
        double dy = y - other.y;
        return sqrt(dx * dx + dy * dy);
    }

    // Calculate magnitude of the vector
    double length() const
    {
        return sqrt(x * x + y * y);
    }

    // Calculate the unit vector
    Position normalized() const
    {
        double len = length();
        if (len > 0.001)
        {
            return Position(x / len, y / len);
        }
        return Position(0, 0);
    }
};

// Struct 2 : GeneticTraits ( characteristic of gene )
struct GeneticTraits
{
    double speed;
    double size;
    double sense_radius;
    int generation;

    // Default constructor
    GeneticTraits()
    {
        speed = 2.0;
        size = 10.0;
        sense_radius = 50.0;
        generation = 1;
    }

    // Parameterized constructor
    GeneticTraits(double s, double sz, double s_r, int gen)
    {
        speed = s;
        size = sz;
        sense_radius = s_r;
        generation = gen;
    }
};

// struct 3 SimulationStats( do the statistics)
struct SimulationStats
{
    int population_count;
    int herbivore_count;
    int predator_count;
    int omnivore_count;
    int food_count;
    int total_born;
    int total_died;
    int total_eaten;

    double average_speed;
    double average_energy;
    int max_generation;

    // Evolution statistics
    double average_size;
    double min_speed;
    double max_speed;
    double average_sense_radius;
    double average_generation;

    double average_predator_speed;
    double average_herbivore_speed;
    double average_omnivore_speed;

    // Default constructor
    SimulationStats()
    {
        population_count = 0;
        herbivore_count = 0;
        predator_count = 0;
        omnivore_count = 0;
        food_count = 0;
        total_born = 0;
        total_died = 0;
        total_eaten = 0;
        average_speed = 0.0;
        average_energy = 0.0;
        max_generation = 1;
        
        average_size = 0.0;
        min_speed = 0.0;
        max_speed = 0.0;
        average_sense_radius = 0.0;
        average_generation = 1.0;

        average_predator_speed = 0.0;
        average_herbivore_speed = 0.0;
        average_omnivore_speed = 0.0;
    }
};

#endif
