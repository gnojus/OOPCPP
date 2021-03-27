#include <sstream>

#include "dimensions.h"
#include "internal.h"

namespace Containers {
    using std::string;

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

    int Dimensions::computeVolume() const {
        return this->length * this->width * this->height;
    }
}