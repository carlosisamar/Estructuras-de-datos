#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "table.h"
#include "../type/type.h"
struct table_ {
  int ncols;
  type_t *types;
  
  FILE *fp;
  long fpos;
};


void table_create(char* path, int ncols, type_t* types) {
  FILE *fp = NULL;
  
  if ((!path)||(!types)) return;
  fp=fopen(path, "w+");
  if (!fp) return;

  fwrite(&ncols, 1, sizeof(int), fp);
  fwrite(types, ncols, sizeof(type_t), fp);
  fclose(fp);
}

table_t* table_open(char* path) {
  table_t *table = NULL;  
  if (!path) return NULL;

  table=(table_t *) malloc(sizeof(table_t));
  if (!table) return NULL;

  table->fp = fopen(path, "r+");
  if (!table->fp) return NULL;

  fread(&(table->ncols), sizeof(int), 1, table->fp);

  table->types = (type_t *) malloc(table->ncols * sizeof(type_t));
  if (!table->types) return NULL;

  fread(table->types, sizeof(type_t), table->ncols, table->fp);
  table->fpos = ftell(table->fp);
  return table;
}


void table_close(table_t* table) {
  if (!table) return;
  fclose(table->fp);
  free(table->types);
  free(table);
}


int table_ncols(table_t* table) {
  if (!table) return -1;
  return table->ncols;
}


type_t* table_types(table_t* table) {
  if (!table) return NULL;
  return table->types;
}


long table_first_pos(table_t* table) {
  if (!table) return -1;
  return table->fpos;
}


long table_last_pos(table_t* table) {
  if (!table) return -1;
  long pos;
  fseek(table->fp, 0L, SEEK_END);
  pos = ftell(table->fp);
  return pos;
}

void table_insert_record(table_t* table, void** values)
{
  int i;
  long buf;
  fseek(table->fp, 0L, SEEK_END);
  for (i = 0; i<table->ncols; i++){
    if (table->types[i] == STR){
      buf = value_length(table->types[i], values[i]);
      fwrite(&buf, sizeof(long), 1, table->fp);
    }
    fwrite(values[i], value_length(table->types[i], values[i]), 1, table->fp);
  }
}

record_t* table_read_record(table_t*table, long pos)
{ 
  long aux, next;
  int i;
  fseek(table->fp, pos, SEEK_SET);
  void **values = (void **) malloc(sizeof(void*)*table->ncols);
  for (i = 0; i<table->ncols; i++){
    if (table->types[i] == STR){
      fread (&aux, sizeof(long), 1, table->fp);
      values[i] = (void*) malloc(sizeof(char)*aux);
      fread(values[i], aux, 1, table->fp);
    }
    else {
      values[i] = (void*) malloc(value_length(table->types[i], NULL));
      fread (values[i], value_length(table->types[i], NULL), 1, table->fp);
    }
  }

  next = ftell(table->fp);
  return record_create(values, table->ncols, next);
}


