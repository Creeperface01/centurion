/*
 * MIT License
 *
 * Copyright (c) 2019-2022 Albin Johansson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef CENTURION_LOCALE_HPP_
#define CENTURION_LOCALE_HPP_

#include <SDL.h>

#include <cassert>  // assert
#include <cstddef>  // size_t
#include <memory>   // unique_ptr

#include "detail/sdl_deleter.hpp"
#include "detail/stdlib.hpp"

namespace cen {

/**
 * \ingroup system
 * \defgroup locale Locale
 *
 * \brief Provides locale information.
 */

/// \addtogroup locale
/// \{

#if SDL_VERSION_ATLEAST(2, 0, 14)

/**
 * \brief Represents a set of locale entries.
 *
 * \see `SDL_Locale`
 */
class locale final
{
 public:
  using size_type = std::size_t;

  /**
   * \brief Returns the current preferred locales on the system.
   *
   * \note The preferred locale might change during the execution of the program.
   *
   * \return the preferred locales on the system.
   */
  [[nodiscard]] static auto get_preferred() noexcept -> locale
  {
    return locale{SDL_GetPreferredLocales()};
  }

  /**
   * \brief Indicates whether a language (and optionally a country) is part of the locale.
   *
   * \param language the language that will be checked, e.g. "en" for english.
   * \param country optional country code that will be checked, e.g. "US" or "GB".
   *
   * \return `true` if the language and country is a part of the locale; `false` otherwise.
   */
  [[nodiscard]] auto has_language(const char* language,
                                  const char* country = nullptr) const noexcept -> bool
  {
    assert(language);

    if (const auto* array = mLocales.get()) {
      for (auto index = 0u; array[index].language; ++index) {
        const auto& item = array[index];

        if (country && item.country) {
          if (detail::cmp(language, item.language) && detail::cmp(country, item.country)) {
            return true;
          }
        }
        else {
          if (detail::cmp(language, item.language)) {
            return true;
          }
        }
      }
    }

    return false;
  }

  /**
   * \brief Returns the amount of entries in the locale.
   *
   * \return the locale entry count.
   */
  [[nodiscard]] auto size() const noexcept -> size_type
  {
    size_type result{0};

    if (const auto* array = mLocales.get()) {
      for (auto index = 0u; array[index].language; ++index) {
        ++result;
      }
    }

    return result;
  }

  /**
   * \brief Indicates whether the locale contains a non-null pointer.
   *
   * \return `true` if the internal pointer is non-null; `false` otherwise.
   */
  explicit operator bool() const noexcept { return mLocales != nullptr; }

 private:
  std::unique_ptr<SDL_Locale, detail::sdl_deleter> mLocales;

  explicit locale(SDL_Locale* locales) noexcept : mLocales{locales} {}
};

#endif  // SDL_VERSION_ATLEAST(2, 0, 14)

/// \} End of group locale

}  // namespace cen

#endif  // CENTURION_LOCALE_HPP_
