#ifndef MAXIMUM_INDEPENDENT_SET_H_
#define MAXIMUM_INDEPENDENT_SET_H_

#include <vector>
#include <type_traits>
#include "value_type.hpp"


namespace graph
{
	template <typename T>
	void findMIS(const T& graph, std::vector<int>& is);

	template <typename T, typename EdgeExtractor>
	void findMIS(const T& graph, std::vector<int>& is, EdgeExtractor edge_extractor);

	template <typename T, typename EdgeExtractor, typename CostExtractor>
	auto findMinCostMIS(const T& graph, std::vector<int>& is, EdgeExtractor edge_extractor, CostExtractor cost_extractor) -> typename std::result_of<CostExtractor(const typename value_type<T>::type&)>::type;
}

#include "mis.cpp"

#endif /*MAXIMUM_INDEPENDENT_SET_H_*/
