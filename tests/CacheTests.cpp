#include <catch2/catch_test_macros.hpp>
#include "Cache.h"

TEST_CASE("Cache Map constructor registers type and debugger", "[Cache]")
{
    const Cache world(8, 8);
    REQUIRE(world.get_type() == "Cache Map");
}

TEST_CASE("Cache: World neighbour_count counts alive neighbors", "[Cache]") {
    // why here cos i need working get cell function
    Cache test(4, 8);
    // Set up a cell at (0,0) and its neighbors.
    test.get_cell(0, 0).set_type('w');
    test.get_cell(1, 0).set_type('w');
    test.get_cell(0, 1).set_type('w');
    // The cell at (0,0) should have 2 alive neighbors
    REQUIRE(test.neighbour_count(0, 0) == 2);
    // The cell at (1,1) should have 3 alive neighbors
    REQUIRE(test.neighbour_count(1, 1) == 3);
}

TEST_CASE("Cache: Unload World", "[Cache]")
{
    Cache world(4, 8);
    // setting data
    for (int x = 0; x < 16; ++x)
        for (int y = 0; y < 16; ++y)
            world.get_cell(x, y).set_type('w');

    REQUIRE(world.get_world().size() == 16); // 10 * 10

    // Unsetting date
    for (int x = 0; x < 16; ++x)
        for (int y = 0; y < 16; ++y)
            world.get_cell(x, y).set_type('0');

    // unload and see how many elements left
    world.unload();
    REQUIRE(world.get_world().size() == 0);
}

TEST_CASE("Cache: Get Cell", "[Cache]")
{
    SECTION("get cell from chunk 'w'")
    {
        Cache world(8, 8);
        world.get_cell(0, 0).set_type('w');
        REQUIRE(world.get_cell(0, 0).get_type() == 'w');
    }

    SECTION("get cell from chunk '0' ")
    {
        Cache world(8, 8);
        world.get_cell(0, 0).set_type('0');
        REQUIRE(world.get_cell(0, 0).get_type() == '0');
    }
}

TEST_CASE("Cache: Get Chunk from world", "[Cache]")
{
    Cache world(8, 8);
    world.get_cell(0, 0).set_type('w');
    REQUIRE(world.get_chunk(0,0).is_populated());
}

TEST_CASE("Cache: World Data Size", "[Cache]")
{
    Cache world(1, 8);
    world.get_cell(0, 0).set_type('w');
    world.get_cell(1, 0).set_type('w');
    world.get_cell(0, 1).set_type('w');

    REQUIRE(world.get_world().size() == 3);
}