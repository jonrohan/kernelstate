//AUTHOR: Jon Rohan
//TITLE: Linux Kernel State
//DATE: 3.22.2005
//PURPOSE: To inspect various aspects of the Linux Kernel

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void getCPUtime();
void getCPUtypeModel();
void getKernelVersion();
void timeSinceLastBoot();
void getNumberofContextSwitches();
void getNumberofProcessesCreated();
void getMemoryConfigured();
void getMemoryFree();
void convertTime(int rawdata);


//This is the main funtion of the program
//it takes in the parameters
//-s for short format or
//-l for long format
int main(int argc, char *argv[])
{
  //Initialize the value for the switch case
  int value;

  //If there are no arguments
  if(argv[1]==0)
  {
     value = 1;
  }

  //If there is the argument -s
  else if((strcmp("-s",argv[1]))==0)
  {
     value = 2;
  }

  //If there is the argument -l
  else if((strcmp("-l",argv[1]))==0)
  {
     value = 3;
  }

  //If there are too many or incorrect arguments
  else
  {
     value = 0;
  }

  switch(value)
  {
      
    //If there are no arguments
    case 1:
      printf("error: no arguments\n");
      printf("usage: kernelstate [-sl]\n");
      return 0;
      break;
    
    //If there is the argument -s for short
    case 2:

      //the three things the short command executes
      getCPUtypeModel();
      getKernelVersion();
      timeSinceLastBoot();
      printf("\n");

      return 0;
      break;

    //If there is the argument -l for long
    case 3:

      //This executes everything
      getCPUtypeModel();
      getKernelVersion();
      timeSinceLastBoot();
      getCPUtime();
      getNumberofContextSwitches();
      getNumberofProcessesCreated();
      getMemoryConfigured();
      getMemoryFree();
      printf("\n");

      return 0;
      break;
    
    //If there was an error in the arguments
    default:
      printf("Incorrect argument %s\n", argv[1]);
      printf("usage: kernelstate [-sl]\n");
      return 0;
      break;

  }//end switch

}//end main


//This function retrieves the CPU time
//for the user, the system, and idle time
//and displays the time. The time is in
//the format of jiffies (1/100 of a second),
//so the converTime function is used to display
//the correct format
void getCPUtime()
{
  //the declaration of the file
  FILE *infile;

  //the strings needed for the function
  char line[100];
  char *user_time[100], *system_time[100], *idle_time[100];
  
  //opening the file /proc/stat to get the info
  infile = fopen("/proc/stat","r");
  
  //this is some needed error checking to make sure
  //the file is there
  if(infile==NULL)
  {
    fprintf(stderr, "Cannot open /proc/stat file.");
    exit(0);
  }

  printf("\nCPU Time\n");
  printf("---------------------------------------\n");
  
  //this gets the first line of the file and stores it
  fgets(line,50,infile);
  
  //this removes the first token of the string
  strtok(line," \t\n");
  
  //retrieves the second token of the string
  //and puts it in the *user_time string
  *user_time = strtok(NULL," \t\n");

  printf("   User mode:   ");
  
  //calls the convertTime function to display the value
  //of the time
  convertTime(atoi(*user_time));

  //this removes the user low level time since it is
  //not needed
  strtok(NULL," \t\n");

  //retrieves the fourth token of the string
  //and puts it in the *system_time string
  *system_time = strtok(NULL," \t\n");

  printf("\n   System mode: ");

  //calls the convertTime function to display the value
  //of the time
  convertTime(atoi(*system_time));

  //retrieves the fifth token of the string
  //and puts it in the *idle_time string
  *idle_time = strtok(NULL," \t\n");
  
  printf("\n   Idle mode:   ");
  
  //calls the convertTime function to display the value
  //of the time
  convertTime(atoi(*idle_time)); 
  printf("\n");

  //closes the file stream
  fclose(infile);

}//end getCPUtime


//This Function retrieves the CPU type
//and the Model from the /proc/cpuinfo
//file
void getCPUtypeModel()
{
  //the declaration of the file
  FILE *infile;
  
  //the strings needed for the function
  char line[100];
  char *vendor_id[100], *model_name[100];
  
  //needed for the for loops
  int i;

  //opening the file /proc/cpuinfo to get the info
  infile = fopen("/proc/cpuinfo","r");
  
  //this is some needed error checking to make sure
  //the file is there
  if(infile==NULL)
  {
    fprintf(stderr, "Cannot open /proc/cpuinfo file.");
    exit(0);
  }
  
  printf("\nCPU Type and Model\n");
  printf("---------------------------------------\n");
  
  //This loop is used to get the second line of the file
  for(i=1;i<=2;i++)
    fgets(line,50,infile);
  
  //this removes the first two tokens since they are
  //not needed
  strtok(line," \t\n");
  strtok(NULL," \t\n");

  //retrieves the next token of the string
  //and puts it in the *vendor_id string
  *vendor_id=strtok(NULL," \t\n");

  printf("   Type: %s\n",*vendor_id);
  
  //This loop is used to get the third line of the file
  for(i=1;i<=3;i++)
    fgets(line,50,infile);

  //removes the first token
  strtok(line," \t\n");
  
  //This loop is used to get the second token of the file
  for(i=1;i<=2;i++)
    strtok(NULL," \t\n");
  
  //retrieves the next token of the string
  //and puts it in the *model_name string
  *model_name=strtok(NULL," \t\n");
  printf("   Model: %s\n",*model_name);

  //closes the file stream
  fclose(infile);

}//end getCPUtypeModel


//This function gets the the Kernel version
//from the file /proc/version
void getKernelVersion()
{
  //the declaration of the file
  FILE *infile;
  
  //the strings needed for the function
  char line[100];
  char *str1[100], *str2[100];
  
  //opening the file /proc/version to get the info
  infile = fopen("/proc/version","r");
  
  //this is some needed error checking to make sure
  //the file is there
  if(infile==NULL)
  {
    fprintf(stderr, "Cannot open /proc/version file.");
    exit(0);
  }

  printf("\nKernel Version\n");
  printf("---------------------------------------\n");
  
  //this gets the first line of the file and stores it
  fgets(line,50,infile);
  
  //removes the first token
  strtok(line," \t\n");

  printf("   %s",line);

  //retrieves the next token of the string
  //and puts it in the *str1 string
  *str1 = strtok(NULL," \t\n");
  printf(" %s",*str1);

  //retrieves the next token of the string
  //and puts it in the *str2 string
  *str2 = strtok(NULL," \t\n");
  printf(" %s\n",*str2);

  //closes the file stream
  fclose(infile);

}//end getKernelVersion


//This function gets the time since last boot
//from the file /proc/stat. It adds the four
//times from the first line then sends the
//value to the convertTime function
void timeSinceLastBoot()
{
  //the declaration of the file
  FILE *infile;
  
  //the strings needed for the function
  char line[100];
  char *time[100];
  
  //the sum of the four times is stored here
  int total_time, i;

  //opening the file /proc/stat to get the info
  infile = fopen("/proc/stat","r");
  
  //this is some needed error checking to make sure
  //the file is there
  if(infile==NULL)
  {
    fprintf(stderr, "Cannot open /proc/stat file.");
    exit(0);
  }

  printf("\nTime Since Last System Boot\n");
  printf("---------------------------------------\n");
  printf("   ");
  
  //this gets the first line of the file and stores it
  fgets(line,50,infile);
  
  //removes the first token
  strtok(line," \t\n");
  
  //This gets the next four tokens and stores them in their values
  for(i=1;i<=4;i++)
  {
    *time = strtok(NULL," \t\n");
    total_time += atoi(*time);
  }

  //This converts the time into the correct format
  convertTime(total_time);

  printf("\n");

  //closes the file stream
  fclose(infile);

}//end timeSinceLastBoot


//This function gets the number of context switches
//from the file /proc/stat.
void getNumberofContextSwitches()
{
  //the declaration of the file
  FILE *infile;
  
  //the strings needed for the function
  char line[100];
  char *context_switches[100];
  
  //for the for loop
  int i;
  
  //opening the file /proc/stat to get the info
  infile = fopen("/proc/stat","r");
  
  //this is some needed error checking to make sure
  //the file is there
  if(infile==NULL)
  {
    fprintf(stderr, "Cannot open /proc/stat file.");
    exit(0);
  }

  printf("\nNumber of Context Switches the Kernel Performed\n");
  printf("---------------------------------------\n");
    
  //this gets the eighth line of the file and stores it
  for(i=1;i<=8;i++)
    fgets(line,50,infile);
  
  //removes the first token
  strtok(line," \t\n");
  
  //retrieves the next token of the string
  //and puts it in the *context_switches string
  *context_switches = strtok(NULL," \t\n");
  printf("   %s\n",*context_switches);

  //closes the file stream
  fclose(infile);

}//end getNumberofContextSwitches


//This function gets the number processes created
//from the file /proc/stat.
void getNumberofProcessesCreated()
{
  //the declaration of the file
  FILE *infile;
  
  //the strings needed for the function
  char line[100];
  char *processes_created[100];
  
  //for the for loop
  int i;
  
  //opening the file /proc/stat to get the info
  infile = fopen("/proc/stat","r");
  
  //this is some needed error checking to make sure
  //the file is there
  if(infile==NULL)
  {
    fprintf(stderr, "Cannot open /proc/stat file.");
    exit(0);
  }

  printf("\nNumber of Processes Created\n");
  printf("---------------------------------------\n");
  
  //this gets the tenth line of the file and stores it
  for(i=1;i<=10;i++)
    fgets(line,50,infile);
  
  //removes the first token
  strtok(line," \t\n");
  
  //retrieves the next token of the string
  //and puts it in the *processes_created string
  *processes_created = strtok(NULL," \t\n");
  printf("   %s\n",*processes_created);
  
  //closes the file stream
  fclose(infile);

}//end getNumberofProcessesCreated


//This function gets the memory configured to the system
//from the file /proc/meminfo
void getMemoryConfigured()
{
  //the declaration of the file
  FILE *infile;
  
  //the strings needed for the function
  char line[100];
  char *memory_configured[100];
  
  //for the for loop
  int i;
  
  //opening the file /proc/meminfo to get the info
  infile = fopen("/proc/meminfo","r");
  
  //this is some needed error checking to make sure
  //the file is there
  if(infile==NULL)
  {
    fprintf(stderr, "Cannot open /proc/meminfo file.");
    exit(0);
  }
  
  printf("\nAmount of Memory Configured\n");
  printf("---------------------------------------\n");
  
  //this gets the sixth line of the file and stores it
  for(i=1;i<=6;i++)
    fgets(line,50,infile);
  
  //removes the first token
  strtok(line," \t\n");
  
  //retrieves the next token of the string
  //and puts it in the *memory_configured string
  *memory_configured = strtok(NULL," \t\n");
  printf("   %s kB\n",*memory_configured);

  //closes the file stream
  fclose(infile);

}//end getMemoryConfigured


//This function gets the memory free in the system
//from the file /proc/meminfo
void getMemoryFree()
{
  //the declaration of the file
  FILE *infile;
  
  //the strings needed for the function
  char line[100];
  char *memory_free[100];
  
  //for the for loop
  int i;

  //opening the file /proc/meminfo to get the info
  infile = fopen("/proc/meminfo","r");
  
  //this is some needed error checking to make sure
  //the file is there
  if(infile==NULL)
  {
    fprintf(stderr, "Cannot open /proc/meminfo file.");
    exit(0);
  }
  
  printf("\nAmount of Memory Currently Available\n");
  printf("---------------------------------------\n");
    
  //this gets the seventh line of the file and stores it
  for(i=1;i<=7;i++)
    fgets(line,50,infile);
  
  //removes the first token
  strtok(line," \t\n");
  
  //retrieves the next token of the string
  //and puts it in the *memory_free string
  *memory_free = strtok(NULL," \t\n");
  printf("   %s kB\n",*memory_free);

  //closes the file stream
  fclose(infile);

}//end getMemoryFree


//This function recives rawdata in the form of
//jiffies (1/100 of a second). It takes this
//data and transforms it into days, hours, minutes
//and seconds and prints it to the console
void convertTime(int rawdata)
{
  //the numbers used to store the info
  int days=0, hours=0, minutes=0, seconds=0;

  //converts the jiffies to seconds
  rawdata = rawdata / 100;
  seconds = rawdata;

  //checks if seconds are greater than 60
  //to set the minutes
  if(seconds >= 60)
  {
      minutes = rawdata / 60;
      seconds = rawdata % 60;
  }

  //checks if minutes are greater than 60
  //to set the hours
  if(minutes >=60)
  {
      hours = rawdata / 3600;
      minutes = minutes % 60;
  }

  //checks if hours are greater than 24
  //to set the days
  if(hours >= 24)
  {
      days = hours / 24;
      hours = hours % 24;
  }

  //These statements are used to make the
  //result more asthetically pleaseing
  //so a printout of 3:2:1:6 would look like
  //03:02:01:06
  if(days < 10)
      printf("0%d:",days);
  else
      printf("%d:",days);
  if(hours < 10)
      printf("0%d:",hours);
  else
      printf("%d:",hours);
  if(minutes < 10)
      printf("0%d:",minutes);
  else
      printf("%d:",minutes);
  if(seconds < 10)
      printf("0%d",seconds);
  else
      printf("%d",seconds);

}//end convertTime
