NAME :=	ft_vox


CXX :=	c++
CXXFLAGS :=	-g -MP -MMD -Wall -Wextra -Werror -std=c++17 -O3 #-fsanitize=address -fno-omit-frame-pointer
LFLAGS := -lSDL2 -lGL

EXTERNAL_DIR := external

INCLUDE_DIRS :=	external/imgui/\
				/usr/include/SDL2/\
				external/glad\
				external/stb_image\
				glad\
				inc/\
				inc/Core\
				inc/Core/Mesh\
				inc/Core/Resources\
				inc/Utils\
				inc/Math\
				inc/Math/Vec\
				inc/Math/Mat\
				inc/World\
				inc/World/Block\
				inc/World/Structures\
				inc/World/Biomes\
				inc/World/Biomes/Temperate\
				inc/World/Biomes/Hot\
				inc/World/Biomes/Cold\
				inc/World/Generation/Noise\


SRCS :=	main\
		App\
		Core/Window\
		Core/Camera\
		Core/Resources/Shader\
		World/Chunk\
		World/World\
		World/Block/BlockProperty\
		World/Block/BlockState\
		World/Block/BlockType\
		World/Generation/Noise/Perlin3D\
		World/Generation/Noise/Perlin2D\


INCLUDE_DIRS :=	$(addprefix -I, $(INCLUDE_DIRS))
SRCS :=	$(addprefix src/, $(SRCS))
SRCS :=	$(addsuffix .cpp, $(SRCS))


SRCS +=	external/glad/glad.cpp\
		external/stb_image/stb_image.cpp

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

TPUT 					= tput -T xterm-256color
_RESET 					:= $(shell $(TPUT) sgr0)
_BOLD 					:= $(shell $(TPUT) bold)
_ITALIC 				:= $(shell $(TPUT) sitm)
_UNDER 					:= $(shell $(TPUT) smul)
_GREEN 					:= $(shell $(TPUT) setaf 2)
_YELLOW 				:= $(shell $(TPUT) setaf 3)
_RED 					:= $(shell $(TPUT) setaf 1)
_GRAY 					:= $(shell $(TPUT) setaf 8)
_PURPLE 				:= $(shell $(TPUT) setaf 5)

all:
	@/bin/time --format='$(_GREEN)(%es)$(_RESET) Done' make timed_all --no-print-directory

timed_all:
	@make clone_dependencies --no-print-directory --silent
	@make -j compile --no-print-directory --silent

clone_dependencies: imgui glad stb_image

compile: $(NAME)

BUILD_DIR := build/
ASSETS_DIR := assets/

build: compile
	@echo Packing build
	@mkdir -p $(BUILD_DIR)
	@cp $(NAME) $(BUILD_DIR)
	@cp -r $(ASSETS_DIR) $(BUILD_DIR)
	@cd $(BUILD_DIR) ; tar -czf ft_vox.tar.gz $(NAME) $(ASSETS_DIR)
	@echo Finished packing build

$(EXTERNAL_DIR):
	@mkdir -p external

imgui: $(EXTERNAL_DIR)
	@if ls external | grep -q "imgui"; then \
		printf ""; \
	else \
		echo "Downloading $(_BOLD)imgui$(_RESET)";\
		git clone https://github.com/ocornut/imgui.git $(IMGUI);\
		echo "Downloaded $(_BOLD)imgui$(_RESET)";\
	fi

stb_image: $(EXTERNAL_DIR)
	@if ls external/stb_image | grep -q "stb_image.h"; then \
		printf ""; \
	else\
		echo "Downloading $(_BOLD)stb_image.h$(_RESET)"; \
		mkdir -p external/stb_image; \
		curl --silent -o external/stb_image/stb_image.h https://raw.githubusercontent.com/nothings/stb/master/stb_image.h;\
		echo "Downloaded $(_BOLD)stb_image.h$(_RESET)"; \
	fi

glad: $(EXTERNAL_DIR)
	@if ls external | grep -q "glad"; then\
		printf "";\
	else \
		echo "Downloading $(_BOLD)glad$(_RESET)";\
		mkdir $(EXTERNAL_DIR)/glad;\
		cd $(EXTERNAL_DIR)/glad;\
		mkdir glad;\
		cd glad;\
		curl https://raw.githubusercontent.com/Manualouest/42_postCC/refs/heads/ft_scop/libs/glad/glad.h --output glad.h;\
		cd ..;\
		curl https://raw.githubusercontent.com/Manualouest/42_postCC/refs/heads/ft_scop/libs/glad/glad.c --output glad.cpp;\
		echo "Downloaded $(_BOLD)glad$(_RESET)";\
	fi

$(NAME): $(OBJS)
	@echo 'Linking $(_BOLD)$(NAME)$(_RESET)'
	@/bin/time --format='$(_GREEN)(%es)$(_RESET) Linked $(_BOLD)$(NAME)$(_RESET)' $(CXX) $(CXXFLAGS) $(LFLAGS) $(INCLUDE_DIRS) -o $@ $^

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo 'Compiling $(_BOLD)$<$(_RESET)'
	@/bin/time --format='$(_GREEN)(%es)$(_RESET) Compiled $(_BOLD)$@$(_RESET)' $(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

re: fclean all

fclean: clean
	@echo 'Removed $(_BOLD)$(NAME)$(_RESET)'
	@rm -rf $(NAME)

clean:
	@echo 'Removed $(_BOLD)$(OBJ_DIR)$(_RESET)'
	@echo 'Removed $(_BOLD)$(BUILD_DIR)$(_RESET)'
	@rm -rf $(OBJ_DIR)
	@rm -rf $(BUILD_DIR)

.PHONY: all clean fclean re compile

-include $(DEPS)
