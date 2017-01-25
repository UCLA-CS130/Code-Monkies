SOURCES=config_parser.cc echo_server.cc
OBJECTS=$(patsubst %.cc,$(OBJ_DIR)/%.o,$(SOURCES)) 

EXECUTABLE=$(BIN_DIR)/echo_server

.PHONY: compile
compile: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/echo_server.o: echo_server.cc $(OBJ_DIR)/config_parser.o
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
