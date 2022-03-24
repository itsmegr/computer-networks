#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>
#include <poll.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/select.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/un.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <netinet/tcp.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <bits/stdc++.h>
#define MAXLINE 4096
using namespace std;
#define ADDRESS "USFD"
#define ADDRESS2 "USFD2"
#define MSG_CMSG_CLOEXEC 0x40000000
int recv_fd(int usfd)
{
	int sent_fd, available_ancillary_element_buffer_space;   struct msghdr socket_message;
	struct iovec io_vector[1];
	struct cmsghdr* control_message = NULL;
	char message_buffer[1];
	char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];

	/* start clean */
	memset(&socket_message, 0, sizeof(struct msghdr));
	memset(ancillary_element_buffer, 0, CMSG_SPACE(sizeof(int)));

	/* setup a place to fill in message contents */
	io_vector[0].iov_base = message_buffer;
	io_vector[0].iov_len = 1;
	socket_message.msg_iov = io_vector;
	socket_message.msg_iovlen = 1;

	/* provide space for the ancillary data */
	socket_message.msg_control = ancillary_element_buffer;   socket_message.msg_controllen = CMSG_SPACE(sizeof(int));
	if (recvmsg(usfd, &socket_message, MSG_CMSG_CLOEXEC) < 0)   return -1;
	if (message_buffer[0] != 'F')
	{
		/* this did not originate from the above function */   return -1;
	}
	if ((socket_message.msg_flags & MSG_CTRUNC) == MSG_CTRUNC) {
		/* we did not provide enough space for the ancillary element array */   return -1;
	}

	/* iterate ancillary elements */
	for (control_message = CMSG_FIRSTHDR(&socket_message);   control_message != NULL;
		control_message = CMSG_NXTHDR(&socket_message, control_message)) {
		if ((control_message->cmsg_level == SOL_SOCKET) &&
			(control_message->cmsg_type == SCM_RIGHTS))
		{
			sent_fd = *((int*)CMSG_DATA(control_message));
			return sent_fd;
		}
	}
	return -1;
}


int send_fd(int usfd, int fd)
{
	struct msghdr socket_message;
	struct iovec io_vector[1];
	struct cmsghdr* control_message = NULL;
	char message_buffer[1];

	/* storage space needed for an ancillary element with a paylod of  length is CMSG_SPACE(sizeof(length)) */
	char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];
	int available_ancillary_element_buffer_space;

	/* at least one vector of one byte must be sent */
	message_buffer[0] = 'F';
	io_vector[0].iov_base = message_buffer;
	io_vector[0].iov_len = 1;

	/* initialize socket message */
	memset(&socket_message, 0, sizeof(struct msghdr));
	socket_message.msg_iov = io_vector;
	socket_message.msg_iovlen = 1;

	/* provide space for the ancillary data */
	available_ancillary_element_buffer_space = CMSG_SPACE(sizeof(int));
	memset(ancillary_element_buffer, 0,
		available_ancillary_element_buffer_space);
	socket_message.msg_control = ancillary_element_buffer;   socket_message.msg_controllen =
		available_ancillary_element_buffer_space;

	/* initialize a single ancillary data element for fd passing */   control_message = CMSG_FIRSTHDR(&socket_message);
	control_message->cmsg_level = SOL_SOCKET;
	control_message->cmsg_type = SCM_RIGHTS;
	control_message->cmsg_len = CMSG_LEN(sizeof(int));
	*((int*)CMSG_DATA(control_message)) = fd;

	return sendmsg(usfd, &socket_message, 0);
}
void printPayload(struct ip* iph, unsigned char* tempBuff, int packetSize)
{
	cout << "ESS is getting serve by: ";
	unsigned short ip_head_len = iph->ip_hl * 4;
	struct tcphdr* tcp_head = (struct tcphdr*)(tempBuff + ip_head_len);
	unsigned char* buf = tempBuff + ip_head_len + tcp_head->th_off * 4;
	size_t length = (packetSize - tcp_head->th_off * 4 - iph->ip_hl * 4);

	for (size_t i = 0; i < length; i++)
	{
		if (i != 0 && i % 16 == 0)
		{
			for (size_t j = (i - 16); j < i; j++)
			{
				if (buf[j] >= 32 && buf[j] <= 128)
				{
				}
			}
			printf("\n");
		}

		cout << buf[i];

		if (i == (length - 1))
		{
			printf("\n");
		}
	}
}

void* readingThread(void* vargp)
{
	int sfd;

	if ((sfd = socket(PF_INET, SOCK_RAW, 0)) < 0)
	{
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}




	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(3106);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(sfd, (const struct sockaddr*)&addr,
		sizeof(addr));


	struct sockaddr_in clientAddr;
	int len = sizeof(clientAddr);
	memset(&clientAddr, 0, sizeof(clientAddr));

	while (true)
	{
		unsigned char buffer[MAXLINE];
		// this will not only rec message but also the client add
		int packetSize = recvfrom(sfd, (char*)buffer, MAXLINE,
			0, (sockaddr*)&clientAddr,
			(socklen_t*)&len);
		if (packetSize <= 0)
		{
			continue;
		}
		unsigned char tempBuff[MAXLINE];
		for (int i = 0; i < MAXLINE; i++)
			tempBuff[i] = buffer[i];

		struct ip* iph = (struct ip*)buffer;
		printPayload(iph, tempBuff, packetSize);

		memset(buffer, 0, MAXLINE);
	}
}

int main()
{

	pthread_t tid;
	pthread_create(&tid, NULL, readingThread, (void*)&tid);
	int usfd;
	struct sockaddr_un userv_addr, ucli_addr;
	socklen_t userv_len, ucli_len;
	usfd = socket(AF_UNIX, SOCK_STREAM, 0);

	if (usfd == -1)
	{
		perror("\nsocket ");
	}

	bzero(&userv_addr, sizeof(userv_addr));

	userv_addr.sun_family = AF_UNIX;
	strcpy(userv_addr.sun_path, ADDRESS);
	userv_len = sizeof(userv_addr);


	while (true) {
		if (connect(usfd, (struct sockaddr*)&userv_addr, userv_len) != -1)
		{
			break;
		}
	}
	printf("\nconnect successful\n");



	int sfd;
	while (true) {
		sfd = recv_fd(usfd);
		if (sfd != -1)
		{
			break;
		}
	}
	printf("sfd receved\n");






	char buff[1024];
	bzero(buff, sizeof(buff));
	strcpy(buff, "I am C2.");
	send(sfd, &buff, sizeof(buff), 0);
	bzero(buff, sizeof(buff));
	recv(sfd, &buff, sizeof(buff), 0);
	cout << "C2 recvd: " << buff << "\n";






	// Passing sfd
	int usfd2;
	// struct sockaddr_un userv_addr,ucli_addr;  
	// socklen_t userv_len,ucli_len;  

	usfd2 = socket(AF_UNIX, SOCK_STREAM, 0);

	if (usfd2 < 0)
	{
		perror("socket");
	}

	bzero(&userv_addr, sizeof(userv_addr));

	userv_addr.sun_family = AF_UNIX;
	strcpy(userv_addr.sun_path, ADDRESS2);
	unlink(ADDRESS2);
	userv_len = sizeof(userv_addr);

	if (bind(usfd2, (struct sockaddr*)&userv_addr, userv_len) == -1)
	{
		perror("server: bind");
	}
	listen(usfd2, 1);
	bzero(&ucli_addr, sizeof(ucli_addr));

	ucli_len = sizeof(ucli_addr);

	int nusfd;


	//  while((nusfd=accept(usfd, (struct sockaddr *)&ucli_addr, &ucli_len))<=0);
	nusfd = accept(usfd2, (struct sockaddr*)&ucli_addr, &ucli_len);

	int x = send_fd(nusfd, sfd);
	if (x == 1) {
		cout << "Sfd passed to C3.\n";
	}

	//  close(fd);
	pthread_join(tid, NULL);

	return 0;
}