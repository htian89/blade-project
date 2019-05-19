#include <fstream>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/file.h>

int lock_reg(int, int, int, off_t, int, off_t);
#define read_lock(fd, offset, whence, len) \
  lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))
#define readw_lock(fd, offset, whence, len) \
  lock_reg((fd), F_SETLKW, F_RDLCK, (offset), (whence), (len))
#define write_lock(fd, offset, whence, len) \
  lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))
#define writew_lock(fd, offset, whence, len) \
  lock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence), (len))
#define writeown_lock(fd, offset, whence, len) \
  lock_reg((fd), F_SETOWN_EX, F_WRLCK, (offset), (whence), (len))
#define un_lock(fd, offset, whence, len) \
  lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))

pid_t lock_test(int, int, off_t, int, off_t);

#define is_read_lockable(fd, offset, whence, len) \
  (lock_test((fd), F_RDLCK, (offset), (whence), (len)) == 0)
#define is_write_lockable(fd, offset, whence, len) \
  (lock_test((fd), F_WRLCK, (offset), (whence), (len)) == 0)

int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len) {
  struct flock lock;

  lock.l_type = type;     /* F_RDLCK, F_WRLCK, F_UNLCK */
  lock.l_start = offset;  /* byte offset, relative to l_whence */
  lock.l_whence = whence; /* SEEK_SET, SEEK_CUR, SEEK_END */
  lock.l_len = len;       /* #bytes (0 means to EOF) */

  return (fcntl(fd, cmd, &lock));
}

/* Note: lock_test always success with the obtained lock process */
pid_t lock_test(int fd, int type, off_t offset, int whence, off_t len) {
  struct flock lock;

  lock.l_type = type;     /* F_RDLCK or F_WRLCK */
  lock.l_start = offset;  /* byte offset, relative to l_whence */
  lock.l_whence = whence; /* SEEK_SET, SEEK_CUR, SEEK_END */
  lock.l_len = len;       /* #bytes (0 means to EOF) */

  if (fcntl(fd, F_GETLK, &lock) < 0) perror("fcntl error");

  /* printf("F_RDLCK=%d, F_WRLCK=%d, F_UNLCK=%d\n", F_RDLCK, F_WRLCK, F_UNLCK);
   */
  printf("    l_type=%d, l_start=%lu, l_where=%d, l_len=%lu, l_pid=%d\n",
         lock.l_type, lock.l_start, lock.l_whence, lock.l_len, lock.l_type);

  if (lock.l_type == F_UNLCK)
    return (0);        /* false, region isn't locked by another proc */
  return (lock.l_pid); /* true, return pid of lock owner */
}

/* ========================================================  */

void Usage(const char* program) {
  fprintf(stderr, "Usage: %s read/read2/write/rd-wr/wr-rd/fork/dup file time\n",
          program);
  exit(-1);
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    Usage(argv[0]);
  }
  int interval = 10;
  if (argc > 3) {
    interval = atoi(argv[3]);
  }
  printf("PID=%d\n", getpid());

  char* path = argv[2];
  if (strcmp(argv[1], "read") == 0) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
      perror("open");
      exit(0);
    }
    printf("read lock try\n");
    /*  read lock entire file */
    if (read_lock(fd, 0, SEEK_SET, 0) < 0) {
      perror("read_lock: ");
      printf("Write Lock by pid=%d\n", lock_test(fd, F_WRLCK, 0, SEEK_SET, 0));
      exit(0);
    }
    printf("read lock success.\n");
    printf("Read Lock by pid=%d\n", lock_test(fd, F_WRLCK, 0, SEEK_SET, 0));

    sleep(interval);

    exit(0);
  } else if (strcmp(argv[1], "read2") == 0) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
      perror("open");
      exit(0);
    }
    printf("read lock try\n");
    /*  read lock entire file */
    if (read_lock(fd, 0, SEEK_SET, 0) < 0) {
      perror("read_lock: ");
      printf("Write Lock by pid=%d\n", lock_test(fd, F_WRLCK, 0, SEEK_SET, 0));
      exit(0);
    }
    printf("read lock success.\n");
    printf("Read Lock by pid=%d\n", lock_test(fd, F_WRLCK, 0, SEEK_SET, 0));

    if (read_lock(fd, 0, SEEK_SET, 1) < 0) {
      perror("read_lock: ");
      printf("Write Lock by pid=%d\n", lock_test(fd, F_WRLCK, 0, SEEK_SET, 0));
      exit(0);
    }
    printf("read lock 2 success.\n");
    sleep(interval);

    exit(0);
  } else if (strcmp(argv[1], "write") == 0) {
    int fd = open(path, O_WRONLY);
    printf("file: %s, fd=%d\n", path, fd);
    if (fd == -1) {
      perror("open");
      exit(0);
    }
    printf("write lock try\n");
    /*  write lock entire file */
    if (writeown_lock(fd, 0, SEEK_SET, 0) < 0) {
      perror("write_lock: ");
      printf("Write Lock by pid=%d\n", lock_test(fd, F_WRLCK, 0, SEEK_SET, 0));
      printf("Read Lock by pid=%d\n", lock_test(fd, F_RDLCK, 0, SEEK_SET, 0));
      exit(0);
    }
    printf("write lock success.\n");
    printf("Write Lock by pid=%d\n", lock_test(fd, F_WRLCK, 0, SEEK_SET, 0));

    // int ret = write(fd, path, strlen(path) );
    // if(ret != strlen(path)){
    //    perror("write");
    //}
    std::ofstream writer(path, std::ios_base::app);
    // std::ofstream writer(path, std::ios_base::out);
    writer << "write!\n";
    sleep(interval);

    writer.close();
    close(fd);
    exit(0);
  } else if (strcmp(argv[1], "fcntl") == 0) {
    int fd = open(path, O_WRONLY);
    printf("file: %s, fd=%d\n", path, fd);
    if (fd == -1) {
      perror("open");
      exit(0);
    }
    printf("write lock try\n");
    /*  write lock entire file */
    int code = fcntl(fd, F_SETOWN_EX, F_WRLCK);
    printf("Code: %d\n", code);
    if (code < 0) {
      perror("write_lock: ");
      printf("Write Lock by pid=%d\n", lock_test(fd, F_WRLCK, 0, SEEK_SET, 0));
      printf("Read Lock by pid=%d\n", lock_test(fd, F_RDLCK, 0, SEEK_SET, 0));
      exit(0);
    }
    printf("write lock success.\n");
    printf("Write Lock by pid=%d\n", lock_test(fd, F_WRLCK, 0, SEEK_SET, 0));

    std::ofstream writer(path, std::ios_base::app);
    // std::ofstream writer(path, std::ios_base::out);
    writer << "write!\n";
    sleep(interval);

    writer.close();
    close(fd);
    exit(0);

  } else if (strcmp(argv[1], "flock") == 0) {
    int fd = open(path, O_WRONLY);
    printf("file: %s, fd=%d\n", path, fd);
    if (fd == -1) {
      perror("open");
      exit(0);
    }
    printf("write lock try\n");
    /*  write lock entire file */
    if (flock(fd, LOCK_EX | LOCK_NB) != 0) {
      perror("lock by other proc: ");
      exit(0);
    }
    printf("write lock success.\n");

    // int ret = write(fd, path, strlen(path) );
    // if(ret != strlen(path)){
    //    perror("write");
    //}
    std::ofstream writer(path, std::ios_base::app);
    // std::ofstream writer(path, std::ios_base::out);
    writer << "write!\n";
    writer.close();

    sleep(interval);
    flock(fd, LOCK_UN);
    close(fd);
    exit(0);
    int done = flock(fd, LOCK_EX | LOCK_NB);
  } else if (strcmp(argv[1], "ofstream") == 0) {
    std::ofstream writer(path, std::ios_base::app);
    writer << "ofstream!\n";
    sleep(interval);
    writer.close();
    exit(0);
  } else if (strcmp(argv[1], "rd-wr") == 0) {
    int fd = open(path, O_RDWR); /* O_RDWR  */
    if (fd == -1) {
      perror("open");
      exit(0);
    }
    /*  read lock entire file */
    if (read_lock(fd, 0, SEEK_SET, 0) < 0) {
      perror("read_lock: ");
      printf("Write Lock by pid=%d\n", lock_test(fd, F_WRLCK, 0, SEEK_SET, 0));
      exit(0);
    }
    printf("read lock success.\n");
    printf("Lock by pid=%d\n", lock_test(fd, F_WRLCK, 0, SEEK_SET, 0));

    /* if( un_lock(fd, 0, SEEK_SET, 0) < 0){
        perror("un-lock:");
    }                                        */

    if (write_lock(fd, 0, SEEK_SET, 1) < 0) {
      perror("write_lock: ");
      printf("Write Lock by pid=%d\n", lock_test(fd, F_WRLCK, 0, SEEK_SET, 0));
      exit(0);
    }
    printf("write lock success.\n");
    printf("Lock by pid=%d\n", lock_test(fd, F_WRLCK, 0, SEEK_SET, 0));
    sleep(interval);

    exit(0);

  } else if (strcmp(argv[1], "wr-rd") == 0) {
    int fd = open(path, O_RDWR); /* O_RDWR */
    if (fd == -1) {
      perror("open");
      exit(0);
    }
    /*  write lock entire file */
    if (write_lock(fd, 0, SEEK_SET, 0) < 0) {
      perror("write_lock: ");
      printf("Write Lock by pid=%d\n", lock_test(fd, F_WRLCK, 0, SEEK_SET, 0));
      exit(0);
    }
    printf("write lock success.\n");
    printf("Lock by pid=%d\n", lock_test(fd, F_WRLCK, 0, SEEK_SET, 0));

    /* if( un_lock(fd, 0, SEEK_SET, 0) < 0){
        perror("un-lock:");
    }                                        */

    if (read_lock(fd, 0, SEEK_SET, 0) < 0) {
      perror("read_lock: ");
      printf("Write Lock by pid=%d\n", lock_test(fd, F_WRLCK, 0, SEEK_SET, 0));
      exit(0);
    }
    printf("read lock success.\n");
    printf("Lock by pid=%d\n", lock_test(fd, F_WRLCK, 0, SEEK_SET, 0));

    sleep(interval);

    exit(0);
  } else if (strcmp(argv[1], "fork") == 0) {
    int fd = open(path, O_RDWR); /* O_RDWR */
    if (fd == -1) {
      perror("open");
      exit(0);
    }
    /*  write lock entire file */
    if (write_lock(fd, 0, SEEK_SET, 0) < 0) {
      perror("write_lock: ");
      printf("Write Lock by pid=%d\n", lock_test(fd, F_WRLCK, 0, SEEK_SET, 0));
      exit(0);
    }
    printf("write lock success.\n");
    printf("Lock by pid=%d\n", lock_test(fd, F_WRLCK, 0, SEEK_SET, 0));

    pid_t pid;
    if ((pid = fork()) < 0) {
      perror("un-lock:");
      exit(1);
    } else if (pid == 0) {
      sleep(1);
      printf("New PID=%d\n", getpid());
      if (read_lock(fd, 0, SEEK_SET, 0) < 0) {
        perror("read_lock: ");
        printf("Write Lock by pid=%d\n",
               lock_test(fd, F_WRLCK, 0, SEEK_SET, 0));
        exit(0);
      }
      printf("read lock success.\n");
      printf("Lock by pid=%d\n", lock_test(fd, F_WRLCK, 0, SEEK_SET, 0));

      sleep(interval);

      exit(0);
    } else {
      exit(1);
    }
  } else if (strcmp(argv[1], "dup") == 0) {
    int fd = open(path, O_RDWR); /* O_RDWR */
    if (fd == -1) {
      perror("open");
      exit(0);
    }
    /*  write lock entire file */
    if (write_lock(fd, 0, SEEK_SET, 0) < 0) {
      perror("write_lock: ");
      printf("Write Lock by pid=%d\n", lock_test(fd, F_WRLCK, 0, SEEK_SET, 0));
      exit(0);
    }
    printf("write lock success.\n");
    printf("Lock by pid=%d\n", lock_test(fd, F_WRLCK, 0, SEEK_SET, 0));

    int fd2 = 0;
    if ((fd2 = dup(fd)) < 0) {
      perror("dup:");
      exit(1);
    }
    close(fd);

    if (read_lock(fd2, 0, SEEK_SET, 0) < 0) {
      perror("read_lock: ");
      printf("Write Lock by pid=%d\n", lock_test(fd2, F_WRLCK, 0, SEEK_SET, 0));
      exit(0);
    }
    printf("read lock success.\n");
    printf("Lock by pid=%d\n", lock_test(fd2, F_WRLCK, 0, SEEK_SET, 0));

    sleep(interval);

    exit(0);
  } else {
    printf("Unknown action!\n");
  }
  return 0;
}
