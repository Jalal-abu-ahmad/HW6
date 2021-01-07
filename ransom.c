#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum hash{size=300};

typedef struct singly_linked_list_node singly_linked_list_node;
typedef struct singly_linked_list singly_linked_list;

struct  singly_linked_list_node {
    char word[6];
    int word_recur;
    singly_linked_list_node *next;
}; 

struct  singly_linked_list{
    singly_linked_list_node *head;
    singly_linked_list_node *tail;
};

char* readline();
char** split_string(char*);
void insert_node_into_singly_linked_list(singly_linked_list *singly_linked_list,
                                                    char *word,  int word_recur);
void free_singly_linked_list(singly_linked_list_node *node);
int hash_function(char *str,int Mod);
singly_linked_list **make_hash_map(int hash_size, char **str , int str_count);

singly_linked_list_node *create_singly_linked_list_node(char *word, 
                                                                int word_recur);
int word_recurrence_in_hash_map(singly_linked_list **hash_map ,char *word, 
                                                                 int hash_size);
void free_map(singly_linked_list **hash_map, int hash_size);
void checkMagazine(int magazine_count, char **magazine, int note_count, char 
                                                                        **note);
/* creats a singly linked list node that contains a word and the number of times 
this word recurres */
singly_linked_list_node *create_singly_linked_list_node(char *word, 
                                                                int word_recur){
    singly_linked_list_node *node = (struct singly_linked_list_node*)malloc(
                                        sizeof(struct singly_linked_list_node));
    if(node==NULL){
        return NULL;
    }
    strcpy(node->word,word);
    node->word_recur = word_recur;
    node->next = NULL;

    return node;
}

/*inserting a new node into an existing linked list*/
void insert_node_into_singly_linked_list(singly_linked_list *singly_linked_list,
                                                   char *word,  int word_recur){
    singly_linked_list_node *node = create_singly_linked_list_node(word,
                                                                    word_recur);
    if(singly_linked_list->head==NULL){
        singly_linked_list-> head = node;
    } else{
        singly_linked_list-> tail-> next = node;
    }
    singly_linked_list-> tail = node;
}

void free_singly_linked_list(singly_linked_list_node *node){
    while(node){
        singly_linked_list_node *temp = node;
        node = node-> next;
 
        free(temp);
    }
}
/* calculates the hash value of a given word(string) */
int hash_function(char *str,int Mod){
    int sum=0,i=0;
    while(*(str+i) != '\0'){
        sum+=*(str+i);
        i++;
    }
    return sum%Mod;
}

/* building a hash map of a given array of strings
takes as input: the size of the hash table
                a pointer to the array of strings
                the number of strings in the array */
singly_linked_list **make_hash_map(int hash_size, char **str , int str_count){
    int i;
    /* dynamically allocating and initializing the hash map*/
    struct singly_linked_list **hash_map=(struct singly_linked_list**)malloc(
                                  sizeof(struct singly_linked_list*)*hash_size);
    for(i=0;i<hash_size;i++){
        hash_map[i]=(struct singly_linked_list*)malloc(sizeof(
                                                    struct singly_linked_list));
        hash_map[i]->head=NULL;
        hash_map[i]->tail=NULL;
    }
    /* going over all the strings in the array and adding them to the hash map 
    using chaining (linked lists) */
    for(i=0;i<str_count;i++){
       if(!hash_map[hash_function(*str , hash_size)]->head){ 
        /*if the hash slot is still empty*/
        singly_linked_list *current_list = (struct singly_linked_list*)malloc(
                                             sizeof(struct singly_linked_list));
        current_list->head = create_singly_linked_list_node(*str,1);
        current_list->tail =current_list->head;
        hash_map[hash_function(*str , hash_size)]->head = current_list->head;
        hash_map[hash_function(*str , hash_size)]->tail = current_list->tail;
        free(current_list);
       } else { 
        /*linking/updating word recurrence if the hash slot is already taken*/
        singly_linked_list *current_list =(struct singly_linked_list*)malloc(
                                             sizeof(struct singly_linked_list));
        current_list->head = hash_map[hash_function(*str , hash_size)]->head;
        current_list->tail = hash_map[hash_function(*str , hash_size)]->tail;
        while(current_list->head!=NULL){
            if(!strcmp((current_list->head->word),*str)){
                current_list->head->word_recur++;
                break;
            }
                current_list->head = current_list->head->next;
        }
        if(current_list->head == NULL){
            insert_node_into_singly_linked_list(hash_map[hash_function(*str, 
                                                            hash_size)],*str,1);
        }
        free(current_list);
       } 
       str++;                         
    }
    return hash_map;
}
/*returns the number of a word recurrences in a given hash map*/
int word_recurrence_in_hash_map(singly_linked_list **hash_map ,char *word, 
                                                                 int hash_size){
    while(hash_map[hash_function(word,hash_size)]->head!=NULL){
        if(!strcmp(hash_map[hash_function(word,hash_size)]->head->word,word)){
            return hash_map[hash_function(word,hash_size)]->head->word_recur;
        }
        hash_map[hash_function(word,hash_size)]->head = 
                         hash_map[hash_function(word,hash_size)]->head->next;
    }
    return 0;
    
}
/* frees a given hash table and all the linked list that are chained to it*/
void free_map(singly_linked_list **hash_map, int hash_size){
    for(int i=0;i<hash_size;i++){
        free_singly_linked_list(hash_map[i]->head);
        free(hash_map[i]);
    }
    free(hash_map);
}

/*comparing to arrays of string to determine if the string array note can be 
written using only words from the string array magazine(with case sensitivity)*/
void checkMagazine(int magazine_count, char **magazine, int note_count, char 
                                                                        **note){
    int i;                   
    singly_linked_list **magazine_map=make_hash_map(size,magazine,
                                                                magazine_count);
    singly_linked_list  **note_map=make_hash_map(size,note,note_count);
    for(i=0;i<note_count;i++){
        if(word_recurrence_in_hash_map(magazine_map,*(note+i),size) <
                           word_recurrence_in_hash_map(note_map,*(note+i),size)){
            printf("No");
            free_map(magazine_map,size);
            free_map(note_map,size);
            return;
        }
    }
    printf("Yes");
    free_map(magazine_map,size);
    free_map(note_map,size);
}

int main()
{
    char** mn = split_string(readline());

    char* m_endptr;
    char* m_str = mn[0];
    int m = strtol(m_str, &m_endptr, 10);

    if (m_endptr == m_str || *m_endptr != '\0') { exit(EXIT_FAILURE); }

    char* n_endptr;
    char* n_str = mn[1];
    int n = strtol(n_str, &n_endptr, 10);

    if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

    char** magazine_temp = split_string(readline());

    char** magazine = malloc(m * sizeof(char*));

    for (int i = 0; i < m; i++) {
        char* magazine_item = *(magazine_temp + i);

        *(magazine + i) = magazine_item;
    }

    int magazine_count = m;

    char** note_temp = split_string(readline());

    char** note = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++) {
        char* note_item = *(note_temp + i);

        *(note + i) = note_item;
    }

    int note_count = n;

    checkMagazine(magazine_count, magazine, note_count, note);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!line) {
            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);
    } else {
        data = realloc(data, data_length + 1);

        data[data_length] = '\0';
    }

    return data;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}
