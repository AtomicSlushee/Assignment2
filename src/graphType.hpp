

#ifndef graphType_hpp
#define graphType_hpp

#include <stdio.h>
#include "linkedListType.hpp"

/*
 From the .cpp file
 #include "graphType.hpp"
 #include <iostream>
 #include <fstream>
 #include <vector>
 */
template<class vType>
class linkedListGraph: public linkedListType<vType>
{
public:
    void getAdjacentVertices(vType adjacencyList[], int& length);
};

template <class vType>
void linkedListGraph<vType>::getAdjacentVertices(vType adjacencyList[], int& length)
{
    nodeType<vType> *current;
    length = 0;
    current = this->first;
    
    while (current != NULL)
    {
        adjacencyList[length++] = current->info;
        current = current->link;
    }
}

const double infinity = 1000000000;

template<class vType, int size>
class graphType
{
    
public:
    bool isEmpty() const;
    
    void clearGraph();
    
    void printGraph() const;
    
    void createWeightedGraph(std::vector<vType> nodeList);
    
    void Reg2RegTopologicalSort(vType vertex);
    
    void Reg2RegTSvisit(vType vertex);
    
    void Reg2RegLongestPath();
    
    void longestPath(vType vertex);
    
    void printLongestPath(vType vertex, bool printPath);
    
    graphType();
    
    ~graphType();
    
protected:
    int maxSize;
    int gSize;
    double weights[size][size];
    double smallestWeight[size];
    linkedListGraph<vType> graph[size];

    
};


//
template <class vType, int size>
bool graphType<vType,size>::isEmpty() const
{
    return(gSize == 0);
    
}

template <class vType, int size>
void graphType<vType, size>::createWeightedGraph(std::vector<vType> nodeList)
{
  
    int nodeIdx = 0;
    if (gSize != 0)
    {
        clearGraph();
    }
    typename std::vector<vType>::iterator it; // credit where credit is due
    // https://stackoverflow.com/questions/11275444/c-template-typename-iterator
    for (it = nodeList.begin(); it != nodeList.end(); it++)
    {
        //vType vertex = it;
        graph[nodeIdx].initializeList();
        graph[nodeIdx].insertLast(*it);
    }
//    for (index = 0; index < gSize; index++)
//    { // read in the coordinates from the file and place nodes in graph
//        node *vertex = new node;
//        vertex->info = index;
//        
//        m_graph[index].initializeList();
//        m_graph[index].insertLast(*vertex);
//        
//        
//    }
//
//    for (index = 0; index < gSize; index++)
//    {
//        float tempX = m_graph[index].first->x;
//        float tempY = m_graph[index].first->y;
//        float distance = 0;
//        // for each node check for which nodes are in range using pythagorean theorem.
//        for (int otherNodeIdx = 0; otherNodeIdx < gSize; otherNodeIdx++)
//        {
//            distance = sqrt(pow(m_graph[otherNodeIdx].first->x - tempX,2) + pow(m_graph[otherNodeIdx].first->y - tempY,2));
//            if (distance < 20.000 && otherNodeIdx != index)
//            {
//                
//                if (distance < 1)
//                    distance  = 1; // per instruction sheet
//                
//                // in range, so add node
//                // but not the same node. Just one that has same first layer.
//                
//                // IF shortest path starts breaking, check to see if it's the fact that the nodes are not really connected. It's only useful for printing.
//                node *tempNode = new node;
//                tempNode->info = m_graph[otherNodeIdx].first->info;
//                tempNode->link = NULL;
//                tempNode->x = m_graph[otherNodeIdx].first->x;
//                tempNode->y = m_graph[otherNodeIdx].first->y;
//                tempNode->power = pow(distance,2);
//                //                m_graph[index].insertLast(*m_graph[otherNodeIdx].first);
//                m_graph[index].insertLast(*tempNode);
//                weights[index][otherNodeIdx] =  pow(distance,2);
//                
//            }
//            else if (index == otherNodeIdx)
//            {
//                weights[index][otherNodeIdx] = 0;
//            }
//            else
//            {
//                weights[index][otherNodeIdx] = infinity;
//            }
//        }
//        
//    }
//    
//    
//    
//    infile.close();
}

template <class vType, int size>
void graphType<vType,size>::clearGraph()
{
    int index;
    for (index = 0; index < gSize; index++)
    {
        this->graph[index].destroyList();
    }
    gSize = 0;
}

template <class vType, int size>
void graphType<vType, size>::printGraph() const
{
    int index;
    
    for (index = 0; index < gSize; index++)
    {
        std::cout << index << " ";
        this->m_graph[index].print();
        std::cout << std::endl;
    }
    
    std::cout << std::endl;
    
}
template <class vType, int size>
void graphType<vType,size>::printLongestPath(vType vertex,bool printPath)
{
 
    // stub for now.
    
}

template <class vType, int size>
void graphType<vType, size>::longestPath(vType vertex)
{
    
    // stub
    // The following does some shortest path analysis. Probably useful for longest path
//    int i,j;
//    int v = 0;
//    double minWeight;
//    
//    for (j = 0; j < this->gSize; j++)
//    {
//        smallestWeight[j] = weights[vertex.info][j];
//    }
//    
//    bool weightFound[maxSize];
//    for (j = 0; j < this->gSize; j++)
//    {
//        weightFound[j] = false;
//        graph[j].first->predecessor = vertex.info;
//    }
//    
//    weightFound[vertex.info] = true;
//    smallestWeight[vertex.info] = 0;
//    
//    for (i = 0; i < this->gSize - 1; i++)
//    {
//        minWeight = infinity;
//        
//        for (j = 0; j < this->gSize; j++)
//        {
//            if(!weightFound[j])
//            {
//                if (smallestWeight[j] < minWeight)
//                {
//                    v = j;
//                    minWeight = smallestWeight[v];
//                    
//                    
//                }
//            }
//            
//        }
//        
//        weightFound[v] = true;
//        
//        for (j = 0; j < this->gSize; j++)
//        {
//            if (!weightFound[j])
//            {
//                if (minWeight + weights[v][j] < smallestWeight[j])
//                {
//                    smallestWeight[j] = minWeight + weights[v][j];
//                    graph[j].first->predecessor = v;
//                    
//                }
//            }
//        }
//    }
//    /* This section prints the weights*/
//    for (int index = 0; index < gSize; index++)
//    {
//        std::cout << "node" << index + 1 << std::endl;
//        for (int j = 0; j < gSize; j++)
//            std::cout <<"node:" << j + 1 << "\t" << weights[index][j] << std::endl;
//        
//        std::cout << std::endl;
//    }
    
    
}




template <class vType, int size>
graphType<vType, size>::graphType()
{
    maxSize = size;
    gSize = 0;
}
template <class vType, int size>
graphType<vType,size>::~graphType()
{
    clearGraph();
}





#endif /* graphType_hpp */
