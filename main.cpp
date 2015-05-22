#include <cstdio>
#include <vector>
#include "mis.h"
#define TEST_USE_NODE

#ifdef TEST_USE_NODE
struct Node
{
    int cost;
	std::vector<int> edge_list;
};
//#define TEST_COST
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
			int cost = 0;
			if (scanf("%d", &cost) != 1) { fail_flag = true; break; }
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
        const int cost = findMinCostMIS(connect, set, [](const Node& node) -> const std::vector<int>& { return node.edge_list; }, [](const Node& node) -> int { return node.cost; } );
#else
        findMIS(connect, set, [](const Node& node) -> const std::vector<int>& { return node.edge_list; });
#endif
#else
        findMIS(connect, set);
#endif

#ifdef TEST_COST
        printf("Maximum independent set (MIS) size and cost: %lu, %d\n", set.size(), cost);
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
