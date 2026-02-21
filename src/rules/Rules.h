/*
 * File: Rules.h
 * Author: Michael Franks
 * Description: Conways rules set with some extra systems
 */

#ifndef RULES_H
#define RULES_H

struct Rule
{
    int survival_min;
    int survival_max;
    int birth_min;
    int birth_max;
};

class Rules
{
public:
    Rule blue = {4, 4, 5, 5};
    Rule green = {6, 6, 7, 7};
    Rule red = {8, 8, 9, 9};
    Rule white = {2, 3, 3, 3};
};

#endif // RULES_H
