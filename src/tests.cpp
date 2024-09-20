#include <gtest/gtest.h>
#include "RB_tree.h"
#include <vector>
TEST(RB_Tree, SmallRotateTree)
{
    RBTree a;
    a.Insert(1);
    a.Insert(2);
    a.Insert(3);
    a.Insert(4);
    a.Insert(5);
    a.Insert(6);
    std::vector<InfoTree> result = a.GetTreeLikePreorderArray();
    std::vector<InfoTree> expected{{Color::kBlack, 2},
                                   {Color::kBlack, 1},
                                   {Color::kRed, 4},
                                   {Color::kBlack, 3},
                                   {Color::kBlack, 5},
                                   {Color::kRed, 6}};
    ASSERT_EQ(result, expected);
}
TEST(RB_Tree, RotateRightLeftRightRight)
{
    RBTree a;
    a.Insert(3);
    a.Insert(12);
    a.Insert(16);
    a.Insert(1);
    a.Insert(5);
    a.Insert(-1);
    a.Insert(20);
    a.Insert(25);
    a.Insert(6);
    a.Insert(7);
    a.Insert(8);
    a.Insert(23);
    a.Insert(4);
    std::vector<InfoTree> expected{{Color::kBlack, 6},
                                   {Color::kBlack, 3},
                                   {Color::kBlack, 1},
                                   {Color::kRed, -1},
                                   {Color::kBlack, 5},
                                   {Color::kRed, 4},
                                   {Color::kBlack, 12},
                                   {Color::kBlack, 7},
                                   {Color::kRed, 8},
                                   {Color::kRed, 20},
                                   {Color::kBlack, 16},
                                   {Color::kBlack, 25},
                                   {Color::kRed, 23}};
    std::vector<InfoTree> result = a.GetTreeLikePreorderArray();
    ASSERT_EQ(expected, result);
}
TEST(RB_Tree, RotateLeftRight)
{
    RBTree a;
    a.Insert(23);
    a.Insert(30);
    a.Insert(11);
    a.Insert(7);
    a.Insert(15);
    a.Insert(20);
    a.Insert(4);
    a.Insert(24);
    a.Insert(34);
    std::vector<InfoTree> expected{{Color::kBlack, 23},
                                   {Color::kRed, 11},
                                   {Color::kBlack, 7},
                                   {Color::kRed, 4},
                                   {Color::kBlack, 15},
                                   {Color::kRed, 20},
                                   {Color::kBlack, 30},
                                   {Color::kRed, 24},
                                   {Color::kRed, 34}};
    std::vector<InfoTree> result = a.GetTreeLikePreorderArray();
    ASSERT_EQ(expected, result);
}
TEST(RB_Tree, RotateRightLeft)
{
    RBTree a;
    a.Insert(12);
    a.Insert(3);
    a.Insert(8);
    a.Insert(32);
    a.Insert(11);
    a.Insert(6);
    a.Insert(7);
    a.Insert(45);
    a.Insert(1);
    a.Insert(9);
    std::vector<InfoTree> expected{{Color::kBlack, 8},
                                   {Color::kRed, 6},
                                   {Color::kBlack, 3},
                                   {Color::kRed, 1},
                                   {Color::kBlack, 7},
                                   {Color::kRed, 12},
                                   {Color::kBlack, 11},
                                   {Color::kRed, 9},
                                   {Color::kBlack, 32},
                                   {Color::kRed, 45}};
    std::vector<InfoTree> result = a.GetTreeLikePreorderArray();
    ASSERT_EQ(expected, result);
}
TEST(RB_Tree, EraseWithTwoChilds)
{
    RBTree a;
    a.Insert(4);
    a.Insert(6);
    a.Insert(5);
    a.Insert(54);
    a.Insert(67);
    a.Insert(7);
    a.Erase(54);
    std::vector<InfoTree> expected{{Color::kBlack, 5},
                                   {Color::kBlack, 4},
                                   {Color::kRed, 7},
                                   {Color::kBlack, 6},
                                   {Color::kBlack, 67}};
    std::vector<InfoTree> result = a.GetTreeLikePreorderArray();
    ASSERT_EQ(expected, result);
}
TEST(RB_Tree, MostBiggestTree)
{
    RBTree a;
    a.Insert(12);
    a.Insert(1);
    a.Insert(16);
    a.Insert(5);
    a.Insert(2);
    a.Insert(32);
    a.Insert(20);
    a.Insert(25);
    a.Insert(6);
    a.Insert(7);
    a.Insert(8);
    a.Insert(23);
    a.Insert(4);
    a.Insert(30);
    a.Insert(11);
    a.Insert(29);
    a.Insert(15);
    a.Insert(3);
    a.Insert(24);
    a.Insert(34);
    a.Erase(29);
    std::vector<InfoTree> expected{{Color::kBlack, 12},
                                   {Color::kBlack, 2},
                                   {Color::kBlack, 1},
                                   {Color::kRed, 6},
                                   {Color::kBlack, 4},
                                   {Color::kRed, 3},
                                   {Color::kRed, 5},
                                   {Color::kBlack, 8},
                                   {Color::kRed, 7},
                                   {Color::kRed, 11},
                                   {Color::kBlack, 25},
                                   {Color::kRed, 20},
                                   {Color::kBlack, 16},
                                   {Color::kRed, 15},
                                   {Color::kBlack, 23},
                                   {Color::kRed, 24},
                                   {Color::kRed, 32},
                                   {Color::kBlack, 30},
                                   {Color::kBlack, 34}};
    std::vector<InfoTree> result = a.GetTreeLikePreorderArray();
    ASSERT_EQ(expected, result);
}
