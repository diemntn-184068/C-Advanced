#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "graph_muahang.h"
Graph input_graph(FILE *fptr1, FILE *fptr2)
{
    Graph G = createGraph();
    char name[20];
    int id,id1,id2,id3;
    double weight=1;
    while(!feof(fptr1))
      {
        fscanf(fptr1, "%d %s\n", &id, name);
	addVertex(G, id, name);
      }
    fscanf(fptr2, "%d %d %d\n", &id1, &id2, &id3);
    addEdge(G,id1,id2,weight);
    addEdge(G,id3,id2,weight);
    addEdge(G,id1,id3,weight);
    printf("%s  %s  %s\n",getVertex(G,id1),getVertex(G,id2),getVertex(G,id3));
    fscanf(fptr2, "%d %d %d\n", &id1, &id2, &id3);
    addEdge(G,id1,id2,weight);
    addEdge(G,id3,id2,weight);
    addEdge(G,id1,id3,weight);
    printf("%s  %s  %s\n",getVertex(G,id1),getVertex(G,id2),getVertex(G,id3));
     while(!feof(fptr2))
      {
        fscanf(fptr2, "%d %d\n", &id1, &id2);
	addEdge(G,id1,id2,weight);
	printf("%s  %s\n",getVertex(G,id1),getVertex(G,id2));
      }
    return G;
}

int main()
{
  Graph G;
  int id,n;
	char name[20];
	FILE *fptr1,*fptr2;
	
	do
	{
		printf("----------MENU------------\n");
		printf("1.In danh sach san pham\n");
		printf("2.In danh sach giao dich\n");
		printf("3.Hien thi muc do lien quan giua 2 san pham\n");
		printf("4.In dach sach cac san pham lien quan\n");
		printf("5.Hien thi moi lien he giua 2 san pham\n");
		printf("6.Exit\n");
		printf("your choice\n");
		scanf("%d%*c",&n);
		switch(n)
		{
			case 1:
		       fptr1=fopen("products.txt","r");
		       if (fptr1==NULL)
                 	{
                	  printf("error\n");
                 		return 0;
                	}
			  while(!feof(fptr1))
			    {
                              fscanf(fptr1, "%d %s\n", &id, name); 
			      printf("Ma san pham: %d\n",id);
			      printf("Ten san pham: %s\n",name);
			      
			    }
			  fclose(fptr1);
			break;
			case 2:
			  fptr1=fopen("products.txt","r");
                          fptr2=fopen("orderhistory.txt","r");
                	if (fptr1==NULL)
                  	{
                	  printf("error\n");
          		return 0;
                 	}
                	if (fptr2==NULL)
                 	{
             		printf("error\n");
         		return 0;
                	}
			 G= input_graph(fptr1,fptr2);
			 fclose(fptr1);
	                 fclose(fptr2);
			break;
		
			case 3:
			{
			  int a,b;
			  double w;
			  JRB tree;
			  printf("Nhap ma 2 san pham:");
			  scanf("%d",&a);
			  scanf("%d",&b);
			  w=getEdgeValue(G, a, b);
			  if(w==INFINITIVE_VALUE)
			    printf("Do lien he giua 2 san pham la: -1\n");
			  else
			    {
			      printf("Do lien he giua 2 san pham la: %.0lf\n",w);
			    }
				break;
			}
			case 4:
			{
			  int output[10],v;
			  double w[10];
			  printf("Nhap ma san pham: ");
			  scanf("%d",&v);
			  int n= getAdjacentVertices(G, v,output,w);
			  
			  int i,j,max,tmp2;
                          double tmp1;
                         for(i=0;i<n-1;i++)
                          {
                            max=i;
                            for(j=i+1;j<=n-1;j++)
                        	{

                         	  if(w[j]>w[max]) max=j;
	  
                        	}
                          tmp1=w[i];
                          w[i]=w[max];
                          w[max]=tmp1;
			  
                          tmp2=output[i];
                          output[i]=output[max];
                          output[max]=tmp2;
                          }
 
			   printf("Cac san pham lien quan:");
			  for(int a=0;a<n; a++)
                           {
			     printf(" %s ",getVertex(G,output[a]));
			   }
			  printf("\n");
				break;
			}
			case 5:
			{
			  int s,t,path[100], length;
			  double d;
			  printf("Nhap ma 2 san pham: ");
			  scanf("%d",&s);
			  scanf("%d",&t);
			  d=shortestPath(G,s,t,path,&length);
			  if(d==INFINITIVE_VALUE)
			    printf("Hai san pham nay khong co lien quan voi nhau(^_^)\n");
			  else
			    {
			   for(int a=0;a<length; a++)
                           {
			     printf(" %s ",getVertex(G,path[a]));
			     if(a!=length-1)
			       printf("-");
			   }
			  printf("\n");
			    }
				break;
			}
		        case 6:
			  {
			    dropGraph(G);
			    break;
			  }
	}
	} while (n!=6);
	return 0;
}
