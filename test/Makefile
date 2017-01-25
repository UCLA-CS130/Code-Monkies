PARSER_ROOT=$(PROJ_ROOT)/src/nginx-configparser

GTEST_LIBS=$(PROJ_ROOT)/build/gtest.a $(PROJ_ROOT)/build/gtest_main.a
CPPFLAGS=-isystem $(GTEST_DIR)/include
CXXFLAGS+=-c
LDFLAGS+=-pthread -L$(PROJ_ROOT)/build

# Objects which unit tests depend upon. Required for compiling final binary.
TEST_DEP_OBJECTS=config_parser.o

TEST_SOURCES=config_parser_test.cc
TEST_OBJECTS=$(patsubst %.cc,$(OBJ_DIR)/%.o,$(SOURCES)) 

EXECUTABLE=webserver_tests

.PHONY: all
all: $(BIN_DIR)/$(EXECUTABLE)

$(BIN_DIR)/$(EXECUTABLE): $(OBJ_DIR)/config_parser.o $(OBJ_DIR)/config_parser_test.o
	$(CXX) $(LDFLAGS) $^ $(GTEST_LIBS) -o $@

$(OBJ_DIR)/config_parser_test.o: $(PARSER_DIR)/config_parser_test.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(PARSER_ROOT) $< -o $@

.PHONY: clean
clean:
	rm -f $(TEST_OBJECTS) $(EXECUTABLES)
