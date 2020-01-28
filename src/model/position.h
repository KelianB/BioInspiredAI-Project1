#ifndef POSITION_H
#define POSITION_H

class Position {
	private:
        int x, y; 
    
    public: 
        Position(int x, int y);
        Position() {set(0,0);}

        int getX() {return x;} 
        int getY() {return y;}
        void setX(int x) {this->x = x;} 
        void setY(int y) {this->y = y;}
        void set(int x, int y) {this->setX(x); this->setY(y);}

        float distanceTo(Position l);
};

#endif