/*
  Authore : Abdullah Jhatial 
  contact : abdullah.jhatial@10xengineers.ai
  This code  develop only for learning purpose 
  
*/
#include <stdio.h>
#define HEADER_SIZE  sizeof(header)
#define MEM_SIZE ( 4*255)+24
#define BYTE_TYPE(ptr) ((char *)ptr)
int mem[MEM_SIZE];
typedef struct node {
int size;
int free;
struct node *next;
struct node *pre;
} header;

void my_free(void * ptr)

{
header *  node = ( header *) (BYTE_TYPE(ptr)-HEADER_SIZE);
char *lower; 
header * temp_next;
header * temp_pre;

 // first node main node

 if( BYTE_TYPE(node) == BYTE_TYPE( mem)  )
 {
 node->free =1;
 node->size =0;
 return ;
 }
else if(node->next==NULL){
node->size=0;
node->free=1;
temp_pre =node->pre;
temp_pre ->next =NULL;

}
else 
{
node->size=0;
node->free=1;
temp_next = node -> next;
temp_pre = node -> pre;
temp_pre->next = temp_next;
temp_next ->pre = temp_pre;

}




}



int  mymem_max_bytes()
{
 header* ptr =( header*  )mem;
 header * temp_next;
 header *node;
 char * lower;
 char *upper;
 int temp=0;
 int max_bytes=0;
 // When whole memory is single block first time assingnment
 if (ptr->free ==1 )
   {
    if(ptr->next==NULL){
   return MEM_SIZE-24;
    }
    if((ptr->next)->free==1)
    {
    return MEM_SIZE-24;
    
    }
    
    }
        
      
      
 while (1)
 {
 // size check between two nodes
   char * byte = (char *) ptr;
   if( ptr->next!=NULL  ){
   
   lower = byte+ptr->size+HEADER_SIZE;
    upper = (char *) ptr->next;
    temp = upper-lower;
    if( temp>max_bytes ){
    max_bytes=temp;
 
    
    }
    ptr=ptr->next;
    
    
    }
    else 
    {
      lower = byte + ptr->size + HEADER_SIZE;
      upper = BYTE_TYPE(mem) + MEM_SIZE;
       temp =upper-lower;
     if (temp>max_bytes)
      
      {
      max_bytes= temp;
       
      }
      return max_bytes;
    
    
    
    }
   
}
}
void * my_malloc(size_t sizet  )
{
int size = (int) sizet;
 header* ptr =( header*  )mem;
 header * temp_next;
 header *node;
 // When whole memory is single block first time assingnment
 if (ptr->free ==1 && ptr->next==NULL)
   {
   char * byte_char = (char*) ptr;
   if( byte_char+MEM_SIZE >= byte_char+ size + HEADER_SIZE  )
   {
    ptr->size=size;
    ptr->free=0;
    temp_next = (header *) (byte_char +HEADER_SIZE+size);
    ptr->next=   temp_next;
    temp_next->pre=ptr;
    temp_next->next =NULL;
    temp_next->free = 1;
    temp_next->size=0;
    return  (void*)(ptr+1);
    
    }else return NULL;
        }
      
      char * upper ;
      char * lower;
 while (1)
 {
 // size check between two nodes
    char * byte_p = (char *) ptr;
    if( byte_p == (char *) mem  && ptr->free ==1)
    {
      lower = byte_p;
      upper =(char *) ptr->next; 
      if(upper-lower >= size+HEADER_SIZE  )
      {
      ptr->free =0;
      ptr->size=size;
      return (ptr+1);
      }
    
    }
    
   if( ptr->next!=NULL   ){
   byte_p = (char * ) ptr;
   lower = byte_p+ptr->size+HEADER_SIZE;
    upper = (char *) ptr->next;
    if(upper-lower>=size+HEADER_SIZE){
    
   
    if(ptr != (header *) mem){
     node = (header*) lower;
    node->next =ptr->next;
    node->pre  =ptr;
    node->free = 0;
    node->size=size;
    temp_next = ptr->next;
    ptr->next =node;
    temp_next->pre = node;
     return (void *)(node+1);
     }
     node =(header*)  ptr;
     node->size=size;
     node->free=0;
     
     return (void *)((char *)node+HEADER_SIZE);
 
    }
    
    ptr=ptr->next;
    
    }
    else 
    {
      byte_p = (char *) ptr ;
      lower = byte_p + ptr->size ;
      upper = ( ((char *) mem)+ MEM_SIZE) ;
      node = (header *)lower;

     if (upper - lower >= size + HEADER_SIZE  )
      
      {
         if(node->free==1)
      {
        node = (header *)byte_p;
        node->size = size;
        node->free = 0;
        node->next = NULL;
        
        return (void *)(node+1);
      
      }else {
        node->size = size;
        node->free = 0;
        node->pre = ptr;
        node->next = NULL;
        ptr->next = node;
        return (void *)(node + 1);}
      }
      
      else
      {
      return NULL;
      }
    
    
    
    }
   
}

}

void  mymem_init( )
{
header * ptr_init = (header *) mem;
ptr_init->next =NULL;
ptr_init->pre =NULL;
ptr_init -> size=0;
ptr_init ->free =1;
return;
}
void* my_relloc(void* p, int size) {
    if (p == NULL) {
        return my_malloc(size);
    }

    if (size == 0) {
        my_free(p);
        return NULL;
    }

    header* node = (header*)p - 1;

    if (node->size == size) {
        return p;
    }

    if (node->size > size) {
        node->size = size;
        return p;
    }

    if (node->next && node->next->free) {
        int combined_size = node->size + HEADER_SIZE + node->next->size;
        if (combined_size >= size) {
            header* next = node->next;
            node->size = size;
            node->next = next->next;
            if (next->next) {
                next->next->pre = node;
            }
            return p;
        }
    }

    void* new_ptr = my_malloc(size);
    if (new_ptr == NULL) return NULL;

    char* src = (char*)p;
    char* dest = (char*)new_ptr;
    for (int i = 0; i < node->size; i++) {
        dest[i] = src[i];
    }

    my_free(p);
    return new_ptr;
}
// basic test , check the address returend from my_malloc function 
/*

int main(){


mymem_init();
int * p,*p1 ,*p2,*p3;
p =my_malloc(16);
printf("  address of arr %p and get %p \n",mem,p);
p1 =my_malloc(16);
printf("  address of its prevous node %p and get %p \n",p,p1);
p2=my_malloc(16);
printf("  address of its prevous node %p and get %p \n",p1,p2);
p3=my_malloc(16);
printf("  address of its prevous node %p and get %p \n",p2,p3);
printf("free addrss %p \n",p);
my_free((void *)p );
p =my_malloc(8);
printf("  address of arr %p and get %p \n",mem,p);
my_free(p2);
p2=my_malloc(16);
printf("  address of its prevous node %p and get %p \n",p1,p2);

}                   */                                                                                                                                                                 
