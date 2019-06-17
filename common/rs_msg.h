/*
	PSP2SHELL
	Copyright (C) 2016, Cpasjuste

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _P2S_MSG_H_
#define _P2S_MSG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#define RS_KMSG_SIZE       400
#define RS_ERR_SOCKET      0x80000001
#define RS_ERR_INVALID_MSG 0x80000002

typedef struct {
    int color;
    char buffer[RS_KMSG_SIZE];
} rs_msg_t;

#define RS_SIZE_MSG        (450)

int rs_msg_receive(int sock, rs_msg_t *msg);

void rs_msg_send(int sock, int color, const char *msg);

int rs_msg_send_msg(int sock, rs_msg_t *msg);

int rs_msg_to_string(char *buffer, rs_msg_t *cmd);

int rs_msg_to_msg(rs_msg_t *msg, const char *buffer);

int rs_msg_to_msg_advanced(rs_msg_t *msg, const char *buffer, size_t len);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_P2S_MSG_H_
