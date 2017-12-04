

struct sockaddr
{
	sa_family_t sa_family;
	char data[14];
}



struct in_addr
{
	unint32_t s_addr;
}

struct sockaddr_in
{
	sa_family_t sin_family;  // internet protocol AF_INET
	in_port_t sin_port;      // address port               2 bytes
	struct in_addr sin_addr; // IPv4                       4 bytes
	char sin_zero[8];        // not used                   8 bytes
}

