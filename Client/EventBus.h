#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <mutex>
#include <any>

class EventBus
{
public:
    template <typename T>
    using Handler = std::function<void(const T &)>;

    template <typename T>
    void publish(const T &event);

    template <typename T>
    void subscribe(const Handler<T> &handler);

private:
    using anyFunction = std::function<void(const std::any &)>;

    std::unordered_map<std::type_index, std::vector<anyFunction>> handlers;
    std::mutex busMutex;
};

#include "EventBus.ipp"