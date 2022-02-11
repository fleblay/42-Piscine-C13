/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_btree.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 17:30:32 by fle-blay          #+#    #+#             */
/*   Updated: 2022/02/11 18:59:20 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_btree.h"
#include "stdlib.h"
#include <stdio.h>

t_btree	*btree_create_node(void *item)
{
	t_btree	*new;

	new = (t_btree *)malloc(sizeof(t_btree));
	if (!new)
		return (NULL);
	*new = (t_btree){.left = NULL, .right = NULL, .item = item};
	return (new);
}

void	btree_apply_prefix(t_btree *root, void (*applyf)(void *))
{
	if (!root)
		return ;
	applyf(root->item);
	btree_apply_prefix(root->left, applyf);
	btree_apply_prefix(root->right, applyf);
}

void	btree_apply_infix(t_btree *root, void (*applyf)(void *))
{
	if (!root)
		return ;
	btree_apply_infix(root->left, applyf);
	applyf(root->item);
	btree_apply_infix(root->right, applyf);
}

void	btree_apply_suffix(t_btree *root, void (*applyf)(void *))
{
	if (!root)
		return ;
	btree_apply_suffix(root->left, applyf);
	btree_apply_suffix(root->right, applyf);
	applyf(root->item);
}

void	print_item(void *item)
{
	printf("[%s] ", (char *)item);
}

int		ft_strcmp(void *s1, void *s2)
{
	int	i;

	char	*str1 = (char *)s1;
	char	*str2 = (char *)s2;

	i = 0;
	while (str1[i] || str2[i])
	{
		if (str1[i] != str2[i])
			break ;
		i++;
	}
	return (str1[i] - str2[i]);
}

void	btree_insert_data(t_btree **root, void *item, int (*cmpf)(void *, void *))
{
	t_btree	*index;

	if (!root || !item || !cmpf)
		return ;
	if (*root == NULL)
	{
		*root = btree_create_node(item);
		return ;
	}
	else
	{
		index = *root;
		while (index)
		{
			printf("item : [%s] index->item [%s]\n", (char *)item, (char *)index->item);
			if (cmpf(item, index->item) > 0)
				index = index->left;
			else
				index = index->right;
		}
		printf("inserting : [%s]\n", (char *)item);
		index = btree_create_node(item);
	}
}

#include <string.h>

int	main(void)
{
	t_btree	*root;

	//root
	root = btree_create_node("1");
	//level 1
	root->left = btree_create_node("2");
	root->right = btree_create_node("3");
	//level 2
	root->left->left = btree_create_node("4");
	root->left->right = btree_create_node("5");
	root->right->left = btree_create_node("6");
	root->right->right = btree_create_node("7");
	//level 3
	root->left->left->left = btree_create_node("8");
	root->left->left->right = btree_create_node("9");
	root->left->right->left = btree_create_node("10");

	printf("Parcours prefix : ");
	btree_apply_prefix(root, print_item);
	printf("\n");
	printf("Parcours infix :");
	btree_apply_infix(root, print_item);
	printf("\n");
	printf("Parcours suffix : ");
	btree_apply_suffix(root, print_item);
	printf("\n");

	printf("Partie sur insert data\n");
	t_btree	*root2;
	root2 = NULL;
	printf("insert 4\n");
	btree_insert_data(&root2, "4", ft_strcmp);
	printf("insert 5\n");
	btree_insert_data(&root2, "5", ft_strcmp);
	printf("insert 2\n");
	btree_insert_data(&root2, "2", ft_strcmp);

	printf("Parcours prefix : ");
	btree_apply_prefix(root2, print_item);
	printf("\n");

	return (0);
}
