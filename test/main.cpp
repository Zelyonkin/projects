#include "stdafx.h"
#include "test_gtest.h"
#include "test_performance.h"
#include <iostream>

static void generate_tree(const wchar_t* sFile, int nElem, bool bShuffle)
{
    // generate set of keys
    const int nValue = 10000;
    std::vector<int> aKey(nElem);
    for(int i = 0; i < nElem; ++i)
        aKey[i] = i;

    if(bShuffle)
    {
        std::random_device device;
        std::default_random_engine generator(device());
        generator.seed(10);
        std::shuffle(aKey.begin(), aKey.end(), generator);
    }

    // create tree
    Tree<int, int> tree;
    for(int i = 0; i < nElem; ++i)
        tree.insert(aKey[i], i);

    // save the tree
    tree.saveToGv(sFile);
}

int main(int argc, wchar_t* argv[])
{    
    if(argc == 2 && *argv[1] == L'g')
    {
        // run google test
        return main_gtest(argc, argv);
    }
    else if(argc == 4 && *argv[1] == L'p')
    {
        // run performance test
        const int nElem = _wtoi(argv[2]);
        const bool bShuffle = _wtoi(argv[3]) == 1;
        test_peformance(nElem, bShuffle);
        return 0;
    }
    else if(argc == 5 && *argv[1] == L's')
    {
        // generate tree and save to gv-file
        const int nElem = _wtoi(argv[2]);
        const bool bShuffle = _wtoi(argv[3]) == 1;
        generate_tree(argv[4], nElem, bShuffle);
        return 0;
    }

    // incorrect command line
    std::cout << "Usage:\n\
                 \r 1: test.exe g                    - run the google test\n\
                 \r 2: test.exe p 1000 1             - run performance test for tree with 1000 elements, initial keys sequence: 1 - shuffled, 0 - consecutive\n\
                 \r 3: test.exe s 1000 1 filename.gv - generate tree and save it to gv-file filename.gv";

    return -1;
}
