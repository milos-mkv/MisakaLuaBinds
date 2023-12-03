#include <utils/FilePath.hpp>

#include <utils/Utils.hpp>
#include <utils/Logger.hpp>

#include <fstream>
#include <filesystem>

FilePath::FilePath(const std::string& path) 
    : m_path(path)
{
    LOG("FilePath::FilePath", path);

    std::filesystem::path p(path);
    m_filename  = p.filename().c_str();
    m_extension = p.extension().c_str();
}

FilePath::~FilePath()
{
    LOG("FilePath::~FilePath");
}

bool FilePath::Write(const std::string& data)
{
    LOG("FilePath::Write", data.size());

    std::ofstream file(m_path);

    if (!file.good()) 
    {
        LOG("Failed to open file:", m_path);
        return false;
	}
  
    file << data;
    file.close();

    return true;
}

std::string FilePath::Read()
{
    LOG("FilePath::Read");

    std::ifstream file(m_path);
    if (!file.good())
	{
        LOG("Failed to open file:", m_path);
        return "";
    }
	
    std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    return data;
}