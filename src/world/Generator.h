/*
 * File: Generator
 * Author: Michael Franks 
 * Description: Pattern Generator System
 */

#ifndef GENERATOR_H
#define GENERATOR_H
#include <string>


/**
 * Request for generator
 */
struct GeneratorRequest
{
    std::string seed;
    int radius = 0;

    bool use_white = false;
    bool use_red = false;
    bool use_green = false;
    bool use_blue = false;
};


class Generator
{
public:

private:
};


#endif // GENERATOR_H
