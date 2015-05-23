# find_max_independent_set
This program provides a polynomial time heurist approach to find a max independent set

Input Format.
The first line of each case containes two numbers: Vnum and Enum, where Vnum and Enum are the number of vertices and edges of a graph.
The following Enum lines describe the edges of this graph.
Each line has two numbers v1 and v2 which tell that there is an undirected edge between v1 and v2.

Sample Input.
4 6
1 2
1 3
1 4
2 3
2 4
3 4

This sample input describes a complete graph with 4 vertices.
Another sample input is 'case.txt'.

#----------------------------------------------------------------------------------------------------------------------

This program also provides a polynomial time heuristic to find a min-cost MIS where each vertex in the graph has cost.

Input Format.
The first line of each case containes two numbers: Vnum and Enum, where Vnum and Enum are the number of vertices and edges of a graph.
The following Vnum lines detail the cost of every vertex where the i+1 line is the cost of the ith vertex.
Then the remaining Enum lines describe the edges of this graph.
Each line has two numbers v1 and v2 which tell that there is an undirected edge between v1 and v2.

Sample Input.
4 6
4
3
2
1
1 2
1 3
1 4
2 3
2 4
3 4

This sample input describes a complete graph with 4 vertices where vertex cost of vertex 1, 2, 3 and 4 are 4, 3, 2, and 1 respectively.
Another sample input is 'cost.txt'.
