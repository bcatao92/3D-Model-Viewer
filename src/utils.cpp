#include "utils.hpp"

#ifdef  _WIN32
    std::string getPath(std::string name){
        //Evita que a string original seja alterada
        std::string nameCopy(name);
        std::replace(nameCopy.begin(), nameCopy.end(), '/', '\\');
        std::filesystem::path path = std::filesystem::current_path();
        std::string texturePath(path.string() + "\\" + nameCopy);
        
        return texturePath;
    }

    std::string getParentFolder(std::string path){
        std::string returnPath = path.substr(0, path.find_last_of('\\'));
        returnPath += '\\';
        return returnPath;
    }

    std::string getPathTo(std::string name){
        std::string result = getPath(name);
        return result + '\\';
    }

    std::string getFileName(std::string path){
        std::string nameCopy(path);
        std::replace(nameCopy.begin(), nameCopy.end(), '\\', '/');
        std::string temp = nameCopy.substr(nameCopy.find_last_of('/')+1, nameCopy.back());
        std::replace(temp.begin(), temp.end(), '/', '\\');
        return temp;

    }

#else
    std::string getPath(std::string name){
        //Evita que a string original seja alterada
        std::string nameCopy(name);
        std::replace(nameCopy.begin(), nameCopy.end(), '\\', '/');
        std::filesystem::path path = std::filesystem::current_path();
        std::string texturePath(path.string() + "/" + nameCopy);

        return texturePath;
    }

    std::string getParentFolder(std::string path){
        std::string returnPath = path.substr(0, path.find_last_of('/'));
        returnPath += '/';
        return returnPath;
    }

    std::string getPathTo(std::string name){
        std::string result = getPath(name);
        return result + '/';
    }

    std::string getFileName(std::string path){
        std::string nameCopy(path);
        std::replace(nameCopy.begin(), nameCopy.end(), '\\', '/');
        std::string temp = nameCopy.substr(nameCopy.find_last_of('/')+1, nameCopy.back());
        return temp;
    }
#endif