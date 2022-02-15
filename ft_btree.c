/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_btree.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 17:30:32 by fle-blay          #+#    #+#             */
/*   Updated: 2022/02/15 12:15:18 by fle-blay         ###   ########.fr       */
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
	new->item = item;
	new->right = NULL;
	new->left = NULL;
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

int	btree_insert_data(t_btree **root, void *item, int (*cmpf)(void *, void *))
{
	if (*root == NULL)
	{
		*root = btree_create_node(item);
		if (!*root)
			return (0);
		return (1);
	}
	else
	{
		if (cmpf(item, (*root)->item) >= 0)
			return (btree_insert_data(&(*root)->right, item, cmpf));
		else
			return (btree_insert_data(&(*root)->left, item, cmpf));
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

void	btree_destroy(t_btree *root)
{
	if (!root)
		return ;
	btree_destroy(root->left);
	btree_destroy(root->right);
	free(root);
}

//

void	add_node_to_right(t_btree *list, t_btree *new_elem)
{
	while (list->right)
		list = list->right;
	list->right = new_elem;
}

int	btree_addlist_lvl(t_btree *root, int curr_level, int desired_level, t_btree **list)
{
	t_btree	*new_elem;

	if (!root || curr_level > desired_level)
		return (1);
	if (!btree_addlist_lvl(root->left, curr_level + 1, desired_level, list))
		return (0);
	if (curr_level == desired_level)
	{
		new_elem = btree_create_node(root->item);
		if (!new_elem)
			return (btree_destroy(*list), 0);
		if (!*list)
			*list = new_elem;
		else
			add_node_to_right(*list, new_elem);
	}
	if (!btree_addlist_lvl(root->right, curr_level + 1, desired_level, list))
		return (0);
	return (1);
}

void	printer_lvl(void *item, int current_level, int is_first_elem)
{
	if (is_first_elem)
		printf("\nDebut de ligne lvl %d :", current_level);
	else
		printf("[%s]", (char *)item);
}

int	btree_apply_by_level(t_btree *root, void (*applyf)(void *item, int current_level, int is_first_elem))
{
	int		i;
	int		max;
	t_btree	*start_curr_level;
	t_btree	*save;

	i = 0;
	max = btree_level_count(root);
	while (i < max)
	{
		start_curr_level = NULL;
		if (!btree_addlist_lvl(root, 0, i, &start_curr_level))
			return (btree_destroy(start_curr_level), 0);
		save = start_curr_level;
		if (start_curr_level)
			applyf(start_curr_level->item, i, 1);
		while (start_curr_level)
		{
			applyf(start_curr_level->item, i, 0);
			start_curr_level = start_curr_level->right;
		}
		btree_destroy(save);
		i++;
	}
	return (1);
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
	printf("insert result : %d\n", btree_insert_data(&root2, "4", ft_strcmp));
	printf("insert 5\n");
	printf("insert result : %d\n", btree_insert_data(&root2, "5", ft_strcmp));
	printf("insert 1\n");
	printf("insert result : %d\n", btree_insert_data(&root2, "1", ft_strcmp));
	printf("insert 7\n");
	printf("insert result : %d\n", btree_insert_data(&root2, "7", ft_strcmp));
	printf("insert 9\n");
	printf("insert result : %d\n", btree_insert_data(&root2, "9", ft_strcmp));
	printf("insert 8\n");
	printf("insert result : %d\n", btree_insert_data(&root2, "8", ft_strcmp));
	printf("insert 2\n");
	printf("insert result : %d\n", btree_insert_data(&root2, "2", ft_strcmp));

	printf("Parcours prefix : ");
	btree_apply_prefix(root2, print_item);
	printf("\n");

	char *ret;

	ret = btree_search_item(root2, "8", ft_strcmp);
	printf("ret : [%s]\n", ret);

	int	count;
	count = btree_level_count(root2);
	printf("biggest level : %d\n", count);

	printf("\nret printer root : %d\n", btree_apply_by_level(root, &printer_lvl));
	printf("\n");
	printf("\nret printer root2 : %d\n", btree_apply_by_level(root2, &printer_lvl));
	printf("\n");

	btree_destroy(root);
	btree_destroy(root2);

	return (0);
}
