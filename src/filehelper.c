#include "filehelper.h"

const char* getFileExt(const char *s){
    const int slen = strlen(s);
    for(int i = slen-1; i>=0; i--){
        if(s[i] == '.'){
            return s+i+1;
        }
    }
    return s;
}
int getFilenameEidxWithoutEndingExt(const char *s){
    const int slen = strlen(s);
    int eidx = slen-1;
    for(int i = slen-1; i>=0; i--){
        if(s[i] == '.'){
            eidx = i - 1;
            break;
        }
    }
    return eidx;
}