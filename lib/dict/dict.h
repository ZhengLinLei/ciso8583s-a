#ifndef DICT_H
#define DICT_H

struct dict_t_struct {
    char *key;
    void *value;
    struct dict_t_struct *next;
};

typedef struct dict_t_struct dict_t;

dict_t **dictAlloc(void);
void dictDealloc(dict_t **dict);
void delItem(dict_t **dict, char *key);
void addItem(dict_t **dict, char *key, void *value);
void *getItem(dict_t *dict, char *key);

#endif