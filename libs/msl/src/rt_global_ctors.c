// Start-up constructor walk and the global-destructor registration list (0x020202bc-0x0202030c).

typedef struct DtorNode {
    struct DtorNode *next;
    void (*func)(void *);
    void *arg;
} DtorNode;

// Head of the list of registered global-object destructors (bss).
extern DtorNode *data_020c17a4;
// Start of the static-constructor pointer table (link-time symbol).
extern void (*ARM9_CTOR_START)(void);

// Empty routine: returns immediately.
void func_020202bc(void) {
}

// Walks the table of constructor pointers starting at ARM9_CTOR_START and calls each
// entry in turn, stopping at the first null entry.
void func_020202c0(void) {
    void (**p)(void) = &ARM9_CTOR_START;
    while (p != 0 && *p != 0) {
        (*p)();
        p++;
    }
}

// Pushes a destructor record onto the front of the global list: node->next takes the old
// head, node->func = func, node->arg = obj, then the head is set to node.
void __register_global_object(void *obj, void (*func)(void *), DtorNode *node) {
    node->next = data_020c17a4;
    node->func = func;
    node->arg = obj;
    data_020c17a4 = node;
}
