INCLUDES=-I. -I$(PARSER_DIR) -Ihttp -I$(GTEST_DIR)/include -I/usr/include/
CXXFLAGS+=$(INCLUDES)

CC_SOURCES=$(wildcard *.cc) \
		   $(wildcard api/*.cc) \
		   $(wildcard handler/*.cc)

OBJECTS=$(patsubst %.cc,$(OBJ_DIR)/%.o,$(CC_SOURCES))
DEPS=$(OBJ_DIR)/config_parser.o

EXECUTABLE=$(BIN_DIR)/webserver

.PHONY: compile
compile: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(DEPS)
	$(CXX) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: %.cc
	$(CXX) $(CXXFLAGS) $< -o $@

# $(OBJ_DIR)/main.o: main.cc
# 	$(CXX) $(CXXFLAGS) $< -o $@

# $(OBJ_DIR)/server.o: server.cc server.h $(OBJ_DIR)/session.o
# 	$(CXX) $(CXXFLAGS) $< -o $@

# $(OBJ_DIR)/session.o: session.cc session.h
# 	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: debug
debug: CXXFLAGS+=$(DEBUG_FLAGS)
debug: compile

.PHONY: test
test: CXXFLAGS+=$(DEBUG_FLAGS) -fprofile-arcs -ftest-coverage
test: compile

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
