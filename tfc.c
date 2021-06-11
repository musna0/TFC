#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#define VERSION		"0.4"

#define MAX_INT		0x7FFFFFFF
#define MAX_DIGITS	10
#define MAX_FIELDS	1024
#define MAX_F_NAME_L	512

#define NULL_F_NAME	"%%NULL_FILE_NAME%%"
#define NUM_NULL	"          "

#define FIRST_PRINT_C	'!'			/* first printable character */
#define LAST_PRINT_C	'~'			/* last printable character */

#define FIRST_L_LETTER	'a'
#define LAST_L_LETTER	'z'
#define FIRST_U_LETTER	'A'
#define LAST_U_LETTER	'Z'

void help_printout();
void additions_printout();

void big_num_err(char *arg);

void set_num(char *arg, int *field_qty, int *field_num);

char *rand_str(int len);
char *rand_wrd(int len);

void create_file(char separator, char *field_typ, int *field_qty, int field_num, int field_tot, char *f_name);


int main(int argc, char *argv[])
{
	int	i;
	bool	options;
	char	separator;
	int	field_num;
	int	field_qty[MAX_FIELDS];
	int	field_tot;
	char	field_typ[MAX_FIELDS];
	char	f_name[MAX_F_NAME_L];

	if (argc == 1) {
		printf("Error: not enough arguments - bailing out\n");
		exit(EXIT_FAILURE);
	}

	srand(time(NULL));

	print();

	options	= false;
	separator = ' ';
	field_num = 0;
	field_tot = 0;
	strcpy(f_name, NULL_F_NAME);

	for (i = 1; i < argc; i++) {
		switch (argv[i][0]) {
		case '-':			/* options */
			switch (argv[i][1]) {
			case 'v':
				printf("TFC (Test File Creator) current version: %s\n", VERSION);
				break;
			case 'h':
				help_printout();
				break;
			case 'a':
				additions_printout();
				break;
			default:
				printf("Error: invalid argument\n");
			}
			options = true;
			break;
		default:
			if (strstr(argv[i], "sep") != NULL && argv[i][3] == '=')
				separator = argv[i][4];
			else if (strcmp(argv[i], "spc") == 0)
				separator = ' ';
			else if (strcmp(argv[i], "tab") == 0)
				separator = '\t';
			else if (strstr(argv[i], "num") != NULL && argv[i][3] == '=') {
				field_typ[field_num] = 'n';
				set_num(argv[i], field_qty, &field_num);
			} else if (strstr(argv[i], "flt") != NULL && argv[i][3] == '=') {
				field_typ[field_num] = 'f';
				set_num(argv[i], field_qty, &field_num);
			} else if (strstr(argv[i], "str") != NULL && argv[i][3] == '=') {
				field_typ[field_num] = 's';
				set_num(argv[i], field_qty, &field_num);
			} else if (strstr(argv[i], "wrd") != NULL && argv[i][3] == '=') {
				field_typ[field_num] = 'w';
				set_num(argv[i], field_qty, &field_num);
			} else if (isdigit(argv[i][0])) {
				field_tot = atoi(argv[i]);
			} else {
				if (i == argc - 1) {
					strcpy(f_name, argv[i]);
				}
			}
			
			break;
		}
	}

	if (strcmp(f_name, NULL_F_NAME) == 0 && options == false) {
		printf("Error: no file name as last argument - bailing out\n");
		exit(EXIT_FAILURE);
	}

	if (field_tot == 0 && options == false)
		printf("Warning: total number of fiels is 0, no output expected\n");

	if (options == false)
		create_file(separator, field_typ, field_qty, field_num, field_tot, f_name);

	return EXIT_SUCCESS;
}

void help_printout()
{
	printf("Usage: tfc [OPTIONS] OR [ARGUMENTS] file_name\n");
	printf("Creates a test file defined by [ARGUMENTS].\n");
	printf("\nOptions:\n");
	printf("  -v:	 prints the current version of TFC.\n  -h:    prints this information.\n");
	printf("  -a:	 prints a list of features I plan to add in the near future\n");
	printf("Arguments:\n");
	printf("  num=n: sets a field to an integer less than n\n");
	printf("  flt=n: sets a field to a float less than n\n");
	printf("  str=n: sets a field to a string of random printable characters of n length\n");
	printf("  wrd=n: sets a field to a string of letters of n length\n");
	printf("  tab:	 sets the tab character as the separator\n");
	printf("  spc:	 sets the space character as the separator (deafult separator value)\n");
	printf("  sep=*: sets a single digit character as the separator (* stands for any character)\n");
}

void additions_printout()
{
	printf("Add decent comments (coming soon™)\n");
	printf("Fix max num size and related stuff (coming in 0.3) - DONE\n");
	printf("Add controls for not exceeding maximum string and number length (coming in 0.3) - DONE\n");
	printf("Add new options for floating point numbers and words which are alphabet only strings (coming in 0.3.5) - DONE\n");
	printf("Improve the -h option, add the arguments section (coming in 0.4) - DONE\n");
	printf("Add new number managing system, add less than, more than, etc. (coming in 0.5)\n");
	printf("Add the option to use multiple separators to render possible using a space after a comma and useful stuff like that, add spc and tab keywords for space and tab separators (coming in 0.5.5) - SECOND PART DONE\n");
	printf("Add the possibility to make string length between two values (coming in 0.6)\n");
}

void big_num_err(char *arg)
{
	printf("Error: the field size is too big in this field: %s, ", arg);
	printf("largest accepted value is %d - bailing out\n", MAX_INT);
	exit(EXIT_FAILURE);
}

void set_num(char *arg, int *field_qty, int *field_num)
{
	int	i;
	char	num[MAX_DIGITS + 1];

	strcpy(num, NUM_NULL);

	if (strlen(arg) > MAX_DIGITS + 4)
		big_num_err(arg);

	for (i = 4; i < strlen(arg); i++)
		num[i - 4] = arg[i];

	if (atol(num) > MAX_INT)
		big_num_err(arg);

	field_qty[*field_num] = atoi(num);

	(*field_num)++;
}

char *rand_str(int len)
{
	int	i;
	char	*str = NULL;

	str = malloc(sizeof(*str) * len);
	if (str == NULL) {
		printf("Error: problem occurred while trying to allocate memory - bailing out");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < len; i++)
		str[i] = FIRST_PRINT_C + rand() % (LAST_PRINT_C - FIRST_PRINT_C);

	return str;
}

char *rand_wrd(int len)
{
	int	i;
	int	condition;
	char	*wrd = NULL;
	char	chr;

	chr = 0;
	condition = 0;

	wrd = malloc(sizeof(*wrd) * len);
	if (wrd == NULL) {
		printf("Error: problem occurred while trying to allocate memory - bailing out");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < len; i++) {
		while (!condition) {
			chr = FIRST_U_LETTER + rand() % (LAST_L_LETTER - FIRST_U_LETTER);

			if (chr > LAST_U_LETTER && chr < FIRST_L_LETTER)
				condition = 0;
			else
				condition = 1;
		}
		condition = 0;

		wrd[i] = chr;
	}

	return wrd;
}

void create_file(char separator, char *field_typ, int *field_qty, int field_num, int field_tot, char *f_name)
{
	int	i, j;
	int	rand_n;
	float	rand_f;
	char	*rand_s;
	char	*rand_w;
	FILE	*fp = NULL;
	
	fp = fopen(f_name, "w");
	if (fp == NULL) {
		printf("Error: problem occurred while creating the file - bailing out\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0 ; i < field_tot; i++) {
		for (j = 0; j < field_num; j++) {
			switch (field_typ[j]) {
			case 'n':
				rand_n = rand() % field_qty[j];

				if (j < field_num - 1)
					fprintf(fp, "%d%c", rand_n, separator);
				else
					fprintf(fp, "%d", rand_n);
				break;
			case 'f':
				rand_f = (float)rand()/RAND_MAX * field_qty[j];

				if (j < field_num - 1)
					fprintf(fp, "%f%c", rand_f, separator);
				else
					fprintf(fp, "%f", rand_f);
				break;
			case 's':
				rand_s = rand_str(field_qty[j]); 

				if (j < field_num - 1)
					fprintf(fp, "%s%c", rand_s, separator);
				else
					fprintf(fp, "%s", rand_s);
				break;
			case 'w':
				rand_w = rand_wrd(field_qty[j]);

				if (j < field_num - 1)
					fprintf(fp, "%s%c", rand_w, separator);
				else
					fprintf(fp, "%s", rand_w);
				break;
			default:
				printf("Error: something weird happened - the program will finish up anyways\n");
				break;
			}
		}
		fprintf(fp, "\n");
	}
}
