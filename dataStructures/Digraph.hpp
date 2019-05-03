// Digraph.hpp
//
// 
// An implementation of a generic directed graph. 
// uses the adjacency lists technique, so each vertex stores a linked
// list of its outgoing edges.
//
// Along with the Digraph class template is a class DigraphException
// and a couple of utility structs that aren't generally useful outside
// of this header file.
//

/*
 * Constriants for template parameters:
 *  > need to have copy assignment, copy constructor
 *  > can be referenced by a const reference.
 */

#ifndef DIGRAPH_HPP
#define DIGRAPH_HPP

#include <exception>
#include <functional>
#include <list>
#include <map>
#include <utility>
#include <vector>
#include <iostream>
#include <limits>
#include <queue>

// DigraphExceptions are thrown from some of the member functions in the
// Digraph class template, so that exception is declared here, so it
// will be available to any code that includes this header file.

class DigraphException : public std::runtime_error
{
public:
    DigraphException(const std::string& reason);
};


inline DigraphException::DigraphException(const std::string& reason)
    : std::runtime_error{reason}
{
}



// A DigraphEdge lists a "from vertex" (the number of the vertex from which
// the edge points), a "to vertex" (the number of the vertex to which the
// edge points), and an EdgeInfo object.  Because different kinds of Digraphs
// store different kinds of edge information, DigraphEdge is a struct template.

template <typename EdgeInfo>
struct DigraphEdge
{
    int fromVertex;
    int toVertex;
    EdgeInfo einfo;
};



// A DigraphVertex includes two things: a VertexInfo object and a list of
// its outgoing edges.  Because different kinds of Digraphs store different
// kinds of vertex and edge information, DigraphVertex is a struct template.

template <typename VertexInfo, typename EdgeInfo>
struct DigraphVertex
{
    VertexInfo vinfo;
    std::list<DigraphEdge<EdgeInfo>> edges;
};



// Digraph is a class template that represents a directed graph implemented
// using adjacency lists.  It takes two type parameters:
//
// * VertexInfo, which specifies the kind of object stored for each vertex
// * EdgeInfo, which specifies the kind of object stored for each edge
//
//
// Each vertex in a Digraph is identified uniquely by a "vertex number".
// Vertex numbers are not necessarily sequential and they are not necessarily
// zero- or one-based.

template <typename VertexInfo, typename EdgeInfo>
class Digraph
{
public:
    // The default constructor initializes a new, empty Digraph so that
    // contains no vertices and no edges.
    Digraph();

    Digraph(const Digraph& d);

    // The move constructor initializes a new Digraph from an expiring one.
    Digraph(Digraph&& d) noexcept;

    // The destructor deallocates any memory associated with the Digraph.
    ~Digraph() noexcept;

    // The assignment operator assigns the contents of the given Digraph
    // into "this" Digraph, with "this" Digraph becoming a separate, deep
    // copy of the contents of the given one (i.e., any change made to
    // "this" Digraph afterward will not affect the other).
    Digraph& operator=(const Digraph& d);

    // The move assignment operator assigns the contents of an expiring
    // Digraph into "this" Digraph.
    Digraph& operator=(Digraph&& d) noexcept;

    // vertices() returns a std::vector containing the vertex numbers of
    // every vertex in this Digraph.
    std::vector<int> vertices() const;

    // edges() returns a std::vector of std::pairs, in which each pair
    // contains the "from" and "to" vertex numbers of an edge in this
    // Digraph.  All edges are included in the std::vector.
    std::vector<std::pair<int, int>> edges() const;

    // This overload of edges() returns a std::vector of std::pairs, in
    // which each pair contains the "from" and "to" vertex numbers of an
    // edge in this Digraph.  Only edges outgoing from the given vertex
    // number are included in the std::vector.  If the given vertex does
    // not exist, a DigraphException is thrown instead.
    std::vector<std::pair<int, int>> edges(int vertex) const;

    // vertexInfo() returns the VertexInfo object belonging to the vertex
    // with the given vertex number.  If that vertex does not exist, a
    // DigraphException is thrown instead.
    VertexInfo vertexInfo(int vertex) const;

    // edgeInfo() returns the EdgeInfo object belonging to the edge
    // with the given "from" and "to" vertex numbers.  If either of those
    // vertices does not exist *or* if the edge does not exist, a
    // DigraphException is thrown instead.
    EdgeInfo edgeInfo(int fromVertex, int toVertex) const;

    // addVertex() adds a vertex to the Digraph with the given vertex
    // number and VertexInfo object.  If there is already a vertex in
    // the graph with the given vertex number, a DigraphException is
    // thrown instead.
    void addVertex(int vertex, const VertexInfo& vinfo);

    // addEdge() adds an edge to the Digraph pointing from the given
    // "from" vertex number to the given "to" vertex number, and
    // associates with the given EdgeInfo object with it.  If one
    // of the vertices does not exist *or* if the same edge is already
    // present in the graph, a DigraphException is thrown instead.
    void addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo);

    // removeVertex() removes the vertex (and all of its incoming
    // and outgoing edges) with the given vertex number from the
    // Digraph.  If the vertex does not exist already, a DigraphException
    // is thrown instead.
    void removeVertex(int vertex);

    // removeEdge() removes the edge pointing from the given "from"
    // vertex number to the given "to" vertex number from the Digraph.
    // If either of these vertices does not exist *or* if the edge
    // is not already present in the graph, a DigraphException is
    // thrown instead.
    void removeEdge(int fromVertex, int toVertex);

    // vertexCount() returns the number of vertices in the graph.
    int vertexCount() const noexcept;

    // edgeCount() returns the total number of edges in the graph,
    // counting edges outgoing from all vertices.
    int edgeCount() const noexcept;

    // This overload of edgeCount() returns the number of edges in
    // the graph that are outgoing from the given vertex number.
    // If the given vertex does not exist, a DigraphException is
    // thrown instead.
    int edgeCount(int vertex) const;

    // isStronglyConnected() returns true if the Digraph is strongly
    // connected (i.e., every vertex is reachable from every other),
    // false otherwise.
    bool isStronglyConnected() const;

    // findShortestPaths() takes a start vertex number and a function
    // that takes an EdgeInfo object and determines an edge weight.
    // It uses Dijkstra's Shortest Path Algorithm to determine the
    // shortest paths from the start vertex to every other vertex
    // in the graph.  The result is returned as a std::map<int, int>
    // where the keys are vertex numbers and the value associated
    // with each key k is the precedessor of that vertex chosen by
    // the algorithm.  For any vertex without a predecessor (e.g.,
    // a vertex that was never reached, or the start vertex itself),
    // the value is simply a copy of the key.
    std::map<int, int> findShortestPaths(
        int startVertex,
        std::function<double(const EdgeInfo&)> edgeWeightFunc) const;


private:

    std::map<int,DigraphVertex<VertexInfo, EdgeInfo>> container;

    void DFTr(int vertexIndex,const DigraphVertex<VertexInfo,EdgeInfo>& v, 
                                            std::map<int,bool>& visitRecords) const;


    int countConnectVertices(const std::map<int,bool>& visitRecords) const;
};


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph()
{
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(const Digraph& d)
{
    for(auto it = d.container.begin(); it!=d.container.end(); it++)
    {
        int vertexIndex = it->first;
        addVertex(vertexIndex,it->second.vinfo);
        for(auto it_list = it->second.edges.begin(); it_list!=it->second.edges.end();it_list++)
            addEdge(vertexIndex, it_list->toVertex, it_list->einfo);
    }
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(Digraph&& d) noexcept
{
    container = d.container;
}

template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::~Digraph() noexcept
{
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(const Digraph& d)
{
    container.clear();
    for(auto it = d.container.begin(); it!=d.container.end(); it++)
    {
        int vertexIndex = it->first;
        addVertex(vertexIndex,it->second.vinfo);
        for(auto it_list = it->second.edges.begin(); it_list!=it->second.edges.end();it_list++)
            addEdge(vertexIndex, it_list->toVertex, it_list->einfo);
    }
    
    return *this;
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(Digraph&& d) noexcept
{
    container = d.container;
    return *this;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<int> Digraph<VertexInfo, EdgeInfo>::vertices() const
{
    std::vector<int> allVerticesNum;
    for(auto it = container.begin(); it!=container.end(); it++)
        allVerticesNum.push_back(it->first);
    return allVerticesNum;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges() const
{
    std::vector<std::pair<int,int>> allEdges;
    for(auto it = container.begin();it!=container.end(); it++)
        for(auto it_list = it->second.edges.begin(); it_list!=it->second.edges.end(); it_list++)
            allEdges.push_back(std::pair<int,int>{it_list->fromVertex,it_list->toVertex});
    return allEdges;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges(int vertex) const
{
    auto it = container.find(vertex);
    if(it==container.end())
        throw DigraphException{std::string("When get edges fro ma vertex, can't find vertex!")};
    else
    {
        std::vector<std::pair<int,int>> edgesFromVertex;
        for(auto it_list = it->second.edges.begin(); it_list!=it->second.edges.end();it_list++)
            edgesFromVertex.push_back(std::pair<int,int>{it_list->fromVertex, it_list->toVertex});
    
        return edgesFromVertex;
    }
}


template <typename VertexInfo, typename EdgeInfo>
VertexInfo Digraph<VertexInfo, EdgeInfo>::vertexInfo(int vertex) const
{
    auto it = container.find(vertex);
    if(it == container.end())
        throw DigraphException{std::string("When vertexInfo, can't find vertex")};
    else
        return it->second.vinfo;
}


template <typename VertexInfo, typename EdgeInfo>
EdgeInfo Digraph<VertexInfo, EdgeInfo>::edgeInfo(int fromVertex, int toVertex) const
{
    auto it = container.find(fromVertex);
    if(it==container.end())
        throw DigraphException{std::string("When edgeInfo, can't find fromVertex")};
    else
    {
        for(auto it_list = it->second.edges.begin(); it_list!= it->second.edges.end(); it_list++)
            if(it_list->toVertex == toVertex)
                return it_list->einfo;
        throw DigraphException{std::string("WHen edgeInfo, can't find toVertex")};
    }
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addVertex(int vertex, const VertexInfo& vinfo)
{
    container.insert( std::pair<int,DigraphVertex<VertexInfo,EdgeInfo>>(vertex,DigraphVertex<VertexInfo,EdgeInfo>{vinfo}));
    
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo)
{
    typename std::map<int,DigraphVertex<VertexInfo,EdgeInfo>>::iterator it;
    it = container.find(fromVertex);

    if(it == container.end())
        throw DigraphException{std::string("When add, can't find startVertex!")};
    else
    {
        typename std::map<int, DigraphVertex<VertexInfo,EdgeInfo>>::iterator it_2;
        it_2 = container.find(toVertex);
        if(it_2 == container.end())
            throw DigraphException{std::string("When add, can't find endVertex!")};
        else
            it->second.edges.push_back(DigraphEdge<EdgeInfo>{fromVertex,toVertex,einfo});
    }
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeVertex(int vertex)
{
    auto it = container.find(vertex);
    if(it == container.end())
        throw DigraphException{std::string("When removeVertex, vertex not found!")};
    else
        container.erase(it);

    for(it = container.begin();it!=container.end();++it)
    {
        std::list<DigraphEdge<EdgeInfo>>& temp = it->second.edges;
        for(auto it_list = temp.begin(); it_list!=temp.end();it_list++)
            if(it_list->toVertex==vertex)
                temp.erase(it_list);
    }
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeEdge(int fromVertex, int toVertex)
{
    auto it = container.find(fromVertex);
    if(it == container.end())
        throw DigraphException{std::string("When removeEdge, fromVertex not found!")};
    else
    {
        std::list<DigraphEdge<EdgeInfo>>& temp = it->second.edges;
        for(auto it_list = temp.begin(); it_list!=temp.end();it_list++)
        {
            if(it_list->toVertex == toVertex)
            {
                temp.erase(it_list);
                return;
            }
        }
        throw DigraphException{std::string("When removeEdge, toVertex not found!")};
    }
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::vertexCount() const noexcept
{
    return container.size();
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount() const noexcept
{
    int edgeNum = 0;
    for(auto it = container.begin(); it!=container.end(); it++)
        edgeNum += it->second.edges.size();
    return edgeNum;
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount(int vertex) const
{
    auto it = container.find(vertex);
    if(it == container.end())
        throw DigraphException{std::string("When edgeCount, vertex not found!")};
    else
        return it->second.edges.size();
}

template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::countConnectVertices(const std::map<int,bool>& visitRecords) const
{
    int count = 0;
    for(auto it = visitRecords.begin(); it != visitRecords.end(); it++)
        if(it->second == true)
            count++;
    return count;
}

template <typename VertexInfo, typename EdgeInfo>
bool Digraph<VertexInfo, EdgeInfo>::isStronglyConnected() const
{
    int sz = vertexCount();
    std::map<int,bool> visitRecords;

    for(auto it = container.begin(); it!=container.end(); it++)
    {
        for(auto it_2 = container.begin(); it_2!= container.end(); it_2++)
            visitRecords[it_2->first] = false;
        DFTr(it->first, it->second, visitRecords);
       
        if(countConnectVertices(visitRecords) != sz)
            return false;
    }

    return true;
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::DFTr(int vertexIndex, 
                  const DigraphVertex<VertexInfo,EdgeInfo>& v, std::map<int,bool>& visitRecords) const
{
    visitRecords[vertexIndex] = true;

    for(auto it_list=v.edges.begin(); it_list!=v.edges.end(); it_list++)
        if(visitRecords[it_list->toVertex]==false)
            DFTr(it_list->toVertex,container.at(it_list->toVertex), visitRecords);
}


struct DijkstraInfo
{
    bool kFlag;
    int d;
    int p;
};

class Compare
{
public:
    bool operator() (const std::pair<int,int>& lhs, const std::pair<int,int>& rhs)
    {
        return lhs.first < rhs.first;
    }
};

template <typename VertexInfo, typename EdgeInfo>
std::map<int, int> Digraph<VertexInfo, EdgeInfo>::findShortestPaths(
    int startVertex,
    std::function<double(const EdgeInfo&)> edgeWeightFunc) const
{
    auto it_finder = container.find(startVertex);
    if(it_finder== container.end())
        throw DigraphException{std::string("When findShortestPaths, startVertex not found!")};

    std::map<int,DijkstraInfo> vData;
    //initialization
    for(auto it=container.begin(); it!=container.end();it++)
        vData[it->first] = DijkstraInfo{false,std::numeric_limits<int>::max(),it->first};
    vData[startVertex].d = 0;
    vData[startVertex].p = startVertex;

    std::priority_queue<std::pair<int,int>, std::vector<std::pair<int,int>>, Compare> pqueue;
                                  
    pqueue.push(std::pair<int,int>{0,startVertex});

    while(pqueue.size()!=0)
    {
        int vIndex = pqueue.top().second;
        //std::cout<<"now we test vertex: "<<vIndex<<std::endl;
        DijkstraInfo& vD = vData[vIndex];
        pqueue.pop();

        if(vD.kFlag == false)
        {
            vD.kFlag = true;
            auto edges_list = container.at(vIndex).edges;
            for(auto it_list = edges_list.begin();it_list!=edges_list.end();it_list++)
            {
                //std::cout<<"\tedge: "<<it_list->toVertex<<std::endl;
                DijkstraInfo& wD = vData[it_list->toVertex];
                //std::cout<<"\t  dw = "<<wD.d<<std::endl;
                //std::cout<<"\t  rhs = "<< vD.d+edgeWeightFunc(it_list->einfo)<<std::endl;
                if(wD.d > (vD.d + edgeWeightFunc(it_list->einfo)))
                {
      
                    //std::cout<<"\t  change"<<std::endl;
                    wD.d = vD.d + edgeWeightFunc(it_list->einfo);
                    wD.p = vIndex;
                    pqueue.push(std::pair<int,int>{wD.d,it_list->toVertex});
                }
            }
        }
    }

    std::map<int,int> ans;
    for(auto it_map = vData.begin(); it_map!=vData.end(); it_map++)
        ans[it_map->first] = it_map->second.p;


    return ans;
}



#endif // DIGRAPH_HPP

