
#ifndef _SMART_PARSER_H_
#define _SMART_PARSER_H_

typedef void* PARSETREE;

extern PARSETREE sp_init(void);
extern int sp_parse(PARSETREE tree, char *term, int type);
extern int sp_dispose(PARSETREE tree);
extern char *sp_get_error(PARSETREE tree);
extern char *sp_sql_clause(PARSETREE tree);

#define SP_TYPE_PLAYLIST 0
#define SP_TYPE_QUERY    1

#define FILTER_TYPE_FIREFLY  0
#define FILTER_TYPE_APPLE    1

#endif /* _SMART_PARSER_H_ */

