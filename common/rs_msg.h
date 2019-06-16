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

typedef struct RS_MSG {
    int color;
    char buffer[RS_KMSG_SIZE];
} RS_MSG;

#define RS_SIZE_MSG        (450)

enum rs_colors_t {
    COL_NONE = 10,
    COL_RED,
    COL_YELLOW,
    COL_GREEN,
    COL_HEX
};

int rs_msg_receive(int sock, RS_MSG *msg);

void rs_msg_send(int sock, int color, const char *msg);

int rs_msg_send_msg(int sock, RS_MSG *msg);

int rs_msg_to_string(char *buffer, RS_MSG *cmd);

int rs_msg_to_msg(RS_MSG *msg, const char *buffer);

int rs_msg_to_msg_advanced(RS_MSG *msg, const char *buffer, size_t len);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_P2S_MSG_H_
