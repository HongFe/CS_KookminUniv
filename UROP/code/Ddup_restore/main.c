#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "md5.h"

#define KEY_SIZE 8								// 64bit MD5_hash
#define TABLE_SIZE 16777216						// 2^24
#define empty(e) (strlen(e.key)==0)				// empty
#define equal(e1,e2) (!strcmp(e1.key,e2.key))	// existing

#define CHUNK_FILE ""
#define FLAG_FILE ""
#define MD_FILE ""

#define BIT_MASK 33554431 //  3bytes = '1' 

typedef char* chunk;	// chunk address 

/* Hash Table element*/
typedef struct{
	chunk ptr;	
}hash_table_element;

/* Packet element*/
typedef struct{
	//char md5[64]; 임시
	int hash;	// 임시
	char flag;
}packet_element;

/* Chunk List*/
typedef struct{
	//char md5[64]; 임시
	int size;	// 임시
	char chunk[20];
}chunk_list;

hash_table_element hash_table[TABLE_SIZE];	// 해싱테이블 구조체 선언

int restore_packet();

int main(){

	restore_packet();

	return 0;
}

int restore_packet(){

	char restored_packet[1500] = "";	// RESTORED PACKET
	
	packet_element packet[6] = { { 15432, '1' }, { 1653, '1' }, { 15432, '0' }, { 1653, '1' }, { 5343, '1' }, { 5343, '0' } }; // <HASH,FLAG>
	chunk_list list[4] = { { 8, "asdfasdf" }, { 8, "qweqwe2e" }, { 8, "asdqwedc" }, { 5, "asdfz" } };			 // <CUNK_SIZE, CHUNK>
	int chunk_num = 6;
	int list_num = 4;
	
	int chunk_size = 0;
	int chunk_ptr = 0;
	int hash_index = 0;
	
	int i = 0;

	/////////////////////////////////////////////
	printf("[HASH  |  FLAG]\n");
	for (i = 0; i < chunk_num; i++)
		printf("[%d:%c] ", packet[i].hash, packet[i].flag); 
	printf("\n\n");
	/////////////////////////////////////////////
	/////////////////////////////////////////////
	printf("[SIZE  |  CHUNK]\n");
	for (i = 0; i < list_num; i++)
		printf("[%d:%s] ", list[i].size, list[i].chunk);
	printf("\n\n");
	/////////////////////////////////////////////

	for (i = 0; i < chunk_num; i++){
		
		hash_index = packet[i].hash & BIT_MASK;	// hash indexing 64bits -> 24bits
		printf("Packet] <HASH : %d> | <FLAG : %c> \n", hash_index, packet[i].flag);

		if (packet[i].flag == '1'){
			printf("<FLAG 1 Hash table change. Update chunk>\n");

			chunk_size = list[chunk_ptr].size*sizeof(char);
			hash_table[hash_index].ptr = (char *)malloc(chunk_size + sizeof(char)); // allocation memory for chunk

			strncpy(hash_table[hash_index].ptr, list[chunk_ptr++].chunk, chunk_size);	// copy chunk to hash table
			hash_table[hash_index].ptr[chunk_size] = '\0'; // end of chunk in hash table 
		}
		sprintf(restored_packet, "%s | %s", restored_packet, hash_table[hash_index].ptr);

		printf("HashTable] <INDEX : %d> | <CHUNK : %s> \n\n", hash_index, hash_table[hash_index].ptr);
	}
	
	printf("\nRestored Packet : %s\n", restored_packet);

	return 0;
}