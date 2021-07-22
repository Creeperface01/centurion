#include "video/message_box_type.hpp"

#include <gtest/gtest.h>

#include <iostream>  // cout

#include "core/to_underlying.hpp"

TEST(MessageBoxType, Values)
{
  ASSERT_EQ(SDL_MESSAGEBOX_INFORMATION,
            cen::to_underlying(cen::message_box_type::information));
  ASSERT_EQ(SDL_MESSAGEBOX_WARNING, cen::to_underlying(cen::message_box_type::warning));
  ASSERT_EQ(SDL_MESSAGEBOX_ERROR, cen::to_underlying(cen::message_box_type::error));
}

TEST(MessageBoxType, ToString)
{
  ASSERT_EQ("information", cen::to_string(cen::message_box_type::information));
  ASSERT_EQ("error", cen::to_string(cen::message_box_type::error));
  ASSERT_EQ("warning", cen::to_string(cen::message_box_type::warning));

  std::cout << "Message box type example: " << cen::message_box_type::information << '\n';
}
