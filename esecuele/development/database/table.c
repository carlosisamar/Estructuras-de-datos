#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "table.h"
#include "../type/type.h"

struct table_ {
	long *f;
	int ncols;
	long first_pos;
	long last_pos;
	
};

void table_create(char* path, int ncols, type_t* types) {
	int i;
	FILE *f;
	f = fopen (path, "w");
	fwrite(ncols, sizeof(int), 1, f);
	fwrite(types, sizeof(int), ncols, f);
	fclose(f);
	return;
}

table_t* table_open(char* path) {
	table_t *t;
	FILE *f;
	t = (table_t*) malloc (sizeof(table_t));
	f = fopen(path, "r");

}

void table_close(table_t* table) {

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

}

long table_last_pos(table_t* table) {

}

record_t* table_read_record(table_t* table, long pos) {
  /* To be implemented */
}

void table_insert_record(table_t* table, void** values) {
  /* To be implemented */
}
