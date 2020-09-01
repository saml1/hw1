/*
 * CSF Assignment 1
 * Arbitrary-precision integer data type
 * Function implementations
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "apint.h"

ApInt *apint_create_from_u64(uint64_t val) {
	/* TODO: implement */
    ApInt * new = (ApInt *) malloc(sizeof(ApInt));
    new->size = 1;
    new -> value = (uint64_t *) malloc(sizeof(new->size));
    new->value[0] = val;
	//assert(0);
	return new;
}

ApInt *apint_create_from_hex(const char *hex) {
	/* TODO: implement */
	//assert(0);
	return NULL;
}

void apint_destroy(ApInt *ap) {
	/* TODO: implement */
    for(int i = 0; i < ap->size; i++){
        free(ap->value);
    }
    free(ap);
	//assert(0);
}

uint64_t apint_get_bits(ApInt *ap, unsigned n) {
	/* TODO: implement */
	//assert(0);
	//return 0UL;
    return ap->value[n];
}

int apint_highest_bit_set(ApInt *ap) {
	/* TODO: implement */
	//assert(0);
	return -1;
}

ApInt *apint_lshift(ApInt *ap) {
	/* TODO: implement */
	//assert(0);
	return NULL;
}

ApInt *apint_lshift_n(ApInt *ap, unsigned n) {
	/* TODO: implement */
	//assert(0);
	return NULL;
}

char *apint_format_as_hex(ApInt *ap) {
	/* TODO: implement */
	//assert(0);
	return NULL;
}

ApInt *apint_add(const ApInt *a, const ApInt *b) {
	/* TODO: implement */
	//assert(0);
	return NULL;
}


ApInt *apint_sub(const ApInt *a, const ApInt *b) {
	/* TODO: implement */
	//assert(0);
	return NULL;
}

int apint_compare(const ApInt *left, const ApInt *right) {
	/* TODO: implement */
	//assert(0);
	return 0;
}
