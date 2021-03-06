#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "jrb.h"
int main()
{
  int i,n,a;
  double  diem;
  char name[80];
  JRB book, node;
  book = make_jrb();;
  jrb_insert_str(book, strdup("Dung"), new_jval_d(8.5));
  jrb_insert_str(book, strdup("Thang"), new_jval_d(7));
  jrb_insert_str(book, strdup("Huong"), new_jval_d(9.5));
  jrb_insert_str(book, strdup("Anh"), new_jval_d(8));
  do
    {
      //system("cls");
      printf("1.Add student\n2.Search student\n3.Printf list\n4.Delete student\n5.Search scores\n6.Exit\nYour choice:");
      scanf("%d",&n);
      switch(n)
	{
	case 1:
	  printf("Name:");
	  //fflush(stdin);
	  getchar();
	  scanf("%[^\n]s",name);
	  printf("Scores:");
	  scanf("%lf",&diem);
	  node = jrb_find_str(book, name);
	  if(node != NULL)
	    {
	      node->val = new_jval_d(diem);
	    }
	  else
	    {
	      jrb_insert_str(book, strdup(name), new_jval_d(diem));
	    }
	  break;
	case 2:
	  printf("Name:");
	  //fflush(stdin);
	  getchar();
	  scanf("%[^\n]s",name);
	  node = jrb_find_str(book, name);
	  if(node == NULL)
	    {
	      printf("Not found scores for %s!\n", name);
	    }
	  else
	    {
	      printf(" Scores of %s is %.2lf!\n", name, jval_d(node->val));
	    }
	  break;
	case 3:
	  jrb_traverse(node, book)
	    {
	      printf("%-15s%10.2lf\n",jval_s(node->key),jval_d(node->val));
	      //getch();
	    }
	  break;
	  case 4:
	   printf("Delete name:");
	  //fflush(stdin);
	  getchar();
	  scanf("%[^\n]s",name);
	  node = jrb_find_str(book, name);
	  if(node == NULL)
	    {
	      printf("Not found scores for %s!\n", name);
	    }
	  else
	    {
	      jrb_delete_node(node);
	    }
	  break;
	  case 5:
	   
	  printf("Scores:");
	  scanf("%lf",&diem);
	 jrb_traverse(node, book)
	   {
	     if(jval_d(node->val) == diem)
	    {
	      printf(" Scores of %s is %.2lf!\n", jval_s(node->key),diem);
	      a++;
	    }
	   }
	 if(a==0) printf("Not found student for scores %.2lf!\n", diem);
	  break;
	case 6:
	  jrb_traverse(node, book)
	    free(jval_s(node->key));
	  jrb_free_tree(book);
	  break;
	}
    }while(n!=6);

  return 0;
}
