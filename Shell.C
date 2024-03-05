#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LENGTH 1000

void execute_shell_bultin(char ** str,int i,int k);
int evaluate_expression(char *str);
void shell();
char *read_input();
char **parse_input(char *str, int k);
int getNumberofWords(char *str);
void execute_command(char ** str,int k,int i);

 void execute_shell_bultin(char ** str,int i,int k)
 {
      if(!strcmp(str[0],"echo"))
      {
           printf("%s\n",str[i]);
      }else if(!strcmp(str[0],"cd"))
      {
        int x=chdir(str[1]);
        if(x==-1)
        {
          perror("Error");
        }
        
      }else if(!strcmp(str[0],"export"))
      {
               
      }

      
      if(i<k)
      { if((!strcmp(str[0],"cd")||!strcmp(str[0],"export")))
      {
        if(i+1<k)
         perror("ERROR");
        return;
      }
          i++;
        execute_shell_bultin(str,i,k);
      }
 }
 int evaluate_expression(char *str)
 {
       if(!strcmp(str,"echo")||!strcmp(str,"cd")||!strcmp(str,"export"))
      {
           return 1;         
      }
      return 2;
 }

    
    void shell(){
      char cwd[1024];
      getcwd(cwd, sizeof(cwd));
        printf("$<%s>: ",cwd);
        
        char* str=read_input();

        int len =getNumberofWords(str);

        char** strings = parse_input(str,len);

     while (1)
     {
        

        if(!strcmp(strings[0],"exit"))exit(1);
        
        switch(evaluate_expression(strings[0])){
            case 1:
                execute_shell_bultin(strings,1,len);
                break;
            case 2:
                execute_command(strings,len,1);
        }
        
         free(strings);
         getcwd(cwd, sizeof(cwd));

         printf("\n$<%s>: ",cwd);char* str=read_input();
        int len =getNumberofWords(str);
        char ** strings = parse_input(str,len);

     }
    
    }

 

 
 char * read_input()
 {

    char* str = (char*)malloc((MAX_INPUT_LENGTH) * sizeof(char));
    fgets(str, MAX_INPUT_LENGTH ,stdin);

    return str;
 }
 char** parse_input(char *str,int k)
{

     k++;
    char** strings = (char**)malloc(k * sizeof(char*));
    for (int i = 0; i < k; i++) {
        strings[i] = (char*)malloc((500) * sizeof(char));
    }

    int i=0;
    int j=0;
    int len=0;
    int b=0;
    int qoutes=0;
      while(str[i]!='\0')
      {
         if(!qoutes){
         if(b)
         {
           if(str[i]==' '||str[i]==10) i++;
           else
          {
            b=0;
            j=0;
            len++;
            if(str[i]==34)
            {
              qoutes=1;
              i++;
            }
          }
         }
          else
         {
            if(str[i]==34)
            {
              qoutes=1;
              i++;
            }
            else if(str[i]!=' '&&str[i]!=10)
            {

               strings[len][j]=str[i];
               j++;
               i++;
            }
            else
            {
                b=1;
                if(str[0]==' ' && i==0)len=-1;
                else
                    {
                        strings[len][j]='\0';
                    }
                i++;
                j=0;
            }
         }}
         else
            {
              strings[len][j]=str[i];
               j++;
               i++;

               if(str[i]==34){qoutes=0;i++;}
               b=1;
            }
      }

      return strings;
}
int getNumberofWords(char * str)
{  // cd "SS"
    int i=0;
    int len=0;
    int b=1;
    int qoutes=0;
      while(str[i]!='\0')
      {

      if(!qoutes){
         if(b)
         {
           if(str[i]==' '||str[i]==10) i++;
           else {
            b=0;

            len++;

            if(str[i]==34)
            {
              qoutes=1;

            }
             i++;
           }
         }else
         {
             if(str[i]==34)qoutes=1;
            else if(str[i]!=' '&&str[i]!=10)
            {
               i++;
            }else{b=1;i++;}
         }}else
         {

               i++;
               if(str[i]==34){qoutes=0;i++;}
               b=1;
         }

      }


      return len;
}

     void execute_command(char ** str,int k,int i)
     { 
      
      
            pid_t pid = fork();
            
           if(pid==-1)
           {
            perror("error");
           } 
           else if (pid==0){

              
                  char  filename[500]="/bin/";
                  strcat(filename, str[0]); 
                  char * arg[]={filename,str[i],NULL};
                  int x=execve(arg[0],arg,NULL);
                  if(x==-1)
                  {
                      perror("unknown command\n");
                      exit(1);
                  }
                
            }else
            {
                wait(NULL);
                
                printf("SUCCSSED");

                if(i+1<=k)
                {
                  i++;
                  printf("\n");
                  execute_command(str,k,i);
                }
            }
            
    
     }

int main()
{
    shell();
}


