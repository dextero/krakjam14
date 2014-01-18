SOURCE_DIR = src
TESTS_DIR = tests

# WARNING: mustn't be . or 'make clean' will do horrible things
OUTPUT_DIR = bin

OUTPUT_RELEASE = $(OUTPUT_DIR)/krakjam14
OUTPUT_DEBUG = $(OUTPUT_DIR)/krakjam14

INCLUDE_DIRS = -Isrc -Iinclude -Iextlibs/Box2D_v2.2.1/include
LIBRARY_DIRS = -Lextlibs/Box2D_v2.2.1/lib

CXX = clang++
CFLAGS = -std=c++11 -Wall -Wextra $(INCLUDE_DIRS) -DPLATFORM_LINUX
CFLAGS_DEBUG = -g -D_DEBUG
CFLAGS_RELEASE = -O2

LD = clang++
LDFLAGS = $(LIBRARY_DIRS)
LDADD = -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -lBox2D
LDADD_DEBUG =
LDADD_RELEASE =

# WARNING: mustn't be . or 'make clean' will do horrible things
INTERMEDIATE_DIR = obj
INTERMEDIATE_DIR_DEBUG = $(INTERMEDIATE_DIR)/debug
INTERMEDIATE_DIR_RELEASE = $(INTERMEDIATE_DIR)/release

ESCAPED_SOURCE_DIR = $(shell echo $(SOURCE_DIR) | sed 's/\//\\\//g')
ESCAPED_INTERMEDIATE_DIR_DEBUG := $(shell echo $(INTERMEDIATE_DIR_DEBUG) | sed 's/\//\\\//g')
ESCAPED_INTERMEDIATE_DIR_RELEASE := $(shell echo $(INTERMEDIATE_DIR_RELEASE) | sed 's/\//\\\//g')

#########
# SOURCES
#########

SOURCES = $(shell find $(SOURCE_DIR) -name '*.cpp')
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS_DEBUG = $(shell echo $(OBJECTS) | sed 's/$(SOURCE_DIR)/$(ESCAPED_INTERMEDIATE_DIR_DEBUG)/g')
OBJECTS_RELEASE = $(shell echo $(OBJECTS) | sed 's/$(SOURCE_DIR)/$(ESCAPED_INTERMEDIATE_DIR_RELEASE)/g')

SOURCES_SUBDIRS = $(shell find $(SOURCE_DIR)/* -type d)
OBJECTS_DIRS_DEBUG = $(shell echo $(SOURCES_SUBDIRS) | sed 's/^$(ESCAPED_SOURCE_DIR)/$(ESCAPED_INTERMEDIATE_DIR_DEBUG)/g')
OBJECTS_DIRS_RELEASE = $(shell echo $(SOURCES_SUBDIRS) | sed 's/^$(ESCAPED_SOURCE_DIR)/$(ESCAPED_INTERMEDIATE_DIR_RELEASE)/g')
OBJECTS_DIRS = $(OBJECTS_DIRS_DEBUG) $(OBJECTS_DIRS_RELEASE)

TESTS_ADDITIONAL_OBJECTS = $(shell echo $(OBJECTS_DEBUG) | awk '{ split($$0, a, " "); for (i=1; i<=NF; i++) print a[i] }' | grep -v 'main.o')
TESTS_OUTPUTS = $(shell find $(TESTS_DIR) -name '*.cpp' | sed 's/\.cpp//g')

#######
# RULES
#######

default: all
all: debug release
rebuild: clean all

release: create_directories print_compile_cmd $(OBJECTS_RELEASE) print_link_cmd
	@echo "LINK $(OUTPUT_RELEASE)"
	@$(LD) $(LDFLAGS) -o $(OUTPUT_RELEASE) $(OBJECTS_RELEASE) $(LDADD) $(LDADD_RELEASE)
	@echo

debug: create_directories print_compile_cmd_debug $(OBJECTS_DEBUG) print_link_cmd_debug
	@echo "LINK $(OUTPUT_DEBUG)"
	@$(LD) $(LDFLAGS) -o $(LDFLAGS_DEBUG) $(OUTPUT_DEBUG) $(OBJECTS_DEBUG) $(LDADD) $(LDADD_DEBUG)
	@echo

build_tests: $(TESTS_OUTPUTS)

tests/%: tests/%.cpp $(TESTS_ADDITIONAL_OBJECTS)
	$(CXX) $(CFLAGS) -o $@ $^ -lm -ldl

tests: build_tests
	cd ./tests && ./run.sh


$(INTERMEDIATE_DIR_DEBUG)/%.o: $(SOURCE_DIR)/%.cpp
	@echo "CXX $@"
	@$(CXX) $(CFLAGS) $(CFLAGS_DEBUG) -c -o $@ $<

$(INTERMEDIATE_DIR_RELEASE)/%.o: $(SOURCE_DIR)/%.cpp
	@echo "CXX $@"
	@$(CXX) $(CFLAGS) $(CFLAGS_RELEASE) -c -o $@ $<


print_compile_cmd_debug:
	@echo CXX: "$(CXX) $(CFLAGS) $(CFLAGS_DEBUG) -c -o OUTPUT INPUT"

print_compile_cmd:
	@echo CXX: "$(CXX) $(CFLAGS) $(CFLAGS_RELEASE) -c -o OUTPUT INPUT"

print_link_cmd_debug:
	@echo LINK: $(LD) $(LDFLAGS) -o $(OUTPUT_DEBUG) $(OBJECTS) $(LDADD) $(LDADD_DEBUG)

print_link_cmd:
	@echo LINK: $(LD) $(LDFLAGS) -o $(OUTPUT_RELEASE) $(OBJECTS) $(LDADD) $(LDADD_RELEASE)


create_directories:
	mkdir -p $(INTERMEDIATE_DIR_DEBUG)	\
	 $(INTERMEDIATE_DIR_RELEASE) 		\
	 $(OUTPUT_DIR)						\
	 $(OBJECTS_DIRS)

clean:
	rm -rf $(INTERMEDIATE_DIR)
	rm -rf $(OUTPUT_DIR)
	rm -f $(TESTS_OUTPUTS)

