#include "internal.h"

namespace Containers {

    using std::string;

    namespace Errors {

        const string INVALID_SYMBOL = "Invalid symbol in stream";
        const string UNKNOWN_VALUE = "Unknown value in stream";

        const string UNINITIALIZED_USAGE = "Attempted to use an uninitialized object";

        namespace Box {
            const string WRONG_INITIALIZATION = "Attempted to initialize an already initialized box";
            const string ALREADY_OPENED = "Cannot open an already opened box";
            const string ALREADY_CLOSED = "Cannot close an already closed box";
            const string ITEM_TOO_HIGH_TO_CLOSE = "Cannot close box because item inside is too high";
            const string PUTING_TO_CLOSED = "Cannot put an item into a closed box";
            const string PUTING_TO_FULL = "Cannot put item into a full box";
            const string ITEM_DOES_NOT_FIT = "Item does not fit into the box";
            const string TAKING_FROM_CLOSED = "Cannot take an item from a closed box";
            const string TAKING_FROM_EMPTY = "There is nothing to take from the box";
        }

        namespace Dimensions {
            const string INVALID = "Dimensions contains invalid (non-positive) value";
        }

    }

    namespace Serialization {

        const char BEGIN_MARK = '{';
        const char END_MARK = '}';
        const char VALUE_MARK = ':';
        const char VALUE_SEPARATOR = ',';
        const string TRUE = "true";
        const string FALSE = "false";

        namespace Box {
            const string VALUE_NONE = "none";
            const string FIELD_IS_OPEN = "is_open";
            const string FIELD_ID = "id";
            const string FIELD_SIZE = "size";
            const string FIELD_ITEM = "item";
        }

        namespace Dimensions {
            const string FIELD_LENGTH = "length";
            const string FIELD_WIDTH = "width";
            const string FIELD_HEIGHT = "height";
        }
    }
}