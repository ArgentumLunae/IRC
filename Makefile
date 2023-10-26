# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: mteerlin <mteerlin@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2023/01/20 13:49:24 by mteerlin      #+#    #+#                  #
#    Updated: 2023/08/04 16:48:59 by mteerlin      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

# include sources.mk

NAME = ircserv
OBJ_DIR = obj
VPATH := $(subst $(" "),:,$(shell find src -type d))
SRC_FILES := main.cpp
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(notdir $(patsubst %.cpp, %.o, $(SRC_FILES))))
HDR_FILES := $(addprefix -I,$(shell find hdr -type d -print))
C_FLAGS = -Wall -Wextra -Werror
STD_FLAGS := -std=c++98
SAN_FLAGS := -g -fsanitize=thread
GCC := c++
CC = ${GCC} $(C_FLAGS) #$(STD_FLAGS) #$(SAN_FLAGS)

all: obj_dir $(NAME)

BOLD = \033[1;37m
BOLDBLUE = \033[1;34m
BOLDGREEN = \033[1;32m
NC = \033[0m

all: obj_dir $(NAME)

$(NAME):	$(OBJ_FILES)
			@echo "$(BOLD)Compiling $(BOLDBLUE)$(NAME)$(BOLD).$(NC)"
			$(CC) $(OBJ_FILES) -o $(NAME)
			@echo "$(BOLD)Compilation finished.$(NC)"

$(OBJ_DIR)/%.o:	%.cpp $(HDR)
			@echo "$(BOLD)creating file: $(NC)"
			$(CC) $(HDR_FILES) -c $< -o $@

obj_dir:
			@clear
			@if [ -d "./$(OBJ_DIR)" ]; then \
			echo "$(BOLDGREEN)obj directory exists$(NC)"; \
			else \
			echo "$(BOLD)creating directory: $(BOLDBLUE)obj $(NC)"; \
			mkdir -p $(OBJ_DIR); \
			fi

run:		all
			./$(NAME)

test:
			@echo $(SRC_FILES)
			@echo $(OBJ_FILES)
			@echo $(HDR_FILES)

clean:
			@echo Removing object files
			rm -rf $(OBJ_DIR)

fclean:
			@$(MAKE) --no-print-directory clean
			@echo Removing excecutables
			@rm -f $(NAME)
re:
			@$(MAKE) --no-print-directory fclean
			@$(MAKE) --no-print-directory all

.PHONY:		all, obj_dir, test, clean, fclean, re;
