# Define the build directory
BUILD_DIR := build

# Define the CMake build type (can be Debug, Release, etc.)
BUILD_TYPE := Debug

# Check if Ninja is available
NINJA_AVAILABLE := $(shell which ninja > /dev/null 2>&1 && echo 1 || echo 0)

# Define the CMake generator based on Ninja availability
ifeq ($(NINJA_AVAILABLE), 1)
  GENERATOR := Ninja
else
  GENERATOR := Unix Makefiles
endif

# Define the CMake command and options
CMAKE := cmake
CMAKE_FLAGS := -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -G "$(GENERATOR)"

# Default target: configure and build the project
all: configure build

# Configure the project using CMake if the build directory doesn't exist
configure:
	@echo "Configuring the project with generator $(GENERATOR)..."
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		mkdir -p $(BUILD_DIR); \
	fi
	@cd $(BUILD_DIR) && $(CMAKE) $(CMAKE_FLAGS) .. || exit 1

# Build the project
build: configure
	@echo "Building the project..."
	@$(CMAKE) --build $(BUILD_DIR) || exit 1

# Clean the build directory
clean:
	@echo "Cleaning the build directory..."
	@rm -rf $(BUILD_DIR)

# Rebuild the project (clean, configure, build)
rebuild: clean all

# Run tests (assuming you have a target named 'test' in CMakeLists.txt)
test:
	@echo "Running tests..."
	@$(CMAKE) --build $(BUILD_DIR) --target test || exit 1

.PHONY: all configure build clean rebuild test

