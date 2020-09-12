/*
 * CSF Assignment 1
 * Arbitrary-precision integer data type
 * Function implementations
 * CSF Assignment 1
 * Sam Lipschitz
 * slipsch3@jhu.edu
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "apint.h"
#include <math.h>

/*
 * Creates an ApInt with a value specified by a uint64_t
 *
 * Parameters:
 *  val - uint64_t that specifies the value of the ApInt
 *
 * Returns:
 *  a pointer to the newly created ApInt
 */
ApInt *apint_create_from_u64(uint64_t val) {
    ApInt * new = (ApInt *) malloc(sizeof(ApInt));
    new->size = 1;
    new -> value = (uint64_t *) malloc(sizeof(uint64_t));
    new->value[0] = val;
	return new;
}

/*
 * Creates an ApInt with a value specified by a char pointer
 *
 * Parameters:
 *  hex - char* that specifies the value of the ApInt
 *
 * Returns:
 *  a pointer to the newly created ApInt
 */
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
            uint64_t temp = 0;
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
/*
 * Deallocates an ApInt
 *
 * Parameters:
 *  ap - the ApInt to be destroyed
 */
void apint_destroy(ApInt *ap) {
    free(ap->value);
    free(ap);
}

/*
 * Returns 64 bits of the binary representation of an ApInt
 *
 * Parameters:
 *  ap - the ApInt being used
 *  n- specifier of which 64 bits to be returned
 *
 * Returns:
 *  uint64_t value containing 64 bits of the binary representation of ap
 */
uint64_t apint_get_bits(ApInt *ap, unsigned n) {
	if(n > (unsigned) ap->size - 1){
	    return 0UL;
	}
    return ap->value[n];
}

/*
 * Returns the position of the most significant bit set to 1 in representation of the ApInt pointed to by ap
 *
 * Parameters:
 *  ap - ApInt being used
 *
 * Returns:
 *  int representing the position of the most significant bit set to 1 in ap
 */
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

/*
 * Returns a pointer to an ApInt formed by shifting each bit of an ApInt by 1 bit position to the left.
 *
 * Parameters:
 *  ap - ApInt being used
 *
 * Returns:
 *  ApInt representing the value of ap shifted left by 1 bit
 */
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

/*
 * Returns a pointer to an ApInt formed by shifting each bit of an ApInt by n bit positions to the left.
 *
 * Parameters:
 *  ap - ApInt being used
 *  n - the amount of bits to be left shifted
 *
 * Returns:
 *  ApInt representing the value of ap shifted left by n bits
 */
ApInt *apint_lshift_n(ApInt *ap, unsigned n) {
	ApInt * new = (ApInt *) malloc(sizeof(ApInt));
	new->size = ap->size;
	new->value = (uint64_t *) malloc(sizeof(uint64_t) * ap->size);


	if(n >= 64 && n % 64 == 0){
        new->size += n / 64;
        new->value = (uint64_t *) realloc(new->value, sizeof(uint64_t)*new->size);
        /*for(int i = 0; i < new->size; i++){
            new->value[i + n/64] = ap->value[i];
        }*/
        for(unsigned int i = new->size -1; i >= n/64; i--){
            new->value[i] = ap->value[i-n/64];
        }
        /*for(int i = new->size - 1; i > 0; i--){
            new->value[i] = new->value[i-1];
        }*/
        for(unsigned int i = 0; i < n / 64; i++){
            new->value[i] = 0;
            //printf("here\n");
        }
        return new;
	}

    uint64_t* overflow_vals = (uint64_t *) malloc(sizeof(uint64_t) * ap->size);

	for(int i = ap->size - 1; i >= 0; i--){//going from lowest to highest index
	    overflow_vals[i] = ap->value[i] >> (64 - n);//this is what needs to be added to the next index
	    new->value[i] = ap->value[i] << n;//may lose bits to overflow but was recorded in overflow_vals
	}

	for(int i = ap->size -1; i >0; i--){//adding overflow value to next bit
	    new->value[i] += overflow_vals[i-1];
	}

	if(overflow_vals[new->size-1] != 0){//bad
	    new->size += 1;
	    new->value = (uint64_t *) realloc(new->value, sizeof(uint64_t)*new->size);
	    new->value[new->size-1] = overflow_vals[new->size-2];
	}

	if(n >= 64 && n % 64 != 0){//check if this should be >=64
        new->size += n / 64;
        new->value = (uint64_t *) realloc(new->value, sizeof(uint64_t)*new->size);
        for(int i = new->size - 1; i > 0; i--){
            new->value[i] = new->value[i-1];
        }
        for(unsigned int i = 0; i < n / 64; i++){
            new->value[i] = 0;
            //printf("here\n");
        }
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

/*
 * Returns a pointer to a dynamically-allocated C char string containing the hexadecimal representation of an ApInt 
 *
 * Parameters:
 *  ap - ApInt being used
 *
 * Returns:
 *  char* representing the value of ap in hex format
 */
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
	            //printf("here bro\n");
	            s[string_size-1] = '0';
	        }
	    }	   
	}
	s[string_size] = '\0';
	return s;
}

/*
 * Returns ApInt representing the sum of 2 ApInt instances
 *
 * Parameters:
 *  a - first ApInt to be added
 *  b - second ApInt to be added
 *
 * Returns:
 *  ApInt representing the sum of ApInts a and b
 */
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
	        new->value[i] += temp;
	        if(i < new->size-1){
	            new->value[i+1] += 1;
	        }else{
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
	     if(a->size > b->size){
	          new->value[i] = a->value[i];
	       }else{
	           new->value[i] = b->value[i];
	       }
	    }
	}
	return new;
}

/*
 * Returns ApInt representing difference between the values of 2 ApInt instances
 *
 * Parameters:
 *  a - ApInt
 *  b - ApInt to be subtracted from a
 *
 * Returns:
 *  ApInt representing value of a minus b
 *  Returns NULL if b > a
 */
ApInt *apint_sub(const ApInt *a, const ApInt *b) {//a minus b
	if(apint_compare(a,b) < 0){//if b is greater than a
	    return NULL;
	}else if(apint_compare(a,b) == 0){//if a = b
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
	            new->value[i+1] -= 1;
	            if(new->value[i+1] == 0){
                    //printf("here\n");
	                new->size -= 1;
	                new->value = (uint64_t*) realloc(new->value, sizeof(uint64_t) * new->size);
	            }
	            //new->value[i] = 0xFFFFFFFFFFFFFFFF - b->value[i] + a->value[i]+1;
	            new->value[i] -= b->value[i];
	        }
	    }
	    return new;
	}
}

/*
 * Compares the values of 2 ApInt instances
 *
 * Parameters:
 *  left - ApInt to be compared
 *  right - ApInt to be compared
 *
 * Returns:
 *  Negative value if left < right
 *  Positive value if left > right
 *  0 if left = right
 */
int apint_compare(const ApInt *left, const ApInt *right) {
	if(left->size > right-> size){
	    return 1;
	}else if(left->size < right->size){
	    return -1;
	}else{;
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
