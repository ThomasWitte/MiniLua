#include <catch2/catch.hpp>

#include <MiniLua/allocator.hpp>
#include <MiniLua/values.hpp>

namespace minilua {

TEST_CASE("raw MemoryAllocator usage") {
    MemoryAllocator alloc;

    auto* table1 = alloc.allocate_table();
    auto* table2 = alloc.allocate_table();

    REQUIRE(alloc.num_objects() == 2);
    CHECK(table1 != table2);

    // at this point all objects from above should be out of scope and not accessed anymore

    alloc.free_all();
    REQUIRE(alloc.num_objects() == 0);
}

TEST_CASE("using MemoryAllocator with Table") {
    MemoryAllocator alloc;

    const Table table1(&alloc);
    REQUIRE(alloc.num_objects() == 1);
    const Table table2(&alloc);
    REQUIRE(alloc.num_objects() == 2);
    Table table3(table2);
    REQUIRE(alloc.num_objects() == 2);

    table3["key"] = table2;

    CHECK(table1 != table2);
    CHECK(table1 != table3);
    CHECK(table2 == table3);

    // at this point all objects from above should be out of scope and not accessed anymore

    alloc.free_all();
    REQUIRE(alloc.num_objects() == 0);
}

TEST_CASE("using MemoryAllocator with Environment") {
    MemoryAllocator alloc;

    Environment environment(&alloc);

    {
        const Table table1(&alloc);
        const Table table2(&alloc);
        Table table3(table2);
        environment.add("table1", table1);
        environment.add("table2", table2);
        environment.add("table3", table3);

        table3["key"] = table2;

        CHECK(table1 != table2);
        CHECK(table1 != table3);
        CHECK(table2 == table3);
    }

    CHECK(environment.get("table2") == environment.get("table3")["key"]);

    REQUIRE(alloc.num_objects() != 0);

    // at this point all objects from above should be out of scope and not accessed anymore

    alloc.free_all();
    REQUIRE(alloc.num_objects() == 0);
}

} // namespace minilua
