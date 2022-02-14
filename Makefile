
all :
	gcc -Wall -Wextra -Werror ft_btree.c -o tree
	./tree

clean :	
	rm -f tree

re : clean all
