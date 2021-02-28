#include "box.h"

#include <stdexcept>
#include <sstream>

namespace Containers {

const Dimensions Box::SMALL = {10, 10, 10};
const Dimensions Box::MEDIUM = {20, 20, 10};
const Dimensions Box::LARGE = {30, 25, 20};

std::string Dimensions::toString() const {
    std::stringstream output;
    output << "length: " << this->length << ", width: " << this->width << ", height: " << this->height;
    return output.str();
}

void validateDimensions(Dimensions dimensions);

int Box::instanceCount = 0;

Box::Box(Dimensions size) : ID(this->instanceCount) {
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
        throw std::logic_error("Cannot open an already opened box");
    }
    this->isOpen = true;
}

void Box::close() {
    if (!this->isOpen) {
        throw std::logic_error("Cannot close an already closed box");
    }
    if (this->hasItem && this->item.height > this->size.height) {
        throw std::logic_error("Cannot close box because item inside is too high");
    }
    this->isOpen = false;
}

void Box::putItem(Dimensions item) {
    validateDimensions(item);
    if (!this->isOpen) {
        throw std::logic_error("Cannot put an item into a closed box");
    }
    if (this->hasItem) {
        throw std::logic_error("Cannot put item into a full box");
    }
    if (this->size.length < item.length || this->size.width < item.width) {
        throw std::logic_error("Item does not fit into the box");
    }
    this->item = item;
    this->hasItem = true;
}

Dimensions Box::takeItem() {
    if (!this->isOpen) {
        throw std::logic_error("Cannot take an item from a closed box");
    }
    if (!this->hasItem) {
        throw std::logic_error("There is nothing to take from the box");
    }
    this->hasItem = false;
    return this->item;
}

void validateDimensions(Dimensions dimensions) {
    if (dimensions.length <= 0 || dimensions.width <= 0 || dimensions.height <= 0) {
        throw std::logic_error("Dimensions are invalid");
    }
}

bool Box::isFull() const {
    return this->hasItem;
}

bool Box::isClosed() const {
    return !this->isOpen;
}

std::string Box::toString() const {
    std::stringstream output;
    output << (this->isOpen ? "Opened" : "Closed");
    output << " Box no. " << this->ID << ", size - " << this->size.toString() << ", ";
    if (this->hasItem) {
        output << "with item - " << this->item.toString();
    } else {
        output << "without any item";
    }
    return output.str();
}

}  // namespace Containers
