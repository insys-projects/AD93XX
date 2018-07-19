
#ifndef _UTYPES_LINUX_H_
#define _UTYPES_LINUX_H_

#if defined (__LINUX__) || defined(__linux__) || defined (__QNX__)

#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

// added for 64-bit windows driver compatibility
#define _IPCS(x)                x
#define IPCS_strlen             strlen
#define IPCS_strcpy             strcpy
#define IPCS_strncpy            strncpy
#define IPCS_strcmp             strcmp
#define IPCS_stricmp            _stricmp
#define IPCS_strnicmp           _strnicmp
#define IPCS_strncmp            strncmp
#define IPCS_strcat             strcat
#define IPCS_strchr             strchr
#define IPCS_strstr             strstr
#define IPCS_strtol             strtol
#define IPCS_strtoll             strtoll
#define IPCS_strtoul            strtoul
#define IPCS_strtod             strtod
#define IPCS_strtok		  		strtok
#define IPCS_atol               atol
#define IPCS_atoi               atoi
#define IPCS_atoi64             atoll
#define IPCS_strtoui64          strtoull
#define IPCS_atof               atof
#define IPCS_strlwr             IPC_strlwr
#define IPCS_itoa               IPC_itoa
#define IPCS_printf             printf
#define IPCS_fprintf            fprintf
#define IPCS_fscanf             fscanf
#define IPCS_sprintf            sprintf
#define IPCS_vsprintf           vsprintf
#define IPCS_sscanf             sscanf
#define IPCS_fopen              fopen
#define IPCS_freopen            freopen
#define IPCS_fdopen             fdopen
#define IPCS_sopen              sopen
#define IPCS_fgets              fgets
#define IPCS_fgetc              fgetc
#define IPCS_getenv             getenv
#define IPCS_main               main
#define IPCS_toupper            toupper
#define IPCS_tolower            tolower
#define IPCS_fputs              fputs
#define IPCS_isspace            isspace
#define IPCS_isdigit            isdigit
#define IPCS_isalnum            isalnum
#define IPCS_isxdigit           isxdigit
#define IPCS_cputs              puts
#define IPCS_cscanf             scanf
#define IPCS_fputchar           putchar
#define IPCS_fgetchar           getchar
#define IPCS_flushall           IPC_flushall
#define IPCS_putch              putchar
#define IPCS_cgets              gets
#define IPCS_puts               puts
#define IPCS_gets               gets
#define IPCS_tmpnam             tmpnam
#define IPCS_perror             perror
#define IPCS_getcwd             getcwd
#define IPCS_chdir              chdir
#define IPCS_rmdir              rmdir
#define IPCS_access             access
#define IPCS_chmod              chmod
#define IPCS_creat              creat
#define IPCS_mktemp             mktemp
#define IPCS_remove             remove
#define IPCS_rename             rename
#define IPCS_asctime            asctime
#define IPCS_ctime              ctime
#define	IPCS_strftime           strftime
//-------------------------------------

#endif /* __linux__ */

#endif /* _UTYPES_LINUX_H_ */

/*
*  End of File
*/
