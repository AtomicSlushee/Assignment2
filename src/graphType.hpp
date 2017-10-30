
#ifndef graphType_hpp
#define graphType_hpp

#include <cstdio>
#include <list>
#include <vector>
#include <memory>

#include "assignment.h"

const double infinity = 1000000000;

#define DEBUG_PRINTS 1
//#define DEBUG_PRINTS 0

class Vertex
{
public:
    typedef std::vector<Vertex> NodeVec;
    typedef std::pair<Vertex, Vertex> IO_Nodes;
    typedef std::vector<IO_Nodes > AdjacencyNodes;

    Vertex() : pA(nullptr), nodeNum(-1) {}
    Vertex(std::shared_ptr<Assignment> a_ptr, int n) : nodeNum(n) 
    {
        pA = a_ptr;
        std::cout << "Constructing Vertex node " << nodeNum << "  pA = " << pA << "  latency = " << pA->getLatency() << std::endl;
    }
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
    double getNodeWeight() const 
    { 
        std::cout << "Calling getNodeWeight " << nodeNum << "  pA = " << pA << "  latency = " << pA->getLatency() << std::endl;
        return pA->getLatency(); 
    }

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
                        std::shared_ptr<Assignment> pAssignment = std::make_shared<Assignment>(tmpNodeList[i]);
                        inputNodes.push_back(Vertex(pAssignment, std::distance(tmpNodeList.begin(), i)));
                    }
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
			std::shared_ptr<Assignment> pAssignment = std::make_shared<Assignment>(tmpNodeList[i]);
                        outputNodes.push_back(Vertex(pAssignment, std::distance(tmpNodeList.begin(),i)));
                    }
                }
            }
        }
        return outputNodes;
    }

    std::shared_ptr<Assignment> pA;
    int nodeNum;

    // Create a vector of paired nodes representing an input node 
    // (1st element in pair), and an output node (2nd element in pair).
    AdjacencyNodes adjacentNodes;
};

class graphType
{
    
public:
    
    void clearGraph();
    
    void printGraph();
    
    void createWeightedGraph();
    
    void topologicalSort();
    
    void TSvisit(Vertex &v);
    
    void Reg2RegLongestPath();
    
    void longestPath();
    
    void printLongestPath();
    
    graphType();
 
    ~graphType();
    
protected:
    enum VisitationColor
    {
        WHITE = -1, // Not yet visited
        GRAY,       // vertex visited, but edges not exhausted
        BLACK       // vertex and edges visited
    };

    std::vector<Vertex> graph;
    std::list<int> topoSortGraph; // Just keeping track of node numbers and sorting off that

    // creating a map between nodes and visitation status.
    std::map<int, VisitationColor> topoNodeVisits;
    
    // creating a map of nodes and cumalative distances to find longest path.
    std::map<int, double> cumalativeSumMap; // (nodeNum, cumSum)

    // creating another map to keep track of the input Node having the largest
    // contribution to the current node's cumalative sum
    std::map<int, int> largestPredNodeMap;   // (nodeNum, maxPredecessorNode)

};

void graphType::createWeightedGraph()
{
    Assignments unsortedAssignments = Assignments::instance();
	
    // DEBUG ONLY -- DISPLAY THE NODES FOR REFERENCE
#if(DEBUG_PRINTS)
    int n=0;
    for (Assignments::iterator_t i = unsortedAssignments.begin(); i != unsortedAssignments.end(); i++,n++)
    {
      std::cout << "Node " << n << ": " << *i << std::endl;
    }
#endif

    for (Assignments::iterator_t i = unsortedAssignments.begin(); i != unsortedAssignments.end(); i++)
    {
        // each assignment is a new node
        // TODO what if this isn't true? for example
        // t1 = a + b
        // t1 = b + c
        // ^ this shouldn't happen in verilog unless something like a blocking assignment is used.
        // For now assume only non-blocking assignments.
        std::shared_ptr<Assignment> pAssignment = std::make_shared<Assignment>(unsortedAssignments[i]);
        Vertex newVertex(pAssignment, std::distance(unsortedAssignments.begin(), i));
        newVertex.createDirectedEdges();
        graph.push_back(newVertex);
    }
    
}

void graphType::TSvisit(Vertex &v)
{
    topoNodeVisits[v.getNodeNumber()] = GRAY;
    Vertex::AdjacencyNodes adjList = v.getAdjacencyList();
    for (auto n = adjList.begin(); n != adjList.end(); n++)
    {
        Vertex inNode = std::get<0>(*n);
        
        if (topoNodeVisits[inNode.getNodeNumber()] == WHITE && inNode.getNodeNumber() != -1)
        {
            TSvisit(inNode);
        }
    }

    topoNodeVisits[v.getNodeNumber()] = BLACK;
    topoSortGraph.push_back(v.getNodeNumber());
}

void graphType::topologicalSort()
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

void graphType::longestPath()
{
    // Initialize cumalative sum map with vertices' own weights
    for (auto vertex = graph.begin(); vertex != graph.end(); vertex++)
    {
        cumalativeSumMap[vertex->getNodeNumber()] = vertex->getNodeWeight();
        largestPredNodeMap[vertex->getNodeNumber()] = -1; 
    }

    for (auto i = cumalativeSumMap.begin(); i != cumalativeSumMap.end(); i++)
    {
        std::cout << "cumSumInit " << std::get<1>(*i) << std::endl;
    }

    for (auto vi = topoSortGraph.begin(); vi != topoSortGraph.end(); vi++)
    {   
        // Cycle through the input nodes to find largest contributor to cumalative latency
        std::cout << "                     vertex " << *vi << std::endl;
        Vertex::AdjacencyNodes adjList = graph[*vi].getAdjacencyList();
        for (auto n = adjList.begin(); n != adjList.end(); n++)
        {
            Vertex inNode = std::get<0>(*n);
            
            if ( inNode.getNodeNumber() != -1 )
            {
                int iNodeNum = inNode.getNodeNumber();
                std::cout << "                      inode " << iNodeNum << "   cumSum " << cumalativeSumMap[iNodeNum] << std::endl;
                if ( (cumalativeSumMap[iNodeNum] + graph[*vi].getNodeWeight()) > cumalativeSumMap[graph[*vi].getNodeNumber()] )
                {
                    cumalativeSumMap[graph[*vi].getNodeNumber()] = cumalativeSumMap[iNodeNum] + graph[*vi].getNodeWeight();
                    largestPredNodeMap[graph[*vi].getNodeNumber()] = iNodeNum;
                }
                std::cout << "                            maxInNode " << iNodeNum << "   maxCumSum "  << cumalativeSumMap[iNodeNum] << std::endl;
            }
        }
    }

#if(DEBUG_PRINTS)    
    std::cout << "============================================================="<< std::endl;
    std::cout << "Cumalative Sum Map - (node, cumSum)"<< std::endl;
    double maxCumWeight = 0;
    for (auto n = cumalativeSumMap.begin(); n != cumalativeSumMap.end(); n++)
    {
        std::cout << "        ( " << std::get<0>(*n) << ", " << std::get<1>(*n) << " )" << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Map to Largest Predecessor - (node, maxPredNode)" << std::endl;
    for (auto n = largestPredNodeMap.begin(); n != largestPredNodeMap.end(); n++)
    {
        std::cout << "        ( " << std::get<0>(*n) << ", " << std::get<1>(*n) << " )" << std::endl;
    }
#endif
}

void graphType::clearGraph()
{
    graph.clear();
}

void graphType::printGraph() 
{
    int index = 0;
    std::cout << "=====================================================================" << std::endl;
    std::cout << "adjacency paths => (inputNode, outputNode); -1 => NOP" << std::endl;
    std::cout << std::endl;

    for (auto gi = graph.begin(); gi != graph.end(); gi++)
    {
        std::cout << "Graph Index: " << index++ << std::endl;
        gi->printInfo();
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
}

void graphType::printLongestPath()
{
    int maxNode = 0;
    double maxSum = 0.0;
    for (auto mapPair = cumalativeSumMap.begin(); mapPair != cumalativeSumMap.end(); mapPair++)
    {
        if (std::get<1>(*mapPair) > maxSum)
        {
            maxNode = std::get<0>(*mapPair);
            maxSum = std::get<1>(*mapPair);
        }
    }
    
    std::list<int> longestPathSeq;
    int predNode = maxNode;
    while ( predNode != -1)
    {
        longestPathSeq.push_front(predNode);
        predNode = largestPredNodeMap[predNode]; 
    }

    std::cout << std::endl;
    std::cout << "========================================================================" << std::endl;
    std::cout << "Critical Path: " << std::endl;
    std::cout << "      INOP -> ";
    for (auto i = longestPathSeq.begin(); i != longestPathSeq.end(); i++)
    {
        std::cout << *i << " -> ";
    }
    std::cout << " ONOP" << std::endl;
    std::cout << std::endl;
    std::cout << "      Latency = " << maxSum << " ns" << std::endl;
    std::cout << "------------------------------------------------------------------------" << std::endl;
    std::cout << std::endl;
}

graphType::graphType()
{
}

graphType::~graphType()
{
    clearGraph();
}

#endif /* graphType_hpp */
