INCDIR = ../calculator

all : expression_tree-test expression-test

expression_tree-test: expression_tree-test.o expression_tree.o
	@echo Linking
	g++ -o expression_tree-test -I$(INCDIR) -Wall -Wextra -pedantic -std=c++98 expression_tree-test.o expression_tree.o

expression_tree.o: expression_tree.cc
	g++ -I$(INCDIR) -Wall -Wextra -pedantic -std=c++98 -c $<

expression_tree.cc: expression_tree.h

expression_tree-test.o: expression_tree-test.cc
	g++ -I$(INCDIR) -Wall -Wextra -pedantic -std=c++98 -c $< 

expression-test: expression-test.o expression.o
	@echo Linking
	g++ -o expression-test -I$(INCDIR) -Wall -Wextra -pedantic -std=c++98 expression-test.o expression.o expression_tree.o

expression.o: expression.cc
	g++ -I$(INCDIR) -Wall -Wextra -pedantic -std=c++98 -c $<

expression.cc: expression.h

expression-test.o: expression-test.cc
	g++ -I$(INCDIR) -Wall -Wextra -pedantic -std=c++98 -c $<

clean:
	$(RM) expression-test expression-test.o expression.o expression_tree-test expression_tree-test.o expression_tree.o

