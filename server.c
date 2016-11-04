#include<stdio.h>
#include<stdlib.h>
#include<string.h> //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h> //write
#include <time.h>
int main(int argc , char *argv[])
{
time_t mytime;
mytime=time(NULL);
int socket_desc , new_socket , c;
int answer[8];//猜數答案和使用者的回答
int count1,count2,countA,countB;//記數用變數
struct sockaddr_in server , client;
char *message;
char reply[8],transform[8],message2[50];
//Create socket
socket_desc = socket(AF_INET , SOCK_STREAM , 0);
if (socket_desc == -1)
{
printf("Could not create socket");
}
//Prepare the sockaddr_in structure
server.sin_family = AF_INET;
server.sin_addr.s_addr = INADDR_ANY;
server.sin_port = htons( 6072 );
//Bind
if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
{
puts("bind failed");
return 1;
}
puts("bind done");
//Listen
listen(socket_desc , 3);
//Accept and incoming connection
puts("Waiting for incoming connections...");
c = sizeof(struct sockaddr_in);
while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
{
puts("Connection accepted");
countA=0; countB=0;
srand( time(NULL) );
for(count1=0;count1<4;count1++)
{
answer[count1]=rand()%10; //亂數產生1~10任意數字並放入陣列內
for(count2=0;count2<count1;count2++)
{
if(answer[count1]==answer[count2]) //比較是否重複
{
count1--; //重複則重新產生
}
}
}
printf("Answer:%d%d%d%d\n",answer[0],answer[1],answer[2],answer[3]);
//Reply to the client
while(1)
{
bzero(reply,8);
read(new_socket,reply,8);
transform[0]=(reply[0]-'0');
transform[1]=(reply[1]-'0');
transform[2]=(reply[2]-'0');
transform[3]=(reply[3]-'0');
for(count1=0;count1<4;count1++)
{
if(answer[count1]==transform[count1]) //比較數字及位置相同的數
{
countA+=1;
}
}
for(count2=0;count2<=3;count2++)
{
for(count1=0;count1<=3;count1++)
{
if(transform[count1]==answer[count2] && count1!=count2)
{
countB++;
}
}
}
if(countA==4)
{
message="Game Over";
write(new_socket , message, strlen(message)+1);
return 1;
}
else
{
sprintf(message2,"%dA%dB",countA,countB);
write(new_socket , message2, strlen(message2)+1);
countA=0;countB=0;
}
}
}
if (new_socket<0)
{
perror("accept failed");
return 1;
}
return 0;
}
