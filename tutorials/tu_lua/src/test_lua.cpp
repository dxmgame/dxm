// test_protobuf
#include "precompiled.h"

#include "stdio.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C"{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}


void test_lua(){

	int status;
	lua_State *lua_state = lua_open();  /* create state */
	if (lua_state == NULL) {
		return;
	}
	luaL_openlibs(lua_state);
	status = luaL_dofile(lua_state, "lua/main.lua");
	printf("error: %d", status);
	lua_close(lua_state);
}
