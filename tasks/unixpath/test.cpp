#include <catch.hpp>

#include "unixpath.h"

TEST_CASE("NormalizePath") {
    REQUIRE("/" == NormalizePath("/", "."));
    REQUIRE("/home/user2" == NormalizePath("/home/user1", "../user2"));

    REQUIRE("/" == NormalizePath("/", ".."));
    REQUIRE("/tmp" == NormalizePath("/home", "../../tmp"));

    REQUIRE("/a" == NormalizePath("/", "../../a/"));
    REQUIRE("/b" == NormalizePath("/", ".././/././/./../b/././././././"));
    REQUIRE("/a/b/c" == NormalizePath("/c/d/e/f", "/a/b/c"));
    REQUIRE("/c/d/e/f/b/c" == NormalizePath("/c/d/e/f", "./a/..////b/c"));
    REQUIRE("/c/d/e/f/b/c" == NormalizePath("//c/d/e/f", "./a/..////b/c"));
    REQUIRE("/a" == NormalizePath("////a//", "."));
}
