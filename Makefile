NAME = pipex
SOURCES = pipex.c \

BSOURCES =	get_next_line_bonus.c \

YELLOW = \033[33m
GREEN = \033[32m
RESET = \033[0m
BOLD = \033[1m

OBJECTS = $(SOURCES:.c=.o)
BOBJECTS = $(BSOURCES:.c=.o)

AR = ar
CC = cc
CFLAGS = -Wall -Wextra -Werror
LIB = ranlib

all: $(NAME)

$(NAME): $(OBJECTS)
	@echo "Creating $(NAME) archive..."
	$(AR) -rvcs $@ $?  # ar update, create, update index
	$(LIB) $(NAME)
	@echo "$(GREEN)$(BOLD)SUCCESS$(RESET)"
	@echo "$(YELLOW)Created: $(words $(OBJECTS) ) object file(s)$(RESET)"
	@echo "$(YELLOW)Created: $(NAME)$(RESET)"

#bonus: .bonus_made

.bonus_made: $(OBJECTS) $(BOBJECTS)
	@echo "Creating $(NAME) archive..."
	$(AR) -rvcs $(NAME) $?
	-@touch .bonus_made
	@echo "$(GREEN)$(BOLD)SUCCESS$(RESET)"
	@echo "$(YELLOW)Created: $(words $(OBJECTS) $(BOBJECTS)) object file(s)$(RESET)"
	@echo "$(YELLOW)Created: $(NAME)$(RESET)"

%.o: %.c
	$(CC) -c $(CFLAGS) $? #Initial compilation of .c files

clean:
	rm -f $(OBJECTS) $(BOBJECTS) 
	-@rm -f .bonus_made
	@echo "$(GREEN)$(BOLD)SUCCESS$(RESET)"
	@echo "$(YELLOW) Deleted: $(words $(OBJECTS) $(BOBJECTS) $(EOBJECTS)) object file(s)$(RESET)"

fclean: clean
	rm -f $(NAME)
	@echo "$(GREEN)SUCCESS$(RESET)"
	@echo "$(YELLOW) Deleted $(words $(NAME)) object files(s)$(RESET)"
	@echo "$(YELLOW) Deleted: $(NAME)"

re: fclean all

so:
	$(CC) -fPIC $(CFLAGS) -c $(SOURCES) $(BSOURCES)
	$(CC) -nostartfiles -shared -o libft.so $(OBJECTS) $(BOBJECTS)


.PHONY: all bonus clean fclean re so
