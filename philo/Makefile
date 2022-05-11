# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: rvan-mee <rvan-mee@student.42.fr>            +#+                      #
#                                                    +#+                       #
#    Created: 2022/05/11 18:10:38 by rvan-mee      #+#    #+#                  #
#    Updated: 2022/05/11 18:12:29 by rvan-mee      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

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
SRCDIR = src
INCLUDE_DIR = include

INC := -I $(INCLUDE_DIR)

CFLAGS = -Wall -Werror -Wextra $(INC)
# -fsanitize=address -g
# -fsanitize=thread -g

$(TARGET): $(OBJ)
	@$(CC) $(OBJ) $(CFLAGS) -pthread -o $(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run:
	./$(TARGET) 5 800 200 200

clean:
	rm -f $(OBJ)
	rm -f $(TARGET)

re: clean all

all: $(TARGET)
