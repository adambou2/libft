*This project has been created as part of the 42 curriculum by abouhou.*

# Libft

## Description

**Libft** is the first project of the 42 curriculum. The goal is to build my own C library, `libft.a`, by re-writing a set of standard C library functions and adding some useful extra functions. The library is then reused in later 42 projects.

The project teaches me to:

- understand how standard functions like `strlen`, `memcpy` or `atoi` really work;
- manage memory by hand (`malloc` and `free`) and avoid leaks;
- work with pointers, strings and function pointers;
- build a static library with a `Makefile` and `ar`;
- write code that follows the 42 Norm and does not crash.

### Overview of the library

The library has three parts, all mandatory.

#### Part 1 - Libc functions

Re-written versions of standard functions, with the same behavior as the originals. Each one has the `ft_` prefix.

| Group | Functions |
|---|---|
| Character checks | `ft_isalpha`, `ft_isdigit`, `ft_isalnum`, `ft_isascii`, `ft_isprint` |
| Character conversion | `ft_toupper`, `ft_tolower` |
| Strings | `ft_strlen`, `ft_strlcpy`, `ft_strlcat`, `ft_strchr`, `ft_strrchr`, `ft_strncmp`, `ft_strnstr`, `ft_strdup` |
| Memory | `ft_memset`, `ft_bzero`, `ft_memcpy`, `ft_memmove`, `ft_memchr`, `ft_memcmp`, `ft_calloc` |
| Conversion | `ft_atoi` |

#### Part 2 - Additional functions

Functions that are not in the standard libc, or that exist in a different form.

| Function | What it does |
|---|---|
| `ft_substr` | Returns a new string taken from a part of `s` (`start` and `len`). |
| `ft_strjoin` | Returns a new string made of `s1` followed by `s2`. |
| `ft_strtrim` | Returns a copy of `s1` without the characters of `set` at the start and at the end. |
| `ft_split` | Splits a string with a delimiter character. Returns a NULL-terminated array of new strings. |
| `ft_itoa` | Converts an integer to a new string (works with `INT_MIN`). |
| `ft_strmapi` | Applies `f(index, char)` to each character and returns a new string with the results. |
| `ft_striteri` | Applies `f(index, &char)` to each character of the string, in place. |
| `ft_putchar_fd` | Writes a character to a file descriptor. |
| `ft_putstr_fd` | Writes a string to a file descriptor. |
| `ft_putendl_fd` | Writes a string followed by a newline to a file descriptor. |
| `ft_putnbr_fd` | Writes an integer to a file descriptor. |

#### Part 3 - Linked list functions

The library uses this structure (declared in `libft.h`):

```c
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;
```

| Function | What it does |
|---|---|
| `ft_lstnew` | Creates a new node with the given content. |
| `ft_lstadd_front` | Adds a node at the start of the list. |
| `ft_lstsize` | Counts the nodes of the list. |
| `ft_lstlast` | Returns the last node of the list. |
| `ft_lstadd_back` | Adds a node at the end of the list. |
| `ft_lstdelone` | Frees the content of one node with `del`, then frees the node. |
| `ft_lstclear` | Deletes and frees a node and all the nodes after it, then sets the pointer to NULL. |
| `ft_lstiter` | Applies a function to the content of each node. |
| `ft_lstmap` | Builds a new list by applying a function to each content. If an allocation fails, everything is cleaned up. |

### Technical choices

- Written in C and compiled with `cc -Wall -Wextra -Werror`.
- Follows the 42 Norm. No global variables. Helper functions are `static`.
- The library is built with `ar` (not `libtool`).
- Memory is freed on every error path, so there are no leaks.
- NULL arguments are handled in the functions where the subject does not define the behavior, so the functions do not crash.

## Instructions

### Compilation

Clone the repository and run `make` at the root:

```bash
git clone <repository_url> libft
cd libft
make
```

This creates `libft.a` at the root of the repository.

### Makefile rules

| Command | Result |
|---|---|
| `make` or `make all` | Builds `libft.a`. |
| `make clean` | Removes the object files (`.o`). |
| `make fclean` | Removes the object files and `libft.a`. |
| `make re` | Runs `fclean`, then builds everything again. |

### Using the library in another project

1. Copy `libft.a` and `libft.h` into your project, or point to them with flags.
2. Include the header in your code:

```c
#include "libft.h"
```

3. Compile and link with the library:

```bash
cc -Wall -Wextra -Werror main.c libft.a -o main
```

### Usage example

```c
#include "libft.h"

int	main(void)
{
	char	*str;

	str = ft_strjoin("Hello, ", "world!");
	ft_putendl_fd(str, 1);
	free(str);
	return (0);
}
```

### Testing

I tested the library with my own test programs, and with `valgrind` or `-fsanitize=address` to check for leaks. The Norm is checked with `norminette`.

```bash
norminette -R DefineCheck
valgrind --leak-check=full ./my_program
```

## Resources

### Documentation and references

**General C language and pointers**

- GEEKSFORGEEKS : C PROGRAMMING TUTORIAL (https://www.geeksforgeeks.org/c/c-programming-language/) - this site is explaining the pointers with simple sturcture and examples.

**Standard library man pages** (`man 3 strlen`, `man 3 memcpy`, ...)

- I used the manual to understand the behavior of the functions and return of that function.

**Memory: `malloc`, `free`, leaks, valgrind**

- MEDUIM : A Deep Dive into Building a Memory Allocator (https://medium.com/@kondam.reddy/a-deep-dive-into-building-a-memory-allocator-dd5333a98195) - I Learned what is an allocatore what are the types of memory allocations.

**Strings and `ft_split`, `ft_itoa`, `ft_strtrim`**

- GEEKSFORGEEKS : C PROGRAMMING TUTORIAL (https://www.geeksforgeeks.org/c/c-programming-language/) - I learned what is a string and how to manipulate it.

**Function pointers (`ft_strmapi`, `ft_striteri`, `ft_lstiter`, `ft_lstmap`)**

- GEEKSFORGEEKS : C PROGRAMMING TUTORIAL (https://www.geeksforgeeks.org/c/c-programming-language/) - I Learned what is function pointer and how to use it in my programs.

**Linked lists**
- GEEKSFORGEEKS : C PROGRAMMING TUTORIAL (https://www.geeksforgeeks.org/c/c-programming-language/) - I took a general idea about linked list.
- I Learned using AI chatbots.

**Makefile and static libraries (`ar`)**

- Jacob Sorber (https://www.youtube.com/results?search_query=jacob+sorber) - I Learned how to creat and set a make file.
- A Simple Makefile Tutorial (https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/index.htm) - ILearned what each part "rules" can do in the make file when you type make in the terminal.

### Use of AI

AI chatbots were used as a learning and checking tool.

| Task | How AI was used |
|---|---|
| Understanding the subject | Explained what functions such as `ft_strmapi`, `ft_striteri` and the list functions do, in simple words, with edge cases. |
| Code review | I wrote the code of the functions. AI pointed out bugs and edge cases, and I fixed and understood them. |
| Header and Makefile | AI checked `libft.h` and the `Makefile` against the subject. |
| Testing | AI generated test programs (`full_test.c`) that I used to check my functions. They are not part of the submission. |
| Project tracking | AI created a checklist spreadsheet of the functions and the subject requirements. |
| Documentation | AI helped in the creattion of this README. |

I read, tested and understood all the code in this repository, and I can explain it.

## Author

- abouhou
