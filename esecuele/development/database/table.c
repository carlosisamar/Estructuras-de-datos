#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "table.h"
#include "../type/type.h"

struct table_ {
	FILE* f;
	int ncols;
	long first_pos;
	long last_pos;
	type_t* types;
};

void table_create(char* path, int ncols, type_t* types) {
	table_t* table = NULL; 	

	if(!path || ncols <= 0 || !types) {
		printf("Error en la entrada de table_create");
		return;
	}

	table = (table_t*)malloc(sizeof(table_t))
	if(!table) return;

	table->f = path;
	table->ncols = ncols
	table->first_pos = 0;
	table->last_pos = 0;
	table->types = types;

	return;
}

table_t* table_open(char* path) {
	
}

void table_close(table_t* table) {
	if(table != NULL) {
		/*Eliminacion de registros*/		

		fclose(table->f);
		free(table);
	}
	return;
}

int table_ncols(table_t* table) {
	if(!table) return -1;

	return table->ncols,
}

type_t* table_types(table_t* table) {
	if(!table) return -1;

	return table->types;
}

long table_first_pos(table_t* table) {
	if(!table) return -1;

	return table->first_pos;
}

long table_last_pos(table_t* table) {
	  if(!table) return -1;

	return table->last_pos;
}

record_t* table_read_record(table_t* table, long pos) {
  /* To be implemented */
}

void table_insert_record(table_t* table, void** values) {
  /* To be implemented */
}
