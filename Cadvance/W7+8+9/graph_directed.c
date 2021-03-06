#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"jrb.h"
#include"dllist.h"
#include"jval.h"
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
int hasEdge(Graph graph, int v1, int v2)
{
  JRB node, tree;
  node = jrb_find_int(graph.edges,v1);
  if(node == NULL)
    return 0;
  tree = (JRB)jval_v(node->val);
  if(jrb_find_int(tree,v2)==NULL)
    return 0;
  else
    return 1;
}
void addEdge(Graph graph, int v1, int v2)
{
  JRB node,tree;
  if(hasEdge(graph,v1,v2)==0)
    {
      node = jrb_find_int(graph.edges,v1);
      if(node==NULL)
    {
	tree=make_jrb();
      jrb_insert_int(graph.edges,v1,new_jval_v(tree));
      jrb_insert_int(tree, v2, new_jval_i(1));
    }
  else
    {
      tree = (JRB)jval_v(node->val);
      jrb_insert_int(tree, v2, new_jval_i(1));
    }
   }
}
int outdegree(Graph graph, int v, int *output)
{
  JRB tree, node;
  int total;
  node=jrb_find_int(graph.edges,v);
  if(node == NULL)
    return 0;
  tree = (JRB)jval_v(node->val);
  total = 0;
  jrb_traverse(node,tree)
    {
      output[total++]=jval_i(node->key);
      total++;
    }
  return total;
}
int indegree(Graph graph, int v, int *output)
{
  JRB tree, node;
  int total=0;
  jrb_traverse(node,graph.edges)
    {
      tree = (JRB)jval_v(node->val);
      if(jrb_find_int(tree,v))
	{
	  output[total]=jval_i(node->key);
	  total++;
	}
    }
  return total;
}
int DAG(Graph graph)
  {
    int visited[1000],n,output[100],i,u,v,start;
    Dllist node,stack;
    JRB vertex;
    jrb_traverse(vertex,graph.vertices)
      {
	memset(visited,0,sizeof(visited));
	start = jval_i(vertex->key);
	stack=new_dllist();
	dll_append(stack,new_jval_i(start));
	while(!dll_empty(stack))
	  {
	    node = dll_last(stack);
	    u = jval_i(node->val);
	    dll_delete_node(node);
	    if(!visited[u])
	      {
		visited[u]=1;
		n = outdegree(graph,u,output);
		for(i=0; i<n; i++)
		  {
		    v=output[i];
		    if(v==start)
		      return 0;
		    if(!visited[v])
		     dll_append(stack,new_jval_i(v)); 
		  }
	      }
	  }
      }
    return 1;
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
  if(DAG(g)==1)
    printf("The graph is acycle\n");
  else
    printf("Have cycles in the graph\n");
  printf("Co the di tu Ho Chi Minh den: ");
  n= outdegree( g, 1, output);
  for(int a=0; a<n-1;a=a+2)
    printf("%10s",getVertex( g, output[a]));
  printf("\n");
  dropGraph(g);
  return 0;
}
