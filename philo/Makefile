SRC =	main.c			\
		forks.c			\
		parsing.c		\
		utils.c			\
		philos.c		\
		tasks.c			\
		time.c			\
		tasks_utils.c 	\


OBJ = $(addprefix objs/, $(SRC:.c=.o))

TARGET = philo
OBJDIR = objs

CFLAGS = -Wall -Werror -Wextra -fsanitize=thread -g
# -fsanitize=address -g
# -fsanitize=thread -g

$(TARGET): $(OBJ)
	@$(CC) $(OBJ) $(CFLAGS) -pthread -o $(TARGET)

$(OBJDIR)/%.o: %.c
	@mkdir -p objs
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	rm -f $(TARGET)

re: clean all

all: $(TARGET)
