#build script for CS240 Web Crawler
CPP = g++
CFLAGS = -g -Wall -Winit-self -Wmissing-include-dirs -Wextra -Wfloat-equal -Wunused -pedantic

CS240_UTIL_OBJ := $(patsubst utils/src/%.cpp, utils/obj/%.o, $(wildcard utils/src/*.cpp)) utils/inc/UnitTest.h
BIN_OBJ = obj/Crawler.o obj/HTMLparser.o obj/Occurrence.o obj/Page.o obj/StopWords.o obj/URL.o obj/Word.o obj/XML.o

#header files that have no cpp counterpart
CS240_UTIL_H = utils/inc/CS240Exception.h utils/inc/InputStream.h utils/inc/UnitTest.h
MY_H = inc/Queue.h inc/Stack.h inc/LinkedList.h inc/BST.h

TEST_OBJ = test/obj/TestCrawler.o test/obj/TestHTMLparser.o test/obj/TestOccurrence.o test/obj/TestPage.o test/obj/TestStopWords.o test/obj/TestURL.o test/obj/TestWord.o test/obj/TestXML.o test/obj/TestStack.o test/obj/TestLinkedList.o test/obj/TestBST.o test/obj/TestQueue.o test/obj/TestPair.o test/obj/TestHTMLparser.o

print:
	@- echo BIN_OBJ is $(BIN_OBJ)
	@- echo UTIL_OBJ is $(CS240_UTIL_OBJ)
	@- echo TEST_OBJ is $(TEST_OBJ)

all: clean test bin

bin: bin/crawler

test: lib bin/Test.exe
	bin/Test.exe
	# $(MEMCHECK) bin/Test.exe

lib: lib/libcs240utils.a

clean:
	@- rm -f obj/*
	@- rm -f utils/obj/*
	@- rm -f bin/*
	@- rm -f lib/*
	@- rm -f test/obj/*

#note disabling of asserts
bin/crawler: lib $(BIN_OBJ) obj/Main.o
	$(CPP) $(CFLAGS) -DNDEBUG -o bin/crawler $(BIN_OBJ) obj/Main.o -Llib -lcs240utils
#chmod +x bin/crawler

bin/Test.exe: $(BIN_OBJ) $(TEST_OBJ) test/src/TestDriver.cpp test/src/tests.h
	$(CPP) $(CFLAGS) -o bin/Test.exe $(BIN_OBJ) $(TEST_OBJ) -I inc -I utils/inc -Llib -lcs240utils test/src/TestDriver.cpp

#catch-all compilers
utils/obj/%.o: utils/src/%.cpp utils/inc/%.h $(CS240_UTIL_H)
	$(CPP) -c $(CFLAGS) -o utils/obj/$@ -I utils/inc $<

#Main has no .h file, so it needs a special target	
obj/Main.o: src/Main.cpp
	$(CPP) -c $(CFLAGS) -o obj/Main.o -I inc -I utils/inc src/Main.cpp
	
obj/%.o: src/%.cpp inc/%.h $(MY_H)
	$(CPP) -c $(CFLAGS) -o $@ -I inc -I utils/inc $<

test/obj/%.o: test/src/%.cpp test/src/tests.h utils/inc/UnitTest.h
	$(CPP) -c $(CFLAGS) -o $@ -I inc -I test/src -I utils/inc $<

lib/libcs240utils.a: $(CS240_UTIL_OBJ)
	ar r lib/libcs240utils.a $(CS240_UTIL_OBJ)
	
utils/obj/%.o: utils/src/%.cpp utils/inc/%.h $(CS240_UTIL_H)
	$(CPP) -c $(CFLAGS) -o $@ -I utils/inc $<
