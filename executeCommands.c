// Shell.

#include "types.h"
#include "user.h"
#include "fcntl.h"

// -------------------------------------------------------------------------------------------------------------------------------
// a command struct store pointers to arguments ,input files opened,output files opened ,there modes and some check variables
// -------------------------------------------------------------------------------------------------------------------------------

struct command{
  char* ptr[10];
  char* inputFile;
  char* outputFile;
  int inputFilemode;
  int outputFilemode;
  int fd;
  int inputRedirExist;
  int outputRedirExist;
};

// -------------------------------------------------------------------------------------------------------------------------------
// this structure check if actual command provided as parameter contains some subcommand ex ls && wc cheeck if giveb command contains &&
// -------------------------------------------------------------------------------------------------------------------------------

struct struct_command{
    
    int exist; // to check if | && || ; exist
    int type;
    int commandLength;
    char *ptrforcommand;
    char  *leftPart;
    char *rightPart;
    
};
// -------------------------------------------------------------------------------------------------------------------------------
// to avoid redundancy used spaces array 
// -------------------------------------------------------------------------------------------------------------------------------

char spaces[] = " \t\r\n\v";


// -------------------------------------------------------------------------------------------------------------------------------
// it should only support the given command if you want to add some commands just manipulate given array
// -------------------------------------------------------------------------------------------------------------------------------

char *commandToSupport[8] = {"ls","cat","grep","wc","echo","ps","procinfo","exit"};

// -------------------------------------------------------------------------------------------------------------------------------
// utilities to find string length
// -------------------------------------------------------------------------------------------------------------------------------

int
strlength(char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

// -------------------------------------------------------------------------------------------------------------------------------
// get start and end of command if command has some leading and trailing spaces it avoid them and it only stores coorect location in start and end vaiables
// -------------------------------------------------------------------------------------------------------------------------------

void getStartandEnd(char* cmd , int *start , int *end){

  int i = 0 , j = strlength(cmd) ;

    for(i = 0;i < j;i++){
      
      if(!strchr(spaces , cmd[i])){
        *start = i;

        for(j = i; !strchr(spaces , cmd[j]) ;j++);

        *end = j;

        break;
      }


    }

    return;
    
}
// -------------------------------------------------------------------------------------------------------------------------------
// find position of character in command
// -------------------------------------------------------------------------------------------------------------------------------

int getPosOfChar(char* cmd , char *c){
    int i =  0;
    while(i < strlength(cmd)){
      if(cmd[i] == *c){
        return i;
      }
      i++;
    }
    return -1;
}

// -------------------------------------------------------------------------------------------------------------------------------
// this function checks if entered command is valid or not by using commands given in character array  
// -------------------------------------------------------------------------------------------------------------------------------


int isValidCommand(char* ptr){

  int startOfCommand = 0 , endOfCommand = strlength(ptr);

  getStartandEnd(ptr , &startOfCommand , &endOfCommand);

  // printf(1,"start of command = %d end Of Command %d\n",startOfCommand , endOfCommand);

  for(int i = 0;i < 8;i++){
  
    int isValid = 1;
    char *validCmd = commandToSupport[i];
    int lenOfVaidCmd = strlength(commandToSupport[i]);
    
    for(int j = 0;j < lenOfVaidCmd ; j++){
        if(ptr[startOfCommand + j] != validCmd[j]){ 
          isValid = 0;
        }
    }

    if(isValid) return 1;

  }

  return 0;
  
}


// -------------------------------------------------------------------------------------------------------------------------------
// copies string (dest , source , bytes to copy)
// -------------------------------------------------------------------------------------------------------------------------------

int
strncopy(char *s, char *t, int n)
{
 
  while(n-- > 0 && (*s++ = *t++) != 0)
    ;
  while(n-- > 0)
    *s++ = '\0';
  return 0;
}


// -------------------------------------------------------------------------------------------------------------------------------

// this should be global command as we need to check if in [&&, || , ; ,| ] do not add illegal commmand thus this funtion will be 
// called by all checking functions 

// -------------------------------------------------------------------------------------------------------------------------------

struct struct_command* 
contains(struct struct_command *parameter , char* ptrforcheckpattern){
    int i = 0,exist = 0 , index = 0;//0 denotes false
    int command_len = parameter->commandLength , string_len = strlength(ptrforcheckpattern) ;
    char *command = parameter->ptrforcommand , *ptr = ptrforcheckpattern;

    for(i = 0;i < command_len - string_len;i++){
         exist = 0;
         for(int j = 0;j < string_len;j++){
             if(ptr[j] == command[i+j]){

                 exist += 1;
       
             }else 
             break;

         }
         if(exist == string_len){
             index = i;
             break;
         }
    }

    if(index == 0){
    
        parameter->exist = 0;
        return (struct struct_command*)parameter;

    }

    parameter->exist = 1;

    char *command1 = malloc(sizeof(char)*(index+1));
    memset(command1,0,sizeof(command1));//this will reset all mem bytes which will save our time to set manually
    strncopy(command1,parameter->ptrforcommand,index);
    
    parameter->leftPart = command1;


    for(i = index + string_len ; i < command_len;i++){
        if(command[i] == ' ')continue;
        else{
            index = i;
            break;
        }
    }

    parameter->rightPart = malloc( (command_len - index + 1)*sizeof(char));
    memset(parameter->rightPart,0,sizeof(parameter->rightPart));
    
    strncopy(parameter->rightPart,command+index,command_len - index + 1);
    return (struct struct_command*)parameter;
}

// -------------------------------------------------------------------------------------------------------------------------------
// it returs file pointer allocates mem and sets up environent for files
// -------------------------------------------------------------------------------------------------------------------------------


char* 
getFilePointer(int pos  , char * cmd , int *endl){

  int start = pos , end = pos;
  for(start = pos ; start < strlength(cmd) ; start++){
    // of character is not spaces
    if( !strchr(spaces , cmd[start])){
      // printf(1," char = %c \n",cmd[start]);
        for(end = start; !strchr(spaces , cmd[end]) ;end++ );
        break;
    }

  }
  *endl = end;
  
  char* filePtr = malloc(sizeof(char)*(end - start + 1));
  memset(filePtr,0,sizeof(filePtr));
  strncopy(filePtr , cmd + start  , end-start);
  
  return filePtr; 

}

// -------------------------------------------------------------------------------------------------------------------------------
// it splits command and store them in pointer structure (character array of pointers)
// -------------------------------------------------------------------------------------------------------------------------------

void split(struct command* pointer , char* s){


int len = strlen(s) , index = 0 ;

  for(int i = 0;i < len;i++){
    
    if(strchr(spaces , s[i]))continue;
    else if(s[i] == '<'){
        int end = 0;
        pointer->inputRedirExist = 1;
        pointer->inputFilemode = O_RDONLY;
        pointer->fd = 0;
        pointer->inputFile = getFilePointer(i + 1 , s , &end);
        i = end-1;
    }
    else if(s[i] == '>'){
        int end = 0;
        pointer->outputRedirExist = 1;
        pointer->outputFilemode = O_WRONLY | O_CREATE;
        pointer->fd = 1;
        pointer->outputFile = getFilePointer(i + 1,s , &end);
        i = end-1;
    }
    else{

      int j = 0;
      for(j = i + 1;j < len ; j++){
        if(!strchr(spaces , s[j]))continue;
        else break;
      }
      
      char *command = malloc(sizeof(char)*(j-i+1));
      memset(command,0,sizeof(command));
      strncopy(command,s+i,j-i);
      
      pointer->ptr[index++] = command;
      i = j;
      
    }
    
  }
  
}

// -------------------------------------------------------------------------------------------------------------------------------
// get command from user
// -------------------------------------------------------------------------------------------------------------------------------

int
getInput(char *buf, int nbuf)
{
  printf(2, "myshell> ");
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  if(buf[0] == 0) // EOF
    return -1;
  return 0;
}

// -------------------------------------------------------------------------------------------------------------------------------
// opening of various files by command
// -------------------------------------------------------------------------------------------------------------------------------


void openFiles(struct command *cmd){

    if(cmd->inputRedirExist){
            close(0);
            if(open(cmd->inputFile, cmd->inputFilemode) < 0){
            printf(2, "open %s failed\n", cmd->inputFile);
            exit(-1);
            }
          }
          if(cmd->outputRedirExist){
            close(1);
            if(open(cmd->outputFile, cmd->outputFilemode) < 0){
            printf(2, "open %s failed\n", cmd->outputFile);
            exit(-1);
            }
          }
}


// -------------------------------------------------------------------------------------------------------------------------------
// execute command provided in buf character pointer
// -------------------------------------------------------------------------------------------------------------------------------


void executeCommand(char* buf){

    for(int i = 0;i < strlength(buf) - 4 ; i++){
        if(buf[i] == 'e' && buf[i+1] == 'x' && buf[i+2] == 'i' && buf[i+3] == 't'){
          exit(0);
        }
    }
    int status;
    if(fork() == 0 ){
      int fileDesc[2];
      struct struct_command *token ;

      token = malloc(sizeof(*token));

      token->ptrforcommand = buf;
      
      token->commandLength = strlen(buf);
      

      if(contains(token , "&&")->exist == 1){
     
          int status = -1, pidchild = 0;
          if(isValidCommand(token->leftPart)){
              pidchild = fork();
              if(pidchild==0){
                struct command* cmd;
                cmd = malloc(sizeof(*cmd));
                memset(cmd,0,sizeof(cmd));
                split(cmd , token->leftPart);

                openFiles(cmd);
                exec(cmd->ptr[0],cmd->ptr);

              }
              else{
                wait(&status);
     
                if(status == 0){
                struct command* cmd;
                cmd = malloc(sizeof(*cmd));
                memset(cmd,0,sizeof(cmd));
                split(cmd , token->rightPart);

                openFiles(cmd);
                exec(cmd->ptr[0],cmd->ptr);

                  }
              }

          }else{
            printf(1,"Illegal command or arguments\n");
          }
          
    

      }else if(contains(token , "||")->exist == 1){

          int status = -1, pidchild = 0;
          
              pidchild = fork();
              if(pidchild==0 && isValidCommand(token->leftPart)){
                struct command* cmd;
                cmd = malloc(sizeof(*cmd));
                memset(cmd,0,sizeof(cmd));
                split(cmd , token->leftPart);

                openFiles(cmd);
                exec(cmd->ptr[0],cmd->ptr);

              }
              


          
          else{
                  // printf(1," status before = %d\n",status);
                  wait(&status);
                  // printf(1,"pidchild = %d pid by wait = %d status after = %d\n",pidchild , pid ,status);

                  if(status != 0){
                    if(isValidCommand(token->rightPart)){
                        struct command* cmd;
                        cmd = malloc(sizeof(*cmd));
                        memset(cmd,0,sizeof(cmd));
                        split(cmd , token->rightPart);

                        openFiles(cmd);
                        exec(cmd->ptr[0],cmd->ptr);
                    }
                    else{
                      printf(1,"Illegal command or arguments\n");

                    }
                    

                  }

              }
          


        
         

    }else if(contains(token , "|")->exist == 1){

        
        if(pipe(fileDesc) < 0)
          exit(-1);

        if(fork() == 0){
        close(1);
        dup(fileDesc[1]);
        close(fileDesc[0]);
        close(fileDesc[1]);
        struct command* cmd;
        cmd = malloc(sizeof(*cmd));
        memset(cmd,0,sizeof(cmd));
        
        split(cmd , token->leftPart);
        openFiles(cmd);
        // printf(1,"left part executed ");
        exec(cmd->ptr[0] , cmd->ptr);
        // exec(argvl[0],argvl);
        }
        if(fork() == 0){
        close(0);
        dup(fileDesc[0]);
        close(fileDesc[0]);
        close(fileDesc[1]);
        struct command* cmd;
        cmd = malloc(sizeof(*cmd));
        memset(cmd,0,sizeof(cmd));
        split(cmd , token->rightPart);
         openFiles(cmd);
        // printf(1,"right part executed ");
        exec(cmd->ptr[0] , cmd->ptr);
        // exec(argvr[0],argvr);
        }
        close(fileDesc[0]);
        close(fileDesc[1]);
        wait(0);
        wait(0);
        // exit(0);
        

    }else if(contains(token , ";")->exist == 1){
          // int status  = 0;
          int pidProcess1 = fork();
          if(pidProcess1 == 0){
            if(isValidCommand(token->leftPart)){
                struct command* cmd;
                cmd = malloc(sizeof(*cmd));
                memset(cmd,0,sizeof(cmd));
                split(cmd , token->leftPart);
                openFiles(cmd);
                // printf(1,"left part executed ");
                exec(cmd->ptr[0] , cmd->ptr);
            }else{
            printf(1,"Illegal command or arguments\n");

            }
            
            // printf(1,"\n");
            // printf(1,"\n");

          }
          
            int pidProcess2 = fork();
            if(pidProcess2 == 0){
              if(isValidCommand(token->rightPart)){
                  struct command* cmd;
                  cmd = malloc(sizeof(*cmd));
                  memset(cmd,0,sizeof(cmd));
                  split(cmd , token->rightPart);
                  openFiles(cmd);
                  exec(cmd->ptr[0] , cmd->ptr);
              }else{
            printf(1,"Illegal command or arguments\n");

              }
            
          
          }
          
          // wait(&status);
          


        

    }else{
        
          if(isValidCommand(token->ptrforcommand)){
              struct command* cmd;
              cmd = malloc(sizeof(*cmd));
              memset(cmd,0,sizeof(cmd));
              split(cmd , buf);
              // print(cmd);

            openFiles(cmd);
              // close(1);
              exec(cmd->ptr[0] , cmd->ptr);
          }else{
            printf(1,"Illegal command or arguments\n");
          }
          
          // exit(0);

        }//else ends

        exit(0);
     
    }
    wait(&status);
    
  
}

char data[1024];

int main(int argc, char const *argv[])
{
    if(argc != 2){
        printf(1,"Illegal command arguments\n");
        exit(-1);
    }else{
        close(0);
        int fd = open(argv[1] , O_RDONLY);
        int numberOfBytesRead, maxBytesChecked;
        char *p_currentptr, *q_lastptr;

        maxBytesChecked = 0;
        while((numberOfBytesRead = read(fd, data+maxBytesChecked, sizeof(data)-maxBytesChecked-1)) > 0){
            maxBytesChecked += numberOfBytesRead;
            data[maxBytesChecked] = '\0';
            p_currentptr = data;
            while((q_lastptr = strchr(p_currentptr, '\n')) != 0){
            *q_lastptr = 0;
            // printf(1,"commands %s ",p);
            
            executeCommand(p_currentptr);

            p_currentptr = q_lastptr+1;
            }
            
            // printf(1,"commands %s m value = %d \n",p,m);

            
            if(p_currentptr == data)
            maxBytesChecked = 0;
            if(maxBytesChecked > 0){
            executeCommand(p_currentptr);
            maxBytesChecked -= p_currentptr - data;
            memmove(data, p_currentptr, maxBytesChecked);
            }


        }

    }
    
    // printf(1,"%d\n",argc);
    exit(0);
}

// 
// executeCommands README