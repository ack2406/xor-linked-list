#include <stdio.h>
#include <stdlib.h>
#include "xor_linked_list.h"


static const char* err_str(xorl_err e) {
    switch (e) {
        case XORL_OK:         return "XORL_OK";
        case XORL_ERR_BADARG: return "XORL_ERR_BADARG";
        case XORL_ERR_OOM:    return "XORL_ERR_OOM";
        case XORL_ERR_EMPTY:  return "XORL_ERR_EMPTY";
        default:              return "XORL_ERR_???";
    }
}

static void print_item(int v, void* user) {
    FILE* f = (FILE*)user;
    fprintf(f, "%d ", v);
}

static void dump(const xorl_list* l, const char* label) {
    printf("%s: size=%zu empty=%d | fwd=[ ", label, xorl_size(l), xorl_list_empty(l));
    xorl_err e1 = xorl_foreach_forward(l, print_item, stdout);
    if (e1 != XORL_OK) printf("(foreach err=%s) ", err_str(e1));
    printf("] bwd=[ ");
    xorl_err e2 = xorl_foreach_backward(l, print_item, stdout);
    if (e2 != XORL_OK) printf("(foreach err=%s) ", err_str(e2));
    printf("]\n");
}

static void show_cursor(const xorl_list* l, const char* label) {
    int v;
    xorl_err e = xorl_cursor_get(l, &v);
    if (e == XORL_OK) {
        printf("%s cursor=%d\n", label, v);
    } else {
        printf("%s cursor=(none) err=%s\n", label, err_str(e));
    }
}

static void must_ok(xorl_err e, const char* what) {
    if (e != XORL_OK) {
        fprintf(stderr, "FAIL: %s -> %s (%d)\n", what, err_str(e), (int)e);
        exit(1);
    }
}


typedef struct {
    long sum;
} sum_ctx;

static void sum_cb(int v, void* user) {
    sum_ctx* s = (sum_ctx*)user;
    s->sum += v;
}

typedef struct {
    int target;
    int found;
} find_ctx;

static void find_cb(int v, void* user) {
    find_ctx* f = (find_ctx*)user;
    if (v == f->target) f->found = 1;
}


int main(void) {
    printf("=== XOR linked list demo ===\n");

    xorl_list* l = xorl_create();
    if (!l) {
        fprintf(stderr, "xorl_create failed (OOM)\n");
        return 1;
    }

    // 1) Empty-list behavior
    printf("\n-- empty list cases --\n");
    dump(l, "init");
    int v = -1;
    printf("pop_front(empty) -> %s\n", err_str(xorl_list_pop_front(l, &v)));
    printf("pop_back(empty)  -> %s\n", err_str(xorl_list_pop_back(l, &v)));
    printf("cursor_get(empty)   -> %s\n", err_str(xorl_cursor_get(l, &v)));
    printf("cursor_next(empty)  -> %s\n", err_str(xorl_cursor_next(l)));
    printf("cursor_prev(empty)  -> %s\n", err_str(xorl_cursor_prev(l)));

    // 2) Basic pushes
    printf("\n-- push_front / push_back --\n");
    must_ok(xorl_list_push_back(l, 1),  "push_back(1)");
    must_ok(xorl_list_push_back(l, 2),  "push_back(2)");
    must_ok(xorl_list_push_front(l, 0), "push_front(0)");
    must_ok(xorl_list_push_back(l, 3),  "push_back(3)");
    dump(l, "after pushes");

    // 3) Cursor navigation
    printf("\n-- cursor: to_head/to_tail, next/prev, get --\n");
    xorl_cursor_to_head(l);
    show_cursor(l, "to_head:");
    must_ok(xorl_cursor_next(l), "cursor_next");
    show_cursor(l, "after next:");
    must_ok(xorl_cursor_next(l), "cursor_next");
    show_cursor(l, "after next:");
    must_ok(xorl_cursor_prev(l), "cursor_prev");
    show_cursor(l, "after prev:");

    xorl_cursor_to_tail(l);
    show_cursor(l, "to_tail:");
    must_ok(xorl_cursor_prev(l), "cursor_prev");
    show_cursor(l, "after prev from tail:");

    // 4) Insert around cursor
    printf("\n-- cursor inserts: insert_after / insert_before --\n");
    xorl_cursor_to_head(l);
    must_ok(xorl_cursor_next(l), "cursor_next (move to 1)");
    show_cursor(l, "cursor at:");
    must_ok(xorl_cursor_insert_after(l, 99), "cursor_insert_after(99)");
    show_cursor(l, "after insert_after (cursor should be 99):");
    dump(l, "after insert_after");

    must_ok(xorl_cursor_insert_before(l, 77), "cursor_insert_before(77)");
    show_cursor(l, "after insert_before (cursor should be 77):");
    dump(l, "after insert_before");

    // 5) Erase current
    printf("\n-- cursor erase: cursor_erase --\n");
    int erased = -1;
    must_ok(xorl_cursor_erase(l, &erased), "cursor_erase");
    printf("erased=%d\n", erased);
    show_cursor(l, "cursor after erase:");
    dump(l, "after erase");

    // 6) Pop operations affecting cursor safety
    printf("\n-- pop_front / pop_back (cursor safety) --\n");
    xorl_cursor_to_head(l);
    show_cursor(l, "cursor to_head:");
    must_ok(xorl_list_pop_front(l, &v), "pop_front");
    printf("pop_front -> %d\n", v);
    show_cursor(l, "cursor after pop_front:");
    dump(l, "state");

    xorl_cursor_to_tail(l);
    show_cursor(l, "cursor to_tail:");
    must_ok(xorl_list_pop_back(l, &v), "pop_back");
    printf("pop_back -> %d\n", v);
    show_cursor(l, "cursor after pop_back:");
    dump(l, "state");

    // 7) foreach callbacks: sum + find
    printf("\n-- foreach callbacks (sum, find) --\n");
    sum_ctx s = { .sum = 0 };
    must_ok(xorl_foreach_forward(l, sum_cb, &s), "foreach_forward(sum)");
    printf("sum(forward)=%ld\n", s.sum);

    find_ctx f = { .target = 99, .found = 0 };
    must_ok(xorl_foreach_forward(l, find_cb, &f), "foreach_forward(find 99)");
    printf("contains 99? %s\n", f.found ? "yes" : "no");

    f.target = 123;
    f.found = 0;
    must_ok(xorl_foreach_backward(l, find_cb, &f), "foreach_backward(find 123)");
    printf("contains 123? %s\n", f.found ? "yes" : "no");

    // 8) Drain list
    printf("\n-- drain list --\n");
    while (xorl_list_pop_front(l, &v) == XORL_OK) {
        printf("drain pop_front -> %d\n", v);
    }
    dump(l, "after drain");
    printf("pop_back(empty) -> %s\n", err_str(xorl_list_pop_back(l, &v)));

    xorl_destroy(l);
    printf("\n=== done ===\n");
    return 0;
}

