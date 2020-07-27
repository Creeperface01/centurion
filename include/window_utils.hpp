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
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file window_utils.hpp
 *
 * @brief Provides utilities related to windows.
 *
 * @author Albin Johansson
 *
 * @date 2019-2020
 *
 * @copyright MIT License
 */

#ifndef CENTURION_WINDOW_UTILS_HEADER
#define CENTURION_WINDOW_UTILS_HEADER

#include "centurion_api.hpp"
#include "centurion_fwd.hpp"
#include "renderer_handle.hpp"

namespace centurion {

/**
 * @brief Returns a handle to the renderer associated with a window.
 *
 * @ingroup graphics
 *
 * @details The returned handle will be null if the supplied window doesn't
 * have an associated renderer.
 *
 * @note You should always check whether or not the returned handle contains
 * a valid renderer pointer before using it!
 *
 * @param window the window associated with the desired renderer.
 *
 * @return a handle to the associated renderer, might not contain a valid
 * renderer pointer.
 *
 * @since 5.0.0
 */
CENTURION_QUERY
auto get_renderer(const window& window) noexcept -> renderer_handle;

/**
 * @copydoc get_renderer
 */
CENTURION_QUERY
auto get_renderer(const window_handle& window) noexcept -> renderer_handle;

}  // namespace centurion

#endif  // CENTURION_WINDOW_UTILS_HEADER