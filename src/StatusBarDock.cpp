#include <engine/ui/docks/StatusBarDock.hpp>

#include <luaaa/luaaa.hpp>
#include <engine/ui/EngineUI.hpp>
#include <engine/ui/docks/DirectoryViewDock.hpp>

StatusBarDock::StatusBarDock()
{

}
static bool LuaStateCheckIfOK(lua_State* L, int r)
{
    if (r != LUA_OK)
    {
        std::cout << lua_tostring(L, -1) << std::endl;
        lua_pop(L, -1);
        return false;
    }
    return true;
}

void StatusBarDock::Render()
{
    return;
    // ImGui::SetNextWindowPos({ImGui::GetMainViewport()->Pos.x, ImGui::GetMainViewport()->Pos.y + 25});
    // ImGui::SetNextWindowSize({ ImGui::GetMainViewport()->Size.x, 25});
    // bool a = true;
    // ImGui::Begin("Status Bar", &a, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |ImGuiWindowFlags_NoScrollbar);
    // if(ImGui::Button("RUN"))
    // {
    //     auto file =EngineUI::Get()->GetDock<DirectoryViewDock>()->currentFolder + "/main.lua";
    //     lua_State* state = luaL_newstate();
    //     luaL_openlibs(state);
    //     if(LuaStateCheckIfOK(state, luaL_dofile(state, file.c_str())))
    //     {

    //     }
    // }
    // ImGui::End();
}

void StatusBarDock::Destroy()
{

}