#include "box.h"

#include <sstream>
#include <stdexcept>

namespace Containers {

using std::string;

namespace Errors {

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
} // namespace Dimensions

}  // namespace Errors

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

void Box::putItem(Dimensions item) {
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

void validateDimensions(Dimensions dimensions) {
    if (dimensions.length <= 0 || dimensions.width <= 0 || dimensions.height <= 0) {
        throw std::logic_error(Errors::Dimensions::INVALID);
    }
}

}  // namespace Containers
