#ifndef __CHAT_SHM_H__
#define __CHAT_SHM_H__

typedef struct chatInfo {

	char	userID[20];
	long	messageTime;
	char	message[40];

} CHAT_INFO;

#endif //__CHAT_SHM_H__
