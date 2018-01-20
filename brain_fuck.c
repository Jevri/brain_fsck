#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
fsize(FILE *fp)
{
  int size = 0;
  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  return size;
}

int //return size of result
load_file_to_memory(const char *filename, char **result) 
{
  FILE *fp = fopen(filename, "r");
  if (f == NULL) 
    { 
      *result = NULL;
      return -1;
    } 
  int size = fsize(fp);
  *result = malloc(size + 1);
  if (size != fread(*result, sizeof(char), size, fp)) 
    { 
      free(*result);
      return -2;
    } 
  fclose(f);
  (*result)[size] = 0;
  return size + 1;
}

int bf(int,char*);

int
main(int argc, char *argv[])
{
  char * file_path = (argc >= 2) ? argv[1] : "script.bf";
  char * str;
  int str_length = load_file_to_memory(file_path, &str);
  if (str_length < 0)
    {
      puts("error loading file");
      return 1;
    }
  bf(str_length, str);
  return 0;
}

int get_op(char *script, int script_length, int *index, char * op);

int
bf(int script_length, char * script)
{
  int memory[30000] = {0};
  int mem_point = 512;
  int loop_stack[256];
  int loop_top = 0;
  int skip_loop_end = 0;
  int i;
  char op[100];
  for (i = 0; i < script_length -1; i++)
    {
      
      switch (get_op(script, script_length, &i, op))
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
	    while (script[++i] != ']' || skip_loop_end)
	      if (script[i] == '[')
		skip_loop_end++;
              else if (str[i] == ']')
		skip_loop_end--;
          break;
	case ']':
          i = loop_stack[--loop_top];
	  break;
	case '.':
	  putchar(memory[mem_point]);
	  break;
	case ',':
	  memory[mem_point] = getchar();
	  break;
	case ':':
	  printf("%d\n",memory[mem_point]);
	  break;
	case '#':
	  int to;
	  int from;
	  if (30000 - mem_point < 5)
	    {
	      to = 2999;
	      from = to - 10;
	    }
	  else if (mem_point < 5)
	    {
	      from = 0;
	      to = from + 10;
	    }
	  else
	    {
	      to = mem_point + 5;
	      from = mem_point - 5;
	    }
	  int j;
	  for (j = from; j < to; j++)
	    if (j == mem_point)
	      printf("\n%d<\n", memory[j]);
	    else
	      printf("\n%d\n", memory[j]);
	  break;
	case '0':
	  memory[mem_point] = atoi(op);
	  break;
	  
	}
    }
  return 0;
}

#include <ctype.h>

int
get_op(char *script,int script_length, int *index, char * op)
{
  if (!isdigit(script[*index]))
    return script[*index];
  int i;
  for (i = 0; isdigit(script[*index]); i++)
    {
      op[i] = script[*index];
      *index++;
    }
  *index--;
  op[i] = 0;
  return '0';
}

