#ifndef FILE_SYSTEM_UTILS_HPP
#define FILE_SYSTEM_UTILS_HPP

#include <string>
#include <filesystem>
#include <thread>
#include <utils/Types.hpp>

class File
{
public:
    std::filesystem::path m_path;

public:
    File() { }
    File(const std::string& path) : m_path(std::filesystem::path(path)) { }

    std::string Path() const { return m_path.c_str(); }
    std::string Name() const { return m_path.filename().c_str(); }
    std::string Extn() const { return m_path.extension().c_str(); }

    std::string Read();
    bool Write(const std::string& data);
    void Rename(const std::string& name);
};

class DirectoryNode : public std::enable_shared_from_this<DirectoryNode>
{
public:
    File                            m_file;
    std::vector<PTR<DirectoryNode>> m_children;
    bool                            m_isDirectory;
    PTR<DirectoryNode>              m_parent;
    bool                            m_open;

public:
    DirectoryNode() {} 
    DirectoryNode(const std::string& filePath);

    void RecursivelyAddDirectoryNodes(const PTR<DirectoryNode>& parentNode, std::filesystem::directory_iterator directoryIterator);
    void Open();

    std::string Name() const { return m_file.Name(); }
    std::string Path() const { return m_file.Path(); }
    std::string Extn() const { return m_file.Extn(); }

    void Rename(const std::string& name);

public:
    static PTR<DirectoryNode> CreateDirectryNodeTreeFromPathAsync(const std::string& rootPath);
};



#endif