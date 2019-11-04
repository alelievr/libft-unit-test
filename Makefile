# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created  2015/04/04 19:22:36 by alelievr          #+#    #+#              #
#    Updated: 2019/11/03 22:05:40 by tjans         ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

#################
##  VARIABLES  ##
#################

#	Libft Makefile path
LIBFTDIR	=	../libft

#	Sources
SRCDIR		=	./
SRC			=	src/main.c						\
				src/test_functions.c			\
				src/display_test_result.c		\
				src/signal.c					\
				src/sandbox.c					\
				src/bench_functions.c			\
				src/additional_functions.c		\
				hardcore-mode/hardcore-main.c   \
				hardcore-mode/error.c           \
				hardcore-mode/ncurses-display.c \
				hardcore-mode/test-init.c       \
				hardcore-mode/test-utils.c      \
				hardcore-mode/test-strcpy.c     \
				hardcore-mode/test-strcmp.c     \
				hardcore-mode/test-strncmp.c    \
				hardcore-mode/test-memset.c     \
				hardcore-mode/test-memcpy.c     \
				hardcore-mode/test-memccpy.c    \
				hardcore-mode/test-memmove.c    \
				hardcore-mode/test-memchr.c     \
				hardcore-mode/test-memcmp.c     \
				hardcore-mode/test-strlen.c     \
				hardcore-mode/test-strncpy.c    \
				hardcore-mode/test-strcat.c     \
				hardcore-mode/test-strncat.c    \
				hardcore-mode/test-strchr.c     \
				hardcore-mode/test-strrchr.c    \
				hardcore-mode/test-strstr.c     \

#	Objects
OBJDIR		=	obj

#	Includes
INCDIR		=	./include

#	Libraries
LIBDIR		=	
LIBS		=	-lncurses

#	Assets
ASSETDIR	=	assets

#	Output
NAME		=	libtests
SONAME		=	libft.so
ANAME		=	libft.a
LIBMALLOC	=	malloc.dylib
TMPLIB		=	$(ASSETDIR)/tmp
WRAPNAME	=	run_test

FRAMEWORK	=	

#	Compiler
CFLAGS		=	-Werror -Wall -Wextra# -g3 -fsanitize=address
CSOFLAGS	=	-shared -fPIC
CSOFLAGS2	=	
CC			=	clang
CC_SO		=	$(CC)

#	Optimization
OPTFLAGS	=	-O2 -funroll-loops

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

OBJ			=	$(addprefix $(OBJDIR)/, $(addsuffix .o, $(basename $(SRC))))
INCFLAG		=	$(addprefix -I,$(INCDIR))
NORME		=	$(addsuffix /*.h,$(INCDIR)) $(addprefix $(SRCDIR)/,$(SRC))
OS			:=	$(shell uname -s)

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

ifeq ($(OS),Linux)
	LIBS += -lbsd -ldl -lpthread -lm
endif

ifeq ($(OS),Linux)
	DYLIBFLAG = -fPIC -shared -ldl
else
	DYLIBFLAG = -dynamiclib
endif


#################
##  TARGETS    ##
#################

#	First target
all: $(ASSETDIR)/$(ANAME) $(SONAME) $(ASSETDIR)/$(NAME) $(ASSETDIR)/$(LIBMALLOC) $(WRAPNAME)

ifneq ($(OS),Linux)
$(SONAME):
	@rm -rf $(TMPLIB) || echo -n
	@mkdir -p $(TMPLIB)
	@cd $(TMPLIB) && ar -xv ../libft.a 1>/dev/null
	@$(call exec_color, "\033[38;5;$(LINK_COLOR_T)m➤ \033[38;5;$(LINK_COLOR)m",\
		$(CC_SO), $(CSOFLAGS), $(TMPLIB)/*.o, -o, $(SONAME))
else
$(SONAME):
	@make -C $(LIBFTDIR) so
	@if [ -e $(LIBFTDIR)/libft.so ];\
		then\
		cp $(LIBFTDIR)/libft.so . ;\
	else\
		echo Please provide a libft.so in the directory $(shell pwd); \
	fi
endif

$(WRAPNAME): $(ASSETDIR)/wrapper.c
	@$(call exec_color, "\033[38;5;$(LINK_COLOR_T)m", $(CC) $(CFLAGS) $(ASSETDIR)/wrapper.c -I $(INCDIR) -o $(WRAPNAME))

$(ASSETDIR)/$(LIBMALLOC): $(ASSETDIR)/malloc.c
	@$(call exec_color, "\033[38;5;$(LINK_COLOR_T)m", $(CC) $(CFLAGS) $(DYLIBFLAG) $(ASSETDIR)/malloc.c -I $(INCDIR) -o $(ASSETDIR)/$(LIBMALLOC))

$(ASSETDIR)/$(ANAME):
	@rm -f $(SONAME)
	@$(call exec_color, "\033[38;5;$(LINK_COLOR_T)m", make -j 3 -C "$(LIBFTDIR)")
	@$(call exec_color, "\033[38;5;$(LINK_COLOR_T)m", make bonus -j 3 -C "$(LIBFTDIR)" || true)
	@$(call exec_color, "\033[38;5;$(LINK_COLOR_T)m", cp "$(LIBFTDIR)/libft.a" $(ASSETDIR)/)

#	Linking
$(ASSETDIR)/$(NAME): $(OBJ)
	@$(call disp_title,Linking,$(LINK_COLOR_T));
	@$(call exec_color, "\033[38;5;$(LINK_COLOR_T)m➤ \033[38;5;$(LINK_COLOR)m",\
		$(CC), $(CFLAGS), $(OPTFLAGS), $(VLIBDIR), $(VLIB), $(RFRAME) $(LIBS), -o, $(ASSETDIR)/$(NAME), $(OBJ))

#	Objects compilation
$(OBJDIR)/%.o: $(SRCDIR)/%.c include/libft_test.h
	@mkdir -p $(OBJDIR)/$(dir $<)
	@$(call disp_title,Building,$(OBJ_COLOR_T))
	@$(call exec_color,"\033[38;5;$(OBJ_COLOR_T)m➤ \033[0m\033[38;5;$(OBJ_COLOR)m",\
		$(CC), $(OPTFLAGS), $(CFLAGS), $(INCFLAG), -o, $@, -c, $<)
	@$(eval ALREADY_OBJ=x)

#	Removing objects
clean:
	@if [ $(ALREADY_RM)x != xx ]; then \
		$(call disp_title,Cleaning,$(CLEAN_COLOR_T)); \
		fi
	@$(call exec_color,"\033[38;5;$(CLEAN_COLOR_T)m➤ \033[38;5;$(CLEAN_COLOR)m",\
		rm -f, $(OBJ)) # <- Cleaning objs
	@$(call exec_color,"\033[38;5;$(CLEAN_COLOR_T)m➤ \033[38;5;$(CLEAN_COLOR)m",\
		rm -f, $(SONAME) $(ASSETDIR)/$(ANAME) $(ASSETDIR)$(NAME) $(ASSETDIR)/$(LIBMALLOC)) # <- Cleaning assets
	@rmdir $(OBJDIR) 2> /dev/null || echo "" > /dev/null
	@$(eval ALREADY_RM=x)

#	Removing objects and exe
fclean: clean
	@if [ $(ALREADY_RM)x != xx ]; then \
		$(call disp_title,Cleaning,$(CLEAN_COLOR_T)); \
		fi
	@$(call exec_color,"\033[38;5;$(CLEAN_COLOR_T)m➤ \033[38;5;$(CLEAN_COLOR)m",\
		rm -f, $(WRAPNAME))
	@$(eval ALREADY_RM=x)

libclean:
	@rm -rf $(ASSETDIR)/$(ANAME) $(ASSETDIR)/$(NAME) $(TMPLIB)

relib:
	@$(call exec_color, "\033[38;5;$(LINK_COLOR_T)m", make re -C "$(LIBFTDIR)")

#	All removing then compiling
re: relib fclean libclean
	@$(MAKE) all

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

f: libclean all
	@rm -rf $(ASSETDIR)/$(ANAME)
	@echo "\033[38;5;93mRUNNING TESTS:"
	@./$(WRAPNAME) ${SPEED}

b: libclean all
	@rm -rf $(ASSETDIR)/$(ANAME)
	@echo "\033[38;5;93mRUNNING BENCH:"
	@./$(WRAPNAME) -b ${ARGS}

libft: libclean all
	@rm -rf $(ASSETDIR)/$(ANAME)

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


.PHONY: all clean fclean re norme codesize coffee
