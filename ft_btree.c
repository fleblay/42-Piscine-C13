/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_btree.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 17:30:32 by fle-blay          #+#    #+#             */
/*   Updated: 2022/02/14 18:10:24 by fle-blay         ###   ########.fr       */
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
	if (!root || !item || !cmpf)
		return ;
	if (*root == NULL)
	{
		*root = btree_create_node(item);
		return ;
	}
	else
	{
		if (cmpf(item, (*root)->item) >= 0)
			btree_insert_data(&(*root)->right, item, cmpf);
		else
			btree_insert_data(&(*root)->left, item, cmpf);
	}
}

void	*btree_search_item(t_btree *root, void *data_ref, int (*cmpf)(void *, void *))
{
	void	*found;
	int		res;
	if (!root)
		return (NULL);
	found = btree_search_item(root->left, data_ref, cmpf);
	if (found)
		return (found);
	res = cmpf(root->item, data_ref);
	if (!res)
		return (root->item);
	found = btree_search_item(root->right, data_ref, cmpf);
	if (found)
		return (found);
	return (NULL);
}

int	btree_level_count(t_btree *root)
{
	int	count_left;
	int	count_right;

	if (!root)
		return (0);
	count_left = btree_level_count(root->left);
	count_right = btree_level_count(root->right);
	if (count_left >= count_right)
		return (count_left + 1);
	return (count_right + 1);
}
// 

void	btree_addlist_lvl(t_btree *root, int curr_level, int desired_level, t_btree **list)
{
	t_btree	*new_elem;
	t_btree	*save;

	if (!root)
		return ;
	if (curr_level == desired_level)
	{
		new_elem = btree_create_node(root->item);
		if (!*list)
			*list = new_elem;
		else
		{
			save = *list;
			while ((*list)->right)
				*list = (*list)->right;
			(*list)->right = new_elem;
			*list = save;
		}
	}
	btree_addlist_lvl(root->left, curr_level++, desired_level, list);
	btree_addlist_lvl(root->right, curr_level++, desired_level, list);
}

/*
t_btree	**btree_get_list_of_node_lvl(t_btree *root, int level)
{
	t_btree	*root_lvl;

	root_lvl = NULL;
	
	apply infix avec un indice et un pointeur sur le debut de la liste chainee de nodes

}

void	btree_destroy_list_of_node(t_btree *root)
{

}

void	btree_apply_by_level(t_btree *root, void (*applyf)(void *item, int current_level, int is_first_elem))
{
	int		i;
	int		max;
	t_btree	*start_curr_level;

	if (!root)
		return ;
	i = 0;
	max = btree_level_count(root);
	while (i < max)
	{

	}
}
*/

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
	btree_insert_data(&root2, "1", ft_strcmp);
	printf("insert 7\n");
	btree_insert_data(&root2, "7", ft_strcmp);
	printf("insert 10\n");
	btree_insert_data(&root2, "9", ft_strcmp);
	printf("insert 8\n");
	btree_insert_data(&root2, "8", ft_strcmp);
	printf("insert 2\n");
	btree_insert_data(&root2, "2", ft_strcmp);

	printf("Parcours prefix : ");
	btree_apply_prefix(root2, print_item);
	printf("\n");

	char *ret;

	ret = btree_search_item(root2, "8", ft_strcmp);
	printf("ret : [%s]\n", ret);

	int	count;
	count = btree_level_count(root2);
	printf("biggest level : %d\n", count);

	t_btree	*list;
	list = NULL;
	btree_addlist_lvl(root, 0, 1, &list);
	while (list)
	{
		printf("content : %s\n", (char *)list->item);
		list = list->right;
	}
	return (0);
}
