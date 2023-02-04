#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<dirent.h>
#include<errno.h>
#include<stdbool.h>
#include<windows.h>
#include<unistd.h>
#define line_size 500
#define file_copy 1000
#define max_line 2049
#define BUFFER_SIZE 1000
#define MAXSIZE 1024
#define MAX_LENGTH 1024
#define MAX_LINE 200
#define TABSIZE "    "
#define MAX_SIZE 90000
#define def_path "d:root"
#define def_folder "root"
#define SPACE ' '
char clipboard[20000] ;
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
}

void createfile(char *address)
{    
    //printf("%s",address);
    FILE *fptr = fopen(address, "r");
    if (fptr == NULL)
    fptr = fopen(address,"w");
    else
    printf("The entered file already exists");
    fclose(fptr);
}
int cat(const char *filename , int row)
{
    int n=1 , linenum=1 , charnum=0 ; 
    char ch ;
    FILE * fptr;
    fptr = fopen(filename , "r");
    if ( fptr == NULL)
    printf("The file cant be opened!\n");
    while(1)
    {
        ch = fgetc(fptr);
        charnum ++;
        if (ch == '\n')
        {
            n++ ;   //number of lines that have been shown in 1 screen
            linenum ++;
        }
        if ( ch == EOF) break ;
        if ( row > 1 && n%(row-1) == 0) //checks if we are end of sceen or no
        {
        printf("---press a key to see more---");
         getchar();    //pause (wait for a key to continue )
         printf("\n");
        n = 1; //number of lines that have been shown in 1 screen should turn to one
        }
    }
        fclose(fptr);
        printf("\n===End of file===");
        if ( charnum > 0)
        printf("numbers of line(s): %d " , linenum);
        printf("number of charcter(s) is: %d" , charnum);
        return 0;
}
int fileexistence(char *filename)
{
    FILE * fptr = fopen(filename,"r");
    if ( fptr == NULL)
    {
    printf("the entered file doesn't exist.");
    return 0 ;
    }
    else return 1;
}
int Directoryexistence(const char *path)     //it returns 1 if folder exits and 0 if it doesnt exist. 
{
    struct stat stats;

    stat(path, &stats);

    // Check for file existence
    if (S_ISDIR(stats.st_mode))
        return 1;

    return 0;
}
void insert(char * filename,int line, int s_pos, char * to_add)
{
    char string[file_copy];
    char str[file_copy];
    char c,ch,ch2 ;
    FILE *fptr,*tfptr;
    int line_counter=0,cur_linenum=0;
    int ch_counter=-1;
    fptr = fopen(filename,"r");
    tfptr = fopen("d:/projecttest/temp.txt" , "w");
    while(c != EOF)      //counting how many lines the file has
    {
        c = getc(fptr);
        if(c=='\n')
        line_counter++;
        }
    fseek(fptr, 0, SEEK_SET);
    //fclose(fptr);
    //printf("The file has %d lines\n ", line_counter);
    if ( line > line_counter)
    {
    printf("the file doesnt include the entered line");
    return;
    }
    else{
        for (int i=0 ; i<line-1 ; i++)
        {
        fgets(string,line_size,fptr);
        //printf("%s",string);
        strcat(str,string);
        cur_linenum++;
    }
    //printf("%s",str);
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
    printf("\n");
    //printf("\n%s" , str);
    if(ch_counter == s_pos){
        for ( int i=0 ; (ch=fgetc(fptr))!= EOF ; i++)
            strncat(str,&ch,1);
    }
        //printf("\n\n%s",str);
        fprintf(tfptr,str);
        fclose(fptr);
        fclose(tfptr);
        /*if (remove(filename)==0)
          printf("The file is deleted successfully.");
        rename("d:/projecttest/temp.txt",filename);
        while((ch2 =fgetc(fptr)) != EOF )
        printf("%c",ch2);*/
}


void removeforward(char * filename,int line,int ch_spoint,int dlength)
{
    char new[5000];
    char c,str[50000];
    char to_delete[50000];
    char ch,ch2,ch3;
    int line_counter=0;
    int cur_linenum=0;
    int ch_counter=0;
    int ch_epoint= ch_spoint + dlength;
    FILE *fptr = fopen(filename,"r");
    FILE *tfptr =fopen("d:/projecttest/tmp.txt" , "w+");
    if(fptr == NULL){
    perror("Error opening file");  
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
    return;
  }
  else{
        for (int i=0; i<line-1 ; i++)
           {
             fgets(str, 200, fptr);
             strcat(new,str);
             //printf("%d ",cur_linenum);
             cur_linenum++;
           }
           //printf("%d",cur_linenum);
           //getchar();
         if(cur_linenum == line-1)
         {
          //printf("IAM HERE");
           while( ch_counter != ch_spoint)
             {      
               ch = fgetc(fptr);
               strncat(new, &ch, 1);
               ch_counter ++ ;
             }
               for(ch_counter=ch_spoint ;ch_counter<=ch_epoint;ch_counter++)
               {
                ch  = fgetc(fptr);
                //strncat(to_delete,&ch,1);
               } 
             }   
             //printf("%s",new);      
             printf("%d***\n",ch_counter);
            if (ch_counter == ch_epoint+1) 
            {
             for(ch_counter=ch_epoint ;(ch2 = fgetc(fptr)) != EOF;ch_counter++)
                strncat(new,&ch2,1);
            }
            }
           fprintf(tfptr,new);
           fclose(fptr);
           fclose(tfptr);
       
         /* if (remove(filename)==0)
        
        printf("The file is deleted successfully.");
        rename(filename,"d:/projecttest/tmp.txt");
      
        //fopen(filename,"r");
        while((ch2 =fgetc(fptr)) != EOF )
        printf("%c",ch2);
   */
  }

  void removebackward(char * filename , int line, int spoint , int length)  
{
    int pos1,pos2;
    char str[file_copy];
    char buff[line_size];
    char ch;
    int cur_linenum =0 , chcount=-0 , ch_copy = 0;
    int epoint = spoint - length ;
    pos2 = pos1 + spoint; 
    FILE *fptr = fopen(filename , "r");
    FILE *tfptr = fopen("d:/projecttest/tmp.txt" , "w");
    //for ( int i=0 ; i<line ; i++)
        //fgets(str,line_size,fptr);
        //while((fgets(buff,line_size,fptr)) != NULL)
        while((ch = fgetc(fptr))!=EOF)
        {
        chcount++;
        if (ch=='\n')
        cur_linenum ++;
        //printf("ch= %c",ch);
        if ( cur_linenum ==line-1)
        {
           // printf("%d",cur_linenum);
            //printf(" %d",pos1);
            //printf("chcount: %d",chcount);
            //getchar(); 
            break;
        }
        }
        fseek(fptr,0,SEEK_SET);
        while((ch = fgetc(fptr))!=EOF)
        {
            strncat(str,&ch,1);
            ch_copy ++;
            if ( ch_copy == chcount + epoint)
                break;
        }
        printf("%d\n",spoint + length);
       // printf("%s",str);
        fseek(fptr,spoint+length,SEEK_CUR);
        while((ch = fgetc(fptr))!=EOF)
        strncat(str,&ch,1);
        printf("%s",str);
        //printf("**%d\n",pos1);
        /*fseek(fptr,epoint,SEEK_CUR);
        pos1 =ftell(fptr);
        printf("***%d\n",pos1);
        fseek(fptr,0,SEEK_SET);
        while( ch_counter != pos1)
        {
        ch = fgetc(fptr);
        strncat(str,&ch,1);
        ch_counter ++;
        }
        printf("%s",str);
        fseek(fptr,length,SEEK_CUR);
        ch = fgetc(fptr);
        strncat(str,&ch,1);
        printf("%s",str);
     */
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
    FILE *tfptr =fopen("d:/projecttest/tmp.txt" , "w+");
    if(fptr == NULL){
    perror("Error opening file");  
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
          //printf("IAM HERE");
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
            printf("%s*",clipboard);
           /*fprintf(tfptr,new);
           fclose(fptr);
           fclose(tfptr);
        /*  if (remove(filename)==0)
        printf("The file is deleted successfully.");
        rename(filename,"d:/projecttest/tmp.txt");
        //fopen(filename,"r");
       // while((ch2 =fgetc(fptr)) != EOF )
        //printf("%c",ch2);*/
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
    FILE *tfptr = fopen("d:/projecttest/tmp.txt" , "w");
    //for ( int i=0 ; i<line ; i++)
        //fgets(str,line_size,fptr);
        //while((fgets(buff,line_size,fptr)) != NULL)
        while((ch = fgetc(fptr))!=EOF)
        {
        chcount++;
        if (ch=='\n')
        cur_linenum ++;
        //printf("ch= %c",ch);
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
        printf("%d",ch_copy);
        printf("\n%d\n",length+ ch_copy);
        while( ch_copy != a  )
        {
            //printf("a");
        ch = fgetc(fptr);
        strncat(clipboard,&ch,1);
        ch_copy++;
        }
        printf("%d",ch_copy);
        printf("%s",clipboard);
        //printf("%d", chcount + epoint);

        //printf("**%d\n",pos1);
        /*fseek(fptr,epoint,SEEK_CUR);
        pos1 =ftell(fptr);
        printf("***%d\n",pos1);
        fseek(fptr,0,SEEK_SET);
        while( ch_counter != pos1)
        {
        ch = fgetc(fptr);
        strncat(str,&ch,1);
        ch_counter ++;
        }
        printf("%s",str);
        fseek(fptr,length,SEEK_CUR);
        ch = fgetc(fptr);
        strncat(str,&ch,1);
        printf("%s",str);
     */
    }
    void cutforward(char * filename,int line,int ch_spoint,int dlength)
{
    char new[5000];
    char c,str[50000];
    char ch,ch2,ch3;
    int line_counter=0;
    int cur_linenum=0;
    int ch_counter=0;
    int ch_epoint= ch_spoint + dlength;
    FILE *fptr = fopen(filename,"r");
    FILE *tfptr =fopen("d:/projecttest/tmp.txt" , "w+");
    if(fptr == NULL){
    perror("Error opening file");  
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
    return;
  }
  else{
        for (int i=0; i<line-1 ; i++)
           {
             fgets(str, 200, fptr);
             strcat(new,str);
             //printf("%d ",cur_linenum);
             cur_linenum++;
           }
           //printf("%d",cur_linenum);
           //getchar();
         if(cur_linenum == line-1)
         {
          //printf("IAM HERE");
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
             //printf("%s",new);      
             printf("%d***\n",ch_counter);
            if (ch_counter == ch_epoint) 
            {
             for(ch_counter=ch_epoint ;(ch2 = fgetc(fptr)) != EOF;ch_counter++)
                strncat(new,&ch2,1);
            }
            }
            printf("%s*",clipboard);
          // printf("\n%s",new);
           /*fprintf(tfptr,new);
           fclose(fptr);
           fclose(tfptr);
        /*  if (remove(filename)==0)
        printf("The file is deleted successfully.");
        rename(filename,"d:/projecttest/tmp.txt");
        //fopen(filename,"r");
       // while((ch2 =fgetc(fptr)) != EOF )
        //printf("%c",ch2);*/
  }
  void cutbackward(char * filename, int line , int spoint,int length)
    {
    int pos1,pos2;
    char str[file_copy];
    char buff[line_size];
    char ch;
    int cur_linenum =0 , chcount=-0 , ch_copy = 0 ,a;
    int epoint = spoint - length ;
    pos2 = pos1 + spoint; 
    FILE *fptr = fopen(filename , "r");
    FILE *tfptr = fopen("d:/projecttest/tmp.txt" , "w");
    //for ( int i=0 ; i<line ; i++)
        //fgets(str,line_size,fptr);
        //while((fgets(buff,line_size,fptr)) != NULL)
        while((ch = fgetc(fptr))!=EOF)
        {
        chcount++;
        if (ch=='\n')
        cur_linenum ++;
        //printf("ch= %c",ch);
        if ( cur_linenum ==line-1)
        {
           // printf("%d",cur_linenum);
            //printf(" %d",pos1);
            //printf("chcount: %d",chcount);
            //getchar(); 
            break;
        }
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
        printf("%d",ch_copy);
        printf("\n%d\n",length+ ch_copy);
        while( ch_copy != a  )
        {
            //printf("a");
        ch = fgetc(fptr);
        strncat(clipboard,&ch,1);
        ch_copy++;
        }
        printf("%d",ch_copy);
        printf("%s",clipboard);
        //printf("%d", chcount + epoint);
        //printf("%d\n",spoint + length);
       // printf("%s",str);
        fseek(fptr,spoint+length,SEEK_CUR);
        while((ch = fgetc(fptr))!=EOF)
        strncat(str,&ch,1);
        printf("%s",str);
        //printf("**%d\n",pos1);
        /*fseek(fptr,epoint,SEEK_CUR);
        pos1 =ftell(fptr);
        printf("***%d\n",pos1);
        fseek(fptr,0,SEEK_SET);
        while( ch_counter != pos1)
        {
        ch = fgetc(fptr);
        strncat(str,&ch,1);
        ch_counter ++;
        }
        printf("%s",str);
        fseek(fptr,length,SEEK_CUR);
        ch = fgetc(fptr);
        strncat(str,&ch,1);
        printf("%s",str);
     */
    }
void paste(char * filename, int line , int spoint)
  {
    char string[file_copy];
    char str[file_copy];
    char c,ch,ch2 ;
    FILE *fptr,*tfptr;
    int line_counter=0,cur_linenum=0;
    int ch_counter=-1;
    fptr = fopen(filename,"r");
    tfptr = fopen("d:/projecttest/temp.txt" , "w");
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
    return;
    }
    else{
        for (int i=0 ; i<line-1 ; i++)
        {
        fgets(string,line_size,fptr);
        //printf("%s",string);
        strcat(str,string);
        cur_linenum++;
    }
    //printf("%s",str);
    if ( cur_linenum == line-1)
    {
        fseek(fptr,0,SEEK_CUR);
        while ( ch_counter != spoint)
        {
            ch = fgetc(fptr);
            strncat(str,&ch,1);
            ch_counter ++;
        }
        //printf("%s" , str);
    }
    }
    strcat(str,clipboard);
    printf("\n");
    //printf("\n%s" , str);
    if(ch_counter == spoint){
        for ( int i=0 ; (ch=fgetc(fptr))!= EOF ; i++)
            strncat(str,&ch,1);
    }
        printf("\n\n%s",str);
        //fprintf(tfptr,str);
        //fptr = fopen(filename,"r");
        
        fclose(fptr);
        fclose(tfptr);
       
      /*  if (remove(filename)==0)
         printf("The file is deleted successfully.");
        rename("d:/projecttest/temp.txt",filename);
        while((ch2 =fgetc(fptr)) != EOF )
        printf("%c",ch2);
   */ 
  } 
  void fcompare(char *file_a,char *file_b)
{
        char temp[100];
        char t1[100];
        char t2[100];
        int line;
        FILE *fp1=fopen(file_a,"r");
        FILE *fp2=fopen(file_b,"r");

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
}

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
int find(char *filename, const char *word ,int *line,int * col)
{
    FILE *fptr=fopen(filename,"r");
    if (fptr == NULL)
    printf("error opening the file");
    *line =-1;
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
            // First index of word in str is 
            // Memory address of pos - memory
            // address of str.
            *col = (pos - str);
            break;
            
            //printf("\nl=%d",l);
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
    return l + *col ;
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
    if(cmp==0)
    {
        printf(" %d",pos);
    }
    else if(flag==0)
    {
        printf("-1");
    }
     fclose(fp);
}

void autoind(char *file_add)
{

//FILE *curr = fopen("d:/root/tmp/$auto_int_tmp$", "w");
FILE *curr = fopen("d:/projecttest/$auto_int_tmp$", "w");
FILE *org = fopen(file_add, "r");


int block = 0;
char *ch1,ch;
//fgets(ch1, MAXSIZE, file);
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
strcat(curname, "FIXED");

rename(file_add,curname);
rename("d:/projecttest/$auto_int_tmp$", file_add);
}

int main()
    {
    //mkdir("d:/root");
    char command [200] ;
    int str_num,col,row,v,startfolder=1;
    //counting lines and colunms of the screen
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbi);
    col = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    row = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    char ch[]="d:/root";
    bool root=true;
    CreateDirectory (def_path,NULL);
    chdir(def_path);
    printf("Default working path: \"d:\\root\"\n");
    while(1)
    {
        printf("Enter command: ");
        gets(command);
        char**word=str_chop(command," ",&str_num);
        if(strcmp(word[0] , "createfile") ==0 && strcmp(word[1] , "--file") == 0)
        {
            if(*word[2] == '\"')
            {
                char**word = str_chop(command,"\"" ,&str_num);
                char**folders= str_chop(word[1],"/",&str_num);
                if((strcmp(folders[1],def_folder)) ==0)
                startfolder = 2 ;
                chdir(def_folder);
                for(int i=startfolder;i<str_num;i++)
                {
                    CreateDirectory (folders[i] , NULL);
                    chdir(folders[i]);
                }
                createfile(folders[str_num-1]);
            }
        
        if (*word[2] =='/')
        {
            char ** folders = str_chop(word[2],"/",&str_num);
            if((strcmp(folders[1],def_folder)) ==0)
            startfolder=2;
            chdir(def_path);
            for(int i=2;i<str_num;i++)
            {
                CreateDirectory(folders[i],NULL);
                chdir(folders[i]);
            }
        }
        }
    else if ((strcmp(word[0],"quit")) == 0)
    exit(0);
    else if ((strcmp(word[0],"cat")) ==0)
    cat("d:/projecttest/test1.txt",row);
    else
    printf("Invalid command.");
    }
    }
    
    
    