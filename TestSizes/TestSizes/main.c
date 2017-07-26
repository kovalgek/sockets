//
//  main.c
//  TestSizes
//
//  Created by Anton Kovalchuk on 29.04.17.
//  Copyright © 2017 Anton Kovalchuk. All rights reserved.
//

#include <limits.h>
#include <stdint.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    
    printf("CHAR_BIT is %d\n\n",CHAR_BIT); // Bits in a char (usually 8!)
    
    printf("sizeof(char) is %lu\n", sizeof(char)); // ALWAYS 1
    printf("sizeof(short) is %lu\n", sizeof(short));
    printf("sizeof(int) is %lu\n", sizeof(int));
    printf("sizeof(long) is %lu\n", sizeof(long));
    printf("sizeof(long long) is %lu\n\n", sizeof(long long));
    
    printf("sizeof(int8_t) is %lu\n", sizeof(int8_t));

    printf("sizeof(int16_t) is %lu\n", sizeof(int16_t));
    printf("sizeof(int32_t) is %lu\n", sizeof(int32_t));
    printf("sizeof(int64_t) is %lu\n\n", sizeof(int64_t));
    
    printf("sizeof(uint8_t) is %lu\n", sizeof(uint8_t));
    printf("sizeof(uint16_t) is %lu\n", sizeof(uint16_t));
    printf("sizeof(uint32_t) is %lu\n", sizeof(uint32_t));
    printf("sizeof(uint64_t) is %lu\n", sizeof(uint64_t));
    
    return 0;
}
