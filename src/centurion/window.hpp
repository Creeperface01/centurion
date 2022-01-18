#ifndef CENTURION_WINDOW_HPP_
#define CENTURION_WINDOW_HPP_

#include <SDL.h>

#include <cassert>   // assert
#include <optional>  // optional
#include <ostream>   // ostream
#include <string>    // string, to_string
#include <utility>   // pair, make_pair, move

#include "common.hpp"
#include "detail/owner_handle_api.hpp"
#include "detail/stdlib.hpp"
#include "features.hpp"
#include "math.hpp"
#include "render.hpp"
#include "surface.hpp"
#include "video.hpp"

#if CENTURION_HAS_FEATURE_FORMAT

#include <format>  // format

#endif  // CENTURION_HAS_FEATURE_FORMAT

namespace cen {

template <typename T>
class BasicWindow;

using Window = BasicWindow<detail::owner_tag>;
using WindowHandle = BasicWindow<detail::handle_tag>;

template <typename T>
class BasicWindow final {
 public:
  enum WindowFlags : uint32 {
    Fullscreen = SDL_WINDOW_FULLSCREEN,
    OpenGL = SDL_WINDOW_OPENGL,
    Shown = SDL_WINDOW_SHOWN,
    Hidden = SDL_WINDOW_HIDDEN,
    Borderless = SDL_WINDOW_BORDERLESS,
    Resizable = SDL_WINDOW_RESIZABLE,
    Minimized = SDL_WINDOW_MINIMIZED,
    Maximized = SDL_WINDOW_MAXIMIZED,
    InputGrabbed = SDL_WINDOW_INPUT_GRABBED,
    InputFocus = SDL_WINDOW_INPUT_FOCUS,
    MouseFocus = SDL_WINDOW_MOUSE_FOCUS,
    FullscreenDesktop = SDL_WINDOW_FULLSCREEN_DESKTOP,
    Foreign = SDL_WINDOW_FOREIGN,
    AllowHighDPI = SDL_WINDOW_ALLOW_HIGHDPI,
    MouseCapture = SDL_WINDOW_MOUSE_CAPTURE,
    AlwaysOnTop = SDL_WINDOW_ALWAYS_ON_TOP,
    SkipTaskbar = SDL_WINDOW_SKIP_TASKBAR,
    Utility = SDL_WINDOW_UTILITY,
    Tooltip = SDL_WINDOW_TOOLTIP,
    PopupMenu = SDL_WINDOW_POPUP_MENU,
    Vulkan = SDL_WINDOW_VULKAN,

#if SDL_VERSION_ATLEAST(2, 0, 14)
    Metal = SDL_WINDOW_METAL
#endif  // SDL_VERSION_ATLEAST(2, 0, 14)
  };

  explicit BasicWindow(maybe_owner<SDL_Window*> window) noexcept(detail::is_handle<T>)
      : mWindow{window}
  {
    if constexpr (detail::is_owner<T>) {
      if (!mWindow) {
        throw exception{"Cannot create window from null pointer!"};
      }
    }
  }

  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  explicit BasicWindow(const char* title,
                       const iarea size = GetDefaultSize(),
                       const uint32 flags = GetDefaultFlags())
  {
    assert(title);

    if (size.width < 1) {
      throw exception{"Bad window width!"};
    }
    else if (size.height < 1) {
      throw exception{"Bad window height!"};
    }

    mWindow.reset(SDL_CreateWindow(title,
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   size.width,
                                   size.height,
                                   flags));
    if (!mWindow) {
      throw sdl_error{};
    }
  }

  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  explicit BasicWindow(const std::string& title,
                       const iarea size = GetDefaultSize(),
                       const uint32 flags = GetDefaultFlags())
      : BasicWindow{title.c_str(), size, flags}
  {}

  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  BasicWindow() : BasicWindow{"Centurion"}
  {}

  template <typename TT = T, detail::enable_for_handle<TT> = 0>
  explicit BasicWindow(const Window& owner) noexcept : mWindow{owner.get()}
  {}

  [[nodiscard]] auto create_renderer(const uint32 flags = renderer::default_flags())
      -> renderer
  {
    if (auto* ptr = SDL_CreateRenderer(get(), -1, flags)) {
      return renderer{ptr};
    }
    else {
      throw sdl_error{};
    }
  }

  void Show() noexcept { SDL_ShowWindow(mWindow); }

  void Hide() noexcept { SDL_HideWindow(mWindow); }

  void Raise() noexcept { SDL_RaiseWindow(mWindow); }

  void Maximize() noexcept { SDL_MaximizeWindow(mWindow); }

  void Minimize() noexcept { SDL_MinimizeWindow(mWindow); }

  void Restore() noexcept { SDL_RestoreWindow(mWindow); }

  void Center() noexcept { SetPosition({SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED}); }

  auto UpdateSurface() noexcept -> result { return SDL_UpdateWindowSurface(mWindow) == 0; }

#if SDL_VERSION_ATLEAST(2, 0, 16)

  auto Flash(const flash_op op = flash_op::briefly) noexcept -> result
  {
    return SDL_FlashWindow(mWindow, static_cast<SDL_FlashOperation>(op)) == 0;
  }

#endif  // SDL_VERSION_ATLEAST(2, 0, 16)

  auto SetFullscreen(const bool enabled) noexcept -> result
  {
    return SDL_SetWindowFullscreen(mWindow, enabled ? Fullscreen : 0) == 0;
  }

  auto SetFullscreenDesktop(const bool enabled) noexcept -> result
  {
    return SDL_SetWindowFullscreen(mWindow, enabled ? FullscreenDesktop : 0) == 0;
  }

  void SetDecorated(const bool decorated) noexcept
  {
    SDL_SetWindowBordered(mWindow, decorated ? SDL_TRUE : SDL_FALSE);
  }

  void SetResizable(const bool resizable) noexcept
  {
    SDL_SetWindowResizable(mWindow, resizable ? SDL_TRUE : SDL_FALSE);
  }

  void SetIcon(const surface& icon) noexcept { SDL_SetWindowIcon(mWindow, icon.get()); }

  void SetTitle(const char* title) noexcept
  {
    assert(title);
    SDL_SetWindowTitle(mWindow, title);
  }

  void SetTitle(const std::string& title) noexcept { SetTitle(title.c_str()); }

  auto SetOpacity(const float opacity) noexcept -> result
  {
    return SDL_SetWindowOpacity(mWindow, opacity) == 0;
  }

  void SetGrabMouse(const bool grab) noexcept
  {
    SDL_SetWindowGrab(mWindow, grab ? SDL_TRUE : SDL_FALSE);
  }

  auto SetBrightness(const float brightness) noexcept -> result
  {
    return SDL_SetWindowBrightness(mWindow, detail::clamp(brightness, 0.0f, 1.0f)) == 0;
  }

#if SDL_VERSION_ATLEAST(2, 0, 16)

  void SetGrabKeyboard(const bool grab) noexcept
  {
    SDL_SetWindowKeyboardGrab(mWindow, grab ? SDL_TRUE : SDL_FALSE);
  }

  void SetAlwaysOnTop(const bool enabled) noexcept
  {
    SDL_SetWindowAlwaysOnTop(mWindow, enabled ? SDL_TRUE : SDL_FALSE);
  }

#endif  // SDL_VERSION_ATLEAST(2, 0, 16)

  void SetX(const int x) noexcept { SetPosition({x, y()}); }

  void SetY(const int y) noexcept { SetPosition({x(), y}); }

  void SetPosition(const ipoint position) noexcept
  {
    SDL_SetWindowPosition(mWindow, position.x(), position.y());
  }

  void SetWidth(const int width) noexcept
  {
    SDL_SetWindowSize(mWindow, detail::max(width, 1), height());
  }

  void SetHeight(const int height) noexcept
  {
    SDL_SetWindowSize(mWindow, width(), detail::max(height, 1));
  }

  void SetSize(const iarea size) noexcept
  {
    assert(size.width > 0);
    assert(size.height > 0);
    SDL_SetWindowSize(mWindow, size.width, size.height);
  }

  void SetMinSize(const iarea size) noexcept
  {
    assert(size.width > 0);
    assert(size.height > 0);
    SDL_SetWindowMinimumSize(mWindow, size.width, size.height);
  }

  void SetMaxSize(const iarea size) noexcept
  {
    assert(size.width > 0);
    assert(size.height > 0);
    SDL_SetWindowMaximumSize(mWindow, size.width, size.height);
  }

  static auto SetCapturingMouse(const bool capture) noexcept -> result
  {
    return SDL_CaptureMouse(capture ? SDL_TRUE : SDL_FALSE) == 0;
  }

  [[nodiscard]] auto GetID() const noexcept -> uint32 { return SDL_GetWindowID(mWindow); }

  [[nodiscard]] auto position() const noexcept -> ipoint
  {
    int x{};
    int y{};
    SDL_GetWindowPosition(mWindow, &x, &y);
    return {x, y};
  }

  [[nodiscard]] auto x() const noexcept -> int { return position().x(); }

  [[nodiscard]] auto y() const noexcept -> int { return position().y(); }

  [[nodiscard]] auto size() const noexcept -> iarea
  {
    iarea size{};
    SDL_GetWindowSize(mWindow, &size.width, &size.height);
    return size;
  }

  [[nodiscard]] auto width() const noexcept -> int { return size().width; }

  [[nodiscard]] auto height() const noexcept -> int { return size().height; }

  [[nodiscard]] auto GetMinSize() const noexcept -> iarea
  {
    iarea size{};
    SDL_GetWindowMinimumSize(mWindow, &size.width, &size.height);
    return size;
  }

  [[nodiscard]] auto GetMaxSize() const noexcept -> iarea
  {
    iarea size{};
    SDL_GetWindowMaximumSize(mWindow, &size.width, &size.height);
    return size;
  }

  [[nodiscard]] auto GetDisplayIndex() const noexcept -> std::optional<int>
  {
    const auto index = SDL_GetWindowDisplayIndex(mWindow);
    if (index != -1) {
      return index;
    }
    else {
      return std::nullopt;
    }
  }

  [[nodiscard]] auto GetTitle() const -> std::string { return SDL_GetWindowTitle(mWindow); }

  [[nodiscard]] auto GetBrightness() const noexcept -> float
  {
    return SDL_GetWindowBrightness(mWindow);
  }

  [[nodiscard]] auto GetOpacity() const noexcept -> float
  {
    float opacity{1};
    SDL_GetWindowOpacity(mWindow, &opacity);
    return opacity;
  }

  [[nodiscard]] auto GetPixelFormat() const noexcept -> pixel_format
  {
    return static_cast<pixel_format>(SDL_GetWindowPixelFormat(mWindow));
  }

  [[nodiscard]] auto GetSurface() noexcept -> surface_handle
  {
    return surface_handle{SDL_GetWindowSurface(mWindow)};
  }

  [[nodiscard]] auto GetFlags() const noexcept -> uint32
  {
    return SDL_GetWindowFlags(mWindow);
  }

  [[nodiscard]] auto CheckFlag(const uint32 flag) const noexcept -> bool
  {
    return GetFlags() & flag;
  }

  [[nodiscard]] auto HasGrabbedInput() const noexcept -> bool
  {
    return CheckFlag(InputGrabbed);
  }

  [[nodiscard]] auto HasInputFocus() const noexcept -> bool { return CheckFlag(InputFocus); }

  [[nodiscard]] auto HasMouseFocus() const noexcept -> bool { return CheckFlag(MouseFocus); }

  [[nodiscard]] auto IsBorderless() const noexcept -> bool { return CheckFlag(Borderless); }

  [[nodiscard]] auto IsDecorated() const noexcept -> bool { return !IsBorderless(); }

  [[nodiscard]] auto IsResizable() const noexcept -> bool { return CheckFlag(Resizable); }

  [[nodiscard]] auto IsHighDPI() const noexcept -> bool { return CheckFlag(AllowHighDPI); }

  [[nodiscard]] auto IsFullscreen() const noexcept -> bool { return CheckFlag(Fullscreen); }

  [[nodiscard]] auto IsFullscreenDesktop() const noexcept -> bool
  {
    return CheckFlag(FullscreenDesktop);
  }

  [[nodiscard]] auto IsVisible() const noexcept -> bool { return CheckFlag(Shown); }

  [[nodiscard]] auto IsHidden() const noexcept -> bool { return CheckFlag(Hidden); }

  [[nodiscard]] auto IsOpenGL() const noexcept -> bool { return CheckFlag(OpenGL); }

  [[nodiscard]] auto IsVulkan() const noexcept -> bool { return CheckFlag(Vulkan); }

#if SDL_VERSION_ATLEAST(2, 0, 14)

  [[nodiscard]] auto IsMetal() const noexcept -> bool { return CheckFlag(Metal); }

#endif  // SDL_VERSION_ATLEAST(2, 0, 14)

  [[nodiscard]] auto IsForeign() const noexcept -> bool { return CheckFlag(Foreign); }

  [[nodiscard]] auto IsCapturingMouse() const noexcept -> bool
  {
    return CheckFlag(MouseCapture);
  }

  [[nodiscard]] auto IsMinimized() const noexcept -> bool { return CheckFlag(Minimized); }

  [[nodiscard]] auto IsMaximized() const noexcept -> bool { return CheckFlag(Maximized); }

  [[nodiscard]] auto IsAlwaysOnTop() const noexcept -> bool { return CheckFlag(AlwaysOnTop); }

  [[nodiscard]] auto IsUtility() const noexcept -> bool { return CheckFlag(Utility); }

  [[nodiscard]] auto IsTooltip() const noexcept -> bool { return CheckFlag(Tooltip); }

  [[nodiscard]] auto IsPopupMenu() const noexcept -> bool { return CheckFlag(PopupMenu); }

  [[nodiscard]] auto IsExcludedFromTaskbar() const noexcept -> bool
  {
    return CheckFlag(SkipTaskbar);
  }

  [[nodiscard]] auto IsGrabbingMouse() const noexcept -> bool
  {
    return SDL_GetWindowGrab(mWindow);
  }

  [[nodiscard]] auto IsScreenKeyboardShown() const noexcept -> bool
  {
    return SDL_IsScreenKeyboardShown(get()) == SDL_TRUE;
  }

  [[nodiscard]] auto data() const noexcept -> SDL_Window* { return mWindow.get(); }

  [[nodiscard]] auto get() const noexcept -> SDL_Window* { return mWindow.get(); }

  template <typename TT = T, detail::enable_for_handle<TT> = 0>
  explicit operator bool() const noexcept
  {
    return mWindow != nullptr;
  }

  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  [[nodiscard]] constexpr static auto GetDefaultSize() noexcept -> iarea
  {
    return {800, 600};
  }

  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  [[nodiscard]] constexpr static auto GetDefaultFlags() noexcept -> uint32
  {
    return Hidden;
  }

 private:
  detail::pointer<T, SDL_Window> mWindow;
};

template <typename T>
[[nodiscard]] auto ToString(const BasicWindow<T>& window) -> std::string
{
#if CENTURION_HAS_FEATURE_FORMAT
  return std::format("Window(data: {}, width: {}, height: {})",
                     detail::address_of(window.data()),
                     window.width(),
                     window.height());
#else
  return "Window(data: " + detail::address_of(window.data()) +
         ", width: " + std::to_string(window.width()) +
         ", height: " + std::to_string(window.height()) + ")";
#endif  // CENTURION_HAS_FEATURE_FORMAT
}

template <typename T>
auto operator<<(std::ostream& stream, const BasicWindow<T>& window) -> std::ostream&
{
  return stream << ToString(window);
}

[[nodiscard]] inline auto GetGrabbedWindow() noexcept -> WindowHandle
{
  return WindowHandle{SDL_GetGrabbedWindow()};
}

[[nodiscard]] inline auto GetMouseFocusWindow() noexcept -> WindowHandle
{
  return WindowHandle{SDL_GetMouseFocus()};
}

[[nodiscard]] inline auto GetKeyboardFocusWindow() noexcept -> WindowHandle
{
  return WindowHandle{SDL_GetKeyboardFocus()};
}

[[nodiscard]] inline auto GetWindow(const uint32 id) noexcept -> WindowHandle
{
  return WindowHandle{SDL_GetWindowFromID(id)};
}

template <typename T>
[[nodiscard]] auto GetRenderer(const BasicWindow<T>& window) noexcept -> renderer_handle
{
  return renderer_handle{SDL_GetRenderer(window.get())};
}

[[nodiscard]] inline auto MakeWindowAndRenderer(const iarea size = Window::GetDefaultSize(),
                                                const uint32 flags = Window::GetDefaultFlags())
    -> std::pair<Window, renderer>
{
  cen::Window window{"Centurion window", size, flags};
  return std::make_pair(std::move(window), window.create_renderer());
}

}  // namespace cen

#endif  // CENTURION_WINDOW_HPP_
