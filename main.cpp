#include <stdio.h>      //printf, fopen, fclose, feof, fgetc
#include <stdlib.h>     //malloc, realloc, free
#include <string.h>     //memcpy
#include <math.h>       //pow

long test_adress1 = 0;
long test_adress2 = 0;
void* test_adress3;

//simple int parser
int int_parse(char str[], int len) 
{
    int result = 0;
    int index = 0;
    char output_str[len];

    // cleans the input from stray chars
    for (int i = 0; i < len; i++)
    {
        if(str[i] < 48 || str[i] > 57) continue;
        output_str[index] = str[i];
        index++;
    }
    // parses the cleaned string
    for (int i = 0; i < index; i++)
    {
        int parse_num = (int) output_str[i] - 48;
        result += parse_num * pow(10, index-(i+1));
    }
    return result;
}
//simple string comparer, returns a 0 if they do not match, 1 if they do
int str_compare(char str_1[], char str_2[], int min_len)
{
    for (int i = 0; i < min_len; i++)
    {
        if(str_1[i] != str_2[i]) return 0;
    }
    return 1;
}

struct target
{
    target* next_target;
    int weight;
    char name[16];
    //creates a terget relation and fill in the infromation automatically
    //I just didn't want to write this chunk of code 5 times
    target create(char* arr_collection[], int src_pos)
    {
        target temp_relation;
        temp_relation.next_target = NULL;
        temp_relation.weight = int_parse(arr_collection[2], 3);
        if(src_pos)
        {
            memcpy(temp_relation.name, arr_collection[0], 16);
            return temp_relation;
        }
        memcpy(temp_relation.name, arr_collection[1], 16);
        return temp_relation;
    }
    //gets the address of the indexed relation
    target* get(int index, target* root)
    {
        target* result = root;
        for (int i = 0; i < index-1; i++)
        {
            result = result -> next_target;
        }
        return result;
    }
};


struct person
{
    char name[16];
    // number of relations, indexing starts at 1
    int relation_num;
    target relations;

    //pushes a new target into the linked list of targets
    void push_relation(target tar)
    {
        target* temp_address = (target*) malloc(sizeof(target));
        tar.get(relation_num, &relations)->next_target = temp_address;
        *temp_address = tar;
        relation_num++;
    }
    //fills out the necessary fields in a new instace of person.
    person create(char name[16], target tar)
    {
        person temp_person;
        memcpy(temp_person.name, name, 16);
        temp_person.relation_num = 1;
        temp_person.relations = tar;
        return temp_person;
    }
};

struct dyn_arr
{
    // the array
    person* arr;
    // the amount of entries
    int count;
    // the amount of allocated memory
    int size;

    void init()
    {
        arr = (person*) malloc(sizeof(person)*8);
        test_adress1 = (long) arr;
        size = 8;
        count = 0;
    } 

    void push(person test)
    {
        if(count == size) expand();
        test_adress2 = (long) arr;
        arr[count] = test;
        count++;
    }

    void expand()
    {
        size = (size*2);
        person* temp_pointer = (person*) realloc(arr, size*sizeof(person));
        arr = temp_pointer;
    }

    void die()
    {
        free(arr);
    }

    // TODO: Later
    void sort()
    {
        int index = 0;
        int sorted = 0;
        
        while(sorted)
        {

        }
    }
};

//Fills an array with ' ' of given length, affixing null at the end 
void clear_arr(char* arr, int arr_size)
{
    for(int i = 0; i < arr_size; i++)
    {
        arr[i] = ' ';
    }
    arr[arr_size-1] = (char) 0;
}


//overflow so i don't need to write the same code chunk twice
void add_char(char* arr_collection[3], dyn_arr* arr, int src)
{
    //checks if the person already exists, if so just adds a relation.
    for (int i = 0; i < arr -> count; i++)
    {
        if(str_compare(arr_collection[src], arr -> arr[i].name, 16))
        {
            target temp_relation;
            temp_relation = temp_relation.create(arr_collection, src);
            arr->arr[i].push_relation(temp_relation);
            return;
        }
    }
    // creates a target/relation with the opposite person
    target temp_relation;
    temp_relation = temp_relation.create(arr_collection, src);

    // creates a person with the name in primary_arr
    person temp_person;
    temp_person = temp_person.create(arr_collection[src], temp_relation);
    arr->push(temp_person); 
}

//adds a person to the dyn_arr, along with whatever relations given.
void add_char(char* arr_collection[], dyn_arr* arr)
{
    //don't want the same chunk of code twice
    add_char(arr_collection, arr, 0);
    add_char(arr_collection, arr, 1);
}

void display_charaters(dyn_arr* arr)
{
    for (int i = 0; i < arr->count; i=i+3)
    {
        printf("%s| %s| %s\n", arr->arr[i].name, arr->arr[i+1].name, arr->arr[i+2].name);
    }
}

int main() 
{
    int cycle_offset = 0;
    dyn_arr arr_cast;
    char* arr_collection[3];

    arr_cast.init();
    
    char primary_name[16];
    arr_collection[0] = primary_name;
    
    char secondary_name[16];
    arr_collection[1] = secondary_name;
    
    char relation_weight[16];
    arr_collection[2] = relation_weight;

    clear_arr(primary_name, 16);
    clear_arr(secondary_name, 16);
    clear_arr(relation_weight, 3);

    char line[32];
    int column = 0;
    int index = 0;
    FILE* file = fopen("./thrones.csv", "r");

    while(primary_name[0] != '\n')
    {
        primary_name[0] = fgetc(file);
    } 
    while(true)
    {

        //adds a 1 cycle offset to the loop so it does the necessary logic with the last line of the sheet
        if(feof(file) != 0)
        {
            cycle_offset++;
        }

        //gets a char
        char cur_char = fgetc(file);
        


        //puts the name in the secondary name array
        if(cur_char == ',')
        {

            index = 0;
            column++;
            continue;
        }
        //adds both people to the cast array, or to their relationship arrays if they already exist
        if(cur_char == '\n' || cycle_offset != 0)
        {
            index = 0;
            column = 0;

            add_char(arr_collection, &arr_cast);

            clear_arr(primary_name, 16);
            clear_arr(secondary_name, 16);
            clear_arr(relation_weight, 3);

            if(cycle_offset != 0) break;

            continue;
        }
        //puts the char into the right array using 'column' as an arr index. Saving the name one char at a time
        arr_collection[column][index] = cur_char;
        index++;
        // printf("%c", cur_char);
    }
    fclose(file);
    // for (int i = 0; i < arr_cast.count; i++)
    // {
    //     printf("%s\n", arr_cast.arr[i].name);
    //     for (int ii = 1; ii <= arr_cast.arr[i].relation_num; ii++)
    //     {
    //         target* temp_target = arr_cast.arr[i].relations.get(ii, &arr_cast.arr[i].relations);
    //         printf("    %s %d\n", temp_target -> name, temp_target -> weight);
    //     }
    // }

    int loop = 1;
    do
    {
        display_charaters(&arr_cast);
        puts("Write a name to inspect relations");
        char name[16];
        clear_arr(name, 16);
        char input[16];
        fgets(input, 16, stdin);
        for (int i = 0; i < 16; i++)
        {
            if(input[i] == '\n') break;
            name[i] = input[i];
        }
        int not_found = 1;
        for (int i = 0; i < arr_cast.count; i++)
        {
            if(!str_compare(name, arr_cast.arr[i].name, 16)) continue;
            not_found--;
            puts("");
            printf("%s\n", arr_cast.arr[i].name);
            for (int ii = 1; ii <= arr_cast.arr[i].relation_num; ii++)
            {
                target* temp_target = arr_cast.arr[i].relations.get(ii, &arr_cast.arr[i].relations);
                printf("    %s| %d\n", temp_target -> name, temp_target -> weight);
            }
        }
        if(not_found)
        {
            puts("Character not found.\nBeware that names are case sensitive.\n\nQuit instead? [y/n]");
            char quit_input[2];
            fgets(quit_input, 2, stdin);
            if(quit_input[0] == 'y') break;
        }
        puts("...");
        fgets(input, 16, stdin);
    } while (loop);
    arr_cast.die();
    return 0;
}