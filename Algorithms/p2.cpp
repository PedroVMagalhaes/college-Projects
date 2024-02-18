#include<cstdio>
#include<queue>
#include<cstring>
#include<vector>
#include<iostream>
using namespace std;
int c[10][10];
int flowPassed[10][10];
vector<int> g[10];
int parList[10];
int currentPathC[10];
int bfs(int sNode, int eNode)//breadth first search
{
   memset(parList, -1, sizeof(parList));
   memset(currentPathC, 0, sizeof(currentPathC));
   queue<int> q;//declare queue vector
   q.push(sNode);
   parList[sNode] = -1;//initialize parlist’s source node
   currentPathC[sNode] = 999;//initialize currentpath’s source node
   while(!q.empty())// if q is not empty
   {
      printf("entrei no while \n");
      int currNode = q.front();
      printf("currNode: %d \n", currNode);
      q.pop();
      for(int i=0; i<g[currNode].size(); i++){
         printf("g[currNode].size(): %d \n", g[currNode].size());
         int to = g[currNode][i];
         printf("to = g[currNode][i]: %d \n", to);
         if(parList[to] == -1)
         {
             printf("if parList[to] == -1 \n");
             printf("c[currNode][to] %d - flowPassed[currNode][to] %d\n\n", c[currNode][to], flowPassed[currNode][to]);


            if(c[currNode][to] - flowPassed[currNode][to] > 0)
            {
               parList[to] = currNode;
               currentPathC[to] = min(currentPathC[currNode],
               c[currNode][to] - flowPassed[currNode][to]);
               if(to == eNode){
                  return currentPathC[eNode];
               }
               q.push(to);
            }
         }
      }
   }
   return 0;
}
int edmondsKarp(int sNode, int eNode)
{
   int maxFlow = 0;
   while(true)
   {
      int flow = bfs(sNode, eNode);
      if (flow == 0)
      {
         break;
      }
      maxFlow += flow;
      int currNode = eNode;
      while(currNode != sNode)
      {
         int prevNode = parList[currNode];
         flowPassed[prevNode][currNode] += flow;
         flowPassed[currNode][prevNode] -= flow;
         currNode = prevNode;
      }
   }
return maxFlow;
}
int main()
{
   int nodCount, edCount;
   cin>>nodCount>>edCount;
   int source = 0, sink = nodCount+1 ;
   for(int ed = 0; ed < nodCount; ed++)
   {
      int cap1, cap2;
      cin>>cap1>>cap2;
      c[source][ed+1] = cap1;
      printf("SOURCE c[%d][%d]: %d \n", source+1, ed+2, c[source][ed+1]);
      g[source].push_back(ed+1);
      g[ed+1].push_back(source);

      c[sink][ed+1] = cap2;
      printf("SINK c[%d][%d]: %d \n", sink+1, ed+2, c[sink][ed+1]);
      g[sink].push_back(ed+1);
      g[ed+1].push_back(sink);
   }
   for(int ed = 0; ed < edCount; ed++)
   {
      int from, to, cap;
      cin>>from>>to>>cap;
      c[from][to] = cap;
      printf("valor c[%d][%d]: %d \n", from+1, ed+1, c[from][to]);
      g[from].push_back(to);
      g[to].push_back(from);
   }
   int maxFlow = edmondsKarp(source, sink);
   cout<<endl<<endl<<"Max Flow is:"<<maxFlow<<endl;
}