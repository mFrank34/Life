#include <catch2/catch_test_macros.hpp>
#include "Cell.h"

TEST_CASE("Default constructor initializes correctly", "[cell]") {
    Cell c;
    REQUIRE(c.get_type() == 0);       // type defaults to 0
    REQUIRE_FALSE(c.is_alive());      // alive defaults to false
}

TEST_CASE("Constructor with type initializes correctly", "[cell]") {
    SECTION("Empty cell type '0'") {
        Cell c('0');
        REQUIRE(c.get_type() == '0');
        REQUIRE_FALSE(c.is_alive());
    }

    SECTION("White cell type 'w'") {
        Cell c('w');
        REQUIRE(c.get_type() == 'w');
        REQUIRE(c.is_alive());
    }

    SECTION("Unknown type sets alive=false") {
        Cell c('x');
        REQUIRE(c.get_type() == 'x');
        REQUIRE_FALSE(c.is_alive());
    }
}

TEST_CASE("set_type updates alive state", "[cell]") {
    Cell c;
    c.set_type('w');
    REQUIRE(c.get_type() == 'w');
    REQUIRE(c.is_alive());

    c.set_type('0');
    REQUIRE(c.get_type() == '0');
    REQUIRE_FALSE(c.is_alive());

    c.set_type('x');
    REQUIRE(c.get_type() == 'x');
    REQUIRE_FALSE(c.is_alive());
}

TEST_CASE("set_alive overrides alive state", "[cell]") {
    Cell c('w');
    REQUIRE(c.is_alive());

    c.set_alive(false);
    REQUIRE_FALSE(c.is_alive());

    c.set_alive(true);
    REQUIRE(c.is_alive());
}
