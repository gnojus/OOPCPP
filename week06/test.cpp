#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <fstream>

#include "containers/box.h"
#include "containers/dimensions.h"

TEST_CASE("testing dimension class") {
    Containers::Dimensions d;
    d.setLength(10);
    d.setWidth(20);
    d.setHeight(30);
    REQUIRE(d.getLength() == 10);
    REQUIRE(d.getWidth() == 20);
    REQUIRE(d.getHeight() == 30);
}

TEST_CASE("testing box class") {
    Containers::Box b;
    REQUIRE_THROWS(b.open());
    REQUIRE_THROWS(b.init({10, 20, 0}));
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

    int res = context.run();

    if (context.shouldExit()) {
        return res;
    }

    return 0;
}
