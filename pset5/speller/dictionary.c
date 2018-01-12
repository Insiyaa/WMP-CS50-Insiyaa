/**
 * Implements a dictionary's functionality.
 */
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "dictionary.h"

/**
 * Returns true if word is in dictionary else false.
 */

bool check(const char *word)
{
    int len = strlen(word);
    int level;
    int index;
    copy = root;
    for(level = 0; level < len; level++)
    {
        index =  charIndex(word[level]);
        if(copy -> children[index] == NULL)
            return false;
        copy = copy -> children[index];
    }
    return (copy != NULL && copy -> is_word);
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
node *getNode()
{
    node *newNode = NULL;
    newNode = (node *)malloc(sizeof(node));
    if(newNode)
    {
        newNode -> is_word = false;
        int i = 0;
        for(i = 0; i < 27; i++)
            newNode -> children[i] = NULL;
    }
    return newNode;
}

int charIndex(char key)
{
    if(key >= 'A' && key <= 'Z')
    {
        return key%65;
    }
    else if(key == '\'')
        return 26;
    else
        return key%97;

}
void insert(node *rooti, const char *word)
{
    int len = strlen(word);
    int level;
    int index;
    copy = rooti;
    for(level = 0; level < len; level++)
    {
        index = charIndex(word[level]);
        if(copy -> children[index] == NULL)
        {
            copy -> children[index] = getNode();
        }
        copy = copy -> children[index];
    }
    copy -> is_word = true;
    sizei++;
}

bool load(const char *dictionary)
{
    loadcheck = false;
    FILE *dict = fopen(dictionary, "r");

    if(dict == NULL)
    {
        fprintf(stderr,"Can't openn\n");
        return false;
    }

    root = getNode();
    while(1)
    {
        char k = fgetc(dict);
        if(k == EOF)
            break;
        char *word = malloc(LENGTH + 1);
        int i = 0;
        do{
            word[i] = k;
            i++;
            k = fgetc(dict);
        }
        while(k != '\n');
        word[i] = '\0';

        insert(root, word);
        free(word);
    }
    loadcheck = true;
    fclose(dict);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if(!loadcheck)
        return 0;
    return sizei;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
void freeIt(node *copy)
{
    for(int i = 0; i < 27; i++)
    {
        if(copy -> children[i] != NULL)
        {
            freeIt(copy -> children[i]);
        }

    }
    free(copy);
}

bool unload(void)
{
    copy = root;
    freeIt(copy);
    return true;
}

