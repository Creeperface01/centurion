#ifndef CENTURION_TOUCH_FINGER_EVENT_SOURCE
#define CENTURION_TOUCH_FINGER_EVENT_SOURCE

#include "touch_finger_event.h"

#include "centurion_utils.h"

namespace centurion {
namespace event {

CENTURION_DEF
TouchFingerEvent::TouchFingerEvent() noexcept : CommonEvent{}
{}

CENTURION_DEF
TouchFingerEvent::TouchFingerEvent(const SDL_TouchFingerEvent& event) noexcept
    : CommonEvent{event}
{}

CENTURION_DEF
TouchFingerEvent::TouchFingerEvent(SDL_TouchFingerEvent&& event) noexcept
    : CommonEvent{std::move(event)}
{}

CENTURION_DEF
void TouchFingerEvent::set_touch_id(TouchID id) noexcept
{
  m_event.touchId = id;
}

CENTURION_DEF
void TouchFingerEvent::set_finger_id(FingerID id) noexcept
{
  m_event.fingerId = id;
}

CENTURION_DEF
void TouchFingerEvent::set_window_id(Uint32 id) noexcept
{
  m_event.windowID = id;
}

CENTURION_DEF
void TouchFingerEvent::set_x(float x) noexcept
{
  m_event.x = clamp_inclusive({0, 1}, x);
}

CENTURION_DEF
void TouchFingerEvent::set_y(float y) noexcept
{
  m_event.y = clamp_inclusive({0, 1}, y);
}

CENTURION_DEF
void TouchFingerEvent::set_dx(float dx) noexcept
{
  m_event.dx = clamp_inclusive({-1, 1}, dx);
}

CENTURION_DEF
void TouchFingerEvent::set_dy(float dy) noexcept
{
  m_event.dy = clamp_inclusive({-1, 1}, dy);
}

CENTURION_DEF
void TouchFingerEvent::set_pressure(float pressure) noexcept
{
  m_event.pressure = clamp_inclusive({0, 1}, pressure);
}

CENTURION_DEF
TouchID TouchFingerEvent::touch_id() const noexcept
{
  return m_event.touchId;
}

CENTURION_DEF
FingerID TouchFingerEvent::finger_id() const noexcept
{
  return m_event.fingerId;
}

CENTURION_DEF
Uint32 TouchFingerEvent::window_id() const noexcept
{
  return m_event.windowID;
}

CENTURION_DEF
float TouchFingerEvent::x() const noexcept
{
  return m_event.x;
}

CENTURION_DEF
float TouchFingerEvent::y() const noexcept
{
  return m_event.y;
}

CENTURION_DEF
float TouchFingerEvent::dx() const noexcept
{
  return m_event.dx;
}

CENTURION_DEF
float TouchFingerEvent::dy() const noexcept
{
  return m_event.dy;
}

CENTURION_DEF
float TouchFingerEvent::pressure() const noexcept
{
  return m_event.pressure;
}

}  // namespace event
}  // namespace centurion

#endif  // CENTURION_TOUCH_FINGER_EVENT_SOURCE