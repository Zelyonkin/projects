#include "stdafx.h"
#include "tree_avl.h"
#include <vector>
#include <random>

int _tmain(int argc, _TCHAR* argv[])
{
    const int nValue = 10000;
    
    // generate random set of keys and values
    std::vector<int> aKey(nValue), aVal(nValue);
    {
        for(int i = 0; i < nValue; ++i)
            aKey[i] = aVal[i] = i;

        std::random_device device;
        std::default_random_engine generator(device());
        std::shuffle(aKey.begin(), aKey.end(), generator);
    }

    Tree<int, int> tree;

    // insert data into the tree
    for(int i = 0; i < nValue; ++i)
        tree.insert(aKey[i], aVal[i]);

    // find
    for(int i = 0; i < nValue; ++i)
    {
        const int* pVal = tree.find(aKey[i]);
        assert(pVal && *pVal == aVal[i]);
    }

    // remove
    for(int i = 0; i < nValue; ++i)
        tree.remove(aKey[i]);
	
    return 0;
}
