#ifndef INTERNAL_H
#define INTERNAL_H

#include <iostream>
#include <string>

#include "box.h"

namespace Containers {

    using std::string;

    namespace Errors {

        extern const string INVALID_SYMBOL;
        extern const string UNKNOWN_VALUE;

        extern const string UNINITIALIZED_USAGE;

        namespace Box {
            extern const string WRONG_INITIALIZATION;
            extern const string ALREADY_OPENED;
            extern const string ALREADY_CLOSED;
            extern const string ITEM_TOO_HIGH_TO_CLOSE;
            extern const string PUTING_TO_CLOSED;
            extern const string PUTING_TO_FULL;
            extern const string ITEM_DOES_NOT_FIT;
            extern const string TAKING_FROM_CLOSED;
            extern const string TAKING_FROM_EMPTY;
        }

        namespace Dimensions {
            extern const string INVALID;
        }

    }

    namespace Serialization {

        extern const char BEGIN_MARK;
        extern const char END_MARK;
        extern const char VALUE_MARK;
        extern const char VALUE_SEPARATOR;
        extern const string TRUE;
        extern const string FALSE;

        namespace Box {
            extern const string VALUE_NONE;
            extern const string FIELD_IS_OPEN;
            extern const string FIELD_ID;
            extern const string FIELD_SIZE;
            extern const string FIELD_ITEM;
        }

        namespace Dimensions {
            extern const string FIELD_LENGTH;
            extern const string FIELD_WIDTH;
            extern const string FIELD_HEIGHT;
        }

        void readMark(std::istream &s, char mark);
        bool readNextSeparator(std::istream &s);
        string readValueName(std::istream &s);
    }

    class Box::BoxImpl {
       private:
        static int instanceCount;
        int ID;
        bool isOpen, hasItem;
        Dimensions size, item;

        BoxImpl(const Dimensions &size);
        ~BoxImpl();

        friend Box;
    };

}

#endif /* INTERNAL_H */
