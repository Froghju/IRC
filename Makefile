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

SRCS = main.cpp client.cpp server.cpp

PLUS =	init_env.c clean_fd.c get_opt.c x.c main_loop.c \
	init_fd.c do_select.c check_fd.c \
	srv_create.c srv_accept.c \
	client_read.c client_write.c

OBJS = ${SRCS:.c=.o}

NAME = ircserv

CFLAGS = -I. -g3 -Wall -Werror -Wextra -

CC = c++
RM = rm -f

${NAME}:	${OBJS}
		${CC} -o ${NAME} ${OBJS}

all:		${NAME}

clean:
		@echo Cleaning ojects
		${RM} ${OBJS}

fclean:		clean
		@echo Cleaning project
		${RM} ${NAME}

re:		fclean all
