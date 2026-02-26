game: main.o bootFunctions.o drawFunctions.o colors.o coordList.o boxList.o moveSprite.o sub.h
	gcc -g objects/*.o -F/opt/homebrew/Frameworks -framework SDL2_image -framework SDL2_ttf -framework SDL2 -rpath /opt/homebrew/Frameworks -lm -o ../subgame
main.o: main.c sub.h
	gcc -c main.c -o objects/main.o
bootFunctions.o: bootFunctions.c sub.h
	gcc -c bootFunctions.c -o objects/bootFunctions.o
drawFunctions.o: drawFunctions.c sub.h
	gcc -c drawFunctions.c -o objects/drawFunctions.o
colors.o: colors.c sub.h
	gcc -c colors.c -o objects/colors.o
coordList.o: coordList.c sub.h
	gcc -c coordList.c -o objects/coordList.o
boxList.o: boxList.c sub.h
	gcc -c boxList.c -o objects/boxList.o
moveSprite.o: moveSprite.c sub.h
	gcc -c moveSprite.c -o objects/moveSprite.o
sub.h:

clean:
	rm -rf ../subgame objects/*
