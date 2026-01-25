#include "xor_linked_list.h"

#include <stdint.h>
#include <stdlib.h>

struct xorl_node {
    int val;
    struct xorl_node* npx;
};

struct xorl_list {
    struct xorl_node* head;
    struct xorl_node* tail;

    size_t size;

    struct xorl_node* act;
    struct xorl_node* act_prev;
};

static inline struct xorl_node* xorl_ptr_xor(struct xorl_node* a, struct xorl_node* b) {
    return (struct xorl_node*)((uintptr_t)a ^ (uintptr_t)b);
}

static struct xorl_node* xorl_node_new(int v) {
    struct xorl_node* n = malloc(sizeof(*n));
    if (!n) return NULL;
    
    n->val = v;
    n->npx = NULL;
    
    return n;
}

xorl_list* xorl_create() {
    xorl_list* l = malloc(sizeof(*l));
    if (!l) return NULL;
    
    l->head = NULL;
    l->tail = NULL; 
    l->size = 0;
    l->act = NULL;
    l->act_prev = NULL;
    
    return l;
}

void xorl_destroy(xorl_list* l) {
    if (!l) return;

    uintptr_t prev_addr = 0;
    struct xorl_node* cur = l->head;

    while (cur) {
        struct xorl_node* next = (struct xorl_node*)(prev_addr ^ (uintptr_t)cur->npx);
        prev_addr = (uintptr_t)cur;
        free(cur);
        cur = next;
    }
    
    free(l);
}

size_t xorl_size(const xorl_list* l) {
    return l ? l->size : 0;
}

int xorl_list_empty(const xorl_list* l) {
    return (!l || l->size == 0) ? 1 : 0;
}

xorl_err xorl_list_push_front(xorl_list* l, int v) {
    if (!l) return XORL_ERR_BADARG;

    struct xorl_node* n = xorl_node_new(v);
    if (!n) return XORL_ERR_OOM;
    
    n->npx = xorl_ptr_xor(NULL, l->head);
    
    if (!l->head) {
        l->head = l->tail = n;
    } else {
        struct xorl_node* old = l->head;
        old->npx = xorl_ptr_xor(n, old->npx);
        l->head = n;
    }
    
    l->size++;

    if (!l->act) {
        l->act = l->head;
        l->act_prev = NULL;
    }

    return XORL_OK;
}

xorl_err xorl_list_push_back(xorl_list* l, int v) {
    if (!l) return XORL_ERR_BADARG;

    struct xorl_node* n = xorl_node_new(v);
    if (!n) return XORL_ERR_OOM;

    n->npx = xorl_ptr_xor(l->tail, NULL);

    if (!l->tail) {
        l->head = l->tail = n;
    } else {
        struct xorl_node* old = l->tail;
        old->npx = xorl_ptr_xor(old->npx, n);
        l->tail = n;
    }

    l->size++;

    if (!l->act) {
        l->act = l->tail;
        l->act_prev = xorl_ptr_xor(l->tail->npx, NULL);
    }

    return XORL_OK;
}

xorl_err xorl_list_pop_front(xorl_list* l, int* out_v) {
    if (!l || !out_v) return XORL_ERR_BADARG;
    if (!l->head) return XORL_ERR_EMPTY;

    struct xorl_node* old = l->head;
    *out_v = old->val;

    struct xorl_node* next = xorl_ptr_xor(NULL, old->npx);

    if (!next) {
        l->head = l->tail = NULL;
    } else {
        struct xorl_node* next_next = xorl_ptr_xor(old, next->npx);
        next->npx = xorl_ptr_xor(NULL, next_next);
        l->head = next;
    }

    if (l->act == old) {
        l->act = next;
        l->act_prev = NULL;
    } else if (l->act_prev == old) {
        l->act = l->head;
        l->act_prev = NULL;
    }

    free(old);
    l->size--;

    if (l->size == 0) {
        l->act = NULL;
        l->act_prev = NULL;
    }

    return XORL_OK;
}

xorl_err xorl_list_pop_back(xorl_list* l, int* out_v) {
    if (!l || !out_v) return XORL_ERR_BADARG;
    if (!l->tail) return XORL_ERR_EMPTY;

    struct xorl_node* old = l->tail;
    *out_v = old->val;

    struct xorl_node* prev = xorl_ptr_xor(old->npx, NULL);

    if (!prev) {
        l->head = l->tail = NULL;
    } else {
        struct xorl_node* prev_prev = xorl_ptr_xor(prev->npx, old);
        prev->npx = xorl_ptr_xor(prev_prev, NULL);
        l->tail = prev;
    }

    if (l->act == old) {
        l->act = prev;

        if (l->act) {
            l->act_prev = xorl_ptr_xor(l->act->npx, NULL);
        } else {
            l->act_prev = NULL;
        }
    } else if (l->act_prev == old) {
        l->act = l->tail;
        l->act_prev = l->tail ? xorl_ptr_xor(l->tail->npx, NULL) : NULL;
    }

    free(old);
    l->size--;

    if (l->size == 0) {
        l->act = NULL;
        l->act_prev = NULL;
    }

    return XORL_OK;
}


void xorl_act_to_head(xorl_list* l) {
    if (!l) return;
    l->act = l->head;
    l->act_prev = NULL;
}

void xorl_act_to_tail(xorl_list* l) {
    if (!l) return;
    l->act = l->tail;
    l->act_prev = l->tail ? xorl_ptr_xor(l->tail->npx, NULL) : NULL;
}

xorl_err xorl_act_next(xorl_list* l) {
    if (!l) return XORL_ERR_BADARG;
    if (!l->act) return XORL_ERR_EMPTY;

    struct xorl_node* next = xorl_ptr_xor(l->act_prev, l->act->npx);
    if (!next) return XORL_ERR_EMPTY; // act is on tail

    l->act_prev = l->act;
    l->act = next;

    return XORL_OK;
}

xorl_err xorl_act_prev(xorl_list* l) {
    if (!l) return XORL_ERR_BADARG;
    if (!l->act) return XORL_ERR_EMPTY;
    if (!l->act_prev) return XORL_ERR_EMPTY; // act is on head 
    
    struct xorl_node* prev_prev = xorl_ptr_xor(l->act_prev->npx, l->act);

    l->act = l->act_prev;
    l->act_prev = prev_prev;

    return XORL_OK;
}

xorl_err xorl_act_insert_after(xorl_list* l, int v) {
    if (!l) return XORL_ERR_BADARG;
   
    if (!l->head || !l->act) {
        xorl_err e = xorl_list_push_back(l, v);
        if (e == XORL_OK) {
            l->act = l->tail;
            l->act_prev = xorl_ptr_xor(l->tail->npx, NULL);
        }
        return e;
    }

    struct xorl_node* prev = l->act_prev;
    struct xorl_node* cur  = l->act;
    struct xorl_node* next = xorl_ptr_xor(prev, cur->npx);

    struct xorl_node* n = xorl_node_new(v);
    if (!n) return XORL_ERR_OOM;

    n->npx   = xorl_ptr_xor(cur, next);
    cur->npx = xorl_ptr_xor(prev, n);

    if (next) {
        struct xorl_node* next_next = xorl_ptr_xor(cur, next->npx);
        next->npx = xorl_ptr_xor(n, next_next);
    } else {
        l->tail = n;
    }

    l->act = n;
    l->act_prev = cur;

    l->size++;
    return XORL_OK;
}
   
xorl_err xorl_act_insert_before(xorl_list* l, int v) {
    if (!l) return XORL_ERR_BADARG;
   
    if (!l->head || !l->act) {
        xorl_err e = xorl_list_push_back(l, v);
        if (e == XORL_OK) {
            l->act = l->tail;
            l->act_prev = xorl_ptr_xor(l->tail->npx, NULL);
        }
        return e;
    }

    if (!l->act_prev) {
        xorl_err e = xorl_list_push_front(l, v);
        if (e == XORL_OK) {
            l->act = l->head;
            l->act_prev = NULL;
        }
        return e;
    }

    struct xorl_node* prev = l->act_prev;
    struct xorl_node* cur  = l->act;
    struct xorl_node* next = xorl_ptr_xor(prev, cur->npx);
    struct xorl_node* prev_prev = xorl_ptr_xor(prev->npx, cur);


    struct xorl_node* n = xorl_node_new(v);
    if (!n) return XORL_ERR_OOM;

    n->npx   = xorl_ptr_xor(prev, cur);
    cur->npx = xorl_ptr_xor(n, next);
    prev->npx = xorl_ptr_xor(prev_prev, n);

    l->act = n;
    l->act_prev = prev;

    l->size++;
    return XORL_OK;
}

xorl_err xorl_act_get(const xorl_list* l, int* out_v) {
    if (!l || !out_v) return XORL_ERR_BADARG;
    if (!l->act) return XORL_ERR_EMPTY;
    *out_v = l->act->val;
    return XORL_OK;
}

xorl_err xorl_act_erase(xorl_list* l, int* out_v) {
    if (!l) return XORL_ERR_BADARG;
    if (!l->act) return XORL_ERR_EMPTY;
    
    struct xorl_node* prev = l->act_prev;
    struct xorl_node* cur = l->act;
    struct xorl_node* next = xorl_ptr_xor(prev, cur->npx);

    if (out_v) *out_v = cur->val;

    struct xorl_node* prev_prev = NULL;
    struct xorl_node* next_next = NULL;

    if (prev) prev_prev = xorl_ptr_xor(prev->npx, cur);
    if (next) next_next = xorl_ptr_xor(cur, next->npx);

    if (prev) {
        prev->npx = xorl_ptr_xor(prev_prev, next);
    } else {
        l->head = next;
    }

    if (next) {
        next->npx = xorl_ptr_xor(prev, next_next);
    } else {
        l->tail = prev;
    }

    free(cur);
    l->size--;

    if (l->size == 0) {
        l->act = NULL;
        l->act_prev = NULL;
        return XORL_OK;
    }

    if (next) {
        l->act = next;
        l->act_prev = prev;
    } else {
        l->act = prev;
        l->act_prev = prev_prev;
    }

    return XORL_OK;
}

xorl_err xorl_foreach_forward(const xorl_list* l, xorl_visit_fn fn, void* user) {
   if (!l || !fn) return XORL_ERR_BADARG;

   struct xorl_node* prev = NULL;
   struct xorl_node* cur = l->head;

   while (cur) {
       fn(cur->val, user);
       struct xorl_node* next = xorl_ptr_xor(prev, cur->npx);
       prev = cur;
       cur  = next;
   }

   return XORL_OK;
}

xorl_err xorl_foreach_backward(const xorl_list* l, xorl_visit_fn fn, void* user) {
   if (!l || !fn) return XORL_ERR_BADARG;

   struct xorl_node* next = NULL;
   struct xorl_node* cur = l->tail;

   while (cur) {
       fn(cur->val, user);
       struct xorl_node* prev = xorl_ptr_xor(cur->npx, next);
       next = cur;
       cur  = prev;
   }

   return XORL_OK;
}
