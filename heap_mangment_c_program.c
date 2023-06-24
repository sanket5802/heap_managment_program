#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Lists
{
    char Name[100];
    int start;
    int end;
    int isFree;
    struct Lists *next;
    struct Lists *prev;
} List;

typedef struct heap
{
    List *lptr;
} heap;
//********************************************************************
List *getnewNode(int start, int end)
{
    List *nptr = (List*)malloc(sizeof(List));
   
    if(nptr!=NULL) {       //nptr will be null if it was not able to allocate necessary space for nptr
        nptr->start = start;
        nptr->end = end;
        nptr->next = NULL;
        nptr->prev = NULL;
        nptr->isFree = 1;
    }
    return nptr;
}

List *AllocateNode(int start, int end, char name[])
{
    List *aptr = (List*)malloc(sizeof(List));

    if(aptr!=NULL) {
        aptr->start = start;
    	aptr->end = end;
    	strcpy(aptr->Name, name);
        aptr->isFree = 0;
    	aptr->next = NULL;
        aptr->prev = NULL;
    }
	return aptr;
}

int Empty(heap *hp)
{
    int retVal = 1;
    List *lptr = hp->lptr;
    while(lptr!=NULL && lptr->isFree) {
        lptr = lptr->next;
    }
    if(lptr!=NULL)
        retVal=0;
    return retVal;
}

int sameName(heap *hp, char name[])
{
    List *lptr = hp->lptr;
    int retVal = 0;
    while(lptr!=NULL) {
        if(!(lptr->isFree) && strcmp(name, lptr->Name)==0)
            retVal = 1;
        lptr = lptr->next;
    }
    return retVal;
}

void allocateSpace(heap *hp, char name[], int size)
{
    List *lptr = hp->lptr;
    if(lptr == NULL) {
        printf("SPACE NOT AVAILABLE!!!\n");
    }
    else {
        List *nptr = lptr;
        List *ptr = NULL;
        int status = 0; 
        while(nptr!=NULL && !status) {
            if(nptr->isFree && (nptr->end - nptr->start + 1) >= size) {
                ptr = nptr;
                status = 1;
            }
            nptr = nptr->next;
        }
        int index;
        if(ptr!=NULL)
        {
            index = ptr->start + size -1;
        }
        if(ptr == NULL || ( ptr->next==NULL && (index > 999) ) ) 
        {
            printf("HEAP IS FULL!!!!\n");
        }
        else {
            List *aptr = AllocateNode(ptr->start, index, name);
            if(aptr==NULL) {
                printf("ALLOCATION FAILED!!!\n");
            }
            else {
                if(index == ptr->end) {
                    lptr = aptr;
                    if(ptr->prev!=NULL) {
                        ptr->prev->next = lptr;
                        lptr->prev = ptr->prev;
                    }
                    if(ptr->next!=NULL) {
                        ptr->next->prev = lptr;
                        lptr->next = ptr->next;
                    }
                    free(ptr);
                }
                else {
                    ptr->start = index+1;
                    lptr = aptr;
                    if(ptr->prev!=NULL) {
                        ptr->prev->next = lptr;
                        lptr->prev = ptr->prev;
                    }
                    lptr->next = ptr;
                    ptr->prev = lptr;
                }
                printf("ALLOCATION SUCCESFULL!!\n");
                while(lptr->prev != NULL) {
                    lptr = lptr->prev;
                }
                hp->lptr = lptr;
            }
        }
    }
}

void freeSpace(heap *hp, char name[])
{
    List *lptr = hp->lptr;
    List *rem=lptr;
    List *aptr = NULL;
    if(Empty(hp)) {
        printf("NO ALLOCATIONS YET!!\n");
    }
    else {
        int status = 0;
        while (lptr!=NULL && !status) {
            if(!(lptr->isFree) && strcmp(name,lptr->Name)==0) {
                aptr = lptr;
                status = 1;
            }
            lptr = lptr->next;
        }
        if(aptr==NULL) {
            printf("NAME input not found \n");
        }
        else {
            List *ptr;
            aptr->isFree = 1;
            if(aptr->prev!=NULL && aptr->prev->isFree) {
                ptr = aptr->prev;
                aptr->start = ptr->start;
                if(ptr->prev != NULL) 
                {
                    ptr->prev->next = aptr;
                    aptr->prev = ptr->prev;
                }
                free(ptr);
            }
            if(aptr->next!=NULL && aptr->next->isFree) {
                ptr = aptr->next;
                aptr->end = ptr->end;
                if(ptr->next != NULL) 
                { 
                    ptr->next->prev = aptr;
                    aptr->next = ptr->next;
                }
                free(ptr);
            }
            lptr = aptr;
            while(lptr->prev != NULL) {
                lptr = lptr->prev;
            }
            printf("free space succesfull!!!!\n");
            hp->lptr = lptr;
        }
    }
}

void printLists(heap *hp)
{
    printf("\nAllocated list is : \n");
    if(Empty(hp)) {
        printf("No allocated space.\n");
    }
    else {
        List *ptr = hp->lptr;
        printf("Name\tSize\tStart\tEnd\n");
		while(ptr!=NULL) {
			if(!(ptr->isFree))
			{
				printf("%s\t%d\t%d\t%d\n",ptr->Name,ptr->end-ptr->start+1,ptr->start,ptr->end);
			}
			ptr = ptr->next;
		}
    }
   
}
void deleteList(heap *hp)
{
    List *ptr;
    ptr = hp->lptr;
    while(hp->lptr!=NULL) {
        ptr = hp->lptr;
        hp->lptr = hp->lptr->next;
        free(ptr);
    }
    hp->lptr = NULL;
}
//*********************************************************************
int main()
{
    heap HP;
    heap *Ptr = &HP;
	List *nptr = getnewNode(0,999);
    if(nptr == NULL) {
        printf("COULD NOT ALLOCATE SPACE !!!\n");
    }
    else {
        printf("Total space = %d\n", nptr->end-nptr->start+1);
        Ptr->lptr = nptr;
    	int status=0, size=0; 
    	int input;
    	char name[100];
    	while(status==0) {
    		printf("\nEnter one of the following number to do the operations\n0 - Exit\n1 - Allocate space\n2 - Print current spaces allocated \n3 - Free Space \n");
    		scanf("%d", &input);
            if(input==0)
            {
                 status=1;
                 deleteList(Ptr);
            }
            else if(input==1)
            {
                printf("Enter Name (without spaces): ");
                 scanf("%s", name);
                 if(sameName(Ptr,name))
                 {
                     printf("Please enter unique variable name !!!\n");
                 }
                 else
                 {
                 printf("Enter the size you want to allocate :");
                 scanf("%d", &size);
                 printf("\n");
                      allocateSpace(Ptr, name,size);
                 }
            }
            else if(input==2)
            {
                  printLists(Ptr);
            }
            else if(input==3)
            {
                 printf("Enter the name of allocated space you want to free : ");
                 scanf("%s", name);
                 printf("\n");
                 freeSpace(Ptr, name);
            }
            else
            {
                printf("Please enter a valid input !!!! \n");
            }
    	}
    	 
    }
    return 0;
}