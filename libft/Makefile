NAME = libft.a
SO_NAME = libft.so

SOURCES =   ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c \
    ft_isascii.c ft_isdigit.c ft_isprint.c ft_isspace.c ft_itoa.c \
    ft_itoa_base.c ft_memccpy_extra.c ft_memchr.c ft_memcmp.c \
    ft_memcpy.c ft_memdup.c ft_memjoin.c ft_memmove.c ft_memset.c \
    ft_printf.c ft_putchar.c ft_putchar_fd.c \
    ft_putchar_utf_extra.c ft_putendl_fd.c ft_putendl_extra.c \
    ft_putmem.c ft_putnbr.c ft_putnbr_fd.c \
    ft_putstr_fd.c ft_putstr_extra.c ft_split.c \
    ft_strdup.c ft_strcat_extra.c ft_strchr.c ft_strjoin.c \
    ft_strlcat.c ft_strlcpy.c ft_strmapi.c ft_strncat_extra.c \
    ft_strncmp.c ft_strncpy_extra.c ft_strnlen_extra.c ft_strnstr.c \
    ft_strrchr.c ft_strscpy_extra.c ft_strtrim.c ft_substr.c \
    ft_tolower.c ft_toupper.c flags.c flags2.c formats.c \
    utils.c utils2.c ft_lstadd_back_bonus.c ft_lstadd_front_bonus.c \
    ft_lstclear_bonus.c ft_lstdelone_bonus.c ft_lstiter_bonus.c \
    ft_lstlast_bonus.c ft_lstmap_bonus.c ft_lstnew_bonus.c \
    ft_lstsize_bonus.c ft_strlen.c

OBJECTS = $(SOURCES:.c=.o)

# Color codes
YELLOW = \033[33m
GREEN = \033[32m
RESET = \033[0m
BOLD = \033[1m

# Compiler and flags
AR = ar -rvcs
CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -g

# Targets
all: $(NAME)

# Rule to compile .c files to .o files
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@ 

# Rule to create the static library
$(NAME): $(OBJECTS)
	@echo "Creating $(NAME) archive..."
	@if ! $(AR) $@ $^ >> error.txt 2>&1; then \
		echo "$(RED)$(BOLD)MAKEFILE TERMINATED!$(RESET)"; \
		echo "$(YELLOW)Error creating $(NAME)$(RESET)"; \
		echo "\n\n$(RED)$(BOLD)ERROR$(RESET)"; \
		sed '$$d' error.txt; \
		echo "\n\n$(YELLOW)EXITING$(RESET)"; \
		exit 1; \
	fi
	@ranlib $@
	@echo "$(GREEN)$(BOLD)SUCCESS$(RESET)"
	@echo "$(YELLOW)Created: $(words $(OBJECTS)) object file(s)$(RESET)"
	@echo "$(YELLOW)Created: $(NAME)$(RESET)"
	
# Rule to create the shared library
$(SO_NAME): $(OBJECTS)
	$(CC) -fPIC $(CFLAGS) -shared -o $(SO_NAME) $(OBJECTS)
	@echo "$(GREEN)$(BOLD)SUCCESS$(RESET)"
	@echo "$(YELLOW)Created: $(SO_NAME)$(RESET)"

clean:
	@if [ -n "$$(ls -A $(OBJECTS)  2>/dev/null)" ]; then \
		$(RM) $(OBJECTS); \
		echo "$(GREEN)$(BOLD)SUCCESS$(RESET)"; \
		echo "$(YELLOW) Deleted: $(words $(OBJECTS) ) object file(s)$(RESET)"; \
	fi

fclean: clean
	@if [ -n "$$(ls -A $(NAME) 2>/dev/null)" ]; then \
		$(RM) $(NAME); \
		echo "$(GREEN)$(BOLD)SUCCESS$(RESET)"; \
		echo "$(YELLOW)Deleted: $(NAME)$(RESET)"; \
	fi
	@if [ -n "$$(ls -A $(SO_NAME) 2>/dev/null)" ]; then \
		$(RM) $(SO_NAME); \
		echo "$(GREEN)$(BOLD)SUCCESS$(RESET)"; \
		echo "$(YELLOW)Deleted: $(SO_NAME)$(RESET)"; \
	fi

# Rebuild everything
re: fclean all

# Ensure shared library is built
so: $(SO_NAME)

.PHONY: all clean fclean re so
