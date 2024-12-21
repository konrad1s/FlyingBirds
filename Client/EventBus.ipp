
template <typename T>
void EventBus::publish(const T &event)
{
    std::lock_guard<std::mutex> lock(busMutex);

    auto it = handlers.find(std::type_index(typeid(T)));

    if (it == handlers.end())
    {
        /* No subscribers found */
        return;
    }

    const std::any eventData(event);

    for (auto &func : it->second)
    {
        /* Notify subscribers */
        func(eventData);
    }
}

template <typename T>
void EventBus::subscribe(const Handler<T> &handler)
{
    std::lock_guard<std::mutex> lock(busMutex);

    auto wrapper = [handler](const std::any &data)
    {
        if (auto ptr = std::any_cast<const T>(&data))
        {
            handler(*ptr);
        }
    };

    handlers[std::type_index(typeid(T))].push_back(wrapper);
}
