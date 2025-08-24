#include "src/include/mouse.hpp"

#include <Windows.h>

std::pair<int, int> gg::Mouse::getPos() const noexcept
{
    return {x, y};

}

int gg::Mouse::getPosX() const noexcept
{
    return x;
}

int gg::Mouse::getPosY() const noexcept
{
    return y;
}

bool gg::Mouse::isInWindow() const noexcept
{
    return is_in_window;
}

bool gg::Mouse::leftIsPressed() const noexcept
{
    return left_is_pressed;
}

bool gg::Mouse::rightIsPressed() const noexcept
{
    return right_is_pressed;
}

gg::Mouse::Event gg::Mouse::read() noexcept
{
    using namespace gg;

    if(buffer.size() > 0u)
    {
        Mouse::Event e = buffer.front();
        buffer.pop();
        return e;
    }
    else
    {
        return Mouse::Event();
    }
}

bool gg::Mouse::isEmpty() const noexcept
{
    return buffer.empty();
}

void gg::Mouse::flush() noexcept
{
    buffer = std::queue<Event>();
}

void gg::Mouse::onMouseMove(int x, int y) noexcept
{
    this->x = x;
    this->y = y;

    buffer.push(Mouse::Event(Mouse::Event::Type::MOVE, *this));
    trimBuffer();
}

void gg::Mouse::onMouseLeave(int x, int y) noexcept
{
    is_in_window = false;

    buffer.push(Mouse::Event(Mouse::Event::Type::LEAVE, *this));
    trimBuffer();
}

void gg::Mouse::onMouseEnter(int x, int y) noexcept
{
    is_in_window = true;

    buffer.push(Mouse::Event(Mouse::Event::Type::ENTER, *this));
    trimBuffer();
}

void gg::Mouse::onLeftPressed(int x, int y) noexcept
{
    left_is_pressed = true;

    buffer.push(Mouse::Event(Mouse::Event::Type::LEFT_BUTTON_PRESSED, *this));
    trimBuffer();
}

void gg::Mouse::onLeftReleased(int x, int y) noexcept
{
    left_is_pressed = false;

    buffer.push(Mouse::Event(Mouse::Event::Type::LEFT_BUTTON_RELEASED, *this));
    trimBuffer();
}

void gg::Mouse::onRightPressed(int x, int y) noexcept
{
    right_is_pressed = true;

    buffer.push(Mouse::Event(Mouse::Event::Type::RIGHT_BUTTON_PRESSED, *this));
    trimBuffer();
}

void gg::Mouse::onRightReleased(int x, int y) noexcept
{
    right_is_pressed = false;

    buffer.push(Mouse::Event(Mouse::Event::Type::RIGHT_BUTTON_RELEASED, *this));
    trimBuffer();
}

void gg::Mouse::onWheelUp(int x, int y) noexcept
{
    buffer.push(Mouse::Event(Mouse::Event::Type::WHEEL_UP, *this));
    trimBuffer();
}

void gg::Mouse::onWheelDown(int x, int y) noexcept
{
    buffer.push(Mouse::Event(Mouse::Event::Type::WHEEL_DOWN, *this));
    trimBuffer();
}

void gg::Mouse::onWheelDelta(int x, int y, int delta) noexcept
{
    wheel_delta_carry = delta;
    while(wheel_delta_carry >= WHEEL_DELTA)
    {
        wheel_delta_carry -= WHEEL_DELTA;
        onWheelUp(x, y);
    }

    while(wheel_delta_carry <= - WHEEL_DELTA)
    {
        wheel_delta_carry += WHEEL_DELTA;
        onWheelDown(x, y);
    }
}

void gg::Mouse::trimBuffer() noexcept
{
    while(buffer.size() > buffer_size)
    {
        buffer.pop();
    }
}
