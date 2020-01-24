#include "catch.hpp"
#include "font.h"
#include "centurion_exception.h"
#include <iostream>

using namespace centurion;

static constexpr auto type_writer_path = "resources/type_writer.ttf";
static constexpr auto fira_code_path = "resources/fira_code.ttf";
static constexpr auto daniel_path = "resources/daniel.ttf";

TEST_CASE("Font(string&, int)", "[Font]") {
  CHECK_THROWS_AS(Font("", 1), CenturionException);
  CHECK_THROWS_AS(Font("", 0), std::invalid_argument);

  Font font{type_writer_path, 12};
  std::cout << font << "\n";
}

TEST_CASE("Font::reset", "[Font]") {
  Font font(type_writer_path, 12);

  font.set_bold(true);
  font.set_italic(true);
  font.set_underlined(true);
  font.set_strikethrough(true);

  font.reset();
  CHECK(!font.is_bold());
  CHECK(!font.is_italic());
  CHECK(!font.is_underlined());
  CHECK(!font.is_strikethrough());
}

TEST_CASE("Font::set_bold", "[Font]") {
  Font font(type_writer_path, 12);

  CHECK(!font.is_bold());

  font.set_bold(true);
  CHECK(font.is_bold());

  font.set_bold(false);
  CHECK(!font.is_bold());
}

TEST_CASE("Font::set_italic", "[Font]") {
  Font font(type_writer_path, 12);

  CHECK(!font.is_italic());

  font.set_italic(true);
  CHECK(font.is_italic());

  font.set_italic(false);
  CHECK(!font.is_italic());
}

TEST_CASE("Font::set_underlined", "[Font]") {
  Font font(type_writer_path, 12);

  CHECK(!font.is_underlined());

  font.set_underlined(true);
  CHECK(font.is_underlined());

  font.set_underlined(false);
  CHECK(!font.is_underlined());
}

TEST_CASE("Font::set_strikethrough", "[Font]") {
  Font font(type_writer_path, 12);

  CHECK(!font.is_strikethrough());

  font.set_strikethrough(true);
  CHECK(font.is_strikethrough());

  font.set_strikethrough(false);
  CHECK(!font.is_strikethrough());
}

TEST_CASE("Font::set_outlined", "[Font]") {
  Font font(type_writer_path, 12);

  CHECK(!font.is_outlined());

  font.set_outlined(true);
  CHECK(font.is_outlined());

  font.set_outlined(false);
  CHECK(!font.is_outlined());
}

TEST_CASE("Font::get_size", "[Font]") {
  const auto size = 12;
  Font font(type_writer_path, size);

  CHECK(size == font.get_size());
}

TEST_CASE("Font::is_fixed_width", "[Font]") {
  Font fira_code(fira_code_path, 12); // Fixed width
  Font daniel(daniel_path, 12);      // Not fixed width

  CHECK(fira_code.is_fixed_width());
  CHECK(!daniel.is_fixed_width());
}

TEST_CASE("Font::get_family_name", "[Font]") {
  Font font(type_writer_path, 12);
  CHECK(font.get_family_name() == "Type Writer");
}
