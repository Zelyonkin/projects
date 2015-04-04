#include <assert.h>
#include <algorithm>

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
    TreeNode(const Key& key) : m_key(key), m_depth(0), m_parent(NULL) { m_child[eLeft] = m_child[eRight] = NULL; }

    enum EBranch { eLeft = 0, eRight = 1 };
    
    void setChild(TreeNode& child, EBranch branch)
    {
        assert(!m_child[branch]);
        m_child[branch] = &child;
        child.m_parent = this;

        // update depth
        m_depth = 1 + std::max(depth(eLeft), depth(eRight));
    }

    void replaceChild(TreeNode& old, TreeNode* pNew)
    {
        assert(old.m_parent == this);
        assert(!pNew || pNew->m_parent == NULL);
        if(&old == m_child[eLeft])
            m_child[eLeft] = pNew;
        else
        {
            assert(&old == m_child[eRight]);
            m_child[eRight] = pNew;
        }
        old.m_parent = NULL;
        if(pNew)
            pNew->m_parent  = this;

        // update depth
        m_depth = 1 + std::max(depth(eLeft), depth(eRight));
    }

    int depth(EBranch branch) const
    {
        return m_child[branch] ? 0 : m_child[branch]->m_depth;
    }

    int balance() const { return depth(eLeft) - depth(eRight); }

    TreeNode* parent() { return m_parent; }
    TreeNode* left() { return m_child[eLeft]; }
    TreeNode* right() { return m_child[eRight]; }

private:
    TreeNode* m_parent;
    TreeNode* m_child[2];
    unsigned char m_depth;

public:
    Val m_value;
    const Key m_key;
};

template<class Key, class Val> class Tree
{
public:
    typedef void(*t_fnInit)(Val& val);
    typedef int(*t_fnCompare)(const Key& a, const Key& b);

    typedef TreeNode<Key, Val> Node;
    
    explicit Tree(t_fnCompare fnCmp = NULL) : m_root(NULL), m_fnCmp(fnCmp ? fnCmp : defCompFunc<Key>) {}
    virtual ~Tree() {}

    Val* find(const Key& key);
    const Val* find(const Key& key) const;
    void remove(const Key& key);
    void insert(const Key& key, const Val& val);

private:
    Node& node_imp(const Key& key);
    bool find_imp(Node*& pNode, const Key& key) const;
    void update_balance(Node& node) {}

private:
    const t_fnCompare m_fnCmp;
    Node* m_root;
};

template<class Key, class Val> 
const Val* Tree<Key, Val>::find(const Key& key) const
{
    Node* pNode;
    return find_imp(pNode, key) ? &pNode->m_value : NULL;
}

template<class Key, class Val> 
Val* Tree<Key, Val>::find(const Key& key)
{
    Node* pNode;
    return find_imp(pNode, key) ? &pNode->m_value : NULL;
}

template<class Key, class Val> 
void Tree<Key, Val>::insert(const Key& key, const Val& val)
{
    node_imp(key).m_value = val;
}

template<class Key, class Val> 
bool Tree<Key, Val>::find_imp(Node*& pNode, const Key& key) const
{
    pNode = m_root;
    while(pNode)
    {
        const int cmp = m_fnCmp(pNode->m_key, key);
        if(cmp < 0)
        {
            if(!pNode->left())
                return false;
            pNode = pNode->left();
        }
        else if(cmp > 0)
        {
            if(!pNode->right())
                return false;
            pNode = pNode->right();
        }
        else if(cmp == 0)
            return true;
    }
    return false;
}

template<class Key, class Val> 
TreeNode<Key, Val>& Tree<Key, Val>::node_imp(const Key& key)
{
    // search for existing node
    Node* pNode;
    if(find_imp(pNode, key))
        return *pNode;

    // case when tree is empty
    if(!pNode)
    {
        assert(pNode == m_root);
        m_root = new Node(key);
        return *m_root;
    }

    // pNode - is a parent, create new child;
    const int cmp = m_fnCmp(pNode->m_key, key);
    assert(cmp != 0);
    Node* pChild = new Node(key);
    pNode->setChild(*pChild, cmp < 0 ? Node::eLeft : Node::eRight);

    // balance tree
    update_balance(*pNode);
    return *pChild;
}

template<class Key, class Val> void Tree<Key, Val>::remove(const Key& key)
{
    // empty tree
    if(!m_root)
        return;

    // search for node 
    Node* pNode;
    if(!find_imp(pNode, key))
        return;
    
    // remove pNode from tree
    Node* pNodeUpdate;
    if(Node* pMin = pNode->right())
    {
        // search for minimal node in right branch
        while(pMin->left())
            pMin = pMin->left();

        // replace pNode by pMin
        if(pNode->parent())
            pNode->parent()->replaceChild(*pNode, pMin);
        else 
            m_root = pMin;

        if(pNode->left())
            pMin->setChild(*pNode->left(), Node::eLeft);
        pMin->setChild(*pNode->right(), Node::eRight);

        // update balance starting from pMin
        pNodeUpdate = pMin;
    }
    else
    {
        // if right branch is empty, replace pNode by left child
        if(pNode->parent())
            pNode->parent()->replaceChild(*pNode, pNode->left());
        else
            m_root = pNode->left();

        // update balance starting from parent
        pNodeUpdate = pNode->parent();
    }

    // destroy node
    delete pNode;

    // update tree balance
    if(pNodeUpdate)
        update_balance(*pNodeUpdate);
}