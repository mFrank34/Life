#include <catch2/catch_test_macros.hpp>
#include "Unordered.h"

TEST_CASE("Unordered Map constructor registers type and debugger", "[UnorderedTests]")
{
    const Unordered world(1);
    REQUIRE(world.get_type() == "Unordered Map");
}

TEST_CASE("World neighbour_count counts alive neighbors", "[World]") {
    // why here cos i need working get cell function
    Unordered test(1);
    // Set up a cell at (0,0) and its neighbors.
    test.get_cell(0, 0).set_type('w');
    test.get_cell(1, 0).set_type('w');
    test.get_cell(0, 1).set_type('w');
    // The cell at (0,0) should have 2 alive neighbors
    REQUIRE(test.neighbour_count(0, 0) == 2);
    // The cell at (1,1) should have 3 alive neighbors
    REQUIRE(test.neighbour_count(1, 1) == 3);
}

// TEST_CASE("s")