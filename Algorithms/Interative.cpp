#include <iostream>
#include <vector>
#include <stack>
#include <cstdio>
using namespace std;

int global_sequence;									// biggest sequence of toppled dominos in the DAG
vector<vector<int>> graph;
stack<int> pilha;


//--- Recursive DFS ---//
void DFS (vector<vector<int>> &g, int v){
	int top;					// new_sequence -> variable that represents the number of dominos the child domino topples
	pilha.push(v);
	while(!pilha.empty()){
		top = pilha.top();
//		printf("entrei no N : %d e tenho o visited a %d\n" , top+1, g[top][0]);

		if(g[top][0] == 1){
			pilha.pop();

		}
		g[top][1] = 1 ;
//		printf("no N : %d e tenho o segment a %d\n" , top+1, g[top][1]);
		if(g[top][0] == 0){
			g[top][0] = 1;
			for (int i = 3; i < (int)g[top].size(); i++){
//				printf("N :%d | %d vs N:%d | %d\n" , top+1, g[top][1], g[top][i]+1,g[g[top][i]][1] );
				if(g[g[top][i]][1] >= g[top][1]){
					g[top][1] = g[g[top][i]][1]+1;
				}
			}
			pilha.pop();
		}
		if(g[top][0] == -1){
			g[top][0] = 0;
		}
		for (int i = 3; i < (int)g[top].size(); i++){
			if (g[g[top][i]][0] == -1){
				pilha.push(g[top][i]);
//				printf("fiz push do  : %d\n" , g[top][i] +1);
		    }
			else {
				if(g[g[top][i]][1] >= g[top][1]){
					g[top][1] = g[g[top][i]][1]+1;
				}
			}
		}
		if (g[top][1] >= global_sequence){              		// checks if its the biggest sequence of toppled dominos and if so replaces the old one
			global_sequence = g[top][1];
		}
	}

}


//--- DAG builder from STD in ---//
void readInput(){
	int n, m, v , u;
	scanf("%d %d", &n ,&m);
	graph = vector<vector<int>>(n ,vector<int>() );

//--- for that occupies/inicialize's the first 3 positions of g[i][] ---//
	for(int i=0; i < n ; i++){
		graph[i].push_back(-1);  						// g[i][0] -visited tag -1 = not visited
		graph[i].push_back(0);	 						// g[i][1] -number of dominos that this one topples if this one does
		graph[i].push_back(-1);	 						// g[i][2] -number of dominos that are pointing to this one
	}

//--- building the DAG ---//
	for(int i=0; i < m ; i++){
		scanf("%d %d", &u ,&v);
		graph[u-1].push_back(v-1);

	}
}

//--- function that calls the first instance of the DFS and that calculates how many dominos we need to topple to topple then all ---//
int the_function(vector<vector<int>> &g){
	int n, test;
	int number_dominos = 0;								//number_dominos -> variable that tracks how many dominos we need to topple to topple then all
	for(int i = 0; i < (int)g.size(); i++){
		//---Adds the number of dominos that are pointing to j domino ---//
		for (int j=3; j<(int)g[i].size(); j++){
			n = g[i][j]+1;
			graph[n-1][2]++;

		}
	}
	//--- Cycles all the dominos that don't have a domino pointing at it and does the DFS on then ---//
	for(int i = 0; i < (int)graph.size(); i++){
			test = graph[i][2];
			if (test == -1){
				number_dominos ++;
			}
			if (g[i][2] == -1){
				DFS(g, i);
			}
	}
	return number_dominos;
}



//--- main ---//
int main() {
	int number_dominos;
	readInput();
	number_dominos = the_function(graph);
	cout << number_dominos << " " << global_sequence  << endl;
	return 0;
}


