VPATH = src
PROG1 = prizematch
PROG2 = geneNo

all: prog1 prog2
.PHONY : all

prog1 : main.o
	g++ -o $(PROG1) $<

prog2 : geneNo.o
	g++ -o $(PROG2) $<

.PHONY : clean
clean:
	rm -f *.o *~ 
