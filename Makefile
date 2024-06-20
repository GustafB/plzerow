BUILD_DIR := build

BUILD_TYPE := Debug

NINJA_AVAILABLE := $(shell which ninja > /dev/null 2>&1 && echo 1 || echo 0)

ifeq ($(NINJA_AVAILABLE), 1)
  GENERATOR := Ninja
else
  GENERATOR := Unix Makefiles
endif

CMAKE := cmake
CMAKE_FLAGS := -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -G "$(GENERATOR)"

PYTHON := python3
GENERATE_AST_SCRIPT := ./tools/generate_ast_classes.py
INCLUDE_DIR := /home/cafebabe/code/plzerow/include/plzerow
SRC_DIR := /home/cafebabe/code/plzerow/src

all: generate_ast_classes configure build

generate_ast_classes:
	@echo "Generating AST classes..."
	@$(PYTHON) $(GENERATE_AST_SCRIPT) $(INCLUDE_DIR) $(SRC_DIR) || exit 1

configure: generate_ast_classes
	@echo "Configuring the project with generator $(GENERATOR)..."
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		mkdir -p $(BUILD_DIR); \
	fi
	@cd $(BUILD_DIR) && $(CMAKE) $(CMAKE_FLAGS) .. || exit 1

build:
	@echo "Building the project..."
	@$(CMAKE) --build $(BUILD_DIR) || exit 1

clean:
	@echo "Cleaning the build directory..."
	@rm -rf $(BUILD_DIR)

rebuild: clean all

test:
	@echo "Running tests..."
	@$(CMAKE) --build $(BUILD_DIR) --target test || exit 1

.PHONY: all generate_ast_classes configure build clean rebuild test

