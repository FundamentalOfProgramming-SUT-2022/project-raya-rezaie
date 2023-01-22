#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<dirent.h>
#include<errno.h>


void createfile(char *address)
{    
FILE *fptr = fopen(address, "w");
fclose(fptr);
}
char** str_chop(const char* inp, const char* space, int* num)
{
    int inp_num = 0;
    char** words = NULL;
    
    while(*inp)
    {
       char** tmp1 = realloc(words, (inp_num + 1) * sizeof *words);
       words = tmp1;

       char* tmp = strstr(inp, space);
       if (tmp == NULL) 
       { 
           int len = strlen(inp);
           words[inp_num] = calloc(len + 1, 1);
           memcpy(words[inp_num], inp, len);
           inp_num++;    
           break;
       }
       
       int len = tmp - inp;
       words[inp_num] = calloc(len + 1, 1);
       memcpy(words[inp_num], inp, len);
       inp_num++;
       inp = tmp + strlen(space);
    }
    *num = inp_num;
    return words;
} /* 
void cat(char *filename)
{
    char read ;
    FILE *file ;
    if (file = fopen("a.txt", "r"))
    {
     do {
        read = fgetc(file);
        printf("%c", read);
 
    
    } while (read != EOF);
}
}
fclose(file);
//else
printf("The file doesnt exist"); */
int main(){
    //mkdir("d:/root");
    char command [105] ;
    int num;     //number of words of a command
    printf("(Enter \"help\" for list of commands)\n\n");
    printf("Command: ");
    fgets(command , 105 , stdin);
    char ** word= str_chop(command, " " , &num);
    for ( int i = 0 ; i < num ; i ++){
    printf ("%s " ,word[i] );
    }
    printf("\n");
    if ( strcmp(word[0] ,"createfile") == 0 ){
        int dir_num ;
        char ** dir = str_chop(word[2] , "/" , &dir_num) ;

        for ( int i = 0 ; i < dir_num - 1 ; i++)
        {
            //printf("%s",dir[i]);
            /* DIR *directory = dir[i] ;
            if ( opendir(directory))  
            break;
            else if (ENOENT == errno)
            mkdir(directory) ; */
          }
        
        //createfile(word[2]);

    }   
    //if (strcmp(word[0], "cat") == 0)
    //cat(word[2]);
}
