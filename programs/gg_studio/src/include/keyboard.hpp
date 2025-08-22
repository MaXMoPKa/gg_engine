#pragma once

#include <queue>
#include <bitset>

namespace gg
{

class Keyboard
{
    friend class Window;
public:
    class Event
    {
    public:
        enum class Type
        {
            PRESS,
            RELEASE,
            INVALID
        };
    private:
        Type type;
        unsigned char code;
    public:
        Event()
            : type(Type::INVALID)
            , code(0u)
        {}

        Event(Type type, unsigned char code) noexcept
            : type(type)
            , code(code)
        {}

        bool isPress() const noexcept
        {
            return type == Type::PRESS;
        }

        bool isRelease() const noexcept
        {
            return type == Type::RELEASE;
        }

        bool isValid() const noexcept
        {
            return type != Type::INVALID;
        }

        unsigned char getCode() const noexcept
        {
            return code;
        }
    };

public:
    Keyboard() = default;
    Keyboard(const Keyboard&) = delete;
    Keyboard& operator=(const Keyboard&) = delete;

    bool keyIsPressed(unsigned char key_code) const noexcept;
    Event readKey() noexcept;
    bool keyIsEmpty() const noexcept;
    void flushKey() noexcept;

    char readChar() noexcept;
    bool charIsEmpty() const noexcept;
    void flushChar() noexcept;
    void flush() noexcept;

    void enableAutorepeat() noexcept;
    void disableAutorepeat() noexcept;
    bool autorepeatIsEnabled() const noexcept;

private:
    void onKeyPressed(unsigned char key_code) noexcept;
    void onKeyReleased(unsigned char key_code) noexcept;
    void onChar(char character) noexcept;
    void clearState() noexcept;
    template<typename T>
    static void trimBuffer(std::queue<T>& buffer) noexcept;

private:
    static constexpr unsigned int n_keys = 256u;
    static constexpr unsigned int buffer_size = 16u;
    bool autorepeat_enabled = false;
    std::bitset<n_keys> key_states;
    std::queue<Event> key_buffer;
    std::queue<char> char_buffer;
};

}
