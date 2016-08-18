#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct Node
{
  bool is_word;
  struct Node* children[27];
} Node;

void initialise_struct(Node*);
int load(char*, Node*);
int get_id(char);
bool check(char*, Node*);
int size();
void unload(Node*);
bool has_no_child(Node*);

int dictionary_size = 0;

int main(void)
{
  Node* root = malloc(sizeof(Node));
  if (root == NULL) {
    return -1;
  }

  initialise_struct(root);


  char word[80];
  FILE* fp = fopen("dictionaries/large", "r");
  if (fp == NULL) {
    printf("error when opening the file");
    return -1;
  }

  while (fscanf(fp, "%s", word) != EOF) {
    load(word, root);
    dictionary_size++;
  }

  if (check("zwitterionic", root)) {
    printf("it exist\n");
  } else {
    printf("it does not exist\n");
  }

  printf("size of the dictionary is: %d\n", dictionary_size);
  unload(root);
  printf("it's free");

  return 0;
}

void initialise_struct(Node* n)
{
  for (int i = 0; i < 27; i++) {
    n->is_word = false;
    n->children[i] = NULL;
  }
}

int load(char* word, Node* n)
{
  if (*word == '\0') {
    n->is_word = true;
    return 0;
  }

  int id = get_id(*word);

  if (n->children[id] == NULL) {
    Node* new_node = malloc(sizeof(Node));

    if (new_node == NULL) {
      printf("error malloc\n");
      return -1;
    }

    initialise_struct(new_node);
    n->children[id] = new_node;
  }

  word++;
  return load(word, n->children[id]);
}

int get_id(char c)
{
  if (c == '\'') {
    return 26;
  } else {
    return tolower((unsigned char)c) - 'a';
  }
}

bool check(char* c, Node* n)
{
  if (*c == '\0') {
    return n->is_word;
  }

  int id = get_id(*c);
  if (n->children[id] == NULL) {
    return false;
  } else {
    c++;
    return check(c, n->children[id]);
  }
}

int size() {
  return dictionary_size;
}

void unload(Node* n)
{
  if (has_no_child(n)) {
    free(n);
    return;
  } else {
    for (int i = 0; i < 27; i++) {
      if (n->children[i] != NULL) {
        unload(n->children[i]);
        n->children[i] = NULL;
      }
    }
    unload(n);
  }
}

bool has_no_child(Node* n)
{
  for (int i = 0; i < 27; i++) {
    if (n->children[i] != NULL) {
      return false;
    }
  }

  return true;
}
