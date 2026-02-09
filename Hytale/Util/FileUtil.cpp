#include "Util.h"

#include <iostream>
#include <Windows.h>

#include "../Core.h"


std::filesystem::path Util::GetDirectory() {
    char path[MAX_PATH];
    GetModuleFileNameA(Core::MODULEPTR, path, MAX_PATH);
    std::cout << "DLL directory: " << path << "\n";
    return std::filesystem::path(path).parent_path();
}