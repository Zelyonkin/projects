
template<class T> int defCompFunc(const T& a, const T& b)
{
    if(a < b)
        return -1;
    if(a > b)
        return 1;
    return 0;
}

template<class Key, class Val> class TreeNode
{
public:
    TreeNode(const Key& key, const Val& val) : m_key(key), m_value(val), m_left(NULL), m_right(NULL), m_depth(0) {}

private:
    Val m_value;
    TreeNode* m_left;
    TreeNode* m_right;
    unsigned char m_depth;

public:
    const Key m_key;
};

template<class Key, class Val> class Tree
{
public:

    typedef TreeNode<Key, Val> Node;

    typedef void(*t_fnInit)(Val& val);
    typedef int(*t_fnCompare)(const Key& a, const Key& b);
    
    explicit Tree(t_fnCompare fnCmp = NULL) : m_root(NULL), m_fnCmp(fnCmp ? fnCmp : defCompFunc<Key>) {}

    Node* find(const Key& key) const;
    Node* insert(const Key& key, const Val& val);
    void remove(const Key& key);

private:
    const t_fnCompare m_fnCmp;
    Node* m_root;
};

template<class Key, class Val> 
TreeNode<Key, Val>* Tree<Key, Val>::find(const Key& key) const
{
    Node* pNode = m_root;
    while(pNode)
    {
        if(pNode->m_key > key)
            pNode = pNode->m_left;
        else if(pNode->m_key < key)
            pNode = pNode->m_right;
        else
            return pNode;
    }
    return NULL;
}

template<class Key, class Val> 
TreeNode<Key, Val>* Tree<Key, Val>::insert(const Key& key, const Val& val)
{
    return NULL;
}

template<class Key, class Val> void Tree<Key, Val>::remove(const Key& key)
{
}