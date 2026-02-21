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

    bool survives(int neighbours) const
    {
        return neighbours >= survival_min && neighbours <= survival_max;
    }

    bool births(int neighbours) const
    {
        return neighbours >= birth_min && neighbours <= birth_max;
    }
};

class Rules
{
public:
    Rule white = {2, 3, 3, 3};
    Rule blue = {3, 4, 5, 5};
    Rule green = {5, 6, 7, 7};
    Rule red = {6, 7, 8, 8};
};

#endif // RULES_H