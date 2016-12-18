#include "operation.h"

/* OFFSET OPERATION: operation whose result all but the first N results of another source operation */

typedef struct {
    operation_t* subop;
    int counter;
    int offset;
} operation_offset_args_t;

void
operation_offset_reset(void* vargs) {
    operation_offset_args_t* args = vargs;

    args->counter = 0;
    operation_reset(args->subop);
    return;
}

int 
operation_offset_next(void* vargs) {
    operation_offset_args_t* args = vargs;
    
    while(args->counter < args->offset) {
    	operation_next(args->subop);
    	args->counter++;
    }
    return operation_next(args->subop);
}

void* 
operation_offset_get(int col, void* vargs) {
    operation_offset_args_t* args = vargs;
    
    return operation_get(col, args->subop);
}

void 
operation_offset_close(void* vargs) {
    operation_offset_args_t* args = vargs;

    operation_close(args->subop);
    free(args);
    return;
}

operation_t*
operation_offset_create(operation_t* suboperation, int offset) {
	int n_cols;
	operation_t* op;
	operation_offset_args_t* args;

	operation_reset(suboperation);
	n_cols = operation_ncols(suboperation);

	args = (void*)malloc(sizeof(operation_offset_args_t));
	if(!args) return NULL;

	args->subop = suboperation;
	args->counter = 0;
	args->offset = offset;

	op = (void*)malloc(sizeof(operation_t));
	if(!op) {
		free(args);
		return NULL;
	}
	
	op->reset = operation_offset_reset;
	op->next = operation_offset_next;
	op->get = operation_offset_get;
	op->close = operation_offset_close;
	op->ncols = n_cols;
	op->args = args;

	op->types = (void*)malloc(n_cols*sizeof(type_t));
	if(!op->types){
    	free(args);
    	free(op);
    	return NULL;
    }

    memcpy(op->types, suboperation->types, n_cols*sizeof(type_t));

    return op;
}

