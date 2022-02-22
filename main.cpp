#include <iostream>
#include <cstring>
#include "LinkedList.h"

int main() {
    LinkedList xorLinkedList;

    int data;

    char option[100] = {};
    while (std::cin >> option) {
        if (strcmp(option, "ACTUAL") == 0) {
            xorLinkedList.ACTUAL();
            std::cout << "\n";
        }
        else if (strcmp(option, "NEXT") == 0) {
            xorLinkedList.NEXT();
            std::cout << "\n";
        }
        else if (strcmp(option, "PREV") == 0) {
            xorLinkedList.PREV();
            std::cout << "\n";
        }
        else if (strcmp(option, "ADD_BEG") == 0) {
            std::cin >> data;
            xorLinkedList.ADD_BEG(data);
        }
        else if (strcmp(option, "ADD_END") == 0) {
            std::cin >> data;
            xorLinkedList.ADD_END(data);
        }
        else if (strcmp(option, "ADD_ACT") == 0) {
            std::cin >> data;
            xorLinkedList.ADD_ACT(data);
        }
        else if (strcmp(option, "DEL_BEG") == 0) {
            xorLinkedList.DEL_BEG();
        }
        else if (strcmp(option, "DEL_END") == 0) {
            xorLinkedList.DEL_END();
        }
        else if (strcmp(option, "DEL_VAL") == 0) {
            std::cin >> data;
            xorLinkedList.DEL_VAL(data);
        }
        else if (strcmp(option, "DEL_ACT") == 0) {
            xorLinkedList.DEL_ACT();
        }
        else if (strcmp(option, "PRINT_FORWARD") == 0) {
            xorLinkedList.PRINT_FORWARD();
            std::cout << "\n";
        }
        else if (strcmp(option, "PRINT_BACKWARD") == 0) {
            xorLinkedList.PRINT_BACKWARD();
            std::cout << "\n";
        }
    }


    return 0;
}