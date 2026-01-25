#include <stddef.h>

typedef enum xorl_err {
    XORL_OK         =  0,
    XORL_ERR_BADARG = -1,
    XORL_ERR_OOM    = -2,
    XORL_ERR_EMPTY  = -3
} xorl_err;


typedef struct xorl_list xorl_list;


xorl_list* xorl_create(void);
void       xorl_destroy(xorl_list* l);

size_t     xorl_size(const xorl_list* l);
int        xorl_list_empty(const xorl_list* l);

xorl_err   xorl_list_push_front(xorl_list* l, int v);
xorl_err   xorl_list_push_back(xorl_list* l, int v);

xorl_err   xorl_list_pop_front(xorl_list* l, int* out_v);
xorl_err   xorl_list_pop_back(xorl_list* l, int* out_v);


void       xorl_act_to_head(xorl_list* l);
void       xorl_act_to_tail(xorl_list* l);

xorl_err   xorl_act_next(xorl_list* l);
xorl_err   xorl_act_prev(xorl_list* l);

xorl_err   xorl_act_insert_after(xorl_list* l, int v);
xorl_err   xorl_act_insert_before(xorl_list* l, int v);

xorl_err   xorl_act_get(const xorl_list* l, int* out_v);
xorl_err   xorl_act_erase(xorl_list* l, int* out_v);


typedef void (*xorl_visit_fn)(int value, void* user);

xorl_err   xorl_foreach_forward(const xorl_list* l, xorl_visit_fn fn, void* user);
xorl_err   xorl_foreach_backward(const xorl_list* l, xorl_visit_fn fn, void* user);
