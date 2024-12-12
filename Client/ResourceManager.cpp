#include "ResourceManager.h"

ResourceManager& ResourceManager::getInstance()
{
    static ResourceManager instance;
    return instance;
}

void ResourceManager::unloadAll()
{
    std::lock_guard<std::mutex> lock(resourceMutex);

    resources.clear();
}
