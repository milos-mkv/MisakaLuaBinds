#include <engine/Engine.hpp>
#include <utils/Logger.hpp>

int main(int argc, char const *argv[])
{
    Engine::Get()->Run();

    return EXIT_SUCCESS;
}