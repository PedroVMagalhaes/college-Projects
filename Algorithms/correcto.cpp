#include <iostream>
#include <vector>
#include <cstdio>
using namespace std;

int global_sequence;
vector<vector<int>> graph;



int DFS (vector<vector<int>> &g, int v){
	int size = 0, new_sequence;
	g[v][1] ++;
	g[v][0] = 1;
	size = (int)g[v].size();
	for(int i = 3; i < size; i++){
		if(g[g[v][i]][0] == -1){
			new_sequence = DFS(g , g[v][i]);
			if(new_sequence >= g[v][1]){
				g[v][1] = new_sequence+1;
			}
		}
		else{
			if(g[g[v][i]][1] >= g[v][1]){
				g[v][1] = g[g[v][i]][1]+1;
			}
		}
	}
	if (g[v][1] >= global_sequence){
		global_sequence = g[v][1];
	}
	return g[v][1];
}


void readInput(){
	int n, m, v , u;
	scanf("%d %d", &n ,&m);
	graph = vector<vector<int>>(n ,vector<int>() );
	for(int i=0; i < n ; i++){
		graph[i].push_back(-1);
		graph[i].push_back(0);
		graph[i].push_back(-1);
	}
	for(int i=0; i < m ; i++){
		scanf("%d %d", &u ,&v);
		graph[u-1].push_back(v-1);

	}
}

int the_function(vector<vector<int>> &g){
	int n, test;
	int number_dominos = 0;
	for(int i = 0; i < (int)g.size(); i++){
		for (int j=3; j<(int)g[i].size(); j++){
			n = g[i][j]+1;
			graph[n-1][2]++;

		}
	}
	for(int i = 0; i < (int)graph.size(); i++){
		test = graph[i][2];
		if (test == -1){
			number_dominos ++;
			DFS(g, i);
		}
	}
	return number_dominos;
}


void printGraph (vector<vector<int>> &g){
	for(int i = 0; i < (int)g.size(); i++){
		for (int j=0; j<(int)g[i].size(); j++){
			if (j != 1){
				printf("%d %d\n", i+1 , g[i][j]+1) ;
			}
		}
	}
}







int main() {
	int number_dominos;
	readInput();
//	printGraph(graph);
	number_dominos = the_function(graph);
	cout << number_dominos << " " << global_sequence  << endl;
	return 0;
}

