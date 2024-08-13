NAME = pipex
OUTPUT = executable

SOURCES = pipex.c doers.c utils.c utils2.c utils3.c string.c ft_splitsub.c 
          
# ft_memset.c ft_memcpy.c ft_memmove.c ft_strlen.c \
          ft_strlcat.c ft_strdup.c ft_strjoin.c ft_substr.c \
          ft_split.c ft_strnlen_extra.c ft_putchar_fd.c \
          ft_putstr_fd.c ft_strchr.c ft_isprint.c ft_strncmp.c \
          ft_strnstr.c ft_strtrim.c ft_splitsub.c ft_strrchr.c
BONUS_SOURCES = $(SOURCES)
BONUS_OBJECTS = $(BONUS_SOURCES:.c=.o)
OBJECTS = $(SOURCES:.c=.o)

# Colors
YELLOW = \033[33m
GREEN = \033[32m
RESET = \033[0m
BOLD = \033[1m

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -L$(LIB_DIR) -lft
LDFLAGS_SO = -L$(LIB_DIR) -lft -Wl,-rpath,$(LIB_DIR)

LIB_CPDIR = .
LIB_DIR = libft
LIB_NAME = libft.a
LIB_NAME_SO = libft.so
LIB_PATH = $(LIB_DIR)/$(LIB_NAME)
LIB_PATH_SO = $(LIB_DIR)/$(LIB_NAME_SO)

# Targets
all: $(NAME)

$(NAME): $(LIB_PATH) $(OBJECTS)
	@echo "Creating $(NAME) $(OUTPUT)..."
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)
	chmod +x $@
	@echo "$(GREEN)$(BOLD)SUCCESS$(RESET)"
	@echo "$(YELLOW)Created: $(words $(OBJECTS) ) object file(s)$(RESET)"
	@echo "$(YELLOW)Created: $(NAME)$(RESET)"

bonus: .bonus_made

.bonus_made: $(NAME)

#	@echo "Creating $(NAME) $(OUTPUT)..."
#	$(CC) $^ -o $@ $(CFLAGS)
#	chmod +x $@
#	-@touch .bonus_made
#	@echo "$(GREEN)$(BOLD)SUCCESS$(RESET)"
#	@echo "$(YELLOW)Created: $(words $(OBJECTS) $(BONUS_OBJECTS)) object file(s)$(RESET)"
#	@echo "$(YELLOW)Created: $(NAME)$(RESET)"

%.o: %.c
	$(CC) -c $(CFLAGS) $? -o $@ #Initial compilation of .c files

# make shared object lib
$(LIB_PATH):
	@$(MAKE) -C $(LIB_DIR)
	@echo "Copying $(LIB_NAME) to ../"
	@cp $(LIB_PATH) .

clean:
	rm -f $(OBJECTS) $(BONUS_OBJECTS) 
	@$(MAKE) -C $(LIB_DIR) clean
	rm -f $(LIB_NAME) # don't delete so!
	-@rm -f .bonus_made
	@echo "$(GREEN)$(BOLD)SUCCESS$(RESET)"
	@echo "$(YELLOW) Deleted: $(words $(OBJECTS) $(BONUS_OBJECTS)) object file(s)$(RESET)"

fclean: clean
	rm -f $(NAME)
	rm -f $(LIB_PATH)
	rm -f $(LIB_NAME)
	rm -f $(LIB_NAME_SO)
	@echo "$(GREEN)SUCCESS$(RESET)"
	@echo "$(YELLOW) Deleted $(words $(NAME)) object files(s)$(RESET)"
	@echo "$(YELLOW) Deleted: $(NAME)"

re: fclean all

so:

	$(CC) -fPIC $(CFLAGS) -c $(SOURCES) $(BONUS_SOURCES)
	$(CC) -nostartfiles -shared -o pipex.so $(OBJECTS) $(BONUS_OBJECTS)

.PHONY: all bonus clean fclean re so
