#include "lxxml.h"
#include "download.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void type(lxml_node* node) {
	for (lxml_attrib* a = node->attributes; a; a = a->next) {
		if (strcmp(a->name, "comment") == 0) {
			printf("//%s\n", a->value);
		}
	}
	
	char* name;
	int apientry = 0;
   for (lxml_node* c = node->children; c; c = c->next_sibling) {
		if (strcmp(c->name, "name") == 0) {
			name = c->text;
		} 
		
		if (strcmp(c->name, "apientry") == 0) {
			apientry = 1;
		}
	}

	if (node->text) {
		size_t len = strlen(node->text);
		if (name)
			node->text[len-1] = '\0'; 

		if (!apientry) {
			printf("%s%s%c\n", node->text, name != NULL ? name : "", name != NULL ? ';' : ' ');
		} else {
			size_t name_len = strlen(name);
			char* new_text = (char*)malloc(len+name_len);
			char* s = node->text;
			while (s[0] != '(' && s[1] != '*') {
				s++;
			}
			s += 2;
			int displacement = s-node->text;
			for (int i = 0; i < displacement; ++i) {
				new_text[i] = node->text[i];
			}

			for (int i = 0; i < name_len; ++i) {
				new_text[i+displacement] = name[i];  
			}

			for (int i = 0; i < len-displacement; ++i) {
				new_text[i+displacement+name_len] = node->text[i+displacement];
			}

			printf("%s;\n", new_text);
			free(new_text);
		}
	}
	name = NULL;
}

void _enum(lxml_node* node) {
	char* name = NULL;
	char* value = NULL;

	for (lxml_attrib* a = node->attributes; a; a = a->next) {
		if (strcmp(a->name, "value") == 0) {
			value = a->value;
		} 
		if (strcmp(a->name, "name") == 0) {
			name = a->value;
		}
		if (strcmp(a->name, "comment") == 0) {
			printf("//%s\n", a->value);
		}
	}

	printf("#define %s %s\n", name, value != NULL ? value : "");
}

void node_to_c(lxml_node* node) {
	if (	strcmp(node->name, "remove") == 0 
		|| strcmp(node->name, "feature") == 0
		|| strcmp(node->name, "require") == 0) {
		return;
	} else if (strcmp(node->name, "type") == 0) {
    	type(node);
	} else if (strcmp(node->name, "enum") == 0) {
		//printf("enums found\n");
		_enum(node);
	} else if (strcmp(node->name, "comment") == 0) {
		printf("/*\n%s\n*/\n", node->text);
	}

   for (lxml_node* c = node->children; c; c = c->next_sibling)
		node_to_c(c);
}

int main(int argc, char** argv) {
	char* glxml = NULL;
	if (download_gl_xml(&glxml) > 0) {
		return 1;
	}
	
	lxml_node* root = 0;
	lxml_parse_node(glxml, &root);

	node_to_c(root);

	free(glxml);
}

