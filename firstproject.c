#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include<unistd.h>
#include<stdbool.h> 
#include<sys/stat.h>
#include<sys/types.h>
#include <dirent.h>

#define TMP_PATH "d:/root/tmp"
#define DEF_PATH "d:/root"
#define DEF_FLDR "root"
#define line_size 500
#define file_copy 100000
#define max_line 2049
#define BUFFER_SIZE 100000
#define MAXSIZE 1024
#define MAX_LENGTH 1024
#define MAX_LINE 2000
#define TABSIZE "    "
#define MAX_SIZE 100000
#define SPACE ' '
#define DRVNAM "d:"

///////////////////GLO VAR///////////
int str_num=0;
bool FLDex=false;
char inp_str[2000];
char clipboard[55000];
////////////////////////////////////
int isDirectoryExists(const char *path);
int insert(char * filename,int line, int s_pos, char * to_add);


int cat(const char *ptr1,int rows)
{
FILE* ptr;
    char ch;
    int num_lines=0,num_chars=0,n=1,row,col;
    //getmaxyx(stdscr,row,col);
    ptr = fopen(ptr1, "r");
    if (NULL == ptr) {
        printf("File can't be opened or dose not exist.\n");
        return -1;
    }
     while (1)
     {
        ch = fgetc(ptr);
        //if (ch=='\n')
        if (ch=='\0'||ch=='\n'){ 
         num_lines++;
         n++;}
        //if (ch == '\n' || ch == '\0')
        if(ch==EOF) break;
        num_chars++;
        printf("%c", ch);
        if(rows>1&&n%(rows-1)==0)
        {
            printf("-----MORE-----");
            getchar();
            printf("\n");
            n=1;
        }

    } 
    fclose(ptr);
    printf("\n============================================EOF====================================================\n");
    if (num_chars>0) //if file not empty add one to line
    num_lines++;
    printf("Number of character(s): %d\n",num_chars);
    if(num_chars>0)
    printf("Number of line(s):      %d\n",num_lines);
    return 0;
}

char** str_chop(const char* str, const char* space, int* num)
{
    int str_num = 0;
    char** words = NULL;
    if (!*str)
    str="EMPTY STRING";    

    while(*str)
    {
       char** tmp1 = realloc(words, (str_num + 1) * sizeof *words);
       words = tmp1;

       char* tmp = strstr(str, space);
       if (tmp == NULL) 
       { 
           int len = strlen(str);
           words[str_num] = calloc(len + 1, 1);
           memcpy(words[str_num], str, len);
           str_num++;    
           break;
       }
       
       int len = tmp - str;
       words[str_num] = calloc(len + 1, 1);
       if (words[str_num] == NULL) exit(1);
       memcpy(words[str_num], str, len);
       str_num++;
       str = tmp + strlen(space);
    }
    *num = str_num;
    return words;
}

/* int filecreate(char *name )
{
    
    FILE *fptr = fopen(name,"w");
    fclose(fptr);

}
 */
int fileexist(char *filename)
{
    FILE * fptr = fopen(filename,"r");
    if ( fptr == NULL)
    {
    //printf("the entered file doesn't exist.");
    return 0 ;
    }
    else return 1;
}

void createfile(char *address)
{    
    //printf("%s",address);
    FILE *fptr = fopen(address, "r");
    if (fptr == NULL)
    fptr = fopen(address,"w");
    else
    printf("\nFile already exists.");
    fclose(fptr);
}



int main()
{
    
    int columns, rows,v,start_folder=1;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    char ch[]="d:/root";
    bool root=true;

CreateDirectory (DEF_PATH, NULL);  
chdir(DEF_PATH);
printf("Default working path: \"d:\\root\"\n");

while(1)
{
    FLDex=FALSE;
    printf("\nCommand: ");
    gets(inp_str);
    //printf("%s",inp_str);
    char** word = str_chop(inp_str, " ", &str_num);
    if (strcmp(word[0], "createfile") == 0&&strcmp(word[1], "-file") == 0&&word[2] != NULL)
    
    //check if pipe command entered and set falg 
    //parse the entered command
    {   //printf("%s",word[1]);
        if(*word[2]=='\"')
        {
            char** word = str_chop(inp_str, "\"", &str_num);
            char** folders = str_chop(word[1], "/", &str_num);
            if((strcmp(folders[1], DEF_FLDR))==0)
            start_folder=2;
            //printf("%d",v);
            //DEF_FLD
            chdir(DEF_PATH);
            for (int i=start_folder;i<str_num-1;i++)
            {
                 CreateDirectory (folders[i], NULL);
                 chdir(folders[i]);
            }
            //printf("%d",str_num);
            //printf("%s",folders[str_num-1]);
            //getchar();
            createfile(folders[str_num-1]);
            //create file  last name in string

            //check for existance of file
            //
            //printf("%d folder(s) created.\n",i);
            //printf("Current folder:%s",folder[i])
        } 
        else if(*word[2]=='/')
        {
            char** folders = str_chop(word[2], "/", &str_num);
            if((strcmp(folders[1], DEF_FLDR))==0)
            start_folder=2;
            chdir(DEF_PATH);
            for (int i=2;i<str_num-1;i++)
            {
                 CreateDirectory (folders[i], NULL);
                 chdir(folders[i]);
            } 
            createfile(folders[str_num-1]);
        }else{
         printf("Invalid arg.\n");
        }
        //for (int i=0;i<str_num;i++)
        //printf("%s\n",folders[i]);
       /*   for (int i=0;i<str_num;i++)
        strcat(folders[0],folders[i]);  
  
            printf("%s\n",folders[0]); */
    }
    else if (strcmp(word[0], "quit") == 0 || strcmp(word[0], "q") == 0)
    exit(0);

    else if (strcmp(word[0], "cat") == 0&&strcmp(word[1], "-file") == 0)
    {char** wordcat;
        if(*word[2]=='\"'){
        wordcat = str_chop(inp_str, "\"", &str_num);
        //printf("%s\n",wordcat[1]);
        // printf("%c\n",*word[2]);
        //getchar();
        //if(*word[2]=='/')
        //printf("%s",word[2]);
        if (isDirectoryExists(wordcat[1]))
            FLDex=true; 
           if((*word[2]=='\"')&&(FLDex==true))
         {
            char** word = str_chop(inp_str, "\"", &str_num);
            //char** folders = str_chop(word[1], "/", &str_num);
          
            //if (strcmp(word[0], "createfile") == 0&&strcmp(word[1], "-file") == 0&&word[2] != NULL)
            //printf("\n%s\n",word[1]);
            //getchar();
            cat(word[1],rows);
         }}
         //printf("%s",word[2]);
         //getchar();
          if (isDirectoryExists(word[2]))
            FLDex=true; 
          if((*word[2]=='/')&&(FLDex==true))
         {
            //char** word = str_chop(inp_str, " ", &str_num);
             //printf("\n%s\n",word[2]);
             //getchar();
            cat(word[2],rows);
            //if((strcmp(folders[1], DEF_FLDR))==0)
         }
         if (FLDex==FALSE) printf("Directory does not exists.\n");
    } //end of cat

    else if (strcmp(word[0], "auto-indent") == 0)
    {
        //printf("%s",word[1]);
        
        autoind(word[1]);
    }

    else if (strcmp(word[0], "tree") == 0)
    {
        //printf("%s",word[1]);
        
        tree(word[1],0);
    }

    else if (strcmp(word[0], "compare") == 0)
    {
        //printf("%s",word[1]);
        
        fcompare(word[1],word[2]);
    }


    else if (strcmp(word[0], "insertstr") == 0)
    {
        char** pose = str_chop(word[6], ":", &str_num);
        //for(int i=0;i<str_num;i++)
        //printf("%s\n",pose[i]);
        //printf("%s %c %c %s\n",word[2],*(word[6]),*(word[6]+2),word[4]);
        int line=atoi(pose[0]);
        int line_pose=atoi(pose[1]);
        //printf("%d",line);
        //printf("%d",line_pose);
        //int line_pose=*(word[6]+2)-48;
        insert(word[2],line,line_pose,word[4]);
        //insert(word[2],line,line_pose,word[4]);
           //insertstr –file "/root/dir1/dir2/file.txt" –str Salam –pos 2:5
             //char** word = str_chop(inp_str, "\"", &str_num);
             //char** folders = str_chop(word[1], "/", &str_num);
             //insert(char * filename,int line, int s_pos, char * to_add)
             //if((strcmp(folders[1], DEF_FLDR))==0)
    }

    else if (strcmp(word[0], "removetstr") == 0)
    {
        char** poser = str_chop(word[4], ":", &str_num);
        //removetstr -file /root/1.txt -pos 2:1 -size 3 -f
        if (strcmp(word[7], "-f") == 0)
        {
             int line=atoi(poser[0]);
             int line_pose=atoi(poser[1]);
             int size1=atoi(word[6]);
            //printf("%s,%d,%d,%s",word[2],line,line_pose,size);
            //removeforward(word[2],line,line_pose,size);
             removeforward(word[2],line,line_pose,size1);
        }
        else if (strcmp(word[7], "-b") == 0)
        {
             int line=atoi(poser[0]);
             int line_pose=atoi(poser[1]);
             int size2=atoi(word[6]);
             removebackward(word[2],line,line_pose,size2);
            //removebackward(word[2],4,15,5);
        }
        //for(int i=0;i<str_num;i++)
        //printf("%s\n",pose[i]);
        //printf("%s %c %c %s\n",word[2],*(word[6]),*(word[6]+2),word[4]);
        //int line=atoi(pose[0]);
        //int line_pose=atoi(pose[1]);
        //int charnum=atoi(pose1[5])
        //printf("%d",line);
        //printf("%d",line_pose);
        //int line_pose=*(word[6]+2)-48;
        //insert(word[2],line,line_pose,word[4]);
        //insert(word[2],line,line_pose,word[4]);
           //insertstr –file "/root/dir1/dir2/file.txt" –str Salam –pos 2:5
             //char** word = str_chop(inp_str, "\"", &str_num);
             //char** folders = str_chop(word[1], "/", &str_num);
             //insert(char * filename,int line, int s_pos, char * to_add)
             //if((strcmp(folders[1], DEF_FLDR))==0)
    }
    else if (strcmp(word[0], "cutstr") == 0)
    {
        char** poser = str_chop(word[4], ":", &str_num);
        //removetstr -file /root/1.txt -pos 2:1 -size 3 -f
        if (strcmp(word[7], "-f") == 0)
        {
             int line=atoi(poser[0]);
             int line_pose=atoi(poser[1]);
             int size1=atoi(word[6]);
            //printf("%s,%d,%d,%s",word[2],line,line_pose,size);
            //removeforward(word[2],line,line_pose,size);
            cutforward(word[2],line,line_pose,size1);
        }
        else if (strcmp(word[7], "-b") == 0)
        {
             int line=atoi(poser[0]);
             int line_pose=atoi(poser[1]);
             int size2=atoi(word[6]);
             cutbackward(word[2],line,line_pose,size2);
            //removebackward(word[2],4,15,5);
        }
        //for(int i=0;i<str_num;i++)
        //printf("%s\n",pose[i]);
        //printf("%s %c %c %s\n",word[2],*(word[6]),*(word[6]+2),word[4]);
        //int line=atoi(pose[0]);
        //int line_pose=atoi(pose[1]);
        //int charnum=atoi(pose1[5])
        //printf("%d",line);
        //printf("%d",line_pose);
        //int line_pose=*(word[6]+2)-48;
        //insert(word[2],line,line_pose,word[4]);
        //insert(word[2],line,line_pose,word[4]);
           //insertstr –file "/root/dir1/dir2/file.txt" –str Salam –pos 2:5
             //char** word = str_chop(inp_str, "\"", &str_num);
             //char** folders = str_chop(word[1], "/", &str_num);
             //insert(char * filename,int line, int s_pos, char * to_add)
             //if((strcmp(folders[1], DEF_FLDR))==0)
    }
    else if (strcmp(word[0], "pastestr") == 0)
    {
        char** poseps = str_chop(word[4], ":", &str_num);
        //for(int i=0;i<str_num;i++)
        //printf("%s\n",pose[i]);
        //printf("%s %c %c %s\n",word[2],*(word[6]),*(word[6]+2),word[4]);
        int line=atoi(poseps[0]);
        int line_pose=atoi(poseps[1]);
        //printf("%d",line);
        //printf("%d",line_pose);
        //int line_pose=*(word[6]+2)-48;
        paste(word[2],line,line_pose);
        //insert(word[2],line,line_pose,word[4]);
           //insertstr –file "/root/dir1/dir2/file.txt" –str Salam –pos 2:5
             //char** word = str_chop(inp_str, "\"", &str_num);
             //char** folders = str_chop(word[1], "/", &str_num);
             //insert(char * filename,int line, int s_pos, char * to_add)
             //if((strcmp(folders[1], DEF_FLDR))==0)
    }
    else if (strcmp(word[0], "find") == 0)
    {
        int *l=0;
        int *l1=0;
        find(word[1],word[2],l,l1);
    }
   else if (strcmp(word[0], "findall") == 0)
    {

        findall(word[1],word[2]);
    
    }
  else if (strcmp(word[0], "findat") == 0)
    {
        int num;
        num=atoi(word[3]);
        findat(word[1],word[2],num);
    
    }
  else if (strcmp(word[0], "findword") == 0)
    {

        findbyword(word[1],word[2]);
    
    }
 



    else
    {
    printf("Invalid Command or Input.\n");
    printf("Try again or enter \"help\" for list of commands.\n");
    }

}//while
          
}//////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\



int insert(char * filename,int line, int s_pos, char * to_add)
{
    char string[file_copy]={0};
    char str[file_copy]={0};
    char c,ch,ch2 ;
    FILE *fptr,*tfptr;
    int line_counter=0,cur_linenum=0;
    int ch_counter=0;
    tfptr = fopen("d:/root/temp.txt" , "w");
    //printf("%s",filename);
    fptr = fopen(filename,"r");
        if (fptr==NULL) {
            fclose(fptr);
            fclose(tfptr);
        printf("File can't be opened or dose not exist.\n");
        return -1;
    }
    
    while(c != EOF)      //counting how many lines the file has
    {
        //printf("%c",c);
        c = getc(fptr);
        if(c=='\n')
        line_counter++;
        }
    fseek(fptr, 0, SEEK_SET);
    if ( line > line_counter)
    {
    printf("the file doesnt include the entered line");       
     fclose(fptr);
     fclose(tfptr);

    return -2;
    }
    else{
        for (int i=0 ; i<line-1 ; i++)
        {
        fgets(string,line_size,fptr);
        strcat(str,string);
        cur_linenum++;
    }

    if ( cur_linenum == line-1)
    {
        fseek(fptr,0,SEEK_CUR);
        while ( ch_counter != s_pos)
        {
            ch = fgetc(fptr);
            strncat(str,&ch,1);
            ch_counter ++;
        }
    }
    }
    strcat(str,to_add);
    if(ch_counter == s_pos){
        for ( int i=0 ; (ch=fgetc(fptr))!= EOF ; i++)
            strncat(str,&ch,1);
    }
        //printf("\n\n%s",str);
        fprintf(tfptr,str);
        fclose(fptr);
        fclose(tfptr);
        if (remove(filename)==0)
       // printf("The file is deleted successfully.");
        rename("d:/root/temp.txt",filename);
        //fptr=fopen(filename, "r");
        /*while((ch2 =fgetc(fptr)) != EOF )
        printf("%c",ch2);*/
        //fclose(fptr);
}



int isDirectoryExists(const char *path)
{
    //printf("%c\n\n",*path[2]);
    //printf("\n\n%s\n",path);
    //getchar();
    char new_path[250];
    char sl='/';
    char** wordp = str_chop(path, "/", &str_num);
    //printf("path: %s\n",path);
    //strncat(new_path,&sl, 1);
    for (int i=0;i<str_num-1;i++)
    {
        //printf("%s",wordp[i]);
        strcat(new_path,wordp[i]);
        strncat(new_path,&sl, 1);
    }
    //printf("%s\n",new_path);
        
        


    struct stat stats;
    stat(new_path, &stats);
    //printf("%s",path);
    //if((path[0])=='\"');
    //printf("%c",path[0]);
    //char** word = str_chop(inp_str, "\"", &str_num);
     //printf("%s",word[2]);
    //printf("%c",path[0]);
    
    

    // Check for file existence
    if (S_ISDIR(stats.st_mode))
        return 1;

    return 0;
}


void copyforward(char * filename,int line,int ch_spoint,int dlength)
{
    char c;
    char str[line_size];
    char ch,ch2,ch3;
    int line_counter=0;
    int cur_linenum=0;
    int ch_counter=0;
    int ch_epoint= ch_spoint + dlength;
    FILE *fptr = fopen(filename,"r");
  while(c != EOF)
  {
    c = getc(fptr);
    if ( c == '\n')
    line_counter ++ ;
  }
  fseek(fptr, 0, SEEK_SET);
  if ( line > line_counter)
  {
    printf("the file doesnt include the entered line");
    return;
  }
  else{
        for (int i=0; i<line-1 ; i++)
        {
        fgets(str, 200, fptr);
             cur_linenum++;
        }
         if(cur_linenum == line-1)
         {
           while( ch_counter != ch_spoint)
             {      
               ch = fgetc(fptr);
               ch_counter ++ ;
             }
               for(ch_counter=ch_spoint ;ch_counter<ch_epoint;ch_counter++)
               {
                ch  = fgetc(fptr);
                strncat(clipboard,&ch,1);
               } 
             }   
            }
            printf("%s\n",clipboard);
            fclose(fptr);
 }  

  


    void removeforward(char * filename,int line,int ch_spoint,int dlength)
{
    char new[50000];
    char c,str[100000];
    char to_delete[100000];
    char ch,ch2,ch3;
    int line_counter=0;
    int cur_linenum=0;
    int ch_counter=0;
    int ch_epoint= ch_spoint + dlength;
    FILE *fptr = fopen(filename,"r");
    FILE *tfptr =fopen("d:/root/tmpbakw.txt" , "w");
     if ( fptr == NULL)
         { 
           printf("the entered file doesn't exist.");
            fclose(fptr);
            fclose(tfptr);

         }


  while(c != EOF)
  {
    c = getc(fptr);
    if ( c == '\n')
    line_counter ++ ;
  }
  fseek(fptr, 0, SEEK_SET);
  if ( line > line_counter)
  {
    printf("the file doesnt include the entered line");
            fclose(fptr);
            fclose(tfptr);

    return;
  }
  else{
        for (int i=0; i<line-1 ; i++)
           {
             fgets(str, 200, fptr);
             strcat(new,str);
             cur_linenum++;
           }
         if(cur_linenum == line-1)
         {
           while( ch_counter != ch_spoint)
             {      
               ch = fgetc(fptr);
               strncat(new, &ch, 1);
               ch_counter ++ ;
             }
               for(ch_counter=ch_spoint ;ch_counter<=ch_epoint;ch_counter++)
               {
                ch  = fgetc(fptr);
               } 
             }   
            if (ch_counter == ch_epoint+1) 
            {
             for(ch_counter=ch_epoint ;(ch2 = fgetc(fptr)) != EOF;ch_counter++)
                strncat(new,&ch2,1);
            }
            }
           fprintf(tfptr,new);
           fclose(fptr);
           fclose(tfptr);
         if (remove(filename)==0)
        rename("d:/root/tmpbakw.txt",filename);
        fptr=fopen(filename, "r");
/*         while((ch2 =fgetc(fptr)) != EOF )
        printf("%c",ch2);
        fclose(fptr); */
  }


void autoind(char *file_add)
{

//FILE *curr = fopen("d:/root/tmp/$auto_int_tmp$", "w");
FILE *curr = fopen("d:/root/$auto_int_tmp$", "w");
FILE *org = fopen(file_add, "r");

        if (org==NULL) {
            fclose(curr);
            fclose(org);
        printf("File can't be opened or dose not exist.\n");
       
    };

int block = 0;
char *ch1,ch;

while (ch!=EOF)
    {
        ch = fgetc(org);
        {
            if (ch == '{')
            {
                fputs("\n", curr);
                for (int i = 0; i < block; i++)
                    fputs(TABSIZE, curr);
                fputc(ch, curr);
                block++;
                fputs("\n", curr);
                for (int i = 0; i < block; i++)
                    fputs(TABSIZE, curr);
            }
            else if (ch == '}')
            {
                block--;
                fputs("\n", curr);
                for (int i = 0; i < block; i++)
                    fputs(TABSIZE, curr);

                fputc(ch, curr);
            }
            else
                fputc(ch, curr);
        }
      
           
      
    } 
fclose(org);
fclose(curr);

char curname[100] = {'0'};
strcpy(curname, file_add);
strcat(curname, "OLD");

rename(file_add,curname);
rename("d:/root/$auto_int_tmp$", file_add);
}

void tree(char *basePath, const int depth)
{
    int i;
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) //ignore . and .. 
        {
            for (i=0; i<depth; i++) 
            {
                if (i%2 == 0 || i == 0)
                    printf("%c", '|');
                else   printf(" ");
            }
            struct stat buf;
            if(!S_ISDIR(buf.st_mode)){
            printf("%c%c%s\n", 195, 196, dp->d_name);} 

            strcpy(path, basePath);  
            strcat(path, "/");
            strcat(path, dp->d_name);
            tree(path, depth + 2);         }
    }

    closedir(dir);
}

//void fcompare(char *file_a,char *file_b);
long int findSize(char file_name[])
{
    FILE* fp = fopen(file_name, "rb");
    if (fp == NULL) {
        printf("File Not Found!\n");
        return -1;
    }
  
    fseek(fp, 0L, SEEK_END);
    long int res = ftell(fp);
    fclose(fp);
  
    return res;
}

int fileline(char *filename)
{
        int c;
        long count = 1;
        FILE *fp = fopen(filename,"r");
        //while ((c = getc(fp)) != EOF)
        for (c = getc(fp); c != EOF; c = getc(fp)) 
        {
              if (c == '\n') 
                 count++;
        }
        fclose(fp);
        return count;
}


void fcompare(char *file_a,char *file_b)
{
        char temp[100];
        char t1[100];
        char t2[100];
        int line;
        FILE *fp1=fopen(file_a,"r");
        FILE *fp2=fopen(file_b,"r");
        if ( fp1 == NULL|| fp2 == NULL)
         { 
           printf("the entered file doesn't exist.");
            fclose(fp1);
            fclose(fp2);

          }


        while((fgets(t1,100,fp1)!= NULL) && (fgets(t2,100,fp2)!= NULL))
        {
                line++;
                if(strcmp(t1, t2) == 0)
                {
                        //printf("same\n");
                }
                else
                {
                        t1[strcspn(t1, "\n")] = 0;
                        t2[strcspn(t2, "\n")] = 0;
                        printf("====================== #%d =======================\n",line);
                        printf("%s\n%s\n",t1,t2);
                }
        } 
fclose(fp1);
fclose(fp2);

}

void removebackward(char * filename , int line, int spoint , int length)  
{
    int pos1,pos2;
    char str[file_copy] ={0};
    char buff[line_size];
    char ch;
    int cur_linenum =0 , chcount=-0 , ch_copy = 0;
    int epoint = spoint - length ;
    pos2 = pos1 + spoint; 
    FILE *fptr = fopen(filename , "r");
    FILE *tfptr = fopen("d:/roo/tmp.txt" , "w");
         if ( fptr == NULL)
         { 
           printf("the entered file doesn't exist.");
            fclose(fptr);
            fclose(tfptr);

        }
        while((ch = fgetc(fptr))!=EOF)
        {
        chcount++;
        if (ch=='\n')
        cur_linenum ++;
        if ( cur_linenum ==line-1)
            break;
        }
        fseek(fptr,0,SEEK_SET);
        while((ch = fgetc(fptr))!=EOF)
        {
            strncat(str,&ch,1);
            ch_copy ++;
            if ( ch_copy == chcount + epoint)
                break;
        }
        fseek(fptr,spoint+length,SEEK_CUR);
        while((ch = fgetc(fptr))!=EOF)
        strncat(str,&ch,1);
        fprintf(tfptr,str);
        fclose(fptr);
        fclose(tfptr);
        if (remove(filename)==0)
        rename("d:/root/tmp.txt",filename);
        fptr=fopen(filename, "r");
        while((ch =fgetc(fptr)) != EOF )
        printf("%c",ch);
        fclose(fptr);
        }

        void cutbackward(char * filename, int line , int spoint,int length)
    {
    int pos1,pos2;
    char str[file_copy] ={0};
    char buff[line_size] ={0};
    char ch;
    int cur_linenum =0 , chcount=-0 , ch_copy = 0 ,a;
    int epoint = spoint - length ;
    pos2 = pos1 + spoint; 
    FILE *fptr = fopen(filename , "r");
    FILE *tfptr = fopen("d:/root/tmp.txt" , "w");
        while((ch = fgetc(fptr))!=EOF)
        {
        chcount++;
        if (ch=='\n')
        cur_linenum ++;
        if ( cur_linenum ==line-1)
            break;
        }
        fseek(fptr,0,SEEK_SET);
        while((ch = fgetc(fptr))!=EOF)
        {
            strncat(str,&ch,1);
            ch_copy ++;
            if ( ch_copy == chcount + epoint)
                break;
        }
        a = length+ ch_copy;
        while( ch_copy != a  )
        {
        ch = fgetc(fptr);
        strncat(clipboard,&ch,1);
        ch_copy++;
        }
        fseek(fptr,0,SEEK_CUR);
        while((ch = fgetc(fptr))!=EOF)
        strncat(str,&ch,1);
        fprintf(tfptr,str);
        fclose(fptr);
        fclose(tfptr);
       if (remove(filename)==0)
        rename("d:/root/temp.txt",filename);
    }

void cutforward(char * filename,int line,int ch_spoint,int dlength)
{
    char new[50000];
    char c,str[50000];
    char ch,ch2,ch3;
    int line_counter=0;
    int cur_linenum=0;
    int ch_counter=0;
    int ch_epoint= ch_spoint + dlength;
    FILE *fptr = fopen(filename,"r");
    FILE *tfptr =fopen("d:/root/tmp.txt" , "w+");
     if ( fptr == NULL)
         { 
           printf("the entered file doesn't exist.");
            fclose(fptr);
            fclose(tfptr);
         }

  while(c != EOF)
  {
    c = getc(fptr);
    if ( c == '\n')
    line_counter ++ ;
  }
  fseek(fptr, 0, SEEK_SET);
  if ( line > line_counter)
  {
    printf("the file doesnt include the entered line");
        fclose(fptr);
        fclose(tfptr);
    return;
  }
  else{
        for (int i=0; i<line-1 ; i++)
           {
             fgets(str, 200, fptr);
             strcat(new,str);
             cur_linenum++;
           }
         if(cur_linenum == line-1)
         {
           while( ch_counter != ch_spoint)
             {      
               ch = fgetc(fptr);
               strncat(new, &ch, 1);
               ch_counter ++ ;
             }
               for(ch_counter=ch_spoint ;ch_counter<ch_epoint;ch_counter++)
               {
                ch  = fgetc(fptr);
                strncat(clipboard,&ch,1);
               } 
             }   
            if (ch_counter == ch_epoint) 
            {
             for(ch_counter=ch_epoint ;(ch2 = fgetc(fptr)) != EOF;ch_counter++)
                strncat(new,&ch2,1);
            }
            }
        fprintf(tfptr,new);
        fclose(fptr);
        fclose(tfptr);
        if (remove(filename)==0)
        rename(filename,"d:/root/tmp.txt");
        //fopen(filename,"r");
        //fclose(fptr);
  }


 void paste(char * filename, int line , int spoint)
  {
    char string[file_copy] = {0};
    char str[file_copy] ={0};
    char c,ch,ch2 ;
    FILE *fptr,*tfptr;
    int line_counter=0,cur_linenum=0;
    int ch_counter=0;
    fptr = fopen(filename,"r");
    tfptr = fopen("d:/projecttest/temp.txt" , "w");
         if ( fptr == NULL)
         { 
           printf("the entered file doesn't exist.");
            fclose(fptr);
            fclose(tfptr);
         }
    while(c != EOF)      //counting how many lines the file has
    {
        c = getc(fptr);
        if(c=='\n')
        line_counter++;
        }
    fseek(fptr, 0, SEEK_SET);
    if ( line > line_counter)
    {
    printf("the file doesnt include the entered line");
            fclose(fptr);
            fclose(tfptr);
    return;
    }
    else{
        for (int i=0 ; i<line-1 ; i++)
        {
        fgets(string,line_size,fptr);
        strcat(str,string);
        cur_linenum++;
    }
    if ( cur_linenum == line-1)
    {
        fseek(fptr,0,SEEK_CUR);
        while ( ch_counter != spoint)
        {
            ch = fgetc(fptr);
            strncat(str,&ch,1);
            ch_counter ++;
        }
    }
    }
    strcat(str,clipboard);
    if(ch_counter == spoint){
        for ( int i=0 ; (ch=fgetc(fptr))!= EOF ; i++)
            strncat(str,&ch,1);
    }
        printf("\n\n%s\n\n",str);
        fprintf(tfptr,str);
        fclose(fptr);
        fclose(tfptr);
       if (remove(filename)==0)
        rename("d:/root/temp.txt",filename);
  } 
  void copybackward(char * filename, int line , int spoint,int length)
    {
    int pos1,pos2;
    char str[file_copy];
    char buff[line_size];
    char ch;
    int cur_linenum =0 , chcount=-0 , ch_copy = 0 ,a;
    int epoint = spoint - length ;
    pos2 = pos1 + spoint; 
    FILE *fptr = fopen(filename , "r");
    if (fptr == NULL)
         { 
           printf("the entered file doesn't exist.");
           fclose(fptr);
        }
        while((ch = fgetc(fptr))!=EOF)
        {
        chcount++;
        if (ch=='\n')
        cur_linenum ++;
        if ( cur_linenum ==line-1)
            break;
        }
        fseek(fptr,0,SEEK_SET);
        while((ch = fgetc(fptr))!=EOF)
        {
            strncat(str,&ch,1);
            ch_copy ++;
            if ( ch_copy == chcount + epoint)
                break;
        }
        a = length+ ch_copy;

        while( ch_copy != a  )
        {
        ch = fgetc(fptr);
        strncat(clipboard,&ch,1);
        ch_copy++;
        }
        fclose(fptr);
    }  


int find(char *filename, const char *word ,int *line,int * col)
{
    FILE *fptr=fopen(filename,"r");
    if (fptr == NULL)
    printf("error opening the file");
    *line =0;
    *col=-1;
    char str[BUFFER_SIZE];
    char *pos;
    int l = 0;

    while ((fgets(str, BUFFER_SIZE, fptr)) != NULL)
    {
        *line += 1;
        // Find first occurrence of word in str
        pos = strstr(str, word);
        
        if (pos != NULL)
        {
            *col = (pos - str);
            break;
        }
                l += strlen(str);

    }
        fclose(fptr);
    // If word is not found then set line to -1
    if (*col == -1)
    {
        *line = -1;
        return -1 ;
    }
    else
    return l + *col +1 ;
}
int findbyword(char *filename ,const char * word)
{
    int i=0,j=0,pos=0,cmp,flag=0;
    char ch;
    char str[500];
    FILE *fp;
    fp=fopen(filename,"r");
    while((ch=fgetc(fp))!=EOF)
    {
        if((ch==' ')||(ch=='\n'))
        {
           pos++;
           cmp=strcmp(str,word);
           if(cmp==0)
           {
                
               printf("%d",pos);
               flag=1;
               return pos;
           }
           for(i=j;i>=0;i--)
           {
               str[i]='\0';
           }
           j=0;
           continue;
        }
        str[j]=ch;
        j++;
    }
    pos++;
    cmp=strcmp(str,word);
    fclose(fp);
    if(cmp==0)
    return pos;
    else if(flag==0)
    return -1 ;
}
int findcount(char * filename ,const char* word)
{
int index;
int counter;
FILE * fptr = fopen(filename , "r");
char str[BUFFER_SIZE];
char * pos;
while(fgets(str,BUFFER_SIZE,fptr) != NULL)
{
    pos = strstr(str,word);
    if ( pos != NULL)
    counter ++;   
}
fclose(fptr);
if (counter == 0)
return -1; 
else 
return counter ;
}
void findall(char *file_A,char *word)
{
FILE *fp;
    char str[500]={'\0'};
    char ch;
    int i=0,j=0,pos=0,cmp,flag=0,chcn=0,k=0;
    int chposition[25000];
    int wordposition[25000];
    //printf("Enter a word to search\n");
    //word=string;
    fp=fopen(file_A,"r");
    while((ch=fgetc(fp))!=EOF)
    {
        chcn++;
        if((ch==SPACE)||(ch=='\n'))
        {
           pos++;
           cmp=strcmp(str,word);
           if(cmp==0)
           {
                //printf("%d",strlen(str));
                k++;
               printf("%d,",chcn-strlen(str));
                chposition[k]=chcn-strlen(str);
                wordposition[k]=pos;
               flag=1;
           }
           for(i=j;i>=0;i--)
           {
               str[i]='\0';
           }
           j=0;
           continue;
        }
        str[j]=ch;
        j++;
    }
    pos++;
    k++;
    cmp=strcmp(str,word);
    if(cmp==0)
    {
        printf("%d ",chcn-strlen(str));
        //printf("%s is found at the position [word:%d][letter:%d] from the beginning\n",word,pos,chcn-strlen(str));
                chposition[k]=chcn-strlen(str);
                wordposition[k]=pos;
    }
    else if(flag==0)
    {
        printf("-1");
    }

}
void findat(char *file_A,char *word,int num)
{
FILE *fp;
    char str[500]={'\0'};
    char ch;
    int i=0,j=0,pos=0,cmp,flag=0,chcn=0,k=0;
    int chposition[25000];
    int wordposition[25000];
    //printf("Enter a word to search\n");
    //word=string;
    fp=fopen(file_A,"r");
    while((ch=fgetc(fp))!=EOF)
    {
        chcn++;
        if((ch==SPACE)||(ch=='\n'))
        {
           pos++;
           cmp=strcmp(str,word);
           if(cmp==0)
           {
                //printf("%d",strlen(str));
                k++;
               //printf("%s is found at the position [word:%d][letter:%d] from the beginning\n",word,pos,chcn-strlen(str));
                chposition[k]=chcn-strlen(str);
                wordposition[k]=pos;
               flag=1;
           }
           for(i=j;i>=0;i--)
           {
               str[i]='\0';
           }
           j=0;
           continue;
        }
        
        str[j]=ch;
        j++;
    }
    pos++;
    k++;
    cmp=strcmp(str,word);
    if(cmp==0)
    {
        printf("%s is found at the position [word:%d][letter:%d] from the beginning\n",word,pos,chcn-strlen(str));
                chposition[k]=chcn-strlen(str);
                wordposition[k]=pos;
    }
    else if(flag==0)
    {
        //printf("%s is not found in this file",word);
    }
    if (num>k)
        printf("-1");
    else
        //printf("found at the position [word:%d][letter:%d] from the beginning\n",wordposition[num],chposition[num]);
        printf("%d\n",chposition[num]);

    //for(i=1;i<k;i++)
    //printf("found at the position [word:%d][letter:%d] from the beginning\n",wordposition[i],chposition[i]);
               

    //return 0;
}


