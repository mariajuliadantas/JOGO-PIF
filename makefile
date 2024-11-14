all:
	gcc ./src/*.c -I./include -o JOGO-PIF.out -lm

run:
	./JOGO-PIF.out

clean:
	rm JOGO-PIF.out