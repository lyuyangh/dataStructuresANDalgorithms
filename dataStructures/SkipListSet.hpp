// SkipListSet.hpp
//
// A SkipListSet is an implementation of a Set that is a skip list, implemented
// Each node should contain only two pointers: one to the node that follows it
// on the same level and another to the equivalent node on the level below it.


#ifndef SKIPLISTSET_HPP
#define SKIPLISTSET_HPP

#include <memory>
#include <random>
#include "Set.hpp"

// SkipListKind indicates a kind of key: a normal one, the special key
// -INF, or the special key +INF.  It's necessary for us to implement
// the notion of -INF and +INF separately, since we're building a class
// template and not all types of keys would have a reasonable notion of
// -INF and +INF.

enum class SkipListKind
{
    Normal,
    NegInf,
    PosInf
};




// A SkipListKey represents a single key in a skip list.  It is possible
// to compare these keys using < or == operators (which are overloaded here)
// and those comparisons respect the notion of whether each key is normal,
// -INF, or +INF.

template <typename ElementType>
class SkipListKey
{
public:
    SkipListKey(SkipListKind kind, const ElementType& element);

    bool operator==(const SkipListKey& other) const;
    bool operator<(const SkipListKey& other) const;

private:
    SkipListKind kind;
    ElementType element;
};


template <typename ElementType>
SkipListKey<ElementType>::SkipListKey(SkipListKind kind, const ElementType& element)
    : kind{kind}, element{element}
{
}


template <typename ElementType>
bool SkipListKey<ElementType>::operator==(const SkipListKey& other) const
{
    return kind == other.kind
        && (kind != SkipListKind::Normal || element == other.element);
}


template <typename ElementType>
bool SkipListKey<ElementType>::operator<(const SkipListKey& other) const
{
    switch (kind)
    {
    case SkipListKind::NegInf:
        return other.kind != SkipListKind::NegInf;

    case SkipListKind::PosInf:
        return false;

    default: // SkipListKind::Normal
        return other.kind == SkipListKind::PosInf
            || (other.kind == SkipListKind::Normal && element < other.element);
    }
}



// The SkipListLevelTester class represents the ability to decide whether
// a key placed on one level of the skip list should also occupy the next
// level.  This is the "coin flip," so to speak.  Note that this is an
// abstract base class with one implementation, RandomSkipListLevelTester,
// just below it.  RandomSkipListLevelTester is what it sounds like: It
// makes the decision at random (with a 50/50 chance of deciding whether
// a key should occupy the next level). 
//

template <typename ElementType>
class SkipListLevelTester
{
public:
    virtual ~SkipListLevelTester() = default;

    virtual bool shouldOccupyNextLevel(const ElementType& element) = 0;
    virtual std::unique_ptr<SkipListLevelTester<ElementType>> clone() = 0;
};


template <typename ElementType>
class RandomSkipListLevelTester : public SkipListLevelTester<ElementType>
{
public:
    RandomSkipListLevelTester();
    virtual ~RandomSkipListLevelTester() = default;

    virtual bool shouldOccupyNextLevel(const ElementType& element) override;
    virtual std::unique_ptr<SkipListLevelTester<ElementType>> clone() override;

private:
    std::default_random_engine engine;
    std::bernoulli_distribution distribution;
};


template <typename ElementType>
RandomSkipListLevelTester<ElementType>::RandomSkipListLevelTester()
    : engine{std::random_device{}()}, distribution{0.5}
{
}


template <typename ElementType>
bool RandomSkipListLevelTester<ElementType>::shouldOccupyNextLevel(const ElementType& element)
{
    return distribution(engine);
}


template <typename ElementType>
std::unique_ptr<SkipListLevelTester<ElementType>> RandomSkipListLevelTester<ElementType>::clone()
{
    return std::unique_ptr<SkipListLevelTester<ElementType>>{
        new RandomSkipListLevelTester<ElementType>};
}




template <typename ElementType>
class SkipListSet : public Set<ElementType>
{
public:
    // Initializes an SkipListSet to be empty, with or without a
    // "level tester" object that will decide, whenever a "coin flip"
    // is needed, whether a key should occupy the next level above.
    SkipListSet();
    explicit SkipListSet(std::unique_ptr<SkipListLevelTester<ElementType>> levelTester);

    // Cleans up the SkipListSet so that it leaks no memory.
    virtual ~SkipListSet() noexcept;

    // Initializes a new SkipListSet to be a copy of an existing one.
    SkipListSet(const SkipListSet& s);

    // Initializes a new SkipListSet whose contents are moved from an
    // expiring one.
    SkipListSet(SkipListSet&& s) noexcept;

    // Assigns an existing SkipListSet into another.
    SkipListSet& operator=(const SkipListSet& s);

    // Assigns an expiring SkipListSet into another.
    SkipListSet& operator=(SkipListSet&& s) noexcept;

    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function runs in an expected time
    // of O(log n) (i.e., over the long run, we expect the average to be
    // O(log n)) with very high probability.
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in an expected time of O(log n)
    // (i.e., over the long run, we expect the average to be O(log n))
    // with very high probability.
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // levelCount() returns the number of levels in the skip list.
    unsigned int levelCount() const noexcept;


    // elementsOnLevel() returns the number of elements that are stored
    // on the given level of the skip list.  Level 0 is the bottom level;
    // level 1 is the one above level 0; and so on.  If the given level
    // doesn't exist, this function returns 0.  (Note that the -INF
    // and +INF shouldn't be counted.)
    unsigned int elementsOnLevel(unsigned int level) const noexcept;


    // isElementOnLevel() returns true if the given element is on the
    // given level, false otherwise.  Level 0 is the bottom level; level 1
    // is the one above level 0; and so on.  If the given level doesn't
    // exist, this function returns false.
    bool isElementOnLevel(const ElementType& element, unsigned int level) const;

private:
    struct Node
    {
        SkipListKey<ElementType> element;
        Node* next;
        Node* below;
    };

    
    static void deleteLinkedList(Node* head);
    static void deleteSkipList(Node* top);
        
    bool lookup(const ElementType& element, Node* skiplist);
    void insertNode(const ElementType& element,const Node* skipList);
private:
    std::unique_ptr<SkipListLevelTester<ElementType>> levelTester;
    Node* top;
    unsigned int height = 1;
};

template <typename ElementType>
SkipListSet<ElementType>::SkipListSet()
    : levelTester{std::make_unique<RandomSkipListLevelTester<ElementType>>()},
        top(new Node{SkipListKey(SkipListKind::NegInf,ElementType()),new Node{SkipListKey(SkipListKind::PosInf,ElementType()),nullptr,nullptr},nullptr}), height(1)
{
}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(std::unique_ptr<SkipListLevelTester<ElementType>> levelTester)
    : levelTester{std::move(levelTester)},top(new Node{SkipListKey(SkipListKind::NegInf,ElementType()),new Node{SkipListKey(SkipListKind::PosInf,ElementType()),nullptr,nullptr},nullptr}), height(1)
        
{
}


template <typename ElementType>
SkipListSet<ElementType>::~SkipListSet() noexcept
{
    deleteSkipList(top);
}

template <typename ElementType>
void SkipListSet<ElementType>::deleteSkipList(Node* top)
{
    while(top!=nullptr)
    {
        Node* temp = top;
        top = top->below;
        deleteLinkedList(temp);
    }
}

template <typename ElementType>
void SkipListSet<ElementType>::deleteLinkedList(Node* head)
{
    while(head!=nullptr)
    {
        Node* temp = head;
        head = head -> next;
        delete temp;
    }
}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(const SkipListSet& s)
    :levelTester{}, top(nullptr),height(s.height)
{
}

/*
template <typename ElementType>
void SkipListSet<ElementType>::copySkipList(Node*& top, const Node*& copyTop, unsigned height)
{
    //get an arr of level heads
    Node** headArr = new Node*[height];
    headArr[0] = copyTop;
    for(int i = 1;i<height;i++)
        headArr[i] = headArr[i-1]->below;

    //rebuild the skipList at top (bottom up)
    //for(int i=height-1; i>=0; i--)
}
*/        


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(SkipListSet&& s) noexcept
    :levelTester(std::move(s.levelTester)), top(s.top),height(s.height)
{
    s.top = nullptr;
}


template <typename ElementType>
SkipListSet<ElementType>& SkipListSet<ElementType>::operator=(const SkipListSet& s)
{
    return *this;
}


template <typename ElementType>
SkipListSet<ElementType>& SkipListSet<ElementType>::operator=(SkipListSet&& s) noexcept
{
    deleteSkipList(top);
    top = s.top;
    s.top = nullptr;
    //levelTester = s.levelTester;
    height = s.height;
    return *this;
}


template <typename ElementType>
bool SkipListSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void SkipListSet<ElementType>::add(const ElementType& element)
{
//    insertNode(element,top);    
}
/*
template <typename ElementType>
void SkipListSet<ElementType>::insertNode(const ElementType& element,Node* skipList)
{
    if(skipList->next->element < element)
        insertNode(element, skipList->next);
    else
    {
        if(skipList->below != nullptr)
            insertNode(element,skipList->below);
        else
        {
            skipList->next = new Node{element, skipList->next, nullptr};
            if(levelTester.shouldOccupyNextLevel())
                return;
        }
    }
}
*/


template <typename ElementType>
bool SkipListSet<ElementType>::contains(const ElementType& element) const
{
//    lookup(element, top);
    return false;
}

/*
template <typename ElementType>
bool SkipListSet<ElementType>::lookup(const ElementType& element, Node* skiplist)
{
    if(skiplist ==nullptr)
        return false;
    else if(skiplist->element == element)
        return true;
    else if(skiplist->next->element < element)
        lookup(element,skiplist->next);
    else
        lookup(element,skiplist->below);
}
*/

template <typename ElementType>
unsigned int SkipListSet<ElementType>::size() const noexcept
{/*
    Node* n = top;
    while(n->next!=nullptr)
       n = n->below;
    unsigned int sz = -1;
    while(n==nullptr)
    {
        sz++;
        n = n->next;
    }
    return sz-1;*/
    return 0;
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::levelCount() const noexcept
{
   return 1; 
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::elementsOnLevel(unsigned int level) const noexcept
{
    return 0;
}


template <typename ElementType>
bool SkipListSet<ElementType>::isElementOnLevel(const ElementType& element, unsigned int level) const
{
    return true;
}



#endif // SKIPLISTSET_HPP

