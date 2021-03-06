#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"jrb.h"
typedef JRB Graph;
Graph createGraph()
{
  return make_jrb();
}
int adjacent(Graph graph, char *v1, char *v2)
{
  JRB node, tree;
  node = jrb_find_str(graph, v1);
  if(node==NULL)
    return 0;
  tree = (JRB)jval_v(node->val);
  if(jrb_find_str(tree, v2)==NULL)
    return 0;
  else
    return 1;
}
void addEdge(Graph graph, char *v1, char *v2)
{
  JRB node, tree;
  if(adjacent(graph, v1, v2) == 0)
    {
      node = jrb_find_str(graph, v1);
      if(node==NULL)
	{
	  tree = make_jrb();
	  jrb_insert_str(graph, v1, new_jval_v(tree));
	}else
	{
	  tree = (JRB)jval_v(node->val);
	}
      jrb_insert_str(tree, v2, new_jval_i(1));
    }
  if(adjacent(graph, v2, v1)== 0)
    {
      node = jrb_find_str(graph, v2);
      if(node==NULL)
	{
	  tree = make_jrb();
	  jrb_insert_str(graph, v2, new_jval_v(tree));
	}else
	{
	  tree = (JRB)jval_v(node->val);
	}
      jrb_insert_str(tree, v1, new_jval_i(1));
    }
}

int getAdjacentVertices(Graph graph, char *v, char output[][30])
{
  JRB tree, node;
  int total;
  node = jrb_find_str(graph, v);
  if(node == NULL)
    return 0;
  tree = (JRB)jval_v(node->val);
  total = 0;
  jrb_traverse(node, tree)
    {
      strcpy(output[total],jval_s(node->key));
      total++;
    }
  return total;
}
void dropGraph(Graph graph)
{
  JRB node, tree;
  jrb_traverse(node, graph)
    {
      tree = (JRB)jval_v(node->val);
      jrb_free_tree(tree);
    }
  jrb_free_tree(graph);
}

int main()
{
  int n;
  char output[10][30];
  Graph g = createGraph();
  addEdge(g, "Ha_Noi", "Da_Nang");
  addEdge(g, "Ha_Noi", "Ho_Chi_Minh");
  addEdge(g, "Da_Nang", "Ho_Chi_Minh");
  addEdge(g, "Da_Nang", "Da_Lat");
  n = getAdjacentVertices(g, "Ha_Noi", output);
  if(n == 0)
    printf("No adjacent vertices of node Ha Noi\n");
  else
    {
      printf("Adjacent vertices of node Ha Noi : ");
      for(int i=0; i<n; i++)
	{
	  printf("%20s",output[i]);
	}
      printf("\n");
      }
  dropGraph(g);
  return 0;
}
  
