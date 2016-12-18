#include "operation.h"

/* COUNT OPERATION: operation whose result is the number of results of the source operation */

typedef struct {
    int counter;
    operation_t* subop;
} operation_count_args_t;

void
operation_count_reset(void* vargs) {
    operation_count_args_t* args = vargs;

    args->counter = -1;
    operation_reset(args->subop);
    return;
}

int operation_count_next(void* vargs) {
    operation_count_args_t* args = vargs;
    if(args->counter >= 0)
    	return 0;
    else {
    	args->counter = 0;
    	while(operation_next(args->subop) != 0){
    		args->counter++;
    	}
    	return 1;
    }
}

void* operation_count_get(int col, void* vargs) {
    operation_count_args_t* args = vargs;

    return &(args->counter);
}

void operation_count_close(void* vargs) {
    operation_count_args_t* args = vargs;

    operation_close(args->subop);
    free(args);

    return;
}

operation_t*
operation_count_create(operation_t* suboperation) {
    operation_t* op;
    operation_count_args_t* args;

    args = malloc(sizeof(operation_count_args_t));
    if(!args) return NULL;

    args->subop = suboperation;
    args->counter = -1;

    op = malloc(sizeof(operation_t));
    if(!op) {
    	free(args);
    	return NULL;
    }

    op->reset = operation_count_reset;
    op->next = operation_count_next;
    op->get = operation_count_get;
    op->close = operation_count_close;
    op->ncols = 1;
    op->args = args;

    op->types = malloc(sizeof(type_t));
    if(!op->types) {
    	free(args);
    	free(op);
    	return NULL;
    }

    op->types[0] = INT;
 
    return op;
}

