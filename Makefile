TARGET = NewtonsFractal.exe
SRC_DIR = src
OUT_DIR = out

SRC_FILES = main.cpp pixel.cpp
OBJECTS = $(addprefix $(OUT_DIR)/, $(SRC_FILES:.cpp=.o))

INCLUDE_PATH = -I"C:\mingw64\include"

LIBS = -lsfml-graphics -lsfml-window -lsfml-system
LIB_PATH = -L"C:\mingw64\lib"

CXX = g++

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@$(CXX) -o $@ $^ $(LIB_PATH) $(LIBS)

$(OUT_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OUT_DIR)
	@$(CXX) -c $< -o $@ $(INCLUDE_PATH)

$(OUT_DIR):
	@mkdir -p $(OUT_DIR)

clean:
	@$(RM) $(OBJECTS) $(TARGET)

run:
	@./$(TARGET)

.PHONY: clean run