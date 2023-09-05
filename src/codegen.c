#include "PH7/ph7.h"
#include "codegen.h"

static void ErrorHandle(const char *zMsg)
{
  if(zMsg != NULL){
    puts(zMsg);
  }
  /* Shutdown the library */
  ph7_lib_shutdown();
  /* Exit immediately */
   exit(0);
 }

static int Output_Consumer(const void *pOutput, unsigned int nOutputLen, void *pUserData /* Unused */)
{   
    cg_tinfo* tinfo = (cg_tinfo*)pUserData;
    if(tinfo->str == NULL){
        //Allocate Default Buffer.
        tinfo->str = (char*)malloc(CG_DEFAULT_STRBUFFER_SIZE);
        for(int i=0; i<CG_DEFAULT_STRBUFFER_SIZE; i++){
            tinfo->str[i] = 0;
        }
        tinfo->strlen = 0;
        tinfo->memlen = CG_DEFAULT_STRBUFFER_SIZE;
    }
    //If allocated, then check size.
    int reallocSize = tinfo->memlen << 1;
    if (tinfo->memlen < tinfo->strlen + 1 + nOutputLen){
        while(reallocSize < tinfo->strlen + 1 + nOutputLen){
            reallocSize <<= 1;
        }
        tinfo->str = (char*)realloc(tinfo->str, reallocSize);
        tinfo->memlen = reallocSize;
    }
    //Memory Problem Solved. Copy result into memory.
    memcpy(tinfo->str + tinfo->strlen, pOutput, nOutputLen);
    tinfo->strlen += nOutputLen;
    tinfo->str[tinfo->strlen] = '\0';

    /* All done, VM output has been written to target's memory */
    return PH7_OK;
 }

//Below functions are exposed to other files by header's declaration
cg_tinfo* new_cg_tinfo(){
    cg_tinfo* ret = (cg_tinfo*)malloc(sizeof(cg_tinfo));
    ret->str = NULL;
    ret->memlen = 0;
    ret->strlen = 0;
    return ret;
}
void free_cg_tinfo(cg_tinfo* tinfo){
    free_cg_tinfo_res(tinfo);
    free(tinfo);
    return;
}
void free_cg_tinfo_res(cg_tinfo* tinfo){
    free(tinfo->str);
    return;
}
int codeGeneration(const char* phpCode, cg_tinfo* target, char** extMsg){
    ph7 *pEngine;
    ph7_vm *pVm;

    int errCode;
    errCode = ph7_init(&pEngine);
    if(errCode != PH7_OK)
        ErrorHandle("PH7 engine initialization error.");
    
    errCode = ph7_compile_v2(
        pEngine,    //PH7 Engine
        phpCode,   //PHP Program Code
        -1,         //Compute input length automatically
        &pVm,       //OUT : Compiled PHP program
        0           //IN  : Compile Flags
    );
    if(errCode != PH7_OK){
        if(errCode == PH7_COMPILE_ERR){
            const char* errLog;
            int logLength;
            ph7_config(pEngine, PH7_CONFIG_ERR_LOG, &errLog, &logLength);
            if(logLength > 0)
                puts(errLog);
        }
        ErrorHandle("Compile Error.");
    }

    errCode = ph7_vm_config(pVm,
        PH7_VM_CONFIG_OUTPUT,
        Output_Consumer,
        target
    );

    if(errCode != PH7_OK)
        ErrorHandle("Error in installing VM output");
    
    //Execute.
    ph7_vm_exec(pVm, 0);

    //Resource free.
    ph7_vm_release(pVm);
    ph7_release(pEngine);
}