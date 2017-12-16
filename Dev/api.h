DUK_EXTERNAL_DECL void *duk_alloc(duk_context *ctx, duk_size_t size);
DUK_EXTERNAL_DECL void *duk_alloc_raw(duk_context *ctx, duk_size_t size);
DUK_EXTERNAL_DECL void duk_base64_decode(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL const char *duk_base64_encode(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL const char *duk_buffer_to_string(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void duk_call(duk_context *ctx, duk_idx_t nargs);
DUK_EXTERNAL_DECL void duk_call_method(duk_context *ctx, duk_idx_t nargs);
DUK_EXTERNAL_DECL void duk_call_prop(duk_context *ctx, duk_idx_t obj_idx, duk_idx_t nargs);
DUK_EXTERNAL_DECL duk_codepoint_t duk_char_code_at(duk_context *ctx, duk_idx_t idx, duk_size_t char_offset);
DUK_EXTERNAL_DECL duk_bool_t duk_check_stack(duk_context *ctx, duk_idx_t extra);
DUK_EXTERNAL_DECL duk_bool_t duk_check_stack_top(duk_context *ctx, duk_idx_t top);
DUK_EXTERNAL_DECL duk_bool_t duk_check_type(duk_context *ctx, duk_idx_t idx, duk_int_t type);
DUK_EXTERNAL_DECL duk_bool_t duk_check_type_mask(duk_context *ctx, duk_idx_t idx, duk_uint_t mask);
DUK_EXTERNAL_DECL void duk_compact(duk_context *ctx, duk_idx_t obj_idx);
DUK_EXTERNAL_DECL void duk_compile(duk_context *ctx, duk_uint_t flags);
DUK_EXTERNAL_DECL void duk_compile_lstring(duk_context *ctx, duk_uint_t flags, const char *src, duk_size_t len);
DUK_EXTERNAL_DECL void duk_compile_lstring_filename(duk_context *ctx, duk_uint_t flags, const char *src, duk_size_t len);
DUK_EXTERNAL_DECL void duk_compile_string(duk_context *ctx, duk_uint_t flags, const char *src);
DUK_EXTERNAL_DECL void duk_compile_string_filename(duk_context *ctx, duk_uint_t flags, const char *src);
DUK_EXTERNAL_DECL duk_double_t duk_components_to_time(duk_context *ctx, duk_time_components *comp);
DUK_EXTERNAL_DECL void duk_concat(duk_context *ctx, duk_idx_t count);
DUK_EXTERNAL_DECL void duk_config_buffer(duk_context *ctx, duk_idx_t idx, void *ptr, duk_size_t len);
DUK_EXTERNAL_DECL void duk_copy(duk_context *ctx, duk_idx_t from_idx, duk_idx_t to_idx);
DUK_EXTERNAL_DECL duk_context *duk_create_heap(duk_alloc_function alloc_func,                             duk_realloc_function realloc_func,                             duk_free_function free_func,                             void *heap_udata,                             duk_fatal_function fatal_handler);
DUK_EXTERNAL_DECL duk_context *duk_create_heap_default(void);
DUK_EXTERNAL_DECL void duk_decode_string(duk_context *ctx, duk_idx_t idx, duk_decode_char_function callback, void *udata);
DUK_EXTERNAL_DECL void duk_def_prop(duk_context *ctx, duk_idx_t obj_idx, duk_uint_t flags);
DUK_EXTERNAL_DECL duk_bool_t duk_del_prop(duk_context *ctx, duk_idx_t obj_idx);
DUK_EXTERNAL_DECL duk_bool_t duk_del_prop_heapptr(duk_context *ctx, duk_idx_t obj_idx, void *ptr);
DUK_EXTERNAL_DECL duk_bool_t duk_del_prop_index(duk_context *ctx, duk_idx_t obj_idx, duk_uarridx_t arr_idx);
DUK_EXTERNAL_DECL duk_bool_t duk_del_prop_lstring(duk_context *ctx, duk_idx_t obj_idx, const char *key, duk_size_t key_len);
DUK_EXTERNAL_DECL duk_bool_t duk_del_prop_string(duk_context *ctx, duk_idx_t obj_idx, const char *key);
DUK_EXTERNAL_DECL void duk_destroy_heap(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_dump_function(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_dup(duk_context *ctx, duk_idx_t from_idx);
DUK_EXTERNAL_DECL void duk_dup_top(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_enum(duk_context *ctx, duk_idx_t obj_idx, duk_uint_t enum_flags);
DUK_EXTERNAL_DECL duk_bool_t duk_equals(duk_context *ctx, duk_idx_t idx1, duk_idx_t idx2);
DUK_EXTERNAL_DECL duk_ret_t duk_error(duk_context *ctx, duk_errcode_t err_code, const char *fmt, ...);
DUK_EXTERNAL_DECL duk_ret_t duk_error_va(duk_context *ctx, duk_errcode_t err_code, const char *fmt, va_list ap);
DUK_EXTERNAL_DECL void duk_eval(duk_context *ctx);
DUK_EXTERNAL_DECL duk_ret_t duk_eval_error(duk_context *ctx, const char *fmt, ...);
DUK_EXTERNAL_DECL duk_ret_t duk_eval_error_va(duk_context *ctx, const char *fmt, va_list ap);
DUK_EXTERNAL_DECL void duk_eval_lstring(duk_context *ctx, const char *src, duk_size_t len);
DUK_EXTERNAL_DECL void duk_eval_lstring_noresult(duk_context *ctx, const char *src, duk_size_t len);
DUK_EXTERNAL_DECL void duk_eval_noresult(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_eval_string(duk_context *ctx, const char *src);
DUK_EXTERNAL_DECL void duk_eval_string_noresult(duk_context *ctx, const char *src);
DUK_EXTERNAL_DECL duk_ret_t duk_fatal(duk_context *ctx, const char *err_msg);
DUK_EXTERNAL_DECL void duk_free(duk_context *ctx, void *ptr);
DUK_EXTERNAL_DECL void duk_free_raw(duk_context *ctx, void *ptr);
DUK_EXTERNAL_DECL void duk_freeze(duk_context *ctx, duk_idx_t obj_idx);
DUK_EXTERNAL_DECL void duk_gc(duk_context *ctx, duk_uint_t flags);
DUK_EXTERNAL_DECL duk_ret_t duk_generic_error(duk_context *ctx, const char *fmt, ...);
DUK_EXTERNAL_DECL duk_ret_t duk_generic_error_va(duk_context *ctx, const char *fmt, va_list ap);
DUK_EXTERNAL_DECL duk_bool_t duk_get_boolean(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_get_boolean_default(duk_context *ctx, duk_idx_t idx, duk_bool_t def_value);
DUK_EXTERNAL_DECL void *duk_get_buffer(duk_context *ctx, duk_idx_t idx, duk_size_t *out_size);
DUK_EXTERNAL_DECL void *duk_get_buffer_data(duk_context *ctx, duk_idx_t idx, duk_size_t *out_size);
DUK_EXTERNAL_DECL void *duk_get_buffer_data_default(duk_context *ctx, duk_idx_t idx, duk_size_t *out_size, void *def_ptr, duk_size_t def_len);
DUK_EXTERNAL_DECL void *duk_get_buffer_default(duk_context *ctx, duk_idx_t idx, duk_size_t *out_size, void *def_ptr, duk_size_t def_len);
DUK_EXTERNAL_DECL duk_c_function duk_get_c_function(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_c_function duk_get_c_function_default(duk_context *ctx, duk_idx_t idx, duk_c_function def_value);
DUK_EXTERNAL_DECL duk_context *duk_get_context(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_context *duk_get_context_default(duk_context *ctx, duk_idx_t idx, duk_context *def_value);
DUK_EXTERNAL_DECL duk_int_t duk_get_current_magic(duk_context *ctx);
DUK_EXTERNAL_DECL duk_errcode_t duk_get_error_code(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void duk_get_finalizer(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_get_global_lstring(duk_context *ctx, const char *key, duk_size_t key_len);
DUK_EXTERNAL_DECL duk_bool_t duk_get_global_string(duk_context *ctx, const char *key);
DUK_EXTERNAL_DECL void *duk_get_heapptr(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void *duk_get_heapptr_default(duk_context *ctx, duk_idx_t idx, void *def_value);
DUK_EXTERNAL_DECL duk_int_t duk_get_int(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_int_t duk_get_int_default(duk_context *ctx, duk_idx_t idx, duk_int_t def_value);
DUK_EXTERNAL_DECL duk_size_t duk_get_length(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL const char *duk_get_lstring(duk_context *ctx, duk_idx_t idx, duk_size_t *out_len);
DUK_EXTERNAL_DECL const char *duk_get_lstring_default(duk_context *ctx, duk_idx_t idx, duk_size_t *out_len, const char *def_ptr, duk_size_t def_len);
DUK_EXTERNAL_DECL duk_int_t duk_get_magic(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void duk_get_memory_functions(duk_context *ctx, duk_memory_functions *out_funcs);
DUK_EXTERNAL_DECL duk_double_t duk_get_now(duk_context *ctx);
DUK_EXTERNAL_DECL duk_double_t duk_get_number(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_double_t duk_get_number_default(duk_context *ctx, duk_idx_t idx, duk_double_t def_value);
DUK_EXTERNAL_DECL void *duk_get_pointer(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void *duk_get_pointer_default(duk_context *ctx, duk_idx_t idx, void *def_value);
DUK_EXTERNAL_DECL duk_bool_t duk_get_prop(duk_context *ctx, duk_idx_t obj_idx);
DUK_EXTERNAL_DECL void duk_get_prop_desc(duk_context *ctx, duk_idx_t obj_idx, duk_uint_t flags);
DUK_EXTERNAL_DECL duk_bool_t duk_get_prop_heapptr(duk_context *ctx, duk_idx_t obj_idx, void *ptr);
DUK_EXTERNAL_DECL duk_bool_t duk_get_prop_index(duk_context *ctx, duk_idx_t obj_idx, duk_uarridx_t arr_idx);
DUK_EXTERNAL_DECL duk_bool_t duk_get_prop_lstring(duk_context *ctx, duk_idx_t obj_idx, const char *key, duk_size_t key_len);
DUK_EXTERNAL_DECL duk_bool_t duk_get_prop_string(duk_context *ctx, duk_idx_t obj_idx, const char *key);
DUK_EXTERNAL_DECL void duk_get_prototype(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL const char *duk_get_string(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL const char *duk_get_string_default(duk_context *ctx, duk_idx_t idx, const char *def_value);
DUK_EXTERNAL_DECL duk_idx_t duk_get_top(duk_context *ctx);
DUK_EXTERNAL_DECL duk_idx_t duk_get_top_index(duk_context *ctx);
DUK_EXTERNAL_DECL duk_int_t duk_get_type(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_uint_t duk_get_type_mask(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_uint_t duk_get_uint(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_uint_t duk_get_uint_default(duk_context *ctx, duk_idx_t idx, duk_uint_t def_value);
DUK_EXTERNAL_DECL duk_bool_t duk_has_prop(duk_context *ctx, duk_idx_t obj_idx);
DUK_EXTERNAL_DECL duk_bool_t duk_has_prop_heapptr(duk_context *ctx, duk_idx_t obj_idx, void *ptr);
DUK_EXTERNAL_DECL duk_bool_t duk_has_prop_index(duk_context *ctx, duk_idx_t obj_idx, duk_uarridx_t arr_idx);
DUK_EXTERNAL_DECL duk_bool_t duk_has_prop_lstring(duk_context *ctx, duk_idx_t obj_idx, const char *key, duk_size_t key_len);
DUK_EXTERNAL_DECL duk_bool_t duk_has_prop_string(duk_context *ctx, duk_idx_t obj_idx, const char *key);
DUK_EXTERNAL_DECL void duk_hex_decode(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL const char *duk_hex_encode(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void duk_insert(duk_context *ctx, duk_idx_t to_idx);
DUK_EXTERNAL_DECL void duk_inspect_callstack_entry(duk_context *ctx, duk_int_t level);
DUK_EXTERNAL_DECL void duk_inspect_value(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_instanceof(duk_context *ctx, duk_idx_t idx1, duk_idx_t idx2);
DUK_EXTERNAL_DECL duk_bool_t duk_is_array(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_boolean(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_bound_function(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_buffer(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_buffer_data(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_c_function(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_callable(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_constructable(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_constructor_call(duk_context *ctx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_dynamic_buffer(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_ecmascript_function(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_error(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_eval_error(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_fixed_buffer(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_function(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_lightfunc(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_nan(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_null(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_null_or_undefined(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_number(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_object(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_object_coercible(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_pointer(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_primitive(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_range_error(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_reference_error(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_strict_call(duk_context *ctx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_string(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_symbol(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_syntax_error(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_thread(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_type_error(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_undefined(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_uri_error(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_is_valid_index(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void duk_join(duk_context *ctx, duk_idx_t count);
DUK_EXTERNAL_DECL void duk_json_decode(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL const char *duk_json_encode(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void duk_load_function(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_map_string(duk_context *ctx, duk_idx_t idx, duk_map_char_function callback, void *udata);
DUK_EXTERNAL_DECL void duk_new(duk_context *ctx, duk_idx_t nargs);
DUK_EXTERNAL_DECL duk_bool_t duk_next(duk_context *ctx, duk_idx_t enum_idx, duk_bool_t get_value);
DUK_EXTERNAL_DECL duk_idx_t duk_normalize_index(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_opt_boolean(duk_context *ctx, duk_idx_t idx, duk_bool_t def_value);
DUK_EXTERNAL_DECL void *duk_opt_buffer(duk_context *ctx, duk_idx_t idx, duk_size_t *out_size, void *def_ptr, duk_size_t def_len);
DUK_EXTERNAL_DECL void *duk_opt_buffer_data(duk_context *ctx, duk_idx_t idx, duk_size_t *out_size, void *def_ptr, duk_size_t def_len);
DUK_EXTERNAL_DECL duk_c_function duk_opt_c_function(duk_context *ctx, duk_idx_t idx, duk_c_function def_value);
DUK_EXTERNAL_DECL duk_context *duk_opt_context(duk_context *ctx, duk_idx_t idx, duk_context *def_value);
DUK_EXTERNAL_DECL void *duk_opt_heapptr(duk_context *ctx, duk_idx_t idx, void *def_value);
DUK_EXTERNAL_DECL duk_int_t duk_opt_int(duk_context *ctx, duk_idx_t idx, duk_int_t def_value);
DUK_EXTERNAL_DECL const char *duk_opt_lstring(duk_context *ctx, duk_idx_t idx, duk_size_t *out_len, const char *def_ptr, duk_size_t def_len);
DUK_EXTERNAL_DECL duk_double_t duk_opt_number(duk_context *ctx, duk_idx_t idx, duk_double_t def_value);
DUK_EXTERNAL_DECL void *duk_opt_pointer(duk_context *ctx, duk_idx_t idx, void *def_value);
DUK_EXTERNAL_DECL const char *duk_opt_string(duk_context *ctx, duk_idx_t idx, const char *def_ptr);
DUK_EXTERNAL_DECL duk_uint_t duk_opt_uint(duk_context *ctx, duk_idx_t idx, duk_uint_t def_value);
DUK_EXTERNAL_DECL duk_int_t duk_pcall(duk_context *ctx, duk_idx_t nargs);
DUK_EXTERNAL_DECL duk_int_t duk_pcall_method(duk_context *ctx, duk_idx_t nargs);
DUK_EXTERNAL_DECL duk_int_t duk_pcall_prop(duk_context *ctx, duk_idx_t obj_idx, duk_idx_t nargs);
DUK_EXTERNAL_DECL duk_int_t duk_pcompile(duk_context *ctx, duk_uint_t flags);
DUK_EXTERNAL_DECL duk_int_t duk_pcompile_lstring(duk_context *ctx, duk_uint_t flags, const char *src, duk_size_t len);
DUK_EXTERNAL_DECL duk_int_t duk_pcompile_lstring_filename(duk_context *ctx, duk_uint_t flags, const char *src, duk_size_t len);
DUK_EXTERNAL_DECL duk_int_t duk_pcompile_string(duk_context *ctx, duk_uint_t flags, const char *src);
DUK_EXTERNAL_DECL duk_int_t duk_pcompile_string_filename(duk_context *ctx, duk_uint_t flags, const char *src);
DUK_EXTERNAL_DECL duk_int_t duk_peval(duk_context *ctx);
DUK_EXTERNAL_DECL duk_int_t duk_peval_lstring(duk_context *ctx, const char *src, duk_size_t len);
DUK_EXTERNAL_DECL duk_int_t duk_peval_lstring_noresult(duk_context *ctx, const char *src, duk_size_t len);
DUK_EXTERNAL_DECL duk_int_t duk_peval_noresult(duk_context *ctx);
DUK_EXTERNAL_DECL duk_int_t duk_peval_string(duk_context *ctx, const char *src);
DUK_EXTERNAL_DECL duk_int_t duk_peval_string_noresult(duk_context *ctx, const char *src);
DUK_EXTERNAL_DECL duk_ret_t duk_pnew(duk_context *ctx, duk_idx_t nargs);
DUK_EXTERNAL_DECL void duk_pop(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_pop_2(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_pop_3(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_pop_n(duk_context *ctx, duk_idx_t count);
DUK_EXTERNAL_DECL duk_idx_t duk_push_array(duk_context *ctx);
DUK_EXTERNAL_DECL duk_idx_t duk_push_bare_object(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_push_boolean(duk_context *ctx, duk_bool_t val);
DUK_EXTERNAL_DECL void *duk_push_buffer(duk_context *ctx, duk_size_t size, duk_bool_t dynamic);
DUK_EXTERNAL_DECL void duk_push_buffer_object(duk_context *ctx, duk_idx_t idx_buffer, duk_size_t byte_offset, duk_size_t byte_length, duk_uint_t flags);
DUK_EXTERNAL_DECL duk_idx_t duk_push_c_function(duk_context *ctx, duk_c_function func, duk_idx_t nargs);
DUK_EXTERNAL_DECL duk_idx_t duk_push_c_lightfunc(duk_context *ctx, duk_c_function func, duk_idx_t nargs, duk_idx_t length, duk_int_t magic);
DUK_EXTERNAL_DECL void duk_push_context_dump(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_push_current_function(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_push_current_thread(duk_context *ctx);
DUK_EXTERNAL_DECL void *duk_push_dynamic_buffer(duk_context *ctx, duk_size_t size);
DUK_EXTERNAL_DECL duk_idx_t duk_push_error_object(duk_context *ctx, duk_errcode_t err_code, const char *fmt, ...);
DUK_EXTERNAL_DECL duk_idx_t duk_push_error_object_va(duk_context *ctx, duk_errcode_t err_code, const char *fmt, va_list ap);
DUK_EXTERNAL_DECL void duk_push_external_buffer(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_push_false(duk_context *ctx);
DUK_EXTERNAL_DECL void *duk_push_fixed_buffer(duk_context *ctx, duk_size_t size);
DUK_EXTERNAL_DECL void duk_push_global_object(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_push_global_stash(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_push_heap_stash(duk_context *ctx);
DUK_EXTERNAL_DECL duk_idx_t duk_push_heapptr(duk_context *ctx, void *ptr);
DUK_EXTERNAL_DECL void duk_push_int(duk_context *ctx, duk_int_t val);
DUK_EXTERNAL_DECL const char *duk_push_lstring(duk_context *ctx, const char *str, duk_size_t len);
DUK_EXTERNAL_DECL void duk_push_nan(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_push_null(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_push_number(duk_context *ctx, duk_double_t val);
DUK_EXTERNAL_DECL duk_idx_t duk_push_object(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_push_pointer(duk_context *ctx, void *p);
DUK_EXTERNAL_DECL duk_idx_t duk_push_proxy(duk_context *ctx, duk_uint_t proxy_flags);
DUK_EXTERNAL_DECL const char *duk_push_sprintf(duk_context *ctx, const char *fmt, ...);
DUK_EXTERNAL_DECL const char *duk_push_string(duk_context *ctx, const char *str);
DUK_EXTERNAL_DECL void duk_push_this(duk_context *ctx);
DUK_EXTERNAL_DECL duk_idx_t duk_push_thread(duk_context *ctx);
DUK_EXTERNAL_DECL duk_idx_t duk_push_thread_new_globalenv(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_push_thread_stash(duk_context *ctx, duk_context *target_ctx);
DUK_EXTERNAL_DECL void duk_push_true(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_push_uint(duk_context *ctx, duk_uint_t val);
DUK_EXTERNAL_DECL void duk_push_undefined(duk_context *ctx);
DUK_EXTERNAL_DECL const char *duk_push_vsprintf(duk_context *ctx, const char *fmt, va_list ap);
DUK_EXTERNAL_DECL void duk_put_function_list(duk_context *ctx, duk_idx_t obj_idx, const duk_function_list_entry *funcs);
DUK_EXTERNAL_DECL duk_bool_t duk_put_global_lstring(duk_context *ctx, const char *key, duk_size_t key_len);
DUK_EXTERNAL_DECL duk_bool_t duk_put_global_string(duk_context *ctx, const char *key);
DUK_EXTERNAL_DECL void duk_put_number_list(duk_context *ctx, duk_idx_t obj_idx, const duk_number_list_entry *numbers);
DUK_EXTERNAL_DECL duk_bool_t duk_put_prop(duk_context *ctx, duk_idx_t obj_idx);
DUK_EXTERNAL_DECL duk_bool_t duk_put_prop_heapptr(duk_context *ctx, duk_idx_t obj_idx, void *ptr);
DUK_EXTERNAL_DECL duk_bool_t duk_put_prop_index(duk_context *ctx, duk_idx_t obj_idx, duk_uarridx_t arr_idx);
DUK_EXTERNAL_DECL duk_bool_t duk_put_prop_lstring(duk_context *ctx, duk_idx_t obj_idx, const char *key, duk_size_t key_len);
DUK_EXTERNAL_DECL duk_bool_t duk_put_prop_string(duk_context *ctx, duk_idx_t obj_idx, const char *key);
DUK_EXTERNAL_DECL duk_ret_t duk_range_error(duk_context *ctx, const char *fmt, ...);
DUK_EXTERNAL_DECL duk_ret_t duk_range_error_va(duk_context *ctx, const char *fmt, va_list ap);
DUK_EXTERNAL_DECL void *duk_realloc(duk_context *ctx, void *ptr, duk_size_t size);
DUK_EXTERNAL_DECL void *duk_realloc_raw(duk_context *ctx, void *ptr, duk_size_t size);
DUK_EXTERNAL_DECL duk_ret_t duk_reference_error(duk_context *ctx, const char *fmt, ...);
DUK_EXTERNAL_DECL duk_ret_t duk_reference_error_va(duk_context *ctx, const char *fmt, va_list ap);
DUK_EXTERNAL_DECL void duk_remove(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void duk_replace(duk_context *ctx, duk_idx_t to_idx);
DUK_EXTERNAL_DECL duk_bool_t duk_require_boolean(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void *duk_require_buffer(duk_context *ctx, duk_idx_t idx, duk_size_t *out_size);
DUK_EXTERNAL_DECL void *duk_require_buffer_data(duk_context *ctx, duk_idx_t idx, duk_size_t *out_size);
DUK_EXTERNAL_DECL duk_c_function duk_require_c_function(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void duk_require_callable(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_context *duk_require_context(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void duk_require_function(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void *duk_require_heapptr(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_int_t duk_require_int(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL const char *duk_require_lstring(duk_context *ctx, duk_idx_t idx, duk_size_t *out_len);
DUK_EXTERNAL_DECL duk_idx_t duk_require_normalize_index(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void duk_require_null(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_double_t duk_require_number(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void duk_require_object(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void duk_require_object_coercible(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void *duk_require_pointer(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void duk_require_stack(duk_context *ctx, duk_idx_t extra);
DUK_EXTERNAL_DECL void duk_require_stack_top(duk_context *ctx, duk_idx_t top);
DUK_EXTERNAL_DECL const char *duk_require_string(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_idx_t duk_require_top_index(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_require_type_mask(duk_context *ctx, duk_idx_t idx, duk_uint_t mask);
DUK_EXTERNAL_DECL duk_uint_t duk_require_uint(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void duk_require_undefined(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void duk_require_valid_index(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void *duk_resize_buffer(duk_context *ctx, duk_idx_t idx, duk_size_t new_size);
DUK_EXTERNAL_DECL void duk_resume(duk_context *ctx, const duk_thread_state *state);
DUK_EXTERNAL_DECL duk_int_t duk_safe_call(duk_context *ctx, duk_safe_call_function func, void *udata, duk_idx_t nargs, duk_idx_t nrets);
DUK_EXTERNAL_DECL const char *duk_safe_to_lstring(duk_context *ctx, duk_idx_t idx, duk_size_t *out_len);
DUK_EXTERNAL_DECL const char *duk_safe_to_string(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_bool_t duk_samevalue(duk_context *ctx, duk_idx_t idx1, duk_idx_t idx2);
DUK_EXTERNAL_DECL void duk_seal(duk_context *ctx, duk_idx_t obj_idx);
DUK_EXTERNAL_DECL void duk_set_finalizer(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void duk_set_global_object(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_set_length(duk_context *ctx, duk_idx_t idx, duk_size_t len);
DUK_EXTERNAL_DECL void duk_set_magic(duk_context *ctx, duk_idx_t idx, duk_int_t magic);
DUK_EXTERNAL_DECL void duk_set_prototype(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void duk_set_top(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void *duk_steal_buffer(duk_context *ctx, duk_idx_t idx, duk_size_t *out_size);
DUK_EXTERNAL_DECL duk_bool_t duk_strict_equals(duk_context *ctx, duk_idx_t idx1, duk_idx_t idx2);
DUK_EXTERNAL_DECL void duk_substring(duk_context *ctx, duk_idx_t idx, duk_size_t start_char_offset, duk_size_t end_char_offset);
DUK_EXTERNAL_DECL void duk_suspend(duk_context *ctx, duk_thread_state *state);
DUK_EXTERNAL_DECL void duk_swap(duk_context *ctx, duk_idx_t idx1, duk_idx_t idx2);
DUK_EXTERNAL_DECL void duk_swap_top(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_ret_t duk_syntax_error(duk_context *ctx, const char *fmt, ...);
DUK_EXTERNAL_DECL duk_ret_t duk_syntax_error_va(duk_context *ctx, const char *fmt, va_list ap);
DUK_EXTERNAL_DECL duk_ret_t duk_throw(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_time_to_components(duk_context *ctx, duk_double_t time, duk_time_components *comp);
DUK_EXTERNAL_DECL duk_bool_t duk_to_boolean(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void *duk_to_buffer(duk_context *ctx, duk_idx_t idx, duk_size_t *out_size);
DUK_EXTERNAL_DECL void *duk_to_dynamic_buffer(duk_context *ctx, duk_idx_t idx, duk_size_t *out_size);
DUK_EXTERNAL_DECL void *duk_to_fixed_buffer(duk_context *ctx, duk_idx_t idx, duk_size_t *out_size);
DUK_EXTERNAL_DECL duk_int_t duk_to_int(duk_context *ctx, duk_int_t index);
DUK_EXTERNAL_DECL duk_int32_t duk_to_int32(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL const char *duk_to_lstring(duk_context *ctx, duk_idx_t idx, duk_size_t *out_len);
DUK_EXTERNAL_DECL void duk_to_null(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_double_t duk_to_number(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void duk_to_object(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void *duk_to_pointer(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void duk_to_primitive(duk_context *ctx, duk_idx_t idx, duk_int_t hint);
DUK_EXTERNAL_DECL const char *duk_to_string(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_uint_t duk_to_uint(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_uint16_t duk_to_uint16(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_uint32_t duk_to_uint32(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void duk_to_undefined(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL void duk_trim(duk_context *ctx, duk_idx_t idx);
DUK_EXTERNAL_DECL duk_ret_t duk_type_error(duk_context *ctx, const char *fmt, ...);
DUK_EXTERNAL_DECL duk_ret_t duk_type_error_va(duk_context *ctx, const char *fmt, va_list ap);
DUK_EXTERNAL_DECL duk_ret_t duk_uri_error(duk_context *ctx, const char *fmt, ...);
DUK_EXTERNAL_DECL duk_ret_t duk_uri_error_va(duk_context *ctx, const char *fmt, va_list ap);
DUK_EXTERNAL_DECL void duk_xcopy_top(duk_context *to_ctx, duk_context *from_ctx, duk_idx_t count);
DUK_EXTERNAL_DECL void duk_xmove_top(duk_context *to_ctx, duk_context *from_ctx, duk_idx_t count);
