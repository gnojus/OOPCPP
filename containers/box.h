#ifndef BOX_H
#define BOX_H

#include <string>

#include "dimensions.h"

namespace Containers {

    /** Box is rectangular container that can hold at most one item at a time */
    class Box {
       private:
        class BoxImpl;
        BoxImpl *impl;

       public:
        /** Lazy initialization of the Box. init must be called before using. */
        Box();

        /** Constructs a Box using given dimensions
         * @param size the dimensions, all of them must be positive
         */
        Box(const Dimensions &size);
        Box(const Box &b);
        ~Box();
        Box &operator=(const Box &b);

        /** Initializes a Box.
         * @see Box(const Dimensions &);
         */
        void init(const Dimensions &size);
        int getId() const;
        void open();
        void close();
        bool isFull() const;

        bool isClosed() const;

        /** Puts an item into the Box, which must be empty and opened.
         * @param item the item to be placed inside
         */
        void putItem(const Dimensions &item);

        /** Takes the item from Box, which must contain an item and be opened.
         * On success the box is emptied
         * @return the item taken
         */
        Dimensions takeItem();

        std::string toString() const;
        friend std::ostream &operator<<(std::ostream &o, const Box &b);

        /** Reads Box from stream, using the toString() format. May throw exceptions,
         *  but strong exception safety is guaranteed. */
        friend std::istream &operator>>(std::istream &s, Box &b);

        /** Post increment increments (sets to smallest unused) ID of this
         *  object and returns old copy. */
        Box operator++(int);

        /** Pre increment increments (sets to smallest unused) ID of this object
         *  and returns this object. */
        Box &operator++();

        /** Checks for complete box equality (size, item, is opened/close)..
         * @param b The box to compare with
         * @return whether the Boxes are equal
         */
        bool equals(const Box &b) const;

        /* Comparison operators compares only the volume of the boxes. */
        bool operator==(const Box &b) const;
        bool operator!=(const Box &b) const;
        bool operator<(const Box &b) const;
        bool operator<=(const Box &b) const;
        bool operator>(const Box &b) const;
        bool operator>=(const Box &b) const;
    };

}

#endif /* BOX_H */
