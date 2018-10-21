#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#define SHMSIZE 10 /* 共享内存大小， 10 字节 */
#define SHMNAME "shmtest" /* 共享内存名称 */
int main()
{
int fd;
char *ptr;
fd = shm_open(SHMNAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR); /*创建共享内存 */
if (fd < 0) {
perror("shm_open error");
exit(-1);
}
ptr = mmap(NULL, SHMSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);/*映射共享内存*/
if (ptr == MAP_FAILED) {
perror("mmap error");
exit(-1);
}
ftruncate(fd, SHMSIZE); /* 设置共享内存大小 */
while (*ptr != 18) { /* 读起始地址，判断值是否为 18 */
sleep(1); /* 不是 18，继续读取 */
}
printf("ptr : %d\n", *ptr); /* 数据是 18，打印显示 */
munmap(ptr, SHMSIZE); /* 取消内存映射 */
shm_unlink(SHMNAME); /* 删除共享内存 */
return 0;
}
