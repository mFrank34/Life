#include <catch2/catch_test_macros.hpp>
#include "Chunk.h"
#include "Cell.h"

TEST_CASE("Constructor sets chunk size correctly", "[Chunk]") {
    SECTION("Chunk Size is 16") {
        Chunk chunk(1, 1, 16);
        REQUIRE(chunk.get_size() == 16);
    }

    SECTION("Chunk Size is 32") {
        const Chunk chunk(1, 1, 32);
        REQUIRE(chunk.get_size() == 32);
    }

    SECTION("Chunk Size is 48") {
        const Chunk chunk(1, 1, 48);
        REQUIRE(chunk.get_size() == 48);
    }
}

TEST_CASE("Local Coordinates for Chunks", "[Chunk]") {
    Chunk chunk(1, 1, 16);
    REQUIRE(chunk.get_CX() == 1);
    REQUIRE(chunk.get_CY() == 1);
}

TEST_CASE("X - Getting Cords from Chunks", "[Chunk]") {
    Chunk chunk(0, 0, 16);

    SECTION("X - Positive values within range") {
        REQUIRE(chunk.get_LX(0) == 0);
        REQUIRE(chunk.get_LX(5) == 5);
        REQUIRE(chunk.get_LX(15) == 15);
    }

    SECTION("X - Positive values beyond range") {
        REQUIRE(chunk.get_LX(16) == 0);
        REQUIRE(chunk.get_LX(17) == 1);
        REQUIRE(chunk.get_LX(32) == 0);
    }

    SECTION("X - Negative values") {
        REQUIRE(chunk.get_LX(-1) == 15);
        REQUIRE(chunk.get_LX(-16) == 0);
        REQUIRE(chunk.get_LX(-17) == 15);
    }
}

TEST_CASE("Y - Getting Coors from Chunks", "[Chunk]") {
    Chunk chunk(0, 0, 16);  // size = 16

    SECTION("Y - Positive values within range") {
        REQUIRE(chunk.get_LY(0) == 0);
        REQUIRE(chunk.get_LY(5) == 5);
        REQUIRE(chunk.get_LY(15) == 15);
    }

    SECTION("Y - Positive values beyond range") {
        REQUIRE(chunk.get_LY(16) == 0);
        REQUIRE(chunk.get_LY(17) == 1);
        REQUIRE(chunk.get_LY(32) == 0);
    }

    SECTION("Y - Negative values") {
        REQUIRE(chunk.get_LY(-1) == 15);
        REQUIRE(chunk.get_LY(-16) == 0);
        REQUIRE(chunk.get_LY(-17) == 15);
    }
}

TEST_CASE("Chunk population checks", "[Chunk]") {
    constexpr int size = 4; // small size for easy testing

    SECTION("Empty chunk has no live cells") {
        Chunk chunk(0, 0, size);
        // By default, all cells should be dead
        REQUIRE_FALSE(chunk.is_populated());
        REQUIRE(chunk.populated_amt() == 0);
    }

    SECTION("Chunk with one live cell") {
        Chunk chunk(0, 0, size);
        chunk.get_cell(0, 0).set_alive(true);
        REQUIRE(chunk.is_populated());
        REQUIRE(chunk.populated_amt() == 1);
    }

    SECTION("Chunk with multiple live cells") {
        Chunk chunk(0, 0, size);
        // Every other cells
        for (int x = 0; x < size; ++x) {
            for (int y = 0; y < size; ++y) {
                int index = y * size + x;
                if (index % 2 == 0) {   // every other cell alive
                    chunk.get_cell(x, y).set_alive(true);
                }
            }
        }
        // checking if chunks
        REQUIRE(chunk.is_populated());
        REQUIRE(chunk.populated_amt() == 8);
    }

    SECTION("Fully populated chunk") {
        Chunk chunk(0, 0, size);
        // activate all cells in
        for (int x = 0; x < size; ++x) {
            for (int y = 0; y < size; ++y) {
                chunk.get_cell(x,y).set_alive(true);
            }
        }
        REQUIRE(chunk.is_populated());
        REQUIRE(chunk.populated_amt() == size * size);
    }
}

// Helper: create a chunk with one alive cell
Chunk makeChunkWithAliveCell() {
    Chunk c(0, 0, 4);
    c.get_cell(1, 1).set_type('w');
    return c;
}

TEST_CASE("Chunk copy constructor copies cells", "[chunk]") {
    Chunk original = makeChunkWithAliveCell();
    Chunk copy(original);

    REQUIRE(copy.get_cell(1, 1).is_alive());
    REQUIRE(original.get_cell(1, 1).is_alive());
}

TEST_CASE("Chunk copy assignment copies cells", "[chunk]") {
    Chunk original = makeChunkWithAliveCell();
    Chunk copyAssign(0, 0, 4);

    copyAssign = original;

    REQUIRE(copyAssign.get_cell(1, 1).is_alive());
    REQUIRE(original.get_cell(1, 1).is_alive());
}

TEST_CASE("Chunk move constructor transfers cells", "[chunk]") {
    Chunk original = makeChunkWithAliveCell();
    Chunk moved(std::move(original));

    REQUIRE(moved.get_cell(1, 1).is_alive());
    // original cells may be empty after move, but moved must have them
}

TEST_CASE("Chunk move assignment transfers cells", "[chunk]") {
    Chunk original = makeChunkWithAliveCell();
    Chunk moveAssign(0, 0, 4);

    moveAssign = std::move(original);

    REQUIRE(moveAssign.get_cell(1, 1).is_alive());
}



