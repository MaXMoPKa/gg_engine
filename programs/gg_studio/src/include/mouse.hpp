#pragma once

#include <queue>

namespace gg
{

class Mouse
{
    friend class Window;

public:
    class Event
    {
    public:
        enum class Type
        {
            LEFT_BUTTON_PRESSED,
            LEFT_BUTTON_RELEASED,
            RIGHT_BUTTON_PRESSED,
            RIGHT_BUTTON_RELEASED,
            WHEEL_UP,
            WHEEL_DOWN,
            MOVE,
            ENTER,
            LEAVE,
            INVALID
        };
    public:
        Event() noexcept
            : type(Type::INVALID)
            , left_is_pressed(false)
            , right_is_pressed(false)
            , x(0)
            , y(0)
        {}

        Event(Type type, const Mouse& parent) noexcept
            : type(type)
            , left_is_pressed(parent.left_is_pressed)
            , right_is_pressed(parent.right_is_pressed)
            , x(parent.x)
            , y(parent.y)
        {}

        bool isValid() const noexcept
        {
            return type != Type::INVALID;
        }

        Type getType() const noexcept
        {
            return type;
        }

        std::pair<int, int> getPos() const noexcept
        {
            return {x, y};
        }

        int getPosX() const noexcept
        {
            return x;
        }

        int getPosY() const noexcept
        {
            return y;
        }

        bool leftIsPressed() const noexcept
        {
            return left_is_pressed;
        }

        bool rightIsPressed() const noexcept
        {
            return right_is_pressed;
        }
    private:
        Type type;
        bool left_is_pressed;
        bool right_is_pressed;
        int x;
        int y;
    };
public:
    Mouse() = default;
    Mouse(const Mouse&) = delete;
    Mouse& operator=(const Mouse&) = delete;

    std::pair<int, int> getPos() const noexcept;
    int getPosX() const noexcept;
    int getPosY() const noexcept;
    bool isInWindow() const noexcept;
    bool leftIsPressed() const noexcept;
    bool rightIsPressed() const noexcept;
    Mouse::Event read() noexcept;
    bool isEmpty() const noexcept;
    void flush() noexcept;

private:
    void onMouseMove(int x, int y) noexcept;
    void onMouseLeave(int x, int y) noexcept;
    void onMouseEnter(int x, int y) noexcept;
    void onLeftPressed(int x, int y) noexcept;
    void onLeftReleased(int x, int y) noexcept;
    void onRightPressed(int x, int y) noexcept;
    void onRightReleased(int x, int y) noexcept;
    void onWheelUp(int x, int y) noexcept;
    void onWheelDown(int x, int y) noexcept;
    void onWheelDelta(int x, int y, int delta) noexcept;
    void trimBuffer() noexcept;
private:
    static constexpr unsigned int buffer_size = 16u;
    int x;
    int y;
    bool is_in_window = false;
    bool left_is_pressed = false;
    bool right_is_pressed = false;
    int wheel_delta_carry = 0;
    std::queue<Event> buffer;

};
    
}
