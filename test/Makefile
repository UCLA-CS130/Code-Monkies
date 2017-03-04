TEST_SRC_DIR=$(PROJ_ROOT)/test
SRC_DIR=$(PROJ_ROOT)/src


GTEST_LIBS=$(PROJ_ROOT)/build/gtest.a $(PROJ_ROOT)/build/gtest_main.a
CPPFLAGS=-isystem $(GTEST_DIR)/include
CXXFLAGS+=-c $(DEBUG_FLAGS)
TEST_LDFLAGS=-pthread -L$(PROJ_ROOT)/build $(LDFLAGS)

INCLUDES=-I$(TEST_SRC_DIR) \
		 -I$(SRC_DIR) \
		 -I$(PARSER_DIR)

# Objects which unit tests depend upon. Required for compiling final binary.
TEST_DEP_OBJECTS=$(OBJ_DIR)/config_parser.o

TEST_SOURCES=$(wildcard *.cc)
TEST_OBJECTS=$(patsubst %.cc,$(OBJ_DIR)/%.o,$(TEST_SOURCES))

SOURCES=$(wildcard $(SRC_DIR)/*.cc) \
		$(wildcard $(SRC_DIR)/http/*.cc) \
		$(wildcard $(SRC_DIR)/handler/*.cc) \
		$(wildcard $(SRC_DIR)/api/*.cc)

SOURCES_NO_MAIN=$(filter-out $(SRC_DIR)/main.cc, $(SOURCES))
SOURCE_OBJECTS=$(patsubst $(SRC_DIR)/%.cc,$(OBJ_DIR)/%.o,$(SOURCES_NO_MAIN))

EXECUTABLE=$(BIN_DIR)/webserver_tests
PROXY_HANDLER_EXECUTABLE=$(BIN_DIR)/proxy_webserver_tests

.PHONY: all
all: $(EXECUTABLE) $(PROXY_HANDLER_EXECUTABLE)

$(EXECUTABLE): $(TEST_OBJECTS) $(SOURCE_OBJECTS) $(TEST_DEP_OBJECTS)
	echo $(SOURCE_OBJECTS)
	$(CXX) $^ $(GTEST_LIBS) -o $@ $(TEST_LDFLAGS)

$(PROXY_HANDLER_EXECUTABLE): $(OBJ_DIR/proxy_handler_test.o) $(SOURCE_OBJECTS) $(TEST_DEP_OBJECTS)
	echo $(SOURCE_OBJECTS)
	$(CXX) $^ $(GTEST_LIBS) -o $@ $(TEST_LDFLAGS)

$(OBJ_DIR)/%.o: %.cc
	$(CXX) $(CPPFLAGS) $(INCLUDES) $(CXXFLAGS) $< -o $@

# $(OBJ_DIR)/server_test.o: $(TEST_SRC_DIR)/server_test.cc $(SRC_DIR)/server.cc $(SRC_DIR)/session.cc
# 	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(TEST_SRC_DIR) -I$(SRC_DIR) $< -o $@

# $(OBJ_DIR)/session_test.o: $(TEST_SRC_DIR)/session_test.cc $(SRC_DIR)/session.cc
# 	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(TEST_SRC_DIR) -I$(SRC_DIR) $< -o $@

.PHONY: clean
clean:
	rm -f $(TEST_OBJECTS) $(EXECUTABLE)
