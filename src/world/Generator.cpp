/*
 * File: Generator.cpp
 * Author: Michael Franks
 * Description: Pattern Generator System (integer-based scatter)
 */

#include "Generator.h"
#include <random>
#include <algorithm>
#include <vector>
#include <cmath>

Generator::Generator(Scheduler& scheduler)
    : scheduler(scheduler)
{
}

void Generator::attach_world(World& world)
{
    this->world = &world;
}

// ---------------- Utility Functions ----------------

static uint32_t makeSeed(const std::string& str)
{
    uint32_t hash = 2166136261u;
    for (unsigned char c : str)
    {
        hash ^= c;
        hash *= 16777619u;
    }
    return hash;
}

// ---------------- Generator Implementation ----------------

void Generator::GenerateRequest(GeneratorRequest& request, std::function<void()> on_complete)
{
    if (!world) return;

    const int chunkSize = 32;

    long long radius = request.radius > 0 ? request.radius : 8;
    long long radiusInCells = radius * chunkSize;

    int gridSpacing = std::max(8, (int)(radiusInCells / 512));

    struct ColorJob
    {
        std::string color;
        CellType type;
    };

    std::vector<ColorJob> jobs;
    if (request.use_white) jobs.push_back({"white", CellType::White});
    if (request.use_red) jobs.push_back({"red", CellType::Red});
    if (request.use_green) jobs.push_back({"green", CellType::Green});
    if (request.use_blue) jobs.push_back({"blue", CellType::Blue});

    if (jobs.empty())
    {
        if (on_complete) on_complete();
        return;
    }

    auto world_ptr = world;
    auto seed = request.seed;
    auto remaining = std::make_shared<std::atomic<int>>((int)jobs.size());

    for (auto& job : jobs)
    {
        scheduler.enqueue_grouped([=]()
        {
            std::mt19937_64 rng(makeSeed(seed + "_" + job.color));
            std::uniform_int_distribution<int> jitter(-gridSpacing / 2, gridSpacing / 2);
            std::uniform_int_distribution<int> clusterDist(-2, 2);

            for (long long gx = -radiusInCells; gx <= radiusInCells; gx += gridSpacing)
            {
                for (long long gy = -radiusInCells; gy <= radiusInCells; gy += gridSpacing)
                {
                    if (gx * gx + gy * gy > radiusInCells * radiusInCells) continue;

                    long long cx = gx + jitter(rng);
                    long long cy = gy + jitter(rng);

                    for (int i = 0; i < 5; i++)
                    {
                        long long x = cx + clusterDist(rng);
                        long long y = cy + clusterDist(rng);
                        world_ptr->get_cell((int)x, (int)y).set_type(job.type);
                    }
                }
            }

            // Last job to finish calls the callback
            if (remaining->fetch_sub(1) == 1)
                if (on_complete) on_complete();
        });
    }
}
