#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// MIT:https://github.com/tomsik68/ansi_colors
// list
#define __ANSI_SEQ(n) "\033[" #n "m"
#define BOLD __ANSI_SEQ(1)
#define WEAK __ANSI_SEQ(2)
#define HIGHLIGHT __ANSI_SEQ(3)
#define UNDERLINE __ANSI_SEQ(4)
#define BLACK __ANSI_SEQ(30)
#define DARK_RED __ANSI_SEQ(31)
#define DARK_GREEN __ANSI_SEQ(32)
#define DARK_YELLOW __ANSI_SEQ(33)
#define DARK_BLUE __ANSI_SEQ(34)
#define DARK_PINK __ANSI_SEQ(35)
#define DARK_CYAN __ANSI_SEQ(36)
#define BLACK_BG __ANSI_SEQ(40)
#define DARK_RED_BG __ANSI_SEQ(41)
#define DARK_GREEN_BG __ANSI_SEQ(42)
#define DARK_YELLOW_BG __ANSI_SEQ(43)
#define DARK_BLUE_BG __ANSI_SEQ(44)
#define DARK_PINK_BG __ANSI_SEQ(45)
#define DARK_CYAN_BG __ANSI_SEQ(46)
#define GRAY __ANSI_SEQ(90)
#define LIGHT_RED __ANSI_SEQ(91)
#define LIGHT_GREEN __ANSI_SEQ(92)
#define LIGHT_YELLOW __ANSI_SEQ(93)
#define LIGHT_BLUE __ANSI_SEQ(94)
#define LIGHT_PINK __ANSI_SEQ(95)
#define LIGHT_CYAN __ANSI_SEQ(96)
#define LIGHT_GRAY __ANSI_SEQ(97)
#define GRAY_BG __ANSI_SEQ(100)
#define LIGHT_RED_BG __ANSI_SEQ(101)
#define LIGHT_GREEN_BG __ANSI_SEQ(102)
#define LIGHT_YELLOW_BG __ANSI_SEQ(103)
#define LIGHT_BLUE_BG __ANSI_SEQ(104)
#define LIGHT_PINK_BG __ANSI_SEQ(105)
#define LIGHT_CYAN_BG __ANSI_SEQ(106)
#define LIGHT_GRAY_BG __ANSI_SEQ(107)
#define RESET "\033[m"
// /list

#define BOX_START "\033(0"
#define BOX_END "\033(B"
#define BOX_ESCAPE(x) BOX_START x BOX_END
#define BOX_TL BOX_ESCAPE("\x6C")
#define BOX_H  BOX_ESCAPE("\x71")
#define BOX_TR BOX_ESCAPE("\x6B")
#define BOX_V  BOX_ESCAPE("\x78")
#define BOX_BL BOX_ESCAPE("\x6D")
#define BOX_BR BOX_ESCAPE("\x6A")

FILE *outfile, *infile;
int is_admin = 0;
char *username = "";

const char *banner =
    "\n"
    "\n"
	LIGHT_GREEN
	"::::    ::::   ::::::::   ::::::::   ::::::::  :::::::::  :::::::::: ::::    :::: \n"
	"+:+:+: :+:+:+ :+:    :+: :+:    :+: :+:    :+: :+:    :+: :+:        +:+:+: :+:+:+\n"
	"+:+ +:+:+ +:+ +:+    +:+ +:+    +:+ +:+    +:+ +:+    +:+ +:+        +:+ +:+:+ +:+\n"
	"+#+  +:+  +#+ +#+    +:+ +#+    +:+ +#+    +:+ +#+    +:+ +#++:++#   +#+  +:+  +#+\n"
	"+#+       +#+ +#+    +#+ +#+    +#+ +#+    +#+ +#+    +#+ +#+        +#+       +#+\n"
	"#+#       #+# #+#    #+# #+#    #+# #+#    #+# #+#    #+# #+#        #+#       #+#\n"
	"###       ###  ########   ########   ########  #########  ########## ###       ###\n"
    "\n"
    RESET;

const char *banner_sub =
	DARK_GREEN
	"        w e l c o m e   t o   t h e   m o o o d e m   b b s   s y s t e m\n"
    "\n"
    "                                    0 . 0 . 0\n"
    "\n"
    RESET;

const char *ooo =
	GRAY
	"\n"
	"\n"
	"                              .,,++++bbbbp+++p,,\n"
	"                         upQPPTT--   ,  ,   ``TTTPQpp,\n"
	"                    ,+pPDT ,  )b )| -D -P (P )P  ,   TPbe,\n"
	"                  +PP,   p `P  O             `, SD eD   .TPp,\n"
	"               ,QP,   Pu T`     ,+bPPPTTPPPb+,     ` -P  +  TQp\n"
	"             +PP , )Te       ,ePT=          `TTb,       T` =p `Tp\n"
	"           ,SP   )T         QPD                `Tb,       `P =u TQD\n"
	"          eP   TT         ,QP                    -PD        `, ), TD\n"
	"         bD  TPb          bP                      )Pu         TT , )Pu\n"
	"       ,bD )Pp   ,,,,,,,,,Pu                      ,bD,,,,,,,,,  TT  `Pb\n"
	"      .PD ep,   TPDTTTTTTTPTTTTTTTTTT)PPDTTTTTTTTTTPPTTTTTTTPP   ePO  Pp\n"
	"      DP ,,      `Pp      Pu        sP, Tb         bG     ,PP     ,ee )P\n"
	"     )P  ,TO       Pb    ,SPp,,    +P    Tb    ,,ubD,    sP`       ,,, Tb\n"
	"     DP )PP         SDbPPTTTPbTTPbQT      )PbPPTSPDTTTPbbP`        `,` )P\n"
	"     D` pp,       =DTTP,     TPp+PDTPp  ,SPTPp+PP      SPTTb       tPP` Pb\n"
	"    )P  ,,      )PP    QD      )PTbpeQDQPeepPTP      ,Q`   )Pp      e+. PP\n"
	"    )P  TT`    sP,      Tb    eP    .SPPD,    TP,   +P-      Tb     ,   PP\n"
	"     D` eDP   ,PC        Tb ,QP      bD-Pu     )P, SP         Pp    TT` PP\n"
	"     DP  ==   TP          TPPP======dPQ,bb======QPPD          ST   SPb )P\n"
	"     )P  , ,  TP            TP      )PG PP      )PO           bT  -u,  eP\n"
	"      Pb STO   Pp            TP      Pb+P,     +P            ,DD  , ` )P\n"
	"       Pu +bP  `Pu            TP,     QPD     pP            ,PP   TT ,PG\n"
	"        PD  upu `Tb,           `Pp   ePTDu  ,PP            sPO  TPb ,PC\n"
	"         Qe ` )+  `Tbp,          DbpPO   TPpP`          ,pPO   Tp  ,P-\n"
	"          Tb `O ,+   .TPPbbbpbbPPTTP      ePTTPbbbpbbPPTO    Pp   pP\n"
	"           -Pp )C  =               )P,  ,eP                Qp   dDD\n"
	"             TTp  T  p              )Pp,PO               Tp ` ,PD\n"
	"               )Tb, `  eP ,           SP,           p `Qu  ,ePO\n"
	"                 `TPbu - +P ,b  u  ,          -, Du Sb  ,ePT`\n"
	"                     TPbpp  `. )P )T )D TD |b )P  , u=bPT\n"
	"                        `TTPQbp,,u    ,  `  ,,,ppPPPT`\n"
	"                                TTTTTTTTTTTTTT`\n"
	"\n"
	RESET;

#define print_prompt_message(...) do { \
	fprintf(outfile, BOLD); \
	fprintf(outfile, __VA_ARGS__); \
	fprintf(outfile, RESET); \
	} while(0)

#define print_info_message(...) do { \
	fprintf(outfile, LIGHT_YELLOW); \
	fprintf(outfile, __VA_ARGS__); \
	fprintf(outfile, RESET); \
	} while(0)

#define print_error_message(...) do { \
	fprintf(outfile, LIGHT_RED); \
	fprintf(outfile, __VA_ARGS__); \
	fprintf(outfile, RESET); \
	} while(0)

// Print a fancy centered header
void print_header(const char *prefix, const char *msg, const char *suffix)
{
	size_t len = strlen(msg);
	size_t num_dashes = (80-len-2)/2;
	
	// Vertical Padding
	fprintf(outfile, "\n\n");

	// Prefix
	fputs(prefix, outfile);

	// Left dashes
	fprintf(outfile, BOX_START);
	for (int i = 0; i < num_dashes; i++) fprintf(outfile, BOX_H);
	fprintf(outfile, BOX_END);

	// Center Text
	fprintf(outfile, " %s ", msg);

	// Right dashes
	if (len % 2) num_dashes += 1;
	fprintf(outfile, BOX_START);
	for (int i = 0; i < num_dashes; i++) fprintf(outfile, BOX_H);
	fprintf(outfile, BOX_END);

	fputs(suffix, outfile);

	// Vertical Padding
	fprintf(outfile, "\n\n");
}

// Recieves into a static buffer, discards input beyond buffer maximum
char *prompt(const char *msg)
{
	static char buffer[64];
	print_prompt_message("%s", msg);

	int i = 0;

	for (;;) {
		char c = fgetc(infile);

		if (c == EOF) {
			if (i == 0) {
				return NULL;
			} else {
				break;
			}
		}

		if (c == '\r') {
			continue;
		}

		if (c == '\n') {
			break;
		}

		if (i < sizeof(buffer)-1) {
			buffer[i++] = c;
		}
	}

	buffer[i] = '\x00';

	return buffer;
}

void slow_print(const char *msg)
{
	while (*msg) {
		fputc(*msg++, outfile);
		usleep(30000);
	}
}

char *get_current_date(void)
{
	int year, month, day;
	static char buffer[64];

#ifdef TESTING
	time_t now = time(NULL);
	struct tm *local = localtime(&now);
	year = local->tm_year+1900;
	month = local->tm_mon + 1;
	day = local->tm_mday;
#else
	// Above is broken in newlib for some reason, just use DOS to figure it out
	// via https://github.com/tkchia/newlib-ia16/blob/newlib-2_4_0-ia16-tkchia/libgloss/ia16/dos-gettimeofday.c
	unsigned ax, bx, cx, dx;
	__asm __volatile ("movb $0x2a, %%ah; int $0x21"
	: "=&a" (ax), "=b" (bx), "=c" (cx), "=d" (dx) : : "cc", "memory");
	year = cx;
	month = dx >> 8;
	day = dx & 0xff;
#endif

	sprintf(buffer, "%02d-%02d-%04d", month, day, year);
	return buffer;
}

/****************************************************************************/
/* Initial greeting, user registration
/****************************************************************************/

void greet(void)
{
	while (1) {
		char *name = prompt("What is your name? ");
		if (name == NULL) {
			return;
		}
		if (strlen(name) == 0) continue;
		username = strdup(name);
		break;
	}

	fprintf(outfile, "Greetings, " LIGHT_GREEN "%s" RESET"!\n", username);
}

/****************************************************************************/
/* Bulletins
/****************************************************************************/

// INTENTIONAL VULNERABILITY: Unbounded read from file into `buffer`.
// This assumes an admin never creates a bulletin with lines >= 255
// characters.
//
// This also mangles input, so the shellcode submitted will need to tolerate:
//     - \t (\x09)  being converted to four spaces
//     - \r (\x0D)  being dropped
//     - \n (\x0A) or \x00 being the last byte

void list_bulletins(void)
{
	// Force stack layout for exploit convenience
	struct {
		FILE *fd;
		int bytes_read;
		int author_len;
		char author[64];
		int date_len;
		char date[64];
		int end_of_record;
		int col;
		int i;

		char buffer[256];
	} l;

	l.fd = fopen("NOTES.TXT", "rb");
	if (l.fd == NULL) {
		print_error_message("No available bulletins\n");
		return;
	}

	print_header(LIGHT_GREEN BOLD, "Recent Bulletins", RESET);

	for (;;) {
		// Read bulletin header (a single null byte)
		l.bytes_read = fread(&l.buffer[0], 1, 1, l.fd);
		if (l.bytes_read == 0) {
			// End of bulletins
			break;
		}

		if (l.buffer[0] != '\x00') {
			print_error_message("Invalid bulletin header\n");
			goto error;
		}

		// Read author line
		fgets(l.author, sizeof(l.author), l.fd);
		l.author_len = strlen(l.author);
		if (l.author_len > 0 && l.author[l.author_len-1] == '\n') {
			l.author[l.author_len-1] = '\x00';
		}

		// Read date line
		fgets(l.date, sizeof(l.date), l.fd);
		l.date_len = strlen(l.date);
		if (l.date_len > 0 && l.date[l.date_len-1] == '\n') {
			l.date[l.date_len-1] = '\x00';
		}

		sprintf(l.buffer, "Bulletin by %s on %s", l.author, l.date);
		fprintf(outfile, "      " BOX_START BOX_TL BOX_H LIGHT_CYAN " %s " RESET, l.buffer);
		for (l.i = 0; l.i < (63-strlen(l.buffer)); l.i++)  fprintf(outfile, BOX_H);
		fprintf(outfile, BOX_TR BOX_END "\n");

		// Read the bulletin line by line into a buffer
		for (l.col = 0;;) {
			l.bytes_read = fread(&l.buffer[l.col], 1, 1, l.fd);
			if (l.bytes_read != 1) {
				print_error_message("Unexpected end of bulletin\n");
				goto error;
			}
			if (l.buffer[l.col] == '\t') {
				l.buffer[l.col] = ' ';
				for (l.i = 0; l.i < 3; l.i++) {
					l.buffer[++l.col] = ' ';
				}
			}

			// Drop carraige returns during print
			if (l.buffer[l.col] == '\r') {
				continue;
			}
			l.end_of_record = (l.buffer[l.col] == '\x00');
			if (l.buffer[l.col] == '\n') {
				// Encountered newline. Decorate last line and print it out.
				l.buffer[l.col] = '\x00';
				fprintf(outfile, "      " BOX_ESCAPE(BOX_V) " %-64s " BOX_ESCAPE(BOX_V) "\n", l.buffer);
				l.col = 0;
			} else {
				l.col += 1;
			}
			if (l.end_of_record) break;
		}

		fprintf(outfile, "      " BOX_START BOX_BL);
		for (l.i = 0; l.i < 66; l.i++)  fprintf(outfile, BOX_H);
		fprintf(outfile, BOX_BR BOX_END "\n");
	}

error:
	fclose(l.fd);
	// asm volatile ("jmp .");
}

void create_bulletin(void)
{
	if (!is_admin) {
		print_error_message("Unauthorized\n");
		return;
	}

	print_header(LIGHT_GREEN BOLD, "New Bulletin", RESET);

	FILE *fd = fopen("NOTES.TXT", "ab+");
	if (fd == NULL) {
		print_error_message("Failed to open\n");
		return;
	}

	// Header: [null byte][username][\n][current date][\n])
	char *today = get_current_date();
	if ((!fwrite("\x00", 1, 1, fd)) ||
		(!fwrite(username, strlen(username), 1, fd)) ||
		(!fwrite("\n", 1, 1, fd)) ||
		(!fwrite(today, strlen(today), 1, fd)) ||
		(!fwrite("\n", 1, 1, fd))
		) {
		print_error_message("Failed to start note\n");
		goto error;
	}

	// Read directly into the file
	int start_of_line = 1;
	print_prompt_message("Bulletin body (end with blank line):\n");
	for (;;) {
		if (start_of_line) {
			// Print a prompt to let the user know we are expecting more input
			print_prompt_message("> ");
		}

		int c = fgetc(infile);
		if ((c == EOF) || (start_of_line && (c == '\n'))) {
			break;
		}

		if (start_of_line && c != '\r') {
			start_of_line = 0;
		}

		// Line feed triggers start of line
		if (c == '\n') {
			start_of_line = 1;
		}

		size_t bytes_written = fwrite(&c, 1, 1, fd);
		if (bytes_written != 1) {
			print_error_message("Failed to write note\n");
			goto error;
		}
	}

	// Dummy footer
	if (fwrite("\x00", 1, 1, fd) != 1) {
		print_error_message("Failed to end note\n");
		goto error;
	}

	print_info_message("New bulletin created\n");

error:
	fclose(fd);
}

/****************************************************************************/
/* Secret Admin Function
/****************************************************************************/

void admin(void)
{
	fputs(GRAY, outfile);
	slow_print("Welcome, trusted admin...\n\n");
	fputs(ooo, outfile);
	is_admin = 1;
}

/****************************************************************************/
/* Quote
/****************************************************************************/

// Return number of newline characters in a file, or seek to a specific line
int seek_lines(FILE *fd, int target)
{
	int count = 0;
	while (1) {
		if (target >= 0 && count >= target) {
			// Seek to desired line has completed
			break;
		}
		char c = fgetc(fd);
		if (c == EOF) break;
		if (c == '\n') count++;
	}
	return count;
}

void show_random_quote(void)
{
	// Quotes lifted from https://raw.githubusercontent.com/thors/fortune/master/data/quote.txt
	FILE *fd = fopen("QUOTES.TXT", "rb");
	if (fd == NULL) {
		print_error_message("Unable to find quotes\n");
		return;
	}

	// Get total number of quotes
	int num_lines = seek_lines(fd, -1);
	rewind(fd);

	// Pick a random quote
	int desired = rand() % num_lines;
	seek_lines(fd, desired);

	// Read it out
	while (1) {
		char c = fgetc(fd);
		if (c == '\n' || c == EOF) break;
		fputc(c, outfile);
	}

	fclose(fd);
}

/****************************************************************************/
/* File Access
/****************************************************************************/

// A dumb file transfer. Just dumps out the file bytes in hex
void list_files(void)
{
	FILE *fd = fopen("FILES.TXT", "rb");	
	if (fd == NULL) {
		print_error_message("Unable to list files\n");
		return;
	}

	// List out all available files
	print_header(LIGHT_GREEN BOLD, "Available Files", RESET);
	char filename[32];
	for (int count = 0; ; count++) {
		if (!fgets(filename, sizeof(filename), fd)) break;
		if (strlen(filename) == 0) continue;
		fprintf(outfile, "     " GRAY "(" LIGHT_GREEN BOLD "%d" GRAY ") " RESET "%s", count, filename);
	}
	fprintf(outfile, "\n");

	// Get desired file number
	char *response = prompt("Download File # (or press 'Return' to cancel): ");
	if (response == NULL) {
		print_info_message("Failed to get input\n");
		fclose(fd);
		return;
	}
	int desired = -1;
	if (sscanf(response, "%d", &desired) <= 0) {
		print_info_message("Cancelled\n");
		fclose(fd);
		return;
	}

	// Loop back to get the filename for this index
	int found = 0;
	rewind(fd);
	for (int count = 0; ; count++) {
		if (!fgets(filename, sizeof(filename), fd)) break;
		if (strlen(filename) == 0) continue;
		if (count == desired) {
			found = 1;
			break;
		}
	}

	fclose(fd);

	if (!found) {
		print_error_message("Invalid file selection\n");
		return;
	}

	// Ready to send the file
	if (filename[strlen(filename)-1] == '\n') {
		filename[strlen(filename)-1] = '\0'; // Trim newline on filename
	}
	fd = fopen(filename, "rb");
	if (fd == NULL) {
		print_error_message("Failed to open the desired file\n");
		return;
	}

	// Prompt user to begin file download
	print_info_message("Sending...");
	fprintf(outfile, "\n");

	size_t bytes_total = 0;
	size_t bytes_read = 0;

	// Determine filesize
	fseek(fd, 0, SEEK_END);
	bytes_total = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	// Read file out
	while (!feof(fd)) {
		unsigned char buf[1];
		size_t bytes_in_chunk = fread(buf, 1, sizeof(buf), fd);
		if (bytes_in_chunk == 0) break;
		bytes_read += bytes_in_chunk;
		fprintf(outfile, "%02x ", buf[0]);
	}
	fclose(fd);

	if (bytes_read != bytes_total) {
		print_error_message("An error occured during transfer. Try again.\n");
	}

	fprintf(outfile, "\n");
	print_info_message("Done");
}

/****************************************************************************/
/* Main Loop
/****************************************************************************/

void main_menu(void)
{
	struct {
		char key;
		char *msg;
	} menu_options[4] = {
		{ 'L', "List Recent Bulletins" },
		{ 'C', "Create New Bulletin" },
		{ 'F', "List Available Files" },
		{ 'Q', "Show Random Quote" },
	};
	const int num_menu_options = sizeof(menu_options)/sizeof(menu_options[0]);
	for (;;) {
		print_header(LIGHT_GREEN BOLD, "Main Menu", RESET);

		fprintf(outfile, "%30sToday is %s\n\n", "", get_current_date());

		// Print main menu options in two columns
		int newline = 0;
		for (int i = 0; i < num_menu_options; i++) {
			fprintf(outfile, "     " GRAY "(" LIGHT_GREEN BOLD "%c" GRAY ")" RESET " %-30s", menu_options[i].key, menu_options[i].msg);
			if (newline) {
				fprintf(outfile, "\n");
			}
			newline = !newline;
		}
		if (newline) {
			fprintf(outfile, "\n");
		}
		fprintf(outfile, "\n");

		// Read in response
		char *response = prompt("Selection: ");
		if (response == NULL) {
			return;
		}
		switch (response[0]) {
		case 'l':
		case 'L':
			list_bulletins();
			break;
		case 'c':
		case 'C':
			create_bulletin();
			break;
		case 'f':
		case 'F':
			list_files();
			break;
		case 'q':
		case 'Q':
			show_random_quote();
			break;
		default:
			break;
		}

		// python -c "print(repr(bytes(x ^ 0x8c for x in b'supersneaky2020'))[1:])"
		const char *admin_password = "\xff\xf9\xfc\xe9\xfe\xff\xe2\xe9\xed\xe7\xf5\xbe\xbc\xbe\xbc";
		for (int i = 0; i < strlen(response); i++) response[i] ^= 0x8c;
		if (strcmp(response, admin_password) == 0) {
			admin();
		}
	}
}

int main(int argc, char const *argv[])
{
	infile = stdin;
	outfile = stdout;
	setvbuf(outfile, NULL, _IONBF, 0);
	setvbuf(infile, NULL, _IONBF, 0);

	fputs(banner, outfile);
	slow_print(banner_sub);
	greet();
	main_menu();

	return 0;
}
