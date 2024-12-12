NAME	= ircserv
CC 		= c++
CPPFLAGS	= -Wall -Werror -Wextra -std=c++98
DFLAGS	= -MMD -MF $(@:.o=.d)
RM 		= rm -f
AUTHOR	= loadjou mnshimiy 
DATE	= $(shell date "+%d %B %T")

################################################################################
#                                 PROGRAM'S SRCS                               #
################################################################################


# Sources and objects
SERVER = Server
CHANNEL = Channel
CLIENT = Client
COMMAND = Commands



HEADERS = $(addsuffix .hpp, $(addprefix src/Server/, $(SERVER))) \
		  $(addsuffix .hpp, $(addprefix src/Channel/, $(CHANNEL))) \
		  $(addsuffix .hpp, $(addprefix src/Client/, $(CLIENT))) \
		  $(addsuffix .hpp, $(addprefix src/Commands/, $(COMMAND))) 
	    

SRCS = $(addsuffix .cpp, $(addprefix src/Server/, $(SERVER))) \
	   $(addsuffix .cpp, $(addprefix src/Channel/, $(CHANNEL))) \
	   $(addsuffix .cpp, $(addprefix src/Client/, $(CLIENT))) \
	   $(addsuffix .cpp, $(addprefix src/Commands/, $(COMMAND))) \
	   src/main.cpp src/utils.cpp


OBJS = $(SRCS:.cpp=.o)

################################################################################
#                                 Makefile rules                             #
################################################################################

all: header $(NAME)
	-@echo "BOOM 💥💥💥💥💥 $(NAME) Compiled! 💯 $(DEFAULT)"

$(NAME): $(OBJS)
	-@$(CC) $(CPPFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	-@$(CC) $(CPPFLAGS) -c $< -o $@

clean: cleaned
	-@$(RM) $(OBJS)

fclean: clean
	-@$(RM) $(NAME)
	

re: fclean all 

.PHONY: all clean fclean re header

#COLORS
RED = \033[1;31m
YELLOW = \033[1;33m
DEFAULT = \033[0m
COM_COLOR   = \033[0;34m
OBJ_COLOR   = \033[0;36m
OK_COLOR    = \033[0;32m
ERROR_COLOR = \033[0;31m
WARN_COLOR  = \033[0;33m
NO_COLOR    = \033[m

header:
	@printf "%b" "$(OK_COLOR)"
	@echo "        ___  _____ ___  ___      _         	"
	@echo "       /   |/ __  \|  \/  |     | |        	"
	@echo "      / /| |\' / /'| .  . | __ _| | _____ 	"
	@echo "     / /_| |  / /  | |\/| |/ _  | |/ / _ \\	"
	@echo "     \___  |./ /___| |  | | (_| |   <  __/	"
	@echo "         |_/\_____/\_|  |_/\__,_|_|\_\___| 	"
	@echo "         								  	"
	@echo "$(DEFAULT)"

cleaned:
	@printf "%b" "$(RED)"
	@echo "      ____   _      ______  ______ _   _ ______ _____  "
	@echo "     / ___| | |    |  ____||  __  | \\ | |  ____|  __ \\ "
	@echo "    | |     | |    | |__   | |  | |  \\| | |__  | |  | |"
	@echo "    | |     | |    |  __|  | |__| | .   |  __| | |  | |"
	@echo "    | |___  | |____| |____ | |  | | |\\  | |____| |__| |"
	@echo "     \\____| |______|______||_|  |_|_| \\_|______|_____/ "
	@echo "                                                      "
	@echo "$(DEFAULT)"


ifneq ($(HASH),)
	@printf "%b" "$(DEFAULT)Name:	$(OBJ_COLOR)$(NAME)@$(HASH)\n"
else
	@printf "%b" "$(DEFAULT)Name:	$(OBJ_COLOR)$(NAME)\n"
endif
	@printf "%b" "$(DEFAULT)Authors:$(OBJ_COLOR)$(AUTHOR)\n"
	@printf "%b" "$(DEFAULT)Date: 	$(OBJ_COLOR)$(DATE)\n\033[m"
	@printf "%b" "$(DEFAULT)CC: 	$(OBJ_COLOR)$(CC)\n\033[m"
	@printf "%b" "$(DEFAULT)Flags: 	$(OBJ_COLOR)$(CPPFLAGS)\n\033[m"
	@echo
