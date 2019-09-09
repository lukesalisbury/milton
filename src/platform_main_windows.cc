// Copyright (c) 2015 Sergio Gonzalez. All rights reserved.
// License: https://github.com/serge-rgb/milton#license
#include <synchapi.h>

int
CALLBACK WinMain(HINSTANCE hInstance,
                 HINSTANCE hPrevInstance,
                 LPSTR lpCmdLine,
                 int nCmdShow)
{
	HANDLE unique_mutex = nullptr;
    win32_cleanup_appdata();
    PATH_CHAR path[MAX_PATH] = TO_PATH_STR("milton.log");
    platform_fname_at_config(path, MAX_PATH);
    g_win32_logfile = platform_fopen(path, TO_PATH_STR("w"));

	char file_to_open[MAX_PATH] = {};
	char * pfile_to_open;
	pfile_to_open = file_to_open;
	b32 start_running = true;
	b32 load_file = false;
    bool is_fullscreen = false;
    //TODO: proper cmd parsing
	size_t cmd_length = strlen(lpCmdLine);
	u8 cmd_mode= 0;
	for (size_t q = 0; q < cmd_length; q++) {
		switch (cmd_mode) {
			case 1:
				if ( lpCmdLine[q] == ' ') {
					cmd_mode = 0; //reset
				} else if (lpCmdLine[q] == 'F') {
					is_fullscreen = true;
					milton_log("Fullscreen is set.\n");
				}
				else if (lpCmdLine[q] == 'U') {
					unique_mutex = CreateMutex( NULL, TRUE, "MiltonInstance" );
					if ( GetLastError() == ERROR_ALREADY_EXISTS ) {
						start_running = false;
					}
				} else {
					cmd_mode = 0; //reset
				}
				break;
			case 3:
			case 2: {
				if ( lpCmdLine[q] == '\\' ) {
					cmd_mode = 3;
				} else if ( lpCmdLine[q] == '"') {
					if (cmd_mode == 2) {
						cmd_mode = 0; // Reset, end of string
					}
				}
				*pfile_to_open = lpCmdLine[q]; pfile_to_open++;

			} break;
			default:{
				if ( lpCmdLine[q] == '-') {
					cmd_mode = 1; //Argument
				} else if ( lpCmdLine[q] == '"') {
					cmd_mode = 2; //String
					load_file = true;
					*pfile_to_open = lpCmdLine[q]; pfile_to_open++;
				}
			}break;
		}
	}
	milton_log("CommandLine is %s\n", lpCmdLine);

	if ( start_running ) {
		milton_main(is_fullscreen, (load_file ? file_to_open : nullptr));
	}
	if ( unique_mutex ) {
		CloseHandle( unique_mutex );
	}
}
