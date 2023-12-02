/* A key/value dict system in C */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"
#include "dict.h"

// #define TEST FALSE /* Comment this line out to compile without a main function (used when including into another application). */


dict_t **dictAlloc(void) {
    return malloc(sizeof(dict_t));
}

void dictDealloc(dict_t **dict) {
    free(dict);
}

void *getItem(dict_t *dict, char *key) {
    dict_t *ptr;
    for (ptr = dict; ptr != NULL; ptr = ptr->next) {
        if (strcmp(ptr->key, key) == 0) {
            return ptr->value;
        }
    }

    return NULL;
}

void delItem(dict_t **dict, char *key) {
    dict_t *ptr, *prev;
    for (ptr = *dict, prev = NULL; ptr != NULL; prev = ptr, ptr = ptr->next) {
        if (strcmp(ptr->key, key) == 0) {
            if (ptr->next != NULL) {
                if (prev == NULL) {
                    *dict = ptr->next;
                } else {
                    prev->next = ptr->next;
                }
            } else if (prev != NULL) {
                prev->next = NULL;
            } else {
                *dict = NULL;
            }
            
            free(ptr->key);
            free(ptr);
            
            return;
        }
    }
}

void addItem(dict_t **dict, char *key, void *value) {
    delItem(dict, key); /* If we already have a item with this key, delete it. */
    dict_t *d = malloc(sizeof(struct dict_t_struct));
    d->key = malloc(strlen(key)+1);
    strcpy(d->key, key);
    d->value = value;
    d->next = *dict;
    *dict = d;
}

// #ifdef TEST

//     int main(int argc, char **argv) {
//         /* Create a dict */
//         dict_t **dict = dictAlloc();
        
//         /* lets add foo, and bar to the dict */
//         addItem(dict, "foo", "bar");
//         addItem(dict, "bar", "foo");
        
//         /* and print their values */
//         printf("%s %s\n", (char *)getItem(*dict, "foo"), (char *)getItem(*dict, "bar"));
        
//         /* lets delete them */
//         delItem(dict, "foo");
//         delItem(dict, "bar");
        
//         /* see, their gone, there NULL */
//         printf("%s %s\n", (char *)getItem(*dict, "foo"), (char *)getItem(*dict, "bar"));
        
//         /* add them again to proof it works */
//         addItem(dict, "foo", "bar");
//         addItem(dict, "bar", "foo");
//         addItem(dict, "bar", "pan");
        
//         /* see, here */
//         printf("%s %s\n", (char *)getItem(*dict, "foo"), (char *)getItem(*dict, "bar"));
        
//         delItem(dict, "foo");
//         delItem(dict, "bar");
        
//         dictDealloc(dict);
        
//         return 0;
//     }

// #endif