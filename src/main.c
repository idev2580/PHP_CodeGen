#include "PH7/ph7.h"
#include "codegen.h"
#include "filehelper.h"

#include <unistd.h>

#define __USE_MISC
#include <dirent.h>

#include <pthread.h>

#define CG_THREADS_NUM 32
#define IF_THREADED
static pthread_t threads[CG_THREADS_NUM];
static threadsCnt = 0;

int worker(const char* filePath){
    char buffer[1024];
    for(int i=0; i<1024; i++){
        buffer[i] = 0;
    }
    //1. Read File.
    char* rdContent;
    int rdByteLen;
    FILE * rdfile = fopen(filePath, "r");
    fseek(rdfile, 0, SEEK_END);
    rdByteLen = ftell(rdfile);
    fseek(rdfile,0,SEEK_SET);

    rdContent = (char*)malloc(rdByteLen + 1);
    fread(rdContent, 1, rdByteLen, rdfile);
    rdContent[rdByteLen] = '\0';
    fclose(rdfile);
    //2. Execute CodeGen.
    cg_tinfo* tinfo = new_cg_tinfo();
    codeGeneration(rdContent, tinfo, NULL);

    //3. Write into file.
    int eidx = getFilenameEidxWithoutEndingExt(filePath);
    strncpy(buffer, filePath, eidx + 1);
    FILE * wtfile = fopen(buffer,"w");
    fwrite(tinfo->str, 1, tinfo->strlen, wtfile);
    fclose(wtfile);
    //4. Free Resources.
    free_cg_tinfo(tinfo);
}
void scanAndExecute(const char* basePath){
    char buffer[1024];
    DIR *dir;
    struct dirent *dent;
    dir = opendir(basePath);
    if(dir){
        while((dent=readdir(dir)) != NULL){
            int is_above = strcmp(dent->d_name, "..") == 0;
            int is_prsnt = strcmp(dent->d_name, ".") == 0;
            if(!(is_above || is_prsnt)){
                if(dent->d_type == DT_DIR){
                    strncpy(buffer, basePath, 1023);
                    strncat(buffer, "/",1023);
                    strncat(buffer, dent->d_name,1023);
                    scanAndExecute(buffer);
                }
                else{
                    char* fileExt = getFileExt(dent->d_name);
                    if(strcmp(fileExt, "php") == 0){
                        //Start Executing CodeGen by threading.
                        strncpy(buffer, basePath, 1023);
                        strncat(buffer, "/",1023);
                        strncat(buffer, dent->d_name,1023);
                        #ifdef IF_THREADED
                        //Dumb Round-Robin, not Thread Pool.
                        if(!threadsCnt < CG_THREADS_NUM){
                            pthread_join(threads[threadsCnt % CG_THREADS_NUM], NULL);
                        }
                        pthread_create(&threads[threadsCnt % CG_THREADS_NUM], NULL, &worker, buffer);
                        #else
                        worker(buffer);
                        #endif
                    }
                }
            }
        }
    }
}
int main(int argc, char*argv[]){
    if(argc != 2){
        puts("Please put only one argument for scan directory.");
        return -1;
    }
    scanAndExecute(argv[1]);
    #ifdef IF_THREADED
    for(int i=0; i< threadsCnt % CG_THREADS_NUM; i++){
        pthread_join(threads[i], NULL);
    }
    #endif
}