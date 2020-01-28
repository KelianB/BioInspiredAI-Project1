#ifndef LOCATABLE_H
#define LOCATABLE_H

class Locatable { 
    private:
        int x, y; 
    
    public: 
        Locatable(int x, int y);

        int getX() {return x;} 
        int getY() {return y;}
        void setX(int x) {this->x = x;} 
        void setY(int y) {this->y = y;}
        void setPosition(int x, int y) {this->setX(x); this->setY(y);}

        float distanceTo(Locatable l);
}; 

#endif