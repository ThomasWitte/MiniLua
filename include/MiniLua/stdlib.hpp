#ifndef MINILUA_STDLIB_HPP
#define MINILUA_STDLIB_HPP

#include <string>
#include <utility>

#include "MiniLua/values.hpp"

namespace minilua {
auto tostring(const CallContext& ctx) -> Value;

auto to_number(const CallContext& ctx) -> Value;

auto type(const CallContext& ctx) -> Value;

auto assert_lua(const CallContext& ctx) -> Vallist;

auto next(const CallContext& ctx) -> Vallist;

/*
If index is a number, returns all arguments after the index-th argument (including index-th
argument). If index is a negative number, the index counting starts at the end (so -1 is the index
of the last argument) If index is the string "#", select returns the total amount of extra arguments
it received. If index is positive and bigger than the size select just returns an empty Vallist. If
index in an invalid index (0 or negative bigger than size) an index out of range error is thrown If
index is an invalid value, an error is thrown that an invalid index was entered
*/
auto select(const CallContext& ctx) -> Vallist;

void print(const CallContext& ctx);
} // namespace minilua

#endif