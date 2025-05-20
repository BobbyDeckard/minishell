/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 17:41:20 by imeulema          #+#    #+#             */
/*   Updated: 2024/11/29 11:13:04 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

int				ft_abs(int i);
int				ft_atoi(const char *str);
int				ft_isalnum(int c);
int				ft_isalpha(int c);
int				ft_isascii(int c);
int				ft_isdigit(int c);
int				ft_isprint(int c);
int				ft_memcmp(const void *s1, const void *s2, size_t n);
int				ft_printf(const char *str, ...);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
int				ft_tolower(int c);
int				ft_toupper(int c);
int				line_complete(char *str);
char			*ft_itoa(int n);
char			*ft_strchr(const char *s, int c);
char			*ft_strdup(const char *s1);
char			*ft_strjoin(const char *s1, const char *s2);
char			*ft_strmapi(const char *s, char (*f)(unsigned int, char));
char			*ft_strnstr(const char *haystack, const char *needle,
					size_t len);
char			*ft_strtrim(const char *s1, const char *set);
char			*ft_strrchr(const char *s, int c);
char			*ft_substr(const char *s, unsigned int start, size_t len);
char			*free_null(char **str);
char			*get_next_line(int fd);
char			**ft_split(const char *s, char c);
long			ft_atol(const char *str);
void			address_conversion(va_list *args, int *ptr);
void			char_conversion(va_list *args, int *ptr);
void			dec_nbr_conversion(va_list *args, int *ptr);
void			ft_bzero(void *s, size_t n);
void			ft_putchar_fd(char c, int fd);
void			ft_putendl_fd(char *s, int fd);
void			ft_putnbr_fd(int n, int fd);
void			ft_putstr_fd(char *s, int fd);
void			ft_striteri(char *s, void (*f)(unsigned int, char *));
void			*ft_calloc(size_t count, size_t size);
void			*ft_memchr(const void *s, int c, size_t n);
void			*ft_memcpy(void *dst, const void *src, size_t n);
void			*ft_memmove(void *dst, const void *src, size_t len);
void			*ft_memset(void *b, int c, size_t len);
void			hex_lo_conversion(va_list *args, int *ptr);
void			hex_up_conversion(va_list *args, int *ptr);
void			uns_conversion(unsigned int n, int *ptr);
void			string_conversion(va_list *args, int *ptr);
double			ft_atod(const char *str);
double			ft_dabs(double n);
size_t			ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t			ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t			ft_strlen(const char *s);
unsigned int	gnl_strlen(char *str);

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

int				ft_lstsize(t_list *lst);
void			ft_lstadd_back(t_list **lst, t_list *new);
void			ft_lstadd_front(t_list **lst, t_list *new);
void			ft_lstclear(t_list **lst, void (*del)(void *));
void			ft_lstdelone(t_list *lst, void (*del)(void *));
void			ft_lstiter(t_list *lst, void (*f)(void *));
t_list			*ft_lstlast(t_list *lst);
t_list			*ft_lstmap(t_list *lst, void *(*f)(void *),
					void (*del)(void *));
t_list			*ft_lstnew(void *content);

#endif
