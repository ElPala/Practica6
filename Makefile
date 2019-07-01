.PHONY: all procesos procesosmsg
all: procesos procesosmsg

procesos: procesos.c
	gcc -o procesos procesos.c

procesosmsg: procesosmsg.c
	gcc -o procesosmsg procesosmsg.c

clean:
	rm procesos
	rm procesosmsg
