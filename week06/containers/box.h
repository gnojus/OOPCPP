#ifndef BOX_H
#define BOX_H

#include <string>

#include "dimensions.h"

namespace Containers {
    class Box {
       private:
        class BoxImpl;
        BoxImpl *impl;

       public:
        // Lazy initialization of the Box. init must be called before using.
        Box();
        Box(const Dimensions &size);
        Box(const Box &b);
        ~Box();
        Box &operator=(const Box &b);
        void init(const Dimensions &size);
        int getId() const;
        void open();
        void close();
        bool isFull() const;
        bool isClosed() const;
        void putItem(const Dimensions &item);
        Dimensions takeItem();
        std::string toString() const;
        friend std::ostream &operator<<(std::ostream &o, const Box &b);

        // Reads Box from stream, using the toString() format. May throw exceptions,
        // but strong exception safety is guaranteed.
        friend std::istream &operator>>(std::istream &s, Box &b);

        // Post increment increments (sets to smallest unused) ID of this object and returns old copy.
        Box operator++(int);

        // Pre increment increments (sets to smallest unused) ID of this object and returns this object.
        Box &operator++();

        // Checks for complete box equality (size, item, is opened/close)..
        bool equals(const Box &b) const;

        // Comparison operators compares only the volume of the boxes.
        bool operator==(const Box &b) const;
        bool operator!=(const Box &b) const;
        bool operator<(const Box &b) const;
        bool operator<=(const Box &b) const;
        bool operator>(const Box &b) const;
        bool operator>=(const Box &b) const;
    };

}

#endif /* BOX_H */
