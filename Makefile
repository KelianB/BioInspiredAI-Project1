# CONFIG
CC=g++
BUILD_DIR=build
OBJECTS_DIR = $(BUILD_DIR)/objs
BIN_DIR =$(BUILD_DIR)/bin
EXECUTABLE=$(BIN_DIR)/run.exe
SRC_DIR=src
INC=-Isrc/ -Isrc/model -Isrc/genetic-algorithm -Itesting-data

####################################

src = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)
obj = $(src:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/objs/%.o)

# Rule for creating the executable
$(EXECUTABLE): $(obj)
	if not exist $(BUILD_DIR) @mkdir $(BUILD_DIR)
	if not exist $(BIN_DIR) @mkdir $(subst /,\,$(BIN_DIR))
	$(CC) $(INC) -o $(EXECUTABLE) $^

# Rule for creating build/ subdirectories
$(BUILD_DIR)/%/:
	if not exist $@ @mkdir $(subst /,\,$@)

# Rule for compiling into object files (requires the .cpp and the directory)
$(OBJECTS_DIR)/%.o : $(SRC_DIR)/%.cpp $(dir $(OBJECTS_DIR)/%/)
	@echo "Compiling: $< -> $@
	$(CC) $(INC) -c $< -o $@

# Cleaner
.PHONY: clean
clean:
	rmdir $(BUILD_DIR) /s