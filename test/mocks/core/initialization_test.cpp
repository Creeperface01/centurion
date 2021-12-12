#include "core/initialization.hpp"

#include <gtest/gtest.h>

#include "core_mocks.hpp"

class InitializationTest : public testing::Test
{
 protected:
  void SetUp() override
  {
    mocks::reset_core();

    /* Sets up expected return values for OK initialization */
    SDL_Init_fake.return_val = cen::SDLConfig{}.flags;
    IMG_Init_fake.return_val = cen::IMGConfig{}.flags;
    Mix_Init_fake.return_val = cen::MixConfig{}.flags;
    TTF_Init_fake.return_val = 0;

    Mix_OpenAudio_fake.return_val = 0;
  }
};

TEST_F(InitializationTest, CoreDefaultConfiguration)
{
  try {
    const cen::SDLLibrary sdl;
    ASSERT_EQ(1u, SDL_Init_fake.call_count);

    constexpr cen::SDLConfig cfg;
    ASSERT_EQ(cfg.flags, SDL_Init_fake.arg0_val);
  }
  catch (...) {
    FAIL();
  }
}

TEST_F(InitializationTest, ImgDefaultConfiguration)
{
  try {
    const cen::IMGLibrary lib;
    ASSERT_EQ(1u, IMG_Init_fake.call_count);

    constexpr cen::IMGConfig cfg;
    ASSERT_EQ(cfg.flags, IMG_Init_fake.arg0_val);
  }
  catch (...) {
    FAIL();
  }
}

TEST_F(InitializationTest, MixDefaultConfiguration)
{
  try {
    const cen::MixLibrary lib;
    ASSERT_EQ(1u, Mix_Init_fake.call_count);

    constexpr cen::MixConfig cfg;
    ASSERT_EQ(cfg.frequency, Mix_OpenAudio_fake.arg0_val);
    ASSERT_EQ(cfg.format, Mix_OpenAudio_fake.arg1_val);
    ASSERT_EQ(cfg.channels, Mix_OpenAudio_fake.arg2_val);
    ASSERT_EQ(cfg.chunk_size, Mix_OpenAudio_fake.arg3_val);
  }
  catch (...) {
    FAIL();
  }
}

TEST_F(InitializationTest, TTFDefaultConfiguration)
{
  try {
    const cen::TTFLibrary lib;
    ASSERT_EQ(1u, TTF_Init_fake.call_count);
  }
  catch (...) {
    FAIL();
  }
}

TEST_F(InitializationTest, SDLCoreInitFailure)
{
  SDL_Init_fake.return_val = -1;
  ASSERT_THROW(cen::SDLLibrary{}, cen::SDLError);
}

TEST_F(InitializationTest, SDLTTFInitFailure)
{
  TTF_Init_fake.return_val = -1;
  ASSERT_THROW(cen::TTFLibrary{}, cen::TTFError);
}

TEST_F(InitializationTest, SDLImageInitFailure)
{
  IMG_Init_fake.return_val = 0;
  ASSERT_THROW(cen::IMGLibrary{}, cen::IMGError);
}

TEST_F(InitializationTest, SDLMixInitFailure)
{
  Mix_Init_fake.return_val = 0;
  ASSERT_THROW(cen::MixLibrary{}, cen::MixError);
}

TEST_F(InitializationTest, SDLMixOpenFailure)
{
  Mix_OpenAudio_fake.return_val = -1;
  ASSERT_THROW(cen::MixLibrary{}, cen::MixError);
}