NAME :=	ft_vox


CXX :=	c++
CXXFLAGS :=	-g -MP -MMD -Wall -Wextra -Werror -std=c++17 # -fsanitize=address -fno-omit-frame-pointer
LFLAGS := -lSDL2 -lGL

EXTERNAL_DIR := external

INCLUDE_DIRS :=	external/imgui/\
				/usr/include/SDL2/\
				external/glad\
				glad\
				inc/\
				inc/Core\
				inc/Core/Resources\
				inc/Utils\
				inc/Math\
				inc/Math/Vec\
				inc/Math/Mat\
				inc/World\


SRCS :=	main\
		App\
		Core/Window\
		Core/Camera\
		Core/Resources/Shader\


INCLUDE_DIRS :=	$(addprefix -I, $(INCLUDE_DIRS))
SRCS :=	$(addprefix src/, $(SRCS))
SRCS :=	$(addsuffix .cpp, $(SRCS))


SRCS +=	external/glad/glad.cpp

IMGUI := $(EXTERNAL_DIR)/imgui
IMGUI_SRCS_RAW =	imgui.cpp\
					imgui_draw.cpp\
					imgui_widgets.cpp\
					imgui_tables.cpp\
					imgui_demo.cpp\
					backends/imgui_impl_opengl3.cpp\
					backends/imgui_impl_sdl2.cpp
IMGUI_SRCS = $(addprefix $(IMGUI)/, $(IMGUI_SRCS_RAW))

SRCS +=	$(IMGUI_SRCS)


OBJ_DIR :=	obj
OBJS =	$(SRCS:%.cpp=$(OBJ_DIR)/%.o)
DEPS =	$(SRCS:%.cpp=$(OBJ_DIR)/%.d)


compile: imgui glad
	@make -j all --no-print-directory

all: $(NAME)

$(EXTERNAL_DIR):
	@mkdir -p external

imgui: $(EXTERNAL_DIR)
	@if ls external | grep -q "imgui"; then \
		printf ""; \
	else \
		echo "\033[31;1mDownloading imgui config\033[0m";\
		git clone https://github.com/ocornut/imgui.git $(IMGUI);\
		echo "\033[31;1mDownloaded imgui config\033[0m";\
	fi

glad: $(EXTERNAL_DIR)
	@if ls external | grep -q "glad"; then\
  		printf "";\
  	else \
		echo "\033[31;1mDownloading glad\033[0m";\
		mkdir $(EXTERNAL_DIR)/glad;\
		cd $(EXTERNAL_DIR)/glad;\
		mkdir glad;\
		cd glad;\
		curl https://raw.githubusercontent.com/Manualouest/42_postCC/refs/heads/ft_scop/libs/glad/glad.h --output glad.h;\
		cd ..;\
		curl https://raw.githubusercontent.com/Manualouest/42_postCC/refs/heads/ft_scop/libs/glad/glad.c --output glad.cpp;\
		echo "\033[31;1mDownloaded glad\033[0m";\
	fi

$(NAME): $(OBJS)
	@echo Compiling $(NAME)
	@$(CXX) $(CXXFLAGS) $(LFLAGS) $(INCLUDE_DIRS) -o $@ $^

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

re: fclean compile

fclean: clean
	@echo Removed $(NAME)
	@rm -rf $(NAME)

clean:
	@echo Removed $(OBJ_DIR)
	@rm -rf $(OBJ_DIR)

.PHONY: all clean fclean re compile

-include $(DEPS)
