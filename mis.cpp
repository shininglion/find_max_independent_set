#include <limits>
#include <algorithm>
#include <iostream>
#include <functional>
#include <unordered_set>


namespace graph
{

	template <typename T>
	inline const T& value(const T& input) { return input; }


	template <typename T, typename EdgeExtractor>
	void findMIS(const T& graph, std::vector<int>& is, EdgeExtractor edge_extractor)
	{
		// find an MIS regardless of cost is equivalent to find a min-cost MIS with all the costs are 0.
		findMinCostMIS(graph, is, edge_extractor, [](const typename value_type<T>::type&) -> int { return 0; } );
	}


	template <typename T>
	inline void findMIS(const T& graph, std::vector<int>& is)
	{
		typedef typename value_type<T>::type NodeType;
		findMIS(graph, is, value<NodeType>);
	}


	template <typename T, typename EdgeExtractor, typename CostExtractor>
	auto findMinCostMIS(const T& graph, std::vector<int>& is, EdgeExtractor edge_extractor, CostExtractor cost_extractor) -> typename std::result_of<CostExtractor(const typename value_type<T>::type&)>::type
	{
		using namespace std;
		typedef typename value_type<T>::type NodeType;
		typedef typename result_of<CostExtractor(const NodeType&)>::type CostType;

		const int graph_size = distance( begin(graph), end(graph) );
		is.clear();
		
		vector< vector<int> > net(graph_size);
		// 'aff' records the number of vertices which will become forbidden at the moment ith-vertex is taken
		// 'con' records the out-degree to untaken vertices
		// 'dis' records the number of vertices which have become forbidden in routine testing
		vector<int> aff, con, dis;
		vector<CostType> cost;
		aff.resize(graph_size);
		con.resize(graph_size);
		dis.resize(graph_size);
		cost.resize(graph_size);
		
		/* record the edge in vector */
		for (int i = 0; i < graph_size; ++i) {
			auto& graph_node = graph[i];
			auto& edge_list = edge_extractor(graph_node); 
			con[i] = distance( begin(edge_list), end(edge_list) );
			cost[i] = cost_extractor(graph_node);
		}

		
		/* search the least out-degree */
		int small = numeric_limits<int>::max();
		for (auto element : con) {
			small = min(small, element);
		}

		
		/* enumerate the vertex with least out-degree as the first taking node */
		CostType total_cost = numeric_limits<CostType>::max();
		for(int node = 0; node < graph_size; ++node){

			/* if the vertex had been taken or the vertex doesn't have least out-degree */
			//if( con[node] != small ) { continue; }

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
				if (forbid[lhs]) { return false; }
				if (forbid[rhs]) { return true; }
				
				if( (aff[lhs] - dis[lhs]) > (aff[rhs] - dis[rhs]) ) { return true; }
				if( (aff[lhs] - dis[lhs]) < (aff[rhs] - dis[rhs]) ) { return false; }
				if( con[lhs] >= con[rhs] ) { return true; }
				return (cost[lhs] > cost[rhs]);
			};
			auto greedy_comp2 = [&](const int lhs, const int rhs) -> bool {
				if (forbid[lhs]) { return false; }
				if (forbid[rhs]) { return true; }
				
				if( aff[lhs] > aff[rhs] ) { return true; }
				if( aff[lhs] < aff[rhs] ) { return false; }
				if( con[lhs] >= con[rhs] ) { return true; }
				return (cost[lhs] > cost[rhs]);
			};
			std::function<bool (const int, const int)> cmp_strategy[2] = {greedy_comp1, greedy_comp2};

			
			for (int sort_strategy = 0; sort_strategy < 2; ++sort_strategy) {
				/* initial setting */
				vector<int> que(graph_size); 
				for (int i = 0; i < graph_size; ++i) {
					aff[i] = con[i];
					que[i] = i;
					forbid[i] = false;
					dis[i] = 0;
				}
				
				/* take first vertex n */
				CostType cur_cost = choose_vertex(node);
				
				vector<int> take;
				take.push_back(node);
				
				/* start greedy strategy to decide maximum_is */
				while( !que.empty() ){
					stable_sort(que.begin(), que.end(), cmp_strategy[sort_strategy]);

					while ( (!que.empty()) && (forbid[que.back()]) ) { que.pop_back(); }
					if (que.empty()) { break; }

					const int tmp = que.back();
					que.pop_back();
					take.push_back(tmp);
					cur_cost += choose_vertex(tmp);
				}
				
				if( (take.size() > is.size()) || ((take.size() == is.size()) && (total_cost > cur_cost)) ) { is = take; total_cost = cur_cost; }
			}
		}

		return total_cost;
	}


	template <typename T, typename EdgeExtractor>
	void naiveMIS(const T& graph, std::vector<int>& is, EdgeExtractor edge_extractor)
	{
		// find an MIS regardless of cost is equivalent to find a min-cost MIS with all the costs are 0.
		naiveMinCostMIS(graph, is, edge_extractor, [](const typename value_type<T>::type&) -> int { return 0; } );
	}


	template <typename T>
	inline void naiveMIS(const T& graph, std::vector<int>& is)
	{
		typedef typename value_type<T>::type NodeType;
		naiveMIS(graph, is, value<NodeType>);
	}


	template <typename T, typename EdgeExtractor, typename CostExtractor>
	auto naiveMinCostMIS(const T& graph, std::vector<int>& is, EdgeExtractor edge_extractor, CostExtractor cost_extractor) -> typename std::result_of<CostExtractor(const typename value_type<T>::type&)>::type
	{
		using namespace std;
		typedef typename value_type<T>::type NodeType;
		typedef typename result_of<CostExtractor(const NodeType&)>::type CostType;

		const int graph_size = distance( begin(graph), end(graph) );
		is.clear();
		CostType total_cost = numeric_limits<CostType>::max();
		
		CostType temp_cost = CostType();
		unordered_set<int> temp_is;
		vector<bool> pick(graph_size, false);
		
		// check legality
		auto is_feasible = [&](const int node) -> bool {
			const auto& edge_list = edge_extractor( graph[node] ); 
			for (auto& edge : edge_list) {
				if (temp_is.find(edge) != temp_is.end()) { return false; }
			}
			return true;
		};
		
		auto pick_node = [&](const int node, function<void (const int)>& find_mic_cost_mis) {
			pick[node] = true;
			temp_is.emplace(node);
			temp_cost += cost_extractor( graph[node] );
			
			find_mic_cost_mis(node);
			
			pick[node] = false;
			temp_is.erase(node);
			temp_cost -= cost_extractor( graph[node] );
		};
		
		// using recursion to find a mis with given starting node
		function<void (const int)> findMicCostMIS = [&](const int node) {
			for (int i = 0; i < graph_size; ++i) {
				if (pick[i]) { continue; }
				if ( !is_feasible(i) ) { continue; }
				pick_node(i, findMicCostMIS);
			}
			
			if ((temp_is.size() > is.size()) || ((temp_is.size() == is.size()) && (temp_cost < total_cost))) {
				is.clear(); is.reserve(temp_is.size());
				for_each(temp_is.cbegin(), temp_is.cend(), [&](int val) { is.push_back(val); } );
				total_cost = temp_cost;
			}
		};
		
		// enumerate every node as starting node
		for (int i = 0; i < graph_size; ++i) {
			fill(pick.begin(), pick.end(), false);
			pick_node(i, findMicCostMIS);
		}
		

		return total_cost;
	}


	template <typename T, typename EdgeExtractor>
	bool validateMIS(const T& graph, const std::vector<int>& mis, EdgeExtractor edge_extractor)
	{
		bool pass = true;
		for (size_t i = 0; pass && (i < mis.size()); ++i) {
			for (size_t j = i + 1; j < mis.size(); ++j) {
				auto& edge_list = edge_extractor( graph[ mis[j] ] );

				bool check = false;
				for (auto& edge : edge_list) {
					if (edge == mis[i]) { check = true; break; }
				}
				if (check) { pass = false; break; }
			}
		}
		return pass;
	}

	template <typename T>
	bool validateMIS(const T& graph, const std::vector<int>& mis)
	{
		typedef typename value_type<T>::type NodeType;
		return validateMIS(graph, mis, value<NodeType>);
	}
}

