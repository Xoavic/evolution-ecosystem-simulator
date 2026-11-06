# Evolution Ecosystem Simulator

A C++ ecosystem simulation built with SplashKit for FIT1045. Creatures move, find food, hunt, reproduce, die, and pass traits to later generations.

## Features

- Herbivores, predators, and omnivores with different behaviours
- Energy, movement, sensing, eating, hunting, and reproduction systems
- Trait inheritance and mutation across generations
- Optional genetic algorithm mode with tournament selection, crossover, fitness scoring, and elitism
- Real-time visual simulation with pause, speed control, reset, debug mode, and mouse spawning
- Population and evolution statistics

## Project structure

- `Creature.*` — abstract creature interface and shared behaviour
- `Herbivore.*`, `Predator.*`, `Omnivore.*` — creature specialisations
- `SimulationEngine.*` — simulation update and ecosystem rules
- `EvolutionEngineGA.*`, `GeneticAlgorithm.*` — genetic algorithm reproduction
- `InputHandler.*`, `InputHandlerGA.*` — keyboard and mouse controls
- `Food.*` — food entities and nutrition
- `main.cpp` — baseline mutation-based simulation
- `mainWithGA.cpp` — genetic algorithm simulation

## Controls

- `Space` — pause/resume
- `+` / `-` — simulation speed
- `D` — toggle debug mode
- `R` — reset the simulation
- Left click — spawn a herbivore
- Shift + left click — spawn a predator
- Ctrl + left click — spawn an omnivore
- Right click — spawn food
- `G` — toggle genetic algorithm mode in the GA build

## Requirements

- C++
- [SplashKit](https://www.splashkit.io/)

Open the project in a SplashKit-compatible environment and run either `main.cpp` or `mainWithGA.cpp` as the entry point.

## Context

This is a student project for FIT1045. It is shared as a portfolio and learning project.
