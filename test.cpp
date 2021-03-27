#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <fstream>

#include "containers/box.h"
#include "containers/dimensions.h"

TEST_CASE("#SET: box object numbering") {
    Containers::Dimensions d(1, 2, 3);
    Containers::Box b0(d), b1(d), b2(d), b3(d);
    REQUIRE(b0.getId() == 0);
    REQUIRE(b1.getId() == b0.getId() + 1);
    REQUIRE(b2.getId() == b1.getId() + 1);
    REQUIRE(b3.getId() == b2.getId() + 1);
}

TEST_CASE("#AUTO_ID: dimensions setters and getters") {
    Containers::Dimensions d;
    d.setLength(10);
    d.setWidth(20);
    d.setHeight(30);
    REQUIRE(d.getLength() == 10);
    REQUIRE(d.getWidth() == 20);
    REQUIRE(d.getHeight() == 30);
}

TEST_CASE("#CMP: matching box comparison operators") {
    Containers::Box b1({1, 2, 3}), b2({1, 2, 3}), b3({2, 2, 2});
    REQUIRE((b1 == b2) != (b1 != b2));
    REQUIRE((b1 < b2) != (b1 >= b2));
    REQUIRE((b1 > b2) != (b1 <= b2));
    REQUIRE((b1 > b2 || b1 < b2) == (b1 != b2));
    REQUIRE((b1 >= b2 && b1 <= b2) == (b1 == b2));
}

TEST_CASE("#STREAM: serializing and deserializing") {
    Containers::Box box({10, 20, 30}), boxFromStream;
    box.open();
    box.putItem({5, 5, 5});

    std::stringstream ss;
    ss << box;
    ss >> boxFromStream;


    REQUIRE(boxFromStream.equals(box));
    boxFromStream.close();
    REQUIRE_FALSE(boxFromStream.equals(box));
}

TEST_CASE("#PRE_POST: pre and post increment validation") {
    Containers::Box b1({10, 20, 30}), b2(b1);

    Containers::Box copy1(b1++), &copy2(++b2);

    REQUIRE(!copy1.equals(b1));
    REQUIRE(copy2.equals(b2));

    REQUIRE(&copy2 == &b2);

    REQUIRE(b1.equals(b2));
}

TEST_CASE("#THROW: throwing of exceptions") {
    Containers::Box b;
    REQUIRE_THROWS(b.open()); // a

    REQUIRE_THROWS_AS(b.init({10, 20, 0}), std::invalid_argument); // b

    REQUIRE_NOTHROW(b.init({10, 10, 10}));

    Containers::Box old(b);
    std::stringstream ss;
    ss << "garbage";
    REQUIRE_THROWS(ss >> old);

    REQUIRE(old.equals(b)); // c
}

TEST_CASE("#COPY: deep  copying") {
    Containers::Box b1({10, 20, 15});
    b1.open();
    b1.putItem({5, 6, 7});

    Containers::Box copy1(b1);
    REQUIRE(copy1.equals(b1));
    copy1.close();
    REQUIRE(!copy1.equals(b1));

    Containers::Box copy2 = b1;
    REQUIRE(copy2.equals(b1));
    copy2.close();
    REQUIRE(!copy2.equals(b1));
}

TEST_CASE("#LEAK: check for memory leaks") {
    {
        REQUIRE_THROWS(Containers::Box({1, 3, 0}));
        Containers::Box b1({20, 10, 5}), b2(b1);
        Containers::Box b3 = b1;
    }
    REQUIRE(Containers::Box::getCurrentInstances() == 0);
}


TEST_CASE("#CUSTOM: general box functionality") {
    Containers::Box b;
    REQUIRE_THROWS(b.open());

    REQUIRE_THROWS_AS(b.init({10, 20, 0}), std::invalid_argument);

    REQUIRE_NOTHROW(b.init({10, 10, 10}));

    REQUIRE_THROWS(b.close());


    REQUIRE_NOTHROW(b.open());
    REQUIRE_THROWS(b.open());

    REQUIRE_THROWS(b.putItem({10, 11, 10}));
    REQUIRE_NOTHROW(b.putItem({10, 10, 12}));
    REQUIRE_THROWS(b.close());

    Containers::Dimensions item = b.takeItem();
    REQUIRE(item.getLength() == 10);
    REQUIRE(item.getWidth() == 10);
    REQUIRE(item.getHeight() == 12);

    b.putItem({5, 9, 10});
    REQUIRE_NOTHROW(b.close());
}

struct StderrReporter : public doctest::ConsoleReporter {
    StderrReporter(const doctest::ContextOptions &opt) : ConsoleReporter(opt, std::cerr) {
    }
};

struct FileReporter : public doctest::ConsoleReporter {
    FileReporter(const doctest::ContextOptions &opt) : ConsoleReporter(opt, *opt.cout) {
    }
};

REGISTER_REPORTER("stderr", 0, StderrReporter);
REGISTER_REPORTER("file", 0, FileReporter);

int main(int argc, char **argv) {
    doctest::Context context;
    context.applyCommandLine(argc, argv);

    return context.run();
}
