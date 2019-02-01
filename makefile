conversigs.doublesig: conversigs.c
	gcc -std=c99 -D_GNU_SOURCE -w -o conversigs double_conversigs.c

conversigs.singlesig: conversigs.c
	gcc -std=c99 -D_GNU_SOURCE -w -o conversigs conversigs.c

.PHONY:clean
clean: 
	rm -rf conversigs