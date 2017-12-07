#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
ae_load_file_to_memory(const char *filename, char **result) 
{
  int size = 0;
  FILE *f = fopen(filename, "r");
  if (f == NULL) 
    { 
      *result = NULL;
      return -1; // -1 means file opening fail 
    } 
  fseek(f, 0, SEEK_END);
  size = ftell(f);
  fseek(f, 0, SEEK_SET);
  *result = (char *)malloc(size+1);
  if (size != fread(*result, sizeof(char), size, f)) 
    { 
      free(*result);
      return -2; // -2 means file reading fail 
    } 
  fclose(f);
  (*result)[size] = 0;
  return size;
}

int
main(int argc,char *argv[])
{
  int memory[30000] = {0};
  int mem_point = 512;
  int loop_stack[256];
  int loop_top = 0;
  char * file_path = (argc >= 2) ? argv[1] : "script.bf";
  char * str;
  int end = 0;
  //printf("%s\n",file_path);
  int str_length = ae_load_file_to_memory(file_path,&str);
  if (str_length < 0)
    {
      puts("error loading file");
      return 1;
    }
  //printf("%d\n",str_length);
  int i;
  for (i = 0; i < str_length; i++)
    {
/*
      printf("%d %c %d %d %d\n",
	     i,
	     str[i],
	     loop_top,
	     mem_point - 512,
	     memory[mem_point]);
*/
      switch (str[i])
	{
	case '+':
	  memory[mem_point]++;
	  break;
	case '-':
	  memory[mem_point]--;
	  break;
	case '<':
	  mem_point--;
	  break;
	case '>':
	  mem_point++;
	  break;
	case '[':
          if (memory[mem_point])
	    loop_stack[loop_top++] = i - 1;
          else
	    while (str[++i] != ']' || end)
	      if (str[i] == '[')
		end++;
              else if (str[i] == ']')
		end--;
          break;
	case ']':
          i = loop_stack[--loop_top];
	  break;
	case '.':
	  putchar(memory[mem_point]);
//putchar('-');
	  break;
	case ',':
	  memory[mem_point] = getchar();
	  break;
	}
    }
  putchar('\n');
  return 0;
}
