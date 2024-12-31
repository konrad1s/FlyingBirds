#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>

class ResourceManager
{
public:
    ResourceManager(const ResourceManager &) = delete;
    ResourceManager(ResourceManager &&) = delete;
    ResourceManager &operator=(const ResourceManager &) = delete;
    ResourceManager &operator=(ResourceManager &&) = delete;

    static ResourceManager &getInstance();

    template <typename T>
    std::shared_ptr<T> acquire(const std::string &key, const std::string &filename);

    template <typename T>
    std::shared_ptr<T> tryGet(const std::string &key);

    template <typename T>
    void release(const std::string &key);

    void unloadAll();

private:
    ResourceManager() = default;
    ~ResourceManager() = default;

    struct ResourceData
    {
        std::shared_ptr<void> resource;
        int refCount{0};
    };

    std::unordered_map<std::type_index, std::unordered_map<std::string, ResourceData>> resources;

    std::shared_mutex resourceMutex;
};

#include "ResourceManager.ipp"
