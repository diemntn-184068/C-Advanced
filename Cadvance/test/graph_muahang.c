#include "graph_muahang.h"

Graph createGraph()
{
    Graph G;
    G.edges = make_jrb();
    G.vertices = make_jrb();
    return G;
}

// Thêm định danh ID cho đỉnh
void addVertex(Graph G, int id, char* name)
{
    if (!jrb_find_int(G.vertices, id))
        jrb_insert_int(G.vertices, id, new_jval_s(strdup(name)));
}

// Lấy đỉnh theo ID
char *getVertex(Graph G, int id)
{
    JRB node = jrb_find_int(G.vertices, id);
    if (!node)
        return NULL;
    return jval_s(node->val);
}

double getEdgeValue(Graph G, int v1, int v2)
{
    JRB node = jrb_find_int(G.edges, v1); // Kiểm tra v1 có đỉnh kề không
    if (!node)
        return INFINITIVE_VALUE;
    node = jrb_find_int((JRB) jval_v(node->val), v2); // Tìm v2 trong các đỉnh kề của v1
    if (node)
        return jval_d(node->val); // Nếu tìm thấy trả về khoảng cách v1 -> v2
    else
        return INFINITIVE_VALUE;
}

// Thêm liên kết v1->v2
void addEdge(Graph G, int v1, int v2, double weight)
{
  JRB node, tree, node1, node2;
    if (getEdgeValue(G, v1, v2) == INFINITIVE_VALUE)
    {
        node = jrb_find_int(G.edges, v1);
        if(!node)
        {
            tree = make_jrb();
            jrb_insert_int(G.edges, v1, new_jval_v(tree));
        }
        else
            tree = (JRB) jval_v(node->val);
        jrb_insert_int(tree, v2, new_jval_d(weight));
    }
    
    if (getEdgeValue(G, v2, v1) == INFINITIVE_VALUE)
    {
        node = jrb_find_int(G.edges, v2);
        if(!node)
        {
            tree = make_jrb();
            jrb_insert_int(G.edges, v2, new_jval_v(tree));
        }
        else
            tree = (JRB) jval_v(node->val);
        jrb_insert_int(tree, v1, new_jval_d(weight));
    }
    else
      {
	double w=getEdgeValue(G, v2, v1);
	
	node1 = jrb_find_int(G.edges, v2);
	node1= jrb_find_int((JRB) jval_v(node1->val), v1);
	node1->val=new_jval_d(w+1);
        node2 = jrb_find_int(G.edges, v1);
	node2= jrb_find_int((JRB) jval_v(node2->val), v2);
	node2->val=new_jval_d(w+1);
	
      }
}

// Liên kết từ v đến các đỉnh khác => Mảng và số phần tử
int outdegree(Graph G, int v, int* output)
{
    JRB node, tree;
    node = jrb_find_int(G.edges, v);
    if (!node)
        return 0;
    
    int total = 0;
    tree = (JRB) jval_v(node->val);
    jrb_traverse(node, tree)
    {
        output[total] = jval_i(node->key);
        total++;
    }
    return total;
}

// Liên kết từ các đỉnh khác đến v => Mảng và số phần tử
int indegree(Graph G, int v, int* output)
{
    JRB node, tree, iter;
    int total = 0;

    // Duyệt tất cả các đỉnh: Tìm liên kết từ đỉnh đến v
    jrb_traverse(iter, G.vertices)
    {
        node = jrb_find_int(G.edges, jval_i(iter->key));
        if (node) // Nếu đỉnh ko phải là sink vertex
        {
            tree = (JRB) jval_v(node->val);
            if (jrb_find_int(tree, v)) // Nếu đỉnh có liên kết đến v
            {
                output[total] = jval_i(node->key);
                total++;
            }
        }
    }
    return total;
}

// Kiểm tra DAG
int isDAG(Graph G)
{
    JRB iter;
    int n, output[100], visited[1000];
    Dllist stack = new_dllist();
    int vertice, start;

    // Duyệt theo chiều sâu bắt đầu từ từng đỉnh
    jrb_traverse(iter, G.vertices)
    {
        memset(visited, 0, sizeof(visited));
        start = jval_i(iter->key);
        dll_prepend(stack, new_jval_i(start));
        while (!dll_empty(stack))
        {
            vertice = jval_i(dll_first(stack)->val);
            dll_delete_node(dll_first(stack));

            n = outdegree(G, vertice, output);
            for (int i = 0; i < n; ++i)
            {
                vertice = output[i];
                if (vertice == start)
                {
                    free_dllist(stack);
                    return 0;
                }
                if (!visited[vertice])
                    dll_prepend(stack, new_jval_i(vertice)); 
            }
        }
    }
    free_dllist(stack);
    return 1;
}
// Từ một đỉnh: duyệt qua tất cả đỉnh có liên kết từ đỉnh đó
// Nếu đỉnh liên kết có liên kết đến đỉnh bắt đầu => Không phải DAG => return 0;
// Nếu đỉnh liên kết không có liên kết đến đỉnh bắt đầu
// + Nếu đã visited => bỏ qua
// + Nếu chưa visited => Thêm vào stack

void dropGraph(Graph G)
{
    JRB iter;
    if (G.vertices != NULL)
        jrb_free_tree(G.vertices);

    if (G.edges != NULL)
    {
        jrb_traverse(iter, G.edges)
            jrb_free_tree((JRB) jval_v(iter->val));
        jrb_free_tree(G.edges);
    }

}
int getAdjacentVertices(Graph graph, int v, int *output, double *w)
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
      w[total]=jval_d(node->val);
      output[total] = jval_i(node->key);
      total++;
    }
  return total;
}
double shortestPath(Graph G, int s, int t, int* path, int* length)
{
    int previous[1000]; // Lưu các node trước đó, dùng để tìm đường đi (path)
    double distance[1000]; // Khoảng cách ngắn nhất giữa S đến các đỉnh
    int add[1000]; // Kiểm tra node đã visited
    Dllist queue = new_dllist();
    Dllist iter;
    Dllist node;
    int u;
    //printf("S = %d - T = %d\n", s, t);
    for (int i = 0; i < 100; ++i)
        distance[i] = INFINITIVE_VALUE;
    distance[s] = 0;
    previous[s]=s;
     memset(add, 0, sizeof(int)*100);
    dll_append(queue, new_jval_i(s));
    add[s] = 1;

    while (!dll_empty(queue))
    {
        // Lấy ra đỉnh có khoảng cách gần s nhất trong queue
        double min = INFINITIVE_VALUE;
	// printf("QUEUE: ");
        dll_traverse(iter, queue)
        {
            u = jval_i(iter->val);
            //printf("%d - %.0lf | ", u, distance[u]);
            if (min > distance[u])
            {
                min = distance[u];
                node = iter;
            }
        }
        u = jval_i(node->val);
        //printf("min = %.0lf | POP: %d\n", min, u);
        dll_delete_node(node);

        if (u == t)
	  {
	   
            break;
	  } 
        // Cập nhật lại khoảng cách của các đỉnh kề u so với s
        // So khoảng cách d[u] và d[v] + c(u, v)
        int n, v, output[1000];
	double w, we[100];
        n = getAdjacentVertices(G, u, output, we);
        for (int i = 0; i < n; ++i)
        {
            v = output[i];
            w = getEdgeValue(G, u, v);
            if (distance[v] > distance[u] + w)
            {
                distance[v] = distance[u] + w;
                previous[v] = u;
            }

	     if (!add[v])
	     {
	          add[v] = 1;
                dll_append(queue, new_jval_i(v));
	      }    
        }
    }
    // Tìm đường đi (path), số đỉnh đi qua (length) và tổng quãng đường (return) (từ previous)
    int n, tmp[1000];
    if(distance[t] != INFINITIVE_VALUE)
    {
        // Path
        tmp[0] = t;
        n = 1;
        u = t;
	while(u != s)
        {
            u = previous[u];
            tmp[n] = u;
            n++;
        }
        *length = n;
        // Reverse
       
	for(int a = n-1 ; a >= 0 ; a--)
	  path[n-a-1] = tmp[a];
	  }
    free_dllist(queue);
    return distance[t];

    }
