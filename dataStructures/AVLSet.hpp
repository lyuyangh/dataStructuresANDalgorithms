// AVLSet.hpp
//
//
// An AVLSet is an implementation of a Set that is an AVL tree. 
#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include "Set.hpp"
#include <algorithm>


template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    // Initializes an AVLSet to be empty, with or without balancing.
    explicit AVLSet(bool shouldBalance = true);

    // Cleans up the AVLSet so that it leaks no memory.
    virtual ~AVLSet() noexcept;

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s) noexcept;

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s) noexcept;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // height() returns the height of the AVL tree.  Note that, by definition,
    // the height of an empty tree is -1.
    int height() const;


    // preorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a preorder traversal of the AVL
    // tree.
    void preorder(VisitFunction visit) const;


    // inorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by an inorder traversal of the AVL
    // tree.
    void inorder(VisitFunction visit) const;


    // postorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a postorder traversal of the AVL
    // tree.
    void postorder(VisitFunction visit) const;


private:
    struct Node
    {
        ElementType element;
        Node* left;
        Node* right;
        int height;
    };

    Node* root;
    bool shouldBalance;

private:
    static void deleteTree(Node*& tree) noexcept;
    static bool lookup(const ElementType& element,Node* tree);
    static void copyTree(Node*& root, const Node* tree);
    
    static void insertTree(const ElementType& element, Node*& tree,bool shouldBalance);
    static unsigned int treeSize(Node* tree) noexcept;
    static int treeHeight(Node* tree);

    static Node* RR(Node* A);
    static Node* RL(Node* A);
    static Node* LR(Node* C);
    static Node* LL(Node* B);

    static void maintainAVL(Node*& tree,bool shouldBalance);
    static void preorderTree(Node* tree, VisitFunction visit);
    static void inorderTree(Node* tree, VisitFunction visit);
    static void postorderTree(Node* tree, VisitFunction visit);
};


template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance)
    :root(nullptr),shouldBalance(shouldBalance)
{
}


template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
    deleteTree(root);
}

template <typename ElementType>
void AVLSet<ElementType>::deleteTree(Node*& tree) noexcept
{
    if(tree == nullptr)
        delete tree;
    else
    {
        deleteTree(tree->left);
        deleteTree(tree->right);
        delete tree;
    }
}

template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
    :root(nullptr),shouldBalance(s.shouldBalance)
{
    copyTree(root,s.root); 
}

template <typename ElementType>
void AVLSet<ElementType>::copyTree(Node*& root, const Node* tree)
{
    if(tree!=nullptr)
    {
        root = new Node{tree->element, nullptr,nullptr,tree->height};
        copyTree(root->left,tree->left);
        copyTree(root->right,tree->right);
    }
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
    :root(s.root),shouldBalance(s.shouldBalance)
{
    s.root = nullptr;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
    deleteTree(root);
    copyTree(root,s.root);
    return *this;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
    deleteTree(root);
    root = s.root;
    s.root = nullptr;
    return *this;
}


template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{
    insertTree(element, root,shouldBalance);
}

template <typename ElementType>
void AVLSet<ElementType>::insertTree(const ElementType& element, Node*& tree, bool shouldBalance)
{
    //insert
    if(tree==nullptr)
    {
        tree = new Node{element, nullptr,nullptr,0};
        
    }
    else
    {
        if(tree->element == element)
            return;
        else if (tree->element > element)
            insertTree(element,tree->left,shouldBalance);

        else
            insertTree(element,tree->right,shouldBalance);
    }
    

    //update height
    tree->height = 1+ std::max(treeHeight(tree->left),treeHeight(tree->right));
    //maintain balance
    maintainAVL(tree,shouldBalance);
}
    
template <typename ElementType>
void AVLSet<ElementType>::maintainAVL(Node*& tree,bool shouldBalance)
{
    if(shouldBalance)
    {
        int leftHeight=0;
        int rightHeight=0;
        if(tree->left)
            leftHeight = tree->left->height +1;
        if(tree->right)
            rightHeight = tree->right->height +1;

        int difference = leftHeight - rightHeight;
        if(difference>1)
        {
            Node* leftTree = tree->left;
            if(treeHeight(leftTree->left) > treeHeight(leftTree->right))
                tree = LL(tree);
            else
                tree = LR(tree);
        }
        else if(difference <-1)
        {
            Node* rightTree = tree->right;
            if(treeHeight(rightTree->left) > treeHeight(rightTree->right))
                tree = RL(tree);
            else
                tree = RR(tree);
        }
        else
            return;
    }
}


template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::LL(Node* B)
{
    Node* A = B->left;
    B->left = A->right;
    A->right = B;

    B->height =1+ std::max(treeHeight(B->left),treeHeight(B->right));
    A->height =1+ std::max(treeHeight(A->left),treeHeight(A->right));
    return A;
}

template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::RR(Node* A)
{
    Node* B = A->right;
    A->right = B->left;
    B->left = A;

    A->height = 1+ std::max(treeHeight(A->left),treeHeight(A->right));
    B->height = 1+ std::max(treeHeight(B->left),treeHeight(B->right));
    return B;
}

template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::LR(Node* C)
{
    Node* A = C->left;
    Node* B = A->right;
    C->left = B->right;
    A->right = B->left;
    B->right = C;
    B->left = A;

    A->height = 1+ std::max(treeHeight(A->left),treeHeight(A->right));
    C->height = 1+ std::max(treeHeight(C->left),treeHeight(C->right));
    B->height = 1+ std::max(treeHeight(B->left),treeHeight(B->right));
    return B;
}

template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::RL(Node* A)
{
    Node* C = A->right;
    Node* B = C->left;
    A->right = B->left;
    C->left = B->right;
    B->right = C;
    B->left = A;
    
    A->height = 1+ std::max(treeHeight(A->left),treeHeight(A->right));
    C->height = 1+ std::max(treeHeight(C->left),treeHeight(C->right));
    B->height = 1+ std::max(treeHeight(B->left),treeHeight(B->right));
    return B;
}



template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{
    return lookup(element,root);
}

template <typename ElementType>
bool AVLSet<ElementType>::lookup(const ElementType& element,Node* tree)
{
    if(tree==nullptr)
        return false;
    else
    {
        if (tree->element == element)
            return true;
        else if (element > tree->element)
            return lookup(element,tree->right);
        else //(element < tree->element)
            return lookup(element, tree->left);
    }
}


template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    return treeSize(root);   
}

template <typename ElementType>
unsigned int AVLSet<ElementType>::treeSize(Node* tree) noexcept
{
    if(tree==nullptr)
        return 0;
    else
        return treeSize(tree->left)+treeSize(tree->right)+1;
}

template <typename ElementType>
int AVLSet<ElementType>::height() const
{
    return treeHeight(root);
}

template <typename ElementType>
int AVLSet<ElementType>::treeHeight(Node* tree)
{
    if(tree==nullptr)
        return -1;
    else
        return tree->height;

}

template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
    preorderTree(root,visit);
}

template <typename ElementType>
void AVLSet<ElementType>::preorderTree(Node* tree, VisitFunction visit)
{
    if(tree!=nullptr)
    {
        visit(tree->element);
        preorderTree(tree->left,visit);
        preorderTree(tree->right,visit);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
    inorderTree(root,visit);
}

template <typename ElementType>
void AVLSet<ElementType>::inorderTree(Node* tree, VisitFunction visit)
{
    if(tree!=nullptr)
    {
        inorderTree(tree->left,visit);
        visit(tree->element);
        inorderTree(tree->right,visit);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
    postorderTree(root,visit);
}

template <typename ElementType>
void AVLSet<ElementType>::postorderTree(Node* tree, VisitFunction visit)
{
    if(tree!=nullptr)
    {
        postorderTree(tree->left,visit);
        postorderTree(tree->right,visit);
        visit(tree->element);
    }
}
#endif // AVLSET_HPP

