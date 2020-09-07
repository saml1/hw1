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
	ApInt * new = (ApInt *) malloc(sizeof(ApInt));
    int digits = 0;
    //finding total amount of "digits"
    while(hex[digits] != '\0'){
        digits++;
    }
    //printf("digits: %d\n", digits);
    //finding how many uint64_ts are needed
    int uint_total = 0;
    if(digits % 16 == 0){
        uint_total = digits / 16;
    }else{
        uint_total = (digits / 16) + 1;
    }
    //printf("uint_total: %d\n", uint_total);
    new->size = (uint64_t) uint_total;
    new->value = (uint64_t *) malloc(sizeof(uint64_t) * uint_total);

    //iterating from least to most significant digit
    for(int i = 0; i < uint_total; i++){
        //printf("%d\n", i);
        uint64_t val = 0;
        for(int j = digits - 1- i*16; j > digits - 1 - i*16 - 16; j--){
            uint64_t temp;
            //printf("j: %d\n", j);
            if(hex[j] >= 48 && hex[j] <= 57){//char is 0-9
                temp = hex[j] - 48;
            }else if(hex[j] >= 65 && hex[j] <= 70){//char is A-F
                temp = 10 + (hex[j] - 65);
            }else if(hex[j] >= 97 && hex[j] <= 102){//char is a-f
                temp = 10 + hex[j] - 97;
            }
            //printf("temp: %d\n", temp);
            val += temp * (uint64_t) pow(16,digits - j-1-i*16);
            //printf("val: %lX\n", val);
            /*printf("16 pow: %lu\n", digits - j - 1);
            printf("temp: %lu\n", temp);
            printf("val in inner for loop: %lu\n", val);
            printf("\n");*/
            //printf("temp*pow: %f\n", (uint64_t) temp * pow(16, digits - j-1-i*16));
            // printf("temp*pow: %d\n", temp*pow(16, digits - j-1));
            //printf("temp: %d\n", temp);
            //printf("val: %lu\n", val);
            if(j == 0){//reached most significant bit
                new->value[i] = val;
                break;
            }
        }
        //printf("val:%lX\n", val);
        new->value[i] = val;
    }
	//assert(0);
	return new;
}

void apint_destroy(ApInt *ap) {
	/* TODO: implement */
    //for(int i = 0; i < ap->size; i++){
        free(ap->value);
    //}
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
	if(ap->size == 1 && ap->value[0] == 0){//special case
	    return -1;
	}
	
	int temp = 0;
	for(int i = 0; i < 64; i++){
	    if(ap->value[ap->size - 1] >= pow(2, i)){
	        temp = i;
	    }
	}
	//printf("returning: %d\n", temp + 1 + 64*(ap->size -1));
	return temp + 64*(ap->size -1);
}

ApInt *apint_lshift(ApInt *ap) {
	/* TODO: implement */
	ApInt * new = (ApInt *) malloc(sizeof(ApInt));
	new->size = ap->size;
	new->value = (uint64_t *) malloc(sizeof(uint64_t) * ap->size);//will have to realloc if overflow

	uint64_t* overflow_vals = (uint64_t *) malloc(sizeof(uint64_t) * ap->size);//will hold values of high bits lost from overflow
	
	for(int i = ap->size - 1; i >= 0; i--){//going from lowest to highest index
	    overflow_vals[i] = ap->value[i] >> 63;//this is what needs to be added to the next index
	    new->value[i] = ap->value[i] << 1;//may lose bits to overflow but was recorded in overflow_vals
	}
	
	/*for(int i = ap->size -2; i >= 0; i--){//add code to deal with index 0 (will have to realloc)
	    new->value[i] += overflow_vals[i+1];
	    //printf("hi\n");
	}*/
	
	for(int i = 0; i < ap->size -1; i++){
	    new->value[i] += overflow_vals[i+1];
	}

	if(overflow_vals[0] != 0){
	    printf("overflow\n");
	    new->size += 1;
	    new->value = (uint64_t *) realloc(new->value, sizeof(uint64_t)*new->size);
	    for(int i = new->size - 1; i > 0; i--){
	        new->value[i] = new->value[i-1];
	    }
	    //for(int i = 0; i < 
	    new->value[0] = overflow_vals[0];
	}
	
	free(overflow_vals);
	return new;
}

ApInt *apint_lshift_n(ApInt *ap, unsigned n) {
	/* TODO: implement */
	//assert(0);
	ApInt * new = (ApInt *) malloc(sizeof(ApInt));
	new->size = ap->size;
	new->value = (uint64_t *) malloc(sizeof(uint64_t) * ap->size);
	uint64_t* overflow_vals = (uint64_t *) malloc(sizeof(uint64_t) * ap->size);
	
	for(int i = ap->size - 1; i >= 0; i--){//going from lowest to highest index
	    overflow_vals[i] = ap->value[i] >> (64 - n);//this is what needs to be added to the next index
	    new->value[i] = ap->value[i] << n;//may lose bits to overflow but was recorded in overflow_vals
	}
	
	for(int i = ap->size -2; i > 0; i--){//add code to deal with index 0 (will have to realloc)
	    new->value[i] += overflow_vals[i+1];
	    //printf("hi\n");
	}

	if(overflow_vals[0] != 0){
	    //printf("overflow\n");
	    new->size += 1;
	    new->value = (uint64_t *) realloc(new->value, sizeof(uint64_t)*new->size);
	    for(int i = new->size - 1; i > 0; i--){
	        new->value[i] = new->value[i-1];
	    }
	    new->value[0] = overflow_vals[0];
	}/*else if(n >= 16){
	    new->size += (n/16);
	    new->value = (uint64_t *) realloc(new->value, sizeof(uint64_t)*new->size);
	    for(int i = new->size - 1; i > 0; i--){
	        new->value[i] = new->value[i-1];
	    }
	}*/
	
	free(overflow_vals);
	return new;
}

char *apint_format_as_hex(ApInt *ap) {
	/* TODO: implement */
	//assert(0);
	if(ap->size == 1 && ap->value[0] == 0){
	    //s = "0";
	    //printf("ok\n");
	    char * s = (char *) malloc(sizeof(char) * 2);
	    s[0] = '0';
	    s[1] = '\0';
	    //s = "0";
	    return s;
	}
	//printf("here\n");
	char * s = (char *) malloc(sizeof(char) * 2);
	int string_size = 0;
	for(int i = ap->size - 1; i >= 0; i--){//looping through each value element
	    uint64_t curr_val = ap->value[i];
	    uint64_t temp = 0;
	    //printf("curr_val: %lX\n", curr_val);
	    for(int j = 15; j >= 0; j--){
	        if(curr_val >= pow(16, j)){
	            //printf("Here, j= %d\n", j);
	            /*if(curr_val % (uint64_t) pow(16, j) != 0){
	                temp = curr_val / (pow(16,j));
	            }*/
	            temp = curr_val / (uint64_t) (pow(16,j));//this is the "number" hex value (0-15)
	            if(temp == 16){
	                //printf("temp's 16\n");
	                temp -= 1;
	            }
	            //printf("temp: %d\n", temp);//problem is dealing with a 0- fix
	            curr_val -= temp * (uint64_t) pow(16, j);
	            //printf("curr_val inside inner loop: %lX\n", curr_val);
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
	            //printf("ok\n");
	        }
	    }	   
	}
	//printf("%d\n", string_size);
	/*if(ap->size == 1 && ap->value[0] == 0){
	    s = "0";
	    printf("ok\n");
	}*/
	//printf("s: %s\n", s);
	//printf("size: %d\n", string_size);
	s[string_size] = '\0';
	//printf("s: %s\n", s);
	return s;
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
