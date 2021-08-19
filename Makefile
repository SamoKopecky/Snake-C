main: fileFunctions.c gameplayFunctions.c initializationOfGame.c  main.c 
	gcc -pthread -std=gnu99 -pedantic -o snake fileFunctions.c gameplayFunctions.c initializationOfGame.c  main.c 

clean:
	del snake.exe