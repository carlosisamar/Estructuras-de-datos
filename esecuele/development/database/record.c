#include "record.h"

struct record_ {
	long next
	int numcols
	void **values
};

record_t* record_create(void** values, int ncols, long next) {
	record_t r;
	r = (record_t*) malloc (sizeof(record_t));
	r->next = next;
	r->ncols = ncols;
	r->values = values;
	return r;
}

void* record_get(record_t* record, int n) {
	return record->values[n];  
}

long record_next(record_t* record) {
	return record->next;
}

void record_free(record_t* record) {
	int i;
	for (i=0; i<ncols; i++){
		free(record->values[i]);
	}
	free(record->values);
	free(record);
}