default_target : all

all: main.c codegen.c filehelper.c PH7/ph7.c ph7_.o codegen_.o main_.o filehelper_.o
	cc ph7_.o codegen_.o main_.o filehelper_.o -o phpCodeGen

clean:
	rm *.o

ph7_.o: PH7/ph7.c PH7/ph7.h
	cc -c -O6 PH7/ph7.c -o ph7_.o

codegen_.o: codegen.c codegen.h PH7/ph7.c PH7/ph7.h
	cc -c -O6 codegen.c -o codegen_.o

filehelper_.o: filehelper.c filehelper.h
	cc -c -O6 filehelper.c -o filehelper_.o

main_.o: main.c codegen.c codegen.h PH7/ph7.c PH7/ph7.h
	cc -c -O6 main.c -o main_.o

