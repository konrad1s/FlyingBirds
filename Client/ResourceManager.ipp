#include "Logger.h"

template <typename T>
std::shared_ptr<T> ResourceManager::acquire(const std::string &key, const std::string &filename)
{
    auto typeIdx = std::type_index(typeid(T));

    {
        std::shared_lock<std::shared_mutex> sharedLock(resourceMutex);
        auto typeIt = resources.find(typeIdx);

        if (typeIt != resources.end())
        {
            auto &resourceMap = typeIt->second;
            auto it = resourceMap.find(key);

            if (it != resourceMap.end())
            {
                it->second.refCount++;
                Logger::debug("Resource '{}' of type '{}' acquired, refCount = {}",
                              key, typeIdx.name(), it->second.refCount);

                auto resourcePtr = std::static_pointer_cast<T>(it->second.resource);
                
                auto deleter = [this, key](T *) {
                    this->release<T>(key);
                };

                return std::shared_ptr<T>(resourcePtr.get(), deleter);
            }
        }
    }

    if (!filename.empty())
    {
        std::unique_lock<std::shared_mutex> uniqueLock(resourceMutex);

        /* Double-check if it got created by another thread in the meantime */
        auto &resourceMap = resources[typeIdx];
        auto it = resourceMap.find(key);
        auto deleter = [this, key](T *)
        {
            this->release<T>(key);
        };

        if (it != resourceMap.end())
        {
            it->second.refCount++;
            Logger::debug("Resource '{}' of type '{}' acquired (via second check), refCount = {}",
                          key, typeIdx.name(), it->second.refCount);

            auto resourcePtr = std::static_pointer_cast<T>(it->second.resource);
        
            return std::shared_ptr<T>(resourcePtr.get(), deleter);
        }

        auto resource = std::make_shared<T>();
        if (!resource->loadFromFile("../../../assets/" + filename))
        {
            Logger::error("Failed to load resource '{}' from file '{}'.", key, filename);
            throw std::runtime_error("Failed to load file: " + filename);
        }

        resourceMap[key] = {resource, 1};
        Logger::debug("Loaded and acquired new resource '{}' of type '{}'. refCount = 1", key, typeIdx.name());

        return std::shared_ptr<T>(resource.get(), deleter);
    }
    else
    {
        throw std::runtime_error("Failed to get resource: " + key);
    }
}

template <typename T>
std::shared_ptr<T> ResourceManager::tryGet(const std::string &key)
{
    return acquire(key, "");
}

template <typename T>
void ResourceManager::release(const std::string &key)
{
    std::unique_lock<std::shared_mutex> lock(resourceMutex);

    auto typeIdx = std::type_index(typeid(T));
    auto itType = resources.find(typeIdx);
    if (itType == resources.end())
    {
        Logger::warning("Attempted to release resource '{}' of type '{}', but type does not exist.", key, typeIdx.name());
        return;
    }

    auto &typeMap = itType->second;
    auto itRes = typeMap.find(key);
    if (itRes == typeMap.end())
    {
        Logger::warning("Attempted to release resource '{}' of type '{}', but key not found.", key, typeIdx.name());
        return;
    }

    itRes->second.refCount--;

    if (itRes->second.refCount <= 0)
    {
        typeMap.erase(itRes);
        Logger::debug("Resource '{}' of type '{}' fully unloaded (refCount=0).", key, typeIdx.name());
    }
}
