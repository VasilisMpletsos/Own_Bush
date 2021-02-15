#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>



 void main(int argc,char *argv[])
 {

	int childpid;
	int status;
	int f=0;
	char* line;
	line=(char *)malloc(512*sizeof(char));
	char* keep;
	keep=(char *)malloc(512*sizeof(char));
	char* command;
	command=(char *)malloc(512*sizeof(char));
	char* argument[3];
	argument[2]=NULL;
	char *individualCommands[10];
	char* restof;
	int error=0;
	int i=0; //to choose commands
	//This is the interactive mode
	
	if(argc>1)
	{
		printf("Batch file mode!!!!!\n");
		FILE *fp;
		if((fp = fopen(argv[1], "r")) == NULL){
            exit(1);
        }
		 while(1){
			if(fgets(line,512,fp))
				{
					f++;
					printf("%d Line is = %s \n",f,line);
					if(strchr(line ,';')!=NULL) 
					{
						strcpy(keep,line);
						command=strtok(line,";");
						command=strtok(NULL,"\0");
						restof=command;
						int count=1;
						int flag=1;
						while(flag==1)
							{
								flag=0;
								if(strchr(restof,';')!=NULL)
									{	
										count=count+1;
										flag=1;
										command=strtok(restof,";");
										command=strtok(NULL,"\0");
										restof=command;
										
									}			
							}
							command=strtok(keep,";");
							char* tempMeddling;
							tempMeddling=(char *)malloc((strlen(command))*sizeof(char));
							stpcpy(tempMeddling,command);
							tempMeddling[strlen(command)-1]='\0';
							individualCommands[i]=(char *)malloc(strlen(tempMeddling)*sizeof(char));
							strcpy(individualCommands[i],tempMeddling);
							
							if(count>1)
								{
									for(int c=0;c<count-1;c++)
									{
										i++;
										command=strtok(NULL,";");
										tempMeddling=(char *)realloc(tempMeddling,(strlen(command))*sizeof(char));
										stpcpy(tempMeddling,command);
										for(int k=0;k<strlen(command)-2;k++) tempMeddling[k]=tempMeddling[k+1];
										tempMeddling[strlen(command)-2]='\0';
										individualCommands[i]=(char *)malloc(strlen(tempMeddling)*sizeof(char));
										strcpy(individualCommands[i],tempMeddling);
									}
								}
								i++;
								command=strtok(NULL,"\0");
								tempMeddling=(char *)realloc(tempMeddling,(strlen(command))*sizeof(char));
								stpcpy(tempMeddling,command);
								for(int k=0;k<strlen(command)-1;k++) tempMeddling[k]=tempMeddling[k+1];
								tempMeddling[strlen(command)-3]='\0';
								individualCommands[i]=(char *)malloc(strlen(tempMeddling)*sizeof(char));
								strcpy(individualCommands[i],tempMeddling);
							
								for(int j=0;j<i+1;j++)
									{
										printf("%s\n",individualCommands[j]);
									}
											
								for(int f=0;f<i+1;f++)
								{
									if(strchr(individualCommands[f],'-')==NULL)
										{
											command=strtok(individualCommands[f],"\0");
											argument[0]=command;
											argument[1]=NULL;
										}else
										{
											command=strtok(individualCommands[f]," ");
											argument[0]=command;
											argument[1]=strtok(NULL,"\0");
										}

									if((childpid = fork()) == -1)
										{
												perror("fork");
												exit(1);
										}

										if(childpid == 0)
											{
												if(execvp(command,argument)==-1)
													{
														printf("Wrong Command \n");
													}
											}
										else
											{
												if (waitpid(childpid, &status, 0) == childpid) 
													{
															
													} else {
														printf("Child Failed to execute");
												}

											}
									
								}	
							
								
					}else if(strchr(line ,'&')!=NULL) 
						{
							strcpy(keep,line);
							command=strtok(line,"&");
							command=strtok(NULL,"\0");
							restof=command;
							int count=1;
							int flag=1;
							while(flag==1)
								{
									flag=0;
									if(strchr(restof,'&')!=NULL)
										{
											int andnum=0;
											for(int j=0;j<(strlen(restof));j++){
												if(restof[j]=='&') andnum++;
											}
											if(andnum==1) break;
											
											count=count+1;
											flag=1;
											command=strtok(restof,"&");
											command=strtok(NULL,"\0");
											strcpy(restof,command);
										}			
								}
							
							command=strtok(keep,"&");
							char* tempMeddling;
							tempMeddling=(char *)malloc((strlen(command))*sizeof(char));
							stpcpy(tempMeddling,command);
							tempMeddling[strlen(command)-1]='\0';
							individualCommands[i]=(char *)malloc(strlen(tempMeddling)*sizeof(char));
							strcpy(individualCommands[i],tempMeddling);
							
							if(count>1){
								for(int c=0;c<count-1;c++)
								{
									i++;
									command=strtok(NULL,"&");
									tempMeddling=(char *)realloc(tempMeddling,(strlen(command))*sizeof(char));
									stpcpy(tempMeddling,command);
									for(int k=0;k<strlen(command)-2;k++) tempMeddling[k]=tempMeddling[k+1];
									tempMeddling[strlen(command)-2]='\0';
									individualCommands[i]=(char *)malloc(strlen(tempMeddling)*sizeof(char));
									strcpy(individualCommands[i],tempMeddling);
								}
							}	
							i++;
							command=strtok(NULL,"\n");
							tempMeddling=(char *)realloc(tempMeddling,(strlen(command))*sizeof(char));
							stpcpy(tempMeddling,command);
							for(int k=0;k<strlen(command)-2;k++) tempMeddling[k]=tempMeddling[k+2];
							tempMeddling[strlen(command)-3]='\0';
							individualCommands[i]=(char *)malloc(strlen(tempMeddling)*sizeof(char));
							strcpy(individualCommands[i],tempMeddling);
							
							
							
							for(int j=0;j<i+1;j++)
							{
								printf("%s\n",individualCommands[j]);
							}
										
							for(int f=0;f<i+1;f++)
							{
								if(strchr(individualCommands[f],'-')==NULL)
								{
									command=strtok(individualCommands[f],"\0");
									argument[0]=command;
									argument[1]=NULL;
								}else
								{
									command=strtok(individualCommands[f]," ");
									argument[0]=command;
									argument[1]=strtok(NULL,"\n");
								}
								
								if((childpid = fork()) == -1)
									{
											perror("fork");
											exit(1);
									}

									if(childpid == 0)
										{
												if(status==1024)
													{
														printf("Going to interactive mode in order to type the command wright");
														goto Loop;
													}
													if(execvp(command,argument)==-1)
													{
														printf("Wrong Command \n Going to start type again the commands \n");
														exit(4);
													}else{
														exit(0);
													}
											
										}
									else
										{
													
													
													if (waitpid(childpid, &status, 0) == childpid) 
														{
															//Do nothing
														} else {
															printf("Child Failed to execute");
														}
											
												

										}
							}
					
				}
				else 
				{
								
						if(strchr(line,'-')==NULL)
						{
							command=strtok(line,"\0");
							char* TrueCommand;
							TrueCommand=(char *)malloc((strlen(command)-1)*sizeof(char));
							strncpy(TrueCommand, command, strlen(command)-2);
							command=TrueCommand;
							argument[0]=TrueCommand;
							argument[1]=NULL;
							
						}else
						{
							command=strtok(line," ");
							argument[0]=command;
							argument[1]=strtok(NULL," ");
							argument[1][strlen(argument[1]-3)]='\0';
							
						}
						if((childpid = fork()) == -1)
								{
										perror("fork");
										exit(1);
								}
						if(childpid == 0)
									{
										if(execvp(command,argument)==-1)
											{
												printf("Wrong Command \n");
											}
									}
								else
									{
										if (waitpid(childpid, &status, 0) == childpid) 
											{
													
											} else {
												printf("Child Failed to execute");
										}

									}
					
				}
            if(line[0]=='q' && line[1]=='u' && line[2]=='i' && line[3]=='t' && line[4]=='\0')
				{
					exit(0);                 	 					       			                         				  
				}
		
            
			}
		}
	}else{
	
	
	Loop: while(1)
	{
		printf("VMpletsos_8687>");
		fflush(NULL);
		fgets(line, 512, stdin);
		if(strchr(line ,';')!=NULL) 
		{
			strcpy(keep,line);
			command=strtok(line,";");
			command=strtok(NULL,"\n");
			restof=command;
			int count=1;
			int flag=1;
			while(flag==1)
				{
					flag=0;
					if(strchr(restof,';')!=NULL)
						{	
							count=count+1;
							flag=1;
							command=strtok(restof,";");
							command=strtok(NULL,"\n");
							restof=command;
						}			
				}
			
			command=strtok(keep,";");
			char* tempMeddling;
			tempMeddling=(char *)malloc((strlen(command))*sizeof(char));
			stpcpy(tempMeddling,command);
			tempMeddling[strlen(command)-1]='\0';
			individualCommands[i]=(char *)malloc(strlen(tempMeddling)*sizeof(char));
			strcpy(individualCommands[i],tempMeddling);
			
			if(count>1)
				{
					for(int c=0;c<count-1;c++)
					{
						i++;
						command=strtok(NULL,";");
						tempMeddling=(char *)realloc(tempMeddling,(strlen(command))*sizeof(char));
						stpcpy(tempMeddling,command);
						for(int k=0;k<strlen(command)-2;k++) tempMeddling[k]=tempMeddling[k+1];
						tempMeddling[strlen(command)-2]='\0';
						individualCommands[i]=(char *)malloc(strlen(tempMeddling)*sizeof(char));
						strcpy(individualCommands[i],tempMeddling);
					}
				}	
			i++;
			command=strtok(NULL,"\n");
			tempMeddling=(char *)realloc(tempMeddling,(strlen(command))*sizeof(char));
			stpcpy(tempMeddling,command);
			for(int k=0;k<strlen(command)-1;k++) tempMeddling[k]=tempMeddling[k+1];
			tempMeddling[strlen(command)-1]='\0';
			individualCommands[i]=(char *)malloc(strlen(tempMeddling)*sizeof(char));
			strcpy(individualCommands[i],tempMeddling);
			
			
			for(int j=0;j<i+1;j++)
				{
					printf("%s\n",individualCommands[j]);
				}
						
			for(int f=0;f<i+1;f++)
			{
				if(strchr(individualCommands[f],'-')==NULL)
					{
						command=strtok(individualCommands[f],"\n");
						argument[0]=command;
						argument[1]=NULL;
					}else
					{
						command=strtok(individualCommands[f]," ");
						argument[0]=command;
						argument[1]=strtok(NULL,"\n");
					}

				if((childpid = fork()) == -1)
					{
							perror("fork");
							exit(1);
					}

					if(childpid == 0)
						{
							if(execvp(command,argument)==-1)
								{
									printf("Wrong Command \n");
								}
						}
					else
						{
							if (waitpid(childpid, &status, 0) == childpid) 
								{
										
								} else {
									printf("Child Failed to execute");
							}

						}
				
			}	
		}else if(strchr(line ,'&')!=NULL) 
		{
			strcpy(keep,line);
			command=strtok(line,"&");
			command=strtok(NULL,"\n");
			restof=command;
			int count=1;
			int flag=1;
			while(flag==1)
				{
					flag=0;
					if(strchr(restof,'&')!=NULL)
						{
							int andnum=0;
							for(int j=0;j<(strlen(restof));j++){
								if(restof[j]=='&') andnum++;
							}
							if(andnum==1) break;
							
							count=count+1;
							flag=1;
							command=strtok(restof,"&");
							command=strtok(NULL,"\n");
							strcpy(restof,command);
						}			
				}
			
			command=strtok(keep,"&");
			char* tempMeddling;
			tempMeddling=(char *)malloc((strlen(command))*sizeof(char));
			stpcpy(tempMeddling,command);
			tempMeddling[strlen(command)-1]='\0';
			individualCommands[i]=(char *)malloc(strlen(tempMeddling)*sizeof(char));
			strcpy(individualCommands[i],tempMeddling);
			
			if(count>1){
				for(int c=0;c<count-1;c++)
				{
					i++;
					command=strtok(NULL,"&");
					tempMeddling=(char *)realloc(tempMeddling,(strlen(command))*sizeof(char));
					stpcpy(tempMeddling,command);
					for(int k=0;k<strlen(command)-2;k++) tempMeddling[k]=tempMeddling[k+1];
					tempMeddling[strlen(command)-2]='\0';
					individualCommands[i]=(char *)malloc(strlen(tempMeddling)*sizeof(char));
					strcpy(individualCommands[i],tempMeddling);
				}
			}	
			i++;
			command=strtok(NULL,"\n");
			tempMeddling=(char *)realloc(tempMeddling,(strlen(command))*sizeof(char));
			stpcpy(tempMeddling,command);
			for(int k=0;k<strlen(command)-2;k++) tempMeddling[k]=tempMeddling[k+2];
			tempMeddling[strlen(command)-2]='\0';
			individualCommands[i]=(char *)malloc(strlen(tempMeddling)*sizeof(char));
			strcpy(individualCommands[i],tempMeddling);
			
			
			for(int j=0;j<i+1;j++)
			{
				printf("%s\n",individualCommands[j]);
			}
						
			for(int f=0;f<i+1;f++)
			{
				if(strchr(individualCommands[f],'-')==NULL)
				{
					command=strtok(individualCommands[f],"\n");
					argument[0]=command;
					argument[1]=NULL;
				}else
				{
					command=strtok(individualCommands[f]," ");
					argument[0]=command;
					argument[1]=strtok(NULL,"\n");
				}
				
				if((childpid = fork()) == -1)
					{
							perror("fork");
							exit(1);
					}

					if(childpid == 0)
						{
								if(status==1024)
									{
										goto Loop;
									}
									if(execvp(command,argument)==-1)
									{
										printf("Wrong Command \n Going to start type again the commands \n");
										exit(4);
									}else{
										exit(0);
									}
							
						}
					else
						{
									
									
									if (waitpid(childpid, &status, 0) == childpid) 
										{
											//Do nothing
										} else {
											printf("Child Failed to execute");
										}
							
								

						}
			}
		}else //There is no ; or &&
		{
			if(strchr(line,'-')==NULL)
			{
				command=strtok(line,"\n");
				argument[0]=command;
				argument[1]=NULL;
				
			}else
			{
				command=strtok(line," ");
				argument[0]=command;
				argument[1]=strtok(NULL,"\n");
				
			}
			if((childpid = fork()) == -1)
					{
							perror("fork");
							exit(1);
					}
			if(childpid == 0)
						{
							if(execvp(command,argument)==-1)
								{
									printf("Wrong Command \n");
								}
						}
					else
						{
							if (waitpid(childpid, &status, 0) == childpid) 
								{
										
								} else {
									printf("Child Failed to execute");
							}

						}
		}
	}
   }
}