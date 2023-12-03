#include <utils/FileSystemUtils.hpp>

#include <filesystem>
#include <fstream>

File::File(const std::string& path) : m_path(std::filesystem::path(path))
{ }

std::string File::Path() const { return m_path.c_str(); }

std::string File::Name() const { return m_path.filename().c_str(); }

std::string File::Extn() const { return m_path.extension().c_str(); }

std::string File::Read() 
{
    std::ifstream file(Path());
    if (!file.good())
	{
        return "";
    }
	
    std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    return data;
}

bool File::Write(const std::string& data)
{
    std::ofstream file(Path());

    if (!file.good()) 
    {
        return false;
	}
  
    file << data;
    file.close();

    return true;
}

DirectoryNode::DirectoryNode(const std::string& filePath) : m_file(File(filePath)), m_parent(nullptr)
{ }

void DirectoryNode::RecursivelyAddDirectoryNodes(const PTR<DirectoryNode>& parentNode, std::filesystem::directory_iterator directoryIterator)
{
	for (const std::filesystem::directory_entry& entry : directoryIterator)
	{
		PTR<DirectoryNode> childNode = CreatePTR(DirectoryNode, entry.path().c_str());
        parentNode->m_children.push_back(childNode);
        childNode->m_parent = parentNode;
		if (childNode->m_isDirectory = entry.is_directory(); childNode->m_isDirectory)
			RecursivelyAddDirectoryNodes(childNode, std::filesystem::directory_iterator(entry));
	}
    
    std::sort(parentNode->m_children.begin(), parentNode->m_children.end(),[](const PTR<DirectoryNode>& d1, const PTR<DirectoryNode>& d2){
        return d1->m_file.Name()[0] < d2->m_file.Name()[0];
    });

	auto moveDirectoriesToFront = [](const PTR<DirectoryNode>& a, const PTR<DirectoryNode>& b) { return (a->m_isDirectory > b->m_isDirectory); };
	std::sort(parentNode->m_children.begin(), parentNode->m_children.end(), moveDirectoriesToFront);
}

PTR<DirectoryNode> DirectoryNode::CreateDirectryNodeTreeFromPathAsync(const std::string& rootPath)
{
	PTR<DirectoryNode> rootNode = CreatePTR(DirectoryNode, rootPath);
	if (rootNode->m_isDirectory = std::filesystem::is_directory(rootPath); rootNode->m_isDirectory)
    {
        if(rootNode->m_thread.joinable()) { rootNode->m_thread.join(); }
		rootNode->m_thread = std::thread(&DirectoryNode::RecursivelyAddDirectoryNodes, rootNode.get(), rootNode, std::filesystem::directory_iterator(rootPath));
    }
	return rootNode;
}