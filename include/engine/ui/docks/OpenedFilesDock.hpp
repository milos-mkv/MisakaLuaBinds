#ifndef MISAKA_ENGINE_OPENED_FILES_DOCK
#define MISAKA_ENGINE_OPENED_FILES_DOCK

#include "../EngineUIDock.hpp"
#include <utils/Logger.hpp>
#include <unordered_map>
#include <engine/gl/Texture.hpp>
#include <engine/ui/docks/CodeEditorDock.hpp>
#include <vector>
#include <filesystem>
#include <fstream>
#include <atomic>
#include <thread>
#include <utils/Types.hpp>

class OpenedFilesDock : public EngineUIDock
{
public:
    std::unordered_map<std::string, PTR<CodeEditorDock>> m_files;

public:

    virtual void Render() override;
    virtual void Destroy() override;

    OpenedFilesDock();
   ~OpenedFilesDock();

   void OpenFile(const std::string& path, std::string fileName = "", std::string ext = "");

};

#endif