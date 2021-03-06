#include "box.h"

#include <sstream>
#include <stdexcept>

namespace Containers {

using std::string;

namespace Errors {

const string INVALID_SYMBOL = "Invalid symbol in stream";
const string UNKNOWN_VALUE = "Unknown value in stream";

namespace Box {
const string ALREADY_OPENED = "Cannot open an already opened box";
const string ALREADY_CLOSED = "Cannot close an already closed box";
const string ITEM_TOO_HIGH_TO_CLOSE = "Cannot close box because item inside is too high";
const string PUTING_TO_CLOSED = "Cannot put an item into a closed box";
const string PUTING_TO_FULL = "Cannot put item into a full box";
const string ITEM_DOES_NOT_FIT = "Item does not fit into the box";
const string TAKING_FROM_CLOSED = "Cannot take an item from a closed box";
const string TAKING_FROM_EMPTY = "There is nothing to take from the box";
}  // namespace Box

namespace Dimensions {
const string INVALID = "Dimensions contains invalid (non-positive) value";
}  // namespace Dimensions

}  // namespace Errors

namespace Serialization {

const char BEGIN_MARK = '{';
const char END_MARK = '}';
const char VALUE_MARK = ':';
const char VALUE_SEPARATOR = ',';
const string TRUE = "true";
const string FALSE = "false";

namespace Box {
const string VALUE_NONE = "none";
const string FIELD_IS_OPEN = "is_open";
const string FIELD_ID = "id";
const string FIELD_SIZE = "size";
const string FIELD_ITEM = "item";
}  // namespace Box

namespace Dimensions {
const string FIELD_LENGTH = "length";
const string FIELD_WIDTH = "width";
const string FIELD_HEIGHT = "height";
}  // namespace Dimensions

}  // namespace Serialization

void readMark(std::istream &s, char mark);
bool readNextSeparator(std::istream &s);
string readValueName(std::istream &s);
void validateDimensions(Dimensions dimensions);

const Dimensions Box::SMALL = {10, 10, 10};
const Dimensions Box::MEDIUM = {20, 20, 10};
const Dimensions Box::LARGE = {30, 25, 20};

string Dimensions::toString() const {
    std::ostringstream output;
    output << Serialization::BEGIN_MARK;

    output << Serialization::Dimensions::FIELD_LENGTH << Serialization::VALUE_MARK << ' ' ;
    output << this->length << Serialization::VALUE_SEPARATOR << ' ';

    output << Serialization::Dimensions::FIELD_WIDTH << Serialization::VALUE_MARK << ' ';
    output << this->width << Serialization::VALUE_SEPARATOR << ' ';

    output << Serialization::Dimensions::FIELD_HEIGHT << Serialization::VALUE_MARK << ' ' ;
    output << this->height;

    output << Serialization::END_MARK;
    return output.str();
}

std::ostream& operator<<(std::ostream &o, const Dimensions &d) {
    o << d.toString();
    return o;
}

std::istream& operator>>(std::istream &s, Dimensions &d) {
    Dimensions tmp;
    readMark(s, Serialization::BEGIN_MARK);

    do {
        string name = readValueName(s);
        if (name == Serialization::Dimensions::FIELD_LENGTH) {
            s >> tmp.length;
        } else if (name == Serialization::Dimensions::FIELD_WIDTH) {
            s >> tmp.width;
        } else if (name == Serialization::Dimensions::FIELD_HEIGHT) {
            s >> tmp.height;
        } else {
            throw std::logic_error(Errors::UNKNOWN_VALUE + " (" + name + ")");
        }
    } while (readNextSeparator(s));
    d = tmp;
    return s;
}

bool Dimensions::operator==(const Dimensions &d) const {
    return this->length == d.length && this->width == d.width && this->height == d.height;
}

int Dimensions::volume() const {
    return this->length * this->width * this->height;
}

int Box::instanceCount = 0;

Box::Box(const Dimensions &size) : ID(this->instanceCount) {
    validateDimensions(size);
    this->size = size;
    this->isOpen = false;
    this->hasItem = false;
    ++(this->instanceCount);
}

Box::~Box() {
}

void Box::open() {
    if (this->isOpen) {
        throw std::logic_error(Errors::Box::ALREADY_OPENED);
    }
    this->isOpen = true;
}

void Box::close() {
    if (!this->isOpen) {
        throw std::logic_error(Errors::Box::ALREADY_CLOSED);
    }
    if (this->hasItem && this->item.height > this->size.height) {
        throw std::logic_error(Errors::Box::ITEM_TOO_HIGH_TO_CLOSE);
    }
    this->isOpen = false;
}

bool Box::isFull() const {
    return this->hasItem;
}

bool Box::isClosed() const {
    return !this->isOpen;
}

void Box::putItem(const Dimensions &item) {
    validateDimensions(item);
    if (!this->isOpen) {
        throw std::logic_error(Errors::Box::PUTING_TO_CLOSED);
    }
    if (this->hasItem) {
        throw std::logic_error(Errors::Box::PUTING_TO_FULL);
    }
    if (this->size.length < item.length || this->size.width < item.width) {
        throw std::logic_error(Errors::Box::ITEM_DOES_NOT_FIT);
    }
    this->item = item;
    this->hasItem = true;
}

Dimensions Box::takeItem() {
    if (!this->isOpen) {
        throw std::logic_error(Errors::Box::TAKING_FROM_CLOSED);
    }
    if (!this->hasItem) {
        throw std::logic_error(Errors::Box::TAKING_FROM_EMPTY);
    }
    this->hasItem = false;
    return this->item;
}

string Box::toString() const {
    std::ostringstream output;
    output << Serialization::BEGIN_MARK;

    output << Serialization::Box::FIELD_ID << Serialization::VALUE_MARK << ' ' ;
    output << this->ID << Serialization::VALUE_SEPARATOR << ' ';

    output << Serialization::Box::FIELD_IS_OPEN << Serialization::VALUE_MARK << ' ';
    output << (this->isOpen ? Serialization::TRUE : Serialization::FALSE) << Serialization::VALUE_SEPARATOR << ' ';

    if (this->hasItem) {
        output << Serialization::Box::FIELD_ITEM << Serialization::VALUE_MARK << ' ';
        output << this->item << Serialization::VALUE_SEPARATOR << ' ';
    }

    output << Serialization::Box::FIELD_SIZE << Serialization::VALUE_MARK << ' ' ;
    output << this->size;

    output << Serialization::END_MARK;
    return output.str();
}

std::ostream& operator<<(std::ostream &o, const Box &b) {
    o << b.toString();
    return o;
}

std::istream& operator>>(std::istream &s, Box &b) {
    Box tmp;
    tmp.open();
    bool leaveOpen = false, putItem = false;
    Dimensions item;
    readMark(s, Serialization::BEGIN_MARK);
    std::ios_base::fmtflags flags = s.flags();
    s.flags(flags | std::ios::boolalpha);

    do {
        string name = readValueName(s);
        if (name == Serialization::Box::FIELD_IS_OPEN) {
            s >> leaveOpen;
        } else if (name == Serialization::Box::FIELD_SIZE) {
            s >> tmp.size;
            validateDimensions(tmp.size);
        } else if (name == Serialization::Box::FIELD_ITEM) {
            s >> item;
            putItem = true;
        } else if (name == Serialization::Box::FIELD_ID) {
            s >> tmp.ID;
        } else {
            throw std::logic_error(Errors::UNKNOWN_VALUE + " (" + name + ")");
        }
    } while (readNextSeparator(s));
    s.flags(flags);
    if (putItem) {
        tmp.putItem(item);
    }
    if (!leaveOpen) {
        tmp.close();
    }
    b = tmp;
    return s;
}

Box Box::operator++(int) {
    Box copy = *this;
    this->ID = Box::instanceCount++;
    return copy;
}

Box& Box::operator++() {
    this->ID = Box::instanceCount++;
    return *this;
    Dimensions a, b;
}

bool Box::operator==(const Box &d) const {
    bool equal = true;
    equal &= this->size == d.size;
    equal &= this->isClosed() == d.isClosed();
    equal &= this->isFull() == d.isFull();
    if (this->isFull() && d.isFull()) {
        equal &= this->item == d.item;
    }
    return equal;
}

bool Box::operator!=(const Box &d) const {
    return !(*this == d);
}

bool Box::operator<(const Box &d) const {
    return this->size.volume() < d.size.volume();
}

bool Box::operator<=(const Box &d) const {
    return this->size.volume() <= d.size.volume();
}

bool Box::operator>(const Box &d) const {
    return this->size.volume() > d.size.volume();
}

bool Box::operator>=(const Box &d) const {
    return this->size.volume() >= d.size.volume();
}

void readMark(std::istream &s, char mark) {
    char tmp;
    s >> tmp;
    if (tmp != mark) {
        throw std::logic_error(Errors::INVALID_SYMBOL + " (" + tmp + ")");
    }
}

bool readNextSeparator(std::istream &s) {
    char tmp;
    s >> tmp;
    if (tmp != Serialization::END_MARK && tmp != Serialization::VALUE_SEPARATOR) {
        throw std::logic_error(Errors::INVALID_SYMBOL + " (" + tmp + ")");
    }
    return tmp == Serialization::VALUE_SEPARATOR;
}

string readValueName(std::istream &s) {
    string tmp;
    s >> tmp;
    if (!tmp.empty() && tmp.back() == Serialization::VALUE_MARK) {
        tmp.pop_back();
    } else {
        readMark(s, Serialization::VALUE_MARK);
    }
    return tmp;
}

void validateDimensions(Dimensions dimensions) {
    if (dimensions.length <= 0 || dimensions.width <= 0 || dimensions.height <= 0) {
        throw std::logic_error(Errors::Dimensions::INVALID);
    }
}

}  // namespace Containers
