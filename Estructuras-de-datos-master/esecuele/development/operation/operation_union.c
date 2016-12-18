
#include "operation.h"

/* UNION OPERATION: concatenation of the results of two operations with the same columns */

typedef struct {
	int flag;
    operation_t* op1;
    operation_t* op2;
} operation_union_args_t;

void
operation_union_reset(void* vargs) {
    operation_union_args_t* args = vargs;

    args->flag = 1;
    operation_reset(args->op1);
    operation_reset(args->op2);

    return;
}

int operation_union_next(void* vargs) {
    operation_union_args_t* args = vargs;
    int ret = 0;

    if(args->flag == 1) {
    	ret = operation_next(args->op1);
    	if(ret == 0)
    			args->flag = 2;
    }
    if(args->flag == 2) {
    	ret = operation_next(args->op2);
    }

    return ret;
}

void* operation_union_get(int col, void* vargs) {
    operation_union_args_t* args = vargs;

    if(args->flag == 1) {
    	return operation_get(col, args->op1);
    }
    else if(args->flag == 2) {
    	return operation_get(col, args->op2);
    }

    return NULL;
}

void operation_union_close(void* vargs) {
    operation_union_args_t* args = vargs;

    operation_close(args->op1);
    operation_close(args->op2);
    free(args);

    return;
}

operation_t*
operation_union_create(operation_t* operation1, operation_t* operation2) {
    operation_t* operation;
    operation_union_args_t* args;

    args = malloc(sizeof(operation_union_args_t));
    if(!args) return NULL;

    args->flag = 1;
    args->op1 = operation1;
    args->op2 = operation2;

    operation = malloc(sizeof(operation_t));
    if(!operation) {
    	free(args);
    	return NULL;
    }

    operation->args = args;
    operation->reset = operation_union_reset;
    operation->next = operation_union_next;
    operation->get = operation_union_get;
    operation->close = operation_union_close;
    /*Suponemos que ambas operationes tienen el mismo numero de columnas, con los mismos tipos*/
    operation->ncols = operation_ncols(operation1);

    operation->types = malloc(operation->ncols*sizeof(type_t));
    if(!operation->types) {
    	free(args);
    	free(operation);
    	return NULL;
    }
    memcpy(operation->types, operation1->types, operation1->ncols*sizeof(type_t));

    return operation;
}

