#include <cstdio>
#include <vector>
#include "mis.h"
#define TEST_USE_NODE
#define USE_NAIVE

#ifdef TEST_USE_NODE
struct Node
{
    double cost;
	std::vector<int> edge_list;
};
#define TEST_COST
#endif


int main(const int argc, const char* argv[])
{
    using namespace std;
	
    if (freopen(argv[1],"r",stdin) == nullptr) {
		fprintf(stderr, "Cannot open file %s\n", argv[1]);
		return 0;
	}

#ifdef TEST_USE_NODE
    vector<Node> connect;
#else
    vector< vector<int> > connect;
#endif
    vector<int> set;

    int Vnum, Enum;
	bool fail_flag = false;
    while(scanf("%d %d", &Vnum, &Enum) == 2) {
        connect.resize(Vnum);
#ifdef TEST_USE_NODE
        for (auto& node : connect) { node.edge_list.reserve(Vnum); }
#else
        for (auto& node : connect) { node.reserve(Vnum); }
#endif

#ifdef TEST_COST
		for (int i = 0; (i < Vnum) && (!fail_flag); ++i) {
			double cost = 0;
			if (scanf("%lf", &cost) != 1) { fail_flag = true; break; }
			connect[i].cost = cost;
		}
#endif

        // set the edge connection by (a b) for convenience
        for(int i = 0; (i < Enum) && (!fail_flag); ++i){
			int from, to;
            if (scanf("%d %d", &from, &to) != 2) { fail_flag = true; break; }
            // set index start from 0
            --from;
            --to;
#ifdef TEST_USE_NODE
            connect[from].edge_list.push_back(to);
            connect[to].edge_list.push_back(from);
#else
            connect[from].push_back(to);
            connect[to].push_back(from);
#endif
        }
		if (fail_flag) {
			fail_flag = false;
			fprintf(stderr, "An incomplete input.\n");
			continue;
		}

#ifdef TEST_USE_NODE
#ifdef TEST_COST
#ifndef USE_NAIVE
		printf("use greedy heuristic...\n");
        const double cost = graph::findMinCostMIS(connect, set, [](const Node& node) -> const std::vector<int>& { return node.edge_list; }, [](const Node& node) -> double { return node.cost; } );
#else
		printf("use naive approach...\n");
        const double cost = graph::naiveMinCostMIS(connect, set, [](const Node& node) -> const std::vector<int>& { return node.edge_list; }, [](const Node& node) -> double { return node.cost; } );
#endif
#else
#ifndef USE_NAIVE
		graph::findMIS(connect, set, [](const Node& node) -> const std::vector<int>& { return node.edge_list; });
#else
		graph::naiveMIS(connect, set, [](const Node& node) -> const std::vector<int>& { return node.edge_list; });
#endif
#endif
#else
#ifndef USE_NAIVE
		graph::findMIS(connect, set);
#else
		graph::naiveMIS(connect, set);
#endif
#endif

		printf("Validate mis...");
#ifdef TEST_USE_NODE
		const bool legal = graph::validateMIS(connect, set, [](const Node& node) -> const std::vector<int>& { return node.edge_list; });
#else
		const bool legal = graph::validateMIS(connect, set);
#endif
		printf(legal ? "pass\n" : "fail\n");

#ifdef TEST_COST
        printf("Maximum independent set (MIS) size and cost: %lu, %.3lf\n", set.size(), cost);
#else
        printf("Maximum independent set (MIS) size: %lu\n", set.size());
#endif
		printf("MIS vertices: ");
        for(size_t j = 0; j < set.size(); ++j )
            printf(" %d", set[j] + 1);
		putchar('\n');
    }
    return 0;
}
