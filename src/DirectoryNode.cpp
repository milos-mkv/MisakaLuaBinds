// #include <utils/DirectoryNode.hpp>

// #include <utils/Logger.hpp>

// DirectoryNode::DirectoryNode()
// {
//     LOG("DirectoryNode::DirectoryNode");
// }

// void DirectoryNode::RecursivelyAddDirectoryNodes(const PTR<DirectoryNode>& parentNode, std::filesystem::directory_iterator directoryIterator)
// {
// 	for (const std::filesystem::directory_entry& entry : directoryIterator)
// 	{
// 		PTR<DirectoryNode> childNode = CreatePTR(DirectoryNode);
//         childNode->m_file = FilePath(entry.path().c_str());

//         parentNode->m_children.push_back(childNode);
// 		if (childNode->m_isDirectory = entry.is_directory(); childNode->m_isDirectory)
// 			RecursivelyAddDirectoryNodes(childNode, std::filesystem::directory_iterator(entry));
// 	}
    
//     std::sort(parentNode->m_children.begin(), parentNode->m_children.end(),[](const PTR<DirectoryNode>& d1, const PTR<DirectoryNode>& d2){
//         return d1->m_file.m_filename[0] < d2->m_file.m_filename[0];
//     });

// 	auto moveDirectoriesToFront = [](const PTR<DirectoryNode>& a, const PTR<DirectoryNode>& b) { return (a->m_isDirectory > b->m_isDirectory); };
// 	std::sort(parentNode->m_children.begin(), parentNode->m_children.end(), moveDirectoriesToFront);
// }

// PTR<DirectoryNode> DirectoryNode::CreateDirectryNodeTreeFromPathAsync(const std::filesystem::path& rootPath)
// {
// 	PTR<DirectoryNode> rootNode = CreatePTR(DirectoryNode);
//     rootNode->m_file = FilePath(rootPath.c_str());

// 	if (rootNode->m_isDirectory = std::filesystem::is_directory(rootPath); rootNode->m_isDirectory)
//     {
//         if(s_thread.joinable()) { s_thread.join(); }
// 		s_thread = std::thread(&DirectoryNode::RecursivelyAddDirectoryNodes, rootNode.get(), rootNode, std::filesystem::directory_iterator(rootPath));
//     }
// 	return rootNode;
// }