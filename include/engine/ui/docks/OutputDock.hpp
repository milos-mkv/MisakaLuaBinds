#ifndef MISAKA_ENGINE_OUTPUT_DOCK
#define MISAKA_ENGINE_OUTPUT_DOCK

#include <engine/ui/EngineUIDock.hpp>
#include <string>
#include <memory>

class OutputDock : public EngineUIDock
{
public:
    OutputDock();
    void Render() override;
    void Destroy() override;

};

#endif