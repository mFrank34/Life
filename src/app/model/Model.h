#pragma once
#ifndef MODEL_H
#define MODEL_H

#include "world/storage/Cache.h"
#include "world/storage/Sparse.h"
#include "world/storage/Unordered.h"
#include "world/Manager.h"

enum worldType { CACHE, SPARSE, UNORDERED };

class Model
{
public:
    Model();
    ~Model();

    // Simple getters - that's it
    Cache& getCache();
    Sparse& getSparse();
    Unordered& getUnordered();
    Manager& getManager();

private:
    // --- world types ---
    Cache cache;
    Sparse sparse;
    Unordered unordered;
};

#endif // MODEL_H
