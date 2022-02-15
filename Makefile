
all :
	gcc -Wall -Wextra -Werror ft_btree.c -o tree
	valgrind ./tree

clean :	
	rm -f tree

re : clean all
