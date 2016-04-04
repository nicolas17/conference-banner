/*
 * Copyright (c) 2016 Nicolás Alvarez
 *
 * Licensed under the General Public License version 3,
 * or (at your option) any later version. See the LICENSE
 * file for details.
 */
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/kd.h>

#include <iostream>
#include <string>

#ifndef KDSKBMUTE
#define KDSKBMUTE 0x4B51
#endif

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <0|1>" << std::endl;
        return 1;
    }
    int enable = std::stoi(argv[1]);

    int fd = ::open("/dev/tty0", O_RDWR);
    int oldMode=-1;
    if (fd != -1) {
        if (enable) {
            std::cout << "Disabling console keyboard" << std::endl;
            ::ioctl(fd, KDGKBMODE, &oldMode);
            ::ioctl(fd, KDSKBMUTE, enable);
            ::ioctl(fd, KDSKBMODE, K_OFF);
            std::cout << "old mode is " << oldMode << std::endl;
        } else {
            std::cout << "Enabling console keyboard" << std::endl;
            ::ioctl(fd, KDSKBMUTE, 0);
            ::ioctl(fd, KDSKBMODE, K_UNICODE);
        }
        ::close(fd);
    }
}
