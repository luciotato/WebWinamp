/*	
  ===================================================================================
   INCLUDES 
  ===================================================================================
*/

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "pocrt.lib")

#include <windows.h>
#include <winsock.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <conio.h>
#include <stdarg.h>     // allows passing an unknown number parameters
#include <signal.h>
#include <process.h>
#include <time.h>
/*#include <limits.h>		// used for INT_MAX declaration*/
#include <sys/types.h>
#include <sys/stat.h>

#include "frontend.h"
#include "ipc_pe.h"

#ifndef min
#define min(x,y) (( x ) < ( y ) ? ( x ) : ( y ))
#define max(x,y) (( y ) < ( x ) ? ( x ) : ( y ))
#endif


/*	======================================================================================================================
	Copyright (C) 1998-2001 Nullsoft, Inc.
	
	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any damages
	arising from the use of this software.
	
	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:
	
	1. The origin of this software must not be misrepresented; you must not
	   claim that you wrote the original software. If you use this software
	   in a product, an acknowledgment in the product documentation would be
	   appreciated but is not required.
	2. Altered source versions must be plainly marked as such, and must not be
	   misrepresented as being the original software.
	3. This notice may not be removed or altered from any source distribution.
	======================================================================================================================

	
	C H A N G E   L O G
	
	2.5.d  - ??

	2.6.02 - 2002/06/14	(rar) Lots o stuff has changed.   Includes support for *.css file., change some of the way
						(rar) this app logs stuff.  added some global variables to help reduce redundant code.  It'll get there.	
	2.6.05 - 2002/06/16 (rar) Added support for *.js file.  Incorporated use of the WebDings font for control symbols.
	2.6.10 - 2002/06/17 (rar) removed [p] button which clears the playlist and adds the selected song as the sole song to the liast
						(rar) added [c] in [p]'s place, which allows you to cue a song to play direct after the song that is currently playing.
	2.6.11 - 2002/06/19 (rar) Added Support for rebots.txt file, which basically disallows indexing of anything from this 
	                          "server".  Content resides within this program.
						(rar) Modified Cache expire time (from 1 hour to 1 year)
	2.6.12 - 2002/07/07 (rar) Added \r's to ROBOTS.TXT output string.
	         2002/11/27 (rar) Added Self Installer using NSIS.
	2.6.13 - 2002/11/27 (mm ) Added end-of-playlist actions
						(mm ) Added cover art options
	2.7.1  - 2002/11/29 (rar) Merge Michael Michon's changes into main.
	2.7.2  - 2003/01/11 (rar) added doRecursiveAddDB prototype to list of function prototypes
                        (rar) printf statement to clear the line in the console when MakeDB is run
                        (rar) removed call to link *.js file since functions are only for windows CE and are commented out at 
						      the moment.  For some reason, the functions within were causing javascript errors
    2.7.3  - 2003/01/12 (rar) added abiltiy to rescan files
						(rar) modified the layout to include controls in middle frame
						(rar) added 'listen' feature, which allows user to listen to the shoutcast stream directly from wwwinamp, and not have to 
						      visit the shoutcast server
						(rar) sprinkled a few comments here and there
                        (rar) change font size to use %, not px
						(rar) updated copyright statement
    2.7.4  - 2003/02/01 (rar) added cover art size, Winamp Class Name and library page size to options config file
						(rar) Formatted vanilla config for easier readability, and created subsections for grouped items
						(rar) added database list sorting
						(rar) redid how the cover art shows up, when enabled.  The size is now configureable, and will show up in a nested table to
						      the left in the song search.
						(rar) updated the interface, with new color scheme and controls images. Nifty
						(rar) started some work on reworking the LoadConfig.  Trying to get it to be a little bit smarter
						(rar) added support for multiple instances of winamp/wwwinamp.  Good for broadcasting multiple servers off of one machine. see readme for more details.
						(rar) Much more code commenting, cleanup, reorganization.
						(rar) fixed a bug that would cause wwwinamp to crash when loading the playlist window & there were no items in the media library
						(mm ) fixed BUG676947 [ 676947 ] Trailing slash on 'Listen' HyperLink
						(mm ) fixed BUG683202 [ 683202 ] Cover art doesn't show with multiple paths 
						(mm ) added RFE683207 [ 683207 ] Library update should be general update
						(mm ) added RFE683222 [ 683222 ] optional HTML header snippet 
						(mm ) added separate size for database list and now playing cover art
						(mm ) added logic to prevent adding duplicate songs, usually caused by accidential double clicking.
						(mm ) added ability to enqueue an entire folder by clicking on the folder name
						(rar) fixed bug with "log into mp3j mode" button & html target tag
						(rar) added a critical section mutex to media library update function
						(rar) removed the need attach "m=left" on operation calls.  this will save bandwith.
						(rar) moved update button to media library window
						(rar) redid how cover art works, uses dynamic urls and not folder names for url.
	2.7.5  - 2003/03/05 (released 2.7.5)
	2.7.6  - 2003/08/19 (rar) changed code to print out server address upon successful start up
						(rar) made modifications to log_printf error messages in several spots, as well as cleaned up some verbiage, more work to be done
	2.7.7  - 2003/12/07 (rar) made changes that caused access violations while loading files with long names. change wsprintf to strncpy. Also changed some test to make it less confusing to figure out how to use configuration files.
	2.7.8  - 2003/12/30 (rar) included source as part of distro
						(rar) added auto select to  input field for search box
						(rar) got sidetracked and re-formated output of startup messages
						(rar) added user:pass to Interfaces output for easier cutting & pasting
						(rar) added search google button

	2.7.9  - 2005/02/13 (lmt) Volume up/dn vUp,vDn
						(lmt) "getMilliseconds()" used to force explorer to send requests to the server
						(lmt) HTMLTitle in configuration file
						(lmt) "separ" Frame used also for dummy reloads
						(lmt) Support for .m3u lists, *lists and SaveList
						(lmt) List Order
						(lmt) void DBAdd
						(lmt) Languaje support
						(lmt) g_position // is the track to be visible  
						(lmt) g_FileHlinkMode: 0=add 1=cue 2=cue & play now. Default 0=add
						(lmt) fix bug with MS IExplorer META REFRESH. used See: time(0)
	         2007/12/1  (lmt) BurnCD support, que grabe un bat para copiar los archivos y la lista m3u 
*/


//#define WWWA_LANG_ENGLISH //un-comment desired language
#define WWWA_LANG_SPANISH //un-comment desired language

#ifdef WWWA_LANG_ENGLISH 
 #define SHOW_ALL "Show All"
 #define Now_Playing "Now Playing"
 #define is_ " is "
 #define stopped "*stopped*"
 #define playing "playing"
 #define paused "*paused*"
 #define of_ " of "
 #define file_ "file"
 #define left_in_queue "left in queue"
 #define No_Winamp_playlist_found "No Winamp playlist found"
 #define No_instance_Winamp_was_found_running "No instance Winamp was found running"
 #define Search "Search"
 #define ReScan "Re-Scan" 
 #define Lists "Lists"
 #define Added_in_the "Added in the "
 #define Ultimos "Last"
 #define Dias "days"
 #define Save_List "Save List"
 #define Order_List "Order List"
 #define Fix_List "Fix List"
 #define Grabar_CD "Burn CD"
 #define Showing "Showing"
 #define all_ "all"
 #define Searching_for "Searching for "
 #define in_ "in"
 #define Starting_at  "Starting at "
 #define click_to_add_file "click to add file to end of playlist"
 #define click_to_cue "click to cue file"
 #define click_to_cue_play "click to cue file and play now"
 #define last "last"
 #define next "next"
 #define No_items_found "No items found"
 #define Try_starting_Winamp "Try starting Winamp"
 #define Show_Previous "Previous"
#endif

#ifdef WWWA_LANG_SPANISH 
 #define SHOW_ALL "Todos"
 #define Now_Playing "Actual"
 #define is_ " esta "
 #define stopped "*detenido*"
 #define playing "tocando"
 #define paused "*en pausa*"
 #define of_ " de "
 #define file_ "archivo"
 #define left_in_queue "quedan en cola"
 #define No_Winamp_playlist_found "No se hallo una lista de temas en Winamp"
 #define No_instance_Winamp_was_found_running "No se hallo el Winamp abierto"
 #define Search "Buscar"
 #define ReScan "Re-Leer" 
 #define Lists "Listas"
 #define Added_in_the "Agregados en los "
 #define Ultimos "Ultimos"
 #define Dias "días"
 #define Save_List "Grabar Lista"
 #define Order_List "Ordenar"
 #define Fix_List "Corrije Lista"
 #define Grabar_CD "Grabar CD"
 #define Showing "Mostrando"
 #define all_ "todos los"
 #define Searching_for "Buscando "
 #define in_ "en"
 #define Starting_at  "Empezando en "
 #define click_to_add_file "click para agrear el tema al final de la lista"
 #define click_to_cue "click para agregar a continuacion"
 #define click_to_cue_play "click para agregar y escuchar ahora"
 #define last "anterior"
 #define next "proximo"
 #define No_items_found "No se hallaron archivos"
 #define Try_starting_Winamp "Iniciar el Winamp"
 #define Show_Previous "Anteriores"
#endif

#define SERV_NAME			"WWWinamp"

#define DEBUG				0
#define SERV_VER 			"2.7.8 (Built: " __DATE__ " " __TIME__ ")"
#define SERV_NAME			"WWWinamp"
#define SERV_NAME_LONG 		"WWWinamp Remote Control"
#define BRANDING 			"(C) 2005-2006 Lucio M. Tato"
#define BRANDING_COPYRIGHT	"(C) 2002-2003 Halo 8 Productions, Inc. All Rights Reserved."
#define COPYRIGHT 			"(C) 1998-2001 Nullsoft, Inc. All Rights Reserved."
#define BRANDING_URL 		"http://projects.halo8.net/"

#define ADMIN_URL			"/mp3j"
#define STYLE_URL			"/wwwinamp.css"
#define JSCRIPT_URL			"/wwwinamp.js"
#define ROBOTS_URL			"/robots.txt"
#define CONTROL_URL			"/controls.gif"
#define CONTROLBG_URL		"/controls.bg.gif"
#define CONTROLADM_URL		"/controls.mp3j.gif"
#define SPACER_URL			"/spacer.gif"
#define FOLDER_URL			"/folder.gif"
#define PLAYLIST_DEFAULT	"?m=left#playing"

#define HOSTNAME_PLACEHOLDER "%hostname"
/*	======================================================================================================================
	TYPES
*/

typedef struct {
	struct sockaddr_in sin;
	int msgsock,s;
	} cnx_inf;

typedef struct {
	char leading_path[MAX_PATH];
	char file[MAX_PATH];
	long daysold;
	byte dbfound;
	} dbType;

/*	======================================================================================================================
	FUNCTION PROTOTYPES
*/

int		main(int argc, char **argv);
int		LoadConfig(void);
int		getip(int which, struct sockaddr_in *sin);
int		init_socketlib(int which);
int		recv_string(int s, char *str, int maxlen);
int		OpenSocket(short port, int mc);
int		WaitForConnection(int sock,struct sockaddr_in *sin);
int		sock_printf(int sock, char *fmt, ...);
int		sock_send(int sock, char *buffer,int size);
int		myrand(void);
int		httpgetFile( int sock, char *fileName, char *ContentType, char *cacheString );
int		hasCoverArt( char *fn );
//unused- char	*getTimeStr(long alt_time);
char	*gethost(struct sockaddr_in *sin);
char	*extension(char *fn);
void	mysrand (unsigned int seed);
void	encodeLP(char *out, char *in);
void	quit();
void	makeDB();
void	doRecursiveAddDB(char *leading_path);
//unused- void	fixstring(char *in, char *out);
void	unfixstring(char *in, char *out, int maxlen);
void	log_printf(char *format, ...);
void	launchthread(void *threadproc, void *data);
void	SetNonBlock(int msgsock, u_long i);
void	CloseSocket(int sock);
void	http_handlereq(char *url, char *user_agent, char *encodedlp, int sock, struct sockaddr_in *sin);
void	quitthread();

unsigned int WINAPI HandleConnection(void *p);

/*	======================================================================================================================
	GLOBAL VARIABLES
*/

// configured/validated in LoadConfig()
short	g_dst_port						= 80;

int		g_show_cover_art				= 0;
int		g_perform_lookups				= 0;
int		g_show_requests					= 0;
int		g_left_refresh					= 60;				// left panel refresh rate
int		g_library_page_size				= 500;
int		g_cover_art_size_db				= 100;
int		g_cover_art_size_playing		= 150;

char	g_acclp[512]					= ""; // encoded access password
char	g_admlp[512]					= ""; // encoded admin password
char	g_acclp_c[512]					= ""; // unencoded access password
char	g_admlp_c[512]					= ""; // unencoded admin password
char	g_dst_ip[128]					= "";
char	g_ext_types[128]				= "";
char	g_sc_server[1024]				= ""; // shoucast server info, null/empty for none
char	g_winamp_class_name[128]		= "Winamp v1.x";

char	g_db_path[MAX_PATH*4]				= "";
char	g_log_file[MAX_PATH]			= "";
char	g_winamp_dir[MAX_PATH]			= "C:\\Program Files\\Winamp";
char	g_db_listspath[MAX_PATH]		= "";
char	g_db_filelist[MAX_PATH]			= "";
int		g_db_filelist_DbStart; //where does g_db_filelist starts in DB
char	g_filler_stream_url[MAX_PATH]	= "";
char	g_html_include_file[MAX_PATH]	= "include.html";
char	g_cover_art_root_dir[MAX_PATH]	= "";
char	g_NeroPath[MAX_PATH]			= "C:\\Nero\\TEMP\\";
char	g_NeroBatPath[MAX_PATH]			= "C:\\Nero";


enum	eop_modes { 
		eop_mode_silence=0, 
		eop_mode_random=1, 
		eop_mode_stream=2 } g_eop_action	= eop_mode_random;

char	g_html_Title[255]	=  BRANDING " " SERV_NAME;
char	g_Last_List[255]	=  "";

int		g_position=-1;  // is the track to be visible
int		g_selected=-1;  // is the track to be marked

int		g_order_mode		= 0;
int		g_base_orden		= 0;
int		g_showfile			= 0;

int		g_FileHlinkMode = 0;

char szHostName[20];

int UltimosDiasSearch=7;

char g_statusText[1024];


// rest of the global config
CRITICAL_SECTION log_mutex;
CRITICAL_SECTION library_mutex;
CRITICAL_SECTION playlist_mutex;
dbType *database;
int		g_count;
int		database_used;
int		database_size;
int		g_log							= 1;
int		g_playing_standby				= 0;
int		g_rand_next						= 1;
int		g_last_track					= -1;            // index to last added track 
char	g_config_file[MAX_PATH];
char	g_working_exe[MAX_PATH];
char	g_working_dir[MAX_PATH];
char	g_winamp_exe[MAX_PATH];
char	g_working_full[MAX_PATH];
char	g_working_name[MAX_PATH];
char	g_ext_type_list[128]			= "mp3;ogg;wav";
char	g_cover_art_filename[256]		= "folder.jpg";
char	g_debug[16]						= "DEBUG:\n\t";
char*	g_include_html					= 0;

/*	======================================================================================================================
	FUNCTION IMPLEMENTATIONS
*/

int main(int argc, char **argv) {
	int MainSocket;
	char log_area[16]="[main]", *q;
	struct sockaddr_in sin;
	// added to get hostname in output
	int iError;
	HOSTENT *hHostent;

	InitializeCriticalSection(&log_mutex);
	InitializeCriticalSection(&library_mutex);
	InitializeCriticalSection(&playlist_mutex);

	// get arg[0], the execution path and filename.
	GetModuleFileName( NULL, g_working_full, MAX_PATH );
	
	//set up g_working_dir
	strncpy( g_working_dir,  g_working_full, MAX_PATH ); // strncpy(s, t, n) copy n chars from s to t
	q = g_working_dir + strlen(g_working_dir);
	while (q >= g_working_dir && *q != '\\') q--;    
	*++q=0;
	
	// set up g_working_exe
	strncpy( g_working_exe,  g_working_full, MAX_PATH );
	q = g_working_exe + strlen(g_working_exe);
	while (q >= g_working_exe && *q != '\\') q--; 
	strcpy( g_working_exe, ++q );
	
	// set up g_working_name   (get the wwwinamp portion to find the rest of the files.
	strncpy( g_working_name, g_working_exe, MAX_PATH );
	q = g_working_name + strlen(g_working_name);
	while (q >= g_working_name && *q != '.') q--; *q=0;
	
	// print out header to show what's going on...
	printf("\n" "-- " SERV_NAME_LONG " " SERV_VER "\n" "-- " BRANDING_COPYRIGHT "\n" "-- " COPYRIGHT "\n" "-- Use \"%s config-file.ini\" to specify an configuration file.\n\n", g_working_exe);

	if (argc > 1) { // our only argument to wwwinamp should be the config file name
		if ( strstr(argv[1],"\\") ) strcpy( g_config_file, argv[1] );			// appears to be in a separate directory (\\ = \) so use whole string
		else wsprintf( g_config_file, "%s%s", g_working_dir, argv[1] );			// looks like simple file name, so we append the current working dir
		}
	else wsprintf( g_config_file, "%s%s.ini", g_working_dir, g_working_name );	// we don't have a command line argument... so just use the default...

	// Debug Statements
	if (DEBUG) printf( 
		"%s" 
		"g_working_full: %s\n\t" 
		"g_working_dir : %s\n\t"
		"g_working_exe : %s\n\t"
		"g_working_name: %s\n\t"
		"g_config_file : %s\n\t"
		, 
		g_debug, 
		g_working_full, 
		g_working_dir, 
		g_working_exe, 
		g_working_name,
		g_config_file
		);
	
	// Init Socket Lib
	log_printf( "%s Init Socket Lib: ", log_area );
	if ( init_socketlib(1) < 0 ) {
		printf( "Failure.\n" );
		quit();
		}
	else printf( "Successful.\n" );

	iError=gethostname(szHostName,sizeof(szHostName));
	// Tell the user that we could not get the host name
	if(iError==SOCKET_ERROR) {
		log_printf("%s ERROR: Could not get the host name.", log_area);
		}

	// Load Configuration
	if ( LoadConfig() ) quit();

	// Opening Socket
	log_printf( "%s Opening Socket Port %d: ", log_area, g_dst_port );
	MainSocket = OpenSocket(g_dst_port,32);
	if (MainSocket < 0) {
		printf( "Failure.\n" );
		quit();
		}
	else printf( "Successful.\n" );

	hHostent=gethostbyname(szHostName);
	// Tell the user that we could not get the host address
	if(hHostent==NULL) {
		log_printf("%s ERROR: Could not get the host address.\n", log_area);
		}
	else {
		// debug log_printf("Host name : %s\n", hHostent->h_name);
		// debug log_printf("IP Address : %s\n", inet_ntoa(*((struct in_addr*)hHostent->h_addr)));
		log_printf( "hostname: %s", szHostName);
		log_printf( "%s User Interface:  http://%s@%s:%d", log_area, g_acclp_c, inet_ntoa(*((struct in_addr*)hHostent->h_addr)), g_dst_port );
		log_printf( "%s Admin Interface: http://%s@%s:%d%s", log_area, g_admlp_c, inet_ntoa(*((struct in_addr*)hHostent->h_addr)), g_dst_port, ADMIN_URL );
		}

	makeDB();

	log_printf("%s %s Start-up: Successful. System available for users.\n\n", log_area, SERV_NAME, g_dst_port);

	mysrand( (unsigned)time( NULL ) );

	while (1) {
		int msgsock;
		cnx_inf *c = (cnx_inf*) calloc( 1, sizeof(cnx_inf) );
		msgsock = WaitForConnection( MainSocket, &sin ); 
		memcpy( &c->sin, &sin, sizeof(sin) );
		c->msgsock = msgsock;
		c->s = MainSocket;
		launchthread( (void*)HandleConnection, (void *)c );
		//HandleConnection((void *)c);
		}
	CloseSocket(MainSocket);
	log_printf( "%s Shutting down socket.", log_area );
	init_socketlib(0);
	return 0;
	}

int LoadConfig(void) {
	FILE *conf=NULL;
	FILE* includeFile;
	int line_number			= 0;
	char log_area[8]		= "[conf]";
	char buffer[1024];
	char *tok;
	char *configarray[] = {
		"AccessLoginPassword", 
		"AdminLoginPassword",
		"NameLookups", 
		"ShowRequests", 
		"ShowCoverArt", 
		"Port", 
		"RefreshRate", 
		"LibraryPageSize", 
		"CoverArtSizeLibrary", 
		"CoverArtSizePlaylist",
		"EndOfPlaylistAction",
		"WinampDir", 
		"LogFile",  
		"DBPath", 
		"DBFileList", 
		"Extensions", 
		"ShoutCastServer", 
		"WinampClassName", 
		"CoverArtFilename", 
		"FillerStreamURL",
		"CoverArtRootDir",
		"HTMLIncludeFile",
		"IP",
		"HTMLTitle",
		"ListsPath",
		"FileHLinkMode",
		"NeroPath",
		"NeroBatPath"
	};
	
	conf = fopen( g_config_file, "rb" );
	log_printf( "%s Loading Configuration: %s.\n", log_area, g_config_file );
	if (!conf) {
		printf( "%s ERROR: couldn't find config file.\n", log_area );
		return -1;
		}
	while (1) {
		int i = 0;
		char *pbuffer;
		fgets(buffer, sizeof(buffer), conf);
		if ( feof(conf) ) break; // end of file, quit
		line_number++; // increment the line number

		// terminate the string where we find a carriage return or line feed
		while ( buffer[strlen(buffer)-1]=='\n' || buffer[strlen(buffer)-1]=='\r' ) buffer[strlen(buffer)-1]='\0';

		pbuffer=buffer;
		while (*pbuffer == ' ' || *pbuffer == '\t') pbuffer++; // skip white space
      
		// if we encounter a ; [ or #, or if it's empty, then we just go to the next line
		if (!*pbuffer || *pbuffer == ';' || *pbuffer == '[' || *pbuffer == '#') continue; 
      
		tok=pbuffer;
		while (*pbuffer != '=' && *pbuffer != '\r' && *pbuffer) pbuffer++;
		if (!*pbuffer) { // basically, if we got to the end of the string with no '='
			log_printf( "%s ERROR: line %d: %s invalid configuration syntax.\n", log_area, line_number, buffer);
			return -1;
			}
		*pbuffer++=0;

		for (i=0; i<(int)(sizeof(configarray)/sizeof(configarray[0])) && stricmp(configarray[i],tok)!=(int)0
           		; i++); // after done, i will = index of configuration
		if (i >= (int)(sizeof(configarray)/sizeof(configarray[0]))) {
			log_printf( "%s ERROR: line %d: %s invalid configuration option.\n", log_area, line_number, buffer);
			return -1;
			}
      
		tok=pbuffer;
		while (*tok == ' ') tok++;
      	//log_printf( "%d: %s = %s", line_number, configarray[i], tok);

		switch (i) {
			// Login & Password Strings
			case 0:		encodeLP(g_acclp, tok);	strcpy(g_acclp_c, tok);	break; // "AccessLoginPassword"
			case 1:		encodeLP(g_admlp, tok);	strcpy(g_admlp_c, tok);	break; // "AdminLoginPassword"

			// boolean values
			case 2:		g_perform_lookups				= !!atoi(tok);	break; // "NameLookups"
			case 3:		g_show_requests					= !!atoi(tok);	break; // "ShowRequests"
			case 4:		g_show_cover_art				= !!atoi(tok);	break; // "ShowCoverArt"

			// Integer Values // atoi() returns zero if it can't find anything int in the string.
			case 5:		g_dst_port						= atoi(tok);	break; // "Port"
			case 6:		g_left_refresh					= atoi(tok);	break; // "RefreshRate"
			case 7:		g_library_page_size				= atoi(tok);	break; // "LibraryPageSize"
			case 8:		g_cover_art_size_db				= atoi(tok);	break; // "CoverArtSizeDB"
			case 9:		g_cover_art_size_playing		= atoi(tok);	break; // "CoverArtSizePlaying"
			case 10:	g_eop_action					= (enum eop_modes)atoi(tok);	break; // "EndOfPlaylistAction"

			// strings
			case 11:		strcpy(g_winamp_dir,			tok);			break; // "WinampDir"
			case 12:	strcpy(g_log_file,				tok);			break; // "LogFile"
			case 13:	strcpy(g_db_path,				tok);			break; // "DBPath"
			case 14:	strcpy(g_db_filelist,			tok);			break; // "DBFileList"
			case 15:	strcpy(g_ext_types,				tok);			break; // "Extensions"
			case 16:	strcpy(g_sc_server,				tok);			break; // "ShoutCastServer"
			case 17:	strcpy(g_winamp_class_name,		tok);			break; // "WinampClassName"
			case 18: 	strcpy(g_cover_art_filename,	tok);			break; // "CoverArtFilename"
			case 19:	strcpy(g_filler_stream_url,		tok);			break; // "FillerStreamURL"
			case 20:	strcpy(g_cover_art_root_dir,	tok);			break; // "CoverArtRootDir"
			case 21:	strcpy(g_html_include_file,		tok);			break; // "HTMLIncludeFile"

			case 22:	if (stricmp(tok,"ANY")) strcpy(g_dst_ip,tok);	break; // "IP"

			case 23:	strcpy(g_html_Title,		tok);				break; // "HTMLTitle"
			case 24:	strcpy(g_db_listspath,		tok);				break; // "ListsPath"
				
			case 25:	g_FileHlinkMode					= atoi(tok);	break; // 0=add 1=cue 2=cue & play now. Default 0=add
			case 26:	strcpy(g_NeroPath,		tok);				break; // "ListsPath"
			case 27:	strcpy(g_NeroBatPath,		tok);				break; // "ListsPath"

			default:	break;
			}
		}
	fclose(conf);

	// check to see if the page sizes are sane, and if they're not, make them so
	g_library_page_size = (g_library_page_size > 10000) ? 10000 : g_library_page_size;
	g_library_page_size = (g_library_page_size < 25) ? 25 : g_library_page_size;
	if (DEBUG) printf ( "g_library_page_size set to %d\n", g_library_page_size);
	if (DEBUG) printf ( "g_winamp_class_name to %s\n", g_winamp_class_name);


	// default for g_db_listspath: g_winamp_dir
	if (!g_db_listspath[0]) strcpy(g_db_listspath,g_winamp_dir);
	
	// append the g_db_listspath to g_db_path
	if (!strstr(g_db_path,g_db_listspath)) {
		strcat(g_db_path,";");
		strcat(g_db_path,g_db_listspath);
	}
	// add \\ to g_db_listspath to use as suffixed path
	if (g_db_listspath[strlen(g_db_listspath)-1]!='\\') strcat(g_db_listspath,"\\");

	tok=strstr(g_html_Title,HOSTNAME_PLACEHOLDER);
	if (tok) {
		*tok=0;
		strcpy(buffer, g_html_Title);
			strcpy(g_html_Title, szHostName);
			if (g_html_Title[0]>'Z') g_html_Title[0]-=32; //capitalize
			strcat(buffer, g_html_Title);
			strcat(buffer, tok+strlen(HOSTNAME_PLACEHOLDER));
		strcpy(g_html_Title,buffer);
		}

	// set up the log file
	if (!stricmp(g_log_file,"none") || !stricmp(g_log_file,"/dev/null") || strlen(g_log_file) < 1) g_log=0;
	else {
		if (!strstr(g_log_file,"\\")) {
			char a[MAX_PATH] = "";
			strcat(a, g_working_dir);
			strcat(a, g_log_file);
			strcpy(g_log_file, a);
			}
		}

	// set up the file types
	if (g_ext_types[0]) {
		char *p=g_ext_type_list;
		strcpy(g_ext_type_list,g_ext_types);
		g_ext_type_list[strlen(g_ext_type_list)+1]=0;
		while (p && *p) {
			p=strstr(p,";");
			if (p && *p) *p++=0;
			}
		}

	wsprintf( g_winamp_exe, "%s\\winamp.exe", g_winamp_dir );

	if(g_include_html!=0) free(g_include_html); // deallocate the previous text

	// load the include file, if it exists
	if(includeFile=fopen(g_html_include_file,"r")){
		struct _stat buf;
		long fileSize;
		int index=0;
		_stat(g_html_include_file, &buf );

		log_printf( "%s loading HTML include file: %s.\n", log_area, g_html_include_file);

		fileSize=buf.st_size;

		g_include_html=(char*)malloc(fileSize+1);
		while(!feof(includeFile)){
			index+=fread(g_include_html+index,1,fileSize,includeFile);
		}
		g_include_html[index]=0; // null-terminate the string after read from file

		fclose(includeFile);
	}
	
	log_printf( "%s Successfully Loaded Configuration.\n", log_area );
	return 0;
	}

unsigned int WINAPI HandleConnection(void *p) {
	cnx_inf *c=(cnx_inf *)p;
	int sock=(int)c->msgsock;
	int i=0;
	char req[1024];
	char user_agent[1024]="";
	char encodedlp[1024]="";
	char buf[1024];
	
	SetNonBlock(sock,1);
	if (recv_string(sock,req,sizeof(req)) > 2) {
		while (recv_string(sock,buf,sizeof(buf)) > 2) {
			buf[sizeof(buf)-1]=0;
			if (!strnicmp(buf,"User-Agent:",strlen("User-Agent:"))) {
				char *p=buf+strlen("User-Agent:");
				while (*p == ' ') p++;
				strncpy(user_agent,p,sizeof(user_agent)-1);
				user_agent[sizeof(user_agent)-1]=0;
			    }
			if (!strnicmp(buf,"Authorization: Basic ",strlen("Authorization: Basic "))) {
				char *p=buf+strlen("Authorization: Basic ");
				while (*p == ' ') p++;
				strncpy(encodedlp,p,sizeof(encodedlp)-1);
				encodedlp[sizeof(encodedlp)-1]=0;
				}
			}
		if (!strnicmp(req,"GET ",4)) {
			char *p=req+4,*p2,*p3;
			p3=p2=p;
			while (*p2 != ' ' && *p2) p2++;
			if (p2 == p) {
				p="/";
				}
			else {
				if (*p2) *p2=0;
				if (strstr(p,"//")) {
					p=strstr(p,"//");
					p+=3;
					while (*p != '/' && *p) p++;
					if (!*p) p="/";
					}
				}
			SetNonBlock(sock,0);
			http_handlereq(p, user_agent, encodedlp, sock, &c->sin);
			}
		}
	CloseSocket(sock);
	free(p);
	quitthread();
	return 0;
	}

static void parselist(char *out, char *in) {
	int nt=8, inquotes=0, neednull=0;
	char c;
	while (*in) {
		c=*in++;
		if (c >= 'A' && c <= 'Z') c+='a'-'A';
		if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
			neednull=1;
			*out++=c;
			}
		else if (c == '\"') {
			inquotes=!inquotes;
			if (!inquotes) {
				*out++=0;
				if (!nt--) break;
				neednull=0;
				}
			}
		else {
			if (inquotes) *out++=c;
			else if (neednull) {
				*out++=0;
				if (!nt--) break;
				neednull=0;
				}
			}
		}
	*out++=0;
	*out++=0;
	}

static int in_string(char *string, char *substring) {
#define LB 300
#define LS 50
	char text[LB];             
	char sub[LS];
	int i;

	int len = strlen(substring);
	int lenbig = strlen(string);

	if (lenbig<1) return 0;
	if (len<1) return 1;

	/* Convert string to upper case. */
  for(i = 0 ; (text[i] = toupper(string[i])) != '\0' && i<LB; i++);
  for(i = 0 ; (sub[i] = toupper(substring[i])) != '\0' && i<LS; i++);
  
  text[LB-1]=0;
  sub[LS-1]=0;

  return (strstr(text, sub) == NULL) ? 0 : 1 ;

/*	while (*string) {
		if ((string[0]|32) == (substring[0]|32)) 
			{
			if (len>=2) 
				{
				if (string[1]|32) == (substring[1]|32)) 
					{
					if (len==2 || !strnicmp(string+2,substring+2,len)) return 1;
					}
				}
			}
		;
		string++;
		}
	return 0;
*/
	}

static int substr_search(char *bigtext, char *littletext_list) { 
	while (*littletext_list) {
		if (!in_string(bigtext,littletext_list)) return 0;
		while (*littletext_list) littletext_list++;
		littletext_list++;
		}
	return 1;  
	}

/*void fixstring(char *in, char *out) {
	while (*in) {
		if ((*in >= 'A' && *in <= 'Z')||
			(*in >= 'a' && *in <= 'z')||
			(*in >= '0' && *in <= '9')) *out++=*in++;
		else {
			int i=*in++;
			wsprintf(out,"%%%02X",i);
			out += 3;
			}
		}
	*out=0;
	} 
*/

void unfixstring(char *in, char *out, int maxlen) {
	while (*in && maxlen) {
		if (*in == '%' && in[1] != '%' && in[1]) {
			int a=0;
			int b=0;
			for ( b = 0; b < 2; b ++) {
				int r=in[1+b];
				if (r>='0'&&r<='9') r-='0';
				else if (r>='a'&&r<='z') r-='a'-10;
				else if (r>='A'&&r<='Z') r-='A'-10;
				else break;
				a*=16;
				a+=r;
				}
			if (b < 2) *out++=*in++;
			else { 
				*out++=a; 
				in += 3;
				}
			}
		else *out++=*in++;
		maxlen--;
		}
	*out=0;
	}

/*char *getTimeStr(long alt_time) {
	static char timeret[50];
	int alt_hour, alt_min, alt_sec;
	alt_hour	= alt_time/3600;
	alt_min		= (alt_time % 3600) / 60;
	alt_sec		= alt_time % 60;
	if (alt_hour) wsprintf(timeret,"%dh&nbsp;%02dm&nbsp;%02ds", alt_hour, alt_min, alt_sec);
	else wsprintf(timeret,"%dm&nbsp;%02ds", alt_min, alt_sec);
	return timeret;
	}
*/
int getip(int which, struct sockaddr_in *sin) {
	if (which==3) return (sin->sin_addr.s_addr>>24);
	if (which==2) return ((sin->sin_addr.s_addr&0xff0000)>>16);
	if (which==1) return ((sin->sin_addr.s_addr&0xff00)>>8);
	return (sin->sin_addr.s_addr&0xff);
	}

char *gethost(struct sockaddr_in *sin) {
	static char hname[256];
	char ipaddr[16] = "";
	wsprintf(ipaddr, "%d.%d.%d.%d", getip(0,sin),getip(1,sin),getip(2,sin),getip(3,sin));
	if (g_perform_lookups) {
		int ip=inet_addr(ipaddr);
		struct hostent *p = gethostbyaddr((char *)&ip,4,AF_INET);
		if (p) wsprintf(hname,"%s",p->h_name);
		else wsprintf(hname, "%s", ipaddr); 
		}
	else wsprintf(hname, "%s", ipaddr);
	return hname;
	}

int recv_string(int s, char *str, int maxlen) {
	int sleeps=0;
	int p=0;
	do {
		int t;
		do {
			t=recv(s,str+p,1,0);
			if (t != 1) {
				if (!t || (t < 0 && WSAGetLastError() != WSAEWOULDBLOCK)) { str[p]=0; return -1; }
				Sleep(100);
				if (sleeps++ > 10*10) { str[p]=0; return -1; }
				}
			if (str[p] == '\r') t=0;
			} while (t!=1);
		} while (str[p] != '\n' && ++p < maxlen-1);
	str[p--]=0;
	while (str[p] == '\n' && p > 0) {
		str[p--]=0;
		}
	if (p < 0) p = 0;
	return p;
	}

void launchthread(void *threadproc, void *data) {
	_beginthread((void(*)(void*))threadproc,0,data);
	}

void quitthread() {
	_endthread();
	}

void SetNonBlock(int msgsock, u_long i) {
	ioctlsocket(msgsock,FIONBIO,&i);
	}

int OpenSocket(short port, int mc) {
	char *p=g_dst_ip;
	int  sock;
	struct sockaddr_in  sin;
  
	sock = socket(AF_INET,SOCK_STREAM,0);
	if (sock < 0) return -1;
  
	memset((char *) &sin, 0,sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons( port );
	if (*p)  sin.sin_addr.s_addr = inet_addr(p);
	if (!*p || !sin.sin_addr.s_addr) sin.sin_addr.s_addr=INADDR_ANY;
   
	if (bind (sock,(struct sockaddr *)&sin,sizeof(sin))) {
		int wsaErr=WSAGetLastError();
		CloseSocket(sock);
		return -wsaErr;
		}
  
	if (listen(sock,mc)==-1) {
		CloseSocket(sock);
		return -1;
		}
	return sock;
	}

void CloseSocket(int sock) {
	shutdown(sock, 2);
	closesocket(sock);
	}

int init_socketlib(int which) {
	WSADATA wsaData;
	if (which) {
		if ( WSAStartup( MAKEWORD(1, 1), &wsaData ) ) return -1;
		}
	else {
		WSACleanup();
		}
	return 0;
	}

char *extension(char *fn) {
	char *s = fn + strlen(fn);
	while (s >= fn && *s != '.' && *s != '\\') s--;
	if (s < fn) return "";
	if (*s == '\\') return "";
	return (s+1);
	}

void quit() {
	printf("Hit any key to close...");
	fflush(stdout);
	getch();
	exit(1);
	}

void log_printf(char *format, ...) { 
	char buf[1024]; 
	time_t t; 
	struct tm *tm; 
	va_list ar;
	va_start(ar,format);
	t = time(NULL);
	tm = localtime(&t);
	wsprintf(buf,"[%02d/%02d/%02d %02d:%02d:%02d] %s\n",
		tm->tm_mon+1, 
		tm->tm_mday, 
		tm->tm_year+1900, 
		tm->tm_hour, 
		tm->tm_min, 
		tm->tm_sec, 
		format);
	EnterCriticalSection(&log_mutex);
	if (g_log) {
		FILE *fp = fopen(g_log_file,"a+");
		if (fp) {
			vfprintf(fp,buf,ar);
			fclose(fp);
			}
		}
	vprintf(buf,ar);
	vsprintf(g_statusText, buf,ar);
	fflush(stdout);
	LeaveCriticalSection(&log_mutex);
	va_end(ar);
	}

static int in_list(char *list, char *v) {
	while (*list) {
		if (!stricmp(v,list)) return 1;
		list+=strlen(list)+1;
		}
	return 0;
	}

int sock_printf(int sock, char *fmt, ...) {
	char buffer[2048]={0,};
	char *p=buffer;
	int i;
	va_list ar;
	va_start(ar,fmt);
	vsprintf(buffer,fmt,ar);
	va_end(ar);
	i=strlen(buffer);
	while (i > 0) {
		int r=send(sock,p,i,0);
		if (r > 0) i-=r;
		else {
			if (r < 0) {
				if (WSAGetLastError() != WSAEWOULDBLOCK) break;
				else Sleep(250);
				}
			}
		}
	return strlen(buffer)-i;
	}

int sock_send(int sock, char *buffer,int size) {
	char *p=buffer;
	int i=size;
	while (i > 0) {
		int r=send(sock,p,i,0);
		if (r > 0) i-=r;
		else {
			if (r < 0) {
				if (WSAGetLastError() != WSAEWOULDBLOCK) break;
				else Sleep(250);
				}
			}
		}
	return size-i;
	}

BOOL DBFind(char* file, char* lp)
{
	int x;
	for (x = 0; x < database_used; x ++) 
		if ( stricmp(file,database[x].file)==0 
			&& stricmp(lp,database[x].leading_path)==0) return TRUE;
	return FALSE;
}

										
void DBAdd(char* file, char* lp, long daysold)
{
	if (database_used >= database_size) {
		if (database) {
			database_size*=2;
			database = (dbType*)realloc(database,database_size*sizeof(dbType));
			}
		else {
			database_size=32;
			database = (dbType*)malloc(database_size*sizeof(dbType));
			}
		}
	strcpy(database[database_used].file, file);
	strcpy(database[database_used].leading_path,lp);
	database[database_used].daysold=daysold;
	database[database_used].dbfound=0;
	database_used++;
}

void doRecursiveAddDB(char *leading_path) {
	char log_area[20] = "doadddb -->";
	char maskstr[MAX_PATH];
	//char file[MAX_PATH];
	WIN32_FIND_DATA d;
	HANDLE h;
	FILETIME now;
	SYSTEMTIME now_struct;
	//__int64 *i64now,*i64file;
	LARGE_INTEGER ula_file, ula_now;
	long daysold;
	const __int64 nano100SecInDay=(__int64)10000000*60*60*24;
	char leading_path_suffixed[MAX_PATH];

	wsprintf(maskstr,"%s%s",leading_path,leading_path[strlen(leading_path)-1]=='\\' ? "" : "\\");
	strcpy(leading_path_suffixed,maskstr);

	GetSystemTime(&now_struct);
	SystemTimeToFileTime(&now_struct,&now);
	ula_now.LowPart=now.dwLowDateTime;
	ula_now.HighPart=now.dwHighDateTime;
	//i64now = (__int64*)&now;
	//i64file= (__int64*)&d.ftCreationTime;

	//if (*this_path) {
	//	strcat(maskstr,this_path);
	//	if (this_path[strlen(this_path)-1]!='\\') strcat(maskstr,"\\");
	//	}
	strcat(maskstr,"*.*");
	h = FindFirstFile(maskstr,&d);
	//printf("Finding %s %s\n",maskstr,spacer);
	if ( h != INVALID_HANDLE_VALUE ) {
		do {
			if ( !(d.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
				// es un file
				char *p=extension(d.cFileName);
				if (*p && in_list(g_ext_type_list,p)) {
					ula_file.LowPart=d.ftLastWriteTime.dwLowDateTime ;
					ula_file.HighPart=d.ftLastWriteTime.dwHighDateTime ;
 					daysold = (long)((ula_now.QuadPart - ula_file.QuadPart) / (double)nano100SecInDay);
					DBAdd ( d.cFileName, leading_path_suffixed, daysold );
					if (!(database_used%16)) {
						int x;
						char s[1024];
						wsprintf(s,"Scanning: %s",d.cFileName);
						x=79-strlen(s); while (x-- > 0) strcat(s," ");
						s[79]=0;
						printf("%s\r",s);
						}
					g_count++;
					}
				}
			else {
				// es un directorio
				if (d.cFileName[0] != '.') {
					char ps[MAX_PATH];
					wsprintf(ps,"%s%s", leading_path_suffixed, d.cFileName);
					doRecursiveAddDB( ps );
					}
				}
			} while (FindNextFile(h,&d));
		FindClose(h);
		}
	}

/*
int _deepcompare( const char *name1, const char *name2, int debug ) {
	int dirCmp;
	char *st1 = strstr( name1, "\\" ), d1[MAX_PATH], *p1;
	char *st2 = strstr( name2, "\\" ), d2[MAX_PATH], *p2;
	if ( !st1 && !st2 ) // both are files 
	{
		return strnicmp( name1, name2, MAX_PATH ); //evaluate using stricmp.
	};
	if (  st1 && !st2 ) return 1;	// 1st file is a dir and 2nd isn't.  2nd gets precedence
	if ( !st1 &&  st2 ) return -1;	// 2nd file is a dir and 1st isn't.  1st gets precedence
	// now we are getting tricky...
	// 1st and 2nd are dirs, we need to compare the 2 dir names and 
	// if they are the same, recurse, else return the stricmp.
	strncpy( d1, name1, MAX_PATH); p1 = d1; while (p1 && *p1++) if (*p1 == '\\') *p1=0; // get 1st dir name in 2nd
	strncpy( d2, name2, MAX_PATH); p2 = d2; while (p2 && *p2++) if (*p2 == '\\') *p2=0; // get 1st dir name in 2nd
	dirCmp = strnicmp( d1, d2, MAX_PATH );
	if (dirCmp) return dirCmp;
	else return _deepcompare( ++st1, ++st2, debug );
	}
*/

int _compare( const dbType *arg1, const dbType *arg2 ) {
	// basically get the file name from the dbType objects, and pass to _deepcompare.
	int r=stricmp(arg1->leading_path,arg2->leading_path);
	if ( r==0 ) 
		if ( arg1->file[0]>='0' && arg1->file[0]<='9' &&
			arg2->file[0]>='0' && arg2->file[0]<='9' ) 
			return stricmp(arg1->file,arg2->file);
		else
			return arg1->daysold > arg2->daysold ? 1 : -1;
	return r;
	
/*	char file1[MAX_PATH];
	char file2[MAX_PATH];
	int test;
	strncpy(file1, arg1->file, MAX_PATH);
	strncpy(file2, arg2->file, MAX_PATH);
	test = _deepcompare( file1, file2, 0 );
	return test;
*/	}

void encodeLP(char *out, char *in) {
	char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int shift = 0;
	int accum = 0;

	while (*in) {
		if (*in) {
			accum <<= 8;
			shift += 8;
			accum |= *in++;
			}
		while ( shift >= 6 ) {
			shift -= 6;
			*out++ = alphabet[(accum >> shift) & 0x3F];
			}
		}
	if (shift == 4) {
		*out++ = alphabet[(accum & 0xF)<<2];
		*out++ = '=';  
		}
	else 
		if (shift == 2) {
			*out++ = alphabet[(accum & 0x3)<<4];
			*out++ = '=';  
    		*out++ = '=';  
			}
	*out++=0;
	}


void AddDBFileToList(HWND hwnd_wa, int x)
{
	char s[MAX_PATH];
	COPYDATASTRUCT cds;

	wsprintf( s, "%s%s", database[x].leading_path, database[x].file );
	cds.dwData = IPC_PLAYFILE; 
	cds.lpData = (void *) s; 
	cds.cbData = strlen((char *) cds.lpData)+1; 
	// Add found file to playlist
	SendMessage( hwnd_wa, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cds ); 
}


void makeDB() {
	char log_area[16]	= "[mLib]";
	char spacer[128]	= "                                             ";
	char *p;
	log_printf( "%s Building Media Library:", log_area );
	if (!g_db_path[0]) return;
	
	p=g_db_path;
	while ( p=strstr(p, ";") ) { // replace all ;'s with \0 to simulate null termination.
		*p++=0;
		p++; // double p++ is to avoid double 0 if double ';'
		}
	
	p=g_db_path;
	while (*p) {
		g_count=0;
		doRecursiveAddDB(p);
		log_printf( "%s -%6d file%s in %s%s", log_area, g_count, (g_count==1)?"":"s", p, spacer );
		p+=strlen(p)+1;
		}
	
	g_db_filelist_DbStart=database_used;

/*
	p=g_db_filelist;
	while ( p=strstr(p, ";") ) { // replace all ;'s with \0 to simulate null termination.
    	*p++=0;
		p++; // double p++ is to avoid double 0 if double ';'
		}
	
	p=g_db_filelist;
	while (*p) {
		FILE *inf=fopen(p,"rt");
		if (inf) {
			char buf[1024];
			char *lp;
			g_count=0;
			fgets(buf,1024,inf);
			while (buf[0] && (buf[strlen(buf)-1] == '\r' || buf[strlen(buf)-1] == '\n')) buf[strlen(buf)-1]=0;
			lp=strdup(buf); //must be freed when DB is re-created
			while (!feof(inf)) {
				fgets(buf,1024,inf);
				while (buf[0] && (buf[strlen(buf)-1] == '\r' || buf[strlen(buf)-1] == '\n')) buf[strlen(buf)-1]=0;
				if (buf[0] && !feof(inf)) {
					DBAdd(buf,lp);
					g_count++;
					}
				}
			log_printf("%s %d files added from %s", log_area, g_count, p );
			fclose(inf);
			}
		p+=strlen(p)+1;
		}
*/
	log_printf( "%s Sorting Database: ", log_area );
	qsort((void*)database, database_used, sizeof(dbType), (int(*)(const void *, const void *))_compare ); //)
	printf( " Successful.\n" );
	log_printf( "%s Media Library Contains %d File%s.", log_area, database_used, (database_used==1)?"":"s" );
	}


int IsPlaying(HWND hwnd_wa)
	{return SendMessage(hwnd_wa,WM_WA_IPC,0,IPC_ISPLAYING);}

void http_handlereq(char *url, char *user_agent, char *encodedlp, int sock, struct sockaddr_in *sin) {
	int midHeight				= 31;
	int isCE	 				= !!strstr(user_agent,"Windows CE");
	int isStyle					= !strncmp(url, STYLE_URL,		strlen(STYLE_URL) );
	int isJScript				= !strncmp(url, JSCRIPT_URL,	strlen(JSCRIPT_URL) );
	int isSpacer				= !strncmp(url, SPACER_URL,		strlen(SPACER_URL) );
	int isFolder				= !strncmp(url, FOLDER_URL,		strlen(FOLDER_URL) );
	int isControls				= !strncmp(url, CONTROL_URL,	strlen(CONTROL_URL) );
	int isControlsADM			= !strncmp(url, CONTROLADM_URL,	strlen(CONTROLADM_URL) );
	int isControlsBG			= !strncmp(url, CONTROLBG_URL,	strlen(CONTROLBG_URL) );
	int isRobots				= !strncmp(url, ROBOTS_URL,		strlen(ROBOTS_URL));
	int admreq					= !strncmp(url, ADMIN_URL,		strlen(ADMIN_URL));
	int is_adm					= admreq && (!g_admlp[0] ||  !stricmp( encodedlp, g_admlp ) || !stricmp( g_admlp, ":" ));
	char headHTTPOK[256]		= "HTTP/1.0 200 OK\r\n";
	char headServer[256]		= "Server: " SERV_NAME SERV_VER "\r\n";
	char headContentType[256]	= "Content-Type: text/html\r\n";
	char headNoCache[256]		= "Cache-Control: no-cache, must-revalidate\r\nPragma: no-cache\r\nExpires: -1\r\n";
	char headCache[256]			= "Cache-Control: max-age=31536000\r\n";
	char headStyle[256]			= "\t<link rel=\"stylesheet\" href=\"" STYLE_URL "\">\n";
	char *p						= strstr(url,"?");
	char *qmark					= strstr(url,"?");
	char log_area[16]			= "[http]";
	char thisurl[128]			= "/";

	char mode[256], opt[256], add[256], coverart[256], search[256], srchstart[256];

	int n;

	struct {
		char *key;	
		char *val;
		} httpgetdata[]= {
			{"m=",mode},
			{"o=",opt},
			{"a=",add},
			{"ca=",coverart},
			{"q=",search},
			{"ss=",srchstart},
		};

	//clear values
	for (int x = 0; x < sizeof(httpgetdata)/sizeof(httpgetdata[0]); httpgetdata[x++].val[0]=0);

	if (g_show_requests) log_printf( "%s %s \"%s\" \"%s\"", log_area, gethost(sin), url, user_agent ); //print out the url before we mangle it

	while (p && *p) { // if url has no ?, then this will not execute...
		int x;
		int index;
		for (x = 0; x < sizeof(httpgetdata)/sizeof(httpgetdata[0]) && strncmp( p+1, httpgetdata[x].key, strlen(httpgetdata[x].key) ); x++);
		if ( x < sizeof(httpgetdata)/sizeof(httpgetdata[0]) ) {
			char *p2=httpgetdata[x].val;
			*p2=0;
			strncpy(p2,p+strlen(httpgetdata[x].key)+1,256);
			index=0;
			while (index<=254 && *p2 && *p2 != '&') p2++, index++;
			*p2=0;
			}
		p=strstr(p+1,"&");
		}

	if ( strstr(mode,"#") ) { //halló "m=..."
		char *q;
		q = mode + strlen(mode);
		while (q >= mode && *q != '#') q--; *q=0;
		}

	if (is_adm) strcpy(thisurl, ADMIN_URL);
	
	if (isRobots) { // ROBOTS.TXT file to disallow server pages to be indexed.
		sock_printf( sock, "%s%sContent-type: text/plain\r\n\r\nUser-agent: *\r\nDisallow: /\r\n", headHTTPOK, headCache );
		return;
		}

	if (isStyle) { // STYLE SHEET
		char style_file[MAX_PATH];
		wsprintf( style_file,"%s%s.css", g_working_dir, g_working_name );
		httpgetFile( sock, style_file, "text/css", headCache );
		return;
		}
	if (isJScript) { // JAVA SCRIPT
		char jscript_file[MAX_PATH];
		wsprintf( jscript_file,"%s%s.js", g_working_dir, g_working_name );
		httpgetFile( sock, jscript_file, "application/x-javascript", headCache );
		return;
		}
	if (isSpacer || isFolder || isControls || isControlsADM || isControlsBG) { // IMAGES USED
		char control_file[MAX_PATH];
		if (isSpacer)		wsprintf( control_file,"%sspacer.gif",			g_working_dir );
		if (isFolder)		wsprintf( control_file,"%sfolder.gif",			g_working_dir );
		if (isControls)		wsprintf( control_file,"%scontrols.gif",		g_working_dir );
		if (isControlsADM)	wsprintf( control_file,"%scontrols.mp3j.gif",	g_working_dir );
		if (isControlsBG)	wsprintf( control_file,"%scontrols.bg.gif",		g_working_dir );
		httpgetFile( sock, control_file, "image/gif", headCache );
		return;
		}

	// check privledges...
	// do not put above downloaded files (css & js)
	if (!is_adm && ((encodedlp[0] && stricmp(encodedlp,g_acclp)) || admreq)) {
		sock_printf( sock, 
			"HTTP/1.0 401 Unauthorized\r\n" 
			"WWW-Authenticate: basic realm=\"" BRANDING "%s\"\r\n" 
			"content-type:text/plain\r\n\r\nAuthorization failed.\r\n"
			, admreq?" Administrator Login":"" );
		return;
		}

	// This bit of code validates that the user is at a correct url... either / or /mp3j
/*	if ( stricmp(url, thisurl) ) {
		httpgetFile( sock, "/dev/null", 0, headNoCache );
		return;
		}
*/

	// mode is a string... if we check the 0th byte and it's 0 (null) then
	// the string is empty...   and the if will fail, but we ! it so we it passes
	// the only time mode should be empty is on the initial request (or if a user hits reload)
	if (qmark==0)  /*(!mode[0] && !opt[0] && !coverart[0])  */
		{
		//if (g_show_requests) log_printf( "\n\nFRAME PAGE\n\n" ); //print out the url before we mangle it

		sock_printf( sock, "%s%s%s\r\n"
			"<HTML>\n"
			"<HEAD>\n"
			"\t<TITLE>%s</TITLE>\n%s"
			,headHTTPOK, headServer, headCache, g_html_Title,!isCE?headStyle:"");

		sock_printf( sock, 	"</HEAD>\n\n"
			"<FRAMESET ROWS=\"%d,50%%,1,50%%\" FRAMEBORDER=\"NO\" FRAMESPACING=\"0\" BORDER=\"0\">\n"
			"\t<FRAME SRC=\"%s?m=control\" NAME=\"wwControl\" SCROLLING=\"NO\">\n"
			"\t<FRAME SRC=\"%s%s\" NAME=\"wwLeft\">\n"
			"\t<FRAME NAME=\"separ\" ID=separ >\n"
			"\t<FRAME SRC=\"%s?m=right\" NAME=\"wwRight\">\n"
			"</FRAMESET>\n\n</HTML>\n",
			  midHeight, thisurl, thisurl, PLAYLIST_DEFAULT, thisurl );
		return;
		}
	else { // mode is not null... 
		// set up main window window handler
		HWND hwnd_wa = FindWindow( g_winamp_class_name, NULL );
		int track=atoi(add);  // r is the track to be operated on

		if ( DEBUG ) printf ("%s looking for window %s\n", g_debug, g_winamp_class_name);
		
		if ( opt[0] ) 
			{ // //halló "o=xxx" => OPTION  
			// ALL ACTIONS WILL REDIRECT USER BACK TO PLAYLIST WINDOW

			if (hwnd_wa) 
				{ // Check to see if we have a winamp handle
				
				// set up playlist window handler.  Move to this section to be executed only when needed.
				// The orginal code only supported one on instance of winamp
				//HWND hwnd_pe = FindWindowEx( hwnd_wa, NULL, "Winamp PE", NULL );
				// One would think the following piece of code would support, however, the winamp playlist is 
				// not a child of the winamp main window.
				//HWND hwnd_pe = FindWindowEx( hwnd_wa, NULL, "Winamp PE", NULL );
				// Instead, we have to cycle thru all the windows from the top until we find the Playlist 
				// window with the same process id as the main window (they are the same).
				HWND hwnd_pe=0;
				HWND hwnd_top = GetDesktopWindow();

				while ( hwnd_pe = FindWindowEx( hwnd_top, hwnd_pe, "Winamp PE", NULL) ) {
					DWORD id1, id2; // create ID insances
					GetWindowThreadProcessId( hwnd_wa, &id1 );  // get the process id
					GetWindowThreadProcessId( hwnd_pe, &id2 );
					if ( DEBUG ) printf ("%s found playlist, checking proccess ids ID: %d ID: %d\n", g_debug, id1, id2);
					if (id1==id2) break;
					}
				// PHEW! we should have a valie winamp pe window handle.

				if		( !stricmp(opt,"rew")    && is_adm)	SendMessage( hwnd_wa, WM_COMMAND, WINAMP_BUTTON1, 0);
				else if ( !stricmp(opt,"pl")     && is_adm)	SendMessage( hwnd_wa, WM_COMMAND, WINAMP_BUTTON2, 0); // play would take it out of standby mode
				else if ( !stricmp(opt,"pau")    && is_adm)	SendMessage( hwnd_wa, WM_COMMAND, WINAMP_BUTTON3, 0);
				else if ( !stricmp(opt,"unp")    && is_adm)	SendMessage( hwnd_wa, WM_COMMAND, WINAMP_BUTTON3, 0);
				else if ( !stricmp(opt,"st")     && is_adm)	SendMessage( hwnd_wa, WM_COMMAND, WINAMP_BUTTON4, 0);
				else if ( !stricmp(opt,"for")	&& is_adm)	SendMessage( hwnd_wa, WM_COMMAND, WINAMP_BUTTON5, 0);
				else if ( !stricmp(opt,"clear")	&& is_adm)	{
								SendMessage( hwnd_wa, WM_WA_IPC, g_base_orden=0, IPC_DELETE );	// clear playlist
								g_last_track=-1;
								}
				else if ( !stricmp(opt,"random")	&& is_adm)	SendMessage( hwnd_pe, WM_COMMAND, IPC_PE_RANDOMIZE, 0); // got this undocumented feature from tag

				else if ( !stricmp(opt,"d")      && is_adm)	SendMessage( hwnd_pe, WM_USER, 104, track); //delete

				else if ( !stricmp(opt,"up")      && is_adm)	{
						if (track) SendMessage( hwnd_pe, WM_USER, IPC_PE_SWAPINDEX, (track<<16)|(track-1));
						g_selected=track-1;
						if ((g_position=g_selected-6)<0) g_position=0;

						}							

				else if ( !stricmp(opt,"dn")      && is_adm)	{
						SendMessage( hwnd_pe, WM_USER, IPC_PE_SWAPINDEX, (track<<16)|(track+1));
						g_selected=track+1;
						if ((g_position=g_selected-6)<0) g_position=0;
						}							

				else if ( !stricmp(opt,"setOrd")      && is_adm)	{
						int numtracks=SendMessage( hwnd_wa, WM_WA_IPC, 0, IPC_GETLISTLENGTH );
						SendMessage( hwnd_pe, WM_USER, IPC_PE_SWAPINDEX, (track<<16)|(g_base_orden));
						if (++g_base_orden>numtracks) g_base_orden=numtracks;
						g_selected=track;
						if ((g_position=g_selected-6)<0) g_position=0;
						}							

				else if ( !stricmp(opt,"QUIT")   && is_adm)	{
						system("RUNDLL32 KRNL386.EXE,exitkernel");
						exit(1);
						}

				else if ( !stricmp(opt,"upPL")   && is_adm)	log_printf( "[list] Updating Playlist" );	
				else if ( !stricmp(opt,"upML")   && is_adm)	{ // reload database
					// can't do that cuz they will change ports :P or other stupid things users do
					// LoadConfig(); // purposely ignore harmless errors
					int x;
					EnterCriticalSection(&library_mutex);
					for ( x = 0; x < database_used; x++ ) {
						database[x].file[0]=0;
						database[x].leading_path[0]=0;
						database[x].dbfound=0;
						}
					database_used=0;
					makeDB();
					LeaveCriticalSection(&library_mutex);
					}

				else if ( !stricmp(opt,"SaveList") && is_adm) { // save list
					// save the winamp playlist....

					char searchstring[1024];
					unfixstring(search,searchstring,1024);
					
					if (g_show_requests) log_printf( "Saving List %s",searchstring ); 

					// if i have winamp, and listanme is not null and is not *
					if (searchstring[0] && hwnd_wa && stricmp(searchstring,"*") ) 
						{
						char fn[MAX_PATH];
						char fn2[MAX_PATH];
						EnterCriticalSection(&playlist_mutex);
						SendMessage( hwnd_wa, WM_WA_IPC, 0, IPC_WRITEPLAYLIST );
						wsprintf( fn, "%s\\winamp.m3u", g_winamp_dir );
						wsprintf( fn2, "%s%s.m3u", g_db_listspath, searchstring );
						remove(fn2);
						rename(fn, fn2);
						LeaveCriticalSection(&playlist_mutex);

						//remember list name
						strcpy(g_Last_List,searchstring);

						//add new list to database
						wsprintf( fn2, "%s.m3u", searchstring );
						if (!DBFind(fn2,g_db_listspath)) DBAdd(fn2,g_db_listspath,0);
						} 
				}

				else if ( !stricmp(opt,"order") && is_adm) {  // Order the list
						g_order_mode = !g_order_mode;
						g_selected=-1;
						}

				else if ( !stricmp(opt,"FixList") && is_adm) { 
				/*	A broken .m3u list will occur if you move the mp3 files
					to another directory. Fix the list means  
					searching every file in the actual list	into the database 
					and if found add it to the list with the correct path.
				*/
					if (hwnd_wa) { //if Winamp is open
						char fn[MAX_PATH];
						FILE *fp;

						EnterCriticalSection(&playlist_mutex);
						// Ask Winamp to write playlist....
						SendMessage( hwnd_wa, WM_WA_IPC, 0, IPC_WRITEPLAYLIST );

						//if (g_show_requests) log_printf( "FIX List Start\n" ); //show info

						wsprintf( fn, "%s\\winamp.m3u", g_winamp_dir );
						fp = fopen(fn, "rt");
						if (fp) {
							char tempID3[MAX_PATH];
							
							// Clear playlist....
							SendMessage( hwnd_wa, WM_WA_IPC, g_base_orden=0, IPC_DELETE );

							while (1) {
								int found=0;
								char *lastfound=0;
								int x;
								char *p;
								char *tp;
								fgets(fn,sizeof(fn),fp);
								if (feof(fp)) break;

								p=fn;
								if (p[0] && p[strlen(p)-1] == '\n') p[strlen(p)-1]=0;

								while (*p == ' ') p++;
								if (*p == '#') {
									if ( !strncmp(p,"#EXTINF",7) ) {
										while (*p != ',') p++;
										strcpy(tempID3,++p);
										}
									continue;
									}

								// get the file name only....
								p=fn+strlen(fn);
								while (*p != '\\' && *p != '/' && p >= fn) p--;
								if (p[0]) p++;
								tp=p;
								while (*tp) tp++;
								while (tp > p && *tp != '.' && *tp != '\\' && *tp != '/') tp--;
								if (tp > p && *tp == '.') *tp=0;

								// search for it in the database
								for (x = 0; x < database_used || found; x ++) {
									found=(0!=strstr(database[x].file,p));
									if (found) 
										if (lastfound==0 || stricmp(lastfound,database[x].file)!=0)
											{
											lastfound=database[x].file;
											AddDBFileToList(hwnd_wa,x);
											}
								}
							}
						}
						fclose(fp);          
						log_printf("Winamp List Fixed");
						//if (g_show_requests) log_printf( "FIX List End\n" ); //show info
						LeaveCriticalSection(&playlist_mutex);
					} 
				}

				else if ( !stricmp(opt,"Duplicates") && is_adm) { 
				/*	Search for duplicates
				*/
					if (hwnd_wa) 
						{ //if Winamp is open

						int base,x,found;
						int baseadded;

						EnterCriticalSection(&playlist_mutex);
						// Clear playlist....
						SendMessage( hwnd_wa, WM_WA_IPC, g_base_orden=0, IPC_DELETE );
						
						g_showfile = 1;
						
						if (g_show_requests) log_printf( "Duplicates List Start\n" ); //show info

						// clear dbfound flag
						for (base = 0; base < database_used;) database[base++].dbfound=0;
						// for each file in db search for duplicates in the database
						for (base = 0; base < database_used ; base++) 
								{
								if ( !database[base].dbfound && strstr(database[base].file,".m3u")==0 )
								   {
    								baseadded=0;
								    // search for it in the database
    								for (x = base+1; x < database_used ; x++) 
                                           {
        									found=(0!=strstr(database[base].file,database[x].file));
        									if (found) 
        										{
        										if (!baseadded) 
        												{ 
        												baseadded=1;
        												AddDBFileToList(hwnd_wa,base); 
        												log_printf( "Dup Found for %s\n",database[base].file ); //show info
        												}
        										AddDBFileToList(hwnd_wa,x);
        										database[x].dbfound=1;
                                                }
    										}
                                      }
								}
						log_printf( "Duplicates List created\n" ); //show info
						LeaveCriticalSection(&playlist_mutex);
						} 
				}

				else if ( !stricmp(opt,"GrabarCD") && is_adm) { 
				/*	Se genera un .BAT para copiar los archivos a un path predeterminado 
					se genera un .m3u para que tome el Nero 5.5 el orden de las canciones
					****
				*/
					if (hwnd_wa) { //if Winamp is open
						char tempID3[MAX_PATH];
						char fn[MAX_PATH];
						char fnw[MAX_PATH];
						char fnx[MAX_PATH];
						FILE *fp,*fw,*fx;

						// Ask Winamp to write playlist....
						SendMessage( hwnd_wa, WM_WA_IPC, 0, IPC_WRITEPLAYLIST );

						//if (g_show_requests) log_printf( "FIX List Start\n" ); //show info

						wsprintf( fn, "%s\\winamp.m3u", g_winamp_dir );
						fp = fopen(fn, "rt");
						if (fp) {

							wsprintf( fnw, "%s\\0.LISTA.m3u", g_NeroBatPath );
							log_printf("Writing %s",fnw);
							fw = fopen(fnw, "wt");
							if (!fw) {
								log_printf("%s :%s",fnw,strerror(errno));
								goto finGrabaCD;
								}

							wsprintf( fnx, "%s\\0.COPIAR.bat", g_NeroBatPath );
							log_printf("Writing %s",fnx);
							fx = fopen(fnx, "wt");
							if (!fx) {
								log_printf("%s :%s",fnx,strerror(errno));
								fclose (fw);
								goto finGrabaCD;
								};

							while (1) {
								char *p;
								fgets(fn,sizeof(fn),fp);
								if (feof(fp)) break;

								p=fn;
								if (p[0] && p[strlen(p)-1] == '\n') p[strlen(p)-1]=0;

								while (*p == ' ') p++;
								if (*p == '#') {
									if ( !strncmp(p,"#EXTINF",7) ) {
										while (*p != ',') p++;
										strcpy(tempID3,++p);
										}
									continue;
									}

								fprintf( fx, "xcopy /d/i/y \"%s\" \"%s\"\n", p, g_NeroPath );

								// get the file name only....
								p=fn+strlen(fn);
								while (*p != '\\' && *p != '/' && p >= fn) p--;
								if (p[0]) p++;
								fprintf( fw, "%s\n", p );

							}
						}

						fprintf( fx, "xcopy /y \"%s\" \"%s\"\n", fnw, g_NeroPath );

						fclose(fw);          
						fclose(fx);          
					finGrabaCD:				
						fclose(fp);          
					} 
				}

				else if ( !stricmp(opt,"j") && is_adm) {
					g_playing_standby=0; // jumped to a file, so we're no longer in standby mode
					SendMessage( hwnd_wa, WM_WA_IPC, g_base_orden=track, IPC_SETPLAYLISTPOS );
					SendMessage( hwnd_wa, WM_COMMAND, WINAMP_BUTTON2, 0 );
					if ((g_position=track-3)<0) g_position=0;
					}
				else { // cue or add track or album
					if ( 0 <= track && track < database_used ) {
						char s[MAX_PATH];
						char *ext;
						char file[MAX_PATH];
						char *lp=database[track].leading_path;
						int n;
						strcpy(file,database[track].file);
						wsprintf( s, "%s%s", lp, file );
						if (g_show_requests) log_printf( "add/cue %s", s ); //show info

						// don't add a track that is the same as the very last track queued, unless winamp isn't playing anything 
						// only checks for files added this section, needs to be reworked so that it checks existing items.
						if ( (track!=g_last_track) || !IsPlaying(hwnd_wa) ) {
							COPYDATASTRUCT cds;
							g_last_track = track;  // save the index of the last track 

							// if we're in standby mode, and adding a track, then we want to clear the current playlist
							if (g_playing_standby) SendMessage( hwnd_wa, WM_WA_IPC, g_base_orden=0, IPC_DELETE );	// clear playlist standby song.

							if (!stricmp(opt,"cue") && is_adm) {
								// if list, clear actual before adding, and remember name
								if (ext=strstr(file,".m3u")) {
									SendMessage( hwnd_wa, WM_COMMAND, WINAMP_BUTTON4, 0); // Stop
									SendMessage( hwnd_wa, WM_WA_IPC, g_base_orden=0, IPC_DELETE); // clear
									for (n=0;n++;n<30000 && IsPlaying(hwnd_wa)) ; //is playing?
									cds.dwData = IPC_PLAYFILE; 
									cds.lpData = (void *) s; 
									cds.cbData = strlen((char *) cds.lpData)+1; 
									SendMessage( hwnd_wa, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cds ); //add
									SendMessage( hwnd_wa, WM_COMMAND, WINAMP_BUTTON2, 0 ); //play
									*search=0; //borro "now" para que no haga "fwd"
									//remember list name
									*ext=0;
									strcpy(g_Last_List,file);
									}
								else
								{
									fileinfo song;
									strcpy(song.file, s);
									song.index = SendMessage( hwnd_wa, WM_WA_IPC, 0, 120 );
									song.index += 1;
									cds.dwData = IPC_PE_INSERTFILENAME;
									cds.lpData = (void *) &song;
									cds.cbData = sizeof(fileinfo);
									SendMessage(hwnd_pe,  WM_COPYDATA, 0, (LPARAM) &cds );
								};

								if (!stricmp(search,"now")) {
										SendMessage( hwnd_wa, WM_COMMAND, WINAMP_BUTTON5, 0);
										if ((g_position=track-3)<0) g_position=0;
									}
								}

							if (!stricmp(opt,"add") ) { 
								cds.dwData = IPC_PLAYFILE; 
								cds.lpData = (void *) s; 
								cds.cbData = strlen((char *) cds.lpData)+1; 
								SendMessage( hwnd_wa, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cds ); //add
							} 
							if (!stricmp(opt,"addd") ) { // add directory to playlist

								int x;
								cds.dwData = IPC_PLAYFILE; 
								cds.lpData = (void *) s; 
								log_printf( "add dir %s %s", lp, file  ); //show info
								for (x = 0; x < database_used; x ++) {
									if (database[x].dbfound!=0 && stricmp(lp,database[x].leading_path)==0) {
										wsprintf( s, "%s%s", database[x].leading_path, database[x].file );	
										//log_printf( "item add dir %s %s \n", database[x].leading_path, database[x].file  ); //show info
										cds.cbData = strlen((char *) cds.lpData)+1; 
										SendMessage( hwnd_wa, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cds ); //add
									}
								}
								/*
								char pathArg[MAX_PATH];
								char dir[MAX_PATH];
								char *p;
								strncpy( dir, s, MAX_PATH );
								p = dir + strlen(dir);
								while (p >= dir && *p != '\\') p--;
								*p=0;
								wsprintf(pathArg," /CLASS=\"%s\" /ADD \"%s\"", g_winamp_class_name, dir );
								_spawnl( _P_NOWAIT, g_winamp_exe, pathArg, NULL ); // add an entire folder and all subfolders
								*/
								}

							// hit play   we'll come back to this later :P 
							// if ( !SendMessage( hwnd_wa, WM_WA_IPC, 0, IPC_ISPLAYING ) ) SendMessage( hwnd_wa, WM_WA_IPC, 0, IPC_STARTPLAY );
							
							// if it's still playing the standby track
							// just skip to new track
							if (g_playing_standby) 	SendMessage( hwnd_wa, WM_WA_IPC, 0, IPC_STARTPLAY ); // start playing newly added track
							g_playing_standby=0; // move out of standby mode, if we were in it

							}
						else{ 
							log_printf( "%s Attempted to cue or add duplicate track or folder: %s", log_area, s ); 
							} 
						}
					}
				}
			else 
				{ // no handler exists.  Winamp is not open.  
				if ( !stricmp(opt,"swa") && is_adm ) 
					{
					// if the option is to start winamp, then we attempt to start winamp.
					// we call winamp.mpu as a parameter so that winamp automatically starts playing.
					// autoplaying prevents having to click twice in the browser window.
					STARTUPINFO startWAinfo={sizeof(startWAinfo),};
					PROCESS_INFORMATION processWAinfo;
					char startWA[1024];
					wsprintf( startWA, "\"%s\" /class=\"%s\" \"%s\\winamp.m3u\"", g_winamp_exe, g_winamp_class_name, g_winamp_dir );
					log_printf("[mp3j] Executing %s",startWA);
					CreateProcess( NULL, startWA, NULL, NULL, 0, 0, NULL, g_winamp_dir, &startWAinfo, &processWAinfo );
					}
				} //end opcion start

			// this type of redirect doesn't support time out refreshing. we want to wait a second 
			// so Winamp updates the playlist after any operations on it
			sock_printf( sock, 
				"HTTP/1.0 302 Found\r\n"
				"Content-type:text/html\r\n"
				"Server: " SERV_NAME SERV_VER "\r\n"
				"Location: %s%s\r\n\r\n"
				"<HTML>\r\n"
				"<HEAD>\r\n\t"
				"<TITLE>Redirect</TITLE>\r\n"
				"<meta http-equiv=\"refresh\" content=\"2; url=%s%s\">"
				"</HEAD>\r\n\r\n"
				"<BODY>\r\n"
				"Loading Winamp<br>Click <a href=\"%s%s\">here</A> for redirect.\r\n"
				"</BODY>\r\n</HTML>\r\n"
				, thisurl, PLAYLIST_DEFAULT, thisurl, PLAYLIST_DEFAULT, thisurl, PLAYLIST_DEFAULT );
			} // fin o=xxx => OPTION

		else 
			{
			if (coverart[0]) 
				{
					char coverArtFile[MAX_PATH];
					char s[MAX_PATH];
					char *p;

					// if we are doing a media list coverart, then get the url from the dbType database.
					if (!stricmp( coverart, "ML" )) wsprintf( s, "%s%s", database[track].leading_path, database[track].file );
					// else we need to get the full file name from wwwinamp, and the IPC_GETPLAYLISTFILE option only works using plugins, not external apps. :(
					if (!stricmp( coverart, "PL" )) {
						FILE *fp;
						int cursong	= SendMessage( hwnd_wa, WM_WA_IPC, 0, IPC_WRITEPLAYLIST );
						int x = 0;
						char fn[MAX_PATH];
						wsprintf( fn, "%s\\winamp.m3u", g_winamp_dir );
						fp = fopen( fn, "rt" );
						if (fp && hwnd_wa) {
							while (1) {
								char *fnp;
								fgets(fn,sizeof(fn),fp);
								if (feof(fp)) break;
								fnp=fn;
								if (fnp[0] && fnp[strlen(fnp)-1] == '\n') fnp[strlen(fnp)-1]=0;
								while (*fnp == ' ') fnp++;
								if (*fnp == '#') continue;
								if ( x == track) {
									strncpy( s, fnp, MAX_PATH );
									break;
									}
								x++;
								}
							}
						}
					// take the actual file name, and then parse out the directory name from that.
					p = s + strlen(s);
					while (p >= s && *p != '\\') p--;
					*++p=0;
					// fetch the cover art
					wsprintf( coverArtFile, "%s%s", s, g_cover_art_filename );
					httpgetFile( sock, coverArtFile, "image/jpeg", headCache );
					return;
				}

			else { // MODE. no option, not an action
			   // mode is not null, but add and opt are... so we are simply displaying a page...

				// http protocol stuff.
				sock_printf( sock, "%s%s%s\r\n", headHTTPOK, headServer, headNoCache );

				// html stuff.
				sock_printf( sock, "<HTML>\n<HEAD>\n%s", !isCE ? headStyle : "");
				if ( !stricmp(mode,"left") ) {
					if ( g_left_refresh ) sock_printf(sock,"\t<META HTTP-EQUIV=\"refresh\" CONTENT=\"%d; url=%s?m=left&q=ref&rnd=%d#playing\">\n", g_left_refresh, thisurl, time(0) );
					// sock_printf(sock,"\t<script language=\"JavaScript\" src=\"" JSCRIPT_URL "\"></script>\r\n");
					}
				sock_printf(sock, "</HEAD>\n\n<BODY CLASS=\"WWWinamp_%s\">\n\n", mode);

				if (!stricmp(mode,"left")) {
					// find the winamp playlist....
					if (hwnd_wa) {
						int cursong,showfrom;
						FILE *fp;
						EnterCriticalSection(&playlist_mutex);
						cursong	= SendMessage( hwnd_wa, WM_WA_IPC, 0, IPC_WRITEPLAYLIST );
						if (!g_order_mode || g_position==-1) {
							if ((g_position=cursong-3)<0) g_position=0;
							}
						
						//MS IExplorer refresh does not work fine with anchors (#playing)
						//I'll not show more than 3 previous songs from the current 
						//showfrom=g_position;
						//if (!stricmp(searchstring,"all")) showfrom=0; //if ?q=all show all items
						showfrom=0;
						if (!stricmp(search,"ref")) showfrom=g_position; //if ?q=ref do not show all items
						if (g_order_mode) showfrom=0;
						
						char fnList[1024];
						wsprintf(fnList, "%s\\winamp.m3u", g_winamp_dir );
						fp = fopen( fnList, "rt" );
						if (fp) {
							int x=0;
							int numtracks	= SendMessage( hwnd_wa, WM_WA_IPC, 0, IPC_GETLISTLENGTH );
							int ps			= IsPlaying(hwnd_wa);
							int listlen = numtracks - cursong;
							char tempID3[MAX_PATH];
							
							if (listlen==1 && ps==0) { // STANDBY MODE
								// if one track left, but stopped (i.e., after last song), either
								// play a random track
								if ( g_eop_action == eop_mode_random ) { 
									int r = (int)(database_used * (int)myrand() / (double)(RAND_MAX+1));
									char s[MAX_PATH];
									COPYDATASTRUCT cds;
									
									wsprintf( s, "%s%s", database[r].leading_path, database[r].file );
									cds.dwData = IPC_PLAYFILE;
									cds.lpData = (void *) s;
									cds.cbData = strlen((char *) cds.lpData)+1;
									SendMessage( hwnd_wa, WM_WA_IPC, g_base_orden=0, IPC_DELETE );		// clear playlist
									SendMessage( hwnd_wa, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cds );	// add new track
									//SendMessage( hwnd_wa, WM_WA_IPC, 1, IPC_SETPLAYLISTPOS );			// skip to the new track
									SendMessage( hwnd_wa, WM_WA_IPC, 0, IPC_STARTPLAY );				// play track
									g_playing_standby=1;
									}
								else { // or play the filler stream
									if ( g_eop_action == eop_mode_stream ) { 
										char pathArg[1024]; 
										 wsprintf( pathArg, " /CLASS=\"%s\" %s", g_winamp_class_name, g_filler_stream_url ); 
										_spawnl( _P_NOWAIT, g_winamp_exe, pathArg, NULL ); 
										g_playing_standby=1;
										}
									}
								}

							// display the "include" file
							if (g_include_html) sock_printf( sock, "<div class=\"include\"%s</div>\n<p>",g_include_html );

							sock_printf(sock,"<pre>");

							while (1) {
								char tmp[1024];
								char buf[1024];
								char *p;
								fgets(buf,sizeof(buf),fp);
								if (feof(fp)) break;
								p=buf;
								if (p[0] && p[strlen(p)-1] == '\n') p[strlen(p)-1]=0;
								while (*p == ' ') p++;
								if (*p == '#') {
									if ( !strncmp(p,"#EXTINF",7) ) {
										while (*p != ',') p++;
										strcpy(tempID3,++p);
										}
									continue;
									}
								if ( tempID3[0] ) {
									strcpy(tmp,tempID3);
									tempID3[0]=0;
									}
								else {
									char *tp;
									p=buf+strlen(buf);
									while (*p != '\\' && *p != '/' && p >= buf) p--;
									while (*p != '\\' && *p != '/' && p >= buf) p--;
									if (p[0]) p++;
									tp=p;
									while (*tp) tp++;
									while (tp > p && *tp != '.' && *tp != '\\' && *tp != '/') tp--;
									if (tp > p && *tp == '.') *tp=0;
									unfixstring(p,tmp,sizeof(tmp));
									}
								
								if (x && x==showfrom)
									sock_printf( sock, "<a href=\"%s?m=left#playing\">&lt&lt " Show_Previous "</a>\n", thisurl);

								if (x >= showfrom) {
								
								if (x == cursong) {
									sock_printf( sock, "<table class=\"OuterCurrentBox\"><tr><td><table class=\"CurrentBox\"><tr><td>");
									if(g_show_cover_art && hasCoverArt(buf) ) sock_printf( sock, 
										"<img src=\"%s?ca=PL&a=%d\" width=\"%d\" height=\"%d\" border=\"0\">"
										, thisurl, x, g_cover_art_size_playing, g_cover_art_size_playing );
									sock_printf( sock, "</td><td class=\"Current\"><a name=%s>%d.</a> ", (x==g_position)?"playing":"current", x+1 );
									if (g_order_mode) sock_printf( sock, "[<a href=\"%s?o=up&a=%d\">^</a>-<a href=\"%s?o=dn&a=%d\">v</a> <a href=\"%s?o=setOrd&a=%d\">(%d)</a>] ", thisurl,x, thisurl, x, thisurl, x , g_base_orden+1 );
									sock_printf( sock, Now_Playing ":<br><b class=\"CurrentSong\">%s</b><br>", tmp );
									sock_printf( sock, "<b class=\"CurrentSong\">%s</b><br>", buf );
									switch ( IsPlaying(hwnd_wa) ) {
										case 0: sock_printf(sock,"Winamp " is_ stopped); break;
										case 1: sock_printf(sock,"Winamp " is_ playing); break;
										case 3: sock_printf(sock,"Winamp " is_ paused);  break;
										default: break;
										}
									sock_printf( sock, ", <b>%d " of_ " %d " file_ "%s</b> " left_in_queue ".</td></tr></table></td></tr></table>", listlen, numtracks, numtracks==1?"":"s");
									}
								else {
									if (!isCE || (isCE && (x > cursong) && (x < cursong + 10))) {
										if (x==g_position) sock_printf(sock,"<a name=playing></a>");
										if (x==g_selected) sock_printf(sock,"<span style=\"{background-color:blue;}\">");
										sock_printf( sock,"% 6d. ", x+1 );
										if (is_adm) {
											sock_printf( sock, "[<a href=\"%s?o=d&a=%d\">d</a>]", thisurl, x );
											if (g_order_mode) sock_printf( sock, "[<a href=\"%s?o=up&a=%d\">^</a>-<a href=\"%s?o=dn&a=%d\">v</a> <a href=\"%s?o=setOrd&a=%d\">(%d)</a>]", thisurl,x, thisurl, x, thisurl, x , g_base_orden+1 );
											sock_printf( sock, "<a href=\"%s?o=j&a=%d\"> ", thisurl, x );
											}
										if (g_showfile)
											sock_printf( sock, "%s %s", tmp, buf );
										else 
											sock_printf( sock, "%s", tmp );

										if (is_adm) sock_printf( sock, "</a>");
										if (x==g_selected) sock_printf(sock,"         </span>");
										sock_printf( sock, "\n");
										}
									}
								}
								x++;
								}
							sock_printf(sock,"</pre>");
							fclose(fp);          
							LeaveCriticalSection(&playlist_mutex);
							} 
						else if (!isCE) sock_printf(sock,"<B>" No_Winamp_playlist_found "</B><br><br>");
						}
					else {
						sock_printf( sock, "%s", !isCE?"<div class='search'>" No_instance_Winamp_was_found_running ".<br>":"No Winamp - " );
						if (is_adm) sock_printf( sock, "[ <a href=\"%s?o=swa\">%s</a> ]\n\n",thisurl, !isCE? Try_starting_Winamp:"start" );
						sock_printf( sock, "%s", !isCE?"</div>":"" );
						}
					}

				else if (stricmp(mode,"vUp")==0) {
						if (is_adm) SendMessage( hwnd_wa, WM_COMMAND, WINAMP_VOLUMEUP, 0); 
						}

				else if (stricmp(mode,"vDn")==0) {
						if (is_adm) SendMessage( hwnd_wa, WM_COMMAND, WINAMP_VOLUMEDOWN, 0); 
						}

				else if (stricmp(mode,"vUpUp")==0) {
						if (is_adm) for (n=0;n<5;n++) SendMessage( hwnd_wa, WM_COMMAND, WINAMP_VOLUMEUP, 0); 
						}

				else if (stricmp(mode,"vDnDn")==0) {
						if (is_adm) for (n=0;n<5;n++) SendMessage( hwnd_wa, WM_COMMAND, WINAMP_VOLUMEDOWN, 0); 
						}

				else if (!stricmp(mode,"control")) {
						sock_printf( sock, "<base target=\"wwLeft\">\n<MAP NAME=\"controls_Map\">\n" );
						if ( !is_adm ) sock_printf( sock, "<AREA SHAPE=\"rect\" ALT=\"Log-In using MP3J Mode\"	COORDS=\"2,11,97,27\" href=\"%s\" target=\"_top\">\n", ADMIN_URL );
						else {
							sock_printf( sock, 
								"<AREA SHAPE=rect ALT=\"Rew\" COORDS=\" 2,12,18,28\"   href=\"%s?o=rew\">\n"
								"<AREA SHAPE=rect ALT=\"Play\" COORDS=\"22,12,38,28\"   href=\"%s?o=pl\">\n"
								"<AREA SHAPE=rect ALT=\"Pause\" COORDS=\"42,12,58,28\"   href=\"%s?o=pau\">\n"
								"<AREA SHAPE=rect ALT=\"Stop\" COORDS=\"62,12,78,28\"   href=\"%s?o=st\">\n"
								"<AREA SHAPE=rect ALT=\"Fwd\" COORDS=\"82,12,98,28\"   href=\"%s?o=for\">\n"

								"<AREA SHAPE=rect ALT=\"Vol+++\" COORDS=\"102,8,118,20\" "
									"onClick=\"var d=new Date();parent.frames.separ.window.location.replace('%s?m=vUpUp&t=' + d.getMilliseconds());return false;\" >\n"
								"<AREA SHAPE=rect ALT=\"Vol+\" COORDS=\"102,21,118,30\" "
									"onClick=\"var d=new Date();parent.frames.separ.window.location.replace('%s?m=vUp&t=' + d.getMilliseconds());return false;\" >\n"
								"<AREA SHAPE=rect ALT=\"Vol---\" COORDS=\"122,8,138,20\" "
									"onClick=\"var d=new Date();parent.frames.separ.window.location.replace('%s?m=vDnDn&t=' + d.getMilliseconds());return false;\" >\n"
								"<AREA SHAPE=rect ALT=\"Vol-\" COORDS=\"122,21,138,32\" "
									"onClick=\"var d=new Date();parent.frames.separ.window.location.replace('%s?m=vDn&t=' + d.getMilliseconds());return false;\" >\n"

								"<AREA SHAPE=rect ALT=\"\" COORDS=\"287,12,342,32\" href=\"%s?o=random\">\n"
								"<AREA SHAPE=rect ALT=\"\" COORDS=\"346,12,375,32\" href=\"%s?o=clear\">\n"
								"<AREA SHAPE=rect ALT=\"\" COORDS=\"380,12,416,32\" href=\"%s?o=upPL\">\n"
								,
								thisurl, thisurl, thisurl, thisurl, thisurl, thisurl , thisurl, thisurl, thisurl, thisurl , thisurl, thisurl
								);
						
							}
						// lmt: target redirected to dummy frame to avoid unnecessary net trafic
						sock_printf( sock,
							"<AREA target=\"separ\" SHAPE=rect ALT=\"Maximize Playlist Window\"	COORDS=\"145,12,184,28\" onClick=\"parent.document.body.rows='%d ,80%%,1,70'\" >\n"
							"<AREA target=\"separ\" SHAPE=rect ALT=\"Maximize Library Window\"	COORDS=\"190,12,229,28\" onClick=\"parent.document.body.rows='%d ,*,1,80%%'\" >\n"
							"<AREA target=\"separ\" SHAPE=rect ALT=\"Default Size\"	COORDS=\"234,12,274,28\" onClick=\"parent.document.body.rows='%d ,50%%,1,50%%'\" >\n"
							, midHeight, midHeight, midHeight	);
						if ( g_sc_server[0] )
							sock_printf( sock,
								"<AREA SHAPE=rect ALT=\"Listen To Stream\"	COORDS=\"425,8,449,28\" HREF=\"%s/listen.pls\">\n"
								"<AREA SHAPE=rect ALT=\"Shoutcast Server\"	COORDS=\"454,8,478,28\" TARGET=\"_blank\" HREF=\"%s\">\n"
								,
								g_sc_server, g_sc_server
								);
						else sock_printf( sock, "<area shape=rect alt=\"shoutcast not configured\" COORDS=\"385,8,438,28\"  onclick=\"alert('WWWinamp is not configured for use with shoutcast at this time');\">\n" );
						sock_printf( sock, "</MAP>\n" );
						sock_printf( sock, "<img src=\"%s\" width=740 height=32 border=0 usemap=\"#controls_Map\">", (is_adm)?CONTROLADM_URL:CONTROL_URL );
						}

				else {
					// Media Library 
					if (!stricmp(mode,"right")) {
						int x;
						int start;
						int showAll=0,showLists=0,showUltimos=0;
						int nf=0;
						int pagesize = isCE ? 64 : g_library_page_size;
						char searchstring[1024], *p;
			
						unfixstring(search,searchstring,1024);
						p=searchstring;

						while (*p) {
							if (*p == '+') *p=' ';
							p++;
							}

						if (!stricmp(searchstring,"*")) showAll=1;
						if (!searchstring[0] || !stricmp(searchstring,"*lists")) {
								showAll=showLists=1;
								*searchstring=0;
							}

						if (!searchstring[0] || !stricmp(searchstring,"*ultimos")) {
								// busca los ultimos N
								UltimosDiasSearch=atoi(srchstart);
								*srchstart=0;
								if (UltimosDiasSearch<=0) UltimosDiasSearch=7;
								showAll=showUltimos=1;
								*searchstring=0;
							}

						sock_printf(sock,
							
							"<table border=0 cellspacing=0 cellpadding=3>\n" 
							"<tr valign=top>\n" 

							"<form method=\"get\" action=\"%s\"><td>\n"
							"<input type=hidden name=\"m\" value=\"right\">\n"
							"<input type=hidden name=\"q\" value=\"*\">\n"
							"<input class=button type=submit value=\"" SHOW_ALL "\"></td>\n"

							"</form>\n"
							
							"<form method=get action=\"%s\" name=search>\n"
							"<td><input type=hidden name=m value=right>\n"
							"<input type=text name=q size=30 maxlen=64 value=\"%s\" onfocus=\"this.select()\"></td>\n"
							"<td><input class=button type=submit value=" Search "></td>\n"
							"</form>\n"
							"<td><input class=button type=button value=\"" in_ " Google\" onclick=\"if ( search.q.value ) window.open( 'http://www.google.com/search?q=' + search.q.value );\">"
							"</td>\n"

							, thisurl, thisurl, searchstring);

						if (is_adm) {

							sock_printf(sock,

								"<form method=\"get\" action=\"%s\" target=\"wwLeft\">\n"
								"<td><input type=hidden name=\"o\" value=\"upML\">\n"
								"<input class=button type=submit value=\"" ReScan "\"></td></form>\n"
								
								"<form method=get action=\"%s\" target=wwLeft>"
								"<td><input type=hidden name=o value=order>\n"
								"<input class=button type=submit value=\"" Order_List "\"></td>\n</form>\n"

								,thisurl,thisurl);
								

							sock_printf(sock,
								"<form method=get action=\"%s\" target=wwLeft>"
								"<td><input type=hidden name=o value=QUIT>\n"
								"<input class=buttonred type=submit value=Shutdown>"
								"</td>\n</form>\n"

								,thisurl);
							}

						// 2da linea
						sock_printf(sock,
								"</tr></table>\n"
								"<table border=0 cellspacing=0 cellpadding=3>\n"
								"<tr valign=top>\n"

								"<form method=get action=\"%s\" name=lists><td>\n"
								"<input type=hidden name=m value=right>\n"
								"<input type=hidden name=q value=*lists>\n"
								"<input class=button type=submit value=\"" Lists "\"></td></form>\n"
																
								"<form method=get action=\"%s\" name=lists><td align=center width=180>\n"
								"<input type=hidden name=m value=right>\n"
								"<input type=hidden name=q value=*ultimos>\n"
								"<span class=label>" Ultimos 
								"</span>&nbsp<input type=text name=ss value=%d style=\"text-align:center\" size=2>"
								"<span class=label>&nbsp;" Dias "</span>&nbsp<input class=button type=submit value=Go style=\"Width:30px\">\n"
								"</td></form>\n"

								,thisurl,thisurl, UltimosDiasSearch);

						if (is_adm) {

							sock_printf(sock,
								"<form method=get name=SaveListForm action=\"%s\" target=wwLeft>\n"
								"<td width=180 align=right>"
								"<input type=hidden name=o value=SaveList>"
								"<span class=label>List:  </span><input type=text name=q size=18 maxlen=32 value=\"%s\" onfocus=\"this.select()\"></td>\n"
								"<td><input class=buttonbig type=submit value=\"" Save_List "\"></td>\n</form>\n"

								,thisurl, g_Last_List);

							sock_printf(sock,
								"<form method=get action=\"%s\" target=wwLeft><td>"
								"<input type=hidden name=m value=right>\n"
								"<input type=hidden name=o value=FixList>\n"
								"<input class=buttonbig type=submit value=\"" Fix_List "\"></td>\n</form>\n"
								,thisurl);

							sock_printf(sock,
								"<form method=get action=\"%s\" target=wwLeft><td>"
								"<input type=hidden name=m value=right>\n"
								"<input type=hidden name=o value=Duplicates>\n"
								"<input class=button type=submit value=Dups></td>\n</form>\n"
								,thisurl);

							sock_printf(sock,
								"<form method=get action=\"%s\" target=wwLeft>"
								"<td><input type=hidden name=o value=GrabarCD>\n"
								"<input type=hidden name=m value=right>\n"
								"<input class=buttonbig type=submit value=\"" Grabar_CD "\"></td>\n</form>\n"
								,thisurl);

							}

						sock_printf(sock,"</tr></table><BR>\n");

						if ( showAll || strlen(searchstring) > 0 ) {
							int istablestarted=0;
							int istdstarted=0;
							char last_leading_path[1024];
							char *ssout = (char*)malloc(strlen(searchstring)*2+8);
							parselist(ssout,searchstring);
							strcpy(last_leading_path,"\\\\\\\\\\");
							start = atoi(srchstart);
			
							sock_printf( sock, "<div class=search>\n\n");
							if (showLists) sock_printf(sock, Showing " " Lists ".");
							   else if (showUltimos) sock_printf(sock, Showing " " Ultimos " %dd.",UltimosDiasSearch );
							   else if (showAll) sock_printf(sock, Showing " " all_ " %d item%s.", database_used,  (database_used==1)?"":"s" );
							   else sock_printf( sock, Searching_for "'%s' " in_ " %d item%s.", ssout, database_used, (database_used==1)?"":"s" );
							if (start) sock_printf( sock, " (" Starting_at " %d)", start );
							sock_printf( sock, "</div>\n\n<p>");

							for (x = 0; x < database_used; x ++) {
								char* file=database[x].file;
								int found=showAll || substr_search(file,ssout) || substr_search(database[x].leading_path,ssout);
								if (showLists) {
									found=(0!=strstr(file,".m3u"));
									}
								else if (showUltimos) {
									found=(database[x].daysold <= UltimosDiasSearch );
									}
								//else if (!found) {
								//	char dummy[1024];
								//	unfixstring( database[x].file, dummy, 1024 );
								//	found = substr_search( file, ssout );
								//	}
								database[x].dbfound = found;
								if (found && ++nf >= start && nf < start+pagesize) {
									char DirName[MAX_PATH];
									/*
									char buf[MAX_PATH];
									char *tp = buf;
									char *p = buf;
									char *ptr = database[x].file;
									unfixstring(ptr,buf,sizeof(buf));
									while (*p) p++;
									while (p > buf && *p != '\\' && *p !=  '/') p--;
									*p=0;
									strncpy( leading_path, buf, sizeof(leading_path)-1 );
									leading_path[sizeof(leading_path)-1]=0;
									*/
									// stricmp returns 0 if equal, will skip if last_leading path eqals leading_path
									if (stricmp(database[x].leading_path,last_leading_path)) {
										char fullFilename[MAX_PATH];
										char addDirLink[128];
										strcpy(last_leading_path,database[x].leading_path);
										strcpy(DirName,database[x].leading_path);
										wsprintf( fullFilename, "%s%s", database[x].leading_path, file );
										wsprintf( addDirLink, "<a target=\"wwLeft\" href=\"%s?o=addd&a=%d\">", thisurl, x );
										if ( g_show_cover_art ) {
											if (istablestarted) sock_printf( sock, "</td></tr></table>\n");
											sock_printf( sock, "<table class=l>\n");
											sock_printf( sock, "<tr><td colspan=2 class=ld>%s<img src=\"/folder.gif\"></a> %s\\</td></tr>\n<tr><td class=ca>%s", addDirLink, DirName, addDirLink );
											if( hasCoverArt(fullFilename) )	sock_printf( sock, "<img src=\"%s?ca=ML&a=%d\" width=\"%d\" height=\"%d\" border=\"0\">", thisurl, x, g_cover_art_size_db, g_cover_art_size_db );
											else sock_printf( sock, "<img src=\"/spacer.gif\" width=\"%d\" height=\"1\" border=\"0\">", g_cover_art_size_db);
											sock_printf( sock, "</a></td>\n<td class=l><pre>");
											}
										else {
											if (!istablestarted) sock_printf( sock, "<pre>");
											sock_printf( sock, "<p><br><b>[%sA</a>] %s</b>\\\n", addDirLink, DirName );
											}
										istablestarted++;
										istdstarted=0;
										}
									/*while (*ptr) ptr++;
									while (*ptr != '\\' && *ptr != '/' && ptr > database[x].file) ptr--;
									if (ptr > database[x].file) ptr--;
									while (*ptr != '\\' && *ptr != '/' && ptr >= database[x].file) ptr--;
									ptr++;
									*/
									sock_printf( sock, "%s", (istdstarted)?"\n":"" );
									if (is_adm) {
										sock_printf(sock,"[<a title=\"" click_to_cue "\" href=\"%s?o=cue&a=%d\" target=wwLeft>c</a>]",thisurl,x);
										sock_printf(sock,"[<a title=\"" click_to_add_file "\" href=\"%s?o=add&a=%d\" target=wwLeft>a</a>] ", thisurl, x );
										}
									/*unfixstring(ptr,buf,sizeof(buf));
									while (*tp) tp++;
									while (tp > buf && *tp != '.' && *tp != '\\' && *tp != '/') tp--;
									if (tp > buf && *tp == '.') *tp=0;
									while (tp >= buf && *tp != '\\' && *tp != '/') tp--;
									tp++;
									*/
									switch (is_adm?g_FileHlinkMode:0) {
										case 1: // 2=FileLink cue file 
											sock_printf( sock, "<a title=\"" click_to_cue "\" href=\"%s?o=cue&a=%d\" target=wwLeft ", thisurl, x );
											break;
										case 2: // 2=FileLink cua and play now
											sock_printf( sock, "<a title=\"" click_to_cue_play "\" href=\"%s?o=cue&q=now&a=%d\" target=wwLeft ", thisurl, x );
											break;
										default: // 0=FileLink adds file to the current list
											sock_printf( sock, "<a title=\"" click_to_add_file "\" href=\"%s?o=add&a=%d\" target=wwLeft ", thisurl, x ); 
									}
									if (showLists) {
										char ListName[MAX_PATH];
										char *ext;
										strcpy(ListName,file);
										if (ext=strstr(ListName,".m3u")) *ext=0;
										sock_printf( sock, " onclick=\"SaveListForm.q.value='%s';\"", ListName );
										}
									sock_printf( sock, ">%s</a>", file );

									// DEBUG. Show days old//  
									//sock_printf( sock, " (%d days old)", database[x].daysold  );
									
									istdstarted++;
									}
								}
							
							if (istablestarted) {
								if( g_show_cover_art ) sock_printf( sock, "</td></tr></table>");
								else sock_printf(sock,"</pre>\n\n");
								}

							if (nf) {
								sock_printf( sock, "<div class=search>" );
								if (start>0) sock_printf( sock, "<a href=\"%s?m=right&q=%s&ss=%d\">&laquo; last &laquo;</a>", thisurl, search, max(start-pagesize,0) );
								else sock_printf( sock, "<span class=inactive>&laquo; " last "</a> &laquo;</span>");
								sock_printf( sock, " " Showing " %d" of_ "%d item%s ", min(pagesize,nf-start), nf,nf==1?"":"s" );
								if (nf > start+pagesize) sock_printf( sock, "<a href=\"%s?m=right&q=%s&ss=%d\">&raquo; " next " &raquo;</a>", thisurl, search, start+pagesize );
								else sock_printf( sock, "<span class=inactive>&raquo; next &raquo;</span>" );
								sock_printf( sock, "</div>" );
								}
							else sock_printf( sock, "<div class=search>" No_items_found ".</div>\n\n" );
							free(ssout);
							}
						if (!isCE) sock_printf( sock, "\n"
							"\n\n<p><div class=\"footer\">" SERV_NAME_LONG "<br>\n"
							"Status: <b>%s</b><br>\n" 
							BRANDING " " SERV_NAME " " SERV_VER "<BR>\n"
							//"<a href=\"" BRANDING_URL "\" target=\"_blank\">" BRANDING_URL "</a><br>\n" 
							BRANDING_COPYRIGHT "<br>\n"
							COPYRIGHT "</div>\n", g_statusText);
						}
					}
				sock_printf( sock, "\n</body>\n</html>\n");
				 }
			return;
			}
		 }
	}

int hasCoverArt( char *fn ) {
	FILE*	fTmp;
	char	coverArtDir[MAX_PATH];
	char	coverArtFile[MAX_PATH]	= "";
	char	*p;
	strncpy( coverArtDir, fn, MAX_PATH );
	p = coverArtDir + strlen(coverArtDir);
	while (p >= coverArtDir && *p != '\\') p--;
	*++p=0;
	wsprintf( coverArtFile, "%s%s", coverArtDir, g_cover_art_filename );
	if( fTmp = fopen(coverArtFile,"r") ) {
		fclose( fTmp );
		return 1;
		}
	return 0;
	}

int WaitForConnection(int sock,struct sockaddr_in *sin) {
	int msgsock, length;
	length = sizeof(struct sockaddr_in);
 	msgsock = accept(sock, (struct sockaddr *)sin, &length);
	return msgsock;
	}

int myrand(void) {
	g_rand_next = g_rand_next * 1103515245 + 12345;
	return ( (g_rand_next >>16) & RAND_MAX);
	}

void mysrand (unsigned int seed) { 
	g_rand_next = seed;
	}

int httpgetFile( int sock, char *fileName, char *ContentType, char *cacheString ) {
	FILE *httpFile=NULL;
	char filebuffer[1024];
	char headHTTP200[256]		= "HTTP/1.0 200 OK\r\n";
	char headHTTP404[256]		= "HTTP/1.0 404 Not Found\r\n";
	char log_area[8]			= "[file]";
	httpFile = fopen(fileName, "rb");
	if (httpFile && ContentType) {
		int readBytes;
		sock_printf( sock, "%s%sContent-type: %s\r\n\r\n", headHTTP200, cacheString, ContentType );
		while (!feof(httpFile)) {
			readBytes = fread( filebuffer, sizeof(char), 1024, httpFile );
			sock_send(sock, filebuffer, readBytes);
			}
		fclose(httpFile);
		return 1;
		}
	else {
		sock_printf( sock, "%sContent-type: text/html\r\n\r\n", headHTTP404 );
		sock_printf( sock, "<HTML><HEAD>\n<TITLE>404 Not Found</TITLE>\n</HEAD><BODY>\n<H1>Not Found</H1>\nThe requested URL was not found on this server.<P>\n<HR><ADDRESS>%s %s Port %d</ADDRESS>\n</BODY></HTML>", SERV_NAME, SERV_VER, g_dst_port );
		if (ContentType) log_printf( "%s ERROR: couldn't find %s .", log_area, fileName );
		return 0;
		}
	}
