#ifndef FOOD_H
#define FOOD_H

#include "splashkit.h"
#include"Types.h"

class Food
{
private:
    // position of the food
   Position position;

    // food properties ( energy value of the food and is the food available to be eaten)
    double nutrition;  
    bool active;       
    
    // visual properties
    double size;
    color food_color;
    
public:
    // constructor 
    Food(double pos_x, double pos_y);
    
    // core methods
    void draw();
    
    // state management
    bool is_active() const;
    void set_active(bool state);
    
     // getters for collision detection in the future
    double get_x() const;
    double get_y() const;
    double get_size() const;
    double get_nutrition() const;
    Position get_position() const;
};

#endif