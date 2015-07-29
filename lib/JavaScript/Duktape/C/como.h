#ifndef _COMO_H
#define _COMO_H

#include "duktape.h"

void _como_init_module(duk_context *ctx, const char *filename);
void _como_module_export (duk_context *ctx, const char *filename);

const char *_COMO_MODULE_PATH = "";

#ifdef __cplusplus
    #define EXTERNC extern "C"
#else
    #define EXTERNC
#endif

#define MODULE_EXPORT(ctx, filename) EXTERNC void _como_module_export (duk_context *ctx, const char *filename)
#define _COMO_INIT void _como_init_module(duk_context *ctx, const char *filename)

#ifdef _WIN32
    #include <windows.h>
    #define dlsym(x, y) GetProcAddress((HMODULE)x, y)
    #define dlopen(x,y) (void*)LoadLibrary(x)
    #define dlclose(x) FreeLibrary((HMODULE)x)
    #define COMO_MODULE_INIT(ctx, filename) EXTERNC __declspec(dllexport) _COMO_INIT
#else
    #include <dlfcn.h>
    #define COMO_MODULE_INIT(ctx, filename) EXTERNC _COMO_INIT
#endif

#ifndef DUKTAPE_DONT_LOAD_SHARED
COMO_MODULE_INIT (ctx, filename) {
    _COMO_MODULE_PATH = filename;
    _como_module_export(ctx,filename);
}
#endif

#endif /*_COMO_H*/
