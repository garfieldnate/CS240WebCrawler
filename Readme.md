#CS 240 Web Crawler

This is a web crawler/word indexer created for an assignment in CS 240 at BYU in 2011. The purpose of the class was to 1) teach us how to work with "large" codebases and 2) introduce us to Linux, C++ and Makefiles. The class was famous for its "nightmare" projects among CS majors, but it was actually a lot of fun. The thing I remember most from the class is that C/C++ are a pain in the butt to work with :).

After we turned out in our painstakingly debugged web crawler, the TA's proceeded to add off-by-one, use after free, and null pointer errors to our code. Then they gave us 90 minutes to find all of the easter eggs :). I wrote my experiences up [here](http://garfieldnate.github.io/blog/2011/11/03/cs-240-web-crawler-at-byu/).

The class has since changed and the assignment no longer exists. I am storing this mostly for historical significance. This is really a toy program, so please don't try to use it in production!. 

The original specification can be found [here](https://faculty.cs.byu.edu/~rodham/cs240/crawler/). Basically you crawl web pages starting at a seed URL, indexing the words found and outputting this insanely verbose index:

        <word>
            <value>
                other
            </value>
            <occurrence>
                <url>
                    http://www.location.where.found.other.com/index.html
                </url>
                <count>
                    23
                </count>
            </occurrence>
        </word>

You can see full example output in test/data/index.xml.

##Building/Running
As-is, the crawler can only be compiled and run on a Posix platform.

Build the executable and tests using `make all`. Run the (hand-written without any framework!) unit tests with `bin/Test.exe`. Run the actual crawler with 
    
    crawler <start-url> <output-file> <stopword-file>

where `start-url` is the seed URL for the crawler, `output-file` is the path to print the word index to, and `stopword-file` is the path to a file containing line-separated words to ignore (you probably want to ignore 'the', etc.).

##Checking Memory
A suppression file is included which suppresses `valgrind` warnings caused by the standard string library. To run with `valgrind`, do this:

    valgrind --suppressions=string.supp -tool=memcheck --leak-check=yes --show-reachable=yes bin/Test.exe

##Author
Nathan Glenn, 2011
