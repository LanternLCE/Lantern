#pragma once
#include <vector>
#include <functional>

#ifdef LANTERN_EXPORTS
#define EVENT_API __declspec(dllexport)
#else  
#define EVENT_API __declspec(dllimport)
#endif

/// <summary>
/// Very barebones Event class.
/// 
/// Allows for listeners to be registered and for all listeners to be later called.
/// </summary>
class EVENT_API Event {
private:
    std::vector<std::function<void()>> listeners;
public:
    /// <summary>
    /// Adds a listener to be called when the event is fired
    /// </summary>
    /// <param name="listener">The method that will get called when fired</param>
    void addEventListener(const std::function<void()>& listener) {
        listeners.push_back(listener);
    }

    // TODO: Does not exist for some reason (can't compare functions)
    ///// <summary>
    ///// Removes a listener from the registered listeners vector
    ///// </summary>
    ///// <param name="listener">The listener that should be removed</param>
    //void removeEventListener(const std::function<void()>& listener) {
    //    listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
    //}

    /// <summary>
    /// Fires an event, which will call all listeners.
    /// </summary>
    void fire() {
        for (const auto& listener : listeners) {
            listener();
        }
    }
};
