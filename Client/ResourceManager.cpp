#include "ResourceManager.h"

ResourceManager& ResourceManager::getInstance()
{
    static ResourceManager instance;
    return instance;
}

void ResourceManager::unloadAll()
{
    std::unique_lock<std::shared_mutex> lock(resourceMutex);

    resources.clear();
}
