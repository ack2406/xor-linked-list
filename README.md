# XOR Linked List

An implementation of an XOR linked list in C.
It supports basic deque operations (push/pop front/back), a movable cursor (`cursor_*`) for local edits, and callback-based iteration (`foreach`) in both directions.

## Build & Run

```bash
gcc -std=c11 -Wall -Wextra -O2 demo.c xor_linked_list.c -o demo && ./demo
```

