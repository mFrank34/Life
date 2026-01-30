#include <catch2/catch_test_macros.hpp>
#include "../source/Unordered.h"

TEST_CASE("Unordered Map constructor registers type and debugger", "[UnorderedTests]")
{
    const Unordered world(1);
    REQUIRE(world.get_type() == "Unordered Map");
}

// TEST_CASE("World neighbour_count counts alive neighbors", "[Unordered]") {
//     // why here cos i need working get cell function
//     Unordered test(1);
//     // Set up a cell at (0,0) and its neighbors.
//     test.get_cell(0, 0).set_type('w');
//     test.get_cell(1, 0).set_type('w');
//     test.get_cell(0, 1).set_type('w');
//     // The cell at (0,0) should have 2 alive neighbors
//     REQUIRE(test.neighbour_count(0, 0) == 2);
//     // The cell at (1,1) should have 3 alive neighbors
//     REQUIRE(test.neighbour_count(1, 1) == 3);
// }

TEST_CASE("Unordered Unload World", "[Unordered]")
{
    Unordered world(1);
    // setting data
    for (int x = 0; x < 10; ++x)
        for (int y = 0; y < 10; ++y)
            world.get_cell(x, y).set_type('w');

    REQUIRE(world.get_world().size() == 100); // 10 * 10

    // Unsetting date
    for (int x = 0; x < 10; ++x)
        for (int y = 0; y < 10; ++y)
            world.get_cell(x, y).set_type('0');

    // unload and see how many elements left
    world.unload();
    REQUIRE(world.get_world().size() == 0);
}

TEST_CASE("Unordered Get Cell", "[Unordered]")
{
    SECTION("get cell from chunk 'w'")
    {
        Unordered world(1);
        world.get_cell(0, 0).set_type('w');
        REQUIRE(world.get_cell(0, 0).get_type() == 'w');
    }

    SECTION("get cell from chunk '0' ")
    {
        Unordered world(1);
        world.get_cell(0, 0).set_type('0');
        REQUIRE(world.get_cell(0, 0).get_type() == '0');
    }
}

TEST_CASE("Unordered Get Chunk from world")
{
    Unordered world(1);
    world.get_cell(0, 0).set_type('w');
    REQUIRE(world.get_chunk(0,0).is_populated());
}

TEST_CASE("World Data Size", "[Unordered]")
{
    Unordered world(1);
    world.get_cell(0, 0).set_type('w');
    world.get_cell(1, 0).set_type('w');
    world.get_cell(0, 1).set_type('w');

    REQUIRE(world.get_world().size() == 3);
}
