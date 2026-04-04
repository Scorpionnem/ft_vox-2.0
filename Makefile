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


compile: imgui glad stb_image
	@make -j all --no-print-directory

all: $(NAME)

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
		echo "\033[31;1mDownloading imgui config\033[0m";\
		git clone https://github.com/ocornut/imgui.git $(IMGUI);\
		echo "\033[31;1mDownloaded imgui config\033[0m";\
	fi

stb_image: $(EXTERNAL_DIR)
	@if ls external/stb_image | grep -q "stb_image.h"; then \
		printf ""; \
	else\
		echo "\033[31;1mDownloading stb_image.h\033[0m"; \
		mkdir -p external/stb_image; \
		curl --silent -o external/stb_image/stb_image.h https://raw.githubusercontent.com/nothings/stb/master/stb_image.h;\
		echo "\033[31;1mDownloaded stb_image.h\033[0m"; \
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
	@echo Compiled $(NAME)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo Compiling $<
	@$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@
	@echo Compiled $@

re: fclean compile

fclean: clean
	@echo Removed $(NAME)
	@rm -rf $(NAME)

clean:
	@echo Removed $(OBJ_DIR)
	@echo Removed $(BUILD_DIR)
	@rm -rf $(OBJ_DIR)
	@rm -rf $(BUILD_DIR)

.PHONY: all clean fclean re compile

-include $(DEPS)
