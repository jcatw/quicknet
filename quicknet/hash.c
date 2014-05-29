#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "hash.h"

hash_t *make_hash(int n_buckets) {
  hash_t *hashtable = (hash_t*) malloc( sizeof(hash_t) );
  hashtable->n_buckets = n_buckets;
  hashtable->buckets = (hash_item_t**) malloc(n_buckets * sizeof(hash_item_t*));
  return hashtable;
}

hash_item_t *make_hash_item(directed_node_t *node, heap_item_t *item) {
  hash_item_t *hashitem = (hash_item_t*) malloc(sizeof(hash_item_t));
  hashitem->node = node;
  hashitem->item = item;
  hashitem->next = NULL;
  return hashitem;
}

int hash_addr(hash_t *table, directed_node_t *node) {
  return ((unsigned int) node) % table->n_buckets;
}

void hash_insert(hash_t *table, directed_node_t *node, heap_item_t *item) {
  int addr;
  hash_item_t *hashitem;
  hash_item_t *head;

  //printf("making item\n");
  hashitem = make_hash_item(node,item);

  // find the bucket
  addr = hash_addr(table, node);
  //printf("getting addr %d\n",addr);
  
  // insert at head
  //printf("inserting at head\n");
  head = table->buckets[addr];
  table->buckets[addr] = hashitem;
  hashitem->next = head;
}

hash_item_t *hash_get(hash_t *table, directed_node_t *node) {
  int addr;
  hash_item_t *head = NULL;

  addr = hash_addr(table, node);
  head = table->buckets[addr];
  while(head) {
    if(head->node == node)
      return head;
    head = head->next;
  }
  return NULL;
}
    
  



  

  
