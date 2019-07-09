#include <stdio.h>
#include <stdlib.h>

#define MAXTOKENS 20
#define MAXTOKENLEN 30
#define MAXLEN 1024

int sp;
enum token_type { TYPE, QUALIFIER, IDENTIFIER };
char *qulify[] ={
	"char",
	"int",
	"long",
	"float",
	"double",
	"unsigned",
	"signed",
	"auto"
};

struct token_tag{
	enum token_type type;
	char string[MAXTOKENLEN];
};

enum token_type classify_string(char *string)
{
	int i = 0;
	enum token_type return_type;
	for(i = 0; i < sizeof(qulify) / sizeof(qulify[0]); i++){
		if(!strcmp(string, qulify[i])){
			return_type = QUALIFIER;
			return return_type;
		}
	}

	return_type = IDENTIFIER;
	return return_type;
}

void read_to_first_identifier(const char *temp)
{
	int i;
	char *p = string;
	while(!isalpha(*temp)){
		sp++;
		
	}

	while(isalnum(*temp) || (*temp == '_')){
		*p++ = *temp++;
	}
	*p = NULL;

	printf("identifier is %s, ", string);

}

void deal_with_function_args()
{
}

void deal_with_arrsys()
{
}

void deal_with_any_pointers()
{
}

void deal_with_declarator()
{
}

int main()
{
	char temp[MAXLEN];
	while(fgets(temp, MAXLEN, stdin) != NULL){
		sp = 0;
		read_to_first_identifier(temp);
		deal_with_declarator();
	}

	exit(EXIT_SUCCESS);
}
