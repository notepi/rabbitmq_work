#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <amqp.h>
#include <amqp_framing.h>

#include <assert.h>

#include <utils.h>

extern "C"{
	#include <ngx_palloc.h>
}

int main()
{
	ngx_create_pool(100, NULL);
}
