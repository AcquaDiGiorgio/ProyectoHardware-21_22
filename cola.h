#ifndef COLA_H
#define COLA_H

#include <inttypes.h>

struct Node
{
    uint64_t data;
    struct Node *next;
};

struct Node *add(struct Node *last, uint64_t data);
uint64_t pop(struct Node *first);

#endif //COLA_H
