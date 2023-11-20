#ifndef MISAKA_ENGINE_CODE_EDITOR_DOCK
#define MISAKA_ENGINE_CODE_EDITOR_DOCK

#include <engine/ui/EngineUIDock.hpp>
#include <string>

class CodeEditorDock : public EngineUIDock
{
public:
    std::string path ="test";
    CodeEditorDock() { }
     void Render() override;
     void Destroy() override { }

};

#endif