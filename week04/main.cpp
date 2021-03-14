#include <stdexcept>
#include <iostream>
#include <sstream>
#include "box.h"

using std::cout;
using std::cin;

int main() {
    Containers::Box box(Containers::Box::SMALL);
    Containers::Dimensions item(5, 8, 7);
    box.open();
    box.putItem(item);
    box.close();
    cout << "Successfully put item into the box!\n";
    cout << "Box: " << box << '\n';
    Containers::Box boxFromStream;
    std::stringstream ss;
    ss << box;
    ss >> boxFromStream;
    cout << "Reproduced box from stream: " << boxFromStream << '\n';
    if (boxFromStream == box) {
        cout << "And it equals to the original one!\n";
    }

    boxFromStream.open();
    if (boxFromStream != box) {
        cout << "And after opening it's not equal\n";
    }

    cout << std::endl;

    cout << "Box: " << box << '\n';
    cout << "Box++: " << box++ << '\n';
    cout << "Box: " << box << '\n';
    cout << "++Box: " << ++box << '\n';
    cout << "Box: " << box << '\n';

    cout << std::endl;

    cout << "Large box is " << (Containers::Box(Containers::Box::LARGE) > Containers::Box(Containers::Box::MEDIUM) ? "" : "not ") << "bigger than a small one.\n";

    cout << std::endl;

    Containers::Box boxes[] = {Containers::Box::SMALL, Containers::Box::MEDIUM, Containers::Box::LARGE, Containers::Box({50, 50, 50})}; 
    const int length = sizeof(boxes) / sizeof(*boxes);

    boxes[0].open();

    boxes[1].open();
    boxes[1].putItem(item);

    boxes[3].open();

    Containers::Dimensions bigItem = {23, 34, 25};

    for (int i = 0; i < length; ++i) {
        try {
            boxes[i].putItem(bigItem);
            cout << "Successfully put item into the box i = " << i << '\n';
            boxes[i].close();
            cout << boxes[i] << '\n';
        } catch(std::exception &e) {
            cout << "Failed to put item into the box i = " << i << ": " << e.what() << '\n';
        }
    }

    cout << std::endl;

    Containers::Box *flatBox = new Containers::Box({40, 40, 5});

    try {
        boxes[3].open();
        Containers::Dimensions item = boxes[3].takeItem();

        flatBox->open();
        flatBox->putItem(item);

        boxes[3].close();
        flatBox->close();
    } catch(std::exception &e) {
        cout << "Exception: " << e.what() << '\n';
    }

    try {
        Containers::Box tmp;
        cout << "Enter a box\n";
        cin >> tmp;
        cout << "Box entered: " << tmp << '\n';
    } catch (std::exception &e) {
        cout << "Failed to enter: " << e.what() << '\n';
    }

    delete flatBox;
}