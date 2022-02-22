# XOR linked list (AiSD 2021)
The task is based on implement a bidirectional list. A classic bidirectional list stores successor and predecessor addresses in nodes, in our task the list stores only xor of these addresses in nodes. This saves approximately 50% of memory used by the list structure.

The terms list node and list element are used interchangeably and means the same.

During adding a new node to the list, we write the xor value of the predecessor and the successor to this node. If it is the first or the last node, as the xor of addresses, we write the address of the successor or the predecessor, respectively, because the address xor with 0 (we take zero as NULL) does not change the address value. From this it follows that we need to store a pointer to the first and last element of the list, which will allow us to browse the list in both directions. If we want to go to the next node in the assumed direction, we read its address as xor values stored in the current and previously visited node. As described earlier, at the boundary node this will be the direct address (since the value of a hypothetical nonexistent earlier node is 0).

In the case of deleting a node, updating the xor values of the deleted node's predecessor and successor is required. When deleting an boundary node, there is only one neighbor that needs to be updated.

The task requires that we have an additional pointer showing the current value used by the ACTUAL command described later. In this case, we also need to keep one of the neighbors (successor or predecessor, preferably both) of pointer showing the current value. Otherwise, we will not be able to restore their addresses (successor or predecessor of the current element). In this case, we must additionally store one of the neighbors (successor or predecessor, preferably both), otherwise we will not be able to restore their addresses. It would seem that this does not apply to the first and last node, but we are not able to conclude (without additional information) that the index to the current element just points to one of the boundary elements.

#### The list provides the following functionalities related to the following commands:

* `ACTUAL` - value of the currently pointed element. For an empty list it is NULL and after adding the first element to the list ACTUAL returns the value of that element as long as it is not moved by other operations.
* `NEXT` - prints the value of the ACTUAL successor setting it ACTUAL at the same time. If ACTUAL points to the last element of the queue, its successor will be the first element of the queue.
* `PREV` - prints the value of the predecessor ACTUAL setting it ACTUAL at the same time. If ACTUAL points to the first element of a queue, its predecessor will be the last element of the queue.
* `ADD_BEG` N - adding an element with value N to the beginning of the list.
* `ADD_END` N - adding an element with value N to the end of the list.
* `ADD_ACT` N - adding an element with the value of N as a predecessor of the currently selected element (ACTUAL).
* `DEL_BEG` - removes the first node from the list.
* `DEL_END` - removes the last node from the list.
* `DEL_VAL` N - removes from the list all nodes whose value is equal to N.
* `DEL_ACT` - removes from the list the node that ACTUAL points to, simultaneously setting ACTUAL to PREV. In the case of the PREV does not exist (ACTUAL was the first element of the list) ACTUAL shows the last element of the list.
* `PRINT_FORWARD` - prints the contents of the list from the first to the last element.
* `PRINT_BACKWARD` - prints the contents of the list from the last to the first element.
The behavior of the NEXT, PREV and DEL_ACT commands suggests action on a circular queue in which (except for an empty queue) there is always a successor and a predecessor for each node. When such a queue has only one element, the xor value of this node is 0. However, the implementation of the circular queue is not required and is not forbidden. Instead, it is possible to implement additional functionality of the NEXT and PREV commands. It will be run when an exceptional situation occurs, i.e. NEXT call when ACTUAL points to the last element of the queue, or PREV or DEL_ACT when ACTUAL points to the first element of a non-cyclic queue.

DEL_BEG, DEL_END, DEL_VAL, and DEL_ACT commands for an empty list do not remove anything. In each of these cases, removing the currently pointed element (ACTUAL command) should result in moving the currently pointed element pointer to the preceding element, and if it does not exist, to the last element of the list.

Standard C ++ Template Library (STL) cannot be used. Please remember to free memory when deleting list elements. Please also delete the entire list before exiting the program, freeing up all memory occupied by all its elements.

### Input
A number of commands that run specific functionalities in the list.
### Output
The results of the operation of the appropriate commands on the list with a certain state. Initially, the list is empty and its later status depends on the commands that are previously invoked. Some commands do not generate any output, e.g. (ADD_BEG, DEL_ACT) but affect the state of the list and other commands that display certain information, e.g. (ACTUAL, PRINT_FORWARD).

### Example
##### Input1

ADD_BEG 1<br>
ADD_BEG 2<br>
ADD_BEG 3<br>
ADD_END 4<br>
PRINT_FORWARD<br>
PRINT_BACKWARD

##### Output1

3 2 1 4<br>
4 1 2 3

##### Input2

ACTUAL<br>
ADD_BEG 1<br>
ACTUAL<br>
ADD_BEG 2<br>
ADD_BEG 3<br>
ADD_END 4<br>
PRINT_FORWARD<br>
PRINT_BACKWARD<br>
ACTUAL<br>
NEXT<br>
ACTUAL<br>
NEXT<br>
NEXT<br>
PREV<br>
ACTUAL<br>
PREV<br>
PREV

##### Output2

NULL<br>
1<br>
3 2 1 4<br>
4 1 2 3<br>
1<br>
4<br>
4<br>
3<br>
2<br>
3<br>
3<br>
4<br>
1

##### Input3

ACTUAL<br>
NEXT<br>
PREV<br>
ADD_BEG 1<br>
ACTUAL<br>
NEXT<br>
PREV<br>
DEL_BEG<br>
ACTUAL<br>
NEXT<br>
PREV<br>
ADD_BEG 2<br>
ACTUAL<br>
NEXT<br>
PREV<br>
DEL_END<br>
ACTUAL<br>
NEXT<br>
PREV<br>
ADD_END 3<br>
ACTUAL<br>
NEXT<br>
PREV<br>
DEL_BEG<br>
ACTUAL<br>
NEXT<br>
PREV

##### Output3

NULL<br>
NULL<br>
NULL<br>
1<br>
1<br>
1<br>
NULL<br>
NULL<br>
NULL<br>
2<br>
2<br>
2<br>
NULL<br>
NULL<br>
NULL<br>
3<br>
3<br>
3<br>
NULL<br>
NULL<br>
NULL

##### Input4

ADD_END 1<br>
ADD_BEG 2<br>
ADD_END 3<br>
ADD_BEG 4<br>
PRINT_FORWARD<br>
DEL_BEG<br>
PRINT_FORWARD<br>
DEL_END<br>
PRINT_FORWARD<br>
ACTUAL<br>
NEXT<br>
ADD_ACT 5<br>
PRINT_FORWARD<br>
ACTUAL<br>
NEXT<br>
ADD_ACT  6<br>
PRINT_FORWARD<br>
ACTUAL<br>
NEXT<br>
DEL_ACT<br>
PRINT_FORWARD

##### Output4

4 2 1 3<br>
2 1 3<br>
2 1<br>
1<br>
2<br>
5 2 1<br>
2<br>
1<br>
5 2 6 1<br>
1<br>
5<br>
2 6 1

##### Input5

ADD_END 1<br>
ADD_BEG 2<br>
ADD_END 3<br>
ADD_BEG 4<br>
PRINT_FORWARD<br>
DEL_VAL 3<br>
PRINT_FORWARD<br>
DEL_VAL 2<br>
PRINT_FORWARD<br>
ADD_END 1<br>
ADD_BEG 2<br>
ADD_END 3<br>
ADD_BEG 4<br>
PRINT_FORWARD<br>
DEL_VAL 4<br>
PRINT_FORWARD

##### Output5

4 2 1 3<br>
4 2 1<br>
4 1<br>
4 2 4 1 1 3<br>
2 1 1 3

##### Input6

DEL_BEG<br>
DEL_END<br>
DEL_ACT<br>
ACTUAL<br>
NEXT<br>
PREV<br>
PRINT_FORWARD<br>
ADD_BEG 42<br>
ACTUAL<br>
NEXT<br>
PREV<br>
PRINT_FORWARD<br>
DEL_ACT<br>
ACTUAL<br>
NEXT<br>
PREV<br>
PRINT_FORWARD<br>
ADD_BEG 4<br>
ADD_END 3<br>
ADD_END 5<br>
ADD_BEG 2<br>
PRINT_FORWARD<br>
ACTUAL<br>
DEL_ACT<br>
ACTUAL<br>
DEL_ACT<br>
ACTUAL<br>
DEL_ACT<br>
ACTUAL<br>
DEL_ACT<br>
ACTUAL

##### Output6

NULL<br>
NULL<br>
NULL<br>
NULL<br>
42<br>
42<br>
42<br>
42<br>
NULL<br>
NULL<br>
NULL<br>
NULL<br>
2 4 3 5<br>
4<br>
2<br>
5<br>
3<br>
NULL

##### Input7

ADD_END 1<br>
ADD_END 2<br>
ADD_BEG 3<br>
ADD_ACT 4<br>
NEXT<br>
PREV<br>
ACTUAL<br>
PREV<br>
DEL_BEG<br>
DEL_BEG<br>
DEL_BEG<br>
DEL_BEG<br>
ADD_END 1<br>
ADD_END 2<br>
ADD_BEG 3<br>
ADD_ACT 4<br>
PREV<br>
ACTUAL<br>
NEXT<br>
NEXT<br>
DEL_END<br>
DEL_END<br>
DEL_END<br>
DEL_END<br>
ADD_END 1<br>
ADD_END 2<br>
ADD_BEG 3<br>
ADD_ACT 4<br>
ACTUAL<br>
PREV<br>
NEXT<br>
NEXT<br>
DEL_ACT<br>
DEL_ACT<br>
DEL_ACT<br>
DEL_ACT<br>
ADD_END 1<br>
ADD_END 2<br>
ADD_BEG 3<br>
ADD_ACT 4<br>
ACTUAL<br>
NEXT<br>
NEXT<br>
NEXT<br>
DEL_VAL 1<br>
DEL_VAL 3<br>
DEL_VAL 2<br>
DEL_VAL 4<br>
ADD_END 1<br>
ADD_END 2<br>
ADD_BEG 3<br>
ADD_ACT 4<br>
ACTUAL<br>
PREV<br>
PREV<br>
PREV

##### Output7

2<br>
1<br>
1<br>
4<br>
4<br>
4<br>
1<br>
2<br>
1<br>
4<br>
1<br>
2<br>
1<br>
2<br>
3<br>
4<br>
1<br>
4<br>
3<br>
2