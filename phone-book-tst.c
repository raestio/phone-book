
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0

typedef struct TContact
{
    char * name;
    char * phoneNumber;
} Contact;

typedef struct TNode
{   
    int flag;
    Contact contact;
    char c;
    struct TNode * mid, * left, * right;
} Node;

typedef struct TSTree
{
    Node * root;
} TernarySearchTree;


/*-----------------------------Input data part---------------------------------*/
/*-----------------------------------------------------------------------------*/
/* A function reallocs memory of an array (char), given as first parameter.
 * Function returns:
 *      pointer to newly allocated array of chars (string)
 */
char * reallocMemory(char * string, int * maxMemory)
{   
    //Increase effectively current memory
    *maxMemory += ( *maxMemory < 100 ) ? 10 : *maxMemory / 2;
    
    string = (char*) realloc(string, *maxMemory * sizeof(char));
    return string;
}

/*-----------------------------------------------------------------------------*/
/* Read command from stdin:
 * + (Add new contact)
 * ? (Find contact)
 */
int readCommand(char * command)
{
    *command = fgetc(stdin);
    if (*command == EOF)
        return EOF;
    else if (*command == '+' || *command == '?')
        return 1;
    else if (*command == '\n')
        return 0;
    else
    {   
        //Incorrect command -> read rest of line
        while (fgetc(stdin) != '\n') {}
    }
        
    return 0;
}

/*-----------------------------------------------------------------------------*/
/* Reads data (phone number) from stdin
 * A function returns:
 *      -2 = Incorrect character - newline 
 *      0  = Incorrect character
 *      1  = OK
 */
int readPhoneNumber(char ** phoneNumber)
{
    char * tmpNumber = NULL;
    char c;
    int maxMemory = 0, pNumberLength = 0;
    
    //Read number characters until a space character occur.
    while (true)
    {   
        
        if ((c = fgetc(stdin)) < '0' || c > '9')
        {
            if (c == '\n')
                return -2;
            else if (c == ' ' && pNumberLength > 0)
                break;
            else
                return 0;
        }
        
        /* If length of phone number (including string's end character '\0') is greater than 
         * currently allocated memory (1 char = 1 B), realloc memory.
         * (We want to avoid getting a segmentation fault.)
         */
        if (pNumberLength +1 >= maxMemory)
            tmpNumber = reallocMemory(tmpNumber, &maxMemory);
        
        tmpNumber[pNumberLength++] = c;
        *phoneNumber = tmpNumber;
    }
        
    tmpNumber[pNumberLength] = '\0';
    *phoneNumber = tmpNumber;
    return 1;
}

/*-----------------------------------------------------------------------------*/
/* Reads data (name) from stdin
 * A function returns:
 *  -2 = Incorrect character - newline 
 *  0  = Incorrect character
 *  1  = OK
 */
int readName(char ** name)
{
    char * tmpName = NULL;
    char c;
    int maxMemory = 0, nameLength = 0, spacesInRow = 0;
    int spaceAtEnd = false;
    
    while (true)
    {
        c = fgetc(stdin);  
        if ((c < 'a' || c > 'z') && (c < 'A' || c > 'Z') && c != ' ')
        {   
            //Empty name string
            if (nameLength == 0)
            {
                if (c == '\n')
                    return -2;
                else
                    return 0;
            }
            
            //Incorrect character
            if (c != '\n')
                return 0;
            //Newline and space at end of name
            else if (c == '\n' && spaceAtEnd)
                return -2;
            //OK
            else
                break;            
        }
        else
        {   
            if (c == ' ')
            {
                spacesInRow++;
                spaceAtEnd = true;
                if (spacesInRow == 2 || nameLength == 0)
                    return 0;
            }
            else
            { 
                spacesInRow = 0;
                spaceAtEnd = false;
            }
            /* If length of name (including string's end character '\0') is greater than 
            * currently allocated memmory (1 char = 1 B), realloc memmory.
            * (We want to avoid getting a segmentation fault.)
            */
            if (nameLength + 1 >= maxMemory)
                tmpName = reallocMemory(tmpName, &maxMemory);
        
            tmpName[nameLength++] = c;
            *name = tmpName;
        }
        
    }
        
    tmpName[nameLength] = '\0';
    *name = tmpName;
    return 1;
}

/*-----------------------------------------------------------------------------*/
/* Reads data
 * A function returns:
 *  -2 = Incorrect character - newline 
 *  0  = Incorrect character
 *  1  = OK
 */
int readContact(Contact * contact)
{   
    //Initialize pointers
    contact->name = NULL;
    contact->phoneNumber = NULL;
    
    int res;
    //Read phone number
    if ((res = readPhoneNumber(&contact->phoneNumber)) == 0)
        return 0;
    else if (res == -2)
        return -2;
    
    //Read name
    if ((res = readName(&contact->name)) == 0)
        return 0;
    else if (res == -2)
        return -2;
    
    return 1;
}

/*-----------------------------------------------------------------------------*/
/* Reads pattern from stdin.
 * A function returns:
 *  -1 = Newline (typed as first character)
 *  0  = Incorrect character
 *  1  = OK - search by name
 *  2  = OK - search by number
 */
int readPattern(char ** pattern)
{   
    char * tmpPattern = NULL;
    char c;
    int maxMemory = 0, patternLength = 0;
    int byName = false, byNumber = false;
    
    while (true)
    {   
        c = fgetc(stdin);
        if (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ') && !byNumber)
            byName = true;
        else if (c >= '0' && c <= '9' && !byName)
            byNumber = true;
        else if (patternLength == 0 && c == '\n')
            return -1;
        else if (c == '\n')
            break;
        else
            return 0;
        
        
        if (patternLength +1 >= maxMemory)
            tmpPattern = reallocMemory(tmpPattern, &maxMemory);
        
        tmpPattern[patternLength++] = c;
        *pattern = tmpPattern;
    }
        
    tmpPattern[patternLength] = '\0';
    *pattern = tmpPattern;
    
    if (byName)
        return 1;
    else
        return 2;
}



/*-------------------------Ternary search tree part----------------------------*/
/*-----------------------------------------------------------------------------*/
/* Initialize the ternary search tree
 */
void initTST(TernarySearchTree * tst)
{
    tst->root = NULL;
}

/*-----------------------------------------------------------------------------*/
/* A function creates new node that is represented by given character (parameter)
 * and returns it. 
 */
Node * getNewNode(char c)
{
    Node * node = (Node*) malloc(sizeof(Node));
    node->c = c;
    node->flag = false;
    //Initialize pointers
    node->left = node->right = node->mid = NULL;
    return node;
}

/*-----------------------------------------------------------------------------*/
/* A recursive function which adds given key (third parameter) to ternary search tree and adds
 * contact data to last node of current key in tree.
 * 
 * Params:
 * node : root of TSTree
 * data : contact 
 * key  : creating nodes by this key
 * d    : array(string) index
 * 
 * Function returns:
 *      pointer to recreated tree ("changed root")
 */
Node * put(Node * node, Contact data, char * key, int d)
{   
    char c = key[d];
    
    //If node doesn't exist, create new one.
    if (node == NULL)
        node = getNewNode(c);
    
    //Current character is less than character in node -> go to left node.
    if (c < node->c)
        node->left = put(node->left, data, key, d);
    //Current character is greater than character in node -> go to right node.
    else if (c > node->c)
        node->right = put(node->right, data, key, d);
    /* Current character is identical to character in node -> go to middle node
     * and increase key's index (node with this character already exist). 
     * And it's not the last key's character.
     */
    else if (d < (int)strlen(key) - 1)
        node->mid = put(node->mid, data, key, d + 1);
    //Last key's character and character in node are identical -> assign data to node.
    else
    {
        node->contact = data;
        node->flag = true; //node contains contact data
    }
    return node;    
}
void putToTST(TernarySearchTree * tst, Contact data, char * key)
{   
    //d = 0 (starting index)
    tst->root = put(tst->root, data, key, 0);
}

/*-----------------------------------------------------------------------------*/
/* A recursive function which searches for a contact in TST by given pattern.

 * Params:
 * node    : root of TST
 * pattern : searched by
 * d       : array(string) index
 * 
 * Function returns:
 *    1. pointer to node containing given pattern
 *    2. null pointer = contact with given pattern doesn't exist
 * 
 */
Node * search(Node * node, char * pattern, int d)
{
    if (node == NULL)
        return NULL;
    
    char c = pattern[d];
    
    if (c < node->c)
        return search(node->left, pattern, d);
    else if (c > node->c)
        return search(node->right, pattern, d);
    else if (d < (int)strlen(pattern) - 1)
        return search(node->mid, pattern, d + 1);
    else
        return node;    
}
Node * searchInTST(TernarySearchTree tst, char * pattern)
{  
    //d = 0 (starting index)
    return search(tst.root, pattern, 0);
}

/*-----------------------------------------------------------------------------*/
/* A function recursively frees the TST.
 * 
 * Params:
 * node          : root of TST
 * freeContent   : 
 *      if true  : free node's data (name, phone number)
 *      if false : free only node
 */
void destroy(Node * node, int freeContent)
{
    if (node == NULL)
        return;
    
    destroy(node->left, freeContent);
    destroy(node->mid, freeContent);
    destroy(node->right, freeContent);
    if (node->flag && freeContent)
    {
        free(node->contact.name);
        free(node->contact.phoneNumber);
    }
    free(node);
}
void DestroyTST(TernarySearchTree tst, int freeContent)
{
    destroy(tst.root, freeContent);
}



/*------------------------------Output part------------------------------------*/
/*-----------------------------------------------------------------------------*/
/* A function prints recursively all nodes that contain contact data.
 * 
 * Params:
 * node : printing all children nodes of this node that contain contact data and node's data itself
 * totalContacts 
 */
void printTST(Node * node, int * totalContacts)
{
    if (node == NULL)
        return;
    
    printTST(node->left, totalContacts);
    printTST(node->mid, totalContacts);
    printTST(node->right, totalContacts);
    //if node contains contact data -> print
    if (node->flag)
    {
        (*totalContacts)++;
        printf("%s %s\n", node->contact.phoneNumber, node->contact.name);
    }
}

/*-----------------------------------------------------------------------------*/
/* A function finds and prints all contacts that match with pattern.  
 */
void printContacts(TernarySearchTree tst, char * pattern)
{   
    int totalContacts = 0;
    //search for node in given TST
    Node * node = searchInTST(tst, pattern);
    if (node == NULL)
    {
       printf("No contact found.\n");    
    }
    else
    {   
       //if node contains contact data -> print 
       if (node->flag)
       {
           totalContacts++;
           printf("%s %s\n", node->contact.phoneNumber, node->contact.name);
       }
       printTST(node->mid, &totalContacts);
    }
    free(pattern);
    printf("Total: %d\n", totalContacts);
}

/*-----------------------------------------------------------------------------*/
int main() {
    
    int res;
    char command, c;
    Contact contact;
    TernarySearchTree numberTST, nameTST;
    
    initTST(&numberTST);
    initTST(&nameTST);
    
    BEGIN : do
    {   
        //Read command from stdin
        if ((res = readCommand(&command)) == EOF)
            break;
        else if (res == 0)
        {
            printf("(Input error) Command not found.\n");
            goto BEGIN;
        }
         
        //Read one gap
        if ((c = fgetc(stdin)) != ' ')
        {
            printf("(Input error) Usage:\n+ phoneNumber name\n? prefix[name||phoneNumber]\n");
            //input error -> read rest of line
            if (c != '\n')
                while (fgetc(stdin) != '\n') {}
            goto BEGIN;
        }
            
        //Add new contact
        if (command == '+')
        {   
            //Read contact data from stdin and check correctness of characters
            if ((res = readContact(&contact)) == 0 || res == -2)
            {
                printf("(Input error) Usage:\n+ phoneNumber name\n");
                free(contact.phoneNumber);
                free(contact.name);
                //If an incorrect character wasn't a newline, read rest of line until a newline character occur.
                if (res != -2)
                    while (fgetc(stdin) != '\n') {}
                goto BEGIN;
            }          
            
            /* Check if contact is already in database(TST).
             * Name and phone number have to be unique.
             */ 
            Node * tmp1, * tmp2;
            if (((tmp1 = searchInTST(numberTST, contact.phoneNumber)) && tmp1->flag == true) || ((tmp2 = searchInTST(nameTST, contact.name)) && tmp2->flag == true))
            {   
                printf("Contact already exist.\n");
                free(contact.name);
                free(contact.phoneNumber);
                goto BEGIN;
            }
            
            /* Add contact to trees.
             * We want to search for contacts by name or by phone number.
             */ 
            putToTST(&nameTST, contact, contact.name);
            putToTST(&numberTST, contact, contact.phoneNumber);
            
            printf("OK\n");
        }
        //Search for contact
        else if (command == '?')
        {   
            //Initialize pointer
            char * pattern = NULL;
            
            //Read pattern(prefix) from stdin and check correctness of characters
            if ((res = readPattern(&pattern)) == 0)
            {
                free(pattern);
                printf("(Input error) Usage:\n? prefix[name||phoneNumber]\n");
                while (fgetc(stdin) != '\n') {}
                goto BEGIN;
            }
            
            //Empty pattern -> print all contacts
            if (res == -1)
            {  
                if (numberTST.root == NULL)
                    printf("Empty phone book.\n");
                else
                {
                    int totalContacts = 0;
                    printTST(numberTST.root, &totalContacts);
                    printf("Total: %d\n", totalContacts);
                }
            }
            //Search for contacts in TST by name and print
            else if (res == 1)
            {   
                printContacts(nameTST, pattern);
            }
            //Search for contacts in TST by phone number and print
            else if (res == 2)
            {
                printContacts(numberTST, pattern);
            }          
        }
    } while(true);
      
    /* Free TSTs
     * Pointers of strings (name, phone number) are identical in nameTST and numberTST 
     * thats why we put "true" to the first tree destroy function and "false" to second (avoid freeing same memory two times).      
     */
    DestroyTST(nameTST, true);
    DestroyTST(numberTST, false);
    
    return 0;
}

