/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_btree.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 17:30:32 by fle-blay          #+#    #+#             */
/*   Updated: 2022/02/16 10:25:13 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_btree.h"
#include "stdlib.h"
#include <stdio.h>

t_btree	*ft_btree_create_node(void *item)
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

void	ft_btree_apply_prefix(t_btree *root, void (*applyf)(void *))
{
	if (!root)
		return ;
	applyf(root->item);
	ft_btree_apply_prefix(root->left, applyf);
	ft_btree_apply_prefix(root->right, applyf);
}

void	ft_btree_apply_infix(t_btree *root, void (*applyf)(void *))
{
	if (!root)
		return ;
	ft_btree_apply_infix(root->left, applyf);
	applyf(root->item);
	ft_btree_apply_infix(root->right, applyf);
}

void	ft_btree_apply_suffix(t_btree *root, void (*applyf)(void *))
{
	if (!root)
		return ;
	ft_btree_apply_suffix(root->left, applyf);
	ft_btree_apply_suffix(root->right, applyf);
	applyf(root->item);
}

void	print_item(void *item)
{
	printf("[%s] ", (char *)item);
}

int	ft_strcmp(void *s1, void *s2)
{
	int		i;
	char	*str1;
	char	*str2;

	str1 = (char *)s1;
	str2 = (char *)s2;
	i = 0;
	while (str1[i] || str2[i])
	{
		if (str1[i] != str2[i])
			break ;
		i++;
	}
	return (str1[i] - str2[i]);
}

int	ft_btree_insert_data(t_btree **root, void *item,
	int (*cmpf)(void *, void *))
{
	if (*root == NULL)
	{
		*root = ft_btree_create_node(item);
		if (!*root)
			return (0);
		return (1);
	}
	else
	{
		if (cmpf(item, (*root)->item) >= 0)
			return (ft_btree_insert_data(&(*root)->right, item, cmpf));
		else
			return (ft_btree_insert_data(&(*root)->left, item, cmpf));
	}
}

void	*ft_btree_search_item(t_btree *root, void *data_ref,
	int (*cmpf)(void *, void *))
{
	void	*found;
	int		res;

	if (!root)
		return (NULL);
	found = ft_btree_search_item(root->left, data_ref, cmpf);
	if (found)
		return (found);
	res = cmpf(root->item, data_ref);
	if (!res)
		return (root->item);
	found = ft_btree_search_item(root->right, data_ref, cmpf);
	if (found)
		return (found);
	return (NULL);
}

int	ft_btree_level_count(t_btree *root)
{
	int	count_left;
	int	count_right;

	if (!root)
		return (0);
	count_left = ft_btree_level_count(root->left);
	count_right = ft_btree_level_count(root->right);
	if (count_left >= count_right)
		return (count_left + 1);
	return (count_right + 1);
}

void	ft_btree_destroy(t_btree *root)
{
	if (!root)
		return ;
	ft_btree_destroy(root->left);
	ft_btree_destroy(root->right);
	free(root);
}

//

void	ft_add_node_to_right(t_btree *list, t_btree *new_elem)
{
	while (list->right)
		list = list->right;
	list->right = new_elem;
}

int	ft_btree_addlist_lvl(t_btree *root, int curr_level,
	int desired_level, t_btree **list)
{
	t_btree	*new_elem;

	if (!root || curr_level > desired_level)
		return (1);
	if (!ft_btree_addlist_lvl(root->left, curr_level + 1, desired_level, list))
		return (0);
	if (curr_level == desired_level)
	{
		new_elem = ft_btree_create_node(root->item);
		if (!new_elem)
			return (ft_btree_destroy(*list), 0);
		if (!*list)
			*list = new_elem;
		else
			ft_add_node_to_right(*list, new_elem);
	}
	if (!ft_btree_addlist_lvl(root->right, curr_level + 1, desired_level, list))
		return (0);
	return (1);
}

void	printer_lvl(void *item, int current_level, int is_first_elem)
{
	if (is_first_elem)
		printf("\nDebut de ligne lvl %d : [%s]", current_level, (char *)item);
	else
		printf("[%s]", (char *)item);
}

int	ft_btree_apply_by_level(t_btree *root,
	void (*applyf)(void *item, int current_level, int is_first_elem))
{
	int		i;
	t_btree	*start_curr_level;
	t_btree	*save;

	i = 0;
	while (i < ft_btree_level_count(root))
	{
		start_curr_level = NULL;
		if (!ft_btree_addlist_lvl(root, 0, i, &start_curr_level))
			return (ft_btree_destroy(start_curr_level), 0);
		save = start_curr_level;
		if (start_curr_level)
		{
			applyf(start_curr_level->item, i, 1);
			start_curr_level = start_curr_level->right;
		}
		while (start_curr_level)
		{
			applyf(start_curr_level->item, i, 0);
			start_curr_level = start_curr_level->right;
		}
		ft_btree_destroy(save);
		i++;
	}
	return (1);
}

t_btree	*ft_btree_search_node(t_btree *root, void *data_ref,
	int (*cmpf)(void *, void *))
{
	t_btree	*found;
	int		res;

	if (!root)
		return (NULL);
	found = ft_btree_search_node(root->left, data_ref, cmpf);
	if (found)
		return (found);
	res = cmpf(root->item, data_ref);
	if (!res)
		return (root);
	found = ft_btree_search_node(root->right, data_ref, cmpf);
	if (found)
		return (found);
	return (NULL);
}

void	printer_lvl_node(t_btree *root, t_btree *list, int current_level, int is_first_elem)
{
	int	left_space;
	int	right_space;
	t_btree	*node_in_real_tree;
	int	offset;

	offset = ft_btree_level_count(root) - current_level;
	node_in_real_tree = ft_btree_search_node(root, list->item, ft_strcmp);
	if (!node_in_real_tree)
		return ;
	right_space = (ft_btree_level_count(node_in_real_tree->right) + 1) * offset;
	left_space = (ft_btree_level_count(node_in_real_tree->left) + 1)* offset;
	if (is_first_elem)
		printf("\nDebut de ligne lvl %d :", current_level);
	while (left_space-- > 0)
		printf(" ");
	printf("[%s]", (char *)list->item);
	while (right_space-- > 0)
		printf(" ");
}

int	ft_btree_apply_by_level_to_node(t_btree *root,
	void (*applyf)(t_btree *root, t_btree *list, int current_level, int is_first_elem))
{
	int		i;
	t_btree	*start_curr_level;
	t_btree	*save;

	i = 0;
	while (i < ft_btree_level_count(root))
	{
		start_curr_level = NULL;
		if (!ft_btree_addlist_lvl(root, 0, i, &start_curr_level))
			return (ft_btree_destroy(start_curr_level), 0);
		save = start_curr_level;
		if (start_curr_level)
		{
			applyf(root, start_curr_level, i, 1);
			start_curr_level = start_curr_level->right;
		}
		while (start_curr_level)
		{
			applyf(root, start_curr_level, i, 0);
			start_curr_level = start_curr_level->right;
		}
		ft_btree_destroy(save);
		i++;
	}
	return (1);
}

int	ft_btree_node_count(t_btree *root)
{
	int	count_left;
	int	count_right;

	if (!root)
		return (0);
	count_left = ft_btree_node_count(root->left);
	count_right = ft_btree_node_count(root->right);
	return (count_right + count_left + 1);
}

static char	**create_sheet(t_btree *root)
{
	int		size;
	char	**sheet;

	size = ft_btree_node_count(root);
	sheet = (char **)malloc((size + 1) * sizeof(char *));
	if (!sheet)
		return (NULL);
	while (size > -1)
	{
		sheet[size] = NULL;
		size--;
	}
	return (sheet);
}

static void	insert_end(char **sheet, int size_sheet, char *to_insert, int lvl)
{
	int	last_index;
	int	i;

	i = 0;
	last_index = size_sheet - 1;
	while (last_index >= 0 && sheet[last_index])
		last_index--;

	while (i < lvl)
	{
		i++;
	}
	sheet[last_index] = to_insert;
}

void	fill_sheet(t_btree *root, int lvl, char **sheet, int size_sheet)
{

	if (!root)
		return ;
	fill_sheet(root->left, lvl + 1, sheet, size_sheet);
	insert_end(sheet, size_sheet, (char *)(root->item), lvl);
	fill_sheet(root->right, lvl + 1, sheet, size_sheet);
}

void ft_btree_print(t_btree *root)
{
	char	**sheet;

	sheet = create_sheet(root);
	fill_sheet(root, 0, sheet, ft_btree_node_count(root));
	while (*sheet)
	{
		printf("%s\n", *sheet);
		sheet++;
	}

	printf("\n");
}

#include <string.h>

int	main(void)
{
	t_btree	*root;

	//root
	root = ft_btree_create_node("1");
	//level 1
	root->left = ft_btree_create_node("2");
	root->right = ft_btree_create_node("3");
	//level 2
	root->left->left = ft_btree_create_node("4");
	root->left->right = ft_btree_create_node("5");
	root->right->left = ft_btree_create_node("6");
	root->right->right = ft_btree_create_node("7");
	//level 3
	root->left->left->left = ft_btree_create_node("8");
	root->left->left->right = ft_btree_create_node("9");
	root->left->right->left = ft_btree_create_node("10");

	printf("Parcours prefix : ");
	ft_btree_apply_prefix(root, print_item);
	printf("\n");
	printf("Parcours infix :");
	ft_btree_apply_infix(root, print_item);
	printf("\n");
	printf("Parcours suffix : ");
	ft_btree_apply_suffix(root, print_item);
	printf("\n");

	printf("Partie sur insert data\n");
	t_btree	*root2;
	root2 = NULL;
	printf("insert 4\n");
	printf("insert result : %d\n", ft_btree_insert_data(&root2, "4", ft_strcmp));
	printf("insert 5\n");
	printf("insert result : %d\n", ft_btree_insert_data(&root2, "5", ft_strcmp));
	printf("insert 1\n");
	printf("insert result : %d\n", ft_btree_insert_data(&root2, "1", ft_strcmp));
	printf("insert 7\n");
	printf("insert result : %d\n", ft_btree_insert_data(&root2, "7", ft_strcmp));
	printf("insert 9\n");
	printf("insert result : %d\n", ft_btree_insert_data(&root2, "9", ft_strcmp));
	printf("insert 8\n");
	printf("insert result : %d\n", ft_btree_insert_data(&root2, "8", ft_strcmp));
	printf("insert 2\n");
	printf("insert result : %d\n", ft_btree_insert_data(&root2, "2", ft_strcmp));

	printf("Parcours prefix : ");
	ft_btree_apply_prefix(root2, print_item);
	printf("\n");

	char *ret;

	ret = ft_btree_search_item(root2, "8", ft_strcmp);
	printf("ret : [%s]\n", ret);

	int	count;
	count = ft_btree_level_count(root2);
	printf("biggest level : %d\n", count);

	printf("\nret printer root : %d\n", ft_btree_apply_by_level(root, &printer_lvl));
	printf("\n");
	printf("\nret printer root2 : %d\n", ft_btree_apply_by_level(root2, &printer_lvl));
	printf("\n");

	ft_btree_apply_by_level_to_node(root, &printer_lvl_node);
	printf("\n");
	ft_btree_apply_by_level_to_node(root2, &printer_lvl_node);
	printf("\n");

	printf("node count :%d\n", ft_btree_node_count(root));

	ft_btree_print(root);

	ft_btree_destroy(root);
	ft_btree_destroy(root2);
	return (0);
}
