#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>           
#include <fcntl.h>            
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>


#include <asm/types.h>        
#include <linux/videodev2.h>
#include <linux/v4l2-mediabus.h>


#define CAMERA_DEVICE "/dev/video0"
#define CAPTURE_FILE "frame.yuv"


int VIDEO_WIDTH = 1280;
int VIDEO_HEIGHT = 720;
#define VIDEO_FORMAT V4L2_MBUS_FMT_YUYV8_2X8
//int VIDEO_FORMAT = V4L2_MBUS_FMT_RGB888_1X24;
#define BUFFER_COUNT 10


typedef struct VideoBuffer {
    void   *start;
    size_t  length;
} VideoBuffer;


int main(int argc, char **argv)
{
    struct v4l2_fmtdesc fmt;
    struct v4l2_capability cap;
    struct v4l2_format stream_fmt;
    int ret;

    int fd;
    fd = open(CAMERA_DEVICE, O_RDWR, 0);
    if (fd < 0) {
        printf("Open %s failed\n", CAMERA_DEVICE);
        return -1;
    }
    //当前视频设备支持的视频格式
    memset(&fmt,0,sizeof(fmt));
    fmt.index = 0;
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    while((ret = ioctl(fd,VIDIOC_ENUM_FMT,&fmt)) == 0)
    {
        fmt.index ++ ;

        printf("{pixelformat = %c%c%c%c},description = '%s'\n",
                fmt.pixelformat & 0xff,(fmt.pixelformat >> 8)&0xff,
                (fmt.pixelformat >> 16) & 0xff,(fmt.pixelformat >> 24)&0xff,
                fmt.description);
    }
}

