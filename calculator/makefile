#
# Makefile för kalkylatorn, Nivå G. GNU GCC (g++)
#
# Filkatalog där andra delar av programet finns.
EXPRESSION = ../expression

# Kompilator
CCC = g++

# Kompilatorflaggor, lägg till '-g' om kompilering för avlusning ska göras.
CCFLAGS  += -std=c++98 -pedantic -Wall -Wextra

# Preprocessorflaggor, -I lägger till en filkatalog i inkluderingssökvägen.
CPPFLAGS += -I$(EXPRESSION) -I.

# Objektkodsmoduler som ingår i den kompletta kalkylatorn.
OBJECTS = kalkylator.o calculator.o variable_table.o expression.o expression_tree.o 

# Huvudmål - skapas med kommandot 'make' eller 'make kalkylator'.
kalkylator: $(OBJECTS) makefile
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -o kalkylator $(OBJECTS)

# Delmål (flaggan -c avbryter innan länkning)
kalkylator.o: kalkylator.cc
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c kalkylator.cc

calculator.o: calculator.h calculator.cc
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c calculator.cc

variable_table.o: variable_table.h variable_table.cc
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c variable_table.cc

expression.o: $(EXPRESSION)/expression.h $(EXPRESSION)/expression.cc
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c $(EXPRESSION)/expression.cc

expression_tree.o: $(EXPRESSION)/expression_tree.h $(EXPRESSION)/expression_tree.cc
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c $(EXPRESSION)/expression_tree.cc

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
