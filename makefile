covertsigs.double: double_covertsigs.c
	gcc -std=c99 -D_GNU_SOURCE -w -o covertsigs double_covertsigs.c
covertsigs.single: single_covertsigs.c
	gcc -std=c99 -D_GNU_SOURCE -w -o covertsigs single_covertsigs.c
	
.PHONY:clean
clean: 
	rm -rf covertsigs