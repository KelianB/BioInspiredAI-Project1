#ifndef LOCATABLE_H
#define LOCATABLE_H

class Locatable { 
    private:
        float x, y; 
    
    public: 
        Locatable(float x, float y);

        float getX() {return x;} 
        float getY() {return y;}
        float distanceTo(Locatable* l);
}; 

#endif