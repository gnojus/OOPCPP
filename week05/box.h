#ifndef BOX_H
#define BOX_H

#include <istream>
#include <string>

namespace Containers {

    class Dimensions {
       private:
        int length, width, height;
       public:
        Dimensions();
        Dimensions(int length, int width, int height);

        void setLength(int length);
        void setWidth(int width);
        void setHeight(int height);

        int getLength() const;
        int getWidth() const;
        int getHeight() const;

        std::string toString() const;
        friend std::ostream &operator<<(std::ostream &o, const Dimensions &d);
        friend std::istream &operator>>(std::istream &o, Dimensions &d);
        bool operator==(const Dimensions &d) const;
        int volume() const;
    };

    class Box {
       private:
        class InnerBox;
        InnerBox *inner;

       public:
        static const Dimensions SMALL, MEDIUM, LARGE;
        Box();
        Box(const Dimensions &size);
        Box(const Box &b);
        ~Box();
        Box& operator=(const Box &d);
        void init(const Dimensions &size = Box::MEDIUM);
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
        bool equals(const Box &d) const;

        // Comparison operators compares only the volume of the boxes.
        bool operator==(const Box &d) const;
        bool operator!=(const Box &d) const;
        bool operator<(const Box &d) const;
        bool operator<=(const Box &d) const;
        bool operator>(const Box &d) const;
        bool operator>=(const Box &d) const;
    };

}

#endif /* BOX_H */