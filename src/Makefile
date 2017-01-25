INCLUDES=-I. -I$(PARSER_DIR)
CXXFLAGS+=$(INCLUDES)

SOURCES=main.cc config_parser.cc server.cc session.cc helpers.cc
OBJECTS=$(patsubst %.cc,$(OBJ_DIR)/%.o,$(SOURCES)) 

EXECUTABLE=$(BIN_DIR)/echo_server

.PHONY: compile
compile: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/main.o: main.cc
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/server.o: server.cc server.h $(OBJ_DIR)/session.o
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/session.o: session.cc session.h
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/helpers.o: helpers.cc helpers.h
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: debug
debug: CXXFLAGS+=-g -DDEBUG
debug: compile

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
