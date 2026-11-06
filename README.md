# Evolution Ecosystem Simulator

A C++ ecosystem simulation built with SplashKit for FIT1045. The project started as a model of creatures competing for food and was later extended into an experiment in evolutionary computation.

## The idea: cellular-automata-inspired emergence

The project is inspired by the way cellular automata produce complex global patterns from simple local rules. There is no single function that directly programs “ecosystem balance”, predator-prey cycles, or population booms. Instead, every creature follows local rules based on what it can sense and its current energy:

- Herbivores move towards nearby food and wander when no food is visible.
- Predators search for herbivores, chase them, and gain part of their energy after a successful hunt.
- Omnivores can both consume food and hunt herbivores.
- Movement consumes energy; low-energy creatures die; sufficiently energetic creatures reproduce.
- Offspring inherit traits, with bounded random mutation in speed, size, and sense radius.
- Food is consumed and respawns gradually, creating a changing resource environment.

The interesting behaviour is emergent: local movement, sensing, energy and reproduction rules create population cycles and species dominance without explicitly scripting those outcomes. The visual simulation makes those effects observable rather than reducing them to a printed table.

## Baseline simulation

`main.cpp` runs the mutation-only version. Each creature is stored polymorphically through `vector<Creature*>`, while the simulation engine coordinates the lifecycle of the population:

1. update movement and sensing;
2. resolve predator hunting and food collisions;
3. remove dead creatures;
4. create offspring;
5. respawn food;
6. render the world and statistics.

The class structure separates shared creature state from species-specific behaviour. `Creature` is an abstract base class, while `Herbivore`, `Predator`, and `Omnivore` implement their own movement, eating, hunting, drawing, reproduction, and energy costs. `SimulationEngine` coordinates the systems without needing a separate update loop for every species.

## H1 extension: what the genetic algorithm actually does

`mainWithGA.cpp` adds a genetic-algorithm reproduction mode on top of the original ecosystem. The GA does not directly control movement or guarantee a balanced ecosystem. It changes **which creatures reproduce and which traits are passed to the next generation**.

### 1. Fitness function

Each living creature receives a fitness score from three measurable traits:

- speed: 40%;
- size: 30%;
- current energy: 30%.

A small generation bonus rewards survival into later generations. The weights are a design choice: they define what “successful” means to the algorithm. In this project, speed is deliberately the strongest component, so the GA tends to favour creatures that move quickly and still maintain enough energy to reproduce.

### 2. Tournament selection

Instead of comparing every creature globally, the algorithm samples a tournament of five creatures and selects the fittest one as a parent. It repeats this for a second parent. Tournament selection applies pressure towards higher fitness while still allowing less dominant individuals to reproduce occasionally, which helps retain variation.

Parents must be alive, have enough energy, and belong to the same creature type. The parents each pay an energy cost when an offspring is created.

### 3. Biased uniform crossover

The offspring does not simply receive the average of its parents. For each trait independently—speed, size, and sense radius—the algorithm chooses one parent using a probability weighted by the parents’ relative fitness. This can combine traits that were separated across the parents, such as one parent’s speed and another parent’s larger sensing range.

The offspring type is inherited from the fitter parent, and the existing constructor mutation step can still make a small bounded change to each inherited trait.

### 4. Elitism

The top 10% of the current population are treated as elites. They receive an energy boost so that strong individuals are less likely to disappear before reproducing. This preserves progress between generations, but it also has a trade-off: strong selection can reduce diversity and can make one species dominate.

### 5. Population controls and comparison mode

The GA engine targets a population of 40, caps the number of offspring created in one reproduction pass, rejects invalid parent pairs, and stops after repeated failed selection attempts. Press `G` in the GA build to switch between GA reproduction and the original mutation-only reproduction, making the two approaches directly comparable.

The important result is that a higher fitness score does not automatically mean a healthier ecosystem. In testing, the GA could accelerate convergence towards dominant traits, increase predator dominance, and reduce diversity through elitism. The algorithm optimises the fitness definition we supplied; it does not understand ecosystem balance unless balance is included in that definition.

## Controls

- `Space` — pause/resume
- `+` / `-` — simulation speed
- `D` — toggle debug mode and show sensing radii
- `R` — reset the simulation
- Left click — spawn a herbivore
- Shift + left click — spawn a predator
- Ctrl + left click — spawn an omnivore
- Right click — spawn food
- `G` — toggle GA reproduction in the GA build

## Project structure

- `Creature.*` — abstract creature interface, shared movement, energy, sensing, and trait state
- `Herbivore.*`, `Predator.*`, `Omnivore.*` — species-specific polymorphic behaviour
- `SimulationEngine.*` — update order, collisions, hunting, deaths, births, food management, and statistics
- `EvolutionEngineGA.*` — coordinates GA reproduction and tracks generations and fitness history
- `GeneticAlgorithm.*` — fitness, tournament selection, crossover, elitism, and aggregate fitness queries
- `InputHandler.*`, `InputHandlerGA.*` — keyboard, mouse, reset, and GA-mode controls
- `Food.*` — food state, nutrition, collision size, and rendering
- `Types.h` — positions, vector operations, creature types, and statistics
- `main.cpp` — baseline mutation-only simulation
- `mainWithGA.cpp` — GA extension

## Requirements and build

- C++
- [SplashKit](https://www.splashkit.io/)

A SplashKit-compatible build can use either entry point. For the GA version, compile the source files with `mainWithGA.cpp` as the entry point and link SplashKit, then run the resulting executable.

## Context

This is a student project for FIT1045. It is shared as a portfolio and learning project. The H1/H3 extension was designed to explore how evolutionary operators change the behaviour of an already-running ecosystem rather than treating a genetic algorithm as an isolated toy example.
