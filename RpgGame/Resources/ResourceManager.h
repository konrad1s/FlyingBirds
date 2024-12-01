#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <mutex>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class ResourceManager 
{
public:
    static ResourceManager& getInstance();

    template <typename T>
    void load(const std::string &resourceName, const std::string &filename);

    template <typename T>
    std::shared_ptr<T> get(const std::string& name);

    template <typename T>
    bool exist(const std::string& name);

    template <typename T>
    void unload(const std::string& name);

    void unloadAll();

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

private:
    ResourceManager() = default;
    ~ResourceManager() = default;

    std::unordered_map<std::type_index, std::unordered_map<std::string, std::shared_ptr<void>>> resources;
    std::mutex resourceMutex;
};

template <typename T>
void ResourceManager::load(const std::string &resourceName, const std::string &filename)
{
    std::lock_guard<std::mutex> lock(resourceMutex);

    auto typeIndex = std::type_index(typeid(T));
    auto& resourceMap = resources[typeIndex];
    if (resourceMap.find(resourceName) != resourceMap.end())
    {
        throw std::runtime_error("ResourceManager::load - Resource '" + resourceName + "' already loaded.");
    }

    std::shared_ptr<T> resource = std::make_shared<T>();
    if (!resource->loadFromFile(filename))
    {
        throw std::runtime_error("ResourceManager::load - Failed to load " + filename);
    }

    resourceMap[resourceName] = resource;
}

template <typename T>
std::shared_ptr<T> ResourceManager::get(const std::string& name)
{
    std::lock_guard<std::mutex> lock(resourceMutex);

    auto typeIt = resources.find(std::type_index(typeid(T)));
    if (typeIt == resources.end())
    {
        throw std::runtime_error("ResourceManager::get - No resources of this type loaded.");
    }

    auto& resourceMap = typeIt->second;
    auto resourceIt = resourceMap.find(name);
    if (resourceIt == resourceMap.end())
    {
        throw std::runtime_error("ResourceManager::get - Resource '" + name + "' not found.");
    }

    return std::static_pointer_cast<T>(resourceIt->second);
}

template <typename T>
bool ResourceManager::exist(const std::string& name)
{
    std::lock_guard<std::mutex> lock(resourceMutex);

    auto typeIt = resources.find(std::type_index(typeid(T)));
    if (typeIt == resources.end())
    {
        return false;
    }

    auto &resourceMap = typeIt->second;
    auto resourceIt = resourceMap.find(name);

    return resourceIt != resourceMap.end();
}

template <typename T>
void ResourceManager::unload(const std::string& name)
{
    std::lock_guard<std::mutex> lock(resourceMutex);

    auto typeIt = resources.find(std::type_index(typeid(T)));
    if (typeIt != resources.end())
    {
        typeIt->second.erase(name);
    }
}
