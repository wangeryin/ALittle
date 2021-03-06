
#include "timerlib.h"
#include "timer.h"

#include "lua/src/lua.h"
#include "lua/src/lauxlib.h"

#define TIMER_MODNAME "timer"

static int timerlib_destroy(lua_State* L)
{
    timer* ts = (timer*)lua_touserdata(L, 1);
    if (ts) timer_destroy(ts);
    return 0;
}

static int timerlib_create(lua_State* L)
{
    timer* ts = (timer*)lua_newuserdata(L, sizeof(timer));
    lua_newtable(L);
    lua_pushcfunction(L, timerlib_destroy);
    lua_setfield(L, -2, "__gc");
    lua_setmetatable(L, -2);
    return 1;
}

static int timerlib_add(lua_State* L)
{
    timer* ts = (timer*)lua_touserdata(L, 1);
    int delay_ms = lua_tonumber(L, 2);
    int loop = lua_tonumber(L, 3);
    int interval_ms = lua_tonumber(L, 4);
    int id = timer_add(ts, delay_ms, loop, interval_ms);
    lua_pushnumber(L, id);
    return 1;
}

static int timerlib_remove(lua_State* L)
{
    timer* ts = (timer*)lua_touserdata(L, 1);
    int id = lua_tonumber(L, 2);
    int result = timer_remove(ts, id);
    lua_pushboolean(L, result);
    return 1;
}

static int timerlib_update(lua_State* L)
{
    timer* ts = (timer*)lua_touserdata(L, 1);
    int frame_ms = lua_tonumber(L, 2);
    timer_update(ts, frame_ms);
    return 0;
}

static int timerlib_poll(lua_State* L)
{
    timer* ts = (timer*)lua_touserdata(L, 1);
    int id = timer_poll(ts);
    lua_pushnumber(L, id);
    return 1;
}

/*
** Assumes the table is on top of the stack.
*/
static void set_info(lua_State* L) {
    lua_pushliteral(L, "_COPYRIGHT");
    lua_pushliteral(L, "Copyright (C) 2003-2019 PUC-Rio");
    lua_settable(L, -3);
    lua_pushliteral(L, "_DESCRIPTION");
    lua_pushliteral(L, "mini heap timer for Lua");
    lua_settable(L, -3);
    lua_pushliteral(L, "_VERSION");
    lua_pushliteral(L, "timer 1.0");
    lua_settable(L, -3);
}

static struct luaL_Reg timerlib[] = {
  {"create", timerlib_create},
  {"add", timerlib_add},
  {"remove", timerlib_remove},
  {"update", timerlib_update},
  {"poll", timerlib_poll},
  {NULL, NULL}
};

int luaopen_timer(lua_State* L) {
    lua_newtable(L);
    luaL_setfuncs(L, timerlib, 0);
    set_info(L);
    lua_pushvalue(L, -1);
    lua_setglobal(L, TIMER_MODNAME);
    return 1;
}
