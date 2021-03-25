#ifndef CENTURION_FILE_HEADER
#define CENTURION_FILE_HEADER

#include <SDL.h>

#include <cassert>   // assert
#include <cstddef>   // size_t
#include <memory>    // unique_ptr
#include <optional>  // optional
#include <string>    // string

#include "../centurion_cfg.hpp"
#include "../misc/czstring.hpp"
#include "../misc/integers.hpp"
#include "../misc/not_null.hpp"

#ifdef CENTURION_USE_PRAGMA_ONCE
#pragma once
#endif  // CENTURION_USE_PRAGMA_ONCE

namespace cen {

/// \addtogroup system
/// \{

/**
 * \enum file_mode
 *
 * \brief Provides values that represent different file modes.
 *
 * \details This enum provides values that directly correspond to each of the
 * possible SDL file mode strings, such as "r" or "rb".
 *
 * \since 5.3.0
 *
 * \headerfile file.hpp
 */
enum class file_mode
{
  read_existing,         ///< "r"
  read_existing_binary,  ///< "rb"

  write,         ///< "w"
  write_binary,  ///< "wb"

  append_or_create,         ///< "a"
  append_or_create_binary,  ///< "ab"

  read_write_existing,         ///< "r+"
  read_write_existing_binary,  ///< "rb+"

  read_write_replace,         ///< "w+"
  read_write_replace_binary,  ///< "wb+"

  read_append,        ///< "a+"
  read_append_binary  ///< "ab+"
};

/**
 * \enum seek_mode
 *
 * \brief Provides values that represent various file seek modes.
 *
 * \since 5.3.0
 *
 * \headerfile file.hpp
 */
enum class seek_mode
{
  from_beginning = RW_SEEK_SET,       ///< From the beginning.
  relative_to_current = RW_SEEK_CUR,  ///< Relative to the current read point.
  relative_to_end = RW_SEEK_END       ///< Relative to the end.
};

/**
 * \enum file_type
 *
 * \brief Provides values that represent different file types.
 *
 * \since 5.3.0
 *
 * \headerfile file_type.hpp
 */
enum class file_type : unsigned
{
  unknown = SDL_RWOPS_UNKNOWN,     ///< An unknown file type.
  win32 = SDL_RWOPS_WINFILE,       ///< A Win32 file.
  stdio = SDL_RWOPS_STDFILE,       ///< A STDIO file.
  jni = SDL_RWOPS_JNIFILE,         ///< An Android asset file.
  memory = SDL_RWOPS_MEMORY,       ///< A memory stream file.
  memory_ro = SDL_RWOPS_MEMORY_RO  ///< A read-only memory stream file.
};

/**
 * \class file
 *
 * \brief Represents a file "context" or handle.
 *
 * \note This class differs slightly from other library classes in that it is
 * owning, but it does *not* throw if the internal pointer can't be created,
 * etc. This is because file operations are error-prone, so we want to avoid
 * throwing a bunch of exceptions, for performance reasons.
 *
 * \since 5.3.0
 *
 * \headerfile file.hpp
 */
class file final
{
 public:
  using size_type = std::size_t;

  /// \name Construction
  /// \{

  /**
   * \brief Creates a file handle based on an existing context.
   *
   * \param context the context that will be used.
   *
   * \since 5.3.0
   */
  explicit file(SDL_RWops* context) noexcept : m_context{context}
  {}

  /**
   * \brief Opens the file at the specified file path.
   *
   * \details Be sure to check the validity of the file, after construction.
     \verbatim
       cen::file file{"foo", cen::file_mode::read_existing_binary};
       if (file) {
         // File was opened successfully!
       }
     \endverbatim
   *
   * \param path the path of the file.
   * \param mode the mode that will be used to open the file.
   *
   * \since 5.3.0
   */
  file(const not_null<czstring> path, const file_mode mode) noexcept
      : m_context{SDL_RWFromFile(path, to_string(mode))}
  {}

  /**
   * \copydoc file(not_null<czstring>, file_mode)
   */
  file(const std::string& path, const file_mode mode) noexcept
      : file{path.c_str(), mode}
  {}

  /// \} End of construction

  /// \name Write API
  /// \{

  /**
   * \brief Writes the supplied data to the file.
   *
   * \tparam T the type of the data.
   *
   * \param data a pointer to the data that will be written to the file.
   * \param count the number of objects that will be written.
   *
   * \return the number of objects that were written to the file.
   *
   * \since 5.3.0
   */
  template <typename T>
  auto write(not_null<const T*> data, const size_type count) noexcept
      -> size_type
  {
    assert(m_context);
    return SDL_RWwrite(get(), data, sizeof(T), count);
  }

  /**
   * \brief Writes the contents of an array to the file, whose size is known at
   * compile-time.
   *
   * \tparam T the type of the array elements.
   * \tparam size the size of the array.
   *
   * \param data the data that will be written.
   *
   * \return the number of objects that were written.
   *
   * \since 5.3.0
   */
  template <typename T, size_type size>
  auto write(const T (&data)[size]) noexcept -> size_type
  {
    return write(data, size);
  }

  // clang-format off

  /**
   * \brief Writes the contents of a container to the file.
   *
   * \pre `Container` *must* be a collection that stores its data
   * contiguously! The behaviour of this function is undefined otherwise.
   *
   * \tparam Container a contiguous container, e.g. `std::vector` or `std::array`.
   *
   * \param container the container that will be written to the file.
   *
   * \return the number of objects that were written.
   *
   * \since 5.3.0
   */
  template <typename Container>
  auto write(const Container& container) noexcept(noexcept(container.data()) &&
                                                  noexcept(container.size()))
      -> size_type
  {
    return write(container.data(), container.size());
  }

  // clang-format on

  /**
   * \brief Writes an unsigned 8-bit integer to the file.
   *
   * \pre the internal file context must not be null.
   *
   * \param value the value that will be written.
   *
   * \return `true` if the value was written to the file; `false` otherwise.
   *
   * \since 5.3.0
   */
  auto write_byte(const u8 value) noexcept -> bool
  {
    assert(m_context);
    return SDL_WriteU8(m_context.get(), value) == 1;
  }

  /**
   * \brief Writes an unsigned 16-bit integer to the file, as a little endian
   * value.
   *
   * \pre the internal file context must not be null.
   *
   * \param value the value that will be written, in the native endianness.
   *
   * \return `true` if the value was written to the file; `false` otherwise.
   *
   * \since 5.3.0
   */
  auto write_as_little_endian(const u16 value) noexcept -> bool
  {
    assert(m_context);
    return SDL_WriteLE16(m_context.get(), value) == 1;
  }

  /**
   * \brief Writes an unsigned 32-bit integer to the file, as a little endian
   * value.
   *
   * \pre the internal file context must not be null.
   *
   * \param value the value that will be written, in the native endianness.
   *
   * \return `true` if the value was written to the file; `false` otherwise.
   *
   * \since 5.3.0
   */
  auto write_as_little_endian(const u32 value) noexcept -> bool
  {
    assert(m_context);
    return SDL_WriteLE32(m_context.get(), value) == 1;
  }

  /**
   * \brief Writes an unsigned 64-bit integer to the file, as a little endian
   * value.
   *
   * \pre the internal file context must not be null.
   *
   * \param value the value that will be written, in the native endianness.
   *
   * \return `true` if the value was written to the file; `false` otherwise.
   *
   * \since 5.3.0
   */
  auto write_as_little_endian(const u64 value) noexcept -> bool
  {
    assert(m_context);
    return SDL_WriteLE64(m_context.get(), value) == 1;
  }

  /**
   * \brief Writes an unsigned 16-bit integer to the file, as a big endian
   * value.
   *
   * \pre the internal file context must not be null.
   *
   * \param value the value that will be written, in the native endianness.
   *
   * \return `true` if the value was written to the file; `false` otherwise.
   *
   * \since 5.3.0
   */
  auto write_as_big_endian(const u16 value) noexcept -> bool
  {
    assert(m_context);
    return SDL_WriteBE16(m_context.get(), value) == 1;
  }

  /**
   * \brief Writes an unsigned 32-bit integer to the file, as a big endian
   * value.
   *
   * \pre the internal file context must not be null.
   *
   * \param value the value that will be written, in the native endianness.
   *
   * \return `true` if the value was written to the file; `false` otherwise.
   *
   * \since 5.3.0
   */
  auto write_as_big_endian(const u32 value) noexcept -> bool
  {
    assert(m_context);
    return SDL_WriteBE32(m_context.get(), value) == 1;
  }

  /**
   * \brief Writes an unsigned 64-bit integer to the file, as a big endian
   * value.
   *
   * \pre the internal file context must not be null.
   *
   * \param value the value that will be written, in the native endianness.
   *
   * \return `true` if the value was written to the file; `false` otherwise.
   *
   * \since 5.3.0
   */
  auto write_as_big_endian(const u64 value) noexcept -> bool
  {
    assert(m_context);
    return SDL_WriteBE64(m_context.get(), value) == 1;
  }

  /// \} End of write API

  /// \name Read API
  /// \{

  /**
   * \brief Reads data from the file.
   *
   * \pre the internal file context must not be null.
   *
   * \tparam T the type of the data that will be read.
   *
   * \param[out] data the pointer to which the read data will be written to.
   * \param maxCount the maximum number of objects that will be read.
   *
   * \return the number of objects that were read.
   *
   * \since 5.3.0
   */
  template <typename T>
  auto read_to(T* data, const size_type maxCount) noexcept -> size_type
  {
    assert(m_context);
    return SDL_RWread(m_context.get(), data, sizeof(T), maxCount);
  }

  /**
   * \brief Reads data from the file to an array whose size is known at
   * compile-time. This function uses the size of the supplied array to
   * determine the amount of elements to read.
   *
   * \pre the internal file context must not be null.
   *
   * \tparam T the type of the data that will be read.
   * \tparam size the size of the array.
   *
   * \param[out] data the pointer to which the read data will be written to.
   *
   * \return the number of objects that were read.
   *
   * \since 5.3.0
   */
  template <typename T, size_type size>
  auto read_to(T (&data)[size]) noexcept -> size_type
  {
    return read_to(data, size);
  }

  // clang-format off

  /**
   * \brief Reads data from the file to a container. This function uses the size
   * of the supplied container to determine the amount of elements to read.
   *
   * \pre the internal file context must not be null.
   * \pre `Container` *must* be a collection that stores its data
   * contiguously! The behaviour of this function is undefined otherwise.
   *
   * \tparam Container the type of the data that will be read, e.g.
   * `std::vector` or `std::array`.
   *
   * \param[out] container the container to which the read data will be written to.
   *
   * \return the number of objects that were read.
   *
   * \since 5.3.0
   */
  template <typename Container>
  auto read_to(Container& container) noexcept(noexcept(container.data()) &&
                                              noexcept(container.size()))
      -> size_type
  {
    return read_to(container.data(), container.size());
  }

  // clang-format on

  // Reads a value of type T, where T must be default-constructible

  /**
   * \brief Reads a single value from the file.
   *
   * \pre the internal file context must not be null.
   *
   * \note `T` must be default-constructible in order to use this function.
   *
   * \tparam T the type of the value, which must be default-constructible.
   *
   * \return the read value.
   *
   * \since 5.3.0
   */
  template <typename T>
  auto read() noexcept(noexcept(T{})) -> T
  {
    T value{};
    read_to(&value, 1);
    return value;
  }

  /**
   * \brief Reads an unsigned 8-bit integer from the file.
   *
   * \pre the internal file context must not be null.
   *
   * \return the read value.
   *
   * \since 5.3.0
   */
  auto read_byte() noexcept -> u8
  {
    assert(m_context);
    return SDL_ReadU8(m_context.get());
  }

  /**
   * \brief Reads an unsigned 16-bit integer from the file, as a little endian
   * value.
   *
   * \pre the internal file context must not be null.
   *
   * \return the read value.
   *
   * \since 5.3.0
   */
  auto read_little_endian_u16() noexcept -> u16
  {
    assert(m_context);
    return SDL_ReadLE16(m_context.get());
  }

  /**
   * \brief Reads an unsigned 32-bit integer from the file, as a little endian
   * value.
   *
   * \pre the internal file context must not be null.
   *
   * \return the read value.
   *
   * \since 5.3.0
   */
  auto read_little_endian_u32() noexcept -> u32
  {
    assert(m_context);
    return SDL_ReadLE32(m_context.get());
  }

  /**
   * \brief Reads an unsigned 64-bit integer from the file, as a little endian
   * value.
   *
   * \pre the internal file context must not be null.
   *
   * \return the read value.
   *
   * \since 5.3.0
   */
  auto read_little_endian_u64() noexcept -> u64
  {
    assert(m_context);
    return SDL_ReadLE64(m_context.get());
  }

  /**
   * \brief Reads an unsigned 16-bit integer from the file, as a big endian
   * value.
   *
   * \pre the internal file context must not be null.
   *
   * \return the read value.
   *
   * \since 5.3.0
   */
  auto read_big_endian_u16() noexcept -> u16
  {
    assert(m_context);
    return SDL_ReadBE16(m_context.get());
  }

  /**
   * \brief Reads an unsigned 32-bit integer from the file, as a big endian
   * value.
   *
   * \pre the internal file context must not be null.
   *
   * \return the read value.
   *
   * \since 5.3.0
   */
  auto read_big_endian_u32() noexcept -> u32
  {
    assert(m_context);
    return SDL_ReadBE32(m_context.get());
  }

  /**
   * \brief Reads an unsigned 64-bit integer from the file, as a big endian
   * value.
   *
   * \pre the internal file context must not be null.
   *
   * \return the read value.
   *
   * \since 5.3.0
   */
  auto read_big_endian_u64() noexcept -> u64
  {
    assert(m_context);
    return SDL_ReadBE64(m_context.get());
  }

  /// \} End of read API

  /**
   * \brief Seeks to the specified offset, using the specified seek mode.
   *
   * \param offset the offset to seek to.
   * \param mode the seek mode that will be used.
   *
   * \return the resulting offset in the data stream; `std::nullopt` if
   * something went wrong.
   *
   * \since 5.3.0
   */
  [[nodiscard]] auto seek(const i64 offset, const seek_mode mode) noexcept
      -> std::optional<i64>
  {
    assert(m_context);
    const auto result =
        SDL_RWseek(m_context.get(), offset, static_cast<int>(mode));
    if (result != -1)
    {
      return result;
    }
    else
    {
      return std::nullopt;
    }
  }

  /**
   * \brief Returns the current offset in the data stream.
   *
   * \return the current offset in the data stream.
   *
   * \since 5.3.0
   */
  [[nodiscard]] auto offset() const noexcept -> i64
  {
    assert(m_context);
    return SDL_RWtell(m_context.get());
  }

  /**
   * \brief Returns the file type associated with the instance.
   *
   * \return the associated file type.
   *
   * \since 5.3.0
   */
  [[nodiscard]] auto type() const noexcept -> file_type
  {
    assert(m_context);
    return static_cast<file_type>(m_context->type);
  }

  /**
   * \brief Returns the size of the file.
   *
   * \return the size of the file; `std::nullopt` if unknown.
   *
   * \since 5.3.0
   */
  [[nodiscard]] auto size() const noexcept -> std::optional<size_type>
  {
    assert(m_context);
    const auto result = SDL_RWsize(m_context.get());
    if (result != -1)
    {
      return result;
    }
    else
    {
      return std::nullopt;
    }
  }

  /**
   * \brief Returns a pointer to the internal file context.
   *
   * \return a pointer to the internal file context, can be null.
   *
   * \since 5.3.0
   */
  [[nodiscard]] auto get() const noexcept -> SDL_RWops*
  {
    return m_context.get();
  }

  /**
   * \brief Indicates whether or not the file holds a non-null pointer.
   *
   * \return `true` if the file holds a non-null pointer; `false` otherwise.
   *
   * \since 5.3.0
   */
  explicit operator bool() const noexcept
  {
    return m_context != nullptr;
  }

 private:
  struct deleter final
  {
    void operator()(SDL_RWops* context) noexcept
    {
      SDL_RWclose(context);
    }
  };
  std::unique_ptr<SDL_RWops, deleter> m_context;

  [[nodiscard]] static auto to_string(const file_mode mode) noexcept -> czstring
  {
    switch (mode)
    {
      default:
        assert(false);

      case file_mode::read_existing:
        return "r";

      case file_mode::read_existing_binary:
        return "rb";

      case file_mode::write:
        return "w";

      case file_mode::write_binary:
        return "wb";

      case file_mode::append_or_create:
        return "a";

      case file_mode::append_or_create_binary:
        return "ab";

      case file_mode::read_write_existing:
        return "r+";

      case file_mode::read_write_existing_binary:
        return "rb+";

      case file_mode::read_write_replace:
        return "w+";

      case file_mode::read_write_replace_binary:
        return "wb+";

      case file_mode::read_append:
        return "a+";

      case file_mode::read_append_binary:
        return "ab+";
    }
  }
};

/// \} End of system group

}  // namespace cen

#endif  // CENTURION_FILE_HEADER