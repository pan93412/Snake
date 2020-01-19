﻿#pragma once 

#include <tgmath.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

// there is a slight difference between Windows and Linux naming on function library
#define Sleep(msec) usleep(msec * 1000);
#define _getch() getc(stdin)

// thanks to https://cboard.cprogramming.com/c-programming/63166-kbhit-linux.html
int _kbhit(void) {
	struct termios oldt, newt;
	int ch;
	int oldf;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if (ch != EOF) {
		ungetc(ch, stdin);
		return 1;
	}

	return 0;
}
