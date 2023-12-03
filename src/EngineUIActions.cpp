#include <engine/ui/EngineUIActions.hpp>

#include <nfd/nfd_common.h>
#include <utils/Logger.hpp>

std::string EngineOpenFileDialog()
{
    LOG("EngineOpenFileDialog - Action");

    nfdchar_t *outPath = NULL;
    nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outPath);
        
    if (result == NFD_OKAY) 
    {
        LOG("File found:", outPath);
        return outPath;
    }
    else if (result == NFD_CANCEL) 
    {
        LOG("Open file dialog canceled");
        return "";
    }
    else 
    {
        LOG(NFD_GetError());
        return "";
    }
}

std::string EngineOpenFolderDialog()
{
    LOG("EngineOpenFolderDialog - Action");

    nfdchar_t *outPath = NULL;
    nfdresult_t result = NFD_PickFolder(NULL, &outPath);
        
    if (result == NFD_OKAY) 
    {
        LOG("Folder found:", outPath);
        return outPath;
    }
    else if (result == NFD_CANCEL) 
    {
        LOG("Open folder dialog canceled");
        return "";
    }
    else 
    {
        LOG(NFD_GetError());
        return "";
    }
}

std::string EngineSaveFileDialog()
{
    LOG("EngineSaveFileDialog - Action");

    nfdchar_t *outPath = NULL;
    nfdresult_t result = NFD_SaveDialog(NULL, NULL, &outPath);
    
    if (result == NFD_OKAY)
    {
        LOG("Folder found:", outPath);
        return outPath;
    }
    else if (result == NFD_CANCEL) 
    {
        LOG("Open folder dialog canceled");
        return "";
    }
    else 
    {
        LOG(NFD_GetError());
        return "";
    }
}