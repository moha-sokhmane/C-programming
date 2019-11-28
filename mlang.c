
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define DELIM " "
#define PRINT 1
#define INPUT   2
#define MEM 3
#define LET 4
#define NUMBER 5
#define PLUS 6
#define QUOTE 7
#define IF 8
#define  EGAL 9
#define STRING 10
#define VARIABLE 11
#define MINUS 12
#define MUL 13
#define DIV 14
#define ICOM 15
#define INT 16
#define THEN 17
#define WHILE 18
#define FOR 19
#define CHAR 20
typedef struct variable variable;
typedef struct stmt stmt;
bool test;
struct variable
{
	char nom[100];
	int valeur;
	char string[100];
};

struct stmt
{
	int discr;
	union commande
	{
		variable var[25];

	} u;
};

// globale variables
int cnt, last, m, nvar;
static int lv;
float x, y;
int type;
char *str;
char *tok[50] = { NULL };

char *lateral = NULL;
variable var[25];
variable *vars;
// fonctions
int lexer(char *tok[], char *str);
int assignement(char *tok[]);
char *strings(char *tok[], int lv);
int doinput(char *tok[]);
void memoire(char *tok[]);
int pointeur(char *tok[]);
float expressions(char *tok[], int lv);
void print_var(char *tok[]);
void if_stmt(char *tok[]);
void for_stmt(char *tok[]);
void while_stmt(char *tok[]);
void address(char *tok[]);
int var_type(char *tok[]);


int main(int argc, char *argv[])
{

	// char *tok[50] = { NULL };
	lateral = malloc(100 * sizeof(char));
	str = malloc(2048 * sizeof(char));
	printf("Cet interpreteur est realisé par M.Sokhmane  ");

	while (1)
	{
		printf("\n>>   ");
		fgets(str, 2048, stdin);
		str[strlen(str) - 1] = '\0';
		if (*str == '\0')
		{
			continue;
		}
		else if (strcmp(str, "exit") == 0)
		{
			return 0;
		}
		else
			type = lexer(tok, str);
		var_type(tok);
		expressions(tok, 0);
		switch (type)
		{

		case PRINT:
			strings(tok, 1);
			expressions(tok, 1);
			print_var(tok);
			address(tok);
			break;
		case INPUT:
			doinput(tok);
			break;
		case MEM:
			memoire(tok);
			break;
		case VARIABLE:
			assignement(tok);
			break;
		case IF:
			if_stmt(tok);
			break;
		case WHILE:
			while_stmt(tok);
			break;
		case FOR:
			for_stmt(tok);
			break;
		}
	}
	return 0;
}




int lexer(char *tok[], char *str)
{

	int i = 0;
	char *token = strtok(str, DELIM);
	while (token != NULL)
	{
		for (int j = 0; token != NULL; j++)
		{
			// puts(token);
			tok[j] = token;
			token = strtok(NULL, DELIM);
			last = j;
		}

	}
	if (strcmp(tok[0], "print") == 0)
	{
		return type = PRINT;
	}

	if (!strcmp(tok[0], "input"))
	{
		return (type = INPUT);
	}
	if (!strcmp(tok[0], "mem"))
	{
		return (type = MEM);
	}

	if (!strcmp(tok[0], "let"))
	{
		return (type = LET);
	}

	if (!strcmp(tok[0], "if"))
	{
		return (type = IF);
	}
	if (!strcmp(tok[0], "then"))
	{
		return (type = THEN);
	}
	if (!strcmp(tok[0], "while"))
	{
		return (type = WHILE);
	}
	if (!strcmp(tok[0], "for"))
	{
		return (type = FOR);
	}
	if (!strcmp(tok[0], "char"))
	{
		return (type = CHAR);
	}
	if (!strcmp(tok[0], "int"))
	{
		return (type = INT);
	}
	if (isalpha(*tok[0]))
	{
		return (type = VARIABLE);
	}

}

char *strings(char *tok[], int lv)
{

	char ch = '\"';
	char *index1, *index2;
	int p1, p2;
	index1 = strchr(tok[lv], ch);
	index2 = strrchr(tok[last], ch);
	p1 = index1 - str;
	p2 = index2 - str;

	if (*tok[lv] == '\"' && str[p2 + 1] == '\0')
	{
		for (int i = p1 + 1; i < p2; i++)
		{
			if (str[i] == '\0')
			{

				lateral[i - p1 - 1] = ' ';
				printf(" ");
			}
			else
			{

				lateral[i - p1 - 1] = str[i];
				printf("%c", str[i]);
			}
		}
	}

	// puts(lateral);
	return lateral;
}



int doinput(char *tok[])
{
	char *st;
	st = malloc(100 * sizeof(char));
	cnt++;
	strcpy(var[cnt - 1].nom, tok[1]);
	printf("-->: ");
	fgets(st, 2048, stdin);
	if (isdigit(*st))
	{
		var[cnt - 1].valeur = atoi(st);
	}
	else
		strcpy(var[cnt - 1].string, st);
}

void memoire(char *tok[])
{
	for (int i = 0; i < 10; i++)
	{
		if (var[i].valeur)
		{
			printf("\n%s  = %d", var[i].nom, var[i].valeur);
		}
		else if (var[i].string)
		{

			printf("\n%s  = %s", var[i].nom, var[i].string);
		}
	}

}

int assignement(char *tok[])
{
	strings(tok, 2);


	if (tok[1] == NULL || tok[2] == NULL)
	{
		return 0;
	}

	if (*tok[1] == '=')
	{
		cnt++;
		strcpy(var[cnt - 1].nom, tok[0]);
		if (isdigit(*tok[2]))
		{
			sscanf(tok[2], "%d", &m);
			var[cnt - 1].valeur = atoi(tok[2]);
		}
		else
		{

			strcpy(var[cnt - 1].string, lateral);


		}
	}


}


void print_var(char *tok[])
{
	int i;
	for (i = 0; i <= 10; i++)
	{
		if (!strcmp(tok[1], var[i].nom) && last == 1)

		{
			if (var[i].valeur)
			{
				printf("%d", var[i].valeur);
			}
			else if (var[i].string)
			{
				printf("%s", var[i].string);
			}
		}
	}
}

float expressions(char *tok[], int lv)
{

	if (tok[lv + 1] == NULL)
	{
		return 0;
	}

	if (!strcmp(tok[lv + 1], "+"))
	{
		x = atof(tok[lv]);
		y = atof(tok[lv + 2]);
		if (isdigit(*tok[lv + 2]))
		{
			for (int i = 0; i <= 10; i++)
			{
				if (strcmp(tok[lv], var[i].nom) == 0)

				{

					x = var[i].valeur;
				}
			}
			y = atof(tok[lv + 2]);
			printf("%.2f", x + y);
		}
		else if (isdigit(*tok[lv]))
		{
			for (int i = 0; i <= 10; i++)
			{
				if (strcmp(tok[lv + 2], var[i].nom) == 0)

				{

					y = var[i].valeur;
				}
			}
			x = atof(tok[lv]);
			printf("%.2f", x + y);
		}
		else if (isalpha(*tok[lv]) && isalpha(*tok[lv + 2]))
		{
			for (int i = 0; i <= 10; i++)
			{
				if (strcmp(tok[lv + 2], var[i].nom) == 0)

				{

					y = var[i].valeur;
				}

				if (strcmp(tok[lv], var[i].nom) == 0)
				{
					x = var[i].valeur;
				}

			}
			// x = atof(tok[lv]);
			printf("%.2f", x + y);
		}

	}

	if (!strcmp(tok[lv + 1], "-"))
	{
		x = atof(tok[lv]);
		y = atof(tok[lv + 2]);
		if (isdigit(*tok[lv + 2]))
		{
			for (int i = 0; i <= 10; i++)
			{
				if (strcmp(tok[lv], var[i].nom) == 0)

				{

					x = var[i].valeur;
				}
			}
			y = atof(tok[lv + 2]);
			printf("%.2f", x - y);
		}
		else if (isdigit(*tok[lv]))
		{
			for (int i = 0; i <= 10; i++)
			{
				if (strcmp(tok[lv + 2], var[i].nom) == 0)

				{

					y = var[i].valeur;
				}
			}
			x = atof(tok[lv]);
			printf("%.2f", x - y);
		}
		else if (isalpha(*tok[lv]) && isalpha(*tok[lv + 2]))
		{
			for (int i = 0; i <= 10; i++)
			{
				if (strcmp(tok[lv + 2], var[i].nom) == 0)

				{

					y = var[i].valeur;
				}

				if (strcmp(tok[lv], var[i].nom) == 0)
				{
					x = var[i].valeur;
				}

			}

			printf("%.2f", x - y);
		}

	}


	if (!strcmp(tok[lv + 1], "*"))
	{
		x = atof(tok[lv]);
		y = atof(tok[lv + 2]);
		if (isdigit(*tok[lv + 2]))
		{
			for (int i = 0; i <= 10; i++)
			{
				if (strcmp(tok[lv], var[i].nom) == 0)

				{

					x = var[i].valeur;
				}
			}
			y = atof(tok[lv + 2]);
			printf("%.2f", x * y);
		}
		else if (isdigit(*tok[lv]))
		{
			for (int i = 0; i <= 10; i++)
			{
				if (strcmp(tok[lv + 2], var[i].nom) == 0)

				{

					y = var[i].valeur;
				}
			}
			x = atof(tok[lv]);
			printf("%.2f", x * y);
		}
		else if (isalpha(*tok[lv]) && isalpha(*tok[lv + 2]))
		{
			for (int i = 0; i <= 10; i++)
			{
				if (strcmp(tok[lv + 2], var[i].nom) == 0)

				{

					y = var[i].valeur;
				}

				if (strcmp(tok[lv], var[i].nom) == 0)
				{
					x = var[i].valeur;
				}

			}

			printf("%.2f", x * y);
		}

	}

	if (!strcmp(tok[lv + 1], "/"))
	{
		x = atof(tok[lv]);
		y = atof(tok[lv + 2]);
		if (isdigit(*tok[lv + 2]))
		{
			for (int i = 0; i <= 10; i++)
			{
				if (strcmp(tok[lv], var[i].nom) == 0)

				{

					x = var[i].valeur;
				}
			}
			y = atof(tok[lv + 2]);
			printf("%.2f", x / y);
		}
		else if (isdigit(*tok[lv]))
		{
			for (int i = 0; i <= 10; i++)
			{
				if (strcmp(tok[lv + 2], var[i].nom) == 0)

				{

					y = var[i].valeur;
				}
			}
			x = atof(tok[lv]);
			printf("%.2f", x / y);
		}
		else if (isalpha(*tok[lv]) && isalpha(*tok[lv + 2]))
		{
			for (int i = 0; i <= 10; i++)
			{
				if (strcmp(tok[lv + 2], var[i].nom) == 0)

				{

					y = var[i].valeur;
				}

				if (strcmp(tok[lv], var[i].nom) == 0)
				{
					x = var[i].valeur;
				}

			}

			printf("%.2f", x / y);
		}

	}
	if (!strcmp(tok[lv + 1], ">"))
	{
		for (int i = 0; i <= 10; i++)
		{
			if (strcmp(tok[lv], var[i].nom) == 0)

			{

				x = var[i].valeur;
			}
			if (strcmp(tok[lv + 2], var[i].nom) == 0)

			{

				y = var[i].valeur;
			}
		}
		// x = atof(tok[lv]);
		// y = atof(tok[lv + 2]);
		if (x > y)
		{
			// printf("True");
			return true;
		}
		else
			// printf("False");
			return false;
	}
	if (!strcmp(tok[lv + 1], "<"))
	{
		for (int i = 0; i <= 10; i++)
		{
			if (strcmp(tok[lv], var[i].nom) == 0)

			{

				x = var[i].valeur;
			}
			if (strcmp(tok[lv + 2], var[i].nom) == 0)

			{

				y = var[i].valeur;
			}
		}
		// y = atof(tok[lv + 2]);
		if (x < y)
		{
			// printf("True");
			return true;
		}
		else
			// printf("False");
			return false;
	}
	if (!strcmp(tok[lv + 1], "=="))
	{
		for (int i = 0; i <= 10; i++)
		{
			if (strcmp(tok[lv], var[i].nom) == 0)

			{

				x = var[i].valeur;
			}
			if (strcmp(tok[lv + 2], var[i].nom) == 0)

			{

				y = var[i].valeur;
			}
		}

		// y = atof(tok[lv + 2]);
		if (x == y)
		{
			// printf("True");
			return true;
		}
		else
			// printf("False");
			return false;
	}

}

void if_stmt(char *tok[])
{

	char *s1;
	char *toks[25] = { NULL };
	s1 = malloc(2048 * sizeof(char));
	printf("  :  ");
	fgets(s1, 2048, stdin);
	s1[strlen(s1) - 1] = '\0';
	if (*s1 == '\0')
	{
		printf("\n>>   ");
		fgets(s1, 2048, stdin);
	}
	else
		type = lexer(toks, s1);
	if (expressions(tok, 1))
	{
		expressions(tok, 0);
		switch (type)
		{

		case PRINT:
			strings(toks, 1);
			expressions(toks, 1);
			print_var(toks);
			address(toks);
			break;
		case INPUT:
			doinput(toks);
			break;
		case IF:
			if_stmt(toks);
			break;
		case VARIABLE:
			assignement(toks);
			break;
		case FOR:
			for_stmt(toks);
			break;
		case WHILE:
			while_stmt(toks);
			break;
		}

	}
}

void while_stmt(char *tok[])
{

	char *s2;
	char *toks[25] = { NULL };
	s2 = malloc(2048 * sizeof(char));
	printf("  :  ");
	fgets(s2, 2048, stdin);
	s2[strlen(s2) - 1] = '\0';
	if (*s2 == '\0')
	{
		printf("\n>>   ");
		fgets(s2, 2048, stdin);
	}
	else
		type = lexer(toks, s2);
	while (expressions(tok, 1))
	{
		expressions(tok, 0);
		switch (type)
		{

		case PRINT:
			printf(" ");
			strings(toks, 1);
			expressions(toks, 1);
			print_var(toks);
			address(toks);
			break;
		case INPUT:
			doinput(toks);
			break;
		case IF:
			if_stmt(toks);
			break;
		case FOR:
			for_stmt(toks);
			break;
		}
	}

}


void address(char *tok[])
{
	if (*tok[1] == '@')
		// printf("%p", &tok[1]);
		for (int i = 0; i <= 10; i++)
		{
			printf("%p", var[0].nom);
		}
}

int var_type(char *tok[])
{
	int *p;
	p = tok[1];
	if (type == CHAR)
	{
		cnt++;
		tok[1] = malloc(sizeof(char));
		strcpy(var[cnt - 1].nom, tok[1]);
		strcpy(var[cnt - 1].string, NULL);

	}
	if (type == INT)
	{
		cnt++;
		p = malloc(sizeof(int));
		strcpy(var[cnt - 1].nom, p);
		var[cnt - 1].valeur = 0;

	}


}

void for_stmt(char *tok[])
{
	int i;
	char *s3;
	char *toks[25] = { NULL };
	s3 = malloc(2048 * sizeof(char));
	printf("  :  ");
	fgets(s3, 2048, stdin);
	s3[strlen(s3) - 1] = '\0';
	if (*s3 == '\0')
	{
		printf("\n>>   ");
		fgets(s3, 2048, stdin);
	}
	// else
	type = lexer(toks, s3);
	if (*tok[2] == '=' && !strcmp(tok[4], "to"))
	{
		for (i = atoi(tok[3]); i <= atoi(tok[5]); i++)
		{
			expressions(toks, 0);

			switch (type)
			{
			case PRINT:
				strings(toks, 1);
				printf("\n");
				expressions(toks, 1);
				print_var(toks);
				address(toks);
				if (!strcmp(toks[1], tok[1]))
				{
					printf("%d", i);
				}
				break;

			case INPUT:
				doinput(toks);
				break;
			case IF:
				if_stmt(toks);
				break;
			case WHILE:
				while_stmt(toks);
				break;
			case FOR:
				for_stmt(toks);
				break;

			}
		}
	}
}