#include "header.h"


/**
 * swap - swap 2 elements in a doubly linked list
 *
 * @list: linked list
 * @to_swap: element to swap
 * @compare: element to swap, placed before to_swap
 */
void swap(file_list_t **list, file_list_t *to_swap, file_list_t *compare)
{
	file_list_t *tmp2;

	if (compare->next == to_swap)
	{
		if (to_swap->next)
			(to_swap->next)->prev = compare;
		tmp2 = to_swap->next;
		to_swap->next = compare;
		compare->next = tmp2;
		tmp2 = to_swap->prev;
		to_swap->prev = compare->prev;
		compare->prev = to_swap;
	}
	else
	{
		if (to_swap->next)
			(to_swap->next->prev) = compare;
		(to_swap->prev)->next = compare;
		(compare->next)->prev = to_swap;
		tmp2 = to_swap->next;
		to_swap->next = compare->next;
		compare->next = tmp2;
		tmp2 = to_swap->prev;
		to_swap->prev = compare->prev;
		compare->prev = tmp2;
	}
	if (to_swap->prev)
		(to_swap->prev)->next = to_swap;
	else
		*list = to_swap;
}


/**
 * cocktail_sort_list - sort with cocktail sort algorithm
 *
 * @list: doubly linked list of int
 */
void cocktail_sort_list(file_list_t **list)
{
	int swapped;
	file_list_t *cursor;

	if (!list || !*list)
		return;
	swapped = 1;
	cursor = *list;
	while (swapped)
	{
		swapped = 0;
		while (cursor->next)
		{
			if (strcoll(cursor->file->d_name, (cursor->next)->file->d_name) > 0)
			{
				swap(list, cursor->next, cursor);
				++swapped;
			}
			else
			{
			cursor = cursor->next;
			}
		}
		while (cursor->prev)
		{
			if (strcoll(cursor->file->d_name, (cursor->prev)->file->d_name) < 0)
			{
				swap(list, cursor, cursor->prev);
				++swapped;
			}
			else
			{
			cursor = cursor->prev;
			}
		}
	}
}
