#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef __WIN32__
# include <windows.h>
#endif

#include <sys/stat.h>

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#include <string.h>
#include <slang.h>

static char *Slsh_Version = "0.6-0";
#define SLSHRC_FILE "slsh.rc"

#ifdef REAL_UNIX_SYSTEM
/* # define DEFAULT_LIBRARY_PATH "/usr/local/share/slsh:/usr/local/lib/slsh:/usr/share/slsh:/usr/lib/slsh"; */
# define DEFAULT_CONF_PATH "/usr/local/etc:/usr/local/slsh:/etc:/etc/slsh";
# define USER_SLSHRC ".slshrc"
#else 
# define DEFAULT_LIBRARY_PATH NULL
# define USER_SLSHRC "slsh.rc"
#endif

#ifdef __os2__
# ifdef __IBMC__
/* IBM VA3 doesn't declare S_IFMT */
#  define	S_IFMT	(S_IFDIR | S_IFCHR | S_IFREG)
# endif
#endif

#ifndef S_ISLNK
# ifdef S_IFLNK
#   define S_ISLNK(m) (((m) & S_IFMT) == S_IFLNK)
# else
#   define S_ISLNK(m) 0
# endif
#endif

#ifndef S_ISREG
# ifdef S_IFREG
#   define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
# else
#   define S_ISREG(m) 0
# endif
#endif

#ifndef S_ISDIR
# ifdef S_IFDIR
#   define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
# else
#   define S_ISDIR(m) 0
# endif
#endif

#ifndef S_ISCHR
# ifdef S_IFCHR
#   define S_ISCHR(m) (((m) & S_IFMT) == S_IFCHR)
# else
#   define S_ISCHR(m) 0
# endif
#endif

#ifndef S_ISBLK
# ifdef S_IFBLK
#   define S_ISBLK(m) (((m) & S_IFMT) == S_IFBLK)
# else
#   define S_ISBLK(m) 0
# endif
#endif

#ifndef S_ISFIFO
# ifdef S_IFIFO
#   define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO)
# else
#   define S_ISFIFO(m) 0
# endif
#endif

#ifndef S_ISSOCK
# ifdef S_IFSOCK
#   define S_ISSOCK(m) (((m) & S_IFMT) == S_IFSOCK)
# else
#   define S_ISSOCK(m) 0
# endif
#endif

   
#ifndef S_IRUSR
# define S_IRUSR	0400
#endif
#ifndef S_IWUSR
# define S_IWUSR	0200
#endif
#ifndef S_IXUSR
# define S_IXUSR	0100
#endif
#ifndef S_IRGRP
# define S_IRGRP	0040
#endif
#ifndef S_IWGRP
# define S_IWGRP	0020
#endif
#ifndef S_IXGRP
# define S_IXGRP	0010
#endif
#ifndef S_IROTH
# define S_IROTH	0004
#endif
#ifndef S_IWOTH
# define S_IWOTH	0002
#endif
#ifndef S_IXOTH
# define S_IXOTH	0001
#endif
#ifndef S_ISUID
# define S_ISUID	04000
#endif
#ifndef S_ISGID
# define S_ISGID	02000
#endif
#ifndef S_ISVTX
# define S_ISVTX	01000
#endif

typedef struct _AtExit_Type
{
   SLang_Name_Type *nt;
   struct _AtExit_Type *next;
}
AtExit_Type;

static AtExit_Type *AtExit_Hooks;

static void at_exit (SLang_Ref_Type *ref)
{
   SLang_Name_Type *nt;
   AtExit_Type *a;

   if (NULL == (nt = SLang_get_fun_from_ref (ref)))
     return;

   a = (AtExit_Type *) SLmalloc (sizeof (AtExit_Type));
   if (a == NULL)
     return;

   a->nt = nt;
   a->next = AtExit_Hooks;
   AtExit_Hooks = a;
}

static void c_exit (int *code)
{
   while (AtExit_Hooks != NULL)
     {
	AtExit_Type *next = AtExit_Hooks->next;
	if (SLang_Error == 0)
	  (void) SLexecute_function (AtExit_Hooks->nt);

	SLfree ((char *) AtExit_Hooks);
	AtExit_Hooks = next;
     }
   exit (*code);
}


static void stat_mode_to_string (void)
{
   int mode, opts;
   char mode_string[12];
   
   opts = 0;
   if (SLang_Num_Function_Args == 2)
     {
	if (-1 == SLang_pop_integer (&opts))
	  return;
     }

   if (-1 == SLang_pop_integer (&mode))
     return;


   if (S_ISREG(mode)) mode_string[0] = '-';
   else if (S_ISDIR(mode)) mode_string[0] = 'd';
   else if (S_ISLNK(mode)) mode_string[0] = 'l';
   else if (S_ISCHR(mode)) mode_string[0] = 'c';
   else if (S_ISFIFO(mode)) mode_string[0] = 'f';
   else if (S_ISSOCK(mode)) mode_string[0] = 's';
   else if (S_ISBLK(mode)) mode_string[0] = 'b';
   
   if (mode & S_IRUSR) mode_string[1] = 'r'; else mode_string[1] = '-';
   if (mode & S_IWUSR) mode_string[2] = 'w'; else mode_string[2] = '-';
   if (mode & S_IXUSR) mode_string[3] = 'x'; else mode_string[3] = '-';
   if (mode & S_ISUID) mode_string[3] = 's';

#ifdef __WIN32__
   mode_string[4] = '-';
   mode_string[5] = '-';
   mode_string[6] = '-';

   if (opts & FILE_ATTRIBUTE_ARCHIVE) mode_string[7] = 'A'; else mode_string[7] = '-';
   if (opts & FILE_ATTRIBUTE_SYSTEM) mode_string[8] = 'S'; else mode_string[8] = '-';
   if (opts & FILE_ATTRIBUTE_HIDDEN) mode_string[9] = 'H'; else mode_string[9] = '-';
#else
   if (mode & S_IRGRP) mode_string[4] = 'r'; else mode_string[4] = '-';
   if (mode & S_IWGRP) mode_string[5] = 'w'; else mode_string[5] = '-';
   if (mode & S_IXGRP) mode_string[6] = 'x'; else mode_string[6] = '-';
   if (mode & S_ISGID) mode_string[6] = 'g';

   if (mode & S_IROTH) mode_string[7] = 'r'; else mode_string[7] = '-';
   if (mode & S_IWOTH) mode_string[8] = 'w'; else mode_string[8] = '-';
   if (mode & S_IXOTH) mode_string[9] = 'x'; else mode_string[9] = '-';
   if (mode & S_ISVTX) mode_string[9] = 't';
#endif
   
   mode_string[10] = 0;
   (void) SLang_push_string (mode_string);
}


static int Verbose_Loading;

static int try_to_load_file (char *path, char *file, char *ns)
{
   int status;

   if (path == NULL)
     path = ".";

   if (file != NULL)
     {
	file = SLpath_find_file_in_path (path, file);
	if (file == NULL)
	  return 0;
     }
   /* otherwise use stdin */

   status = SLns_load_file (file, ns);
   SLfree (file);
   if (status == 0)
     return 1;
   return -1;
}


static int load_startup_file (void)
{
   char *dir;
   int status;

   dir = getenv ("SLSH_CONF_DIR");
   if (dir == NULL)
     dir = getenv ("SLSH_LIB_DIR");

   if (NULL == dir)
     {
#ifdef SLSH_CONF_DIR
	dir = SLSH_CONF_DIR;
	if (dir != NULL)
	  {
	     status = try_to_load_file (dir, SLSHRC_FILE, NULL);
	     if (status == -1)
	       return -1;
	     if (status == 1)
	       return 0;
	  }
#endif
	dir = DEFAULT_CONF_PATH;
     }

   if (-1 == (status = try_to_load_file (dir, SLSHRC_FILE, NULL)))
     return -1;

   if ((status == 0) && Verbose_Loading)
     {
	SLang_vmessage ("*** Installation Problem?  Unable to find the %s config file.",
			SLSHRC_FILE);
     }
     
   return 0;
}


#if 0
static int is_script (char *file)
{
   FILE *fp;
   char buf[3];
   int is;

   if (NULL == (fp = fopen (file, "r")))
     return 0;

   is = ((NULL != fgets (buf, sizeof(buf), fp))
	 && (buf[0] == '#') && (buf[1] == '!'));

   fclose (fp);
   return is;
}
#endif

static int setup_paths (void)
{
   char *libpath;

   if (NULL == (libpath = getenv ("SLSH_PATH")))
     {
#ifdef SLSH_PATH
	libpath = SLSH_PATH;
#endif
     }

   return SLpath_set_load_path (libpath);
}

/* Create the Table that S-Lang requires */
static SLang_Intrin_Fun_Type Intrinsics [] =
{
   MAKE_INTRINSIC_I("exit", c_exit, VOID_TYPE),
   MAKE_INTRINSIC_1("atexit", at_exit, VOID_TYPE, SLANG_REF_TYPE),
   MAKE_INTRINSIC_0("stat_mode_to_string", stat_mode_to_string, VOID_TYPE),
   SLANG_END_INTRIN_FUN_TABLE
};

static void usage (void)
{
   char *libpath;
   fprintf (stderr, "\
Usage: slsh [OPTIONS] [-|file [args...]]\n\
 --help           Print this help\n\
 --version        Show slsh version information\n\
 -g               Compile with debugging code, tracebacks, etc\n\
 -n               Don't load personal init file\n\
 -i init-file     Use this file instead of ~/%s\n\
 -v               Show verbose loading messages\n\
",
	    USER_SLSHRC
	    );
   libpath = SLpath_get_load_path ();
   fprintf (stderr, "Default search path: %s\n", (libpath == NULL) ? "" : libpath);
   SLang_free_slstring (libpath);
   
   exit (1);
}

static void version (void)
{
   fprintf (stdout, "slsh version %s\n", Slsh_Version);
   fprintf (stdout, "S-Lang Library Version: %s\n", SLang_Version_String);
   if (SLANG_VERSION != SLang_Version)
     {
	fprintf (stdout, "\t** Note: This program was compiled against version %s.\n",
		 SLANG_VERSION_STRING);
     }
   
   exit (0);
}

int main (int argc, char **argv)
{
   char *file = NULL;
   char *init_file = USER_SLSHRC;
   char *init_file_dir;

   if (SLang_Version < SLANG_VERSION)
     {
	fprintf (stderr, "***Warning: Executable compiled against S-Lang %s but linked to %s\n",
		 SLANG_VERSION_STRING, SLang_Version_String);
	fflush (stderr);
     }

   if ((-1 == SLang_init_all ())
       || (-1 == SLang_init_array_extra ())
       || (-1 == SLang_init_import ()) /* dynamic linking */
       || (-1 == SLadd_intrin_fun_table (Intrinsics, NULL)))
     {
	fprintf(stderr, "Unable to initialize S-Lang.\n");
	return 1;
     }

   /* FIXME for other systems */
   init_file_dir = getenv ("HOME");

   if (-1 == setup_paths ())
     return -1;

   while (argc > 1)
     {
	if (0 == strcmp (argv[1], "--version"))
	  version ();

	if (0 == strcmp (argv[1], "--help"))
	  usage ();

	if (0 == strcmp (argv[1], "-g"))
	  {
	     SLang_generate_debug_info (1);
	     argc--;
	     argv++;
	     continue;
	  }

	if (0 == strcmp (argv[1], "-n"))
	  {
	     init_file = NULL;
	     argc--;
	     argv++;
	     continue;
	  }
	
	if (0 == strcmp (argv[1], "-v"))
	  {
	     (void) SLang_load_file_verbose (1);
	     Verbose_Loading = 1;
	     argc--;
	     argv++;
	     continue;
	  }
	
	if ((0 == strcmp (argv[1], "-i"))
	    && (argc > 2))
	  {
	     init_file = argv[2];
	     init_file_dir = NULL;
	     argc -= 2;
	     argv += 2;
	     continue;
	  }
	break;
     }

   if (argc == 1)
     {
	if (0 == isatty (fileno(stdin)))
	  file = NULL;
	else
	  usage ();
     }
   else
     {
	file = argv[1];
	if (0 == strcmp (file, "-"))
	  file = NULL;
#if 0
	if (is_script (file))
	  {
	     argv++;
	     argc--;
	  }
#else
	argc--;
	argv++;
#endif
     }
   /* fprintf (stdout, "slsh: argv[0]=%s\n", argv[0]); */
   if (-1 == SLang_set_argc_argv (argc, argv))
     return 1;

   /* Turn on traceback generation */
   SLang_Traceback = 1;

   if (-1 == load_startup_file ())
     return SLang_Error;
   
   if ((init_file != NULL)
       && (-1 == try_to_load_file (init_file_dir, init_file, NULL)))
     return SLang_Error;

   /* Now load an initialization file and exit */
   if (0 == try_to_load_file (NULL, file, NULL))
     {
	fprintf (stderr, "%s: file not found\n", file);
	exit (1);
     }
   
   (void) SLang_run_hooks ("slsh_main", 0);
   return SLang_Error;
}
