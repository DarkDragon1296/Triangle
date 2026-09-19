# TODO: use environment variables (CXX FLAGS, CC, etc.)
#
# -----------------------------
# Makefile commands:
#		`make`             - same as `make debug`
#		`make debug`       - build the target without optimizations 
#		                       and with sanitizers, then update todo
#		`make release`     - build the target with optimizations 
#		                       and no debug features, nor sanitizers
#		`make clean`       - delete artifacts and the target
#		`make clear`       - same as `make clean`
#		`make update_todo` - grep the project for TODO's and 
#		                       store them in $(TODO_FILE)
# -----------------------------
#
#
COMPILER := g++

ARTIFACT_PATH := build
BINARY_PATH   := bin

SOURCE_PATH   := src
INCLUDE_PATH  := include

INCLUDE_FLAGS := -I $(SOURCE_PATH)/ -I $(INCLUDE_PATH)/
LIBS          := -lm -lc -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

# Common defines
DEFINE_FLAGS  :=

# Debug exclusive defines
DEBUG_DEFINE_FLAGS := -D _DEBUG

# Release exclusive defines
RELEASE_DEFINE_FLAGS := -D NDEBUG

PROG_NAME     := triangles
MAIN_TARGET   := $(BINARY_PATH)/$(PROG_NAME)
# Source files (src/ is autoappended)
RAW_SOURCES   := main.cpp glad.cpp render.cpp
SOURCES       := $(patsubst %.cpp, $(SOURCE_PATH)/%.cpp, $(RAW_SOURCES))

OBJECTS      := $(RAW_SOURCES:%.cpp=$(ARTIFACT_PATH)/%.o)
DEPENDENCIES := $(OBJECTS:%.o=%.d)

SANITIZER_FLAGS := -fsanitize=address,alignment,bool,bounds,enum,$\
		 		           float-cast-overflow,float-divide-by-zero,$\
				           integer-divide-by-zero,leak,nonnull-attribute,$\
				           null,object-size,return,returns-nonnull-attribute,$\
				           shift,signed-integer-overflow,undefined,$\
				           unreachable,vla-bound,vptr
# Debug exclusive flags
DEBUG_CXX_FLAGS := -ggdb3 -O0 \
                   -Wstack-protector -fstack-protector \
									 -fno-omit-frame-pointer \
									 $(SANITIZER_FLAGS)
# Release exclusive flags
RELEASE_CXX_FLAGS := -O3

# Common flags
CXX_FLAGS := -std=c++17 -Wall -Wextra -Weffc++                         \
						 -Waggressive-loop-optimizations                           \
						 -Wc++14-compat -Wmissing-declarations -Wcast-align        \
						 -Wcast-qual -Wchar-subscripts -Wconditionally-supported   \
						 -Wconversion -Wctor-dtor-privacy -Wempty-body             \
						 -Wfloat-equal -Wformat-nonliteral -Wformat-security       \
						 -Wformat-signedness -Wformat=2 -Winline -Wlogical-op      \
						 -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual     \
						 -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls    \
						 -Wshadow -Wsign-conversion -Wsign-promo                   \
						 -Wstrict-null-sentinel -Wstrict-overflow=2                \
						 -Wsuggest-attribute=noreturn -Wsuggest-final-methods      \
						 -Wsuggest-final-types -Wsuggest-override -Wswitch-default \
						 -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code      \
						 -Wunused -Wuseless-cast -Wvariadic-macros                 \
						 -Wno-literal-suffix -Wno-missing-field-initializers       \
						 -Wno-narrowing -Wno-old-style-cast -Wno-varargs           \
						 -fcheck-new -fsized-deallocation -fstrict-overflow        \
						 -flto-odr-type-merging -Wstack-usage=8192                 \
						 -pie -fPIE -Werror=vla

.PHONY: debug debug_prehook     \
	      release release_prehook \
				build

# Default debug target ('make' == 'make debug')
debug: debug_prehook build update_todo

debug_prehook:
	$(eval DEFINE_FLAGS += $(DEBUG_DEFINE_FLAGS))
	$(eval CXX_FLAGS    += $(DEBUG_CXX_FLAGS))
	@echo "!Debug compilation mode!"

# Release target (activated by running 'make release')
release: release_prehook build

release_prehook:
	$(eval DEFINE_FLAGS += $(RELEASE_DEFINE_FLAGS))
	$(eval CXX_FLAGS    += $(RELEASE_CXX_FLAGS))
	@echo "!Release compilation mode!"

build: ensure_directories_exist $(MAIN_TARGET)

$(MAIN_TARGET): $(OBJECTS)
	@echo -e "• Linking "$(MAIN_TARGET)" together"
	@$(COMPILER) $(CXX_FLAGS) $^ -o $@ $(LIBS)

# Include dependencies (object: headers + source file), if they exist
-include $(DEPENDENCIES)

# Implied rule for objects
$(ARTIFACT_PATH)/%.o: $(SOURCE_PATH)/%.cpp
	@echo -e "• Compiling" $<
	@mkdir -p $(@D)
	@$(COMPILER) -c -MMD $(DEFINE_FLAGS) \
	             $(INCLUDE_FLAGS) $(LIBS) $(CXX_FLAGS) $< -o $@

.PHONY: ensure_directories_exist \
	      clean clear build update_todo

ensure_directories_exist:
	mkdir -p $(BINARY_PATH) $(ARTIFACT_PATH)

clear: clean

clean:
	rm -f $(MAIN_TARGET)
	rm -f -r $(ARTIFACT_PATH)
	mkdir -p $(ARTIFACT_PATH)

TODO_FILE := TODO.txt

TODO_EXCLUDED_FILES := Makefile .gitignore
TODO_EXCLUDED_FILES := $(foreach i,$(TODO_EXCLUDED_FILES),--exclude="$(i)")

TODO_EXCLUDED_DIRS := .git
TODO_EXCLUDED_DIRS := $(foreach i,$(TODO_EXCLUDED_DIRS),--exclude-dir="$(i)")

update_todo:
	@echo -e "• Updating $(TODO_FILE)"
	@rm -f $(TODO_FILE)
	@grep -r -n "TODO"  $(TODO_EXCLUDED_FILES) \
	                    $(TODO_EXCLUDED_DIRS)  \
							  			| sed G >> $(TODO_FILE)
