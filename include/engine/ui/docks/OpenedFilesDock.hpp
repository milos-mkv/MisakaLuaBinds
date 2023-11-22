#ifndef MISAKA_ENGINE_OPENED_FILES_DOCK
#define MISAKA_ENGINE_OPENED_FILES_DOCK

#include "../EngineUIDock.hpp"
#include <utils/Logger.hpp>
#include <unordered_map>
#include <engine/gl/Texture.hpp>
#include <vector>
#include <filesystem>
#include <fstream>
#include <atomic>
#include <thread>

class OpenedFilesDock : public EngineUIDock
{
public:

    virtual void Render() override;
    virtual void Destroy() override;

    OpenedFilesDock();
   ~OpenedFilesDock();

};

#endif