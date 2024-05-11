#ifndef LIST_H
# define LIST_H 1

/* forward declarations */
typedef struct s_list_node	t_list_node;
typedef struct s_list		t_list;

/** Comprehensive enumeration of data types, must match union members of t_data.
 */
enum e_type
{
	INT,
	SIZE_T,
	CHAR,
	STR,
	PTR,
	LIST_NODE,
};

/** Data structure to be used with generic data structure implementations.
 */
typedef struct s_data
{
	enum e_type		type;
	union
	{
		int			as_int;
		size_t		as_size_t;
		char		as_char;
		char		*as_str;
		void		*as_ptr;
		t_list_node	*as_list_node;
	};
}	t_data;

/** List node structure for `next` and `prev' pointers
 *  and anonymous union that must be identical to t_data
 *  and must come as the first member of the structure.
 */
struct s_list_node
{
	struct
	{
		enum e_type		type;
		union
		{
			int			as_int;
			size_t		as_size_t;
			char		as_char;
			char		*as_str;
			void		*as_ptr;
			t_list_node	*as_list_node;
		};
	};
	t_list_node	*next;
	t_list_node	*prev;
};

/** List structure with `first' and `last' pointers,
 *  `len', and `current' for iteration.
 */
typedef struct s_list
{
	t_list_node		*first;
	t_list_node		*last;
	t_list_node		*current;
	size_t			current_idx;
	t_list			*current_stack;     /* don't iterate over this one */
	t_list			*current_idx_stack; /* don't iterate over this one */
	size_t			len;
}	t_list;

#endif /* list.h */
