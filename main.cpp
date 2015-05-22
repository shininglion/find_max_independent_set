struct Node
{
    int edge;
    int cost;

    Node(const int e = 0, const int c = 0) : edge(e), cost(0) {}
};


int main(const int argc, const char* argv[])
{
    using namespace std;
	
    freopen(argv[1],"r",stdin);
    //freopen("out.txt","w",stdout);

    //vector< vector<int> > connect;
    //vector< vector<Node> > connect;
    vector<int> set;

    int Vnum, Enum;
    while(scanf("%d %d", &Vnum, &Enum) == 2) {
        connect.resize(Vnum);
        for (auto& node : connect) { node.reserve(Vnum); }

        // set the edge connection by (a b) for convenience
        for(int i = 0; i < Enum; ++i){
            int from, to;
            scanf("%d %d", &from, &to);
            // set index start from 0
            --from;
            --to;
            connect[from].emplace_back(to);
            connect[to].emplace_back(from);
        }

        // using greedy instead of A* to reduce the exponential time to polynomial time( N^2logN )
        //findMIS(connect, set);
        //findMIS(connect, set, [](const Node& node) -> int { return node.edge; } );

        printf("It takes %lu vertex:", set.size());
        for(size_t j = 0; j < set.size(); ++j )
            printf(" %d", set[j] + 1);
        printf("\n\n");
    }
    return 0;
}
