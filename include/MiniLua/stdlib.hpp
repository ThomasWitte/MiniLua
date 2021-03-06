#ifndef MINILUA_STDLIB_HPP
#define MINILUA_STDLIB_HPP

#include <string>
#include <utility>

#include "MiniLua/values.hpp"

namespace minilua {

/**
 * Returns a table with all the math functions.
 */
auto create_math_table(MemoryAllocator* allocator) -> Table;

/**
 * @brief Tries to force the first argument to take on the value of the second
 * argument.
 *
 * The same as calling `arg1.force(arg2)` on the [Values](@ref Value) in C++
 * where `arg1` and `arg2` are the first and second arguments.
 *
 * \note This is not part of the official Lua stdlib.
 */
auto force(const CallContext& ctx) -> CallResult;

void error(const CallContext& ctx);

auto to_string(const CallContext& ctx) -> Value;

auto to_number(const CallContext& ctx) -> Value;

auto type(const CallContext& ctx) -> Value;

auto next(const CallContext& ctx) -> Vallist;

/**
 * If `index` is a number, returns all arguments after the `index`-th argument
 * (including `index`-th argument).
 *
 * If index is a negative number, the index counting starts at the end (so `-1`
 * is the index of the last argument). If index is the string `"#"`, select
 * returns the total amount of extra arguments it received. If index is
 * positive and bigger than the size select just returns an empty Vallist. If
 * index in an invalid index (0 or negative bigger than size) an index out of
 * range error is thrown if index is an invalid value, an error is thrown that
 * an invalid index was entered.
 */
auto select(const CallContext& ctx) -> Vallist;

void print(const CallContext& ctx);

/**
 * Remove the origin of all passed in values and return them.
 *
 * \note This is not part of the official lua stdlib.
 */
auto discard_origin(const CallContext& ctx) -> Vallist;

/**
 * Returns the metatable of the first parameter or nil if there is none.
 */
auto get_metatable(const CallContext& ctx) -> Value;

/**
 * Sets the metatable (second parameter) for the table (first parameter) and
 * return the table.
 *
 * Will error if the first value is not a table or second value is not a table
 * or nil.
 */
auto set_metatable(const CallContext& ctx) -> Value;

/**
 * Gets the real value of table[index], without invoking any metamethod.
 *
 * First param is the table, second param is the index.
 *
 * Will error if the table is not a Table.
 */
auto rawget(const CallContext& ctx) -> Value;

/**
 * Sets the real value of table[index] to value, without invoking any metamethod.
 *
 * First param is the table, second param is the index and third param is the value.
 *
 * Will error if the table is not a Table or if the index is invalid.
 *
 * This function returns table.
 */
auto rawset(const CallContext& ctx) -> Value;

} // namespace minilua

#endif
