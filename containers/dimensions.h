#ifndef DIMENSIONS_H
#define DIMENSIONS_H

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
        friend std::istream &operator>>(std::istream &i, Dimensions &d);
        bool operator==(const Dimensions &d) const;
        int computeVolume() const;
    };

}
#endif /* DIMENSIONS_H */
