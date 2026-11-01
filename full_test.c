/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   full_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouhou <abouhou@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by n only func       #+#    #+#             */
/*   Updated: 2026/11/01 01:31:28 by abouhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*   tests still run.                                                         */
/*   Leak check:  valgrind --leak-check=full ./full_test                      */
/* ************************************************************************** */

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include "libft.h"

static int	*g_stats;	/* [0] passed, [1] failed, [2] crashed functions */
static int	g_freed = 0;

/* ------------------------------ helpers ---------------------------------- */

static void	check(const char *name, int ok)
{
	if (ok)
	{
		g_stats[0]++;
		printf("  [OK]   %s\n", name);
	}
	else
	{
		g_stats[1]++;
		printf("  [FAIL] %s\n", name);
	}
}

static void	check_int(const char *name, long got, long exp)
{
	if (got == exp)
	{
		g_stats[0]++;
		printf("  [OK]   %s\n", name);
	}
	else
	{
		g_stats[1]++;
		printf("  [FAIL] %s (got %ld, expected %ld)\n", name, got, exp);
	}
}

static void	quote(char *out, size_t n, const char *s)
{
	if (!s)
		snprintf(out, n, "NULL");
	else
		snprintf(out, n, "\"%.80s\"", s);
}

static void	check_str(const char *name, const char *got, const char *exp)
{
	int		ok;
	char	g[128];
	char	e[128];

	if (!got || !exp)
		ok = (got == exp);
	else
		ok = (strcmp(got, exp) == 0);
	if (ok)
	{
		g_stats[0]++;
		printf("  [OK]   %s\n", name);
		return ;
	}
	quote(g, sizeof(g), got);
	quote(e, sizeof(e), exp);
	g_stats[1]++;
	printf("  [FAIL] %s (got %s, expected %s)\n", name, g, e);
}

static int	sign(int x)
{
	return ((x > 0) - (x < 0));
}

/* reference definitions used for the ctype tests */
static int	ref_alpha(int c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

static int	ref_digit(int c)
{
	return (c >= '0' && c <= '9');
}

static int	ref_alnum(int c)
{
	return (ref_alpha(c) || ref_digit(c));
}

static int	ref_ascii(int c)
{
	return (c >= 0 && c <= 127);
}

static int	ref_print(int c)
{
	return (c >= 32 && c <= 126);
}

static int	ref_upper(int c)
{
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}

static int	ref_lower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}

static int	range_ok(int (*f)(int), int (*ref)(int))
{
	int	c;

	c = 0;
	while (c < 256)
	{
		if (f(c) != ref(c))
			return (0);
		c++;
	}
	return (1);
}

/* ---------------------------- part 1: libc ------------------------------- */

static void	t_isalpha(void)
{
	check("0-255 give exactly 1 or 0", range_ok(ft_isalpha, ref_alpha));
}

static void	t_isdigit(void)
{
	check("0-255 give exactly 1 or 0", range_ok(ft_isdigit, ref_digit));
}

static void	t_isalnum(void)
{
	check("0-255 give exactly 1 or 0", range_ok(ft_isalnum, ref_alnum));
}

static void	t_isascii(void)
{
	check("0-255 give exactly 1 or 0", range_ok(ft_isascii, ref_ascii));
}

static void	t_isprint(void)
{
	check("0-255 give exactly 1 or 0", range_ok(ft_isprint, ref_print));
}

static void	t_toupper(void)
{
	check("0-255 match the expected value", range_ok(ft_toupper, ref_upper));
}

static void	t_tolower(void)
{
	check("0-255 match the expected value", range_ok(ft_tolower, ref_lower));
}

static void	t_strlen(void)
{
	char	big[1001];

	memset(big, 'a', 1000);
	big[1000] = '\0';
	check_int("empty string", ft_strlen(""), 0);
	check_int("hello", ft_strlen("hello"), 5);
	check_int("with spaces", ft_strlen("a b c d"), 7);
	check_int("stops at the first \\0", ft_strlen("ab\0cd"), 2);
	check_int("1000 chars", ft_strlen(big), 1000);
}

static void	t_memset(void)
{
	static const int	vals[] = {0, 'A', 300, -1};
	static const int	ns[] = {0, 1, 7, 20};
	char				a[20];
	char				b[20];
	int					i;
	int					j;
	int					ok;

	ok = 1;
	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			memset(a, 'x', 20);
			memset(b, 'x', 20);
			ft_memset(a, vals[i], ns[j]);
			memset(b, vals[i], ns[j]);
			if (memcmp(a, b, 20) != 0)
				ok = 0;
		}
	}
	check("same result as libc (values 0, 'A', 300, -1; n = 0, 1, 7, 20)", ok);
	check("returns the pointer", ft_memset(a, 'z', 5) == a);
}

static void	t_bzero(void)
{
	char	a[20];
	char	b[20];

	memset(a, 'x', 20);
	memset(b, 'x', 20);
	ft_bzero(a, 7);
	memset(b, 0, 7);
	check("n = 7", memcmp(a, b, 20) == 0);
	ft_bzero(a, 0);
	check("n = 0 changes nothing", memcmp(a, b, 20) == 0);
	ft_bzero(a, 20);
	memset(b, 0, 20);
	check("n = 20", memcmp(a, b, 20) == 0);
}

static void	t_memcpy(void)
{
	static const int			ns[] = {0, 1, 5, 10};
	static const unsigned char	bin[5] = {1, 0, 2, 0, 3};
	char						a[20];
	char						b[20];
	int							i;
	int							ok;

	ok = 1;
	i = -1;
	while (++i < 4)
	{
		memset(a, 'x', 20);
		memset(b, 'x', 20);
		ft_memcpy(a, "abcdefghij", ns[i]);
		memcpy(b, "abcdefghij", ns[i]);
		if (memcmp(a, b, 20) != 0)
			ok = 0;
	}
	check("same result as libc (n = 0, 1, 5, 10)", ok);
	memset(a, 'x', 20);
	ft_memcpy(a, bin, 5);
	check("copies bytes after a \\0", memcmp(a, bin, 5) == 0);
	check("returns dest", ft_memcpy(a, "abc", 3) == a);
}

static void	t_memmove(void)
{
	char	a[20];
	char	b[20];

	strcpy(a, "0123456789");
	strcpy(b, "0123456789");
	ft_memmove(a + 2, a, 5);
	memmove(b + 2, b, 5);
	check("overlap, dest after src", strcmp(a, b) == 0);
	strcpy(a, "0123456789");
	strcpy(b, "0123456789");
	ft_memmove(a, a + 2, 5);
	memmove(b, b + 2, 5);
	check("overlap, dest before src", strcmp(a, b) == 0);
	strcpy(a, "0123456789");
	ft_memmove(a, a, 5);
	check("dest == src", strcmp(a, "0123456789") == 0);
	ft_memmove(a + 1, a, 0);
	check("n = 0", strcmp(a, "0123456789") == 0);
	memset(b, 'x', 20);
	ft_memmove(b, "hello", 6);
	check("no overlap", strcmp(b, "hello") == 0);
	check("returns dest", ft_memmove(b, "ab", 2) == b);
}

static void	t_strlcpy(void)
{
	char	d[10];

	memset(d, 'x', 10);
	check_int("size 10: return value", ft_strlcpy(d, "hello", 10), 5);
	check_str("size 10: result", d, "hello");
	memset(d, 'x', 10);
	check_int("size 3: return value (length of src)", ft_strlcpy(d, "hello", 3), 5);
	check_str("size 3: truncated and terminated", d, "he");
	check("size 3: bytes after are untouched", d[3] == 'x');
	memset(d, 'x', 10);
	check_int("size 0: return value", ft_strlcpy(d, "hello", 0), 5);
	check("size 0: dst untouched", d[0] == 'x');
	memset(d, 'x', 10);
	check_int("size 6 (exact fit): return value", ft_strlcpy(d, "hello", 6), 5);
	check_str("size 6 (exact fit): result", d, "hello");
	memset(d, 'x', 10);
	ft_strlcpy(d, "hello", 5);
	check_str("size 5: truncated to 4 chars", d, "hell");
	memset(d, 'x', 10);
	check_int("empty src: return value", ft_strlcpy(d, "", 10), 0);
	check_str("empty src: result", d, "");
}

static void	t_strlcat(void)
{
	char	d[10];

	memset(d, 0, 10);
	strcpy(d, "ab");
	check_int("size 10: return value", ft_strlcat(d, "cd", 10), 4);
	check_str("size 10: result", d, "abcd");
	memset(d, 0, 10);
	strcpy(d, "ab");
	check_int("size 4: return value", ft_strlcat(d, "cd", 4), 4);
	check_str("size 4: truncated", d, "abc");
	memset(d, 0, 10);
	strcpy(d, "ab");
	check_int("size 3: return value", ft_strlcat(d, "cd", 3), 4);
	check_str("size 3: no room, dst unchanged", d, "ab");
	check_int("size 2: return value (size + len(src))", ft_strlcat(d, "cd", 2), 4);
	check_str("size 2: dst unchanged", d, "ab");
	check_int("size 1: return value", ft_strlcat(d, "cd", 1), 3);
	check_int("size 0: return value", ft_strlcat(d, "cd", 0), 2);
	check_str("size 0: dst unchanged", d, "ab");
	memset(d, 0, 10);
	check_int("empty dst: return value", ft_strlcat(d, "xyz", 10), 3);
	check_str("empty dst: result", d, "xyz");
}

static void	t_strchr(void)
{
	const char	*s = "hello world";
	const int	cs[] = {'h', 'o', 'l', 'd', 'z', '\0', 'o' + 256};
	int			i;
	int			ok;

	ok = 1;
	i = -1;
	while (++i < 7)
		if (ft_strchr(s, cs[i]) != strchr(s, cs[i]))
			ok = 0;
	check("same result as libc (found, not found, '\\0', c + 256)", ok);
	check("empty string, '\\0'", ft_strchr("", '\0') != NULL);
	check("empty string, 'a'", ft_strchr("", 'a') == NULL);
	check("returns the first match", ft_strchr(s, 'l') == s + 2);
}

static void	t_strrchr(void)
{
	const char	*s = "hello world";
	const int	cs[] = {'h', 'o', 'l', 'd', 'z', '\0', 'o' + 256};
	int			i;
	int			ok;

	ok = 1;
	i = -1;
	while (++i < 7)
		if (ft_strrchr(s, cs[i]) != strrchr(s, cs[i]))
			ok = 0;
	check("same result as libc (found, not found, '\\0', c + 256)", ok);
	check("empty string, 'a'", ft_strrchr("", 'a') == NULL);
	check("returns the last match", ft_strrchr(s, 'o') == s + 7);
}

static void	t_strncmp(void)
{
	const char	*s1[] = {"abc", "abc", "abd", "abc", "abcd", "", "a", "\200", "same"};
	const char	*s2[] = {"abc", "abd", "abc", "abcd", "abc", "a", "", "a", "same"};
	const int	ns[] = {0, 1, 3, 4, 5, 10};
	int			i;
	int			j;
	int			ok;

	ok = 1;
	i = -1;
	while (++i < 9)
	{
		j = -1;
		while (++j < 6)
			if (sign(ft_strncmp(s1[i], s2[i], ns[j]))
				!= sign(strncmp(s1[i], s2[i], ns[j])))
				ok = 0;
	}
	check("same sign as libc on 9 pairs and 6 values of n", ok);
	check("n = 0 returns 0", ft_strncmp("a", "b", 0) == 0);
	check("compares as unsigned char", ft_strncmp("\200", "a", 1) > 0);
	check("stops after n chars", ft_strncmp("abcX", "abcY", 3) == 0);
}

static void	t_memchr(void)
{
	const unsigned char	buf[6] = {1, 2, 3, 0, 4, 200};
	const int			cs[] = {3, 0, 4, 200, 99, 1};
	const int			ns[] = {0, 1, 3, 4, 6};
	int					i;
	int					j;
	int					ok;

	ok = 1;
	i = -1;
	while (++i < 6)
	{
		j = -1;
		while (++j < 5)
			if (ft_memchr(buf, cs[i], ns[j]) != memchr(buf, cs[i], ns[j]))
				ok = 0;
	}
	check("same result as libc (6 values, 5 sizes)", ok);
	check("finds a byte after a \\0", ft_memchr(buf, 4, 6) == buf + 4);
	check("n limits the search", ft_memchr(buf, 4, 4) == NULL);
}

static void	t_memcmp(void)
{
	const unsigned char	a[4] = {1, 2, 3, 200};
	const unsigned char	b[4] = {1, 2, 3, 100};
	const unsigned char	c[4] = {1, 2, 9, 100};
	const unsigned char	z1[3] = {1, 0, 2};
	const unsigned char	z2[3] = {1, 0, 3};

	check("equal, n = 4", ft_memcmp(a, a, 4) == 0);
	check("n = 0 returns 0", ft_memcmp(a, c, 0) == 0);
	check("compares as unsigned (200 > 100)", ft_memcmp(a, b, 4) > 0);
	check("opposite order is negative", ft_memcmp(b, a, 4) < 0);
	check("difference before n", sign(ft_memcmp(a, c, 4)) == sign(memcmp(a, c, 4)));
	check("difference after n is ignored", ft_memcmp(a, c, 2) == 0);
	check("does not stop at \\0", ft_memcmp(z1, z2, 3) < 0);
}

static void	t_strnstr(void)
{
	const char	*h = "hello world";
	const char	*aaabc = "aaabc";
	const char	*abc = "abc";

	check("found at the end", ft_strnstr(h, "world", 11) == h + 6);
	check("len too short to contain it", ft_strnstr(h, "world", 10) == NULL);
	check("empty little returns big", ft_strnstr(h, "", 5) == h);
	check("len = 0 with a non-empty little", ft_strnstr(h, "h", 0) == NULL);
	check("little longer than big", ft_strnstr("abc", "abcd", 10) == NULL);
	check("partial match first", ft_strnstr(aaabc, "aab", 5) == aaabc + 1);
	check("one char at the end", ft_strnstr("abc", "c", 3) == abc + 2);
	check("one char cut by len", ft_strnstr("abc", "c", 2) == NULL);
	check("not found", ft_strnstr(h, "xyz", 11) == NULL);
	check("empty big, empty little", ft_strnstr("", "", 5) != NULL);
}

static void	t_atoi(void)
{
	const char	*t[] = {"42", "-42", "+42", "   42", "\t\n\v\f\r 42abc", "abc",
		"", "--5", "+-5", "2147483647", "-2147483648", "0", "  -0", "12 34",
		"- 5", "00012", NULL};
	int			i;
	int			j;
	char		name[64];

	i = -1;
	while (t[++i])
	{
		snprintf(name, sizeof(name), "\"%.30s\"", t[i]);
		j = -1;
		while (name[++j])
			if (name[j] < 32)
				name[j] = '.';
		check_int(name, ft_atoi(t[i]), atoi(t[i]));
	}
}

static void	t_calloc(void)
{
	int				*p;
	void			*q;
	size_t			i;
	int				ok;
	volatile size_t	big;

	p = ft_calloc(10, sizeof(int));
	check("returns a pointer", p != NULL);
	ok = (p != NULL);
	i = 0;
	while (p && i < 10)
		if (p[i++] != 0)
			ok = 0;
	check("memory is zeroed", ok);
	if (p)
		p[9] = 42;
	free(p);
	q = ft_calloc(0, 0);
	check("calloc(0, 0) returns a pointer free() accepts", q != NULL);
	free(q);
	q = ft_calloc(0, 5);
	check("calloc(0, 5) returns a pointer free() accepts", q != NULL);
	free(q);
	q = ft_calloc(5, 0);
	check("calloc(5, 0) returns a pointer free() accepts", q != NULL);
	free(q);
	big = SIZE_MAX;
	q = ft_calloc(big, 2);
	check("overflow returns NULL (extra)", q == NULL);
	free(q);
}

static void	t_strdup(void)
{
	const char	*s = "hello";
	char		*d;

	d = ft_strdup(s);
	check_str("copy is equal", d, "hello");
	check("copy is a different pointer", d != s);
	free(d);
	d = ft_strdup("");
	check_str("empty string", d, "");
	free(d);
	d = ft_strdup("with spaces and\ttab");
	check_str("spaces and tab", d, "with spaces and\ttab");
	free(d);
}

/* ------------------------- part 2: additional ---------------------------- */

static void	sub(const char *name, const char *s, unsigned int st, size_t len,
		const char *exp)
{
	char	*r;

	r = ft_substr(s, st, len);
	check_str(name, r, exp);
	free(r);
}

static void	t_substr(void)
{
	char	*r;

	sub("middle", "hello world", 6, 5, "world");
	sub("from the start", "hello world", 0, 5, "hello");
	sub("len larger than the rest", "hello", 2, 100, "llo");
	sub("len = 0", "hello", 1, 0, "");
	sub("start == strlen", "hello", 5, 3, "");
	sub("start past the end", "hello", 50, 3, "");
	sub("empty string", "", 0, 5, "");
	sub("whole string", "hello", 0, 5, "hello");
	r = ft_substr(NULL, 0, 3);
	check("NULL s returns NULL (extra)", r == NULL);
	free(r);
}

static void	join(const char *name, const char *a, const char *b, const char *exp)
{
	char	*r;

	r = ft_strjoin(a, b);
	check_str(name, r, exp);
	free(r);
}

static void	t_strjoin(void)
{
	char	*r;

	join("two words", "foo", "bar", "foobar");
	join("empty s1", "", "bar", "bar");
	join("empty s2", "foo", "", "foo");
	join("both empty", "", "", "");
	join("spaces kept", "a ", " b", "a  b");
	r = ft_strjoin(NULL, "a");
	check("NULL s1 returns NULL (extra)", r == NULL);
	free(r);
}

static void	trim(const char *name, const char *s, const char *set, const char *exp)
{
	char	*r;

	r = ft_strtrim(s, set);
	check_str(name, r, exp);
	free(r);
}

static void	t_strtrim(void)
{
	char	*r;

	trim("spaces both sides", "  hello  ", " ", "hello");
	trim("one char both sides", "xxhixx", "x", "hi");
	trim("empty set", "abc", "", "abc");
	trim("everything is trimmed", "xxx", "x", "");
	trim("middle is kept", "  a b  ", " ", "a b");
	trim("several chars in set", "abcHELLOcba", "abc", "HELLO");
	trim("empty s1", "", "x", "");
	trim("nothing to trim", "hello", "xyz", "hello");
	trim("only start", "  hello", " ", "hello");
	trim("only end", "hello  ", " ", "hello");
	r = ft_strtrim(NULL, "a");
	check("NULL s1 returns NULL (extra)", r == NULL);
	free(r);
}

static int	split_is(const char *s, char c, const char **exp)
{
	char	**r;
	int		i;
	int		ok;

	r = ft_split(s, c);
	if (!r)
		return (0);
	ok = 1;
	i = 0;
	while (exp[i])
	{
		if (!r[i] || strcmp(r[i], exp[i]) != 0)
		{
			ok = 0;
			break ;
		}
		i++;
	}
	if (ok && r[i] != NULL)
		ok = 0;
	i = 0;
	while (r[i])
		free(r[i++]);
	free(r);
	return (ok);
}

static void	t_split(void)
{
	const char	*e1[] = {"a", "b", "c", NULL};
	const char	*e2[] = {"hello", "world", NULL};
	const char	*e3[] = {NULL};
	const char	*e4[] = {"abc", NULL};
	const char	*e5[] = {"only", NULL};
	const char	*e6[] = {"a", "b", "c", "d", NULL};

	check("\"a b c\"", split_is("a b c", ' ', e1));
	check("leading, trailing and repeated delimiters",
		split_is("  hello   world  ", ' ', e2));
	check("empty string gives an empty NULL-terminated array",
		split_is("", ' ', e3));
	check("only delimiters", split_is("     ", ' ', e3));
	check("delimiter not found", split_is("abc", 'x', e4));
	check("no delimiter at all", split_is("only", ' ', e5));
	check("other delimiter, empty fields skipped", split_is("a,b,,c,", ',', e1));
	check("single chars", split_is("a-b-c-d", '-', e6));
	check("delimiter '\\0' returns the whole string", split_is("abc", '\0', e4));
}

static void	itoa_is(int n, const char *exp)
{
	char	*r;
	char	name[32];

	r = ft_itoa(n);
	snprintf(name, sizeof(name), "%d", n);
	check_str(name, r, exp);
	free(r);
}

static void	t_itoa(void)
{
	itoa_is(0, "0");
	itoa_is(1, "1");
	itoa_is(-1, "-1");
	itoa_is(9, "9");
	itoa_is(10, "10");
	itoa_is(-10, "-10");
	itoa_is(42, "42");
	itoa_is(-42, "-42");
	itoa_is(100, "100");
	itoa_is(123456, "123456");
	itoa_is(INT_MAX, "2147483647");
	itoa_is(INT_MIN, "-2147483648");
}

static char	shift_idx(unsigned int i, char c)
{
	return ((char)(c + i));
}

static char	up_idx(unsigned int i, char c)
{
	(void)i;
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}

static void	t_strmapi(void)
{
	char	*r;
	char	src[8];

	r = ft_strmapi("aaa", shift_idx);
	check_str("index is passed as first argument", r, "abc");
	free(r);
	r = ft_strmapi("hello world", up_idx);
	check_str("uppercase", r, "HELLO WORLD");
	free(r);
	r = ft_strmapi("", up_idx);
	check_str("empty string gives an allocated empty string", r, "");
	free(r);
	strcpy(src, "abc");
	r = ft_strmapi(src, up_idx);
	check_str("source string is not changed", src, "abc");
	check("result is a new pointer", r != src);
	free(r);
	r = ft_strmapi(NULL, up_idx);
	check("NULL s returns NULL (extra)", r == NULL);
	free(r);
	r = ft_strmapi("abc", NULL);
	check("NULL f returns NULL (extra)", r == NULL);
	free(r);
}

static void	add_idx(unsigned int i, char *c)
{
	*c = (char)(*c + i);
}

static void	up_ptr(unsigned int i, char *c)
{
	(void)i;
	if (*c >= 'a' && *c <= 'z')
		*c -= 32;
}

static void	t_striteri(void)
{
	char	a[8];
	char	e[1];

	strcpy(a, "aaa");
	ft_striteri(a, add_idx);
	check_str("index is passed as first argument", a, "abc");
	strcpy(a, "hello");
	ft_striteri(a, up_ptr);
	check_str("string is changed in place", a, "HELLO");
	e[0] = '\0';
	ft_striteri(e, up_ptr);
	check("empty string: nothing happens", e[0] == '\0');
	ft_striteri(NULL, up_ptr);
	check("NULL s: no crash (extra)", 1);
	strcpy(a, "abc");
	ft_striteri(a, NULL);
	check("NULL f: no crash (extra)", 1);
}

static char	*cap_str(void (*f)(char *, int), char *s)
{
	static char	buf[256];
	int			p[2];
	ssize_t		n;

	if (pipe(p) < 0)
		return (NULL);
	f(s, p[1]);
	close(p[1]);
	n = read(p[0], buf, sizeof(buf) - 1);
	close(p[0]);
	if (n < 0)
		n = 0;
	buf[n] = '\0';
	return (buf);
}

static char	*cap_chr(char c)
{
	static char	buf[8];
	int			p[2];
	ssize_t		n;

	if (pipe(p) < 0)
		return (NULL);
	ft_putchar_fd(c, p[1]);
	close(p[1]);
	n = read(p[0], buf, sizeof(buf) - 1);
	close(p[0]);
	if (n < 0)
		n = 0;
	buf[n] = '\0';
	return (buf);
}

static char	*cap_nbr(int v)
{
	static char	buf[32];
	int			p[2];
	ssize_t		n;

	if (pipe(p) < 0)
		return (NULL);
	ft_putnbr_fd(v, p[1]);
	close(p[1]);
	n = read(p[0], buf, sizeof(buf) - 1);
	close(p[0]);
	if (n < 0)
		n = 0;
	buf[n] = '\0';
	return (buf);
}

static void	t_putchar_fd(void)
{
	check_str("'A'", cap_chr('A'), "A");
	check_str("'\\n'", cap_chr('\n'), "\n");
	ft_putchar_fd('a', -1);
	check("invalid fd: no crash", 1);
}

static void	t_putstr_fd(void)
{
	check_str("hello", cap_str(ft_putstr_fd, "hello"), "hello");
	check_str("empty string", cap_str(ft_putstr_fd, ""), "");
	check_str("with spaces and \\n", cap_str(ft_putstr_fd, "a b\nc"), "a b\nc");
	ft_putstr_fd("x", -1);
	check("invalid fd: no crash", 1);
	cap_str(ft_putstr_fd, NULL);
	check("NULL s: no crash (extra)", 1);
}

static void	t_putendl_fd(void)
{
	check_str("hi", cap_str(ft_putendl_fd, "hi"), "hi\n");
	check_str("empty string prints only a newline", cap_str(ft_putendl_fd, ""),
		"\n");
	cap_str(ft_putendl_fd, NULL);
	check("NULL s: no crash (extra)", 1);
}

static void	t_putnbr_fd(void)
{
	check_str("0", cap_nbr(0), "0");
	check_str("42", cap_nbr(42), "42");
	check_str("-42", cap_nbr(-42), "-42");
	check_str("10", cap_nbr(10), "10");
	check_str("INT_MAX", cap_nbr(INT_MAX), "2147483647");
	check_str("INT_MIN", cap_nbr(INT_MIN), "-2147483648");
}

/* --------------------------- part 3: linked list ------------------------- */

static void	count_del(void *content)
{
	g_freed++;
	free(content);
}

static void	upper_first(void *content)
{
	char	*s;

	s = content;
	if (s[0] >= 'a' && s[0] <= 'z')
		s[0] -= 32;
}

static void	*dup_upper(void *content)
{
	char	*s;

	s = strdup(content);
	if (s && s[0] >= 'a' && s[0] <= 'z')
		s[0] -= 32;
	return (s);
}

static t_list	*node(const char *str)
{
	return (ft_lstnew(strdup(str)));
}

static void	t_lstnew(void)
{
	t_list	*n;
	int		x;

	x = 42;
	n = ft_lstnew(&x);
	check("node is created", n != NULL);
	check("content is the given pointer", n && n->content == &x);
	check("next is NULL", n && n->next == NULL);
	free(n);
	n = ft_lstnew(NULL);
	check("NULL content is allowed", n && n->content == NULL);
	free(n);
}

static void	t_lstadd_front(void)
{
	t_list	*lst;
	t_list	*a;
	t_list	*b;

	lst = NULL;
	a = node("a");
	b = node("b");
	ft_lstadd_front(&lst, a);
	check("empty list: head is a", lst == a);
	check("empty list: a->next is NULL", a->next == NULL);
	ft_lstadd_front(&lst, b);
	check("head is now b", lst == b);
	check("b->next is a", b->next == a);
	ft_lstadd_front(&lst, NULL);
	check("new is NULL: list unchanged", lst == b);
	ft_lstadd_front(NULL, a);
	check("lst is NULL: no crash", 1);
	ft_lstclear(&lst, free);
}

static void	t_lstsize(void)
{
	t_list	*lst;

	lst = NULL;
	check_int("NULL list is 0", ft_lstsize(lst), 0);
	ft_lstadd_front(&lst, node("a"));
	check_int("one node", ft_lstsize(lst), 1);
	ft_lstadd_front(&lst, node("b"));
	ft_lstadd_front(&lst, node("c"));
	check_int("three nodes", ft_lstsize(lst), 3);
	ft_lstclear(&lst, free);
}

static void	t_lstlast(void)
{
	t_list	*lst;
	t_list	*a;
	t_list	*b;

	check("NULL list returns NULL", ft_lstlast(NULL) == NULL);
	a = node("a");
	b = node("b");
	lst = a;
	check("one node returns that node", ft_lstlast(lst) == a);
	a->next = b;
	check("two nodes returns the second", ft_lstlast(lst) == b);
	check("list is not changed", lst == a && a->next == b && b->next == NULL);
	ft_lstclear(&lst, free);
}

static void	t_lstadd_back(void)
{
	t_list	*lst;
	t_list	*a;
	t_list	*b;
	t_list	*c;

	lst = NULL;
	a = node("a");
	b = node("b");
	c = node("c");
	ft_lstadd_back(&lst, a);
	check("empty list: head is a", lst == a);
	ft_lstadd_back(&lst, b);
	check("a->next is b", a->next == b);
	ft_lstadd_back(&lst, c);
	check("b->next is c", b->next == c);
	check("head did not change", lst == a);
	check_int("size is 3", ft_lstsize(lst), 3);
	ft_lstadd_back(&lst, NULL);
	check_int("new is NULL: size still 3", ft_lstsize(lst), 3);
	ft_lstadd_back(NULL, c);
	check("lst is NULL: no crash", 1);
	ft_lstclear(&lst, free);
}

static void	t_lstdelone(void)
{
	t_list	*a;
	t_list	*b;

	a = node("a");
	b = node("b");
	a->next = b;
	g_freed = 0;
	ft_lstdelone(a, count_del);
	check_int("del called once", g_freed, 1);
	check("next node is still usable", strcmp(b->content, "b") == 0);
	ft_lstdelone(NULL, count_del);
	check_int("lst is NULL: no crash, del not called", g_freed, 1);
	ft_lstdelone(b, count_del);
	check_int("second node deleted", g_freed, 2);
}

static void	t_lstclear(void)
{
	t_list	*lst;

	lst = node("a");
	ft_lstadd_back(&lst, node("b"));
	ft_lstadd_back(&lst, node("c"));
	g_freed = 0;
	ft_lstclear(&lst, count_del);
	check_int("del called for each node", g_freed, 3);
	check("list pointer is NULL", lst == NULL);
	ft_lstclear(&lst, count_del);
	check_int("empty list: del not called", g_freed, 3);
	ft_lstclear(NULL, count_del);
	check("lst is NULL: no crash", 1);
}

static void	t_lstiter(void)
{
	t_list	*lst;

	lst = node("apple");
	ft_lstadd_back(&lst, node("banana"));
	ft_lstadd_back(&lst, node("cherry"));
	ft_lstiter(lst, upper_first);
	check_str("first node", lst->content, "Apple");
	check_str("second node", lst->next->content, "Banana");
	check_str("third node", lst->next->next->content, "Cherry");
	ft_lstiter(NULL, upper_first);
	check("NULL list: no crash", 1);
	ft_lstclear(&lst, free);
}

static void	t_lstmap(void)
{
	t_list	*lst;
	t_list	*res;

	lst = node("apple");
	ft_lstadd_back(&lst, node("banana"));
	ft_lstadd_back(&lst, node("cherry"));
	res = ft_lstmap(lst, dup_upper, free);
	check("new list is created", res != NULL);
	check_int("new list size", ft_lstsize(res), 3);
	check_str("first content", res ? res->content : NULL, "Apple");
	check_str("second content", res ? res->next->content : NULL, "Banana");
	check_str("third content", res ? res->next->next->content : NULL, "Cherry");
	check("new nodes are different from old ones", res != lst);
	check_str("old list is not changed", lst->content, "apple");
	check("NULL list returns NULL", ft_lstmap(NULL, dup_upper, free) == NULL);
	ft_lstclear(&res, free);
	ft_lstclear(&lst, free);
}

/* -------------------------------- runner --------------------------------- */

typedef struct s_case
{
	const char	*name;
	void		(*fn)(void);
}	t_case;

static int	selected(const char *name, int ac, char **av)
{
	int	i;

	if (ac < 2)
		return (1);
	i = 1;
	while (i < ac)
	{
		if (strstr(name, av[i]))
			return (1);
		i++;
	}
	return (0);
}

static void	run(const t_case *c)
{
	pid_t	pid;
	int		st;

	printf("\n== %s ==\n", c->name);
	pid = fork();
	if (pid < 0)
	{
		printf("  fork failed\n");
		return ;
	}
	if (pid == 0)
	{
		alarm(5);
		c->fn();
		_exit(0);
	}
	waitpid(pid, &st, 0);
	if (WIFSIGNALED(st))
	{
		g_stats[2]++;
		if (WTERMSIG(st) == SIGALRM)
			printf("  [CRASH] timeout after 5s (endless loop?)\n");
		else
			printf("  [CRASH] killed by signal %d (%s)\n", WTERMSIG(st),
				strsignal(WTERMSIG(st)));
	}
}

int	main(int ac, char **av)
{
	static const t_case	cases[] = {
	{"ft_isalpha", t_isalpha}, {"ft_isdigit", t_isdigit},
	{"ft_isalnum", t_isalnum}, {"ft_isascii", t_isascii},
	{"ft_isprint", t_isprint}, {"ft_strlen", t_strlen},
	{"ft_memset", t_memset}, {"ft_bzero", t_bzero},
	{"ft_memcpy", t_memcpy}, {"ft_memmove", t_memmove},
	{"ft_strlcpy", t_strlcpy}, {"ft_strlcat", t_strlcat},
	{"ft_toupper", t_toupper}, {"ft_tolower", t_tolower},
	{"ft_strchr", t_strchr}, {"ft_strrchr", t_strrchr},
	{"ft_strncmp", t_strncmp}, {"ft_memchr", t_memchr},
	{"ft_memcmp", t_memcmp}, {"ft_strnstr", t_strnstr},
	{"ft_atoi", t_atoi}, {"ft_calloc", t_calloc}, {"ft_strdup", t_strdup},
	{"ft_substr", t_substr}, {"ft_strjoin", t_strjoin},
	{"ft_strtrim", t_strtrim}, {"ft_split", t_split}, {"ft_itoa", t_itoa},
	{"ft_strmapi", t_strmapi}, {"ft_striteri", t_striteri},
	{"ft_putchar_fd", t_putchar_fd}, {"ft_putstr_fd", t_putstr_fd},
	{"ft_putendl_fd", t_putendl_fd}, {"ft_putnbr_fd", t_putnbr_fd},
	{"ft_lstnew", t_lstnew}, {"ft_lstadd_front", t_lstadd_front},
	{"ft_lstsize", t_lstsize}, {"ft_lstlast", t_lstlast},
	{"ft_lstadd_back", t_lstadd_back}, {"ft_lstdelone", t_lstdelone},
	{"ft_lstclear", t_lstclear}, {"ft_lstiter", t_lstiter},
	{"ft_lstmap", t_lstmap}};
	size_t				i;

	setvbuf(stdout, NULL, _IONBF, 0);
	g_stats = mmap(NULL, 3 * sizeof(int), PROT_READ | PROT_WRITE,
			MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (g_stats == MAP_FAILED)
		return (1);
	g_stats[0] = 0;
	g_stats[1] = 0;
	g_stats[2] = 0;
	i = 0;
	while (i < sizeof(cases) / sizeof(cases[0]))
	{
		if (selected(cases[i].name, ac, av))
			run(&cases[i]);
		i++;
	}
	printf("\n==== Result: %d passed, %d failed, %d crashed ====\n",
		g_stats[0], g_stats[1], g_stats[2]);
	return (g_stats[1] != 0 || g_stats[2] != 0);
}
