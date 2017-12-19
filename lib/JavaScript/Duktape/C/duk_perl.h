#ifndef _DUKPERL_H
#define _DUKPERL_H

#include "./lib/duktape.h"

typedef struct {
    /* The double value in the union is there to ensure alignment is
     * good for IEEE doubles too.  In many 32-bit environments 4 bytes
     * would be sufficiently aligned and the double value is unnecessary.
     */
    union {
        size_t sz;
        double d;
    } u;
} perlDukMemHdr;

typedef struct {
    int timeout;
    size_t max_memory;
    size_t total_allocated;
    duk_context *ctx;
} perlDuk;

void _duk_perl_init_module(duk_context *ctx, const char *filename);
void _duk_perl_module_export (duk_context *ctx, const char *filename);

const char *_DUKPERL_MODULE_PATH = "";

#ifdef __cplusplus
    #define EXTERNC extern "C"
#else
    #define EXTERNC
#endif

#define MODULE_EXPORT(ctx, filename) EXTERNC void _duk_perl_module_export (duk_context *ctx, const char *filename)
#define _DUKPERL_INIT void _duk_perl_init_module(duk_context *ctx, const char *filename)

#ifdef _WIN32
    #include <windows.h>
    #define dlsym(x, y) GetProcAddress((HMODULE)x, y)
    #define dlopen(x,y) (void*)LoadLibrary(x)
    #define dlclose(x) FreeLibrary((HMODULE)x)
    #define DUKPERL_MODULE_INIT(ctx, filename) EXTERNC __declspec(dllexport) _DUKPERL_INIT
#else
    #include <dlfcn.h>
    #define DUKPERL_MODULE_INIT(ctx, filename) EXTERNC _DUKPERL_INIT
#endif

#ifndef PERL_DUKTAPE
DUKPERL_MODULE_INIT (ctx, filename) {
    _DUKPERL_MODULE_PATH = filename;
    _duk_perl_module_export(ctx,filename);
}
#endif

#endif /*_DUKPERL_H*/
