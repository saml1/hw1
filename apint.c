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
    ApInt * new = (ApInt *) malloc(sizeof(ApInt));
    new->size = 1;
    new -> value = (uint64_t *) malloc(sizeof(uint64_t));
    new->value[0] = val;
	return new;
}

ApInt *apint_create_from_hex(const char *hex) {
	ApInt * new = (ApInt *) malloc(sizeof(ApInt));
    int digits = 0;
    //finding total amount of "digits"
    while(hex[digits] != '\0'){
        digits++;
    }
    //finding how many uint64_ts are needed
    int uint_total = 0;
    if(digits % 16 == 0){
        uint_total = digits / 16;
    }else{
        uint_total = (digits / 16) + 1;
    }
    
    new->size = (uint64_t) uint_total;
    new->value = (uint64_t *) malloc(sizeof(uint64_t) * uint_total);

    //iterating from least to most significant digit
    for(int i = 0; i < uint_total; i++){
        uint64_t val = 0;
        for(int j = digits - 1- i*16; j > digits - 1 - i*16 - 16; j--){
            uint64_t temp;
            if(hex[j] >= 48 && hex[j] <= 57){//char is 0-9
                temp = hex[j] - 48;
            }else if(hex[j] >= 65 && hex[j] <= 70){//char is A-F
                temp = 10 + (hex[j] - 65);
            }else if(hex[j] >= 97 && hex[j] <= 102){//char is a-f
                temp = 10 + hex[j] - 97;
            }
            val += temp * (uint64_t) pow(16,digits - j-1-i*16);
            if(j == 0){//reached most significant bit
                new->value[i] = val;
                break;
            }
        }
        new->value[i] = val;
    }
	return new;
}

void apint_destroy(ApInt *ap) {
    free(ap->value);
    free(ap);
}

uint64_t apint_get_bits(ApInt *ap, unsigned n) {
	if(n > (unsigned) ap->size - 1){
	    return 0UL;
	}
    return ap->value[n];
}

int apint_highest_bit_set(ApInt *ap) {
	if(ap->size == 1 && ap->value[0] == 0){//special case
	    return -1;
	}
	
	int temp = 0;
	for(int i = 0; i < 64; i++){
	    if(ap->value[ap->size - 1] >= pow(2, i)){
	        temp = i;
	    }
	}
	return temp + 64*(ap->size -1);
}

ApInt *apint_lshift(ApInt *ap) {
	ApInt * new = (ApInt *) malloc(sizeof(ApInt));
	new->size = ap->size;
	new->value = (uint64_t *) malloc(sizeof(uint64_t) * ap->size);//will have to realloc if overflow

	uint64_t* overflow_vals = (uint64_t *) malloc(sizeof(uint64_t) * ap->size);//will hold values of high bits lost from overflow
	
	for(int i = ap->size - 1; i >= 0; i--){//going from lowest to highest index
	    overflow_vals[i] = ap->value[i] >> 63;//this is what needs to be added to the next index);
	    new->value[i] = ap->value[i] << 1;//may lose bits to overflow but was recorded in overflow_vals
	}
	
	for(int i = ap->size -1; i >0; i--){
	    new->value[i] += overflow_vals[i-1];
	}

	if(overflow_vals[0] != 0){
	    new->size += 1;
	    new->value = (uint64_t *) realloc(new->value, sizeof(uint64_t)*new->size);
	    new->value[new->size-1] = overflow_vals[0];
	}
	
	free(overflow_vals);
	return new;
}

ApInt *apint_lshift_n(ApInt *ap, unsigned n) {
	ApInt * new = (ApInt *) malloc(sizeof(ApInt));
	new->size = ap->size;
	new->value = (uint64_t *) malloc(sizeof(uint64_t) * ap->size);
	uint64_t* overflow_vals = (uint64_t *) malloc(sizeof(uint64_t) * ap->size);
	
	for(int i = ap->size - 1; i >= 0; i--){//going from lowest to highest index
	    overflow_vals[i] = ap->value[i] >> (64 - n);//this is what needs to be added to the next index
	    new->value[i] = ap->value[i] << n;//may lose bits to overflow but was recorded in overflow_vals
	}

	for(int i = ap->size -1; i >0; i--){
	    new->value[i] += overflow_vals[i-1];
	}

	if(overflow_vals[new->size-1] != 0){//bad
	    new->size += 1;
	    new->value = (uint64_t *) realloc(new->value, sizeof(uint64_t)*new->size);
	    new->value[new->size-1] = overflow_vals[new->size-2];
	}
	
	/*if(overflow_vals[0] != 0){//what ive been using
	    //printf("overflow\n");
	    new->size += 1;
	    new->value = (uint64_t *) realloc(new->value, sizeof(uint64_t)*new->size);
	    new->value[new->size-1] = overflow_vals[new->size-2];
	}*/
	
	free(overflow_vals);
	return new;
}

char *apint_format_as_hex(ApInt *ap) {
	if(ap->size == 1 && ap->value[0] == 0){
	    char * s = (char *) malloc(sizeof(char) * 2);
	    s[0] = '0';
	    s[1] = '\0';
	    return s;
	}
	char * s = (char *) malloc(sizeof(char) * 2);
	int string_size = 0;
	for(int i = ap->size - 1; i >= 0; i--){//looping through each value element
	    uint64_t curr_val = ap->value[i];
	    uint64_t temp = 0;
	    for(int j = 15; j >= 0; j--){
	        if(curr_val >= pow(16, j)){
	            temp = curr_val / (uint64_t) (pow(16,j));//this is the "number" hex value (0-15)
	            if(temp == 16){
	                temp -= 1;
	            }
	            curr_val -= temp * (uint64_t) pow(16, j);
	            string_size ++;
	            s = realloc(s, string_size * sizeof(char) + 1);
	            if(temp < 10){
	                s[string_size -1] = 48 + temp;//48 is '0'
	            }else{
	                s[string_size-1] = 87 + temp; //97 is 'a'
	            }
	        }else if(string_size > 0){
	            string_size++;
	            s = realloc(s, string_size * sizeof(char) + 1);
	            s[string_size-1] = '0';
	        }
	    }	   
	}
	s[string_size] = '\0';
	return s;
}

ApInt *apint_add(const ApInt *a, const ApInt *b) {
	ApInt * new = (ApInt *) malloc(sizeof(ApInt));
	int smallest_size = 0;
	
	if(a->size > b->size){
	    new->size = a->size;
	    smallest_size = b->size;
	}else{
	    new->size = b->size;
	    smallest_size = a->size;
	}
	new->value = (uint64_t *) malloc(sizeof(uint64_t) * new->size);
	for(int i = 0; i < new->size; i++){
	    new->value[i] = 0;
	}
	
	for(int i = 0; i < smallest_size; i++){//only adding bits that exist for a and b
	    uint64_t temp = a->value[i] + b->value[i];
	    if(temp < a->value[i] || temp < b->value[i]){//overflow
	        //printf("overflow\n");
	        new->value[i] += temp;
	        if(i < new->size-1){
	            new->value[i+1] += 1;
	        }else{
	            //printf("realloc");
	            new->size += 1;
	            new->value = (uint64_t *) realloc(new->value, sizeof(uint64_t)*new->size);
	            new->value[i+1] = 1;
	        }
	    }else{
	        new->value[i] += temp;
	    }
	}
	if(a->size != b->size){
	    for(int i = smallest_size; i < new->size; i++){
	     //fix to add up the bits only available on 1 apint
	     if(a->size > b->size){
	          new->value[i] = a->value[i];
	       }else{
	           new->value[i] = b->value[i];
	       }
	    }
	}
	return new;
}


ApInt *apint_sub(const ApInt *a, const ApInt *b) {//a minus b
	/* TODO: implement */
	//assert(0);
	if(apint_compare(a,b) < 0){//if b is greater than a
	    return NULL;
	}else if(apint_compare(a,b) == 0){
	    return apint_create_from_u64(0UL);
	}else{
	    ApInt * new = (ApInt *) malloc(sizeof(ApInt));
	    new->size = a->size;
	    new->value = (uint64_t *) malloc(sizeof(uint64_t) * new->size);
	    for(int i = 0; i < new->size; i++){//setting new values to a's initially
	        new->value[i] = a->value[i];
	    }
	    for(int i = 0; i < b->size; i++){
	        if(a->value[i] > b->value[i]){//simple math
	            new->value[i] -= b->value[i];
	        }else{//more complicated
	            //printf("complicated\n");
	            new->value[i+1] -= 1;
	            new->value[i] = 0xFFFFFFFFFFFFFFFF - b->value[i] + a->value[i]+1;
	        }
	    }
	    return new;
	}
	//return
}

int apint_compare(const ApInt *left, const ApInt *right) {
	/* TODO: implement */
	//assert(0);
	if(left->size > right-> size){
	    return 1;
	}else if(left->size < right->size){
	    return -1;
	}else{
	    //ASSERT(left-> size == right->size);
	    for(int i = left->size - 1; i >= 0; i--){
	        if(left->value[i] > right->value[i]){
	            return 1;
	        } else if(left->value[i] < right->value[i]){
	            return -1;
	        }
	    }
	}
	return 0;
}
