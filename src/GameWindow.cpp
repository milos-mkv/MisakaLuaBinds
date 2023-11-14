#include <GameWindow.hpp>

#include <lua.hpp>

void GameWindow::CreateGameWindow(int width, int height, const char* title)
{
    if (s_instance == nullptr)
        s_instance = std::make_shared<GameWindow>(width, height, title);
}

GameWindow::GameWindow(int width, int height, const char* title)
    : m_width(width), m_height(height), m_title(title)
{
    Log("Game Window constructor: width=", width, "height=", height, "title=", title); 
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);
}

void GameWindow::Run()
{
    Log("Game Window run.");

    while(!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
        glfwSwapBuffers(m_window);
    }
}

static int lua_CreateGameWindow(lua_State* state)
{
    int paramCount = lua_gettop(state);

    switch(paramCount)
    {
        case 3:
        {
            int width = (int) luaL_checkint(state, 1);
            int height = (int) luaL_checkint(state, 2);
            const char* title = (const char*) luaL_checkstring(state, 3);
            GameWindow::CreateGameWindow(width, height, title);
            break;
        }  
        default:
            Log("lua_CreateGameWindow number of params=", paramCount, "exected=", 3);
    }
    return 0;
}

static int lua_Run(lua_State* state)
{
    int paramCount = lua_gettop(state);

    switch (paramCount)
    {
    case 0:
    {
        GameWindow::s_instance->Run();
        break;
    }
    default:
        break;
    }
    return 0;
}

void lua_RegisterGameWindow(lua_State* state)
{
    static const luaL_Reg static_methods[] = {
		{ "CreateGameWindow", lua_CreateGameWindow },
		{ "Run", lua_Run },
		{ NULL,	NULL },
	};

    //	Create an empty metatable and push it onto the stack.
	//	NOTE: The name does NOT need to match the name of our static struct above
	//	this is only done for readability
	luaL_newmetatable(state, "Entity_Methods");

    //	Duplicate the metatable so 2 now exist on the stack.
	lua_pushvalue(state, -1);

    /* Pop the first metatable off the stack and assign it to __index
	* of the second one. We set the metatable for the table to itself.
	* This is equivalent to the following in lua:
	* metatable = {}
	* metatable.__index = metatable
	*/
	lua_setfield(state, -2, "__index");

    //	Add our methods into the metatable.
	luaL_setfuncs(state, static_methods, 0);
	lua_setglobal(state, "GameWindow");
}
