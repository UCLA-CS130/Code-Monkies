INCLUDES=-I. -I$(PARSER_DIR)
CXXFLAGS+=$(INCLUDES)

SOURCES=main.cc config.cc config_parser.cc server.cc session.cc
OBJECTS=$(patsubst %.cc,$(OBJ_DIR)/%.o,$(SOURCES))

EXECUTABLE=$(BIN_DIR)/echo_server

.PHONY: compile
compile: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/main.o: main.cc
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/config.o: config.cc config.h
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/server.o: server.cc server.h
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/session.o: session.cc session.h
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: debug
debug: CXXFLAGS+=$(DEBUG_FLAGS)
debug: compile

.PHONY: test
test: CXXFLAGS+=$(DEBUG_FLAGS) -fprofile-arcs -ftest-coverage
test: compile

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
