

#ifndef graphType_hpp
#define graphType_hpp

#include <cstdio>
#include <list>
#include "linkedListType.hpp"

#include "assignment.h" // couldn't template all of it

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
    
    void createWeightedGraph(std::list<Assignment> nodeList); // can't template this one
    
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
    // TODO edge weights?
    linkedListGraph<vType> graph[size];
    
};

template <class vType, int size>
bool graphType<vType,size>::isEmpty() const
{
    return(gSize == 0);
    
}

template <class vType, int size>
void graphType<vType, size>::createWeightedGraph(std::list<Assignment> nodeList)
{
  
    int nodeIdx = 0; // for adding nodes to the graph
    if (gSize != 0)
    {
        clearGraph();
    }
    for (Assignments::iterator_t i = Assignments::instance().begin(); i != Assignments::instance().end(); i++)
    {
        // each assignment is a new node
        // TODO what if this isn't true? for example
        // t1 = a + b
        // t1 = b + c
        // ^ this shouldn't happen in verilog unless something like a blocking assignment is used.
        // also, wtf would this happen?
        // For now assume only non-blocking assignments.
        
        graph[nodeIdx].initializeList(); // clears it out for this node
        
        graph[nodeIdx].insertLast(Assignments::instance().assignment(i)); // adds node to the graph
        
        // Now, check to see how this node connects to the existing nodes (if any)
        for (int tempNodeIdx = 0; tempNodeIdx < nodeList.size(); tempNodeIdx++)
        {
            // check every node in the list to see if any of the outputs match our current inputs
            // this is a simple way to check for dependencies... perhaps too simple?

            // Todo: if the input to current node are outputs of other nodes then add current node
            //       to list of other nodes
            if ( graph[tempNodeIdx].length() > 0 &&
                (Assignments::instance().assignment(i).getInput1().name() == graph[tempNodeIdx].front().getResult().name()  ||
                Assignments::instance().assignment(i).getInput2().name() == graph[tempNodeIdx].front().getResult().name() ||
                Assignments::instance().assignment(i).getInput3().name() == graph[tempNodeIdx].front().getResult().name() ))
            {
                std::cout << " Node " << nodeIdx << " depends on Node " << tempNodeIdx << std::endl;
                graph[tempNodeIdx].insertLast(Assignments::instance().assignment(i));
                 // todo missing some nodes?
                // todo update the weight
                 weights[nodeIdx][tempNodeIdx] = Assignments::instance().assignment(i).getLatency() + graph[tempNodeIdx].front().getLatency();
                weights[tempNodeIdx][nodeIdx] = weights[nodeIdx][tempNodeIdx]; // symmetric matrix? eh it'll do for now
            }
            else if (nodeIdx == tempNodeIdx)
            {
                weights[nodeIdx][tempNodeIdx] = 0; // distance to itself is zero   
            }
            else
            {
                
                weights[nodeIdx][tempNodeIdx] = infinity; // no connection infinite weight
            }
            
        }
        
        
        nodeIdx++;
        //
    }


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
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            weights[i][j] = infinity;
        }
    }
}
template <class vType, int size>
graphType<vType,size>::~graphType()
{
    clearGraph();
}





#endif /* graphType_hpp */
