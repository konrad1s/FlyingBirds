#pragma once

#include <string>
#include <unordered_map>
#include <cstddef>
#include <stdexcept>
#include <shared_mutex>
#include <optional>
#include <utility>
#include <functional>

class EmbeddedAssetRegistry
{
public:
    static EmbeddedAssetRegistry& instance()
    {
        static EmbeddedAssetRegistry registry;
        return registry;
    }

    EmbeddedAssetRegistry(const EmbeddedAssetRegistry&) = delete;
    EmbeddedAssetRegistry(EmbeddedAssetRegistry&&) = delete;
    EmbeddedAssetRegistry& operator=(const EmbeddedAssetRegistry&) = delete;
    EmbeddedAssetRegistry& operator=(EmbeddedAssetRegistry&&) = delete;

    void registerAsset(const std::string& assetPath, const unsigned char* data, size_t size)
    {
        std::unique_lock<std::shared_mutex> lock(resMutex);
        assets[assetPath] = { data, size };
    }

    std::optional<std::reference_wrapper<const std::pair<const unsigned char*, size_t>>> getAsset(const std::string& assetPath) const
    {
        std::shared_lock<std::shared_mutex> lock(resMutex);
        auto it = assets.find(assetPath);
        if (it != assets.end())
        {
            return std::cref(it->second);
        }
        return std::nullopt;
    }

private:
    EmbeddedAssetRegistry() = default;

    mutable std::shared_mutex resMutex;

    std::unordered_map<std::string, std::pair<const unsigned char*, size_t>> assets;
};
