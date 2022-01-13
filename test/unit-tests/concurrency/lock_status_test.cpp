#include <gtest/gtest.h>

#include <iostream>  // clog

#include "concurrency.hpp"
#include "common.hpp"

TEST(LockStatus, Values)
{
  ASSERT_EQ(0, cen::ToUnderlying(cen::LockStatus::Success));
  ASSERT_EQ(SDL_MUTEX_TIMEDOUT, cen::ToUnderlying(cen::LockStatus::TimedOut));
  ASSERT_EQ(-1, cen::ToUnderlying(cen::LockStatus::Error));
}

TEST(LockStatus, ToString)
{
  ASSERT_THROW(cen::ToString(static_cast<cen::LockStatus>(2)), cen::Error);

  ASSERT_EQ("Success", cen::ToString(cen::LockStatus::Success));
  ASSERT_EQ("TimedOut", cen::ToString(cen::LockStatus::TimedOut));
  ASSERT_EQ("Error", cen::ToString(cen::LockStatus::Error));

  std::clog << "Lock status example: " << cen::LockStatus::TimedOut << '\n';
}
