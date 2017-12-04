#include <stdio.h>
#include <stdbool.h>
#include "inttypes.h"

const int LOGIN_MAX = 64;
const int TOKEN_MAX = 64;
const int MESSAGE_MAX = 128;

// Login
struct LoginRequest {
	char login[LOGIN_MAX];
};
typedef struct LoginRequest LoginRequest;

struct LoginResponse {
	char token[TOKEN_MAX];
};
typedef struct LoginResponse LoginResponse;

//Message
struct MessageRequest {
	char token[TOKEN_MAX];
    char message[MESSAGE_MAX];
	int type;
};
typedef struct MessageRequest MessageRequest;
