#include "MiniLua/luaast.hpp"
#include "MiniLua/luainterpreter.hpp"

VISITABLE_IMPL(_LuaName)
VISITABLE_IMPL(_LuaExplist)
VISITABLE_IMPL(_LuaValue)
VISITABLE_IMPL(_LuaOp)
VISITABLE_IMPL(_LuaUnop)
VISITABLE_IMPL(_LuaNameVar)
VISITABLE_IMPL(_LuaIndexVar)
VISITABLE_IMPL(_LuaMemberVar)
VISITABLE_IMPL(_LuaFunctioncall)
VISITABLE_IMPL(_LuaReturnStmt)
VISITABLE_IMPL(_LuaBreakStmt)
VISITABLE_IMPL(_LuaForStmt)
VISITABLE_IMPL(_LuaLoopStmt)
VISITABLE_IMPL(_LuaIfStmt)
VISITABLE_IMPL(_LuaChunk)
VISITABLE_IMPL(_LuaTableconstructor)
VISITABLE_IMPL(_LuaAssignment)
VISITABLE_IMPL(_LuaFunction)
VISITABLE_IMPL(_LuaField)
