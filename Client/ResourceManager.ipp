#include "Logger.h"
#include "EmbeddedAssetRegistry.h"

template <typename T>
std::shared_ptr<T> ResourceManager::acquire(const std::string &key, const std::string &filename)
{
    std::unique_lock<std::shared_mutex> lock(resourceMutex);
    auto &resourceMap = resources[std::type_index(typeid(T))];

    auto it = resourceMap.find(key);
    if (it != resourceMap.end())
    {
        return std::static_pointer_cast<T>(it->second);
    }

    auto assetOpt = EmbeddedAssetRegistry::instance().getAsset(filename);
    if (!assetOpt)
    {
        throw std::runtime_error("Embedded asset not found: " + filename);
    }

    const auto &[dataPtr, dataSize] = assetOpt->get();

    auto resource = std::make_shared<T>();
    if (!resource->loadFromMemory(dataPtr, dataSize))
    {
        throw std::runtime_error("Failed to load embedded asset: " + filename);
    }

    resourceMap[key] = resource;
    return resource;
}

template <typename T>
std::shared_ptr<T> ResourceManager::tryGet(const std::string &key)
{
    std::shared_lock<std::shared_mutex> lock(resourceMutex);
    auto &resourceMap = resources[std::type_index(typeid(T))];
    auto it = resourceMap.find(key);
    if (it != resourceMap.end())
    {
        return std::static_pointer_cast<T>(it->second);
    }
    return nullptr;
}

template <typename T>
void ResourceManager::release(const std::string &key)
{
    std::unique_lock<std::shared_mutex> lock(resourceMutex);
    auto &resourceMap = resources[std::type_index(typeid(T))];
    resourceMap.erase(key);
}
