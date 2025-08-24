#include "src/include/keyboard.hpp"

bool gg::Keyboard::keyIsPressed(unsigned char key_code) const noexcept
{
    return key_states[key_code];
}

gg::Keyboard::Event gg::Keyboard::readKey() noexcept
{
    using namespace gg;

    if(key_buffer.size() > 0u)
    {
        Keyboard::Event e = key_buffer.front();
        key_buffer.pop();
        return e;
    }
    else
    {
        return Keyboard::Event();
    }
}

bool gg::Keyboard::keyIsEmpty() const noexcept
{
    return key_buffer.empty();
}

void gg::Keyboard::flushKey() noexcept
{
    key_buffer = std::queue<Event>();
}

char gg::Keyboard::readChar() noexcept
{
    if(char_buffer.size() > 0u)
    {
        unsigned char char_code = char_buffer.front();
        char_buffer.pop();
        return char_code;
    }
    else
    {
        return 0;
    }
}

bool gg::Keyboard::charIsEmpty() const noexcept
{
    return char_buffer.empty();
}

void gg::Keyboard::flushChar() noexcept
{
    char_buffer = std::queue<char>();
}

void gg::Keyboard::flush() noexcept
{
    flushKey();
    flushChar();
}

void gg::Keyboard::enableAutorepeat() noexcept
{
    autorepeat_enabled = true;
}

void gg::Keyboard::disableAutorepeat() noexcept
{
    autorepeat_enabled = false;
}

bool gg::Keyboard::autorepeatIsEnabled() const noexcept
{
    return autorepeat_enabled;
}

void gg::Keyboard::onKeyPressed(unsigned char key_code) noexcept
{
    using namespace gg;

    key_states[key_code] = true;
    key_buffer.push(Keyboard::Event(Keyboard::Event::Type::PRESS, key_code));
    trimBuffer(key_buffer);
}

void gg::Keyboard::onKeyReleased(unsigned char key_code) noexcept
{
    using namespace gg;

    key_states[key_code] = false;
    key_buffer.push(Keyboard::Event(Keyboard::Event::Type::RELEASE, key_code));
    trimBuffer(key_buffer);
}

void gg::Keyboard::onChar(char character) noexcept
{
    char_buffer.push(character);
    trimBuffer(char_buffer);
}

void gg::Keyboard::clearState() noexcept
{
    key_states.reset();
}

template <typename T>
void gg::Keyboard::trimBuffer(std::queue<T>& buffer) noexcept
{
    while(buffer.size() > buffer_size)
    {
        buffer.pop();
    }
}
