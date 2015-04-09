#pragma once
#include "tree_avl.h"
#include <vector>
#include <random>
#include <iostream>
#include <time.h>
#include <map>

class Timing
{
public:
    Timing() : m_time(0) {}

    void start() 
    {
        m_time = clock();
    }

    double stop()
    {
        m_time = clock() - m_time;
        return time();
    }

    double time()
    {
        return double(m_time)/CLOCKS_PER_SEC;
    }
private:
    clock_t m_time; 
};

template<class Tree> void test_peformance(double& insert, double& find, double& remove, const std::vector<int>& aKey)
{
    // tree to be tested
    Timing time;
    Tree tree;

    // insert data into the tree
    time.start();
    for(size_t i = 0, n = aKey.size(); i < n; ++i)
        tree.insert(std::make_pair(aKey[i], (int)i));
    insert = time.stop();

    // search data in the tree
    time.start();
    for(size_t i = 0, n = aKey.size(); i < n; ++i)
        tree.find(aKey[i]);
    find = time.stop();

    // remove data from the tree
    time.start();
    for(size_t i = 0, n = aKey.size(); i < n; ++i)
        tree.erase(aKey[i]);
    remove = time.stop();
}

void test_peformance(int nKey, bool bShuffle)
{
    // prepare data
    std::vector<int> aKey(nKey);
    for(int i = 0; i < nKey; ++i)
        aKey[i] = i;

    // shuffle if need
    if(bShuffle)
    {
        std::random_device device;
        std::default_random_engine generator(device());
        generator.seed(10);
        std::shuffle(aKey.begin(), aKey.end(), generator);
    }

    // test
    std::cout << "Compare performance with std::map. Number of elements: " << aKey.size() << ", order: " << (bShuffle ? "shuffled." : "consecutive.");
   
    double insert, find, remove;
    test_peformance<Tree<int, int>>(insert, find, remove, aKey);
    std::cout << "\nTree timing:\n insert=" << insert << " sec, find=" << find << " sec, remove=" << remove << " sec";

    double insert_std, find_std, remove_std;
    test_peformance<std::map<int, int>>(insert_std, find_std, remove_std, aKey);
    std::cout << "\nstd::map timing:\n insert=" << insert_std << " sec, find=" << find_std << " sec, remove=" << remove_std << " sec";

    std::cout << "\nDifference:\n insert=" << insert_std / insert << " find=" << find_std / find << " remove=" << remove_std / remove;
    std::cout << "\n****\n\n";
}