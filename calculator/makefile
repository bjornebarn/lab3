#
# Makefile f�r kalkylatorn, Niv� G. GNU GCC (g++)
#
# Filkatalog d�r andra delar av programet finns.
EXPRESSION = ../expression

# Kompilator
CCC = g++

# Kompilatorflaggor, l�gg till '-g' om kompilering f�r avlusning ska g�ras.
CCFLAGS  += -std=c++98 -pedantic -Wall -Wextra

# Preprocessorflaggor, -I l�gger till en filkatalog i inkluderingss�kv�gen.
CPPFLAGS += -I$(EXPRESSION) -I.

# Objektkodsmoduler som ing�r i den kompletta kalkylatorn.
OBJECTS = kalkylator.o calculator.o variable_table.o expression.o expression_tree.o 

# Huvudm�l - skapas med kommandot 'make' eller 'make kalkylator'.
kalkylator: $(OBJECTS) makefile
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -o kalkylator $(OBJECTS)

# Delm�l (flaggan -c avbryter innan l�nkning)
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

# 'make zap' tar �ven bort det k�rbara programmet och reservkopior (filer
# som slutar med tecknet '~').
zap: clean
	@ \rm -rf kalkylator *~

# '@' medf�r att kommandot inte skrivs ut p� sk�rmen d� det utf�rs av make.
# 'rm' �r ett alias f�r 'rm -i' p� IDA:s system, '\rm' inneb�r att "original-
# versionen", utan skyddsn�tet '-i', anv�nds. Se upp vid �ndring, '*' f�r 
# absolut inte finnas f�r sig!
