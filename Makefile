#***********************************************#
#	 _____ _                 ___ _      _ 		#
#	/__   \ |__   __ _      / _ \ |__  (_)		#
#	  / /\/ '_ \ / _` |    / /_\/ '_ \ | |		#
#	 / /  | | | | (_| |   / /_\\| | | || |		#
#	 \/   |_| |_|\__,_|___\____/|_| |_|/ |		#
#	                 |_____|         |__/ 		#
#	            _____  __    ___          		#
#	            \_   \/__\  / __\         		#
#	             / /\/ \// / /            		#
#	          /\/ /_/ _  \/ /___          		#
#	          \____/\/ \_/\____/          		#
#												#
#***********************************************#

SRCS = srcs/main.cpp srcs/client.cpp srcs/server.cpp \
		srcs/utils.cpp srcs/frogy.cpp srcs/parse.cpp srcs/channel.cpp \
		srcs/dataBot.cpp

PLUS =	init_env.c clean_fd.c get_opt.c x.c main_loop.c \
	init_fd.c do_select.c check_fd.c \
	srv_create.c srv_accept.c \
	client_read.c client_write.c

OBJS = ${SRCS:.cpp=.o}

NAME = ircserv

CFLAGS = -I. -g3 -Wall -Werror -Wextra -std=c++98 -g #-fsanitize=address,undefined -g3

CC = c++
RM = rm -f

all: ${NAME}

${NAME}: ${OBJS}
	${CC} ${CFLAGS} -o $(NAME) $(OBJS)

%.o: %.cpp
	${CC} $(CFLAGS) -c -o $@ $<

clean:
		@echo Cleaning ojects
		${RM} ${OBJS}

fclean:		clean
		@echo Cleaning project
		${RM} ${NAME}

re:		fclean all

.PHONY: all clean fclean re
