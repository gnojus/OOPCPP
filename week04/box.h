#ifndef BOX_H
#define BOX_H

#include <string>
#include <istream>

namespace Containers {

struct Dimensions {
    int length, width, height;
    std::string toString() const;
    friend std::ostream& operator<<(std::ostream &o, const Dimensions &d);
    friend std::istream& operator>>(std::istream &o, Dimensions &d);
    bool operator==(const Dimensions &d) const;
    int volume() const;
};

class Box {
   private:
    static int instanceCount;
    int ID;
    bool isOpen, hasItem;
    Dimensions size, item;

   public:
    static const Dimensions SMALL, MEDIUM, LARGE;
    Box(const Dimensions &size = MEDIUM);
    ~Box();
    void open();
    void close();
    bool isFull() const;
    bool isClosed() const;
    void putItem(const Dimensions &item);
    Dimensions takeItem();
    std::string toString() const;
    friend std::ostream& operator<<(std::ostream &o, const Box &b);

    // Reads Box from stream, using the toString() format. May throw exceptions,
    // but strong exception safety is guaranteed. 
    friend std::istream& operator>>(std::istream &s, Box &b);

    // Post increment increments (sets to smallest unused) ID of this object and returns old copy.
    Box operator++(int);

    // Pre increment increments (sets to smallest unused) ID of this object and returns this object.
    Box& operator++();

    // Equality operators checks for complete box equality (size, item, is opened/close)..
    bool operator==(const Box &d) const;
    bool operator!=(const Box &d) const;

    // Comparison operators compares only the volume of the boxes.
    bool operator<(const Box &d) const;
    bool operator<=(const Box &d) const;
    bool operator>(const Box &d) const;
    bool operator>=(const Box &d) const;
};

}  // namespace Containers

#endif /* BOX_H */
