# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: rvan-mee <rvan-mee@student.42.fr>            +#+                      #
#                                                    +#+                       #
#    Created: 2022/07/26 14:41:28 by rvan-mee      #+#    #+#                  #
#    Updated: 2022/07/26 14:42:05 by rvan-mee      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

SRC =	main.c			\
		parsing.c		\
		error.c			\
		signals.c		\
		tasks.c			\
		time.c			\
		semaphore.c		\
		init.c			\
		philo.c			\

TARGET = philo_bonus
OBJDIR = objs
SRCDIR = src
INCLUDE_DIR = include

INC := -I $(INCLUDE_DIR)

OBJ = $(addprefix objs/, $(SRC:.c=.o))

CFLAGS = -Wall -Werror -Wextra -Ofast $(INC)
# -fsanitize=address -g
# -fsanitize=thread -g

$(TARGET): $(OBJ)
	@$(CC) $(OBJ) $(CFLAGS) -pthread -o $(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDE_DIR)/philo_bonus.h
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
	@rm -rf $(OBJDIR)

re: fclean all

.PHONY: all clean fclean re bonus