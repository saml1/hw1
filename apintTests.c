/*
 * Unit tests for arbitrary-precision integer data type
 *
 * These tests are by no means comprehensive.  You will need to
 * add more tests of your own!  In particular, make sure that
 * you have tests for more challenging situations, such as
 *
 * - large values
 * - adding/subtracting/comparing values with different lengths
 * - special cases (carries when adding, borrows when subtracting, etc.)
 * - etc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "apint.h"
#include "tctest.h"

typedef struct {
	ApInt *ap0;
	ApInt *ap1;
	ApInt *ap110660361;
	ApInt *max1;
	ApInt *easy;
	ApInt *easy1;
	ApInt *ap11;
	ApInt *ap1c1;
	ApInt *apfffff;
	ApInt *max2;
	ApInt *apf7;
	ApInt *apf8;
	ApInt *apf15;
	ApInt *rand16;
	ApInt *apf17;
	ApInt *randbig1;
	ApInt *randbig2;
	ApInt *rubyrand1;
	ApInt *rubyrand2;
	/* TODO: add additional fields of test fixture */
} TestObjs;

TestObjs *setup(void);
void cleanup(TestObjs *objs);

void testCreateFromU64(TestObjs *objs);
void testHighestBitSet(TestObjs *objs);
void testLshiftN(TestObjs *objs);
void testCompare(TestObjs *objs);
void testFormatAsHex(TestObjs *objs);
void testAdd(TestObjs *objs);
void testSub(TestObjs *objs);
void testCreateFromHex(TestObjs *objs);
void testLshift(TestObjs *objs);
/* TODO: add more test function prototypes */

int main(int argc, char **argv) {
	TEST_INIT();

	if (argc > 1) {
		/*
		 * name of specific test case to execute was provided
		 * as a command line argument
		 */
		tctest_testname_to_execute = argv[1];
	}

	TEST(testCreateFromU64);
	TEST(testHighestBitSet);
	TEST(testLshiftN);
	TEST(testCompare);
	TEST(testFormatAsHex);
	TEST(testAdd);
	TEST(testSub);
	TEST(testCreateFromHex);
	TEST(testLshift);
	/* TODO: use TEST macro to execute more test functions */

	TEST_FINI();
}

TestObjs *setup(void) {
	TestObjs *objs = malloc(sizeof(TestObjs));
	objs->ap0 = apint_create_from_u64(0UL);
	objs->ap1 = apint_create_from_u64(1UL);
	objs->ap110660361 = apint_create_from_u64(110660361UL);
	objs->max1 = apint_create_from_u64(0xFFFFFFFFFFFFFFFFUL);
	objs->easy = apint_create_from_hex("0");
	objs->easy1 = apint_create_from_hex("1");
	objs->ap11 = apint_create_from_hex("11");
	objs->ap1c1 = apint_create_from_hex("1c1\0");
	objs->apfffff = apint_create_from_hex("fFfFf\0");
	objs->max2 = apint_create_from_hex("fFfFffffffffffff");
	objs->apf7 = apint_create_from_hex("fffffff\0");
	objs->apf8 = apint_create_from_hex("ffffffff\0");
	objs->apf15 = apint_create_from_hex("FfFffffffffffff\0");
	objs->rand16 = apint_create_from_hex("484FA4cb04359dee\0");
	objs->apf17 = apint_create_from_hex("fFfFfffffffffffff\0");
	objs->randbig1 = apint_create_from_hex("9031f4a0b41b53cc61d084e\0");
	objs->randbig2 = apint_create_from_hex("624aab9dc8ef44f0574833f57a606dd17dd7d8d2ae61debb8b08\0");
	objs->rubyrand1 = apint_create_from_hex("c8e0ef88dac5dafde8ed537c1ca7e95604e6724362e349721061cb1cdb816cc0b72d0fb0ea690d9c90b91d71");
	objs->rubyrand2 = apint_create_from_hex("569dd3f8a1faab4e675d07b0c64845adb9366b2333d66b06315b4d1a4a98860d3453729cebd656a197c2b");
	/* TODO: initialize additional members of test fixture */

	return objs;
}

void cleanup(TestObjs *objs) {
	apint_destroy(objs->ap0);
	apint_destroy(objs->ap1);
	apint_destroy(objs->ap110660361);
	apint_destroy(objs->max1);
	apint_destroy(objs->easy);
	apint_destroy(objs->easy1);
	apint_destroy(objs->ap11);
	apint_destroy(objs->ap1c1);
	apint_destroy(objs->apfffff);
	apint_destroy(objs->max2);
	apint_destroy(objs->apf7);
	apint_destroy(objs->apf8);
	apint_destroy(objs->apf15);
	apint_destroy(objs->rand16);
	apint_destroy(objs->apf17);
	apint_destroy(objs->randbig1);
	apint_destroy(objs->randbig2);
	apint_destroy(objs->rubyrand1);
	apint_destroy(objs->rubyrand2);
	/* TODO: destroy additional members of test fixture */

	free(objs);
}

void testCreateFromU64(TestObjs *objs) {
	ASSERT(0UL == apint_get_bits(objs->ap0, 0));
	//uint64_t a = apint_get_bits(objs->ap0, 0);
    //printf("%d\n", &a);
	ASSERT(1UL == apint_get_bits(objs->ap1, 0));
	ASSERT(110660361UL == apint_get_bits(objs->ap110660361, 0));
	ASSERT(0xFFFFFFFFFFFFFFFFUL == apint_get_bits(objs->max1, 0));
}

void testHighestBitSet(TestObjs *objs) {
	ASSERT(-1 == apint_highest_bit_set(objs->ap0));
	ASSERT(0 == apint_highest_bit_set(objs->ap1));
	ASSERT(26 == apint_highest_bit_set(objs->ap110660361));
	ASSERT(63 == apint_highest_bit_set(objs->max1));
	ASSERT(91 == apint_highest_bit_set(objs->randbig1));
	
}

void testLshiftN(TestObjs *objs) {
	ApInt *result;

	result = apint_lshift_n(objs->ap0, 17);
	ASSERT(0UL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);

	result = apint_lshift_n(objs->ap1, 17);
	ASSERT(0x20000UL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);

	result = apint_lshift_n(objs->ap110660361, 17);
	ASSERT(0xD3116120000UL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);
}

void testCompare(TestObjs *objs) {
	/* 1 > 0 */
	ASSERT(apint_compare(objs->ap1, objs->ap0) > 0);
	/* 0 < 1 */
	ASSERT(apint_compare(objs->ap0, objs->ap1) < 0);
	/* 110660361 > 0 */
	ASSERT(apint_compare(objs->ap110660361, objs->ap0) > 0);
	/* 110660361 > 1 */
	ASSERT(apint_compare(objs->ap110660361, objs->ap1) > 0);
	/* 0 < 110660361 */
	ASSERT(apint_compare(objs->ap0, objs->ap110660361) < 0);
	/* 1 < 110660361 */
	ASSERT(apint_compare(objs->ap1, objs->ap110660361) < 0);
	
	ASSERT(apint_compare(objs->rubyrand1, objs->rubyrand2) > 0);
}

void testFormatAsHex(TestObjs *objs) {
	char *s;
	//s = apint_format_as_hex(objs->ap0);
    //printf("returned: %c\n", s[0]);
    //printf("strcmp: %d\n", strcmp("0", s));
	ASSERT(0 == strcmp("0", (s = apint_format_as_hex(objs->ap0))));
	free(s);

	ASSERT(0 == strcmp("1", (s = apint_format_as_hex(objs->ap1))));
	free(s);

	ASSERT(0 == strcmp("6988b09", (s = apint_format_as_hex(objs->ap110660361))));
	free(s);

	ASSERT(0 == strcmp("ffffffffffffffff", (s = apint_format_as_hex(objs->max1))));
	free(s);
	
	ASSERT(0 == strcmp("484fa4cb04359dee", (s = apint_format_as_hex(objs->rand16))));
	free(s);
	
	ASSERT(0 == strcmp("9031f4a0b41b53cc61d084e", (s = apint_format_as_hex(objs->randbig1))));
	free(s);
	
	ASSERT(0 == strcmp("624aab9dc8ef44f0574833f57a606dd17dd7d8d2ae61debb8b08", (s = apint_format_as_hex(objs->randbig2))));
	free(s);
	
	ASSERT(0 == strcmp("fffffffffffffffff", (s = apint_format_as_hex(objs->apf17))));
	free(s);
}

void testAdd(TestObjs *objs) {
	ApInt *sum;
	char *s;

	/* 0 + 0 = 0 */
	sum = apint_add(objs->ap0, objs->ap0);
	ASSERT(0 == strcmp("0", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* 1 + 0 = 1 */
	sum = apint_add(objs->ap1, objs->ap0);
	ASSERT(0 == strcmp("1", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* 1 + 1 = 2 */
	sum = apint_add(objs->ap1, objs->ap1);
	ASSERT(0 == strcmp("2", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* 110660361 + 1 = 110660362 */
	sum = apint_add(objs->ap110660361, objs->ap1);
	ASSERT(0 == strcmp("6988b0a", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* FFFFFFFFFFFFFFFF + 1 = 10000000000000000 */
	sum = apint_add(objs->max1, objs->ap1);
	ASSERT(0 == strcmp("10000000000000000", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);
}

void testSub(TestObjs *objs) {
	ApInt *a, *b, *diff;
	char *s;

	/* subtracting 1 from ffffffffffffffff is fffffffffffffffe */
	diff = apint_sub(objs->max1, objs->ap1);
	ASSERT(0 == strcmp("fffffffffffffffe", (s = apint_format_as_hex(diff))));
	apint_destroy(diff);
	free(s);

	/* subtracting 0 from 1 is 1 */
	diff = apint_sub(objs->ap1, objs->ap0);
	ASSERT(0 == strcmp("1", (s = apint_format_as_hex(diff))));
	ASSERT(0 == apint_compare(diff, objs->ap1));
	apint_destroy(diff);
	free(s);

	/* subtracting 1 from 1 is 0 */
	diff = apint_sub(objs->ap1, objs->ap1);
	ASSERT(0 == strcmp("0", (s = apint_format_as_hex(diff))));
	ASSERT(0 == apint_compare(diff, objs->ap0));
	apint_destroy(diff);
	free(s);

	/* subtracting 1 from 0 can't be represented because it is negative */
	ASSERT(NULL == apint_sub(objs->ap0, objs->ap1));

	/* test involving larger values */
	a = apint_create_from_hex("7e35207519b6b06429378631ca460905c19537644f31dc50114e9dc90bb4e4ebc43cfebe6b86d");
	b = apint_create_from_hex("9fa0fb165441ade7cb8b17c3ab3653465e09e8078e09631ec8f6fe3a5b301dc");
	diff = apint_sub(a, b);
	ASSERT(0 == strcmp("7e35207519b6afc4883c6fdd8898213a367d73b918de95f20766963b0251c622cd3ec4633b691",
		(s = apint_format_as_hex(diff))));
	apint_destroy(diff);
	apint_destroy(b);
	apint_destroy(a);
	free(s);
}

void testLshift(TestObjs *objs){
    ApInt * result;
    
    result = apint_lshift(objs->ap0);
    ASSERT(0UL == apint_get_bits(result, 0));
    apint_destroy(result);
    
    result = apint_lshift(objs->ap1);
    ASSERT(2UL == apint_get_bits(result, 0));
    apint_destroy(result);
    
    result = apint_lshift(objs->ap110660361);
    //printf("%lX\n", apint_get_bits(result, 0));
    ASSERT(221320722UL == apint_get_bits(result, 0));
    apint_destroy(result);
    
    result = apint_lshift(objs->max1);
    //printf("%lX\n", apint_get_bits(result, 0));
    //printf("%lX\n", apint_get_bits(result, 1));
    ASSERT(0xfffffffffffffffe == apint_get_bits(result, 1));
    ASSERT(0x1 == apint_get_bits(result, 0));
    apint_destroy(result);
}

void testCreateFromHex(TestObjs *objs){
    //uint64_t a = apint_get_bits(objs->easy, 0);
    //printf("%d\n", &a);
    ASSERT(0x0 == apint_get_bits(objs->easy, 0));
    ASSERT(0x1 == apint_get_bits(objs->easy1, 0));
    ASSERT(0x11 == apint_get_bits(objs->ap11, 0));
    ASSERT(0x1c1 == apint_get_bits(objs->ap1c1, 0));
    ASSERT(0xfffff == apint_get_bits(objs->apfffff, 0));
    ASSERT(0xfffffff == apint_get_bits(objs->apf7, 0));
    ASSERT(0xffffffff == apint_get_bits(objs->apf8, 0));
    ASSERT(0xfffffffffffffff == apint_get_bits(objs->apf15, 0));
    ASSERT(0xffffffffffffffff == apint_get_bits(objs->max2, 0));
    ASSERT(0x484fa4cb04359dee == apint_get_bits(objs->rand16, 0));
    ASSERT(0xffffffffffffffff == apint_get_bits(objs->apf17, 0));
    ASSERT(0xf == apint_get_bits(objs->apf17, 1));
    ASSERT(0x0b41b53cc61d084e == apint_get_bits(objs->randbig1, 0));
    ASSERT(0x9031f4a == apint_get_bits(objs->randbig1, 1));
    ASSERT(0xd8d2ae61debb8b08 == apint_get_bits(objs->randbig2, 0));
    ASSERT(0x33f57a606dd17dd7 == apint_get_bits(objs->randbig2, 1));
    ASSERT(0xab9dc8ef44f05748 == apint_get_bits(objs->randbig2, 2));
    ASSERT(0x624a == apint_get_bits(objs->randbig2, 3));
    //printf("yeet\n");
    //printf("bit 0: %lX\n", apint_get_bits(objs->randbig1, 0));
    //printf("bit 1: %lX\n", apint_get_bits(objs->randbig1, 1));
    /*printf("%X\n", apint_get_bits(objs->apf17, 0));
    printf("%lX", apint_get_bits(objs->apf17, 1));*/
}

/* TODO: add more test functions */
