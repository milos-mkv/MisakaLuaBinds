#include <utils/FileSystemUtils.hpp>

#include <filesystem>
#include <fstream>
#include <utils/Logger.hpp>

std::string File::Read() 
{
    std::ifstream file(Path());

    if (!file.good()) { return ""; }
	
    std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    return data;
}

bool File::Write(const std::string& data)
{
    std::ofstream file(Path());

    if (!file.good()) { return false; }
  
    file << data;
    file.close();

    return true;
}

void File::Rename(const std::string& name)
{
    const std::string newPath = std::string(m_path.parent_path().c_str()) + "/" + name;
    std::filesystem::rename(m_path, newPath);
    m_path = std::filesystem::path(newPath);
}

DirectoryNode::DirectoryNode(const std::string& filePath) : m_file(File(filePath)), m_parent(nullptr), m_open(false) { 
    m_isDirectory = std::filesystem::is_directory(filePath);
}

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
	return CreatePTR(DirectoryNode, rootPath);
}

void DirectoryNode::Open()
{
    if (m_open || !m_isDirectory)
    {
        return;
    }
    m_open = true;
    auto directoryIterator = std::filesystem::directory_iterator(m_file.Path());

    for (const std::filesystem::directory_entry& entry : directoryIterator)
	{
		PTR<DirectoryNode> childNode = CreatePTR(DirectoryNode, entry.path().c_str());
        m_children.push_back(childNode);
        childNode->m_parent = shared_from_this();
	}
    
    std::sort(m_children.begin(), m_children.end(),[](const PTR<DirectoryNode>& d1, const PTR<DirectoryNode>& d2){
        return d1->m_file.Name()[0] < d2->m_file.Name()[0];
    });

	auto moveDirectoriesToFront = [](const PTR<DirectoryNode>& a, const PTR<DirectoryNode>& b) { return (a->m_isDirectory > b->m_isDirectory); };
	std::sort(m_children.begin(), m_children.end(), moveDirectoriesToFront);
}

void DirectoryNode::Rename(const std::string& name)
{
    if (name.empty())
    {
        LOG("FAILED TO RENAME!");
        return;
    }
    m_file.Rename(name);
    m_children.clear();
    m_open = false;
}