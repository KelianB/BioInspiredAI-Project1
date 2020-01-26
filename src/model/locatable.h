#ifndef LOCATABLE_H
#define LOCATABLE_H

class Locatable { 
    private:
        float x, y; 
    
    public: 
        Locatable(float x, float y);

        float getX() {return x;} 
        float getY() {return y;}
        void setX(float x) {this->x = x;} 
        void setY(float y) {this->y = y;}
        void setPosition(float x, float y) {this->setX(x); this->setY(y);}

        float distanceTo(Locatable l);
}; 

#endif