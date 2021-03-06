#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "trinity.h"	// __unused__
#include "arch.h"	// page_size
#include "files.h"
#include "log.h"
#include "maps.h"
#include "shm.h"
#include "sanitise.h"
#include "constants.h"

static int files_added = 0;
char **fileindex;
unsigned int files_in_index = 0;

struct namelist {
	struct namelist *prev;
	struct namelist *next;
	char *name;
};

static struct namelist *names = NULL;

static uid_t my_uid;
static gid_t my_gid;

static int ignore_files(const char *path)
{
	unsigned int i, j;
	unsigned int pathlen, offset = 0;

	/* These are exact matches. */
	const char *ignored_paths[] = {
		".", "..",

		/* dangerous/noisy/annoying stuff in /proc */
		"/proc/sysrq-trigger", "/proc/kmem", "/proc/kcore",

		/* dangerous/noisy/annoying stuff in /dev */
		"/dev/log", "/dev/mem", "/dev/kmsg",
		NULL
	};

	/* Partial matches. */	//FIXME: This whole function should just use globs to pattern match.
	const char *ignored_patterns[] = {

		/* dangerous/noisy/annoying per-process stuff. */
		"coredump_filter", "make-it-fail", "oom_adj", "oom_score_adj",
		NULL
	};

	pathlen = strlen(path);

	/* First do the exact matches */
	for (i = 0; ignored_paths[i]; i++) {
		if (strlen(ignored_paths[i]) != pathlen) {
			continue;
		}

		if (!strcmp(path, ignored_paths[i])) {
//			printf("Skipping %s\n", path);
			return 1;
		}
	}

	/* Now make sure none of the patterns match the end of the pathname */
	for (j = 0; j < pathlen; j++) {
		if (path[j] == '/')
			offset = j;
	}
	offset++;

	if (offset == 1)
		return 0;

	for (i = 0; ignored_patterns[i]; i++) {
		if (!strcmp(path + offset, ignored_patterns[i])) {
//			printf("Skipping pattern %s\n", path);
			return 1;
		}
	}

	/* special case to match tty* until I do globbing */
	if (!strncmp(path + offset, "tty", 3)) {
//		printf("Skipping %s\n", path);
		return 1;
	}
	return 0;
}

static struct namelist *list_alloc(void)
{
	struct namelist *node;

	node = malloc(sizeof(struct namelist));
	if (node == NULL)
		exit(EXIT_FAILURE);
	memset(node, 0, sizeof(struct namelist));
	return node;
}

static void __list_add(struct namelist *new, struct namelist *prev, struct namelist *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

static void list_add(struct namelist *list, const char *name)
{
	struct namelist *newnode;

	newnode = list_alloc();

	if (list == NULL) {
		list = names = newnode;
		list->next = list;
		list->prev = list;
	}
	newnode->name = strdup(name);

	__list_add(newnode, list, list->next);
}

static int check_stat_file(const struct stat *sb)
{
	int openflag;
	bool set_read = FALSE;
	bool set_write = FALSE;

	if (S_ISLNK(sb->st_mode))
		return -1;

	if (sb->st_uid == my_uid) {
		if (sb->st_mode & S_IRUSR)
			set_read = TRUE;
		if (sb->st_mode & S_IWUSR)
			set_write = TRUE;

	} else if (sb->st_gid == my_gid) {
		if (sb->st_mode & S_IRGRP)
			set_read = TRUE;
		if (sb->st_mode & S_IWGRP)
			set_write = TRUE;

	} else {
		if ((sb->st_mode & S_IROTH))
			set_read = TRUE;
		if (sb->st_mode & S_IWOTH)
			set_write = TRUE;
	}

	if ((set_read | set_write) == 0)
		return -1;

	if (set_read == TRUE)
		openflag = O_RDONLY;
	if (set_write == TRUE)
		openflag = O_WRONLY;
	if ((set_read == TRUE) && (set_write == TRUE))
		openflag = O_RDWR;

	if (S_ISDIR(sb->st_mode))
		openflag = O_RDONLY;

	return openflag;
}

static int file_tree_callback(const char *fpath, const struct stat *sb, __unused__ int typeflag, __unused__ struct FTW *ftwbuf)
{

	if (ignore_files(fpath)) {
		return FTW_SKIP_SUBTREE;
	}

	// Check we can read it.
	if (check_stat_file(sb) == -1)
		return FTW_CONTINUE;

	if (shm->exit_reason != STILL_RUNNING)
		return FTW_STOP;

	list_add(names, fpath);
	//printf("Adding %s\n", fpath);
	files_added++;

	return FTW_CONTINUE;
}


static void open_fds(const char *dirpath)
{
	int before = files_added;
	int flags = FTW_DEPTH | FTW_ACTIONRETVAL | FTW_MOUNT;
	int ret;

	/* By default, don't follow symlinks so we only get each file once.
	 * But, if we do something like -V /lib, then follow it
	 *
	 * I'm not sure about this, might remove later.
	 */
	if (victim_path == NULL)
		flags |= FTW_PHYS;

	ret = nftw(dirpath, file_tree_callback, 32, flags);
	if (ret != 0) {
		if (shm->exit_reason != EXIT_SIGINT)
			output(0, "Something went wrong during nftw(%s). Returned %d\n", dirpath, ret);
		return;
	}

	output(0, "Added %d filenames from %s\n", files_added - before, dirpath);
}

void generate_filelist(void)
{
	unsigned int i = 0;
	struct namelist *node;

	my_uid = getuid();
	my_gid = getgid();

	output(1, "Generating file descriptors\n");

	if (victim_path != NULL) {
		open_fds(victim_path);
	} else {
		open_fds("/dev");
		open_fds("/proc");
		open_fds("/sys");
	}

	if (files_added == 0) {
		output(1, "Didn't add any files!!\n");
		return;
	}

	if (shm->exit_reason != STILL_RUNNING)
		return;

	/*
	 * Generate an index of pointers to the filenames
	 */
	fileindex = malloc(sizeof(char *) * files_added);

	node = names;
	do {
		fileindex[i++] = node->name;
		node = node->next;
	} while (node->next != names);
	files_in_index = i;
}

static int open_file(void)
{
	int fd;
	int ret;
	char *filename;
	int flags;
	const char *modestr;
	struct stat sb;

retry:
	filename = get_filename();
	ret = lstat(filename, &sb);
	if (ret == -1)
		goto retry;

	flags = check_stat_file(&sb);
	if (flags == -1)
		goto retry;

	fd = open(filename, flags | O_NONBLOCK);
	if (fd < 0) {
		output(2, "Couldn't open %s : %s\n", filename, strerror(errno));
		return fd;
	}

	switch (flags) {
	case O_RDONLY:  modestr = "read-only";  break;
	case O_WRONLY:  modestr = "write-only"; break;
	case O_RDWR:    modestr = "read-write"; break;
	default: modestr = "unknown"; break;
	}
	output(2, "[%d] fd[%i] = %s (%s)\n",
		getpid(), fd, filename, modestr);
	return fd;
}

void open_files(void)
{
	unsigned int i, nr_to_open;
	int fd;

	if (files_in_index < NR_FILE_FDS)
		nr_to_open = files_in_index;
	else
		nr_to_open = NR_FILE_FDS;

	if (fileindex == NULL)	/* this can happen if we ctrl-c'd */
		return;

	for (i = 0; i < nr_to_open; i++) {
		fd = open_file();

		shm->file_fds[i] = fd;
		nr_file_fds++;
	}
}

void close_files(void)
{
	unsigned int i;
	int fd;

	shm->current_fd = 0;
	shm->fd_lifetime = 0;

	// FIXME: Does this need locking? At the least, check for NULL fd's
	for (i = 0; i < nr_file_fds; i++) {
		fd = shm->file_fds[i];
		shm->file_fds[i] = 0;
		if (fd != 0)
			close(fd);
	}

	nr_file_fds = 0;
}

char * get_filename(void)
{
	if (files_in_index == 0)	/* This can happen if we run with -n. Should we do something else ? */
		return NULL;

	return fileindex[rand() % files_in_index];
}

char * generate_pathname(void)
{
	char *pathname = get_filename();
	char *newpath;
	unsigned int len;
	unsigned int i, chance;

	if (pathname == NULL)		/* As above, handle -n correctly. */
		return NULL;

	len = strlen(pathname);

	chance = rand() % 100;
	switch (chance) {

	case 0 ... 90:
		/* 90% chance of returning an unmangled filename */
		if ((rand() % 100) > 10)
			return get_filename();

	case 91 ... 99:
		/* Create a bogus filename. */
		newpath = malloc(page_size);	// FIXME: We leak this.
		if (newpath == NULL)
			return get_filename();	// give up.

		generate_random_page(newpath);

		/* sometimes, just complete junk. */
		if (rand() % 2)
			goto out;

		/* Sometimes, pathname + junk */
		if (rand() % 2)
			(void) strncpy(newpath, pathname, len);
		else {
			/* make it look relative to cwd */
			newpath[0] = '.';
			(void) strncpy(newpath + 1, pathname, len);
		}

		/* Sometimes, remove all /'s */
		if ((rand() % 2) == 0) {
			for (i = 0; i < len; i++) {
				if (newpath[i] == '/')
					newpath[i] = rand();
			}
		}
out:
		/* 50/50 chance of making it look like a dir */
		if ((rand() % 2) == 0) {
			newpath[len] = '/';
			newpath[len + 1] = 0;
		}

		return newpath;
	default:
		BUG("Unreachable");
	}
}
