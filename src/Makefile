INCLUDES=-I. -I$(PARSER_DIR) -Ihttp
CXXFLAGS+=$(INCLUDES)

CC_SOURCES=$(wildcard *.cc) \
		   $(wildcard http/*.cc)

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
debug: CXXFLAGS+=-g -DDEBUG
debug: compile

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
