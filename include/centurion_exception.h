/*
 * MIT License
 *
 * Copyright (c) 2019-2020 Albin Johansson
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

#ifndef CENTURION_EXCEPTION_HEADER
#define CENTURION_EXCEPTION_HEADER

#include "centurion_api.h"
#include <stdexcept>
#include <string>
#include "centurion_api.h"

namespace centurion {

/**
 * A general exception used by the Centurion library.
 *
 * @since 3.0.0
 */
class CENTURION_API CenturionException final : public std::exception {
 private:
  std::string msg = "";

 public:
  /**
   * @since 3.0.0
   */
  CenturionException() = default;

  /**
   * @param msg the message of the exception. If the pointer is null, the string "N/A" is used.
   * @since 3.0.0
   */
  CENTURION_API explicit CenturionException(const char* msg);

  /**
   * @param msg the message of the exception.
   * @since 3.0.0
   */
  CENTURION_API explicit CenturionException(const std::string& msg);

  [[nodiscard]]
  CENTURION_API const char* what() const noexcept override;
};

}

#endif // CENTURION_EXCEPTION_HEADER