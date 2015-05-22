#ifndef MAXIMUM_INDEPENDENT_SET_H_
#define MAXIMUM_INDEPENDENT_SET_H_

template <typename T>
void findMIS(const T& graph, std::vector<int>& is);

template <typename T, typename EdgeExtractor>
void findMIS(const T& graph, std::vector<int>& is, EdgeExtractor edge_extractor);


#include "mis.cpp"

#endif /*MAXIMUM_INDEPENDENT_SET_H_*/
