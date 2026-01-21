module;

export module input.key;

import types.base_types;

namespace gg
{
    export enum class Key : U16
    {
        Unknown = 0,

        Ctrl,
        Alt,
        Shift,
        Space,

        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,

        D0,
        D1,
        D2,
        D3,
        D4,
        D5,
        D6,
        D7,
        D8,
        D9,

        Escape,
        Return,
        Tab,
        Backspace,
        Insert,
        Delete,
        Home,
        End,
        PageUp,
        PageDown,

        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,

        Up,
        Down,
        Left,
        Right,

        Comma,       // ,
        Period,      // .
        Slash,       // /
        Semicolon,   // ;
        Quote,       // '
        Backquote,   // `
        Backslash,
        LeftBracket, // [
        RightBracket,// ]
        Minus,       // -
        Equals,      // =
        Numpad0,
        Numpad1,
        Numpad2,
        Numpad3,
        Numpad4,
        Numpad5,
        Numpad6,
        Numpad7,
        Numpad8,
        Numpad9,
        NumpadEnter,
        NumpadAdd,
        NumpadSubtract,
        NumpadMultiply,
        NumpadDivide,
        
    }; // enum class Key;
} // namespace gg;
