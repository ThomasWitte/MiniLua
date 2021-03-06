#ifndef MINILUA_MATH_HPP
#define MINILUA_MATH_HPP

#include <climits>
#include <cmath>
#include <cstddef>
#include <functional>
#include <random>
#include <string>

#include "MiniLua/values.hpp"

/**
 * @brief Math functions in the lua standard library.
 */
namespace minilua::math {

const Number::Int MAXINTEGER = LONG_MAX;
const Number::Int MININTEGER = LONG_MIN;
const Number::Float PI = M_PI;
const Number::Float HUGE = HUGE_VAL;

auto abs(const CallContext& ctx) -> Value;

auto acos(const CallContext& ctx) -> Value;

auto asin(const CallContext& ctx) -> Value;

auto atan(const CallContext& ctx) -> Value;

auto ceil(const CallContext& ctx) -> Value;

auto cos(const CallContext& ctx) -> Value;

auto deg(const CallContext& ctx) -> Value;

auto exp(const CallContext& ctx) -> Value;

auto floor(const CallContext& ctx) -> Value;

auto fmod(const CallContext& ctx) -> Value;

auto log(const CallContext& ctx) -> Value;

auto max(const CallContext& ctx) -> Value;

auto min(const CallContext& ctx) -> Value;

auto modf(const CallContext& ctx) -> Vallist;

auto rad(const CallContext& ctx) -> Value;

auto random(const CallContext& ctx) -> Value;

void randomseed(const CallContext& ctx);

auto sin(const CallContext& ctx) -> Value;

auto sqrt(const CallContext& ctx) -> Value;

auto tan(const CallContext& ctx) -> Value;

auto to_integer(const CallContext& ctx) -> Value;

auto type(const CallContext& ctx) -> Value;

auto ult(const CallContext& ctx) -> Value;

auto get_random_seed() -> std::default_random_engine;
} // namespace minilua::math

#endif
