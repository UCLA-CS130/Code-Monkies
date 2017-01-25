clean:
	rm -f config_parser config_parser_test gtest-all.o libgtest.a


test: clean
	./build_tests.sh && ./config_parser_test

build: clean
	./build.sh