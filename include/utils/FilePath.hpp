#ifndef MISAKA_ENGINE_UTILS_FILE_PATH
#define MISAKA_ENGINE_UTILS_FILE_PATH

#include <string>

struct FilePath
{
    std::string m_path;
    std::string m_filename;
    std::string m_extension;

    FilePath() {}
    FilePath(const std::string& path);
   ~FilePath();
   
    bool Write(const std::string& data);
    std::string Read();
};

#endif