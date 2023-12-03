// #ifndef MISAKA_ENGINE_UTILS_DIRECTORY_NODE
// #define MISAKA_ENGINE_UTILS_DIRECTORY_NODE

// #include <string>
// #include <vector>
// #include <utils/Types.hpp>
// #include <utils/FilePath.hpp>
// #include <filesystem>
// #include <thread>

// struct DirectoryNode
// {
//     static inline std::thread s_thread;

//     FilePath m_file;
//     std::vector<PTR<DirectoryNode>> m_children;
//     bool m_isDirectory;

//     DirectoryNode();

//     void RecursivelyAddDirectoryNodes(const PTR<DirectoryNode>& parentNode, std::filesystem::directory_iterator directoryIterator);
//     static PTR<DirectoryNode> CreateDirectryNodeTreeFromPathAsync(const std::filesystem::path& rootPath);
// };

// #endif