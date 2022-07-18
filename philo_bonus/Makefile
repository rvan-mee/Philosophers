SRC =	main.c			\
		parsing.c		\
		error.c			\
		signals.c		\
		tasks.c			\
		time.c			\
		semaphore.c		\
		init.c			\
		philo.c			\

OBJ = $(addprefix objs/, $(SRC:.c=.o))

TARGET = philo_bonus
OBJDIR = objs
SRCDIR = src
INCLUDE_DIR = include

INC := -I $(INCLUDE_DIR)

CFLAGS = -Wall -Werror -Wextra $(INC) -fsanitize=thread -g
# -fsanitize=address -g
# -fsanitize=thread -g

$(TARGET): $(OBJ)
	@$(CC) $(OBJ) $(CFLAGS) -pthread -o $(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(TARGET)

run:
	./$(TARGET) 5 800 200 200

clean:
	@echo Cleaning Object Files
	@rm -f $(OBJ)

fclean: clean
	@echo Cleaning All Remaining Files
	@rm -f $(TARGET)
	@rm -rf objs

re: fclean all

.PHONY: all clean fclean re bonus