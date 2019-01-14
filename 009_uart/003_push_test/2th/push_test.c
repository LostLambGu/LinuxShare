
#include <stdio.h>

struct person
{
	char *name;
	int age;
	char score;
	int id;
};

int push_test(const char * format, ...)
{
	char *p = (char *)&format;
	int i;
	struct person per;

	printf("arg1: %s\n", format);

	p = p + sizeof(char *);

	i = *((int *)p);
	p = p + sizeof(int);
	printf("arg2: %d\n", i);

	per = *((struct person *)p);
	p = p + sizeof(struct person);
	printf("arg3: .name = %s, .age = %d, .socre=%c  .id=%d\n",\
		          per.name,   per.age,   per.score, per.id); 

	return 0;
}

int main(int argc,char **argv)
{
	struct  person per={"www.100ask.org",10,'A',123};
	
	printf("sizeof(char   )=%d\n",sizeof(char   ));
	printf("sizeof(int    )=%d\n",sizeof(int    ));
	printf("sizeof(char  *)=%d\n",sizeof(char  *));
	printf("sizeof(char **)=%d\n",sizeof(char **));	
	printf("sizeof(struct  person)=%d\n",sizeof(struct  person));		
	
    //push_test("abcd");
    //push_test("abcd",123);	 
    push_test("abcd",123,per); 				
    //push_test("abcd",123,per,'c');  	 
    //push_test("abcd",123,per,'c',2.79); 	
	 		
	return 0;
}
