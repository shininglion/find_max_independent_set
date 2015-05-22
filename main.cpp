#include <cstdio>
#include <vector>
#include "mis.h"

struct Node
{
    int edge;
    int cost;

    Node(const int e = 0, const int c = 0) : edge(e), cost(0) {}
};


int main(const int argc, const char* argv[])
{
    using namespace std;
	
    if (freopen(argv[1],"r",stdin) == nullptr) {
		fprintf(stderr, "Cannot open file %s\n", argv[1]);
		return 0;
	}

    vector< vector<int> > connect;
    //vector< vector<Node> > connect;
    vector<int> set;

    int Vnum, Enum;
	bool fail_flag = false;
    while(scanf("%d %d", &Vnum, &Enum) == 2) {
        connect.resize(Vnum);
        for (auto& node : connect) { node.reserve(Vnum); }

        // set the edge connection by (a b) for convenience
        for(int i = 0; (i < Enum) && (!fail_flag); ++i){
            int from, to;
            if (scanf("%d %d", &from, &to) != 2) { fail_flag = true; }
			if (fail_flag) { break;}
            // set index start from 0
            --from;
            --to;
            connect[from].emplace_back(to);
            connect[to].emplace_back(from);
        }
		if (fail_flag) {
			fail_flag = false;
			fprintf(stderr, "An incomplete input.\n");
			continue;
		}

        findMIS(connect, set);
        //findMIS(connect, set, [](const Node& node) -> int { return node.edge; } );

        printf("Maximum independent set (MIS) size: %lu\n", set.size());
		printf("MIS vertices: ");
        for(size_t j = 0; j < set.size(); ++j )
            printf(" %d", set[j] + 1);
		putchar('\n');
    }
    return 0;
}
