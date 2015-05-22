#include <climits>
#include <vector>
#include <algorithm>
#include <iterator>
#include "value_type.hpp"


template <typename T>
inline T value(const T& input) { return input; }


template <typename T, typename EdgeExtractor>
void findMIS(const T& graph, std::vector<int>& is, EdgeExtractor edge_extractor)
{
    using namespace std;
    
    const int graph_size = distance( begin(graph), end(graph) );
    is.clear();
    
    vector< vector<int> > net(graph_size);
    // 'aff' records the number of vertices which will become forbidden at the moment ith-vertex is taken
    // 'con' records the out-degree to untaken vertices
    // 'dis' records the number of vertices which have become forbidden in routine testing
    vector<int> aff, con, dis;
    aff.reserve(graph_size);
    con.reserve(graph_size);
    dis.reserve(graph_size);
    
    /* record the edge in vector */
    for(auto graph_node : graph){
        con.push_back( distance( begin(graph_node), end(graph_node) ) );
    }

    
    /* search the least out-degree */
    int small = INT_MAX;
    for (auto element : con) {
        if (element < small) { small = element; }
    }

    
    
    
    
    /* enumerate the vertex with least out-degree as the first taking node */
    int sort_strategy = 0;
    for(int node = 0; node < graph_size;){

        /* if the vertex had been taken or the vertex doesn't have least out-degree */
        if( con[node] != small ){
            ++node;
            continue;
        }

        vector<int> que;        
        // if forbid[i] is true, it means there are some neighboring vertices have been taken in routine testing
        vector<bool> forbid(graph_size, false);

        // The complexity of this function is O(n^2) where n is the number of vertices.
        // This will happen when the graph is a complete graph.
        auto choose_vertex = [&](const int vertex) {
            forbid[vertex] = true;
            auto& graph_node = graph[vertex];
            for (auto& node_edge : graph_node) {
                const int edge = edge_extractor(node_edge);
                if ( forbid[edge] ) { continue; }
                forbid[edge] = true;
                
                auto& neighbor_node = graph[edge];
                for (auto& element : neighbor_node) {
                    const int neighbor_edge = edge_extractor(element);
                    if ( !forbid[neighbor_edge] ) {
                        --aff[neighbor_edge];
                        ++dis[neighbor_edge];
                    }
                }
            }
        };
        
        auto greedy_comp1 = [&](const int lhs, const int rhs) -> bool {
            if (forbid[lhs]) { return true; }
            if (forbid[rhs]) { return false; }
            
            if( (aff[lhs] - dis[lhs]) > (aff[rhs] - dis[rhs]) ) { return true; }
            else if( (aff[lhs] - dis[lhs]) < (aff[rhs] - dis[rhs]) ) { return false; }
            else if( con[lhs] >= con[rhs] ) { return true; }
            return false;
        };
        auto greedy_comp2 = [&](const int lhs, const int rhs) -> bool {
            if (forbid[lhs]) { return true; }
            if (forbid[rhs]) { return false; }
            
            if( aff[lhs] > aff[rhs] ) { return true; }
            else if( aff[lhs] < aff[rhs] ) { return false; }
            else if( con[lhs] >= con[rhs] ) { return true; }
            return false;
        };

        
        /* initial setting */
        for (int i = 0; i < graph_size; ++i) {
            aff.push_back( con[i] );
            dis.push_back(0);
            que.push_back(i);
        }
        
        /* take first vertex n */
        choose_vertex(node);
        
        vector<int> take;
        take.push_back(node);
        
        /* start greedy strategy to decide maximum_is */
        while( !que.empty() ){
			if (sort_strategy == 0) { sort(que.begin(), que.end(), greedy_comp1); }
			else { sort(que.begin(), que.end(), greedy_comp2); }
            const int tmp = que.back();
            que.pop_back();
            if( forbid[tmp] ) { break; }
            take.push_back(tmp);
            choose_vertex(tmp);
        }
        
        if( take.size() > is.size() ) { is = take; }
        if( sort_strategy == 1 ) { ++node; }
        sort_strategy ^= 1;
    }
}


template <typename T>
inline void findMIS(const T& graph, std::vector<int>& is)
{
    typedef typename value_type<typename value_type<T>::type>::type NodeType;
    findMIS(graph, is, value<NodeType>);
}
