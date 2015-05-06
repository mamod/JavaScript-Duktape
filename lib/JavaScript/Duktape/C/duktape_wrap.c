#define NO_XSLOCKS
#define PERL_NO_GET_CONTEXT

#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#include "ppport.h"

#define NEED_newRV_noinc
#include "duktape.c"



#ifndef Newx
#  define Newx(v,n,t) New(0,v,n,t)
#endif

void Throw_pDuk_Error (){
    croak("Duk::Error");
}

int dump_stack(duk_context *ctx, const char *name) {
    duk_idx_t i, n;
    n = duk_get_top(ctx);
    printf("%s (top=%ld):", name, (long) n);
    for (i = 0; i < n; i++) {
        printf(" ");
        duk_dup(ctx, i);
        printf("%s", duk_safe_to_string(ctx, -1));
        duk_pop(ctx);
    }
    printf("\n");
    fflush(stdout);
    return 1;
}

void perl_duk_reset_top(duk_context *ctx);

void fatal_handler (duk_context *ctx, duk_errcode_t code, const char *str) {
    perl_duk_reset_top(ctx);
    if (duk_get_error_code(ctx, -1) > 0){
        croak(duk_to_string(ctx, -1));
    } else {
        croak("duktape uncaught error");
    }
}

SV *new(const char * classname) {
    duk_context *ctx;
    SV         *obj;
    SV         *obj_ref;

    Newx(ctx, 1, duk_context);
    ctx = duk_create_heap(NULL, NULL, NULL, NULL, fatal_handler);

    obj = newSViv((IV)ctx);
    obj_ref = newRV_noinc(obj);
    sv_bless(obj_ref, gv_stashpv(classname, GV_ADD));
    SvREADONLY_on(obj);

    return obj_ref;
}

/**
  * quick helper function to reset stack top
******************************************************************************/
void perl_duk_reset_top(duk_context *ctx){
    duk_idx_t top = duk_get_top(ctx);
    duk_pop_n(ctx, top);
}

/**
  * duk_safe_call hack
  * ===================
  * 
  *
******************************************************************************/
static SV * gCallback = (SV*)NULL;
int call_safe_perl_sub(duk_context *ctx) {

    dTHX;
    dSP;
    int n;
    dJMPENV;
    JMPENV_PUSH(n);
    char *error = NULL;
    STRLEN error_len;
    SV    *sv;
    int count;
    int ret = 0;
    ENTER;
    SAVETMPS;
    PUSHMARK(SP);
    PUTBACK;
    if (n == 0){
        count = call_sv(gCallback, G_NOARGS | G_SCALAR);
        JMPENV_POP;
        SPAGAIN;
        if( count > 0) {
            sv = POPs;
            if(SvIOK(sv)) {
                ret = SvIV(sv);
            }
        }
    } else {
        JMPENV_POP;
        FREETMPS;
        LEAVE;
        
        if (SvTRUE(ERRSV)){
            error = SvPV( ERRSV, error_len);
            duk_error(ctx, 100, error);
        }
        
        JMPENV_JUMP(n);
    }

    PUTBACK;
    FREETMPS;
    LEAVE;
    return ret;
}

duk_int_t perl_duk_safe_call(duk_context *ctx, SV *func, duk_idx_t nargs, duk_idx_t nrets) {
    
    dTHX;
    duk_int_t ret;

    if (gCallback == (SV*)NULL) {
        gCallback = newSVsv(func);
    } else {
        SvSetSV(gCallback, func);
    }

    int n;
    dJMPENV;
    JMPENV_PUSH(n);
    if (n == 0){
        ret = duk_safe_call(ctx, call_safe_perl_sub, nargs, nrets);
        JMPENV_POP;
    } else {
        JMPENV_POP;
        JMPENV_JUMP(n);
    }

    if (ret != 0){ Throw_pDuk_Error(); }
    return ret;
}


/**
  * process.call_perl_sub(ptr)
******************************************************************************/
static const int call_perl_function(duk_context *ctx) {
    duk_push_current_function(ctx);
    duk_get_prop_string(ctx, -1, "_my_perl_sub");
    SV *sub = duk_require_pointer(ctx, -1);
    duk_pop_2(ctx);

    dSP;
    int count;
    ENTER;
    SAVETMPS;
    PUSHMARK(SP);
    XPUSHs(sub);
    PUTBACK;
    call_pv("JavaScript::Duktape::Vm::call_function", G_DISCARD | G_EVAL);
    PUTBACK;
    FREETMPS;
    LEAVE;
    if (SvTRUE(ERRSV)){
        duk_throw(ctx);
    }
    return 1;
}

duk_idx_t perl_push_function (duk_context *ctx, SV *sub, duk_idx_t nargs) {
    duk_idx_t n = duk_push_c_function(ctx, call_perl_function, nargs);
    duk_push_pointer(ctx, sub);
    duk_put_prop_string(ctx, -2, "_my_perl_sub");
    return n;
}

SV *perl_duk_require_context(duk_context *ctx, duk_idx_t index) {
    duk_context *ctx2;
    SV         *obj;
    SV         *obj_ref;

    Newx(ctx2, 1, duk_context);

    ctx2 = duk_require_context(ctx, index);

    obj = newSViv((IV)ctx2);
    obj_ref = newRV_noinc(obj);
    sv_bless(obj_ref, gv_stashpv("JavaScript::Duktape::Vm", GV_ADD));
    SvREADONLY_on(obj);
    return obj_ref;
}

void DESTROY(duk_context *ctx) {
    printf("Destroying %p\n", ctx);
    // Safefree(ctx);
    //duk_destroy_heap(ctx);
}



/* 
    Auto Generated C Code by parser.pl
    parser.pl reads duktape.h file and create both
    perl & C map code to Duktape API
*/

//duk_int_t duk_pcall_method(duk_context *ctx, duk_idx_t nargs);
duk_int_t aperl_duk_pcall(duk_context *ctx, duk_idx_t nargs) {
    duk_int_t ret = duk_pcall(ctx, nargs);
    //if (ret == 1){ croak("Duk::Error"); }
    return ret;
}

//void *duk_alloc(duk_context *ctx, duk_size_t size);
void *aperl_duk_alloc(duk_context *ctx, duk_size_t size) {
    void *ret = duk_alloc(ctx, size);
    return ret;
}

//void *duk_alloc_raw(duk_context *ctx, duk_size_t size);
void *aperl_duk_alloc_raw(duk_context *ctx, duk_size_t size) {
    void *ret = duk_alloc_raw(ctx, size);
    return ret;
}

//void duk_base64_decode(duk_context *ctx, duk_idx_t index);
void aperl_duk_base64_decode(duk_context *ctx, duk_idx_t index) {
    duk_base64_decode(ctx, index);
}

//const char *duk_base64_encode(duk_context *ctx, duk_idx_t index);
const char *aperl_duk_base64_encode(duk_context *ctx, duk_idx_t index) {
    const char *ret = duk_base64_encode(ctx, index);
    return ret;
}

//void duk_call(duk_context *ctx, duk_idx_t nargs);
void aperl_duk_call(duk_context *ctx, duk_idx_t nargs) {
    duk_call(ctx, nargs);
}

//void duk_call_method(duk_context *ctx, duk_idx_t nargs);
void aperl_duk_call_method(duk_context *ctx, duk_idx_t nargs) {
    duk_call_method(ctx, nargs);
}

//void duk_call_prop(duk_context *ctx, duk_idx_t obj_index, duk_idx_t nargs);
void aperl_duk_call_prop(duk_context *ctx, duk_idx_t obj_index, duk_idx_t nargs) {
    duk_call_prop(ctx, obj_index, nargs);
}

//duk_codepoint_t duk_char_code_at(duk_context *ctx, duk_idx_t index, duk_size_t char_offset);
duk_codepoint_t aperl_duk_char_code_at(duk_context *ctx, duk_idx_t index, duk_size_t char_offset) {
    duk_codepoint_t ret = duk_char_code_at(ctx, index, char_offset);
    return ret;
}

//duk_bool_t duk_check_stack(duk_context *ctx, duk_idx_t extra);
duk_bool_t aperl_duk_check_stack(duk_context *ctx, duk_idx_t extra) {
    duk_bool_t ret = duk_check_stack(ctx, extra);
    return ret;
}

//duk_bool_t duk_check_stack_top(duk_context *ctx, duk_idx_t top);
duk_bool_t aperl_duk_check_stack_top(duk_context *ctx, duk_idx_t top) {
    duk_bool_t ret = duk_check_stack_top(ctx, top);
    return ret;
}

//duk_bool_t duk_check_type(duk_context *ctx, duk_idx_t index, duk_int_t type);
duk_bool_t aperl_duk_check_type(duk_context *ctx, duk_idx_t index, duk_int_t type) {
    duk_bool_t ret = duk_check_type(ctx, index, type);
    return ret;
}

//duk_bool_t duk_check_type_mask(duk_context *ctx, duk_idx_t index, duk_uint_t mask);
duk_bool_t aperl_duk_check_type_mask(duk_context *ctx, duk_idx_t index, duk_uint_t mask) {
    duk_bool_t ret = duk_check_type_mask(ctx, index, mask);
    return ret;
}

//void duk_compact(duk_context *ctx, duk_idx_t obj_index);
void aperl_duk_compact(duk_context *ctx, duk_idx_t obj_index) {
    duk_compact(ctx, obj_index);
}

//void duk_compile(duk_context *ctx, duk_uint_t flags);
void aperl_duk_compile(duk_context *ctx, duk_uint_t flags) {
    duk_compile(ctx, flags);
}

//void duk_compile_file(duk_context *ctx, duk_uint_t flags, const char *path);
void aperl_duk_compile_file(duk_context *ctx, duk_uint_t flags, const char *path) {
    duk_compile_file(ctx, flags, path);
}

//void duk_compile_lstring(duk_context *ctx, duk_uint_t flags, const char *src, duk_size_t len);
void aperl_duk_compile_lstring(duk_context *ctx, duk_uint_t flags, const char *src, duk_size_t len) {
    duk_compile_lstring(ctx, flags, src, len);
}

//void duk_compile_lstring_filename(duk_context *ctx, duk_uint_t flags, const char *src, duk_size_t len);
void aperl_duk_compile_lstring_filename(duk_context *ctx, duk_uint_t flags, const char *src, duk_size_t len) {
    duk_compile_lstring_filename(ctx, flags, src, len);
}

//void duk_compile_string(duk_context *ctx, duk_uint_t flags, const char *src);
void aperl_duk_compile_string(duk_context *ctx, duk_uint_t flags, const char *src) {
    duk_compile_string(ctx, flags, src);
}

//void duk_compile_string_filename(duk_context *ctx, duk_uint_t flags, const char *src);
void aperl_duk_compile_string_filename(duk_context *ctx, duk_uint_t flags, const char *src) {
    duk_compile_string_filename(ctx, flags, src);
}

//void duk_concat(duk_context *ctx, duk_idx_t count);
void aperl_duk_concat(duk_context *ctx, duk_idx_t count) {
    duk_concat(ctx, count);
}

//void duk_copy(duk_context *ctx, duk_idx_t from_index, duk_idx_t to_index);
void aperl_duk_copy(duk_context *ctx, duk_idx_t from_index, duk_idx_t to_index) {
    duk_copy(ctx, from_index, to_index);
}

//void duk_decode_string(duk_context *ctx, duk_idx_t index, duk_decode_char_function callback, void *udata);
void aperl_duk_decode_string(duk_context *ctx, duk_idx_t index, duk_decode_char_function callback, void *udata) {
    duk_decode_string(ctx, index, callback, udata);
}

//void duk_def_prop(duk_context *ctx, duk_idx_t obj_index, duk_uint_t flags);
void aperl_duk_def_prop(duk_context *ctx, duk_idx_t obj_index, duk_uint_t flags) {
    duk_def_prop(ctx, obj_index, flags);
}

//duk_bool_t duk_del_prop(duk_context *ctx, duk_idx_t obj_index);
duk_bool_t aperl_duk_del_prop(duk_context *ctx, duk_idx_t obj_index) {
    duk_bool_t ret = duk_del_prop(ctx, obj_index);
    return ret;
}

//duk_bool_t duk_del_prop_index(duk_context *ctx, duk_idx_t obj_index, duk_uarridx_t arr_index);
duk_bool_t aperl_duk_del_prop_index(duk_context *ctx, duk_idx_t obj_index, duk_uarridx_t arr_index) {
    duk_bool_t ret = duk_del_prop_index(ctx, obj_index, arr_index);
    return ret;
}

//duk_bool_t duk_del_prop_string(duk_context *ctx, duk_idx_t obj_index, const char *key);
duk_bool_t aperl_duk_del_prop_string(duk_context *ctx, duk_idx_t obj_index, const char *key) {
    duk_bool_t ret = duk_del_prop_string(ctx, obj_index, key);
    return ret;
}

//void duk_destroy_heap(duk_context *ctx);
void aperl_duk_destroy_heap(duk_context *ctx) {
    duk_destroy_heap(ctx);
}

//void duk_dump_context_stderr(duk_context *ctx);
void aperl_duk_dump_context_stderr(duk_context *ctx) {
    duk_dump_context_stderr(ctx);
}

//void duk_dump_context_stdout(duk_context *ctx);
void aperl_duk_dump_context_stdout(duk_context *ctx) {
    duk_dump_context_stdout(ctx);
}

//void duk_dup(duk_context *ctx, duk_idx_t from_index);
void aperl_duk_dup(duk_context *ctx, duk_idx_t from_index) {
    duk_dup(ctx, from_index);
}

//void duk_dup_top(duk_context *ctx);
void aperl_duk_dup_top(duk_context *ctx) {
    duk_dup_top(ctx);
}

//void duk_enum(duk_context *ctx, duk_idx_t obj_index, duk_uint_t enum_flags);
void aperl_duk_enum(duk_context *ctx, duk_idx_t obj_index, duk_uint_t enum_flags) {
    duk_enum(ctx, obj_index, enum_flags);
}

//duk_bool_t duk_equals(duk_context *ctx, duk_idx_t index1, duk_idx_t index2);
duk_bool_t aperl_duk_equals(duk_context *ctx, duk_idx_t index1, duk_idx_t index2) {
    duk_bool_t ret = duk_equals(ctx, index1, index2);
    return ret;
}

//void duk_error_va(duk_context *ctx, duk_errcode_t err_code, const char *fmt, va_list ap);
void aperl_duk_error_va(duk_context *ctx, duk_errcode_t err_code, const char *fmt, va_list ap) {
    duk_error_va(ctx, err_code, fmt, ap);
}

//void duk_eval(duk_context *ctx);
void aperl_duk_eval(duk_context *ctx) {
    duk_eval(ctx);
}

//void duk_eval_file(duk_context *ctx, const char *path);
void aperl_duk_eval_file(duk_context *ctx, const char *path) {
    duk_eval_file(ctx, path);
}

//void duk_eval_file_noresult(duk_context *ctx, const char *path);
void aperl_duk_eval_file_noresult(duk_context *ctx, const char *path) {
    duk_eval_file_noresult(ctx, path);
}

//void duk_eval_lstring(duk_context *ctx, const char *src, duk_size_t len);
void aperl_duk_eval_lstring(duk_context *ctx, const char *src, duk_size_t len) {
    duk_eval_lstring(ctx, src, len);
}

//void duk_eval_lstring_noresult(duk_context *ctx, const char *src, duk_size_t len);
void aperl_duk_eval_lstring_noresult(duk_context *ctx, const char *src, duk_size_t len) {
    duk_eval_lstring_noresult(ctx, src, len);
}

//void duk_eval_noresult(duk_context *ctx);
void aperl_duk_eval_noresult(duk_context *ctx) {
    duk_eval_noresult(ctx);
}

//void duk_eval_string(duk_context *ctx, const char *src);
void aperl_duk_eval_string(duk_context *ctx, const char *src) {
    duk_eval_string(ctx, src);
}

//void duk_eval_string_noresult(duk_context *ctx, const char *src);
void aperl_duk_eval_string_noresult(duk_context *ctx, const char *src) {
    duk_eval_string_noresult(ctx, src);
}

//void duk_fatal(duk_context *ctx, duk_errcode_t err_code, const char *err_msg);
void aperl_duk_fatal(duk_context *ctx, duk_errcode_t err_code, const char *err_msg) {
    duk_fatal(ctx, err_code, err_msg);
}

//void duk_free(duk_context *ctx, void *ptr);
void aperl_duk_free(duk_context *ctx, void *ptr) {
    duk_free(ctx, ptr);
}

//void duk_free_raw(duk_context *ctx, void *ptr);
void aperl_duk_free_raw(duk_context *ctx, void *ptr) {
    duk_free_raw(ctx, ptr);
}

//void duk_gc(duk_context *ctx, duk_uint_t flags);
void aperl_duk_gc(duk_context *ctx, duk_uint_t flags) {
    duk_gc(ctx, flags);
}

//duk_bool_t duk_get_boolean(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_get_boolean(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_get_boolean(ctx, index);
    return ret;
}

//void *duk_get_buffer(duk_context *ctx, duk_idx_t index, duk_size_t *out_size);
void *aperl_duk_get_buffer(duk_context *ctx, duk_idx_t index, SV *out_len) {
    duk_size_t sz;
    void *ret = duk_get_buffer(ctx, index, &sz);
    sv_setnv(out_len, sz);
    return ret;
}

//duk_c_function duk_get_c_function(duk_context *ctx, duk_idx_t index);
duk_c_function aperl_duk_get_c_function(duk_context *ctx, duk_idx_t index) {
    duk_c_function ret = duk_get_c_function(ctx, index);
    return ret;
}

//duk_context *duk_get_context(duk_context *ctx, duk_idx_t index);
duk_context *aperl_duk_get_context(duk_context *ctx, duk_idx_t index) {
    duk_context *ret = duk_get_context(ctx, index);
    return ret;
}

//duk_int_t duk_get_current_magic(duk_context *ctx);
duk_int_t aperl_duk_get_current_magic(duk_context *ctx) {
    duk_int_t ret = duk_get_current_magic(ctx);
    return ret;
}

//duk_errcode_t duk_get_error_code(duk_context *ctx, duk_idx_t index);
duk_errcode_t aperl_duk_get_error_code(duk_context *ctx, duk_idx_t index) {
    duk_errcode_t ret = duk_get_error_code(ctx, index);
    return ret;
}

//void duk_get_finalizer(duk_context *ctx, duk_idx_t index);
void aperl_duk_get_finalizer(duk_context *ctx, duk_idx_t index) {
    duk_get_finalizer(ctx, index);
}

//duk_bool_t duk_get_global_string(duk_context *ctx, const char *key);
duk_bool_t aperl_duk_get_global_string(duk_context *ctx, const char *key) {
    duk_bool_t ret = duk_get_global_string(ctx, key);
    return ret;
}

//void *duk_get_heapptr(duk_context *ctx, duk_idx_t index);
void *aperl_duk_get_heapptr(duk_context *ctx, duk_idx_t index) {
    void *ret = duk_get_heapptr(ctx, index);
    return ret;
}

//duk_int_t duk_get_int(duk_context *ctx, duk_idx_t index);
duk_int_t aperl_duk_get_int(duk_context *ctx, duk_idx_t index) {
    duk_int_t ret = duk_get_int(ctx, index);
    return ret;
}

//duk_size_t duk_get_length(duk_context *ctx, duk_idx_t index);
duk_size_t aperl_duk_get_length(duk_context *ctx, duk_idx_t index) {
    duk_size_t ret = duk_get_length(ctx, index);
    return ret;
}

//const char *duk_get_lstring(duk_context *ctx, duk_idx_t index, duk_size_t *out_len);
const char *aperl_duk_get_lstring(duk_context *ctx, duk_idx_t index, SV *out_len) {
    duk_size_t sz;
    const char *ret = duk_get_lstring(ctx, index, &sz);
    sv_setnv(out_len, sz);
    return ret;
}

//duk_int_t duk_get_magic(duk_context *ctx, duk_idx_t index);
duk_int_t aperl_duk_get_magic(duk_context *ctx, duk_idx_t index) {
    duk_int_t ret = duk_get_magic(ctx, index);
    return ret;
}

//void duk_get_memory_functions(duk_context *ctx, duk_memory_functions *out_funcs);
void aperl_duk_get_memory_functions(duk_context *ctx, duk_memory_functions *out_funcs) {
    duk_get_memory_functions(ctx, out_funcs);
}

//duk_double_t duk_get_number(duk_context *ctx, duk_idx_t index);
duk_double_t aperl_duk_get_number(duk_context *ctx, duk_idx_t index) {
    duk_double_t ret = duk_get_number(ctx, index);
    return ret;
}

//void *duk_get_pointer(duk_context *ctx, duk_idx_t index);
void *aperl_duk_get_pointer(duk_context *ctx, duk_idx_t index) {
    void *ret = duk_get_pointer(ctx, index);
    return ret;
}

//duk_bool_t duk_get_prop(duk_context *ctx, duk_idx_t obj_index);
duk_bool_t aperl_duk_get_prop(duk_context *ctx, duk_idx_t obj_index) {
    duk_bool_t ret = duk_get_prop(ctx, obj_index);
    return ret;
}

//duk_bool_t duk_get_prop_index(duk_context *ctx, duk_idx_t obj_index, duk_uarridx_t arr_index);
duk_bool_t aperl_duk_get_prop_index(duk_context *ctx, duk_idx_t obj_index, duk_uarridx_t arr_index) {
    duk_bool_t ret = duk_get_prop_index(ctx, obj_index, arr_index);
    return ret;
}

//duk_bool_t duk_get_prop_string(duk_context *ctx, duk_idx_t obj_index, const char *key);
duk_bool_t aperl_duk_get_prop_string(duk_context *ctx, duk_idx_t obj_index, const char *key) {
    duk_bool_t ret = duk_get_prop_string(ctx, obj_index, key);
    return ret;
}

//void duk_get_prototype(duk_context *ctx, duk_idx_t index);
void aperl_duk_get_prototype(duk_context *ctx, duk_idx_t index) {
    duk_get_prototype(ctx, index);
}

//const char *duk_get_string(duk_context *ctx, duk_idx_t index);
const char *aperl_duk_get_string(duk_context *ctx, duk_idx_t index) {
    const char *ret = duk_get_string(ctx, index);
    return ret;
}

//duk_idx_t duk_get_top(duk_context *ctx);
duk_idx_t aperl_duk_get_top(duk_context *ctx) {
    duk_idx_t ret = duk_get_top(ctx);
    return ret;
}

//duk_idx_t duk_get_top_index(duk_context *ctx);
duk_idx_t aperl_duk_get_top_index(duk_context *ctx) {
    duk_idx_t ret = duk_get_top_index(ctx);
    return ret;
}

//duk_int_t duk_get_type(duk_context *ctx, duk_idx_t index);
duk_int_t aperl_duk_get_type(duk_context *ctx, duk_idx_t index) {
    duk_int_t ret = duk_get_type(ctx, index);
    return ret;
}

//duk_uint_t duk_get_type_mask(duk_context *ctx, duk_idx_t index);
duk_uint_t aperl_duk_get_type_mask(duk_context *ctx, duk_idx_t index) {
    duk_uint_t ret = duk_get_type_mask(ctx, index);
    return ret;
}

//duk_uint_t duk_get_uint(duk_context *ctx, duk_idx_t index);
duk_uint_t aperl_duk_get_uint(duk_context *ctx, duk_idx_t index) {
    duk_uint_t ret = duk_get_uint(ctx, index);
    return ret;
}

//duk_bool_t duk_has_prop(duk_context *ctx, duk_idx_t obj_index);
duk_bool_t aperl_duk_has_prop(duk_context *ctx, duk_idx_t obj_index) {
    duk_bool_t ret = duk_has_prop(ctx, obj_index);
    return ret;
}

//duk_bool_t duk_has_prop_index(duk_context *ctx, duk_idx_t obj_index, duk_uarridx_t arr_index);
duk_bool_t aperl_duk_has_prop_index(duk_context *ctx, duk_idx_t obj_index, duk_uarridx_t arr_index) {
    duk_bool_t ret = duk_has_prop_index(ctx, obj_index, arr_index);
    return ret;
}

//duk_bool_t duk_has_prop_string(duk_context *ctx, duk_idx_t obj_index, const char *key);
duk_bool_t aperl_duk_has_prop_string(duk_context *ctx, duk_idx_t obj_index, const char *key) {
    duk_bool_t ret = duk_has_prop_string(ctx, obj_index, key);
    return ret;
}

//void duk_hex_decode(duk_context *ctx, duk_idx_t index);
void aperl_duk_hex_decode(duk_context *ctx, duk_idx_t index) {
    duk_hex_decode(ctx, index);
}

//const char *duk_hex_encode(duk_context *ctx, duk_idx_t index);
const char *aperl_duk_hex_encode(duk_context *ctx, duk_idx_t index) {
    const char *ret = duk_hex_encode(ctx, index);
    return ret;
}

//void duk_insert(duk_context *ctx, duk_idx_t to_index);
void aperl_duk_insert(duk_context *ctx, duk_idx_t to_index) {
    duk_insert(ctx, to_index);
}

//duk_bool_t duk_is_array(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_is_array(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_is_array(ctx, index);
    return ret;
}

//duk_bool_t duk_is_boolean(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_is_boolean(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_is_boolean(ctx, index);
    return ret;
}

//duk_bool_t duk_is_bound_function(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_is_bound_function(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_is_bound_function(ctx, index);
    return ret;
}

//duk_bool_t duk_is_buffer(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_is_buffer(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_is_buffer(ctx, index);
    return ret;
}

//duk_bool_t duk_is_c_function(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_is_c_function(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_is_c_function(ctx, index);
    return ret;
}

//duk_bool_t duk_is_callable(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_is_callable(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_is_callable(ctx, index);
    return ret;
}

//duk_bool_t duk_is_constructor_call(duk_context *ctx);
duk_bool_t aperl_duk_is_constructor_call(duk_context *ctx) {
    duk_bool_t ret = duk_is_constructor_call(ctx);
    return ret;
}

//duk_bool_t duk_is_dynamic_buffer(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_is_dynamic_buffer(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_is_dynamic_buffer(ctx, index);
    return ret;
}

//duk_bool_t duk_is_ecmascript_function(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_is_ecmascript_function(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_is_ecmascript_function(ctx, index);
    return ret;
}

//duk_bool_t duk_is_error(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_is_error(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_is_error(ctx, index);
    return ret;
}

//duk_bool_t duk_is_fixed_buffer(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_is_fixed_buffer(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_is_fixed_buffer(ctx, index);
    return ret;
}

//duk_bool_t duk_is_function(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_is_function(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_is_function(ctx, index);
    return ret;
}

//duk_bool_t duk_is_lightfunc(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_is_lightfunc(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_is_lightfunc(ctx, index);
    return ret;
}

//duk_bool_t duk_is_nan(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_is_nan(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_is_nan(ctx, index);
    return ret;
}

//duk_bool_t duk_is_null(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_is_null(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_is_null(ctx, index);
    return ret;
}

//duk_bool_t duk_is_null_or_undefined(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_is_null_or_undefined(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_is_null_or_undefined(ctx, index);
    return ret;
}

//duk_bool_t duk_is_number(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_is_number(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_is_number(ctx, index);
    return ret;
}

//duk_bool_t duk_is_object(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_is_object(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_is_object(ctx, index);
    return ret;
}

//duk_bool_t duk_is_object_coercible(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_is_object_coercible(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_is_object_coercible(ctx, index);
    return ret;
}

//duk_bool_t duk_is_pointer(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_is_pointer(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_is_pointer(ctx, index);
    return ret;
}

//duk_bool_t duk_is_primitive(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_is_primitive(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_is_primitive(ctx, index);
    return ret;
}

//duk_bool_t duk_is_strict_call(duk_context *ctx);
duk_bool_t aperl_duk_is_strict_call(duk_context *ctx) {
    duk_bool_t ret = duk_is_strict_call(ctx);
    return ret;
}

//duk_bool_t duk_is_string(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_is_string(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_is_string(ctx, index);
    return ret;
}

//duk_bool_t duk_is_thread(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_is_thread(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_is_thread(ctx, index);
    return ret;
}

//duk_bool_t duk_is_undefined(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_is_undefined(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_is_undefined(ctx, index);
    return ret;
}

//duk_bool_t duk_is_valid_index(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_is_valid_index(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_is_valid_index(ctx, index);
    return ret;
}

//void duk_join(duk_context *ctx, duk_idx_t count);
void aperl_duk_join(duk_context *ctx, duk_idx_t count) {
    duk_join(ctx, count);
}

//void duk_json_decode(duk_context *ctx, duk_idx_t index);
void aperl_duk_json_decode(duk_context *ctx, duk_idx_t index) {
    duk_json_decode(ctx, index);
}

//const char *duk_json_encode(duk_context *ctx, duk_idx_t index);
const char *aperl_duk_json_encode(duk_context *ctx, duk_idx_t index) {
    const char *ret = duk_json_encode(ctx, index);
    return ret;
}

//void duk_log_va(duk_context *ctx, duk_int_t level, const char *fmt, va_list ap);
void aperl_duk_log_va(duk_context *ctx, duk_int_t level, const char *fmt, va_list ap) {
    duk_log_va(ctx, level, fmt, ap);
}

//void duk_map_string(duk_context *ctx, duk_idx_t index, duk_map_char_function callback, void *udata);
void aperl_duk_map_string(duk_context *ctx, duk_idx_t index, duk_map_char_function callback, void *udata) {
    duk_map_string(ctx, index, callback, udata);
}

//void duk_new(duk_context *ctx, duk_idx_t nargs);
void aperl_duk_new(duk_context *ctx, duk_idx_t nargs) {
    duk_new(ctx, nargs);
}

//duk_bool_t duk_next(duk_context *ctx, duk_idx_t enum_index, duk_bool_t get_value);
duk_bool_t aperl_duk_next(duk_context *ctx, duk_idx_t enum_index, duk_bool_t get_value) {
    duk_bool_t ret = duk_next(ctx, enum_index, get_value);
    return ret;
}

//duk_idx_t duk_normalize_index(duk_context *ctx, duk_idx_t index);
duk_idx_t aperl_duk_normalize_index(duk_context *ctx, duk_idx_t index) {
    duk_idx_t ret = duk_normalize_index(ctx, index);
    return ret;
}

//duk_int_t duk_pcall(duk_context *ctx, duk_idx_t nargs);
duk_int_t aperl_duk_pcall_method(duk_context *ctx, duk_idx_t nargs) {
    duk_int_t ret = duk_pcall_method(ctx, nargs);
    return ret;
}



//duk_int_t duk_pcall_prop(duk_context *ctx, duk_idx_t obj_index, duk_idx_t nargs);
duk_int_t aperl_duk_pcall_prop(duk_context *ctx, duk_idx_t obj_index, duk_idx_t nargs) {
    duk_int_t ret = duk_pcall_prop(ctx, obj_index, nargs);
    return ret;
}

//duk_int_t duk_pcompile(duk_context *ctx, duk_uint_t flags);
duk_int_t aperl_duk_pcompile(duk_context *ctx, duk_uint_t flags) {
    duk_int_t ret = duk_pcompile(ctx, flags);
    return ret;
}

//duk_int_t duk_pcompile_file(duk_context *ctx, duk_uint_t flags, const char *path);
duk_int_t aperl_duk_pcompile_file(duk_context *ctx, duk_uint_t flags, const char *path) {
    duk_int_t ret = duk_pcompile_file(ctx, flags, path);
    return ret;
}

//duk_int_t duk_pcompile_lstring(duk_context *ctx, duk_uint_t flags, const char *src, duk_size_t len);
duk_int_t aperl_duk_pcompile_lstring(duk_context *ctx, duk_uint_t flags, const char *src, duk_size_t len) {
    duk_int_t ret = duk_pcompile_lstring(ctx, flags, src, len);
    return ret;
}

//duk_int_t duk_pcompile_lstring_filename(duk_context *ctx, duk_uint_t flags, const char *src, duk_size_t len);
duk_int_t aperl_duk_pcompile_lstring_filename(duk_context *ctx, duk_uint_t flags, const char *src, duk_size_t len) {
    duk_int_t ret = duk_pcompile_lstring_filename(ctx, flags, src, len);
    return ret;
}

//duk_int_t duk_pcompile_string(duk_context *ctx, duk_uint_t flags, const char *src);
duk_int_t aperl_duk_pcompile_string(duk_context *ctx, duk_uint_t flags, const char *src) {
    duk_int_t ret = duk_pcompile_string(ctx, flags, src);
    return ret;
}

//duk_int_t duk_pcompile_string_filename(duk_context *ctx, duk_uint_t flags, const char *src);
duk_int_t aperl_duk_pcompile_string_filename(duk_context *ctx, duk_uint_t flags, const char *src) {
    duk_int_t ret = duk_pcompile_string_filename(ctx, flags, src);
    return ret;
}

//duk_int_t duk_peval(duk_context *ctx);
duk_int_t aperl_duk_peval(duk_context *ctx) {
    duk_int_t ret = duk_peval(ctx);
    return ret;
}

//duk_int_t duk_peval_file(duk_context *ctx, const char *path);
duk_int_t aperl_duk_peval_file(duk_context *ctx, const char *path) {
    duk_int_t ret = duk_peval_file(ctx, path);
    return ret;
}

//duk_int_t duk_peval_file_noresult(duk_context *ctx, const char *path);
duk_int_t aperl_duk_peval_file_noresult(duk_context *ctx, const char *path) {
    duk_int_t ret = duk_peval_file_noresult(ctx, path);
    return ret;
}

//duk_int_t duk_peval_lstring(duk_context *ctx, const char *src, duk_size_t len);
duk_int_t aperl_duk_peval_lstring(duk_context *ctx, const char *src, duk_size_t len) {
    duk_int_t ret = duk_peval_lstring(ctx, src, len);
    return ret;
}

//duk_int_t duk_peval_lstring_noresult(duk_context *ctx, const char *src, duk_size_t len);
duk_int_t aperl_duk_peval_lstring_noresult(duk_context *ctx, const char *src, duk_size_t len) {
    duk_int_t ret = duk_peval_lstring_noresult(ctx, src, len);
    return ret;
}

//duk_int_t duk_peval_noresult(duk_context *ctx);
duk_int_t aperl_duk_peval_noresult(duk_context *ctx) {
    duk_int_t ret = duk_peval_noresult(ctx);
    return ret;
}

//duk_int_t duk_peval_string(duk_context *ctx, const char *src);
duk_int_t aperl_duk_peval_string(duk_context *ctx, const char *src) {
    duk_int_t ret = duk_peval_string(ctx, src);
    return ret;
}

//duk_int_t duk_peval_string_noresult(duk_context *ctx, const char *src);
duk_int_t aperl_duk_peval_string_noresult(duk_context *ctx, const char *src) {
    duk_int_t ret = duk_peval_string_noresult(ctx, src);
    return ret;
}

//void duk_pop(duk_context *ctx);
void aperl_duk_pop(duk_context *ctx) {
    duk_pop(ctx);
}

//void duk_pop_2(duk_context *ctx);
void aperl_duk_pop_2(duk_context *ctx) {
    duk_pop_2(ctx);
}

//void duk_pop_3(duk_context *ctx);
void aperl_duk_pop_3(duk_context *ctx) {
    duk_pop_3(ctx);
}

//void duk_pop_n(duk_context *ctx, duk_idx_t count);
void aperl_duk_pop_n(duk_context *ctx, duk_idx_t count) {
    duk_pop_n(ctx, count);
}

//duk_idx_t duk_push_array(duk_context *ctx);
duk_idx_t aperl_duk_push_array(duk_context *ctx) {
    duk_idx_t ret = duk_push_array(ctx);
    return ret;
}

//void duk_push_boolean(duk_context *ctx, duk_bool_t val);
void aperl_duk_push_boolean(duk_context *ctx, duk_bool_t val) {
    duk_push_boolean(ctx, val);
}

//void *duk_push_buffer(duk_context *ctx, duk_size_t size, duk_bool_t dynamic);
void *aperl_duk_push_buffer(duk_context *ctx, duk_size_t size, duk_bool_t dynamic) {
    void *ret = duk_push_buffer(ctx, size, dynamic);
    return ret;
}

//duk_idx_t duk_push_c_function(duk_context *ctx, duk_c_function func, duk_idx_t nargs);
duk_idx_t aperl_duk_push_c_function(duk_context *ctx, duk_c_function func, duk_idx_t nargs) {
    duk_idx_t ret = duk_push_c_function(ctx, func, nargs);
    return ret;
}

//duk_idx_t duk_push_c_lightfunc(duk_context *ctx, duk_c_function func, duk_idx_t nargs, duk_idx_t length, duk_int_t magic);
duk_idx_t aperl_duk_push_c_lightfunc(duk_context *ctx, duk_c_function func, duk_idx_t nargs, duk_idx_t length, duk_int_t magic) {
    duk_idx_t ret = duk_push_c_lightfunc(ctx, func, nargs, length, magic);
    return ret;
}

//void duk_push_context_dump(duk_context *ctx);
void aperl_duk_push_context_dump(duk_context *ctx) {
    duk_push_context_dump(ctx);
}

//void duk_push_current_function(duk_context *ctx);
void aperl_duk_push_current_function(duk_context *ctx) {
    duk_push_current_function(ctx);
}

//void duk_push_current_thread(duk_context *ctx);
void aperl_duk_push_current_thread(duk_context *ctx) {
    duk_push_current_thread(ctx);
}

//void *duk_push_dynamic_buffer(duk_context *ctx, duk_size_t size);
void *aperl_duk_push_dynamic_buffer(duk_context *ctx, duk_size_t size) {
    void *ret = duk_push_dynamic_buffer(ctx, size);
    return ret;
}

//duk_idx_t duk_push_error_object_va(duk_context *ctx, duk_errcode_t err_code, const char *fmt, va_list ap);
duk_idx_t aperl_duk_push_error_object_va(duk_context *ctx, duk_errcode_t err_code, const char *fmt, va_list ap) {
    duk_idx_t ret = duk_push_error_object_va(ctx, err_code, fmt, ap);
    return ret;
}

//void duk_push_false(duk_context *ctx);
void aperl_duk_push_false(duk_context *ctx) {
    duk_push_false(ctx);
}

//void *duk_push_fixed_buffer(duk_context *ctx, duk_size_t size);
void *aperl_duk_push_fixed_buffer(duk_context *ctx, duk_size_t size) {
    void *ret = duk_push_fixed_buffer(ctx, size);
    return ret;
}

//void duk_push_global_object(duk_context *ctx);
void aperl_duk_push_global_object(duk_context *ctx) {
    duk_push_global_object(ctx);
}

//void duk_push_global_stash(duk_context *ctx);
void aperl_duk_push_global_stash(duk_context *ctx) {
    duk_push_global_stash(ctx);
}

//void duk_push_heap_stash(duk_context *ctx);
void aperl_duk_push_heap_stash(duk_context *ctx) {
    duk_push_heap_stash(ctx);
}

//duk_idx_t duk_push_heapptr(duk_context *ctx, void *ptr);
duk_idx_t aperl_duk_push_heapptr(duk_context *ctx, void *ptr) {
    duk_idx_t ret = duk_push_heapptr(ctx, ptr);
    return ret;
}

//void duk_push_int(duk_context *ctx, duk_int_t val);
void aperl_duk_push_int(duk_context *ctx, duk_int_t val) {
    duk_push_int(ctx, val);
}

//const char *duk_push_lstring(duk_context *ctx, const char *str, duk_size_t len);
const char *aperl_duk_push_lstring(duk_context *ctx, const char *str, duk_size_t len) {
    const char *ret = duk_push_lstring(ctx, str, len);
    return ret;
}

//void duk_push_nan(duk_context *ctx);
void aperl_duk_push_nan(duk_context *ctx) {
    duk_push_nan(ctx);
}

//void duk_push_null(duk_context *ctx);
void aperl_duk_push_null(duk_context *ctx) {
    duk_push_null(ctx);
}

//void duk_push_number(duk_context *ctx, duk_double_t val);
void aperl_duk_push_number(duk_context *ctx, duk_double_t val) {
    duk_push_number(ctx, val);
}

//duk_idx_t duk_push_object(duk_context *ctx);
duk_idx_t aperl_duk_push_object(duk_context *ctx) {
    duk_idx_t ret = duk_push_object(ctx);
    return ret;
}

//void duk_push_pointer(duk_context *ctx, void *p);
void aperl_duk_push_pointer(duk_context *ctx, void *p) {
    duk_push_pointer(ctx, p);
}

//const char *duk_push_string(duk_context *ctx, const char *str);
const char *aperl_duk_push_string(duk_context *ctx, const char *str) {
    const char *ret = duk_push_string(ctx, str);
    return ret;
}

//const char *duk_push_string_file(duk_context *ctx, const char *path);
const char *aperl_duk_push_string_file(duk_context *ctx, const char *path) {
    const char *ret = duk_push_string_file(ctx, path);
    return ret;
}

//void duk_push_this(duk_context *ctx);
void aperl_duk_push_this(duk_context *ctx) {
    duk_push_this(ctx);
}

//duk_idx_t duk_push_thread(duk_context *ctx);
duk_idx_t aperl_duk_push_thread(duk_context *ctx) {
    duk_idx_t ret = duk_push_thread(ctx);
    return ret;
}

//duk_idx_t duk_push_thread_new_globalenv(duk_context *ctx);
duk_idx_t aperl_duk_push_thread_new_globalenv(duk_context *ctx) {
    duk_idx_t ret = duk_push_thread_new_globalenv(ctx);
    return ret;
}

//void duk_push_thread_stash(duk_context *ctx, duk_context *target_ctx) ;
void aperl_duk_push_thread_stash(duk_context *ctx, duk_context *target_ctx)  {
    duk_push_thread_stash (ctx, target_ctx);
}

//void duk_push_true(duk_context *ctx);
void aperl_duk_push_true(duk_context *ctx) {
    duk_push_true(ctx);
}

//void duk_push_uint(duk_context *ctx, duk_uint_t val);
void aperl_duk_push_uint(duk_context *ctx, duk_uint_t val) {
    duk_push_uint(ctx, val);
}

//void duk_push_undefined(duk_context *ctx);
void aperl_duk_push_undefined(duk_context *ctx) {
    duk_push_undefined(ctx);
}

//const char *duk_push_vsprintf(duk_context *ctx, const char *fmt, va_list ap);
const char *aperl_duk_push_vsprintf(duk_context *ctx, const char *fmt, va_list ap) {
    const char *ret = duk_push_vsprintf(ctx, fmt, ap);
    return ret;
}

//void duk_put_function_list(duk_context *ctx, duk_idx_t obj_index, const duk_function_list_entry *funcs);
void aperl_duk_put_function_list(duk_context *ctx, duk_idx_t obj_index, const duk_function_list_entry *funcs) {
    duk_put_function_list(ctx, obj_index, funcs);
}

//duk_bool_t duk_put_global_string(duk_context *ctx, const char *key);
duk_bool_t aperl_duk_put_global_string(duk_context *ctx, const char *key) {
    duk_bool_t ret = duk_put_global_string(ctx, key);
    return ret;
}

//void duk_put_number_list(duk_context *ctx, duk_idx_t obj_index, const duk_number_list_entry *numbers);
void aperl_duk_put_number_list(duk_context *ctx, duk_idx_t obj_index, const duk_number_list_entry *numbers) {
    duk_put_number_list(ctx, obj_index, numbers);
}

//duk_bool_t duk_put_prop(duk_context *ctx, duk_idx_t obj_index);
duk_bool_t aperl_duk_put_prop(duk_context *ctx, duk_idx_t obj_index) {
    duk_bool_t ret = duk_put_prop(ctx, obj_index);
    return ret;
}

//duk_bool_t duk_put_prop_index(duk_context *ctx, duk_idx_t obj_index, duk_uarridx_t arr_index);
duk_bool_t aperl_duk_put_prop_index(duk_context *ctx, duk_idx_t obj_index, duk_uarridx_t arr_index) {
    duk_bool_t ret = duk_put_prop_index(ctx, obj_index, arr_index);
    return ret;
}

//duk_bool_t duk_put_prop_string(duk_context *ctx, duk_idx_t obj_index, const char *key);
duk_bool_t aperl_duk_put_prop_string(duk_context *ctx, duk_idx_t obj_index, const char *key) {
    duk_bool_t ret = duk_put_prop_string(ctx, obj_index, key);
    return ret;
}

//void *duk_realloc(duk_context *ctx, void *ptr, duk_size_t size);
void *aperl_duk_realloc(duk_context *ctx, void *ptr, duk_size_t size) {
    void *ret = duk_realloc(ctx, ptr, size);
    return ret;
}

//void *duk_realloc_raw(duk_context *ctx, void *ptr, duk_size_t size);
void *aperl_duk_realloc_raw(duk_context *ctx, void *ptr, duk_size_t size) {
    void *ret = duk_realloc_raw(ctx, ptr, size);
    return ret;
}

//void duk_remove(duk_context *ctx, duk_idx_t index);
void aperl_duk_remove(duk_context *ctx, duk_idx_t index) {
    duk_remove(ctx, index);
}

//void duk_replace(duk_context *ctx, duk_idx_t to_index);
void aperl_duk_replace(duk_context *ctx, duk_idx_t to_index) {
    duk_replace(ctx, to_index);
}

//duk_bool_t duk_require_boolean(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_require_boolean(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_require_boolean(ctx, index);
    return ret;
}

//void *duk_require_buffer(duk_context *ctx, duk_idx_t index, duk_size_t *out_size);
void *aperl_duk_require_buffer(duk_context *ctx, duk_idx_t index, SV *out_len) {
    duk_size_t sz;
    void *ret = duk_require_buffer(ctx, index, &sz);
    sv_setnv(out_len, sz);
    return ret;
}

//duk_c_function duk_require_c_function(duk_context *ctx, duk_idx_t index);
duk_c_function aperl_duk_require_c_function(duk_context *ctx, duk_idx_t index) {
    duk_c_function ret = duk_require_c_function(ctx, index);
    return ret;
}

//duk_context *duk_require_context(duk_context *ctx, duk_idx_t index);
duk_context *aperl_duk_require_context(duk_context *ctx, duk_idx_t index) {
    duk_context *ret = duk_require_context(ctx, index);
    return ret;
}

//void *duk_require_heapptr(duk_context *ctx, duk_idx_t index);
void *aperl_duk_require_heapptr(duk_context *ctx, duk_idx_t index) {
    void *ret = duk_require_heapptr(ctx, index);
    return ret;
}

//duk_int_t duk_require_int(duk_context *ctx, duk_idx_t index);
duk_int_t aperl_duk_require_int(duk_context *ctx, duk_idx_t index) {
    duk_int_t ret = duk_require_int(ctx, index);
    return ret;
}

//const char *duk_require_lstring(duk_context *ctx, duk_idx_t index, duk_size_t *out_len);
const char *aperl_duk_require_lstring(duk_context *ctx, duk_idx_t index, SV *out_len) {
    duk_size_t sz;
    const char *ret = duk_require_lstring(ctx, index, &sz);
    sv_setnv(out_len, sz);
    return ret;
}

//duk_idx_t duk_require_normalize_index(duk_context *ctx, duk_idx_t index);
duk_idx_t aperl_duk_require_normalize_index(duk_context *ctx, duk_idx_t index) {
    duk_idx_t ret = duk_require_normalize_index(ctx, index);
    return ret;
}

//void duk_require_null(duk_context *ctx, duk_idx_t index);
void aperl_duk_require_null(duk_context *ctx, duk_idx_t index) {
    duk_require_null(ctx, index);
}

//duk_double_t duk_require_number(duk_context *ctx, duk_idx_t index);
duk_double_t aperl_duk_require_number(duk_context *ctx, duk_idx_t index) {
    duk_double_t ret = duk_require_number(ctx, index);
    return ret;
}

//void duk_require_object_coercible(duk_context *ctx, duk_idx_t index);
void aperl_duk_require_object_coercible(duk_context *ctx, duk_idx_t index) {
    duk_require_object_coercible(ctx, index);
}

//void *duk_require_pointer(duk_context *ctx, duk_idx_t index);
void *aperl_duk_require_pointer(duk_context *ctx, duk_idx_t index) {
    void *ret = duk_require_pointer(ctx, index);
    return ret;
}

//void duk_require_stack(duk_context *ctx, duk_idx_t extra);
void aperl_duk_require_stack(duk_context *ctx, duk_idx_t extra) {
    duk_require_stack(ctx, extra);
}

//void duk_require_stack_top(duk_context *ctx, duk_idx_t top);
void aperl_duk_require_stack_top(duk_context *ctx, duk_idx_t top) {
    duk_require_stack_top(ctx, top);
}

//const char *duk_require_string(duk_context *ctx, duk_idx_t index);
const char *aperl_duk_require_string(duk_context *ctx, duk_idx_t index) {
    const char *ret = duk_require_string(ctx, index);
    return ret;
}

//duk_idx_t duk_require_top_index(duk_context *ctx);
duk_idx_t aperl_duk_require_top_index(duk_context *ctx) {
    duk_idx_t ret = duk_require_top_index(ctx);
    return ret;
}

//void duk_require_type_mask(duk_context *ctx, duk_idx_t index, duk_uint_t mask);
void aperl_duk_require_type_mask(duk_context *ctx, duk_idx_t index, duk_uint_t mask) {
    duk_require_type_mask(ctx, index, mask);
}

//duk_uint_t duk_require_uint(duk_context *ctx, duk_idx_t index);
duk_uint_t aperl_duk_require_uint(duk_context *ctx, duk_idx_t index) {
    duk_uint_t ret = duk_require_uint(ctx, index);
    return ret;
}

//void duk_require_undefined(duk_context *ctx, duk_idx_t index);
void aperl_duk_require_undefined(duk_context *ctx, duk_idx_t index) {
    duk_require_undefined(ctx, index);
}

//void duk_require_valid_index(duk_context *ctx, duk_idx_t index);
void aperl_duk_require_valid_index(duk_context *ctx, duk_idx_t index) {
    duk_require_valid_index(ctx, index);
}

//void *duk_resize_buffer(duk_context *ctx, duk_idx_t index, duk_size_t new_size);
void *aperl_duk_resize_buffer(duk_context *ctx, duk_idx_t index, duk_size_t new_size) {
    void *ret = duk_resize_buffer(ctx, index, new_size);
    return ret;
}

//duk_int_t duk_safe_call(duk_context *ctx, duk_safe_call_function func, duk_idx_t nargs, duk_idx_t nrets);
duk_int_t aperl_duk_safe_call(duk_context *ctx, duk_safe_call_function func, duk_idx_t nargs, duk_idx_t nrets) {
    duk_int_t ret = duk_safe_call(ctx, func, nargs, nrets);
    return ret;
}

//const char *duk_safe_to_lstring(duk_context *ctx, duk_idx_t index, duk_size_t *out_len);
const char *aperl_duk_safe_to_lstring(duk_context *ctx, duk_idx_t index, SV *out_len) {
    duk_size_t sz;
    const char *ret = duk_safe_to_lstring(ctx, index, &sz);
    sv_setnv(out_len, sz);
    return ret;
}

//const char *duk_safe_to_string(duk_context *ctx, duk_idx_t index);
const char *aperl_duk_safe_to_string(duk_context *ctx, duk_idx_t index) {
    const char *ret = duk_safe_to_string(ctx, index);
    return ret;
}

//void duk_set_finalizer(duk_context *ctx, duk_idx_t index);
void aperl_duk_set_finalizer(duk_context *ctx, duk_idx_t index) {
    duk_set_finalizer(ctx, index);
}

//void duk_set_global_object(duk_context *ctx);
void aperl_duk_set_global_object(duk_context *ctx) {
    duk_set_global_object(ctx);
}

//void duk_set_magic(duk_context *ctx, duk_idx_t index, duk_int_t magic);
void aperl_duk_set_magic(duk_context *ctx, duk_idx_t index, duk_int_t magic) {
    duk_set_magic(ctx, index, magic);
}

//void duk_set_prototype(duk_context *ctx, duk_idx_t index);
void aperl_duk_set_prototype(duk_context *ctx, duk_idx_t index) {
    duk_set_prototype(ctx, index);
}

//void duk_set_top(duk_context *ctx, duk_idx_t index);
void aperl_duk_set_top(duk_context *ctx, duk_idx_t index) {
    duk_set_top(ctx, index);
}

//duk_bool_t duk_strict_equals(duk_context *ctx, duk_idx_t index1, duk_idx_t index2);
duk_bool_t aperl_duk_strict_equals(duk_context *ctx, duk_idx_t index1, duk_idx_t index2) {
    duk_bool_t ret = duk_strict_equals(ctx, index1, index2);
    return ret;
}

//void duk_substring(duk_context *ctx, duk_idx_t index, duk_size_t start_char_offset, duk_size_t end_char_offset);
void aperl_duk_substring(duk_context *ctx, duk_idx_t index, duk_size_t start_char_offset, duk_size_t end_char_offset) {
    duk_substring(ctx, index, start_char_offset, end_char_offset);
}

//void duk_swap(duk_context *ctx, duk_idx_t index1, duk_idx_t index2);
void aperl_duk_swap(duk_context *ctx, duk_idx_t index1, duk_idx_t index2) {
    duk_swap(ctx, index1, index2);
}

//void duk_swap_top(duk_context *ctx, duk_idx_t index);
void aperl_duk_swap_top(duk_context *ctx, duk_idx_t index) {
    duk_swap_top(ctx, index);
}

//void duk_throw(duk_context *ctx);
void aperl_duk_throw(duk_context *ctx) {
    duk_throw(ctx);
}

//duk_bool_t duk_to_boolean(duk_context *ctx, duk_idx_t index);
duk_bool_t aperl_duk_to_boolean(duk_context *ctx, duk_idx_t index) {
    duk_bool_t ret = duk_to_boolean(ctx, index);
    return ret;
}

//void *duk_to_buffer(duk_context *ctx, duk_idx_t index, duk_size_t *out_size);
void *aperl_duk_to_buffer(duk_context *ctx, duk_idx_t index, SV *out_len) {
    duk_size_t sz;
    void *ret = duk_to_buffer(ctx, index, &sz);
    sv_setnv(out_len, sz);
    return ret;
}

//void duk_to_defaultvalue(duk_context *ctx, duk_idx_t index, duk_int_t hint);
void aperl_duk_to_defaultvalue(duk_context *ctx, duk_idx_t index, duk_int_t hint) {
    duk_to_defaultvalue(ctx, index, hint);
}

//void *duk_to_dynamic_buffer(duk_context *ctx, duk_idx_t index, duk_size_t *out_size);
void *aperl_duk_to_dynamic_buffer(duk_context *ctx, duk_idx_t index, SV *out_len) {
    duk_size_t sz;
    void *ret = duk_to_dynamic_buffer(ctx, index, &sz);
    sv_setnv(out_len, sz);
    return ret;
}

//void *duk_to_fixed_buffer(duk_context *ctx, duk_idx_t index, duk_size_t *out_size);
void *aperl_duk_to_fixed_buffer(duk_context *ctx, duk_idx_t index, SV *out_len) {
    duk_size_t sz;
    void *ret = duk_to_fixed_buffer(ctx, index, &sz);
    sv_setnv(out_len, sz);
    return ret;
}

//duk_int_t duk_to_int(duk_context *ctx, duk_int_t index);
duk_int_t aperl_duk_to_int(duk_context *ctx, duk_int_t index) {
    duk_int_t ret = duk_to_int(ctx, index);
    return ret;
}

//duk_int32_t duk_to_int32(duk_context *ctx, duk_idx_t index);
duk_int32_t aperl_duk_to_int32(duk_context *ctx, duk_idx_t index) {
    duk_int32_t ret = duk_to_int32(ctx, index);
    return ret;
}

//const char *duk_to_lstring(duk_context *ctx, duk_idx_t index, duk_size_t *out_len);
const char *aperl_duk_to_lstring(duk_context *ctx, duk_idx_t index, SV *out_len) {
    duk_size_t sz;
    const char *ret = duk_to_lstring(ctx, index, &sz);
    sv_setnv(out_len, sz);
    return ret;
}

//void duk_to_null(duk_context *ctx, duk_idx_t index);
void aperl_duk_to_null(duk_context *ctx, duk_idx_t index) {
    duk_to_null(ctx, index);
}

//duk_double_t duk_to_number(duk_context *ctx, duk_idx_t index);
duk_double_t aperl_duk_to_number(duk_context *ctx, duk_idx_t index) {
    duk_double_t ret = duk_to_number(ctx, index);
    return ret;
}

//void duk_to_object(duk_context *ctx, duk_idx_t index);
void aperl_duk_to_object(duk_context *ctx, duk_idx_t index) {
    duk_to_object(ctx, index);
}

//void *duk_to_pointer(duk_context *ctx, duk_idx_t index);
void *aperl_duk_to_pointer(duk_context *ctx, duk_idx_t index) {
    void *ret = duk_to_pointer(ctx, index);
    return ret;
}

//void duk_to_primitive(duk_context *ctx, duk_idx_t index, duk_int_t hint);
void aperl_duk_to_primitive(duk_context *ctx, duk_idx_t index, duk_int_t hint) {
    duk_to_primitive(ctx, index, hint);
}

//const char *duk_to_string(duk_context *ctx, duk_idx_t index);
const char *aperl_duk_to_string(duk_context *ctx, duk_idx_t index) {
    const char *ret = duk_to_string(ctx, index);
    return ret;
}

//duk_uint_t duk_to_uint(duk_context *ctx, duk_idx_t index);
duk_uint_t aperl_duk_to_uint(duk_context *ctx, duk_idx_t index) {
    duk_uint_t ret = duk_to_uint(ctx, index);
    return ret;
}

//duk_uint16_t duk_to_uint16(duk_context *ctx, duk_idx_t index);
duk_uint16_t aperl_duk_to_uint16(duk_context *ctx, duk_idx_t index) {
    duk_uint16_t ret = duk_to_uint16(ctx, index);
    return ret;
}

//duk_uint32_t duk_to_uint32(duk_context *ctx, duk_idx_t index);
duk_uint32_t aperl_duk_to_uint32(duk_context *ctx, duk_idx_t index) {
    duk_uint32_t ret = duk_to_uint32(ctx, index);
    return ret;
}

//void duk_to_undefined(duk_context *ctx, duk_idx_t index);
void aperl_duk_to_undefined(duk_context *ctx, duk_idx_t index) {
    duk_to_undefined(ctx, index);
}

//void duk_trim(duk_context *ctx, duk_idx_t index);
void aperl_duk_trim(duk_context *ctx, duk_idx_t index) {
    duk_trim(ctx, index);
}
