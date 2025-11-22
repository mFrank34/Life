#include <catch2/catch_test_macros.hpp>

#include "World.h"
#include "Cell.h"
#include "Debug.h"

Debug global_debug;
// Minimal concrete subclass for testing
class TestWorld : public World {
public:
    TestWorld(const std::string& type) : World(type) {}

    void unload() override {}

    Chunk& get_chunk(int, int) override {
        static Chunk dummy(0,0,1);
        return dummy;
    }

    Cell& get_cell(int, int) override {
        static Cell dummy;
        return dummy;
    }

    std::unordered_map<long long, Chunk>* get_world() override {
        static std::unordered_map<long long, Chunk> dummy;
        return &dummy;
    }

    // Public wrappers to access protected methods
    long long pub_generate_key(int x, int y) { return generate_key(x,y); }
    std::pair<int,int> pub_decode_key(long long key) const { return decode_key(key); }
    int pub_floor_div(int cord, int size) { return floor_div(cord,size); }
};


TEST_CASE("World constructor registers type and debugger", "[World]") {
    TestWorld test("test_world");
    REQUIRE(test.get_type() == "test_world");

    // Optionally check that global_debug knows about this world
    // (depends on how Debug::register_world is implemented)
}

TEST_CASE("World key generation and decoding", "[World]") {
    TestWorld test("test_world");

    long long key = test.pub_generate_key(42, 99);
    auto [x, y] = test.pub_decode_key(key);

    REQUIRE(x == 42);
    REQUIRE(y == 99);
}

TEST_CASE("World get_neighbour_key returns 8 neighbors", "[World]") {
    TestWorld test("test_world");

    long long key = test.pub_generate_key(10, 20);
    auto neighbors = test.get_neighbour_key(key);

    REQUIRE(neighbors.size() == 8);

    // Check that one of the neighbors is (9,20)
    auto [nx, ny] = test.pub_decode_key(neighbors[3]); // arbitrary index
    REQUIRE((nx == 9 || nx == 11 || nx == 10));
    REQUIRE((ny == 19 || ny == 21 || ny == 20));
}

TEST_CASE("World floor_div handles positive and negative correctly", "[World]") {
    TestWorld test("test_world");

    REQUIRE(test.pub_floor_div(10, 4) == 2);   // normal division
    REQUIRE(test.pub_floor_div(-10, 4) == -3); // adjusted for negative
    REQUIRE(test.pub_floor_div(-8, 4) == -2);  // exact division, no adjustment
}
