#include <engine/ui/EngineUI.hpp>

#include <utils/Logger.hpp>
#include <engine/ui/docks/DirectoryViewDock.hpp>
#include <memory>
std::shared_ptr<EngineUI> EngineUI::Get()
{
    if (s_instance == nullptr)
        s_instance = std::make_shared<EngineUI>();
    return s_instance;
}
    
EngineUI::EngineUI()
{
    LOG("Initialize engine UI!");
    m_uiDocks.insert(
        std::make_pair<std::string, std::shared_ptr<EngineUIDockComponent>>("Directory view", std::make_shared<DirectoryViewDock>())
        );
}
EngineUI::~EngineUI()
{
    LOG("Destroy engine UI!");
}

void EngineUI::Render()
{
    // LOG("EngineUI::Render");
    for(const auto& [key, value] : m_uiDocks)
    {
        value->Render();
    }
}