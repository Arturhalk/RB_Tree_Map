#include <gtest/gtest.h>
#include "RB_tree.demo.h"
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
TEST(RB_Tree, EraseRedParenrAndBlackChildLeftLeft)
{
    RBTree a;
    a.Insert(16);
    a.Insert(19);
    a.Insert(30);
    a.Insert(15);
    a.Insert(14);
    a.Insert(13);
    a.Erase(16);
    std::vector<InfoTree> expected{{Color::kBlack, 19},
                                   {Color::kRed, 14},
                                   {Color::kBlack, 13},
                                   {Color::kBlack, 15},
                                   {Color::kBlack, 30}};
    std::vector<InfoTree> result = a.GetTreeLikePreorderArray();
    ASSERT_EQ(expected, result);
}
TEST(RB_Tree, EraseRedParentAndBlackChildRightRight)
{
    RBTree a;
    a.Insert(23);
    a.Insert(13);
    a.Insert(32);
    a.Insert(60);
    a.Insert(42);
    a.Insert(62);
    a.Erase(32);
    std::vector<InfoTree> expected{{Color::kBlack, 23},
                                   {Color::kBlack, 13},
                                   {Color::kRed, 60},
                                   {Color::kBlack, 42},
                                   {Color::kBlack, 62}};
    std::vector<InfoTree> result = a.GetTreeLikePreorderArray();
    ASSERT_EQ(expected, result);
}

TEST(RB_Test, RotateEraseBlackUncleAndRedChildLeft)
{
    RBTree a;
    a.Insert(3);
    a.Insert(1);
    a.Insert(5);
    a.Insert(0);
    a.Erase(5);
    std::vector<InfoTree> expected{{Color::kBlack, 1},
                                   {Color::kBlack, 0},
                                   {Color::kBlack, 3}};
    std::vector<InfoTree> result = a.GetTreeLikePreorderArray();
    ASSERT_EQ(expected, result);
}
TEST(RB_Test, RotateEraseBlackUncleAndRedChildRight)
{
    RBTree a;
    a.Insert(3);
    a.Insert(5);
    a.Insert(0);
    a.Insert(6);
    a.Erase(0);
    std::vector<InfoTree> expected{{Color::kBlack, 5},
                                   {Color::kBlack, 3},
                                   {Color::kBlack, 6}};
    std::vector<InfoTree> result = a.GetTreeLikePreorderArray();
    ASSERT_EQ(expected, result);
}
TEST(RB_Test, RotateEraseBlackUncleBlackChildRedChildLeft)
{
    RBTree a;
    a.Insert(10);
    a.Insert(11);
    a.Insert(2);
    a.Insert(34);
    a.Insert(56);
    a.Insert(4);
    a.Insert(7);
    a.Insert(5);
    a.Insert(23);
    a.Erase(56);
    std::vector<InfoTree> expected{{Color::kBlack, 10},
                                   {Color::kRed, 4},
                                   {Color::kBlack, 2},
                                   {Color::kBlack, 7},
                                   {Color::kRed, 5},
                                   {Color::kRed, 23},
                                   {Color::kBlack, 11},
                                   {Color::kBlack, 34}};
    std::vector<InfoTree> result = a.GetTreeLikePreorderArray();
    ASSERT_EQ(expected, result);
}
TEST(RB_Test, RotateEraseBlackUncleBlackChildRedChildRight)
{
    RBTree a;
    a.Insert(34);
    a.Insert(1);
    a.Insert(35);
    a.Insert(44);
    a.Insert(47);
    a.Insert(46);
    a.Erase(35);
    std::vector<InfoTree> expected{{Color::kBlack, 34},
                                   {Color::kBlack, 1},
                                   {Color::kRed, 46},
                                   {Color::kBlack, 44},
                                   {Color::kBlack, 47}};
    std::vector<InfoTree> result = a.GetTreeLikePreorderArray();
    ASSERT_EQ(expected, result);
}
TEST(RB_Test, RotateEraseRedUncleBlackChildsLeft)
{
    RBTree a;
    a.Insert(23);
    a.Insert(24);
    a.Insert(12);
    a.Insert(11);
    a.Insert(13);
    a.Insert(14);
    a.Erase(14);
    a.Erase(24);
    std::vector<InfoTree> expected{{Color::kBlack, 12},
                                   {Color::kBlack, 11},
                                   {Color::kBlack, 23},
                                   {Color::kRed, 13}};
    std::vector<InfoTree> result = a.GetTreeLikePreorderArray();
    ASSERT_EQ(expected, result);
}
TEST(RB_Test, RotateEraseRedUncleBlackChildsRight)
{
    RBTree a;
    a.Insert(13);
    a.Insert(1);
    a.Insert(23);
    a.Insert(24);
    a.Insert(25);
    a.Insert(26);
    a.Erase(26);
    a.Erase(1);
    std::vector<InfoTree> expected{{Color::kBlack, 24},
                                   {Color::kBlack, 13},
                                   {Color::kRed, 23},
                                   {Color::kBlack, 25}};
    std::vector<InfoTree> result = a.GetTreeLikePreorderArray();
    ASSERT_EQ(expected, result);
}
TEST(RB_Test, RotateEraseRedUncleRedGrandGrandSonLeft)
{
    RBTree a;
    a.Insert(13);
    a.Insert(1);
    a.Insert(23);
    a.Insert(24);
    a.Insert(25);
    a.Insert(26);
    a.Erase(26);
    a.Erase(1);
    std::vector<InfoTree> expected{{Color::kBlack, 24},
                                   {Color::kBlack, 13},
                                   {Color::kRed, 23},
                                   {Color::kBlack, 25}};
    std::vector<InfoTree> result = a.GetTreeLikePreorderArray();
    ASSERT_EQ(expected, result);
}
TEST(RB_Test, RandomBigTree)
{
    RBTree a;
    a.Insert(23);
    a.Insert(24);
    a.Insert(1);
    a.Insert(8);
    a.Insert(45);
    a.Insert(33);
    a.Insert(18);
    a.Insert(13);
    a.Insert(7);
    a.Insert(12);
    a.Insert(34);
    a.Insert(56);
    a.Insert(11);
    a.Erase(33);
    a.Erase(12);
    a.Erase(23);
    a.Erase(8);
    std::vector<InfoTree> expected{{Color::kBlack, 18},
                                   {Color::kBlack, 7},
                                   {Color::kBlack, 1},
                                   {Color::kRed, 13},
                                   {Color::kBlack, 11},
                                   {Color::kBlack, 45},
                                   {Color::kBlack, 24},
                                   {Color::kRed, 34},
                                   {Color::kBlack, 56}};
    std::vector<InfoTree> result = a.GetTreeLikePreorderArray();
    ASSERT_EQ(expected, result);
}
TEST(RB_Test, RandomBigTree2)
{
    RBTree a;
    a.Insert(13);
    a.Insert(11);
    a.Insert(1);
    a.Insert(2);
    a.Insert(3);
    a.Insert(4);
    a.Insert(36);
    a.Insert(78);
    a.Insert(0);
    a.Insert(7);
    a.Insert(8);
    a.Insert(15);
    a.Insert(5);
    a.Erase(4);
    a.Erase(11);
    a.Erase(36);
    a.Erase(8);
    std::vector<InfoTree> expected{{Color::kBlack, 3},
                                   {Color::kBlack, 1},
                                   {Color::kBlack, 0},
                                   {Color::kBlack, 2},
                                   {Color::kBlack, 7},
                                   {Color::kBlack, 5},
                                   {Color::kRed, 15},
                                   {Color::kBlack, 13},
                                   {Color::kBlack, 78}};
    std::vector<InfoTree> result = a.GetTreeLikePreorderArray();
    ASSERT_EQ(expected, result);
}
TEST(RB_Test, RandomBigTree3)
{
    RBTree a;
    a.Insert(1);
    a.Insert(2);
    a.Insert(3);
    a.Insert(4);
    a.Insert(5);
    a.Insert(6);
    a.Insert(7);
    a.Insert(8);
    a.Insert(9);
    a.Insert(10);
    a.Insert(11);
    a.Insert(12);
    a.Insert(13);
    a.Erase(2);
    a.Erase(10);
    a.Erase(4);
    a.Erase(6);
    std::vector<InfoTree> expected{{Color::kBlack, 3},
                                   {Color::kBlack, 1},
                                   {Color::kBlack, 0},
                                   {Color::kBlack, 2},
                                   {Color::kBlack, 7},
                                   {Color::kBlack, 5},
                                   {Color::kRed, 15},
                                   {Color::kBlack, 13},
                                   {Color::kBlack, 78}};
    std::vector<InfoTree> result = a.GetTreeLikePreorderArray();
    ASSERT_EQ(expected, result);
}