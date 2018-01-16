#ifndef GM_ERROR_H
#define GM_ERROR_H

#include "gm_ast.h"
#include "gm_typecheck.h"

enum GM_ERRORS_AND_WARNINGS
{
    GM_ERROR_UNKNOWN, GM_ERROR_UNDEFINED, GM_ERROR_UNDEFINED_FIELD, GM_ERROR_UNDEFINED_FIELD_GRAPH,
    //GM_ERROR_MULTIPLE_TARGET,
    GM_ERROR_PROPERTY_ARGUMENT,
    GM_ERROR_INVALID_OUTPUT_TYPE,
    GM_ERROR_NONGRAPH_TARGET,
    GM_ERROR_NONNODE_TARGET,
    GM_ERROR_NONSET_TARGET,
    GM_ERROR_DUPLICATE,
    GM_ERROR_NONGRAPH_FIELD,
    GM_ERROR_OPERATOR_MISMATCH,
    GM_ERROR_OPERATOR_MISMATCH2,
    GM_ERROR_COMPARE_MISMATCH,
    GM_ERROR_KEY_MISSMATCH,
    GM_ERROR_READONLY,
    GM_ERROR_TARGET_MISMATCH,
    GM_ERROR_TARGET_MISMATCH2, // expr
    GM_ERROR_ASSIGN_TYPE_MISMATCH,
    GM_ERROR_WRONG_PROPERTY,
    //GM_ERROR_INVALID_ITERATOR,
    GM_ERROR_NEED_NODE_ITERATION,
    GM_ERROR_NEED_BFS_ITERATION,
    GM_ERROR_NEED_BOOLEAN,
    GM_ERROR_NEED_NUMERIC,
    GM_ERROR_UNBOUND_REDUCE,
    GM_ERROR_NEED_ITERATOR,
    GM_ERROR_DOUBLE_BOUND_ITOR,
    GM_ERROR_DOUBLE_BOUND_OP,
    //GM_ERROR_WRITE_TO_BOUND_ID,
    //GM_ERROR_WRITE_TO_BOUND_FIELD,
    GM_ERROR_GRAPH_REDUCE,
    GM_ERROR_TYPE_CONVERSION,
    GM_ERROR_TYPE_CONVERSION_BOOL_NUM,
    GM_ERROR_REQUIRE_NUMERIC_REDUCE,
    GM_ERROR_REQUIRE_BOOLEAN_REDUCE,
    GM_ERROR_NEED_PRIMITIVE,
    GM_ERROR_TARGET_GRAPH_MISMATCH,
    GM_ERROR_GROUP_REDUCTION,
    GM_ERROR_PAR_RETURN,

    GM_ERROR_NEED_ORDER,
    GM_ERROR_NESTED_BFS,
    GM_ERROR_INVALID_GROUP_DRIVER,
    GM_ERROR_INVALID_ITERATOR_FOR_RARROW,
    GM_ERROR_NEED_ITEM_ITERATION,

    GM_ERROR_DEFAULT_GRAPH_AMBIGUOUS,

    // Conflict erors
    GM_ERROR_READ_REDUCE_CONFLICT,
    GM_ERROR_WRITE_REDUCE_CONFLICT,
    GM_ERROR_WRITE_WRITE_CONFLICT,
    GM_ERROR_READ_WRITE_CONFLICT,
    GM_ERROR_READ_MUTATE_CONFLICT,
    GM_ERROR_WRITE_MUTATE_CONFLICT,
    GM_ERROR_MUTATE_MUTATE_CONFLICT,

    GM_ERROR_GROUP_MISMATCH, // error in group assignment
    GM_ERROR_INVALID_BUILTIN, // Invalid builtin
    GM_ERROR_INVALID_BUILTIN_ARG_COUNT, // Invalid builtin
    GM_ERROR_INVALID_BUILTIN_ARG_TYPE,

    GM_ERROR_INVALID_ARGMAX_COUNT,  // Invalid argument in <;..> min= <;..>
    GM_ERROR_INCONSISTENT_ARGMAX,   // LHS list should have all same driver

    GM_ERROR_RETURN_FOR_VOID,
    GM_ERROR_NO_VOID_RETURN,
    GM_ERROR_RETURN_MISMATCH,

    GM_ERROR_CPP_UNSUPPORTED_SYNTAX,

    GM_ERROR_CPPMAIN_RETURN_TYPE,
    GM_ERROR_CPPMAIN_NUM_GRAPHS,
    GM_ERROR_CPPMAIN_UNSUPPORTED_TYPE,

    // BACKED ERRORS
    GM_ERROR_FILEWRITE_ERROR,

    GM_ERROR_GPS_NUM_PROCS,
    GM_ERROR_GPS_PROC_NAME,
    GM_ERROR_GPS_UNSUPPORTED_OP,
    GM_ERROR_GPS_UNSUPPORTED_RANGE_MASTER,
    GM_ERROR_GPS_UNSUPPORTED_RANGE_VERTEX,
    GM_ERROR_GPS_NEED_PARALLEL,
    GM_ERROR_GPS_NBR_LOOP_INSIDE_WHILE,
    GM_ERROR_GPS_UNSUPPORTED_COLLECTION,
    GM_ERROR_GPS_MULTIPLE_GRAPH,
    GM_ERROR_GPS_NO_GRAPH,
    GM_ERROR_GPS_PULL_SYNTAX,
    GM_ERROR_GPS_RANDOM_NODE_WRITE,
    GM_ERROR_GPS_RANDOM_NODE_WRITE_DEF_SCOPE,
    GM_ERROR_GPS_RANDOM_NODE_WRITE_USE_SCOPE,
    GM_ERROR_GPS_RANDOM_NODE_WRITE_REDEF,
    GM_ERROR_GPS_RANDOM_NODE_WRITE_CONDITIONAL,
    GM_ERROR_GPS_RANDOM_NODE_READ,
    GM_ERROR_GPS_NBR_LOOP_TOO_DEEP,
    GM_ERROR_GPS_MULTIPLE_INNER_LOOP,

    GM_ERROR_GPS_EDGE_INIT,
    GM_ERROR_GPS_EDGE_READ_RANDOM,
    GM_ERROR_GPS_EDGE_WRITE_RANDOM,
    GM_ERROR_GPS_EDGE_WRITE_CONDITIONAL,
    GM_ERROR_GPS_EDGE_WRITE_RHS,
    GM_ERROR_GPS_EDGE_SEND_VERSIONS,

    GM_ERROR_END
// END_MARKER
};

extern void test_undefined_reference();
extern void gm_type_error(int errnumber, ast_id* id, char const* str1 = "", char const* str2 = "");
extern void gm_type_error(int errnumber, ast_id* id, ast_id* id2);
extern void gm_type_error(int errnumber, int l, int c, char const* str1 = "", char const* str2 = "", char const* str3 = "");
extern void gm_type_error(int errnumber, char const* str);

/*extern void gm_type_error(int errnumber, ast_id* id, const char* str1 = "", const char* str2 = "");
extern void gm_type_error(int errnumber, ast_id* id, ast_id* id2);
extern void gm_type_error(int errnumber, int l, int c, const char* str1 = "", const char* str2 = "", const char* str3 = "");
extern void gm_type_error(int errnumber, const char* str);*/

//extern void gm_conf_error(int errnumber, gm_symtab_entry* target, ast_id* evidence1);
extern void gm_conf_error(int errnumber, gm_symtab_entry* target, ast_id* ev1, ast_id* ev2, bool is_warning);
//extern void gm_conf_warning(int errnumber, gm_symtab_entry* target, ast_id* evidence1, ast_id* evidence2);

extern void gm_backend_error(int errnumber, const char* str1, const char* str2 = "");
extern void gm_backend_error(int errnumber, int l, int c, const char* str1 = "");

extern void gm_set_current_filename(char* fname);
extern char* gm_get_current_filename();
extern void gm_set_curr_procname(char* pname);

#endif