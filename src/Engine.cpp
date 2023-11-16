#include <engine/Engine.hpp>
#include <Utils.hpp>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <OpenGL/gl.h>
#include <TextEditor.h>
#include <fstream>
#include <filesystem>
// #include <OpenGL/gl3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void Engine::CreateEngine()
{
    if (s_instance == nullptr)
        s_instance = std::make_shared<Engine>();
}

Engine::Engine()
{
    Log("Engine initialize!");
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 

    m_window = glfwCreateWindow(800, 600, "Misaka Engine", nullptr, nullptr);
    if (m_window == nullptr)
    {
        Log("Failed to create GLFW window!");
        return;
    }

    glfwMakeContextCurrent(m_window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 150");
}

struct DirectoryNode
{
	std::string FullPath;
	std::string FileName;
	std::vector<DirectoryNode> Children;
	bool IsDirectory;
};

void RecursivelyAddDirectoryNodes(DirectoryNode& parentNode, std::filesystem::directory_iterator directoryIterator)
{
	for (const std::filesystem::directory_entry& entry : directoryIterator)
	{
		DirectoryNode& childNode = parentNode.Children.emplace_back();
		childNode.FullPath = entry.path().c_str();
		childNode.FileName = entry.path().filename().c_str();
		if (childNode.IsDirectory = entry.is_directory(); childNode.IsDirectory)
			RecursivelyAddDirectoryNodes(childNode, std::filesystem::directory_iterator(entry));
	}

	auto moveDirectoriesToFront = [](const DirectoryNode& a, const DirectoryNode& b) { return (a.IsDirectory > b.IsDirectory); };
	std::sort(parentNode.Children.begin(), parentNode.Children.end(), moveDirectoriesToFront);
}

DirectoryNode CreateDirectryNodeTreeFromPath(const std::filesystem::path& rootPath)
{
	DirectoryNode rootNode;
	rootNode.FullPath = rootPath.c_str();
	rootNode.FileName = rootPath.filename().c_str();
	if (rootNode.IsDirectory = std::filesystem::is_directory(rootPath); rootNode.IsDirectory)
		RecursivelyAddDirectoryNodes(rootNode, std::filesystem::directory_iterator(rootPath));

	return rootNode;
}
GLuint id;
GLuint id2;
void RecursivelyDisplayDirectoryNode(const DirectoryNode& parentNode)
{
	ImGui::PushID(&parentNode);
	if (parentNode.IsDirectory)
	{
        auto cur = ImGui::GetCursorPos();
       cur.x += 4;
       cur.y += 2;

		if (ImGui::TreeNodeEx(parentNode.FileName.c_str(), ImGuiTreeNodeFlags_SpanFullWidth))
		{
			for (const DirectoryNode& childNode : parentNode.Children)
			{	
                RecursivelyDisplayDirectoryNode(childNode);
            }

			ImGui::TreePop();
                        

		}

        auto cur2 = ImGui::GetCursorPos();
        ImGui::SetCursorPos(cur);
        ImGui::Image((ImTextureID) id, {20, 20}); 
                ImGui::SetCursorPos(cur2);

	}
	else
	{
          auto cur = ImGui::GetCursorPos();
          auto cur2 = cur;
       cur.x += 4;
       cur.y += 2;
               ImGui::SetCursorPos(cur);

        ImGui::Image((ImTextureID) id2, {20, 20}); 
        ImGui::SameLine();
        ImGui::SetCursorPos(cur2);
		if (ImGui::TreeNodeEx(parentNode.FileName.c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanFullWidth))
		{
          
		}

	}
	ImGui::PopID();
}

void Engine::Run()
{
    TextEditor editor;
    auto lang = TextEditor::LanguageDefinition::Lua();

	// set your own known preprocessor symbols...
	static const char* ppnames[] = { "NULL", "PM_REMOVE",
		"ZeroMemory", "DXGI_SWAP_EFFECT_DISCARD", "D3D_FEATURE_LEVEL", "D3D_DRIVER_TYPE_HARDWARE", "WINAPI","D3D11_SDK_VERSION", "assert" };
	// ... and their corresponding values
	static const char* ppvalues[] = { 
		"#define NULL ((void*)0)", 
		"#define PM_REMOVE (0x0001)",
		"Microsoft's own memory zapper function\n(which is a macro actually)\nvoid ZeroMemory(\n\t[in] PVOID  Destination,\n\t[in] SIZE_T Length\n); ", 
		"enum DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD = 0", 
		"enum D3D_FEATURE_LEVEL", 
		"enum D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE  = ( D3D_DRIVER_TYPE_UNKNOWN + 1 )",
		"#define WINAPI __stdcall",
		"#define D3D11_SDK_VERSION (7)",
		" #define assert(expression) (void)(                                                  \n"
        "    (!!(expression)) ||                                                              \n"
        "    (_wassert(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0) \n"
        " )"
		};
	for (int i = 0; i < sizeof(ppnames) / sizeof(ppnames[0]); ++i)
	{
		TextEditor::Identifier id;
		id.mDeclaration = ppvalues[i];
		lang.mPreprocIdentifiers.insert(std::make_pair(std::string(ppnames[i]), id));
	}

	// set your own identifiers
	static const char* identifiers[] = {
		"HWND", "HRESULT", "LPRESULT","D3D11_RENDER_TARGET_VIEW_DESC", "DXGI_SWAP_CHAIN_DESC","MSG","LRESULT","WPARAM", "LPARAM","UINT","LPVOID",
		"ID3D11Device", "ID3D11DeviceContext", "ID3D11Buffer", "ID3D11Buffer", "ID3D10Blob", "ID3D11VertexShader", "ID3D11InputLayout", "ID3D11Buffer",
		"ID3D10Blob", "ID3D11PixelShader", "ID3D11SamplerState", "ID3D11ShaderResourceView", "ID3D11RasterizerState", "ID3D11BlendState", "ID3D11DepthStencilState",
		"IDXGISwapChain", "ID3D11RenderTargetView", "ID3D11Texture2D", "TextEditor" };
	static const char* idecls[] = 
	{
		"typedef HWND_* HWND", "typedef long HRESULT", "typedef long* LPRESULT", "struct D3D11_RENDER_TARGET_VIEW_DESC", "struct DXGI_SWAP_CHAIN_DESC",
		"typedef tagMSG MSG\n * Message structure","typedef LONG_PTR LRESULT","WPARAM", "LPARAM","UINT","LPVOID",
		"ID3D11Device", "ID3D11DeviceContext", "ID3D11Buffer", "ID3D11Buffer", "ID3D10Blob", "ID3D11VertexShader", "ID3D11InputLayout", "ID3D11Buffer",
		"ID3D10Blob", "ID3D11PixelShader", "ID3D11SamplerState", "ID3D11ShaderResourceView", "ID3D11RasterizerState", "ID3D11BlendState", "ID3D11DepthStencilState",
		"IDXGISwapChain", "ID3D11RenderTargetView", "ID3D11Texture2D", "class TextEditor" };

        for (int i = 0; i < sizeof(identifiers) / sizeof(identifiers[0]); ++i)
	{
		TextEditor::Identifier id;
		id.mDeclaration = std::string(idecls[i]);
		lang.mIdentifiers.insert(std::make_pair(std::string(identifiers[i]), id));
	}
	editor.SetLanguageDefinition(lang);

    std::string path = "../include/";
    for (const auto & entry : std::filesystem::directory_iterator(path))
         std::cout << entry.path() << std::endl; 

    // TextEditor::ErrorMarkers markers;
	// markers.insert(std::make_pair<int, std::string>(6, "Example error here:\nInclude file not found: \"TextEditor.h\""));
	// markers.insert(std::make_pair<int, std::string>(41, "Another example error"));
	// editor.SetErrorMarkers(markers);

	static const char* fileToEdit = "../scripts/script.lua";
    {
		std::ifstream t(fileToEdit);
		if (t.good())
		{
			std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
			editor.SetText(str);
		}
	}

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImFont* pFont = io.Fonts->AddFontFromFileTTF("./JetBrainsMono-Bold.ttf", 25.0f);

    char* a = "HELLO";
    bool save = false;

    // stbi_set_flip_vertically_on_load(true);
    GLint width;
    GLint height;
    GLint noc;
    GLubyte* data = stbi_load("./folder.png", &width, &height, &noc, 0);

    GLenum format = (noc == 3) ? GL_RGB : (noc == 4) ? GL_RGBA : GL_RED;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    // glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

     data = stbi_load("./file.png", &width, &height, &noc, 0);

     format = (noc == 3) ? GL_RGB : (noc == 4) ? GL_RGBA : GL_RED;

    glGenTextures(1, &id2);
    glBindTexture(GL_TEXTURE_2D, id2);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    // glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

std::filesystem::path sss("/Users/milos.milicevic/Documents/GitHub/MisakaLuaBinds");

 DirectoryNode rootNode = CreateDirectryNodeTreeFromPath(sss);




    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        if (ImGui::BeginMainMenuBar()) {
              if (ImGui::BeginMenu("File")) {
                   if (ImGui::MenuItem("Open", "Ctrl+O")) { 
                   }
                    ImGui::Separator();
                    if(ImGui::MenuItem("Exit")) {}
             ImGui::EndMenu();
             }
             ImGui::EndMainMenuBar();
        }
        
        ImGui::Begin("Finder");
        	RecursivelyDisplayDirectoryNode(rootNode);
        ImGui::End();

        ImGui::Begin("script.lua");
        ImGui::PushFont(pFont);
		editor.Render("TextEditor");

        
        if (glfwGetKey(m_window, GLFW_KEY_0) && save == false)
        {
            std::ofstream s("../scripts/script.lua");
            s << editor.GetText();
            s.close();
            save = true;
        }

        ImGui::PopFont();
        ImGui::End();
        ImGui::ShowDemoWindow();

        ImGui::Render();
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);
    }
}

Engine::~Engine()
{
    Log("Engine destroy!");
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}