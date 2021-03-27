#include <sstream>
#include <stdexcept>

#include "box.h"
#include "internal.h"

namespace Containers {

    using std::string;

    void validateDimensions(Dimensions dimensions);
    void checkInstance(void *instance, string filename, int line);

    int Box::BoxImpl::idCounter = 0;
    int Box::BoxImpl::instanceCounter = 0;

    Box::BoxImpl::BoxImpl(const Dimensions &size) : ID(this->idCounter) {
        validateDimensions(size);
        this->size = size;
        this->isOpen = false;
        this->hasItem = false;
        ++Box::BoxImpl::idCounter;
        ++Box::BoxImpl::instanceCounter;
    }

    Box::BoxImpl::BoxImpl(const BoxImpl &b) : ID(b.ID), isOpen(b.isOpen), hasItem(b.hasItem), size(b.size), item(b.item) {
        ++Box::BoxImpl::instanceCounter;
    }

    Box::BoxImpl::~BoxImpl() {
        --Box::BoxImpl::instanceCounter;
    }

    Box::Box() {
        impl = NULL;
    }

    Box::Box(const Dimensions &size) {
        impl = new BoxImpl(size);
    }

    Box::Box(const Box &b) {
        if (b.impl == NULL) {
            this->impl = NULL;
        } else {
            this->impl = new BoxImpl(*b.impl);
        }
    }

    Box::~Box() {
        delete impl;
    }

    Box &Box::operator=(const Box &b) {
        if (this == &b) {
            return *this;
        }
        checkInstance(b.impl, __FILE__, __LINE__);
        BoxImpl *tmp = new BoxImpl(*b.impl);
        delete this->impl;
        this->impl = tmp;

        return *this;
    }

    void Box::init(const Dimensions &size) {
        if (impl != NULL) {
            throw std::logic_error(Errors::Box::WRONG_INITIALIZATION);
        }
        impl = new BoxImpl(size);
    }

    int Box::getId() const {
        checkInstance(this->impl, __FILE__, __LINE__);
        return this->impl->ID;
    }

    void Box::open() {
        checkInstance(this->impl, __FILE__, __LINE__);
        if (impl->isOpen) {
            throw std::logic_error(Errors::Box::ALREADY_OPENED);
        }
        impl->isOpen = true;
    }

    void Box::close() {
        checkInstance(this->impl, __FILE__, __LINE__);
        if (!impl->isOpen) {
            throw std::logic_error(Errors::Box::ALREADY_CLOSED);
        }
        if (impl->hasItem && impl->item.getHeight() > impl->size.getHeight()) {
            throw std::logic_error(Errors::Box::ITEM_TOO_HIGH_TO_CLOSE);
        }
        impl->isOpen = false;
    }

    bool Box::isFull() const {
        checkInstance(this->impl, __FILE__, __LINE__);
        return impl->hasItem;
    }

    bool Box::isClosed() const {
        checkInstance(this->impl, __FILE__, __LINE__);
        return !impl->isOpen;
    }

    void Box::putItem(const Dimensions &item) {
        checkInstance(this->impl, __FILE__, __LINE__);
        validateDimensions(item);
        if (!impl->isOpen) {
            throw std::logic_error(Errors::Box::PUTING_TO_CLOSED);
        }
        if (impl->hasItem) {
            throw std::logic_error(Errors::Box::PUTING_TO_FULL);
        }
        if (impl->size.getLength() < item.getLength() || impl->size.getWidth() < item.getWidth()) {
            throw std::logic_error(Errors::Box::ITEM_DOES_NOT_FIT);
        }
        impl->item = item;
        impl->hasItem = true;
    }

    Dimensions Box::takeItem() {
        checkInstance(this->impl, __FILE__, __LINE__);
        if (!impl->isOpen) {
            throw std::logic_error(Errors::Box::TAKING_FROM_CLOSED);
        }
        if (!impl->hasItem) {
            throw std::logic_error(Errors::Box::TAKING_FROM_EMPTY);
        }
        impl->hasItem = false;
        return impl->item;
    }

    string Box::toString() const {
        checkInstance(this->impl, __FILE__, __LINE__);
        std::ostringstream output;
        output << Serialization::BEGIN_MARK;

        output << Serialization::Box::FIELD_ID << Serialization::VALUE_MARK << ' ';
        output << impl->ID << Serialization::VALUE_SEPARATOR << ' ';

        output << Serialization::Box::FIELD_IS_OPEN << Serialization::VALUE_MARK << ' ';
        output << (impl->isOpen ? Serialization::TRUE : Serialization::FALSE) << Serialization::VALUE_SEPARATOR << ' ';

        if (impl->hasItem) {
            output << Serialization::Box::FIELD_ITEM << Serialization::VALUE_MARK << ' ';
            output << impl->item << Serialization::VALUE_SEPARATOR << ' ';
        }

        output << Serialization::Box::FIELD_SIZE << Serialization::VALUE_MARK << ' ';
        output << impl->size;

        output << Serialization::END_MARK;
        return output.str();
    }

    std::ostream &operator<<(std::ostream &o, const Box &b) {
        checkInstance(b.impl, __FILE__, __LINE__);
        o << b.toString();
        return o;
    }

    std::istream &operator>>(std::istream &s, Box &b) {
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
            } else {
                throw std::logic_error(Errors::UNKNOWN_VALUE + " (" + name + ")");
            }
        } while (Serialization::readNextSeparator(s));
        s.flags(flags);
        Box tmp(size);
        tmp.impl->ID = ID;
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
        checkInstance(this->impl, __FILE__, __LINE__);
        Box copy = *this;
        ++(impl->ID);
        return copy;
    }

    Box &Box::operator++() {
        checkInstance(this->impl, __FILE__, __LINE__);
        ++(impl->ID);
        return *this;
    }

    bool Box::equals(const Box &b) const {
        checkInstance(this->impl, __FILE__, __LINE__);
        bool equal = true;
        equal &= impl->size == b.impl->size;
        equal &= this->isClosed() == b.isClosed();
        equal &= this->isFull() == b.isFull();
        equal &= this->getId() == b.getId();
        if (this->isFull() && b.isFull()) {
            equal &= impl->item == b.impl->item;
        }
        return equal;
    }

    bool Box::operator==(const Box &b) const {
        checkInstance(this->impl, __FILE__, __LINE__);
        checkInstance(b.impl, __FILE__, __LINE__);
        return impl->size.computeVolume() == b.impl->size.computeVolume();
    }

    bool Box::operator!=(const Box &b) const {
        return !(*this == b);
    }

    bool Box::operator<(const Box &b) const {
        checkInstance(this->impl, __FILE__, __LINE__);
        checkInstance(b.impl, __FILE__, __LINE__);
        return impl->size.computeVolume() < b.impl->size.computeVolume();
    }

    bool Box::operator<=(const Box &b) const {
        return *this < b || *this == b;
    }

    bool Box::operator>(const Box &b) const {
        return !(*this <= b);
    }

    bool Box::operator>=(const Box &b) const {
        return !(*this < b);
    }

    int Box::getCurrentInstances() {
        return BoxImpl::instanceCounter;
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
            throw std::invalid_argument(Errors::Dimensions::INVALID);
        }
    }

    void checkInstance(void *instance, string filename, int line) {
        if (instance == NULL) {
            std::ostringstream os;
            os << Errors::UNINITIALIZED_USAGE << " in " << filename << ":" << line;
            throw std::logic_error(os.str());
        }
    }
}
