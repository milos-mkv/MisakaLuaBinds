#ifndef FILE_SYSTEM_UTILS_HPP
#define FILE_SYSTEM_UTILS_HPP

#include <string>
#include <filesystem>
#include <thread>
#include <utils/Types.hpp>

class File
{
private:
    std::filesystem::path m_path;

public:
    File() { }
    File(const std::string& path);

    std::string Path() const;
    std::string Name() const;
    std::string Extn() const;
    std::string Read();
    bool Write(const std::string& data);
};

class DirectoryNode
{
public:
    std::thread                     m_thread;
    File                            m_file;
    std::vector<PTR<DirectoryNode>> m_children;
    bool                            m_isDirectory;
    PTR<DirectoryNode>              m_parent;

public:
    DirectoryNode() {} 
    DirectoryNode(const std::string& filePath);

    void RecursivelyAddDirectoryNodes(const PTR<DirectoryNode>& parentNode, std::filesystem::directory_iterator directoryIterator);

    const std::string Name() const { return m_file.Name(); }
    const std::string Path() const { return m_file.Path(); }
    const std::string Extn() const { return m_file.Extn(); }

public:
    static PTR<DirectoryNode> CreateDirectryNodeTreeFromPathAsync(const std::string& rootPath);
};

#endif