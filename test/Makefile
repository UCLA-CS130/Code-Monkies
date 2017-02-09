TEST_SRC_DIR=$(PROJ_ROOT)/test
SRC_DIR=$(PROJ_ROOT)/src

GTEST_LIBS=$(PROJ_ROOT)/build/gtest.a $(PROJ_ROOT)/build/gtest_main.a
CPPFLAGS=-isystem $(GTEST_DIR)/include
CXXFLAGS+=-c $(DEBUG_FLAGS)
TEST_LDFLAGS=-pthread -L$(PROJ_ROOT)/build $(LDFLAGS)

INCLUDES=-I$(TEST_SRC_DIR) -I$(SRC_DIR) -I$(PARSER_DIR)

# Objects which unit tests depend upon. Required for compiling final binary.

TEST_SOURCES=session_test.cc config_test.cc
TEST_OBJECTS=$(patsubst %.cc,$(OBJ_DIR)/%.o,$(TEST_SOURCES)) 

SOURCES=server.cc session.cc config.cc config_parser.cc
SOURCE_OBJECTS=$(patsubst %.cc,$(OBJ_DIR)/%.o,$(SOURCES)) 

EXECUTABLE=$(BIN_DIR)/webserver_tests

.PHONY: all
all: $(EXECUTABLE)

$(EXECUTABLE): $(TEST_OBJECTS) $(SOURCE_OBJECTS)
	$(CXX) $^ $(GTEST_LIBS) -o $@ $(TEST_LDFLAGS)

$(OBJ_DIR)/session_test.o: $(TEST_SRC_DIR)/session_test.cc $(SRC_DIR)/session.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) $< -o $@

$(OBJ_DIR)/config_test.o: $(TEST_SRC_DIR)/config_test.cc $(SRC_DIR)/config.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) $< -o $@

.PHONY: clean
clean:
	rm -f $(TEST_OBJECTS) $(EXECUTABLE)
