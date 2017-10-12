

#ifndef linkedListType_hpp
#define linkedListType_hpp

#include <iostream>
#include <assert.h>

template<class Type>
struct nodeType
{
    Type* info;
    nodeType<Type> *link;
};


template<class Type>
class linkedListType
{
public:
    
     linkedListType<Type>& operator=( linkedListType<Type>&);
    // Overload assignment operator
    
    void initializeList();
    // initialize the list to an empty state
    
    bool isEmptyList() ;
    
    void print() ;
    
    int length() ;
    
    void destroyList();
    
    Type front() ;
    
    Type back() ;
    
    bool search( Type& searchItem) ;
    
    void insertFirst( Type& newItem);
    
    void insertLast(  Type& newItem);
    
    void deleteNode(  Type& deleteItem);
    
    void getAdjacentVertices(Type adjacencyList[], int& length);

    linkedListType();
    
    linkedListType( linkedListType<Type>& otherList);
    
    ~linkedListType();
    
protected:
    int count;
    
    nodeType<Type> *first;
    nodeType<Type> *last;
    
private:
    void copyList( linkedListType<Type>& otherList);

};


template<class Type>
 linkedListType<Type>& linkedListType<Type>::operator=( linkedListType<Type>& otherList)
{
    if (this != &otherList)
    {
        copyList(otherList);
    }
    
    return *this;
}
template<class Type>
bool linkedListType<Type>::isEmptyList() 
{
    return (first == NULL);
}

template<class Type>
linkedListType<Type>::linkedListType() //default ructor
{
    first   = NULL;
    last    = NULL;
    count   = 0;
}


template<class Type>
void linkedListType<Type>::destroyList()
{
    nodeType<Type> *temp;
    while (first != NULL)
    {
        temp = first;
        first = first->link;
        delete temp;
    }
    last = NULL;
    count = 0;
}

template<class Type>
void linkedListType<Type>::initializeList()
{
    destroyList();
}
template <class Type>
void linkedListType<Type>::print() 
{
    nodeType<Type> *current;
    
    current = first;
    
    while (current != NULL)
    {
        std::cout << current->info  << " ";
        current = current->link;
        
    }
    std::cout << "\n\n";
}

template <class Type>
int linkedListType<Type>::length() 
{
    return count;
}

template <class Type>
Type linkedListType<Type>::front() 
{
    assert(first != NULL);
    return *first->info;
    
}

template <class Type>
Type linkedListType<Type>::back() 
{
    assert(last != NULL);
    return *last->info;
    
}

template <class Type>
bool linkedListType<Type>::search( Type& searchItem)
{
    nodeType<Type> *current;
    bool found = false;
    
    current = first;
    
    while (current != NULL && !found)
        if (current->info == searchItem->info)
            found = true;
        else
            current = current->link;
    
    return found;
}
template <class Type>
void linkedListType<Type>::insertFirst(  Type& newItem)
{
    nodeType<Type> *newNode;
    
    newNode = new nodeType<Type>;
    
    assert(newNode != NULL);
    
    newNode->info = &newItem;
    newNode->link = first;
    first = newNode;
    
    count++;
    
    if (last == NULL)
        last = newNode;
    
}

template<class Type>
void linkedListType<Type>::insertLast( Type& newItem)
{
    nodeType<Type> *newNode;
    
    newNode = new nodeType<Type>;
    
    assert(newNode != NULL);
    
    newNode->info = &newItem;
    newNode->link = NULL;
    
    if (first == NULL)
    {
        first = newNode;
        last = newNode;
        this->count++;
    }
    else
    {
        last->link = newNode;
        last = newNode;
        count++;
    }
    
}
template<class Type>
void linkedListType<Type>::deleteNode( Type& deleteItem)
{
    nodeType<Type> *current;
    nodeType<Type> *trailCurrent;
    bool found;
    
    
    if (first == NULL)
    {
        std::cout << "Cannot delete from an empty list." << std::endl;
    }
    
    else
    {
        if (first->info == deleteItem)
        {
            current = first;
            first = first->link;
            count--;
            if (first == NULL)
                last = NULL;
            delete current;
        }
        else
        {
            found = false;
            trailCurrent = first;
            
            current = first->link;
            
            while (current != NULL && !found)
            {
                if (current->info != deleteItem)
                {
                    trailCurrent = current;
                    current = current->link;
                }
                else
                    found = true;
            }
            
            if (found)
            {
                trailCurrent->link = current->link;
                count--;
                
                if(last == current)
                    last = trailCurrent;
                
                delete current;
            }
            else
                std::cout << "The item to be deleted is not in the list" << std::endl;
        }
    }
}



template <class Type>
void linkedListType<Type>::copyList( linkedListType<Type>& otherList)
{
    nodeType<Type> *newNode;
    nodeType<Type> *current;
    
    if (first != NULL)
    {
        destroyList();
    }
    
    if (otherList.first == NULL)
    {
        first = NULL;
        last = NULL;
        count = 0;
        
    }
    
    else
    {
        
        current = otherList.first;
        
        count = otherList.count;
        
        first = new nodeType<Type>;
        
        assert(first != NULL);
        
        first->info = current->info;
        
        first->link = NULL;
        
        current = current->link;
        
        while( current != NULL)
        {
            newNode = new nodeType<Type>;
            
            assert(newNode != NULL);
            
            newNode->info = current->info;
            newNode->link = NULL;
            
            last->link = newNode;
            last = newNode;
            
            current = current->link;
        }
    }
}


template<class Type>
linkedListType<Type>::~linkedListType()
{
    destroyList();
}


template<class Type>
linkedListType<Type>::linkedListType( linkedListType<Type>& otherList)
{
    first = NULL;
    copyList(otherList);
}

template<class Type>
void linkedListType<Type>::getAdjacentVertices(Type adjacencyList[], int &length)
{
    nodeType<Type> *current;
    
    length = 0;
    current = this->first;
    
    while (current != NULL)
    {
        adjacencyList[length++] = current->info;
        current = current->link;
    }
}


#endif /* linkedListType_hpp */
