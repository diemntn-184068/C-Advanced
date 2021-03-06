#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"jrb.h"
#include"dllist.h"
typedef struct
{
  JRB edges;
  JRB vertices;
}Graph;
Graph createGraph()
{
  Graph g;
  g.edges = make_jrb();
  g.vertices = make_jrb();
  return g;
}
void addVertex(Graph g, int id, char *name)
{
  JRB node = jrb_find_int(g.vertices,id);
  if(node==NULL)
    jrb_insert_int(g.vertices,id,new_jval_v(strdup(name)));
}
char *getVertex(Graph g, int id)
{
  JRB node = jrb_find_int(g.vertices,id);
  if(node == NULL)
    return NULL;
  else
    return jval_s(node->val);
}
int adjacent(Graph graph, int v1, int v2)
{
  JRB node, tree;
  node = jrb_find_int(graph.edges, v1);
  if(node==NULL)
    return 0;
  tree = (JRB)jval_v(node->val);
  if(jrb_find_int(tree, v2)==NULL)
    return 0;
  else
    return 1;
}
void addEdge(Graph graph, int v1, int v2)
{
  JRB node, tree;
  if(adjacent(graph, v1, v2) == 0)
    {
      node = jrb_find_int(graph.edges, v1);
      if(node==NULL)
	{
	  tree = make_jrb();
	  jrb_insert_int(graph.edges, v1, new_jval_v(tree));
	}else
	{
	  tree = (JRB)jval_v(node->val);
	}
      jrb_insert_int(tree, v2, new_jval_i(1));
    }
  if(adjacent(graph, v2, v1)== 0)
    {
      node = jrb_find_int(graph.edges, v2);
      if(node==NULL)
	{
	  tree = make_jrb();
	  jrb_insert_int(graph.edges, v2, new_jval_v(tree));
	}else
	{
	  tree = (JRB)jval_v(node->val);
	}
      jrb_insert_int(tree, v1, new_jval_i(1));
    }
}

int getAdjacentVertices(Graph graph, int v, int *output)
{
  JRB tree, node;
  int total;
  node = jrb_find_int(graph.edges, v);
  if(node == NULL)
    return 0;
  tree = (JRB)jval_v(node->val);
  total = 0;
  jrb_traverse(node, tree)
    {
      output[total] = jval_i(node->key);
      total++;
    }
  return total;
}
void BFS(Graph graph, int start, int stop,void (*func)(Graph,int))
{
  int visited[1000] = {};
  int n, output[100],i , u, v;
  Dllist node, queue;
  queue = new_dllist();
  dll_append(queue, new_jval_i(start));
  while(!dll_empty(queue))
    {
      node = dll_first(queue);
      u = jval_i(node->val);
      dll_delete_node(node);
      if(visited[u]!=1)
	{
	  func(graph,u);
	  visited[u] = 1;
	  if(u == stop)return;
	  n = getAdjacentVertices(graph, u, output);
	  for(i=0; i<n; i++)
	    {
	      v = output[i];
	      if(visited[v]!=1)
		{
		  dll_append(queue, new_jval_i(v));
		}
	    }
	}
    }
}
void DFS(Graph graph, int start, int stop,void (*func)(Graph,int))
{
  int visited[1000] = {};
  int n, output[100],i , u, v;
  Dllist node, stack;
  stack = new_dllist();
  dll_append(stack, new_jval_i(start));
  while(!dll_empty(stack))
    {
      node = dll_last(stack);
      u = jval_i(node->val);
      dll_delete_node(node);
      if(visited[u]!=1)
	{
	  func(graph,u);
	  visited[u] = 1;
	  if(u == stop)return;
	  n = getAdjacentVertices(graph, u, output);
	  for(i=0; i<n; i++)
	    {
	      v = output[i];
	      if(visited[v]!=1)
		{
		  dll_append(stack, new_jval_i(v));
		}
	    }
	}
    }
}
void printVertex(Graph g,int v)
{
  printf("%10s",getVertex(g,v));
}
void dropGraph(Graph graph)
{
  JRB node, tree;
  jrb_traverse(node, graph.edges)
    {
      tree = (JRB)jval_v(node->val);
      jrb_free_tree(tree);
    }
  jrb_free_tree(graph.edges);
  jrb_free_tree(graph.vertices);
}
/*void dropGraph(Graph graph)
{
  JRB node, tree;
  jrb_traverse(node, graph.edges)
    {
      tree = (JRB)jval_v(node->val);
      jrb_free_tree(tree);
    }
  jrb_free_tree(graph.edges);
  jrb_free_tree(graph.vertices);
  }*/
int main()
{
 int i,n,output[100];
  Graph g = createGraph();
  addVertex(g,0, "Ha_Noi");
  addVertex(g,1, "Ho_Chi_Minh");
  addVertex(g,2, "Da_Nang");
  addVertex(g,3, "Da_Lat");
  addEdge(g,0,1);
  addEdge(g,1,2);
  addEdge(g,2,0);
  addEdge(g,1,3);
  printf("\nBFS: start from node 1 to all: ");
  BFS(g, 1, -1, printVertex);
  printf("\nDFS: start from node 1 to all: ");
  DFS(g, 1, -1, printVertex);
  printf("\n");
  dropGraph(g);
  return 0;
}
  
