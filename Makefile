# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/04/04 19:22:36 by alelievr          #+#    #+#              #
#    Updated: 2015/11/21 17:14:20 by bciss            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#################
##  VARIABLES  ##
#################

#	Libft Makefile path
LIBFTDIR	=	~/rendu/wolfi/libft/

#	Sources
SRCDIR		=	./src
SRC			=	main.c					\
				test_functions.c		\
				display_test_result.c	\
				signal.c				\
				sandbox.c				\

#	Objects
OBJDIR		=	obj

#	Includes
INCDIR		=	./include

#	Libraries
LIBDIR		=	
LIBS		=	

#	Output
NAME		=	run_test
SONAME		=	libft.so
ANAME		=	libft.a

FRAMEWORK	=	

#	Compiler
CFLAGS		=	-Werror -Wall -Wextra -g
CSOFLAGS	=	-fpic -shared -Wl,-all_load
CC			=	clang

#	Optimization
OPTFLAGS	=	-funroll-loops -O3 -pipe

################
##   COLORS   ##
################

CLEAN_COLOR		=	9
CLEAN_COLOR_T	=	196
LINK_COLOR		=	47
LINK_COLOR_T	=	46
OBJ_COLOR		=	122
OBJ_COLOR_T		=	123
NORM_COLOR		=	153
NORM_COLOR_T	=	141
NORM_COLOR_ERR	=	160
NORM_COLOR_WAR	=	214
RUNT_COLR_T		=	7
RUN_COLOR		=	147


#################
##    AUTO     ##
#################

SHELL		=	/bin/zsh
OBJ			=	$(addprefix $(OBJDIR)/, $(addsuffix .o, $(basename $(SRC))))
INCFLAG		=	$(addprefix -I,$(INCDIR))
NORME		=	$(addsuffix /*.h,$(INCDIR)) $(addprefix $(SRCDIR)/,$(SRC))

$(foreach L, $(LIBS), \
	$(eval VLIB += -l$(L)) \
)

$(foreach LD, $(LIBDIR), \
	$(eval VLIBDIR += -L$(LD)) \
)

$(foreach FRM, $(FRAMEWORK), \
	RFRAME += $(addprefix -framework, $(FRM)) \
)

$(foreach S, $(SRC), \
	$(eval VPATH_TMP += $(SRCDIR)/$(dir $(S)) \
	))
VPATH		=	$(VPATH_TMP:./=)

disp_indent	=	for I in `seq 1 $(MAKELEVEL)`; do \
				test "$(MAKELEVEL)" '!=' '0' && printf "\t"; \
				done

exec_color	=	$(call disp_indent); \
				echo $(1)$(2) $(3) $(4) $(5) $(6) $(7) $(8) $(9) $(10)"\033[31m"; \
				$(2) $(3) $(4) $(5) $(6) $(7) $(8) $(9) $(10)

disp_title	=	$(call disp_indent); \
				echo "\033[38;5;$(2);m[  $(1) \#$(MAKELEVEL)  ]\033[0m"


#################
##  TARGETS    ##
#################

#	First target
all: $(ANAME) $(SONAME) $(NAME)

$(SONAME): shared

$(ANAME):
	@$(call exec_color, "\033[38;5;$(LINK_COLOR_T)m", make -C $(LIBFTDIR))
	@$(call exec_color, "\033[38;5;$(LINK_COLOR_T)m", cp $(LIBFTDIR)/libft.a .)
	
shared:
	@$(call exec_color, "\033[38;5;$(LINK_COLOR_T)m➤ \033[38;5;$(LINK_COLOR)m",\
		$(CC), $(CSOFLAGS), -o, $(SONAME), libft.a)

#	Linking
$(NAME): $(OBJ)
	@$(call disp_title,Linking,$(LINK_COLOR_T));
	@$(call exec_color, "\033[38;5;$(LINK_COLOR_T)m➤ \033[38;5;$(LINK_COLOR)m",\
		$(CC), $(CFLAGS), $(OPTFLAGS), $(VLIBDIR), $(VLIB), $(RFRAME), -o, $(NAME), $(OBJ))

#	Objects compilation
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@$(call disp_title,Building,$(OBJ_COLOR_T))
	@$(call exec_color,"\033[38;5;$(OBJ_COLOR_T)m➤ \033[0m\033[38;5;$(OBJ_COLOR)m",\
		$(CC), $(OPTFLAGS), $(CFLAGS), $(INCFLAG), -o, $@, -c, $<)
	@$(eval ALREADY_OBJ=x)

#	Removing objects
clean:
	@rm -f $(SONAME) $(ANAME)
	@if [ $(ALREADY_RM)x != xx ]; then \
		$(call disp_title,Cleaning,$(CLEAN_COLOR_T)); \
		fi
	@$(call exec_color,"\033[38;5;$(CLEAN_COLOR_T)m➤ \033[38;5;$(CLEAN_COLOR)m",\
		rm -f, $(OBJ)) # <- Cleaning objs
	@rmdir $(OBJDIR) 2> /dev/null || echo "" > /dev/null
	@$(eval ALREADY_RM=x)

#	Removing objects and exe
fclean: clean
	@if [ $(ALREADY_RM)x != xx ]; then \
		$(call disp_title,Cleaning,$(CLEAN_COLOR_T)); \
		fi
	@$(call exec_color,"\033[38;5;$(CLEAN_COLOR_T)m➤ \033[38;5;$(CLEAN_COLOR)m",\
		rm -f, $(NAME))
	@$(eval ALREADY_RM=x)

#	All removing then compiling
re: fclean all

#	Checking norme
norme:
	@$(call disp_title,Norme,$(NORM_COLOR_T))
	@norminette $(NORME) | sed "s/Norme/[0;1;$(NORM_COLOR_T)m➤  [0;38;5;$(NORM_COLOR)mNorme/g;s/Warning/[38;5;$(NORM_COLOR_WAR)mWarning/g;s/Error/[38;5;$(NORM_COLOR_ERR)mError/g"

run: $(NAME)
	@echo "\033[1;35;$(RUN_COLOR_T)m[  Running #$(MAKELEVEL)  ]\033[0m"
	@echo "\033[0;1;35m➤ \033[0;38;5;$(RUN_COLOR)m./a.out ${ARGS}\033[0m"
	@./$(NAME) ${ARGS}

codesize:
	@cat $(NORME) |grep -v '/\*' |wc -l

coffee:
	@clear
	@echo ""
	@echo "                   ("
	@echo "	                     )     ("
	@echo "               ___...(-------)-....___"
	@echo '           .-""       )    (          ""-.'
	@echo "      .-''''|-._             )         _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |                             |"
	@echo "     \  \   |                             |"
	@echo "      '\ '\ |                             |"
	@echo "        '\ '|                             |"
	@echo "        _/ /\                             /"
	@echo "       (__/  \                           /"
	@echo '    _..---""` \                         /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'
	@sleep 0.5
	@clear
	@echo ""
	@echo "                 ("
	@echo "	                  )      ("
	@echo "               ___..(.------)--....___"
	@echo '           .-""       )   (           ""-.'
	@echo "      .-''''|-._      (       )        _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |                             |"
	@echo "     \  \   |                             |"
	@echo "      '\ '\ |                             |"
	@echo "        '\ '|                             |"
	@echo "        _/ /\                             /"
	@echo "       (__/  \                           /"
	@echo '    _..---""` \                         /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'
	@sleep 0.5
	@clear
	@echo ""
	@echo "               ("
	@echo "	                  )     ("
	@echo "               ___..(.------)--....___"
	@echo '           .-""      )    (           ""-.'
	@echo "      .-''''|-._      (       )        _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |                             |"
	@echo "     \  \   |                             |"
	@echo "      '\ '\ |                             |"
	@echo "        '\ '|                             |"
	@echo "        _/ /\                             /"
	@echo "       (__/  \                           /"
	@echo '    _..---""` \                         /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'
	@sleep 0.5
	@clear
	@echo ""
	@echo "             (         ) "
	@echo "	              )        ("
	@echo "               ___)...----)----....___"
	@echo '           .-""      )    (           ""-.'
	@echo "      .-''''|-._      (       )        _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |                             |"
	@echo "     \  \   |                             |"
	@echo "      '\ '\ |                             |"
	@echo "        '\ '|                             |"
	@echo "        _/ /\                             /"
	@echo "       (__/  \                           /"
	@echo '    _..---""` \                         /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'


f: all
	./$(NAME)

.PHONY: all clean fclean re norme codesize coffee
