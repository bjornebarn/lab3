all : expression_tree-test

expression_tree-test: expression_tree-test.o expression_tree.o
	@echo Linking
	g++ -o expression_tree-test -Wall -Wextra -pedantic -std=c++98 expression_tree-test.o expression_tree.o

expression_tree.o: expression_tree.cc
	g++ -Wall -Wextra -pedantic -std=c++98 -c $<

expression_tree.cc: expression_tree.h

expression_tree-test.o: expression_tree-test.cc
	g++ -Wall -Wextra -pedantic -std=c++98 -c $< 

clean:
	$(RM) expression_tree-test expression_tree-test.o expression_tree.o

