#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"jrb.h"
#include"dllist.h"
#include"jval.h"
#define INFINITIVE_VALUE 1000000
typedef struct
{
  JRB edges;
  JRB vertices;
}Graph;
Graph createGraph();
void addVertex(Graph graph,int id,char* name);
char* getVertex(Graph graph,int id);
void addEdge(Graph graph,int v1, int v2,double weight);
int indegree(Graph graph,int v,int* output);
int outdegree(Graph graph,int v, int* output);
int DAG(Graph graph);
void dropGraph(Graph graph);
double getEdgeValue(Graph graph,int v1,int v2);
double shortestPath(Graph g,int s,int t,int* path,int* length);

int main()
{
  int i,length,path[100],n;/*output[100]*/
  double w;
  Graph g = createGraph();
  addVertex(g,0, "Ha_Noi");
  addVertex(g,1, "Da_Nang");
  addVertex(g,2, "Ho_Chi_Minh");
  addVertex(g,3, "Da_Lat");
  addEdge(g,0,1,1);
  addEdge(g,1,2,3);
  addEdge(g,2,0,3);
  addEdge(g,1,3,1);
  addEdge(g,3,2,1);
  int s=0, t=2;
  /*if(DAG(g)==1)
    printf("The graph is acycle\n");
  else
    printf("Have cycles in the graph\n");
  printf("Co the di tu Ho Chi Minh den: ");
  n= outdegree( g, 1, output);
  for(int a=0; a<n-1;a=a+2)
    printf("%10s",getVertex( g, output[a]));
  printf("\n");

  */
  w=shortestPath(g, s, t,path, &length);
  if(w==INFINITIVE_VALUE)
    {
      printf("No path from %s to %s\n",getVertex(g,s),getVertex(g,t));
    }
  else
    {
      printf("Path from %s to %s(with total distance %.2f)\n",getVertex(g,s),getVertex(g,t),w);
      printf("Route");
      for(i=0; i<length; i++)
	printf("=>%s",getVertex(g,path[i]));
    }
  printf("\n");
  dropGraph(g);
  
}

Graph createGraph()
{
    Graph g;
    g.edges = make_jrb();
    g.vertices = make_jrb();
    return g;
}


void addVertex(Graph g,int id,char* name)
{
    JRB node = jrb_find_int(g.vertices,id);
    if(node==NULL)
    jrb_insert_int(g.vertices,id,new_jval_s(strdup(name)));
}


char* getVertex(Graph g,int id)
{
    JRB node = jrb_find_int(g.vertices,id);
    if(node==NULL) return NULL;
    else return jval_s(node->val);
}

double getEdgeValue(Graph graph,int v1,int v2)
{
    JRB node,tree;
    node = jrb_find_int(graph.edges,v1);
    if(node==NULL) return INFINITIVE_VALUE;
    tree = (JRB) jval_v(node->val);
    node = jrb_find_int(tree,v2);
    if(node==NULL) return INFINITIVE_VALUE;
    else return jval_d(node->val);
}


void addEdge(Graph graph,int v1,int v2,double weight)
{
    JRB node, tree;
    if(getEdgeValue(graph,v1,v2)==INFINITIVE_VALUE)
    {
        node = jrb_find_int(graph.edges,v1);
        if(node==NULL) 
        {
            tree = make_jrb();
            jrb_insert_int(graph.edges,v1,new_jval_v(tree));
            jrb_insert_int(tree,v2,new_jval_d(weight));
        }else
        {
            tree = (JRB) jval_v(node->val);
            jrb_insert_int(tree,v2,new_jval_d(weight));
        }
    }
}


int outdegree(Graph graph,int v, int* output)
{
    JRB tree, node;
    int total;
    node = jrb_find_int(graph.edges,v);
    if(node==NULL) return 0;
    tree =(JRB) jval_v(node->val);
    total = 0;
    jrb_traverse(node,tree)
    {
        output[total]=jval_i(node->key);
        total++;
    }
    return total;
}


int indegree(Graph graph,int v,int* output)
{
    JRB tree,node;
    int total =0;
    jrb_traverse(node,graph.edges)
    {
        tree = (JRB) jval_v(node->val);
        if(jrb_find_int(tree,v))
        {
            output[total]= jval_i(node->key);
            total++;
        }
    }   
    return total;
}


int DAG(Graph graph)
{
    int visited[100];
    int output[100],n,i,u,v,start;
    Dllist node, stack;
    JRB vertex;


    jrb_traverse(vertex,graph.vertices)
    {
        memset(visited,0,sizeof(visited));
        start = jval_i(vertex->key);
        stack = new_dllist();
        dll_append(stack,new_jval_i(start));
        while(!dll_empty(stack))
        {
            node = dll_last(stack);
            u = jval_i(node->val);
            dll_delete_node(node);
            if(!visited[u])
            { 
                visited[u]= 1;
                n = outdegree(graph,u,output);
            for(i=0;i<n;i++)
            {
                v = output[i];
                if(v==start) return 0;
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
    jrb_traverse(node,graph.edges)
    {
        tree =(JRB) jval_v(node->val);
        jrb_free_tree(tree);
    }
    jrb_free_tree(graph.edges);
    jrb_free_tree(graph.vertices);
}


double shortestPath(Graph g,int s,int t,int* path,int* length)
{
    double distance[1000],min,w,total;
    int previous[1000], tmp[1000];
    int n,output[1000],i,u,v,start;
    Dllist ptr,queue,node;

    for(i=0;i<1000;i++)
        distance[i]= INFINITIVE_VALUE;
        distance[s]=0;
        previous[s]=s;
        queue = new_dllist();
        dll_append(queue,new_jval_i(s));
        while(!dll_empty(queue))
        {
            min = INFINITIVE_VALUE;
            dll_traverse(ptr,queue)
            {
                u = jval_i(ptr->val);
                if(min>distance[u])
                {
                    min = distance[u];
                    node = ptr;
                }
            }
            dll_delete_node(node);
            if(u==t) break;
            n = outdegree(g,u,output);
            for(i=0;i<n;i++)
            {
                v = output[i];
                w = getEdgeValue(g,u,v);
                if(distance[v]>distance[u]+w);
                {
                    distance[v]= distance[u]+w;
                    previous[v]=u;
                }
                dll_append(queue,new_jval_i(v));
            }
        }
        total = distance[t];
        if(total!=INFINITIVE_VALUE)
        {
            tmp[0] = t;
            n = 1;
            while(t!=s)
            {
                t = previous[t];
                tmp[n++]= t;
            }
            for(i=n-1;i>=0;i--)
            {
                path[n-i-1]=tmp[i];
            }
            *length = n;
        }   
    return total;
}
