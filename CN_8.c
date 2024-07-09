/*
        UDP_Server. This Program will will create the Server side for UDP_Socket Programming.
        It will receive the data from the client and then send the same data back to client.
*/

#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr

int main(void)
{
        int socket_number = 5006;
      
        int socket_desc;
        struct sockaddr_in server_addr, client_addr;
        char server_message[2000], client_message[2000];
              
        int client_struct_length = sizeof(client_addr);
        
        //Cleaning the Buffers
        
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));
        
        //Creating Socket
        
        socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        
        if(socket_desc < 0)
        {
                printf("Could Not Create Socket. Error!!!!!\n");
                return -1;
        }
        
        printf("Socket Created\n");
        
        //Binding IP and Port to socket
        
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(socket_number);
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        
        if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0)
        {
                printf("Bind Failed. Error!!!!!\n");
                return -1;
        }        
        
        printf("Bind Done\n");
        
        //----------------------------------------------------------//
        //read from file and initialize the array
        
		FILE* fptr = fopen("Attendance.txt", "r");
		
		int current_size = 0;
        
		fscanf(fptr, "%d", &current_size);

		//--read empty space if any--//
		//char buff[2];	
		//fscanf(fptr, "%s", buff);        

		//using pointers to efficiently manage memory        
        char *strs[20];		
		char temp_roll[15];
		
		int l = 0;
		
		for(; l<current_size; l++)
		{
			fscanf(fptr, "%s", temp_roll);        		
			strs[l] = malloc(strlen(temp_roll+1));	
			strcpy(strs[l], temp_roll);
		}
		
		fclose(fptr);
       
                
        while (1)
        {
        	int write_to_file = 0;
        	
		    printf("Listening for Messages...\n\n");
		    
		    //Receive the message from the client
		    
		    if (recvfrom(socket_desc, client_message, sizeof(client_message), 0, (struct sockaddr*)	&client_addr,&client_struct_length) < 0)
		    {
		            printf("Receive Failed. Error!!!!!\n");
		            return -1;
		    }
	  
		    printf("Received Message from IP: %s and Port No: %i\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
		
			// get roll number
			char rollnum[15];
			strcpy(rollnum, client_message);
			rollnum[7] = '\0';

			int i = 0;
			int found = 0;
			int f_index = 0;
		
			for (i = 0; i < current_size; i++)
			{
				if(strcmp(strs[i], rollnum) == 0)
				{
					found = 1;
					f_index = i;
					break;
				}
			}		

			// Check In Scenario
			if (client_message[8] == 'C' && client_message[9] == 'I')
			{			
				// if it doesnt exist then allocate memory to it								
				if (found == 0)
				{
					strs[current_size] = malloc(strlen(rollnum) + 1);
					strcpy(strs[current_size], rollnum);
					current_size++;
					
					strcpy(server_message, "Welcome Student ");
					strcat(server_message, rollnum);
					strcat(server_message, " !\n");
					write_to_file = 1;
				}
				else
				{
					strcpy(server_message, "You are already here.\n");
				}		
			}	
		
			// Check Out Scenario
			else if (client_message[8] == 'C' && client_message[9] == 'O')
			{
				// remove that student from array and move all students one place backwards
				if (found == 1)
				{
					int j = f_index;
				
					//moving all students one place back
					for(; j < current_size - 1;j++)
					{
						strcpy(strs[j], strs[j+1]);
					}
	
					// mark one place free now
					free (strs[current_size - 1]);
					
					current_size--;
					
												
					strcpy(server_message, "Goodbye Student ");
					strcat(server_message, rollnum);
					strcat(server_message, " ! Have a nice day!\n");
					write_to_file = 1;
				}
				else
				{
					strcpy(server_message, "You didn't check in today. Contact System Administrator.\n");
				}
		
			}	
		
			else
				printf("Wrong Input Message\n");
			
			printf("Current Size is %d . ", current_size);
			
			if (current_size > 0)
			{			
				//print all members in the array
				printf("Following students are present:\n");

				int c = 0;
				for(; c < current_size; c++)
				{
					printf(strs[c]);
					printf("\n");
				}
			}
			else
				printf("No student is present.\n");
		
		    //Send the message back to client
		    
		    if (sendto(socket_desc, server_message, strlen(server_message), 0, (struct sockaddr*)&client_addr,client_struct_length)<0)
		    {
		            printf("Send Failed. Error!!!!!\n");
		            return -1;
		    }
		    
		    memset(server_message,'\0',sizeof(server_message));
		    memset(client_message,'\0',sizeof(client_message));
		    
		    // write to file only when the array is modified
		    if (write_to_file == 1)
		    {
		 		//in case the server shuts down for some reason, write the array back to file for persistence of database
		
				fptr = fopen("Attendance.txt", "w");
		
				// write current size of array to file
				char a_size[5];
				snprintf (a_size, sizeof(a_size), "%d",current_size);
				fputs (a_size, fptr);
				fputs (" ", fptr);
		
				int k = 0;
		
				for(; k < current_size; k++)
				{
					fputs(strs[k], fptr);
					fputs (" ", fptr);
				}
		
				fclose(fptr);
			}
		}              
		
		
        //Closing the Socket        
        close(socket_desc);
        return 0;       
}

