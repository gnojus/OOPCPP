#ifndef BOX_H
#define BOX_H

#include <string>

namespace Containers {

struct Dimensions {
    int length, width, height;
    std::string toString() const;
};

class Box {
   private:
    static int instanceCount;
    const int ID;
    bool isOpen, hasItem;
    Dimensions size, item;

   public:
    static const Dimensions SMALL, MEDIUM, LARGE;
    Box(Dimensions size = MEDIUM);
    ~Box();
    void open();
    void close();
    bool isFull() const;
    bool isClosed() const;
    void putItem(Dimensions item);
    Dimensions takeItem();
    std::string toString() const;
};

}  // namespace Containers

#endif /* BOX_H */
