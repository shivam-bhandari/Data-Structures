#include "netflow.hpp"

#include <iostream>
#include <queue>
#include <iterator>
#include <map>
#include <algorithm>
using namespace std;

#define TOO_FEW_VERTICES "Too few vertices."
#define TOO_FEW_EDGES "Too few edges."
#define EDGE_WEIGHT_ZERO "Detected edge weight of 0."
#define EDGE_BAD_ENDPOINT "Edge interacts with nonexistent vertex."
#define SELF_LOOP "At least one self-loop."
#define MULTI_EDGES "Detected multi-edges."
#define NOT_ONE_SRC "Zero or more than one source."
#define NOT_ONE_SINK "Zero or more than one sink."




unsigned numSources = 0;
unsigned numSinks = 0;
unsigned source = 0;
unsigned sink = 0;

bool BFS(vector<vector<unsigned>> &ResidualGraph, vector<unsigned> &path)
{
    bool discovered[ResidualGraph.size()];
    for (size_t i = 0; i < ResidualGraph.size(); i++)
    {
        discovered[i] = false;
    }

    queue<unsigned> que;

    que.push(source);
    path[source] = 10000000; 
    discovered[source] = true;
     
    while(!que.empty())
        {
        unsigned b;
        b = que.front();
        que.pop();
        
        for(unsigned i = 0; i < ResidualGraph.size(); i++)
            {
            unsigned cap = ResidualGraph[b][i];

            if(!discovered[i] && cap != 0)
                {
                path[i] = b;             
                que.push(i);
                discovered[i] = true;
                }
            }
        }


    if(discovered[sink]) 
    { 
    return true; 
    }
    else
    {
    return false;
    }

}


std::vector<Edge> solveNetworkFlow(const std::vector<Edge>& flowNetwork, unsigned numVertices)
{
    if (numVertices < 2)
    {
        throw std::runtime_error(TOO_FEW_VERTICES);
    }
    if (flowNetwork.size() == 0)
    {
        throw std::runtime_error(TOO_FEW_EDGES);
    }

    vector<vector<unsigned>>CapacityGraph(numVertices,vector<unsigned>(numVertices, 0)); 

    for (size_t i = 0; i < flowNetwork.size(); i++)
    {
        if (flowNetwork[i].weight == 0)
        {
            throw std::runtime_error(EDGE_WEIGHT_ZERO);
        }
        
        if (flowNetwork[i].from >= numVertices || flowNetwork[i].to >= numVertices)
        {
            throw std::runtime_error(EDGE_BAD_ENDPOINT);
        }

        if (flowNetwork[i].from == flowNetwork[i].to)
        {
            throw std::runtime_error(SELF_LOOP);
        }
        
        if (CapacityGraph[flowNetwork[i].from][flowNetwork[i].to] != 0)
        {
            throw std::runtime_error(MULTI_EDGES);
        }
        CapacityGraph[flowNetwork[i].from][flowNetwork[i].to] = flowNetwork[i].weight;
    }
    
    bool flag = true;

    for (size_t i = 0; i < numVertices; i++)
    {
        flag = true;
        for (size_t j = 0; j < numVertices; j++)
        {
            if (CapacityGraph[i][j] != 0)
            {
                flag = false;
                break;
            }
        }
        if (flag)
        {          
            numSinks++;
            sink = i;  
        }
    }

    for (size_t m = 0; m < numVertices; m++)
    {
        flag = true;
        for (size_t n = 0; n < numVertices; n++)
        {
            if (CapacityGraph[n][m]!=0)
            {
                flag = false;
            }
        }
        if (flag)
        {
        numSources++;
        source = m;
        }
    
    }

    if (numSources!=1)
    {
        throw std::runtime_error(NOT_ONE_SRC);
    }
    if (numSinks!=1)
    {
        throw std::runtime_error(NOT_ONE_SINK);
    }
    
    vector<vector<unsigned>> ResidualGraph(CapacityGraph);
    vector<unsigned> path(numVertices, 1000000);
    
    vector<vector<unsigned>>FlowGraph(numVertices,vector<unsigned>(numVertices, 0));
    while(BFS(ResidualGraph, path))
    {
        unsigned bottleneck = 10000000;

        unsigned a = sink;
        // find bottleneck
        while(a != source)
            {
            unsigned b = path[a]; 

            unsigned cap = ResidualGraph[b][a];
            bottleneck = min(bottleneck, cap);
            
            a = b;
            }

        a = sink;
        // update bottlenecks in residual graph and flowGraph, both backward and forward edges
        while(a != source)
            {
            unsigned b = path[a]; 
                        
            ResidualGraph[b][a] -= bottleneck;
            ResidualGraph[a][b] += bottleneck;
            FlowGraph[b][a] += bottleneck;
    
            a = b;
            }
        
    }

    std::vector<Edge> result;
    for (size_t i = 0; i < numVertices; i++)
    {
        for (size_t j = 0; j < numVertices; j++)
        {
            if (CapacityGraph[i][j]!=0)
            {
                Edge e;
                e.from = i;
                e.to = j;
                e.weight = FlowGraph[i][j];
                result.push_back(e);
            }
            
        }
        
    }

    return result;
}

void assignCourses(
    std::vector<Instructor>& instructors,
    const std::vector<std::string>& courses)
{
    std::vector<Edge> BiGraph;
    unsigned nV = instructors.size() + courses.size() + 2; // source and sink implies +2
    map <string, unsigned> instructorsmap;
    map <string, unsigned> coursesmap;
    map<string, unsigned>::iterator itr;
    typedef vector::iterator IntIterator;
    size_t x;

    for (x = 0; x < instructors.size(); x++)
    {
        instructorsmap.insert(pair<string, unsigned>(instructors[x].lastName, x+1));
    }
    for (size_t j = 0; j < courses.size(); j++)
    {
        coursesmap.insert(pair<string, unsigned>(courses[j], x+1));
        x++;
    }

    for (size_t i = 0; i < instructors.size(); i++)
    {
        for (size_t j = 0; j < instructors[i].preferences.size(); j++)
        {
            Edge a;
            a.from = i+1;
            a.to = coursesmap.find(instructors[i].preferences[j])->second;
            a.weight = instructors[i].maxCourses;
            BiGraph.push_back(a);
        }
    Edge b;
    b.from = 0;
    b.to = instructorsmap.find(instructors[i].lastName)->second;
    b.weight = 1;
    BiGraph.push_back(b);
    
    }
 
    for (size_t i = instructors.size() + 1; i < courses.size() + instructors.size() + 1; i++)
    {
        Edge c;
        c.from = i;
        c.to = nV-1;
        c.weight = 1;
        BiGraph.push_back(c);
    }
    
    std::vector<Edge> result = solveNetworkFlow(BiGraph,nV);

    for (size_t i = 0; i < result.size(); i++)
    {
        if (result[i].weight>0 && result[i].from!=0 && result[i].to!=nV-1 )
        {
            for (auto& it1 : instructorsmap) 
            {
                if (it1.second == result[i].from) 
                {
                    for (auto& it2 : coursesmap) 
                    {
                        if (it2.second == result[i].to) 
                        {
                            IntIterator w = find(instructors.begin(), instructors.end(), it1.first);
                            instructors[IntIterator].assignedCourses.push_back(it2.first);
                        }
                    
                    }
                }
            
            }
        
        }
        
    }
    


}

    
    
