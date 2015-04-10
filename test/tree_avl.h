#pragma once
#include <assert.h>
#include <algorithm>
#include <fstream>

/// <summary> The default keys comparison function. </summary>
/// <remarks> Author: Vladimir Zelyonkin </remarks>
template<class T> int defCompFunc(const T& a, const T& b)
{
    if(a < b)
        return -1;
    if(a > b)
        return 1;
    return 0;
}

/// <summary> Implements tree node, contains pair of value and key. </summary>
/// <remarks> Author: Vladimir Zelyonkin </remarks>
template<class Key, class Val> class TreeNode
{
public:
    // Enumeration for manupulations with left/right children of the node
    enum EBranch { eLeft = 0, eRight = 1 };

public:
    // constructor/destructor
    TreeNode(const Key& key) : m_key(key), m_height(1), m_parent(NULL) { m_child[eLeft] = m_child[eRight] = NULL; }
    ~TreeNode() { delete m_child[0]; delete m_child[1]; }

    // access to node height and balance
    unsigned char height(EBranch branch) const { return m_child[branch] ? m_child[branch]->m_height : 0; }
    int balance() const { return height(eLeft) - height(eRight); }
    void update_height() { m_height = 1 + std::max(height(eLeft), height(eRight)); }

    // access to parent/left/right nodes
    TreeNode* parent() { return m_parent; }
    TreeNode* left() { return m_child[eLeft]; }
    TreeNode* right() { return m_child[eRight]; }

private:
    // assignment is forbidden
    TreeNode<Key, Val>& operator=(const TreeNode<Key, Val>&) { return *this; }

public:
    // parent node
    TreeNode* m_parent;
    // left/fight child
    TreeNode* m_child[2];
    // height of the node in tree (for an empty node height = 1) 
    unsigned char m_height;
    // node value
    Val m_value;
    // node key
    const Key m_key;
};

/// <summary> The AVL tree itemplate implementation: balanced binary tree. </summary>
/// <remarks> Author: Vladimir Zelyonkin </remarks>
template<class Key, class Val> class Tree
{
public:
    typedef int(*t_fnCompare)(const Key& a, const Key& b);
    typedef TreeNode<Key, Val> Node;

public:
    /// <summary> Tree iterator. </summary>
    /// <remarks> Author: Vladimir Zelyonkin </remarks>
    class Iterator
    {       
        friend class Tree<Key, Val>;
    public:

        /// <summary> Constructor </summary>
        /// <remarks> Author: Vladimir Zelyonkin </remarks>
        explicit Iterator(Node* node) : m_node(node) {}
        
        /// <summary> Moves iterator to next node in the tree. </summary>
        /// <returns> True if the curent node isn't end </returns>
        /// <remarks> Author: Vladimir Zelyonkin </remarks>
        bool next();

        /// <summary> Checks whether the node is end node of the tree. </summary>
        /// <remarks> Author: Vladimir Zelyonkin </remarks>
        bool isEnd() const { return m_node == NULL; }

        /// <summary> Queries the node key. </summary>
        /// <remarks> Author: Vladimir Zelyonkin </remarks>
        const Key& key() const { return m_node->m_key; }

        /// <summary> Accesses the node value. </summary>
        /// <remarks> Author: Vladimir Zelyonkin </remarks>
        Val& value() { return m_node->m_value; }

    private:
        Node* m_node;
    };
    
    /// <summary> Constructor </summary>
    /// <param name="fnCmp"> in. Optional. Pointer to function for comparison of keys. </param>
    /// <remarks> Author: Vladimir Zelyonkin </remarks>
    explicit Tree(t_fnCompare fnCmp = NULL) : m_root(NULL), m_fnCmp(fnCmp ? fnCmp : defCompFunc<Key>) {}

    /// <summary> Destructor </summary>
    /// <remarks> Author: Vladimir Zelyonkin </remarks>
    virtual ~Tree() { delete m_root; }

    /// <summary> Searches for node with specified key. </summary>
    /// <returns> Pointer to node value, NULL if specified key isn't found in the tree. </returns>
    /// <param name="key"> in. The key of node to be found. </param>
    /// <remarks> Author: Vladimir Zelyonkin </remarks>
    Val* find(const Key& key);

    /// <summary> Searches for node with specified key. </summary>
    /// <returns> Pointer to node value, NULL if specified key isn't found in the tree. </returns>
    /// <param name="key"> in. The key of node to be found. </param>
    /// <remarks> Author: Vladimir Zelyonkin </remarks>
    const Val* find(const Key& key) const;
    
    /// <summary> Removes node with specified key from the tree. </summary>
    /// <param name="key"> in. The key of node to be removed. </param>
    /// <remarks> Author: Vladimir Zelyonkin </remarks>
    void erase(const Key& key);

    /// <summary> Inserts new node into the tree. </summary>
    /// <param name="key"> in. The node key. </param>
    /// <param name="val"> in. The node value. </param>
    /// <remarks> Author: Vladimir Zelyonkin </remarks>
    void insert(const Key& key, const Val& val);

    /// <summary> Inserts new node into the tree. </summary>
    /// <param name="pair"> in. The node key and value. </param>
    /// <remarks> Author: Vladimir Zelyonkin </remarks>
    void insert(const std::pair<Key, Val>& pair) { insert(pair.first, pair.second); }
    
    /// <summary> Accesses the node value by its key. Important: if node with specified key isn't exists in tree - node with default value will be inserted. </summary>
    /// <returns> The reference to node value. </returns>
    /// <param name="key"> in. The key of node to be found. </param>
    /// <remarks> Author: Vladimir Zelyonkin </remarks>
    Val& operator[](const Key& key) { return node_imp(key).m_value; }
    
    /// <summary> Queries the tree iterator. </summary>
    /// <returns> The iteraror. </returns>
    /// <remarks> Author: Vladimir Zelyonkin </remarks>
    Iterator begin();

    /// <summary> 
    /// Saves the tree to graphviz-format file. 
    /// This file may be converted to .pdf or .png using graphviz utility. 
    /// Example of command line to convert file to .png: "%GRAPHVIZ%\release\bin\dot.exe -Kdot -Gratio=0.6 -Gsplines=true -Nfontsize=20 -Tpng [file_name] -O"
    /// </summary>
    /// <param name="sFile"> in. The output file name. </param>
    /// <remarks> Author: Vladimir Zelyonkin </remarks>
    void saveToGv(const char* sFile);

private:
    bool find_imp(Node*& pNode, const Key& key) const;
    Node& node_imp(const Key& key);
    void update_balance(Node& node);
    void setChild(Node& parent, Node& child, typename Node::EBranch b) const;
    void moveChild(Node& from, typename Node::EBranch bf, Node& to, typename Node::EBranch bt) const;
    void moveNode(Node& parent, typename Node::EBranch bf, Node& toNode);
    Node* rotate_left(Node& node);
    Node* rotate_right(Node& node);  

    // assignment is forbidden
    Tree<Key, Val>& operator=(const Tree<Key, Val>&) { return *this; }
private:
    const t_fnCompare m_fnCmp;
    Node* m_root;
};


/// <summary> Moves iterator to next node in the tree. </summary>
/// <returns> True if the curent node isn't end </returns>
/// <remarks> Author: Vladimir Zelyonkin </remarks>
template<class Key, class Val> bool Tree<Key, Val>::Iterator::next()
{
    if(!m_node)
        return false;

    // minimal element in right branch
    if(m_node->right())
    {
        m_node = m_node->right();
        while(m_node->left())
            m_node = m_node->left();
        return m_node != NULL;
    }

    // parent node
    Node* pParent = m_node->parent();
    while(pParent && pParent->right() == m_node)
    {
        m_node = pParent;
        pParent = m_node->parent();
    }
    m_node = pParent;
    return m_node != NULL;
}

/// <summary> Queries the tree iterator. </summary>
/// <returns> The iteraror. </returns>
/// <remarks> Author: Vladimir Zelyonkin </remarks>
template<class Key, class Val> typename Tree<Key, Val>::Iterator Tree<Key, Val>::begin()
{
    if(!m_root)
        return Iterator(NULL);
    Node* pNode = m_root;
    while(pNode->left())
        pNode = pNode->left();
    return Iterator(pNode);
}

/// <summary> Searches for node with specified key. </summary>
/// <returns> Pointer to node value, NULL if specified key isn't found in the tree. </returns>
/// <param name="key"> in. The key of node to be found. </param>
/// <remarks> Author: Vladimir Zelyonkin </remarks>
template<class Key, class Val> const Val* Tree<Key, Val>::find(const Key& key) const
{
    Node* pNode;
    return find_imp(pNode, key) ? &pNode->m_value : NULL;
}


/// <summary> Searches for node with specified key. </summary>
/// <returns> Pointer to node value, NULL if specified key isn't found in the tree. </returns>
/// <param name="key"> in. The key of node to be found. </param>
/// <remarks> Author: Vladimir Zelyonkin </remarks>
template<class Key, class Val> Val* Tree<Key, Val>::find(const Key& key)
{
    Node* pNode;
    return find_imp(pNode, key) ? &pNode->m_value : NULL;
}

/// <summary> Inserts new node into the tree. </summary>
/// <param name="key"> in. The node key. </param>
/// <param name="val"> in. The node value. </param>
/// <remarks> Author: Vladimir Zelyonkin </remarks>
template<class Key, class Val> void Tree<Key, Val>::insert(const Key& key, const Val& val)
{
    node_imp(key).m_value = val;
}

/// <summary> Searches for node with specified key. </summary>
/// <returns> True if node found. </returns>
/// <param name="pNode"> out. Pointer to node if node found, otherwise - pointer to parent node for node to be inserted. </param>
/// <param name="key"> in. The key to be found. </param>
/// <remarks> Author: Vladimir Zelyonkin </remarks>
template<class Key, class Val> bool Tree<Key, Val>::find_imp(Node*& pNode, const Key& key) const
{
    pNode = m_root;
    while(pNode)
    {
        const int cmp = m_fnCmp(key, pNode->m_key);
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

/// <summary> Accesses the node value by its key. Important: if node with specified key isn't exists in tree - node with default value will be inserted. </summary>
/// <returns> The reference to node. </returns>
/// <param name="key"> in. The key of node to be found. </param>
/// <remarks> Author: Vladimir Zelyonkin </remarks>
template<class Key, class Val> TreeNode<Key, Val>& Tree<Key, Val>::node_imp(const Key& key)
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
    const int cmp = m_fnCmp(key, pNode->m_key);
    assert(cmp != 0);
    Node* pChild = new Node(key);
    setChild(*pNode, *pChild, cmp < 0 ? Node::eLeft : Node::eRight);

    // balance tree
    update_balance(*pNode);
    return *pChild;
}

/// <summary> Sets child for specified parent. </summary>
/// <param name="parent"> inout. The parent node. </param>
/// <param name="child"> inout. The child node. </param>
/// <param name="b"> in. The branch which specified left/right child.  </param>
/// <remarks> Author: Vladimir Zelyonkin </remarks>
template<class Key, class Val> void Tree<Key, Val>::setChild(Node& parent, Node& child, typename Node::EBranch b) const
{
    assert(!parent.m_child[b]);
    assert(b == Node::eLeft ? m_fnCmp(child.m_key, parent.m_key) < 0 : m_fnCmp(child.m_key, parent.m_key) > 0);
    parent.m_child[b] = &child;
    child.m_parent = &parent;
}

/// <summary> Moves child from parent to another one. </summary>
/// <param name="from"> inout. The source parent node. </param>
/// <param name="bf"> in. The source branch. </param>
/// <param name="to"> inout. The destination parent node.  </param>
/// <param name="bt"> in. The destination branch. </param>
/// <remarks> Author: Vladimir Zelyonkin </remarks>
template<class Key, class Val> void Tree<Key, Val>::moveChild(Node& from, typename Node::EBranch bf, Node& to, typename Node::EBranch bt) const
{
    assert(&from != &to);

    if(to.m_child[bt])
        to.m_child[bt]->m_parent = NULL;

    to.m_child[bt] = from.m_child[bf];
    from.m_child[bf] = NULL;

    if(to.m_child[bt])
    {
        to.m_child[bt]->m_parent = &to;
        assert(bt == Node::eLeft ? m_fnCmp(to.m_child[bt]->m_key, to.m_key) < 0 : m_fnCmp(to.m_child[bt]->m_key, to.m_key) > 0);
    }
}

/// <summary> Moves node to specified parent. </summary>
/// <param name="parent"> inout. The destination parent node. </param>
/// <param name="bt"> in. The destination branch. </param>
/// <param name="node"> inout. The node to be moved. </param>
/// <remarks> Author: Vladimir Zelyonkin </remarks>
template<class Key, class Val> void Tree<Key, Val>::moveNode(Node& parent, typename Node::EBranch b, Node& node)
{
    if(Node* pParentTo = node.parent())
    {
        const Node::EBranch bt = &node == pParentTo->left() ? Node::eLeft : Node::eRight;
        moveChild(parent, b, *pParentTo, bt);
    }
    else
    {
        // node is root
        m_root = parent.m_child[b];
        if(m_root)
            m_root->m_parent = NULL;
        parent.m_child[b] = NULL;
    }
}

/// <summary> Removes node with specified key from the tree. </summary>
/// <param name="key"> in. The key of node to be removed. </param>
/// <remarks> Author: Vladimir Zelyonkin </remarks>
template<class Key, class Val> void Tree<Key, Val>::erase(const Key& key)
{
    // empty tree
    if(!m_root)
        return;

    // search for node 
    Node* pNode;
    if(!find_imp(pNode, key))
        return;
    
    // remove pNode from tree
    Node* pNodeUpdate = NULL;
    if(Node* pMin = pNode->right())
    {
        // search for minimal node in right branch
        const Node::EBranch branchMin = pMin->left() ? Node::eLeft : Node::eRight;
        while(pMin->left())
            pMin = pMin->left();
        
        Node& parentMin = *pMin->parent();

        // replace pNode by pMin
        moveNode(parentMin, branchMin, *pNode);

        // replace pMin by pMin->right
        moveChild(*pMin, Node::eRight, parentMin, branchMin);

        // move children from pNode to pMin
        moveChild(*pNode, Node::eLeft, *pMin, Node::eLeft);
        moveChild(*pNode, Node::eRight, *pMin, Node::eRight);

        // update balance starting from parent of pMin
        pNodeUpdate = pMin;
    }
    else
    {
        // update balance starting from parent
        pNodeUpdate = pNode->parent();

        // if right branch is empty, replace pNode by left child
        moveNode(*pNode, Node::eLeft, *pNode);
    }

    // destroy node
    assert(pNode->left() == NULL && pNode->right() == NULL && pNode->parent() == NULL);
    delete pNode;

    // update tree balance
    if(pNodeUpdate)
        update_balance(*pNodeUpdate);
}

/// <summary> 
/// Saves the tree to graphviz-format file. 
/// This file may be converted to .pdf or .png using graphviz utility. 
/// Example of command line to convert file to .png: "%GRAPHVIZ%\release\bin\dot.exe -Kdot -Gratio=0.6 -Gsplines=true -Nfontsize=20 -Tpng [file_name] -O"
/// </summary>
/// <param name="sFile"> in. The output file name. </param>
/// <remarks> Author: Vladimir Zelyonkin </remarks>
template<class Key, class Val> void Tree<Key, Val>::saveToGv(const char* sFile)
{
    std::fstream file;
    file.open(sFile, std::ios_base::out | std::ios_base::trunc);

    file << "digraph tree\n{";
    
    // list of nodes with labels
    for(Tree<int, int>::Iterator it = begin(); !it.isEnd(); it.next())
        file << "\n node_" << it.key() << " [label=\"" << it.key() << " h" << int(it.m_node->m_height) << " b" << it.m_node->balance() << "\"];";

    // lenks between nodes
    file << "\n";
    for(Tree<int, int>::Iterator it = begin(); !it.isEnd(); it.next())
    {
        if(it.m_node->left())
            file << "\n node_" << it.m_node->m_key << " -> node_" << it.m_node->left()->m_key;
        if(it.m_node->right())
            file << "\n node_" << it.m_node->m_key << " -> node_" << it.m_node->right()->m_key;
    }
    file << "\n}";
}

/// <summary> Makes small left rotation around the specified node. </summary>
/// <returns> The pointer to new root node. </returns>
/// <param name="node"> in. The node to be balanced. </param>
/// <remarks> Author: Vladimir Zelyonkin </remarks>
template<class Key, class Val> TreeNode<Key, Val>* Tree<Key, Val>::rotate_left(Node& node)
{
    Node& right = *node.right();
    moveNode(node, Node::eRight, node);
    moveChild(right, Node::eLeft, node, Node::eRight);
    setChild(right, node, Node::eLeft);
    
    node.update_height();
    right.update_height();
    return &right;
}

/// <summary> Makes small right rotation around the specified node. </summary>
/// <returns> The pointer to new root node. </returns>
/// <param name="node"> in. The node to be balanced. </param>
/// <remarks> Author: Vladimir Zelyonkin </remarks>
template<class Key, class Val> TreeNode<Key, Val>* Tree<Key, Val>::rotate_right(Node& node)
{
    Node& left = *node.left();
    moveNode(node, Node::eLeft, node);
    moveChild(left, Node::eRight, node, Node::eLeft);
    setChild(left, node, Node::eRight);

    node.update_height();
    left.update_height();
    return &left;
}

/// <summary> Perofrms balancing of the tree from the specified node till root. </summary>
/// <param name="node"> in. The node to be balanced. </param>
/// <remarks> Author: Vladimir Zelyonkin </remarks>
template<class Key, class Val> void Tree<Key, Val>::update_balance(Node& node)
{
    for(Node* pNode = &node; pNode;)
    {
        // update node balance
        pNode->update_height();

        // rebalance
        if(pNode->balance() == -2)
        {
            if(pNode->right() && pNode->right()->balance() > 0)
                rotate_right(*pNode->right());
            pNode = rotate_left(*pNode);
        }
        else if(pNode->balance() == 2)
        {
            if(pNode->left() && pNode->left()->balance() < 0)
                rotate_left(*pNode->left());
            pNode = rotate_right(*pNode);
        }
        else
            pNode = pNode->parent();
    }
}
