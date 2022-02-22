#ifndef XOR_LINKED_LIST_LINKEDLIST_H
#define XOR_LINKED_LIST_LINKEDLIST_H

#include "Node.h"

class LinkedList {
    Node* head = nullptr;
    Node* last = head;
    Node* actual = head;


public:
    // XOR operation on addresses, returning pointer to Node
    static Node* XOR(Node* a, Node* b) {
        return (Node*)((uintptr_t)a ^ (uintptr_t)b);
    }

    // Printing actual value
    void ACTUAL() const {
        if (actual != nullptr) {
            std::cout << actual->data << " ";
        }
        else {
            std::cout << "NULL ";
        }
    }

    // Setting actual to next Node
    void NEXT() {
        if (actual == nullptr) { // If actual is not set
            std::cout << "NULL ";
            return;
        }
        else if (actual == last) { // If actual is the last element of the list
            actual = head;
        }
        else { // If actual is inside the list
            Node* curr = head;
            Node* prev = nullptr;
            Node* next;

            while (curr != actual) { // Looking for prev and next Nodes of actual Node
                next = XOR(prev, curr->npx);
                prev = curr;
                curr = next;
            }

            actual = XOR(prev, curr->npx); // actual = prev ^ prev ^ next == next
        }

        std::cout << actual->data << " ";
    }

    // Setting actual to prev Node
    void PREV() {
        if (actual == nullptr) { // If actual is not set
            std::cout << "NULL ";
            return;
        }
        else if (actual == head) { // If actual is the first element of the list
            actual = last;
        }
        else { // If actual is inside the list
            Node* curr = head;
            Node* prev = nullptr;
            Node* next;

            while (curr != actual) { // Looking for prev and next Nodes of actual Node
                next = XOR(prev, curr->npx);
                prev = curr;
                curr = next;
            }

            actual = XOR(curr->npx, XOR(prev, curr->npx)); // actual = prev ^ next ^ prev ^ prev ^ next == prev
        }
        std::cout << actual->data << " ";
    }

    // Adding new Node to the beginning of the list
    void ADD_BEG(int data) {
        Node* newNode = new Node();
        newNode->data = data;
        newNode->npx = head;

        if (head != nullptr) { // If list is not empty
            head->npx = XOR(newNode, head->npx); // head->npx = newNode ^ NULL ^ next == newNode ^ next
        }
        else { // If is empty, set new Node to be actual and last Node too
            actual = newNode;
            last = newNode;
        }

        head = newNode; // Setting new Node to be the head of the list
    }

    // Adding new Node to the end of the list
    void ADD_END(int data) {
        if (head == nullptr) {
            ADD_BEG(data);
            return;
        }
        Node* newNode = new Node();
        newNode->data = data;
        newNode->npx = last;

        if (last != nullptr) { // If list is not empty
            last->npx = XOR(last->npx, newNode); // last->npx = prev ^ NULL ^ newNode == prev ^ newNode
        }

        last = newNode; // Setting new Node to be the last Node of the list
    }

    // Removing Node from the beginning
    void DEL_BEG() {
        if (head != nullptr) { // If list is not empty
            Node* toDel = head; // Pointer to removed element
            head = XOR(nullptr, toDel->npx); // head = NULL ^ NULL ^ next == next

            if (head != nullptr) { // If list is still not empty
                (head)->npx = XOR(nullptr, XOR(toDel, head->npx)); // head->npx = NULL ^ head ^ head ^ nextnext == NULL ^ nextnext
                if (toDel == actual) { // If actual is the old head, change actual to the new head
                    actual = head;
                }
            }
            else { // If list is empty, clear actual and last
                actual = nullptr;
                last = nullptr;
            }
        }

    }

    // Removing Node from the end
    void DEL_END() {
        if (head == last) { // If there is only one element in the list
            head = nullptr;
            last = nullptr;
            actual = nullptr;
            return;
        }
        if (head != nullptr) { // If list is not empty
            Node* curr = head;
            Node* prev = nullptr;
            Node* next;

            while (XOR(prev, curr->npx) != nullptr) { // While next is not null
                next = XOR(prev, curr->npx);
                prev = curr;
                curr = next;
            }

            if (prev != nullptr) { // If list doesn't have just one Node
                prev->npx = XOR(prev->npx, curr); // prev->npx = prevprev ^ curr ^ curr == prevprev
                last = prev;
            }
            else { // Clearing head, if list has only one element
                head = nullptr;
            }
        }
    }


    // Adding new Node as prev of actual Node
    void ADD_ACT(int data) {
        if (head != nullptr) { // If list is not empty
            Node* curr = head;
            Node* prev = nullptr;
            Node* next;

            while (curr != actual) {
                next = XOR(prev, curr->npx);
                prev = curr;
                curr = next;
            }
            next = XOR(prev, curr->npx);

            if (prev == nullptr) { // If actual Node is the first element, add Node at the beginning
                ADD_BEG(data);
            }
            else {
                Node* newNode = new Node();
                newNode->npx = XOR(prev, curr);
                newNode->data = data;

                prev->npx = XOR(XOR(curr, prev->npx), newNode); // prev->npx = curr ^ prevprev ^ curr ^ newNode = prevprev ^ newNode
                curr->npx = XOR(newNode, next); // curr->npx = newNode ^ next
            }
        }

    }

    // Removing actual Node and changing it to his prev Node
    void DEL_ACT() {
        if (actual != nullptr) {
            Node* curr = head;
            Node* prev = nullptr;
            Node* next;

            while (curr != actual) {
                next = XOR(prev, curr->npx);
                prev = curr;
                curr = next;
            }
            next = XOR(prev, curr->npx);


            if (next != nullptr) { // If actual is not the last Node of the list
                next->npx = XOR(prev, XOR(curr, next->npx)); // next->npx = prev ^ curr ^ curr ^ nextnext == prev ^ nextnext
            }
            else {
                last = prev;
                actual = prev;
            }

            if (prev != nullptr) { // If actual is not the first Node of the list
                prev->npx = XOR(XOR(prev->npx, curr), next); // prev->npx = prevprev ^ curr ^ curr ^ next = prevprev ^ next
                actual = prev; // actual becomes its prev Node
            }
            else { // Changing actual Node which is first Node to last
                head = next;
                actual = last;
            }

            if (prev == nullptr && next == nullptr) { // If list has only one Node, remove actual
                actual = nullptr;
            }
        }
    }

    // Overloaded version of DEL_ACT with parameter for removing other elements
    void DEL_ACT(Node** pos) {
        if ((*pos) != nullptr) {
            Node* curr = head;
            Node* prev = nullptr;
            Node* next;

            while (curr != (*pos)) {
                next = XOR(prev, curr->npx);
                prev = curr;
                curr = next;
            }
            next = XOR(prev, curr->npx);


            if (next != nullptr) { // If Node is not the last Node of the list
                next->npx = XOR(prev, XOR(curr, next->npx)); // next->npx = prev ^ curr ^ curr ^ nextnext == prev ^ nextnext
            }
            else {
                last = prev;
                (*pos) = prev;
            }

            if (prev != nullptr) { // If Node is not the first Node of the list
                prev->npx = XOR(XOR(prev->npx, curr), next); // prev->npx = prevprev ^ curr ^ curr ^ next = prevprev ^ next
                (*pos) = prev; // Node becomes its prev Node
            }
            else { // Changing Node which is first Node to last
                head = next;
                (*pos) = last;
            }

            if (prev == nullptr && next == nullptr) { // If list has only one Node, remove Node
                (*pos) = nullptr;
            }
        }
    }

    // Removing all Nodes with given value
    void DEL_VAL(int data) {
        if (head != nullptr) {
            Node* curr = head;
            Node* prev = nullptr;
            Node* next;

            while (curr != nullptr) { // Going through the whole list
                next = XOR(prev, curr->npx);
                if (curr->data == data) { // If curr Node has the same value as given
                    if (curr == actual) { // If curr Node is same as actual
                        DEL_ACT(&actual); // Do DEL_ACT with last parameter
                    }
                    else {
                        DEL_ACT(&curr); // Do DEL_ACT with curr parameter
                    }
                    if (next == nullptr) { // If curr element is the last one, end the loop
                        break;
                    }
                    // Reset values of prev, curr and next
                    curr = head;
                    prev = nullptr;
                    next = XOR(prev, curr->npx);
                }
                prev = curr;
                curr = next;
            }
        }

    }

    // Print list from the beginning
    void PRINT_FORWARD() const {
        if (head == nullptr) {
            std::cout << "NULL";
            return;
        }
        Node* curr = head;
        Node* prev = nullptr;
        Node* next;

        while (curr != nullptr) {
            std::cout << curr->data << " ";
            next = XOR(prev, curr->npx); // next = prev ^ prev ^ next == next
            prev = curr;
            curr = next;
        }
    }

    // Print list from the end
    void PRINT_BACKWARD() const {
        if (head == nullptr) {
            std::cout << "NULL";
            return;
        }
        Node* curr = last;
        Node* next = nullptr;
        Node* prev;

        while (curr != nullptr) {
            std::cout << curr->data << " ";
            prev = XOR(curr->npx, next); // prev = prev ^ next ^ next == prev
            next = curr;
            curr = prev;
        }
    }
};


#endif //XOR_LINKED_LIST_LINKEDLIST_H
