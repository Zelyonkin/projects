#pragma once
#include "gtest\gtest.h"
#include "tree_avl.h"

template<int nKey, bool bShuffle> class TestTree : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        // prepare data
        m_aKey.resize(nKey);
        m_aVal.resize(nKey);
        for(int i = 0; i < nKey; ++i)
            m_aKey[i] = m_aVal[i] = i;

        // disable warning "conditional expression is constant"
#pragma warning(push)
#pragma warning (disable : 4127)
        // shuffle if need
        if(bShuffle)
        {
            std::random_device device;
            std::default_random_engine generator(device());
            generator.seed(nKey);
            std::shuffle(m_aKey.begin(), m_aKey.end(), generator);
        }
#pragma warning(pop)

        // insert data into the tree
        for(int i = 0; i < nKey; ++i)
            m_tree.insert(m_aKey[i], m_aVal[i]);
    }

    void test_find()
    {
        // search for each key and compare with corresponding value
        for(int i = 0, n = (int)m_aKey.size(); i < n; ++i)
        {
            const int val = m_aVal[i];
            const int* pVal = m_tree.find(m_aKey[i]);
            ASSERT_TRUE(pVal != NULL);
            ASSERT_EQ(*pVal, val);
        }
    }

    void test_erase()
    {
        // erase each key and ensure that corresponding key missed in the tree
        for(int i = 0, n = (int)m_aKey.size(); i < n; ++i)
        {
            m_tree.erase(m_aKey[i]);
            const int* pVal = m_tree.find(m_aKey[i]);
            ASSERT_TRUE(pVal == NULL);
        }

        // ensure that the tree is empty
        ASSERT_TRUE(m_tree.begin().isEnd());
    }

    void test_iterator()
    {
        // iterate all keys in the tree, ensure that prev < next
        Tree<int, int>::Iterator it = m_tree.begin();
        int count = 1, prev = it.key();
        for(it.next(); !it.isEnd(); it.next(), ++count)
        {
            const int cur = it.value();
            ASSERT_LT(prev, cur);
            prev = cur;
        }

        // ensure that number of elements in the tree equals to number of initial keys
        ASSERT_EQ(count, m_aKey.size());
    }

protected:
    std::vector<int> m_aVal;
    std::vector<int> m_aKey;
    Tree<int, int> m_tree;
};

// tests for tree with 10 consecutive elements
typedef TestTree<10, false> Tree_10_consec;
TEST_F(Tree_10_consec, TestFind)
{
    test_find();
}

TEST_F(Tree_10_consec, TestErase)
{
    test_erase();
}

TEST_F(Tree_10_consec, TestIterator)
{
    test_iterator();
}

// tests for tree with 10 shuffled elements
typedef TestTree<10, false> Tree_10_shuffled;
TEST_F(Tree_10_shuffled, TestFind)
{
    test_find();
}

TEST_F(Tree_10_shuffled, TestErase)
{
    test_erase();
}

TEST_F(Tree_10_shuffled, TestIterator)
{
    test_iterator();
}

int main_gtest(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}