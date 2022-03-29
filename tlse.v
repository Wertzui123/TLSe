module tlse

$if gcboehm ? {
	#define TLS_malloc(x,u) ((void)(u),GC_malloc(x))
	#define TLS_realloc(x,u) ((void)(u),GC_realloc(x))
	#define TLS_free(x,u) ((void)(u),GC_free(x))
}

#flag -I @VMODROOT
#flag -D TLS_AMALGAMATION
#flag -D LTC_NO_ASM
$if windows {
	#flag -l ws2_32
}
#include "thirdparty/adapter.c"
