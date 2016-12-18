#include "operation.h"

/* LIMIT OPERATION: operation whose result is the first N results of another source operation */

typedef struct {
    operation_t* subop;
    int counter;
    int lim;
} operation_limit_args_t;

void
operation_limit_reset(void* vargs) {
    operation_limit_args_t* args = vargs;

    args->counter = 0;
    operation_reset(args->subop);
    return;
}

/*Revisar retorno en el control de flujo*/
int 
operation_limit_next(void* vargs) {
    operation_limit_args_t* args = vargs;

    args->counter++;
    if(operation_next(args->subop) != 0) {
    	if(args->lim >= args->counter)
    		return 1;
    }
    return 0;

}

void* 
operation_limit_get(int col, void* vargs) {
    operation_limit_args_t* args = vargs;

    return operation_get(col, args->subop);
}

void 
operation_limit_close(void* vargs) {
    operation_limit_args_t* args = vargs;

    operation_close(args->subop);
    free(args);
    return;
}

operation_t*
operation_limit_create(operation_t* suboperation, int limit) {
    int n_cols;
    operation_t* op;
	operation_limit_args_t* args;

	operation_reset(suboperation);
	n_cols = operation_ncols(suboperation);


	args = (void*)malloc(sizeof(operation_limit_args_t));
	if(!args) return NULL;

	args->subop = suboperation;
	args->counter = 0;
	args->lim = limit;

	op = (void*)malloc(sizeof(operation_t));
	if(!op) {
		free(args);
		return NULL;
	}
	
	op->reset = operation_limit_reset;
	op->next = operation_limit_next;
	op->get = operation_limit_get;
	op->close = operation_limit_close;
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


