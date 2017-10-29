
#ifndef graphType_hpp
#define graphType_hpp

#include <cstdio>
#include <list>
#include <vector>

#include "assignment.h"

const double infinity = 1000000000;

class Vertex
{
public:
    typedef std::vector<Vertex> NodeVec;
    typedef std::pair<Vertex, Vertex> IO_Nodes;
    typedef std::vector<IO_Nodes > AdjacencyNodes;

    Vertex() : pA(0), nodeNum(-1) {}
    Vertex(Assignment &a, int n) : pA(&a), nodeNum(n) {}
    ~Vertex() { }

    void createDirectedEdges()
    {
        NodeVec inputNodes = findInputNodes();
        NodeVec outputNodes = findOutputNodes();

        for (auto i = inputNodes.begin(); i != inputNodes.end(); i++)
        {
            for (auto j = outputNodes.begin(); j != outputNodes.end(); j++)
            {
                auto nodePair = std::make_pair(*i, *j);
                adjacentNodes.push_back(nodePair);
            }
        }
    }

    void printInfo()
    {
        std::cout << "Vertex: " << nodeNum << std::endl;
        std::cout << "        adjacency paths:" << std::endl;
        for (auto i = adjacentNodes.begin(); i != adjacentNodes.end(); i++)
        {
            std::cout << "( " << std::get<0>(*i).nodeNum << ", " << std::get<1>(*i).nodeNum << " )" << std::endl;
        }
        std::cout << std::endl;
    }

    AdjacencyNodes getAdjacencyList() const { return adjacentNodes; }
    int getNodeNumber() const { return nodeNum; }

private:
    NodeVec findInputNodes()
    {
        Assignments tmpNodeList = Assignments::instance();
        NodeVec inputNodes;

        if (pA)
        {
            if (pA->getInput1().ioClass() == IOClass::INPUT ||
                pA->getInput2().ioClass() == IOClass::INPUT ||
                pA->getInput3().ioClass() == IOClass::INPUT)
            {
                Vertex inputNOP = Vertex(); // creating a dummy vertex using default constructor
                inputNodes.push_back(inputNOP);
            }
           
            for (Assignments::iterator_t i = tmpNodeList.begin(); i != tmpNodeList.end(); i++)
            { 
                if (nodeNum != std::distance(tmpNodeList.begin(),i))
                {
                    if (pA->getInput1().name() == tmpNodeList[i].getResult().name() ||
			pA->getInput2().name() == tmpNodeList[i].getResult().name() ||
			pA->getInput3().name() == tmpNodeList[i].getResult().name())
                    {
                        inputNodes.push_back(Vertex(tmpNodeList[i], std::distance(tmpNodeList.begin(), i)));
                    }
                }
                else 
                {
                    std::cout << "Saying Hello to myself" << std::endl;
                }
             }
        }
        return inputNodes;
    }

    NodeVec findOutputNodes()
    {
        Assignments tmpNodeList = Assignments::instance();
        NodeVec outputNodes;

        if (pA)
        {
            if (pA->getResult().ioClass() == IOClass::OUTPUT)
            {
                Vertex outputNOP = Vertex(); // creating a dummy vertex using default constructor
                outputNodes.push_back(outputNOP);
            }

            for (Assignments::iterator_t i = tmpNodeList.begin(); i != tmpNodeList.end(); i++)
            {
                if (nodeNum != std::distance(tmpNodeList.begin(),i))
                {
                    if (tmpNodeList[i].getInput1().name() == pA->getResult().name() ||
                        tmpNodeList[i].getInput2().name() == pA->getResult().name() ||
                        tmpNodeList[i].getInput3().name() == pA->getResult().name())
                    {
                        outputNodes.push_back(Vertex(tmpNodeList[i], std::distance(tmpNodeList.begin(),i)));
                    }
                }
                else 
                {
                    std::cout << "Saying Hello to myself again" << std::endl;
                }
            }
        }
        return outputNodes;
    }

    Assignment *pA;
    int nodeNum;

    // Create a vector of paired nodes representing an input node (1st node), and
    // and an output node (2nd node).
    AdjacencyNodes adjacentNodes;
};

template<class vType, int size>
class graphType
{
    
public:
    
    void clearGraph();
    
    void printGraph();
    
    void createWeightedGraph();
    
    void topologicalSort();
    
    void TSvisit(Vertex &v);
    
    void Reg2RegLongestPath();
    
    void longestPath(vType vertex);
    
    void printLongestPath(vType vertex, bool printPath);
    
    graphType();
    
    ~graphType();
    
protected:
    enum VisitationColor
    {
        WHITE = -1, // Not yet visited
        GRAY,       // vertex visited, but edges not exhausted
        BLACK       // vertex and edges visited
    };

    std::list<Vertex> graph;
    std::list<int> topoSortGraph; // Just keeping track of node numbers and sorting off that

    // creating a map between nodes and visitation status.
    // -- normally I'd try to put visit status in the Vertex class, but then I'd have to
    //    keep track of lists of lists of pointers to make sure the right vertex objects
    //    aren't mixed up. What a headache though--keeping a separate scratchpad instead
    std::map<int, VisitationColor> topoNodeVisits;

};

template <class vType, int size>
void graphType<vType, size>::createWeightedGraph()
{
    Assignments unsortedAssignments = Assignments::instance();
	
    // DEBUG ONLY -- DISPLAY THE NODES FOR REFERENCE
    int n=0;
    for (Assignments::iterator_t i = unsortedAssignments.begin(); i != unsortedAssignments.end(); i++,n++)
    {
      std::cout << "Node " << n << ": " << *i << std::endl;
    }

    for (Assignments::iterator_t i = unsortedAssignments.begin(); i != unsortedAssignments.end(); i++)
    {
        // each assignment is a new node
        // TODO what if this isn't true? for example
        // t1 = a + b
        // t1 = b + c
        // ^ this shouldn't happen in verilog unless something like a blocking assignment is used.
        // also, wtf would this happen?
        // For now assume only non-blocking assignments.
        
        Vertex newVertex(unsortedAssignments[i], std::distance(unsortedAssignments.begin(), i));
        newVertex.createDirectedEdges();
        graph.push_back(newVertex);
    }
}

template <class vType, int size>
void graphType<vType, size>::TSvisit(Vertex &v)
{
    topoNodeVisits[v.getNodeNumber()] = GRAY;
    Vertex::AdjacencyNodes adjList = v.getAdjacencyList();
    for (auto n = adjList.begin(); n != adjList.end(); n++)
    {
        Vertex outNode = std::get<0>(*n);
        
        if (topoNodeVisits[outNode.getNodeNumber()] == WHITE && outNode.getNodeNumber() != -1)
        {
            TSvisit(outNode);
        }
    }

    topoNodeVisits[v.getNodeNumber()] = BLACK;
    topoSortGraph.push_back(v.getNodeNumber());
}

template <class vType, int size>
void graphType<vType, size>::topologicalSort()
{
    // Initialize topoNodeVisits map by making all vertices WHITE
    for (auto vertex = graph.begin(); vertex != graph.end(); vertex++)
    {
        topoNodeVisits[vertex->getNodeNumber()] = WHITE;
    }

    for (auto vertex = graph.begin(); vertex != graph.end(); vertex++)
    {
        if (topoNodeVisits[vertex->getNodeNumber()] == WHITE)
        {
            TSvisit(*vertex);
        }
    }
}

template <class vType, int size>
void graphType<vType,size>::clearGraph()
{
    graph.clear();
}

template <class vType, int size>
void graphType<vType, size>::printGraph() 
{
    int index = 0;
    std::cout << "=====================================================================" << std::endl;

    for (auto i = graph.begin(); i != graph.end(); i++)
    {
        std::cout << "Graph Index: " << index++ << std::endl;
        i->printInfo();
        std::cout << "--------------------------------------------------" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "=====================================================================" << std::endl;
    
    std::cout << "Topo Sort List: " << std::endl;
    std::cout << "      INOP -> ";
    for (auto i = topoSortGraph.begin(); i != topoSortGraph.end(); i++)
    {
        std::cout << *i << " -> ";
    }
    std::cout << "ONOP" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << std::endl;    
}

template <class vType, int size>
void graphType<vType,size>::printLongestPath(vType vertex,bool printPath)
{    
}

template <class vType, int size>
void graphType<vType, size>::longestPath(vType vertex)
{    
}

template <class vType, int size>
graphType<vType, size>::graphType()
{
}

template <class vType, int size>
graphType<vType,size>::~graphType()
{
    clearGraph();
}

#endif /* graphType_hpp */
