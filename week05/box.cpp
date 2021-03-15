#include "box.h"

#include <sstream>
#include <stdexcept>

namespace Containers {

    using std::string;

    namespace Errors {

        const string INVALID_SYMBOL = "Invalid symbol in stream";
        const string UNKNOWN_VALUE = "Unknown value in stream";

        const string UNINITIALIZED_USAGE = "Attempted to use an uninitialized object";

        namespace Box {
            const string WRONG_INITIALIZATION = "Attempted to initialize an already initialized box";
            const string ALREADY_OPENED = "Cannot open an already opened box";
            const string ALREADY_CLOSED = "Cannot close an already closed box";
            const string ITEM_TOO_HIGH_TO_CLOSE = "Cannot close box because item inside is too high";
            const string PUTING_TO_CLOSED = "Cannot put an item into a closed box";
            const string PUTING_TO_FULL = "Cannot put item into a full box";
            const string ITEM_DOES_NOT_FIT = "Item does not fit into the box";
            const string TAKING_FROM_CLOSED = "Cannot take an item from a closed box";
            const string TAKING_FROM_EMPTY = "There is nothing to take from the box";
        }

        namespace Dimensions {
            const string INVALID = "Dimensions contains invalid (non-positive) value";
        }

    }

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
        }

        namespace Dimensions {
            const string FIELD_LENGTH = "length";
            const string FIELD_WIDTH = "width";
            const string FIELD_HEIGHT = "height";
        }

        void readMark(std::istream &s, char mark);
        bool readNextSeparator(std::istream &s);
        string readValueName(std::istream &s);
    }

    void validateDimensions(Dimensions dimensions);
    void checkInstance(void *instance, std::string filename, int line);

    const Dimensions Box::SMALL = {10, 10, 10};
    const Dimensions Box::MEDIUM = {20, 20, 10};
    const Dimensions Box::LARGE = {30, 25, 20};

    Dimensions::Dimensions() : Dimensions(0, 0, 0) {
    }

    Dimensions::Dimensions(int length, int width, int height) {
        this->setLength(length);
        this->setWidth(width);
        this->setHeight(height);
    }

    void Dimensions::setLength(int length) {
        this->length = length;
    }

    void Dimensions::setWidth(int width) {
        this->width = width;
    }

    void Dimensions::setHeight(int height) {
        this->height = height;
    }

    int Dimensions::getLength() const {
        return this->length;
    }

    int Dimensions::getWidth() const {
        return this->width;
    }

    int Dimensions::getHeight() const {
        return this->height;
    }

    string Dimensions::toString() const {
        std::ostringstream output;
        output << Serialization::BEGIN_MARK;

        output << Serialization::Dimensions::FIELD_LENGTH << Serialization::VALUE_MARK << ' ';
        output << this->length << Serialization::VALUE_SEPARATOR << ' ';

        output << Serialization::Dimensions::FIELD_WIDTH << Serialization::VALUE_MARK << ' ';
        output << this->width << Serialization::VALUE_SEPARATOR << ' ';

        output << Serialization::Dimensions::FIELD_HEIGHT << Serialization::VALUE_MARK << ' ';
        output << this->height;

        output << Serialization::END_MARK;
        return output.str();
    }

    std::ostream &operator<<(std::ostream &o, const Dimensions &d) {
        o << d.toString();
        return o;
    }

    std::istream &operator>>(std::istream &s, Dimensions &d) {
        Dimensions tmp;
        Serialization::readMark(s, Serialization::BEGIN_MARK);

        do {
            string name = Serialization::readValueName(s);
            if (name == Serialization::Dimensions::FIELD_LENGTH) {
                s >> tmp.length;
            } else if (name == Serialization::Dimensions::FIELD_WIDTH) {
                s >> tmp.width;
            } else if (name == Serialization::Dimensions::FIELD_HEIGHT) {
                s >> tmp.height;
            } else {
                throw std::logic_error(Errors::UNKNOWN_VALUE + " (" + name + ")");
            }
        } while (Serialization::readNextSeparator(s));
        d = tmp;
        return s;
    }

    bool Dimensions::operator==(const Dimensions &d) const {
        return this->length == d.length && this->width == d.width && this->height == d.height;
    }

    int Dimensions::volume() const {
        return this->length * this->width * this->height;
    }

    class Box::InnerBox {
       private:
        static int instanceCount;
        int ID;
        bool isOpen, hasItem;
        Dimensions size, item;

        InnerBox(const Dimensions &size);
        ~InnerBox();
    
        friend Box;
    };

    int Box::InnerBox::instanceCount = 0;

    Box::InnerBox::InnerBox(const Dimensions &size) : ID(this->instanceCount) {
        validateDimensions(size);
        this->size = size;
        this->isOpen = false;
        this->hasItem = false;
        ++(this->instanceCount);
    }

    Box::InnerBox::~InnerBox() {
    }

    Box::Box() {
        inner = NULL;
    }

    Box::Box(const Dimensions &size) {
        inner = new InnerBox(size);
    }

        Box::Box(const Box &b) {
            if (b.inner == NULL) {
                this->inner = NULL;
            } else {
                this->inner = new InnerBox(*b.inner);
            }
        }


    Box::~Box() {
        delete inner;
    }

    Box& Box::operator=(const Box &b) {
        checkInstance(b.inner, __FILE__, __LINE__);
        InnerBox *tmp = new InnerBox(*b.inner);
        delete this->inner;
        this->inner = tmp;

        return *this;
    }

    void Box::init(const Dimensions &size) {
        if (inner != NULL) {
            throw std::logic_error(Errors::Box::WRONG_INITIALIZATION);
        }
        inner = new InnerBox(size);
    }

    void Box::open() {
        checkInstance(this->inner, __FILE__, __LINE__);
        if (inner->isOpen) {
            throw std::logic_error(Errors::Box::ALREADY_OPENED);
        }
        inner->isOpen = true;
    }

    void Box::close() {
        checkInstance(this->inner, __FILE__, __LINE__);
        if (!inner->isOpen) {
            throw std::logic_error(Errors::Box::ALREADY_CLOSED);
        }
        if (inner->hasItem && inner->item.getHeight() > inner->size.getHeight()) {
            throw std::logic_error(Errors::Box::ITEM_TOO_HIGH_TO_CLOSE);
        }
        inner->isOpen = false;
    }

    bool Box::isFull() const {
        checkInstance(this->inner, __FILE__, __LINE__);
        return inner->hasItem;
    }

    bool Box::isClosed() const {
        checkInstance(this->inner, __FILE__, __LINE__);
        return !inner->isOpen;
    }

    void Box::putItem(const Dimensions &item) {
        checkInstance(this->inner, __FILE__, __LINE__);
        validateDimensions(item);
        if (!inner->isOpen) {
            throw std::logic_error(Errors::Box::PUTING_TO_CLOSED);
        }
        if (inner->hasItem) {
            throw std::logic_error(Errors::Box::PUTING_TO_FULL);
        }
        if (inner->size.getLength() < item.getLength() || inner->size.getWidth() < item.getWidth()) {
            throw std::logic_error(Errors::Box::ITEM_DOES_NOT_FIT);
        }
        inner->item = item;
        inner->hasItem = true;
    }

    Dimensions Box::takeItem() {
        checkInstance(this->inner, __FILE__, __LINE__);
        if (!inner->isOpen) {
            throw std::logic_error(Errors::Box::TAKING_FROM_CLOSED);
        }
        if (!inner->hasItem) {
            throw std::logic_error(Errors::Box::TAKING_FROM_EMPTY);
        }
        inner->hasItem = false;
        return inner->item;
    }

    string Box::toString() const {
        checkInstance(this->inner, __FILE__, __LINE__);
        std::ostringstream output;
        output << Serialization::BEGIN_MARK;

        output << Serialization::Box::FIELD_ID << Serialization::VALUE_MARK << ' ';
        output << inner->ID << Serialization::VALUE_SEPARATOR << ' ';

        output << Serialization::Box::FIELD_IS_OPEN << Serialization::VALUE_MARK << ' ';
        output << (inner->isOpen ? Serialization::TRUE : Serialization::FALSE) << Serialization::VALUE_SEPARATOR << ' ';

        if (inner->hasItem) {
            output << Serialization::Box::FIELD_ITEM << Serialization::VALUE_MARK << ' ';
            output << inner->item << Serialization::VALUE_SEPARATOR << ' ';
        }

        output << Serialization::Box::FIELD_SIZE << Serialization::VALUE_MARK << ' ';
        output << inner->size;

        output << Serialization::END_MARK;
        return output.str();
    }

    std::ostream &operator<<(std::ostream &o, const Box &b) {
        checkInstance(b.inner, __FILE__, __LINE__);
        o << b.toString();
        return o;
    }

    std::istream &operator>>(std::istream &s, Box &b) {
        checkInstance(b.inner, __FILE__, __LINE__);
        bool leaveOpen = false, putItem = false;
        int ID;
        Dimensions item, size;
        Serialization::readMark(s, Serialization::BEGIN_MARK);
        std::ios_base::fmtflags flags = s.flags();
        s.flags(flags | std::ios::boolalpha);

        do {
            string name = Serialization::readValueName(s);
            if (name == Serialization::Box::FIELD_IS_OPEN) {
                s >> leaveOpen;
            } else if (name == Serialization::Box::FIELD_SIZE) {
                s >> size;
                validateDimensions(size);
            } else if (name == Serialization::Box::FIELD_ITEM) {
                s >> item;
                putItem = true;
            } else if (name == Serialization::Box::FIELD_ID) {
                s >> ID;
                // skip id for now 
            } else {
                throw std::logic_error(Errors::UNKNOWN_VALUE + " (" + name + ")");
            }
        } while (Serialization::readNextSeparator(s));
        s.flags(flags);
        Box tmp(size);
        tmp.open();
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
        checkInstance(this->inner, __FILE__, __LINE__);
        Box copy = *this;
        inner->ID = InnerBox::instanceCount++;
        return copy;
    }

    Box &Box::operator++() {
        checkInstance(this->inner, __FILE__, __LINE__);
        inner->ID = InnerBox::instanceCount++;
        return *this;
        Dimensions a, b;
    }

    bool Box::equals(const Box &d) const {
        checkInstance(this->inner, __FILE__, __LINE__);
        bool equal = true;
        equal &= inner->size == d.inner->size;
        equal &= this->isClosed() == d.isClosed();
        equal &= this->isFull() == d.isFull();
        if (this->isFull() && d.isFull()) {
            equal &= inner->item == d.inner->item;
        }
        return equal;
    }

    bool Box::operator==(const Box &d) const {
        checkInstance(this->inner, __FILE__, __LINE__);
        checkInstance(d.inner, __FILE__, __LINE__);
        return inner->size.volume() == d.inner->size.volume();
    }

    bool Box::operator!=(const Box &d) const {
        return !(*this == d);
    }

    bool Box::operator<(const Box &d) const {
        checkInstance(this->inner, __FILE__, __LINE__);
        checkInstance(d.inner, __FILE__, __LINE__);
        return inner->size.volume() < d.inner->size.volume();
    }

    bool Box::operator<=(const Box &d) const {
        return *this < d || *this == d;
    }

    bool Box::operator>(const Box &d) const {
        return !(*this <= d);
    }

    bool Box::operator>=(const Box &d) const {
        return !(*this < d);
    }

    void Serialization::readMark(std::istream &s, char mark) {
        char tmp;
        s >> tmp;
        if (tmp != mark) {
            throw std::logic_error(Errors::INVALID_SYMBOL + " (" + tmp + ")");
        }
    }

    bool Serialization::readNextSeparator(std::istream &s) {
        char tmp;
        s >> tmp;
        if (tmp != Serialization::END_MARK && tmp != Serialization::VALUE_SEPARATOR) {
            throw std::logic_error(Errors::INVALID_SYMBOL + " (" + tmp + ")");
        }
        return tmp == Serialization::VALUE_SEPARATOR;
    }

    string Serialization::readValueName(std::istream &s) {
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
        if (dimensions.getLength() <= 0 || dimensions.getWidth() <= 0 || dimensions.getHeight() <= 0) {
            throw std::logic_error(Errors::Dimensions::INVALID);
        }
    }

    void checkInstance(void *instance, std::string filename, int line) {
        if (instance == NULL) {
            std::ostringstream os;
            os << Errors::UNINITIALIZED_USAGE << " in " << filename << ":" << line;
            throw std::logic_error(os.str());
        }
    }
}
