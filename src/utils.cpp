#include "utils.hpp"

#ifdef  _WIN32
    std::string getPath(std::string name){
        //Evita que a string original seja alterada
        std::string nameCopy(name);
        std::replace(nameCopy.begin(), nameCopy.end(), '/', '\\');
        std::filesystem::path path = std::filesystem::current_path();
        std::string parentPath(path.parent_path().string());
        std::string texturePath(parentPath + "\\" + nameCopy);
        
        return texturePath;
    }

    std::string getParentFolder(std::string path){
        std::string returnPath = path.substr(0, path.find_last_of('\\'));
        returnPath += '\\';
        return returnPath;
    }

#elif
    std::string getPath(std::string name){
        //Evita que a string original seja alterada
        std::string nameCopy(name);
        std::replace(nameCopy.begin(), nameCopy.end(), '\\', '/');
        std::filesystem::path path = std::filesystem::current_path();
        std::string parentPath(path.parent_path().string());
        std::string texturePath(parentPath + "/" + nameCopy);

        return texturePath;
    }

    std::string getParentFolder(std::string path){
        std::string returnPath = path.substr(0, path.find_last_of('/'));
        returnPath += '\\';
        return returnPath;
    }
#endif