#pragma once
#define MAXOBSERVERS 26
#include "Actor.h"
#include "Singleton.h"

//template<typename T>
class Event
{
public:
    using Callback = std::function<void()>;
    using CallbackId = size_t;

    Event() : m_callbackIdCounter(0) {}

    CallbackId Subscribe(Callback callback)
    {
        m_callbacks[m_callbackIdCounter] = callback;
        return m_callbackIdCounter++;
    }

    void Unsubscribe(CallbackId id)
    {
        m_callbacks.erase(id);
    }

    void Invoke()
    {
        if (m_callbackIdCounter == 0)
        {
            return;
        }
        for (auto& [id, callback] : m_callbacks)
        {
            callback();
        }
    }

private:
    std::map<CallbackId, Callback> m_callbacks;
    CallbackId m_callbackIdCounter;
};

class EventManager : public Singleton<EventManager>
{
    //EventManager() {}
    //static EventManager* m_Instance;

public:
    //static EventManager* Instance() { return m_Instance = (m_Instance == nullptr) ? new EventManager() : m_Instance; }
    Event CoinCollected;
    void OnCoinCollected() {

        CoinCollected.Invoke();
    }

    Event PlayerDead;
    void OnPlayerDead() { PlayerDead.Invoke(); }
    
    Event PlayMusic;
    void OnPlayMusic() { PlayMusic.Invoke(); }

};