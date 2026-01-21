module;

#include <gtest/gtest.h>

export module tests.window_manager;

import core.window_manager;
import core.window_handle;

namespace gg
{
    TEST(WindowManagerTest, CreateReturnsValidHandle)
    {
        WindowHandle handle = WindowManager::create({800, 600, "Test"});
        EXPECT_TRUE(handle);
    }
} // namespace gg;
