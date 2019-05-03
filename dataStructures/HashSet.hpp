// HashSet.hpp
//

// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8, the HashSet should be resized so
// that it is twice as large as it was before.
//

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"


template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    virtual ~HashSet() noexcept;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8.  In the case
    // where the array is resized, this function runs in linear time (with
    // respect to the number of elements, assuming a good hash function);
    // otherwise, it runs in constant time (again, assuming a good hash
    // function).
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns 0.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;

private:
    struct Node
    {
        ElementType element;
        Node* next;
    };

private:
    HashFunction hashFunction;
    Node** arr;
    unsigned int sz = 0;
    unsigned int capacity = DEFAULT_CAPACITY;
    void deleteTable();
};

namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction},arr{new Node*[DEFAULT_CAPACITY]}
{
    for(unsigned int i = 0; i<capacity; i++)
        arr[i] = nullptr;
}


template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
    deleteTable();
}

template <typename ElementType>
void HashSet<ElementType>::deleteTable()
{
    if(arr!=nullptr)
    {
        for (int i = 0; i<capacity;i++)
        {
            for(Node* n = arr[i];n!=nullptr;)
            {
                Node* temp = n;
                n = n->next;
                delete temp;
            }
        }
        delete[] arr;
    }
}

template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{s.hashFunction},
        arr{new Node*[s.capacity]},sz(s.sz),capacity(s.capacity)
{
    for(int i = 0; i<capacity; i++)
    {
        //first initialize all pointers to nullptr
        arr[i] = nullptr;
        //add all the elements
        for(Node* n = s.arr[i];n!=nullptr;n=n->next)
            arr[i] = new Node{n->element,arr[i]};
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashFunction{s.hashFunction},
        arr{s.arr},sz(s.sz),capacity(s.capacity)
{
    s.arr = nullptr;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
    deleteTable();
    
    hashFunction = s.hashFunction;
    capacity = s.capacity;
    sz = s.sz;
    arr = new Node* [capacity];


    for(int i = 0; i<capacity;i++)
    {
        arr[i] = nullptr;
        for(Node* n = s.arr[i]; n!=nullptr; n=n->next)
            arr[i] = new Node{n->element, arr[i]};
    }
    
    return *this;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    deleteTable();
    arr = s.arr;
    s.arr = nullptr;
    hashFunction = s.hashFunction;
    sz = s.sz;
    capacity = s.capacity;
    
    return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
    unsigned int position = hashFunction(element) % capacity;
    if(isElementAtIndex(element,position)==false)
    {
        arr[position] = new Node{element, arr[position]};
        sz++;
        
        if((float) sz/(float) capacity >= 0.8)
        {
            Node** new_arr = new Node*[capacity*2];
            capacity = capacity*2;
            for(int i=0;i<capacity;i++)
                new_arr[i] = nullptr;
            
            for(int i=0; i<capacity/2;i++)
            {   
                for(Node* n=arr[i]; n!=nullptr; )
                {
                    Node* temp = n;
                    unsigned int newPosition = hashFunction(n->element) % capacity;
                    new_arr[newPosition] = new Node{n->element,new_arr[newPosition]};
                    
                    n = n->next;
                    delete temp;
                }
                
            }
            delete[] arr;

                      
            arr = new_arr;
       }
    }

}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    unsigned int position = hashFunction(element) % capacity;

    return isElementAtIndex(element,position);
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    return sz;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
    if(index<0 || index>=capacity)
        return 0;
    int count = 0;
    for(Node* n = arr[index]; n!=nullptr;n=n->next)
        count++;
    return count;
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
    if(index<0 || index>=capacity)
        return false;
    for(Node* n = arr[index]; n!=nullptr;n=n->next)
        if(element == n->element)
            return true;
    return false;
}



#endif // HASHSET_HPP

