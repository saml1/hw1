/*
 * CSF Assignment 1
 * Arbitrary-precision integer data type
 * Function implementations
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "apint.h"
#include <math.h>

ApInt *apint_create_from_u64(uint64_t val) {
	/* TODO: implement */
    ApInt * new = (ApInt *) malloc(sizeof(ApInt));
    new->size = 1;
    //new -> value = (uint64_t *) malloc(sizeof(new->size));
    new -> value = (uint64_t *) malloc(sizeof(uint64_t));
    new->value[0] = val;
	//assert(0);
	return new;
}

ApInt *apint_create_from_hex(const char *hex) {
	/* TODO: implement */
	ApInt * new = (ApInt *) malloc(sizeof(ApInt));
    int digits = 0;
    //finding total amount of "digits"
    while(hex[digits] != '\0'){
        digits++;
    }

    //finding how many uints are needed
    int uint_total = 0;
    if(digits % 16 == 0){
        uint_total = digits / 16;
    }else{
        uint_total = (digits / 16) + 1;
    }
    
    new->size = (uint64_t) uint_total;
    new->value = (uint64_t *) malloc(sizeof(uint64_t) * uint_total);
    
    //int curr_apint = 0;//

    //iterating from least to most significant digit
    for(int i = 0; i < uint_total; i++){
        uint64_t val = 0;
        for(int j = digits - 1- i*16; j > digits - 1 - i*16 - 16; j--){
            int temp;
            switch(hex[j]){
                case '0':
                    temp = 0;
                    break;
                case '1':
                    temp = 1;
                    break;
            }
            //val += temp * pow(16, digits - j);//fix to work with multiples of 16 etc
            val += temp;
            if(j == 0){//reached most significant bit
                new->value[i] = val;
                break;
            }
        }
        new->value[i] = val;
    }
	//assert(0);
	return new;
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
