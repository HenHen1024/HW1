#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ...existing code...
typedef struct Node {
    char *value;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
} List;

// Create a heap-allocated copy of a C string.
static char *dup_string(const char *s) {
    size_t len = strlen(s) + 1;
    char *copy = malloc(len);
    if (copy == NULL) {
        perror("malloc");
        exit(1);
    }
    memcpy(copy, s, len);
    return copy;
}

void list_init(List *list) {
    list->head = NULL;
    list->tail = NULL;
}

// Insert in sorted order (alphabetical)
void list_insert(List *list, const char *value) {
    Node *new_node = malloc(sizeof(*new_node));
    if (new_node == NULL) {
        perror("malloc");
        exit(1);
    }

    new_node->value = dup_string(value);
    new_node->prev = NULL;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
        list->tail = new_node;
        return;
    }

    Node *cur = list->head;
    while (cur != NULL && strcmp(cur->value, value) < 0) {
        cur = cur->next;
    }

    if (cur == list->head) {
        // Insert before head
        new_node->next = list->head;
        list->head->prev = new_node;
        list->head = new_node;
    } else if (cur == NULL) {
        // Insert at tail
        new_node->prev = list->tail;
        list->tail->next = new_node;
        list->tail = new_node;
    } else {
        // Insert in middle
        Node *prev_node = cur->prev;
        new_node->prev = prev_node;
        new_node->next = cur;
        prev_node->next = new_node;
        cur->prev = new_node;
    }
}

Node *list_find(List *list, const char *target) {
    for (Node *cur = list->head; cur != NULL; cur = cur->next) {
        if (strcmp(cur->value, target) == 0) {
            return cur;
        }
    }
    return NULL;
}

bool list_delete(List *list, const char *target) {
    Node *node = list_find(list, target);
    if (node == NULL) {
        return false;
    }

    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }

    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }

    free(node->value);
    free(node);
    return true;
}

void list_free(List *list) {
    Node *cur = list->head;
    while (cur != NULL) {
        Node *next = cur->next;
        free(cur->value);
        free(cur);
        cur = next;
    }
    list->head = NULL;
    list->tail = NULL;
}

int main(void) {
    List list;
    list_init(&list);

    const char *items[] = {"pear", "apple", "banana", "orange"};
    size_t count = sizeof(items) / sizeof(items[0]);

    for (size_t i = 0; i < count; i++) {
        list_insert(&list, items[i]);
    }

    printf("List contents:\n");
    for (Node *cur = list.head; cur != NULL; cur = cur->next) {
        printf("  %s\n", cur->value);
    }

    printf("\nFind 'banana': ");
    if (list_find(&list, "banana") != NULL) {
        printf("found\n");
    } else {
        printf("not found\n");
    }

    printf("\nDelete 'apple'\n");
    if (list_delete(&list, "apple")) {
        printf("Deleted successfully.\n");
    } else {
        printf("Delete failed.\n");
    }

    printf("\nList after deletion:\n");
    for (Node *cur = list.head; cur != NULL; cur = cur->next) {
        printf("  %s\n", cur->value);
    }

    list_free(&list);
    return 0;
}