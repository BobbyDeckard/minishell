NAME = libft.a
CC = cc
CFLAGS = -Wextra -Wall -Werror -g
SRCS = ft_abs.c \
       ft_atod.c \
       ft_atoi.c \
       ft_atol.c \
       ft_bzero.c \
       ft_calloc.c \
       ft_dabs.c \
       ft_isalnum.c \
       ft_isalpha.c \
       ft_isascii.c \
       ft_isdigit.c \
       ft_isprint.c \
       ft_itoa.c \
       ft_memchr.c \
       ft_memcmp.c \
       ft_memcpy.c \
       ft_memmove.c \
       ft_memset.c \
       ft_printf.c \
       ft_printf_srcs/address_conversion.c \
       ft_printf_srcs/char_conversion.c \
       ft_printf_srcs/dec_nbr_conversion.c \
       ft_printf_srcs/hex_lo_conversion.c \
       ft_printf_srcs/hex_up_conversion.c \
       ft_printf_srcs/string_conversion.c \
       ft_printf_srcs/uns_nbr_conversion.c \
       ft_putchar_fd.c \
       ft_putendl_fd.c \
       ft_putnbr_fd.c \
       ft_putstr_fd.c \
       ft_split.c \
       ft_strchr.c \
       ft_strdup.c \
       ft_striteri.c \
       ft_strjoin.c \
       ft_strlcat.c \
       ft_strlcpy.c \
       ft_strlen.c \
       ft_strmapi.c \
       ft_strncmp.c \
       ft_strnstr.c \
       ft_strrchr.c \
       ft_strtrim.c \
       ft_substr.c \
       ft_tolower.c \
       ft_toupper.c \
       get_next_line.c \
       get_next_line_utils.c
OBJS = $(SRCS:.c=.o)
B_SRCS = ft_lstadd_back_bonus.c \
	 ft_lstadd_front_bonus.c \
	 ft_lstclear_bonus.c \
	 ft_lstdelone_bonus.c \
	 ft_lstiter_bonus.c \
	 ft_lstlast_bonus.c \
	 ft_lstmap_bonus.c \
	 ft_lstnew_bonus.c \
	 ft_lstsize_bonus.c
B_OBJS = $(B_SRCS:.c=.o)
HEADERS = libft.h

all: $(NAME)

$(NAME): $(OBJS)
	ar -cr $(NAME) $?

bonus: $(OBJS) $(B_OBJS)
	ar -cr $(NAME) $?
	
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(B_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus

.SILENT:
