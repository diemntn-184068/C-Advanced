#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <conio.h>
#define INITIAL_SIZE 10
#define INCREMENTAL_SIZE 5
typedef struct
{
  char name[80];
  int  number;
}PhoneEntry;
typedef struct
{
  PhoneEntry *entries;
  int total;
  int size;
}PhoneBook;
PhoneBook createPhoneBook()
{
  PhoneBook p;
  p.total = 0;
  p.size =  INITIAL_SIZE;
  p.entries = ( PhoneEntry*)malloc( INITIAL_SIZE*sizeof(PhoneEntry));
  return p;
}
void dropPhoneBook(PhoneBook *book)
{
  free(book->entries);
  book->total = 0;
  book->size =0;
}
int binarySearch(PhoneEntry *entries, int l, int r, char *name, int *found)
{
  int i, res;
  if(r < l)
    {
      *found = 0;
      return l;
    }
  i = (l+r)/2;
  res = strcmp(name, entries[i].name);
  if(res == 0)
    {
      *found =1;
      return i;
    }
  else if(res < 0)
    {
      return binarySearch(entries, l, i-1,name, found) ;
    }
  else
    {
      return binarySearch(entries, i+1, l,name, found) ;
    }
}
void addPhoneNumber(char *name, int number,PhoneBook *book)
{
  int found, pos;
  if(book->total > 0)
    {
      pos = binarySearch(book->entries, 0, book->total-1,name, &found);
    }
  else
    {
      pos =0;
      found =0;
    }
  if(found)
    {
      book -> entries[pos].number = number;
    }
  else
    {
      if(book->total >= book->size)
	{
	  PhoneEntry *p;
	  book->size += INCREMENTAL_SIZE;
	  (book->entries) = (PhoneEntry*)realloc(book->entries, sizeof(PhoneEntry)*( book->size));
	}
      if(pos < book->total)
	{
	  memcpy(&(book->entries[pos+1]),&(book->entries[pos]), (book->total-pos)*sizeof(PhoneEntry));
	}
      strcpy(book->entries[pos].name,name);
      book->entries[pos].number = number;
      book->total++;
    }
}
PhoneEntry *getPhoneNumber(char *name, PhoneBook book)
{
  int found, pos;
  pos = binarySearch(book.entries, 0, book.total-1,name, &found);
  if(found)
    {
      return &(book.entries[pos]);
    }
  else return NULL;
}
int main()
{
  int i,n;
  int sdt;
  char name[80];
  PhoneEntry *entries;
  PhoneBook book;
  book = createPhoneBook();
  addPhoneNumber("Anh", 12835744, &book);
  addPhoneNumber("Dung", 89896768, &book);
  addPhoneNumber("Diem", 01234567, &book);
  do
    {
      //system("cls");
      printf("1.Add phone\n2.Search phone\n3.Printf list\n4.Exit\nYour choice:");
      scanf("%d",&n);
      switch(n)
	{
	case 1:
	  printf("Name:");
	  //fflush(stdin);
	  getchar();
	  scanf("%[^\n]s",name);
	  printf("Number:");
	  scanf("%d",&sdt);
	  addPhoneNumber(name,sdt, &book);
	  break;
	case 2:
	  printf("Name:");
	  //fflush(stdin);
	  getchar();
	  scanf("%[^\n]s",name);
	  PhoneEntry *s;
	  if(getPhoneNumber(name,book)==NULL)
	    printf("Khong tim thay!!!\n");
	  else
	    {
	    s=getPhoneNumber(name,book);
	    printf("Tim thay %s - %d\n",s->name,s->number);
	    }
	  break;
	case 3:
	  for(int i=0; i< book.total; i++)
	    {
	      printf("%-15s%10d\n",book.entries[i].name,book.entries[i].number);
	      
	    }
	  break;
	case 4:
	  dropPhoneBook(&book);
	  break;
	}
    }while(n!=4);
  return 0;
}
