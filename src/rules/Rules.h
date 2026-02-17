/*
 * File: Rules.h
 * Author: Michael Franks
 * Created: 2026-02-17
 * Last Edited: 2026-02-10
 * Description:
 */

#ifndef LIFE_H
#define LIFE_H

#include "vector"

class Rules
{
public:
    Rules();

    // rules
    bool underpopulation(int c) const;
    bool survival(int c) const;
    bool overpopulation(int c) const;
    bool reproduction(int c) const;

    ~Rules();

private:
    // limits 
    int limit = 3;
    int min = 2;
};

#endif