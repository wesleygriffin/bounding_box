#include <utility> // move

namespace bb {

class Point {
  float x_;
  float y_;

public:
  constexpr float x() const noexcept { return x_; }
  constexpr float y() const noexcept { return y_; }

  constexpr Point(float x, float y) noexcept
    : x_(x)
    , y_(y) {}

  // Rule of zero
}; // class Point

class Box {
  Point topLeft_;
  Point bottomRight_;

public:
  constexpr Point topLeft() const noexcept { return topLeft_; }
  constexpr Point bottomRight() const noexcept { return bottomRight_; }

  constexpr float width() const noexcept { return topLeft_.x() + bottomRight_.x(); }
  constexpr float height() const noexcept { return topLeft_.y() + bottomRight_.y(); }

  constexpr Point topRight() const noexcept { return {width(), topLeft_.y()}; }
  constexpr Point bottomLeft() const noexcept { return {topLeft_.x(), height()}; }

  constexpr Box(Point topLeft, Point bottomRight) noexcept
    : topLeft_(std::move(topLeft))
    , bottomRight_(std::move(bottomRight)) {}

  // Rule of zero
}; // class Box

/*
    +-----------+       +-----------+
    | b         |       | a         |
    |     +-----+----+  |     +-----+----+
    |     | a   |    |  |     | b   |    |
    +-----+-----+    |  +-----+-----+    |
          |          |        |          |
          +----------+        +----------+

          +----------+        +----------+
          | b        |        | a        |
    +-----+-----+    |  +-----+-----+    |
    | a   |     |    |  | b   |     |    |
    |     +-----+----+  |     +-----+----+
    |           |       |           |
    +-----------+       +-----------+
*/
inline constexpr bool intersects(Box const& a, Box const& b) noexcept {
  bool const aTopLeftInside =
    ((a.topLeft().x() > b.topLeft().x()) && (a.topLeft().x() < b.topRight().x())) &&
    ((a.topLeft().y() > b.bottomRight().y()) && (a.topLeft().y() < b.topLeft().y()));
  bool const bTopLeftInside =
    ((b.topLeft().x() > a.topLeft().x()) && (b.topLeft().x() < a.topRight().x())) &&
    ((b.topLeft().y() > a.bottomRight().y()) && (b.topLeft().y() < a.topLeft().y()));
  bool const aTopRightInside =
    ((a.topRight().x() > b.topLeft().x()) && (a.topRight().x() < b.topRight().x())) &&
    ((a.topRight().y() > b.bottomRight().y()) && (a.topRight().y() < b.topLeft().y()));
  bool const bTopRightInside =
    ((b.topRight().x() > a.topLeft().x()) && (b.topRight().x() < a.topRight().x())) &&
    ((b.topRight().y() > a.bottomRight().y()) && (b.topRight().y() < a.topRight().y()));

  return aTopLeftInside || bTopLeftInside || aTopRightInside || bTopRightInside;
} // intersects(Box, Box)

} // namespace bb

//////
//
// ostream operators for debugging in unit tests
//
/////

#include <ostream>

inline std::ostream& operator<<(std::ostream& os, bb::Point const& p) {
  return os << "Point(" << p.x() << ", " << p.y() << ")";
}

inline std::ostream& operator<<(std::ostream& os, bb::Box const& b) {
  return os << "Box(" << b.topLeft() << ", " << b.bottomRight() << ")";
}

/////
//
// unit tests
//
/////

#define CATCH_CONFIG_IMPL
#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

using namespace Catch::literals;
using namespace bb;

TEST_CASE("Point construct") {
  Point const p(1.f, 2.f);
  CHECK(p.x() == 1_a);
  CHECK(p.y() == 2_a);
}

TEST_CASE("Box construct") {
  Box const b({1.f, 2.f}, {3.f, 4.f});
  CHECK(b.topLeft().x() == 1_a);
  CHECK(b.topLeft().y() == 2_a);
  CHECK(b.bottomRight().x() == 3_a);
  CHECK(b.bottomRight().y() == 4_a);
  CHECK(b.width() == 4_a);
  CHECK(b.height() == 6_a);
}

TEST_CASE("Box intersects 0") {
  Box const b1({1.f, 1.f}, {2.f, 2.f});
  Box const b2({3.f, 3.f}, {4.f, 4.f});
  CHECK(!intersects(b1, b2));
}

TEST_CASE("Box intersects 1") {
  Box const b1({2.f, 3.f}, {4.f, 1.f});
  Box const b2({1.f, 4.f}, {3.f, 2.f});
  CHECK(intersects(b1, b2));
}

TEST_CASE("Box intersects 2") {
  Box const b1({2.f, 3.f}, {4.f, 1.f});
  Box const b2({1.f, 4.f}, {3.f, 2.f});
  CHECK(intersects(b2, b1));
}

TEST_CASE("Box intersects 3") {
  Box const b1({2.f, 3.f}, {4.f, 1.f});
  Box const b2({3.f, 4.f}, {5.f, 2.f});
  CHECK(intersects(b1, b2));
}

TEST_CASE("Box intersects 4") {
  Box const b1({2.f, 3.f}, {4.f, 1.f});
  Box const b2({3.f, 4.f}, {5.f, 2.f});
  CHECK(intersects(b2, b1));
}
