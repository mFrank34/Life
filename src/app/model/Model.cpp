#include "Model.h"

Model::Model()
    : cache(32, 16),
      sparse(32),
      unordered(1),
      manager()
{
}

Model::~Model()
{
}

Cache& Model::getCache()
{
    return cache;
}

Sparse& Model::getSparse()
{
    return sparse;
}

Unordered& Model::getUnordered()
{
    return unordered;
}

Manager& Model::getManager()
{
    return manager;
}
