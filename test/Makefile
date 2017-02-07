TEST_SRC_DIR=$(PROJ_ROOT)/test
SRC_DIR=$(PROJ_ROOT)/src

GTEST_LIBS=$(PROJ_ROOT)/build/gtest.a $(PROJ_ROOT)/build/gtest_main.a
CPPFLAGS=-isystem $(GTEST_DIR)/include
CXXFLAGS+=-c
TEST_LDFLAGS=-pthread -L$(PROJ_ROOT)/build $(LDFLAGS)

# Objects which unit tests depend upon. Required for compiling final binary.
TEST_DEP_OBJECTS=config_parser.o

TEST_SOURCES=session_test.cc
TEST_OBJECTS=$(patsubst %.cc,$(OBJ_DIR)/%.o,$(TEST_SOURCES)) 

SOURCES=server.cc session.cc
SOURCE_OBJECTS=$(patsubst %.cc,$(OBJ_DIR)/%.o,$(SOURCES)) 

EXECUTABLE=$(BIN_DIR)/webserver_tests

.PHONY: all
all: $(EXECUTABLE)

$(EXECUTABLE): $(TEST_OBJECTS) $(SOURCE_OBJECTS)
	$(CXX) $^ $(GTEST_LIBS) -o $@ $(TEST_LDFLAGS)

$(OBJ_DIR)/session_test.o: $(TEST_SRC_DIR)/session_test.cc $(SRC_DIR)/session.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(TEST_SRC_DIR) -I$(SRC_DIR) $< -o $@

.PHONY: clean
clean:
	rm -f $(TEST_OBJECTS) $(EXECUTABLE)
