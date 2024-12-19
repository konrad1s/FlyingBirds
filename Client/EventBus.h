#pragma once

#include <unordered_map>

class EventBus
{
public:

private:

};


class EventBase {
public:
    virtual ~EventBase() = default;
};

template<typename T>
class Event : public EventBase {
public:
    T data;
    explicit Event(const T &d) : data(d) {}
};

// Simple EventBus
class EventBus {
public:
    template<typename T>
    using Handler = std::function<void(const T&)>;

    template<typename T>
    void subscribe(Handler<T> handler);

    template<typename T>
    void publish(const T &eventData);

private:
    std::unordered_map<std::type_index, std::vector<std::function<void(EventBase*)>>> handlers;
    std::mutex busMutex;
};

eventBus.subscribe<uint32_t>([this](const uint32_t& cid) { this->onServerWelcome(cid); });
eventBus.subscribe<agario::GameState>([this](const agario::GameState &st) { this->onGameState(st); });

eventBus.publish<agario::GameState>(state);
eventBus.publish<uint32_t>(cid);

#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <mutex>

class EventBus {
public:
    template<typename T>
    using Handler = std::function<void(const T&)>;

    template<typename T>
    void subscribe(Handler<T> handler) {
        std::lock_guard<std::mutex> lock(busMutex);
        auto tid = std::type_index(typeid(T));
        handlers[tid].push_back([handler](void* e){
            handler(*static_cast<T*>(e));
        });
    }

    template<typename T>
    void publish(const T &eventData) {
        std::lock_guard<std::mutex> lock(busMutex);
        auto tid = std::type_index(typeid(T));
        auto it = handlers.find(tid);
        if (it == handlers.end()) return;
        for (auto &func : it->second) {
            func((void*)&eventData);
        }
    }

private:
    std::unordered_map<std::type_index, std::vector<std::function<void(void*)>>> handlers;
    std::mutex busMutex;
};
