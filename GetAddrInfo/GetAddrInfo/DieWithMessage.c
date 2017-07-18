//
//  DieWithMessage.c
//  GetAddrInfo 
//
//  Created by Anton Kovalchuk on 16.04.17.
//  Copyright © 2017 Anton Kovalchuk. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "DieWithMessage.h"

void DieWithUserMessage(const char *msg, const char *detail) {
	printf("%s: %s\n",msg,detail);
    exit(1);
}

void DieWithSystemMessage(const char *msg) {
    perror(msg);
    exit(1);
}
