#include <world/vec2.hpp>
#include <world/world.hpp>

#include <gtest/gtest.h>

#include <iostream>

TEST(Vec2, BasicConstruction)
{
  Vec2 v(12, 23);
  ASSERT_EQ(v.x, 12);
  ASSERT_EQ(v.y, 23);
}

TEST(Vec2, Comparison)
{
  Vec2 v(-12, 300);
  Vec2 w(-12, 300);
  ASSERT_EQ(v, w);

  Vec2 y(18, 0);
  ASSERT_NE(y, v);
}

TEST(Vec2, Addition)
{
  Vec2 v(0, 0);
  Vec2 c(18, -7823);
  Vec2 res = v + c;

  ASSERT_EQ(res, c);
  res += res;
  ASSERT_EQ(res.x, 36);
  ASSERT_EQ(res.y, -15646);
}

TEST(Vec2, Substraction)
{
  Vec2 v(1, 2);
  Vec2 c(14, 15);
  Vec2 res = v - c;

  ASSERT_EQ(res.x, -13);
  ASSERT_EQ(res.y, -13);
  res -= res;
}

TEST(Vec2, Multiplication)
{
  Vec2 v(12, -12.5);
  v *= 3;
  ASSERT_EQ(v.x, 36);
  ASSERT_EQ(v.y, -37.5);

  auto w = 10 * v;
  ASSERT_EQ(w.x, 360);
  ASSERT_EQ(w.y, -375);
}

TEST(Vec2, Division)
{
  Vec2 v(888, -3333);
  ASSERT_EQ(v/2, v*0.5);
}

TEST(Vec2, Distance)
{
  Vec2 v(0, 6);
  Vec2 w(8, 0);
  ASSERT_EQ(Vec2::distance(v, w), 10);

  Vec2 z(1038, -200.8);
  ASSERT_EQ(Vec2::distance(z, z), 0);

  Vec2 zero(0, 0);
  ASSERT_EQ(Vec2::distance(z, zero), z.length());
}

TEST(Vec2, Length)
{
  Vec2 v(8, 6);
  ASSERT_EQ(v.length(), 10);

  v.set_length(20);
  ASSERT_EQ(v.length(), 20);
  ASSERT_EQ(v.x, 16);
  ASSERT_EQ(v.y, 12);
}

TEST(World, CellAtPosition)
{
  World world;
  auto step = 2_fix;

  auto check_line_contained_in_single_cell = [&world, &step](const Position& start,
                                                      const Position& goal)
    {
      Cell expected = world.get_cell_at_position(start);
      Vec2 mvmt = goal - start;
      mvmt.set_length(step);


      log_debug("Going from " << start  << " to " << goal <<
                " with a movement of " << mvmt <<
                ". Should be in cell " << expected);

      for (auto pos = start; Position::distance(goal, pos) > mvmt.length(); pos += mvmt)
        EXPECT_EQ(expected, world.get_cell_at_position(pos)) <<
          " failed at position " << pos;
    };

  EXPECT_EQ(Cell(8, 4), world.get_cell_at_position({800, 250}));
  EXPECT_EQ(Cell(8, 4), world.get_cell_at_position({801, 250}));
  EXPECT_EQ(Cell(7, 5), world.get_cell_at_position({800, 251}));

  EXPECT_EQ(Cell(8, 5), world.get_cell_at_position({850, 300}));
  EXPECT_EQ(Cell(8, 5), world.get_cell_at_position({851, 300}));
  EXPECT_EQ(Cell(8, 6), world.get_cell_at_position({850, 301}));

  check_line_contained_in_single_cell({500, 550}, {550, 501});

  check_line_contained_in_single_cell({550, 501}, {500, 550});

  check_line_contained_in_single_cell({400, 550}, {499, 550});
  check_line_contained_in_single_cell({499, 550}, {400, 550});

  check_line_contained_in_single_cell({700, 450}, {799, 450});
  check_line_contained_in_single_cell({799, 450}, {700, 450});

  check_line_contained_in_single_cell({800, 450}, {899, 450});
  check_line_contained_in_single_cell({899, 450}, {800, 450});


  check_line_contained_in_single_cell({850, 400}, {949, 400});
  check_line_contained_in_single_cell({949, 400}, {850, 400});

  check_line_contained_in_single_cell({499, 548}, {549, 499});

  ASSERT_EQ(InvalidCell, world.get_cell_at_position({0, 99}));
  ASSERT_EQ(InvalidCell, world.get_cell_at_position({0, 100}));
  ASSERT_EQ(InvalidCell, world.get_cell_at_position({0, 101}));
}

TEST(World, Astar)
{
  World world;
  Unit* unit = world.do_new_unit(0, {500, 500});
  Position goal{650, 350};
  auto cell_path = world.get_map().do_astar(world.get_cell_at_position(unit->pos),
                                            world.get_cell_at_position(goal));
  CellPath expected{606, 706, 806, 905, 1005, 904};
  ASSERT_EQ(cell_path, expected);
}

TEST(World, LineOfSight)
{
  World world;
  ASSERT_EQ(world.can_walk_in_straight_line({500, 500}, {450, 450}, 2, 10), true);
  ASSERT_EQ(world.can_walk_in_straight_line({500, 500}, {450, 550}, 2, 10), true);
  ASSERT_EQ(world.can_walk_in_straight_line({500, 500}, {500, 600}, 2, 10), true);
  ASSERT_EQ(world.can_walk_in_straight_line({500, 500}, {550, 550}, 2, 10), true);
  ASSERT_EQ(world.can_walk_in_straight_line({500, 500}, {550, 450}, 2, 10), false);

  ASSERT_EQ(world.can_walk_in_straight_line({550, 505}, {650, 405}, 2, 10), true);
}

TEST(World, CanWalkInStraightLine)
{
  World world;
  ASSERT_EQ(true, world.can_walk_in_straight_line({500, 475}, {550, 505}, 1, 10));
}

TEST(World, NextPathPosition)
{
  World world;
  Unit* unit = world.do_new_unit(0, {500, 475});
  Position goal{650, 350};
  auto cell_path = world.get_map().do_astar(world.get_cell_at_position(unit->pos),
                                            world.get_cell_at_position(goal));
  Position pos = world.get_next_path_position(cell_path, unit->pos, goal,
                                              unit->width);
  ASSERT_EQ(pos, Position(550, 505));

  pos = world.get_next_path_position(cell_path, pos, goal,
                                     unit->width);
}

TEST(World, PositionHeight)
{
  World world;
  Fix16 height;

  ASSERT_EQ(0.5, world.get_position_height({1000, 400}));
  ASSERT_EQ(1, world.get_position_height({1050, 400}));
  ASSERT_EQ(0, world.get_position_height({1051, 400}));
}

TEST(World, SmoothingPath)
{
  World world;
  Unit* unit = world.do_new_unit(0, {500, 475});
  Position goal{652, 353};

  Path path = world.calculate_path(goal, unit);

  for (const Position& pos: path)
    std::cout << pos << std::endl;

  Path expected{{550, 505}, {655, 400}, goal};
  ASSERT_EQ(expected, path);


  unit = world.do_new_unit(0, {525, 400});
  goal = {600, 500};
  path = world.calculate_path(goal, unit);

  expected = {{495, 450}, {550, 505}, goal};
  ASSERT_EQ(expected, path);

  unit = world.do_new_unit(0, {1300, 900});
  goal = {1200, 1000};
  expected = {{1250, 895}, {1195, 950}, goal};
  path = world.calculate_path(goal, unit);
  ASSERT_EQ(expected, path);
}
