#include <limits>
#include <algorithm>
#include <iostream>


template <typename T>
inline const T& value(const T& input) { return input; }


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
		auto& edge_list = edge_extractor(graph_node); 
        con.push_back( distance( begin(edge_list), end(edge_list) ) );
    }

    
    /* search the least out-degree */
    int small = numeric_limits<int>::max();
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
			auto& node_edge_list = edge_extractor(graph_node);
            for (auto& node_edge : node_edge_list) {
                if ( forbid[node_edge] ) { continue; }
                forbid[node_edge] = true;
                
                auto& neighbor_node = graph[node_edge];
				auto& neighbor_edge_list = edge_extractor(neighbor_node);
                for (auto& neighbor_edge : neighbor_edge_list) {
                    if ( !forbid[neighbor_edge] ) {
                        --aff[neighbor_edge];
                        //++dis[neighbor_edge];
                        dis[neighbor_edge] = max(dis[neighbor_edge], dis[vertex] + 2);
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
    typedef typename value_type<T>::type NodeType;
    findMIS(graph, is, value<NodeType>);
}


template <typename T, typename EdgeExtractor, typename CostExtractor>
auto findMinCostMIS(const T& graph, std::vector<int>& is, EdgeExtractor edge_extractor, CostExtractor cost_extractor) -> typename std::result_of<CostExtractor(const typename value_type<typename value_type<T>::type>::type&)>::type
{
    using namespace std;
	typedef typename value_type<typename value_type<T>::type>::type NodeType;
	typedef typename result_of<CostExtractor(const NodeType&)>::type CostType;

    const int graph_size = distance( begin(graph), end(graph) );
    is.clear();
    
    vector< vector<int> > net(graph_size);
    // 'aff' records the number of vertices which will become forbidden at the moment ith-vertex is taken
    // 'con' records the out-degree to untaken vertices
    // 'dis' records the number of vertices which have become forbidden in routine testing
    vector<int> aff, con, dis;
	vector<CostType> cost;
    aff.reserve(graph_size);
    con.reserve(graph_size);
    dis.reserve(graph_size);
	cost.reserve(graph_size);
    
    /* record the edge in vector */
    for(auto graph_node : graph){
		auto& edge_list = edge_extractor(graph_node); 
        con.push_back( distance( begin(edge_list), end(edge_list) ) );
		cost.push_back( cost_extractor(graph_node) );
    }

    
    /* search the least out-degree */
    int small = numeric_limits<int>::max();
    for (auto element : con) {
        if (element < small) { small = element; }
    }

    
    /* enumerate the vertex with least out-degree as the first taking node */
    int sort_strategy = 0;
	CostType total_cost = numeric_limits<CostType>::max();
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
        auto choose_vertex = [&](const int vertex) -> CostType {
            forbid[vertex] = true;
            auto& graph_node = graph[vertex];
			auto& node_edge_list = edge_extractor(graph_node);
            for (auto& node_edge : node_edge_list) {
                if ( forbid[node_edge] ) { continue; }
                forbid[node_edge] = true;
                
                auto& neighbor_node = graph[node_edge];
				auto& neighbor_edge_list = edge_extractor(neighbor_node);
                for (auto& neighbor_edge : neighbor_edge_list) {
                    if ( !forbid[neighbor_edge] ) {
                        --aff[neighbor_edge];
                        //++dis[neighbor_edge];
                        dis[neighbor_edge] = max(dis[neighbor_edge], dis[vertex] + 2);
					}
                }
            }
			return cost[vertex];
        };
        
        auto greedy_comp1 = [&](const int lhs, const int rhs) -> bool {
            if (forbid[lhs]) { return true; }
            if (forbid[rhs]) { return false; }
            
            if( (aff[lhs] - dis[lhs]) > (aff[rhs] - dis[rhs]) ) { return true; }
            else if( (aff[lhs] - dis[lhs]) < (aff[rhs] - dis[rhs]) ) { return false; }
            else if( con[lhs] >= con[rhs] ) { return true; }
			return (cost[lhs] > cost[rhs]);
            //return false;
        };
        auto greedy_comp2 = [&](const int lhs, const int rhs) -> bool {
            if (forbid[lhs]) { return true; }
            if (forbid[rhs]) { return false; }
            
            if( aff[lhs] > aff[rhs] ) { return true; }
            else if( aff[lhs] < aff[rhs] ) { return false; }
            else if( con[lhs] >= con[rhs] ) { return true; }
			return (cost[lhs] > cost[rhs]);
            //return false;
        };

        
        /* initial setting */
        for (int i = 0; i < graph_size; ++i) {
            aff.push_back( con[i] );
            dis.push_back(0);
            que.push_back(i);
        }
        
        /* take first vertex n */
        CostType cur_cost = choose_vertex(node);
		cout << node + 1;
        
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
            cur_cost += choose_vertex(tmp);
			cout << ' ' << tmp + 1;
        }
        
        if( (take.size() > is.size()) || ((take.size() == is.size()) && (total_cost > cur_cost)) ) { is = take; total_cost = cur_cost; }
        if( sort_strategy == 1 ) { ++node; }
        sort_strategy ^= 1;
		cout << '\n';
    }

	return total_cost;
}
