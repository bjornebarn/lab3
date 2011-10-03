#
# Makefile för kalkylatorn, Nivå G. GNU GCC (g++)
#
# Filkatalog där andra delar av programet finns.
EXPRESSION = ../Expression

# Kompilator
CCC = g++

# Kompilatorflaggor, lägg till '-g' om kompilering för avlusning ska göras.
CCFLAGS  += -std=c++98 -pedantic -Wall -Wextra

# Preprocessorflaggor, -I lägger till en filkatalog i inkluderingssökvägen.
CPPFLAGS += -I$(EXPRESSION)

# Objektkodsmoduler som ingår i den kompletta kalkylatorn.
OBJECTS = kalkylator.o Calculator.o Expression.o Expression_Tree.o

# Huvudmål - skapas med kommandot 'make' eller 'make kalkylator'.
kalkylator: $(OBJECTS) Makefile
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -o kalkylator $(OBJECTS)

# Delmål (flaggan -c avbryter innan länkning)
kalkylator.o: kalkylator.cc
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c kalkylator.cc

Calculator.o: Calculator.h Calculator.cc
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c Calculator.cc

Expression.o: $(EXPRESSION)/Expression.h $(EXPRESSION)/Expression.cc
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c $(EXPRESSION)/Expression.cc

Expression_Tree.o: $(EXPRESSION)/Expression_Tree.h $(EXPRESSION)/Expression_Tree.cc
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c $(EXPRESSION)/Expression_Tree.cc

# 'make clean' tar bort objektkodsfiler och 'core' (minnesdump).
clean:
	@ \rm -rf *.o core

# 'make zap' tar även bort det körbara programmet och reservkopior (filer
# som slutar med tecknet '~').
zap: clean
	@ \rm -rf kalkylator *~

# '@' medför att kommandot inte skrivs ut på skärmen då det utförs av make.
# 'rm' är ett alias för 'rm -i' på IDA:s system, '\rm' innebär att "original-
# versionen", utan skyddsnätet '-i', används. Se upp vid ändring, '*' får 
# absolut inte finnas för sig!
