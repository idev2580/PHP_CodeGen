#ifndef __PHP_CODEGEN_HEADER__
#define __PHP_CODEGEN_HEADER__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define CG_DEFAULT_STRBUFFER_SIZE 4096
typedef struct _codegen_targetInfo {
    char * str;
    int strlen;
    int memlen;
} cg_tinfo;
cg_tinfo* new_cg_tinfo();
void free_cg_tinfo(cg_tinfo* tinfo);
void free_cg_tinfo_res(cg_tinfo* tinfo);
int codeGeneration(const char* phpCode, cg_tinfo* target, char** extMsg);

#endif