#ifndef LIFE_H
#define LIFE_H

class Life
{
public:
    Life();

    bool underpopulation(int c);

    bool survival(int c);

    bool overpopulation(int c);

    bool reproduction(int c);

    ~Life();

private:
    // limits 
    int limit = 3;
    int min = 2;
    
};

#endif