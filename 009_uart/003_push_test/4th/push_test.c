
#include <stdio.h>
//#include <stdarg.h>
typedef char *  va_list;
#define _INTSIZEOF(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )

#define va_start(ap,v)  ( ap = (va_list)&v + _INTSIZEOF(v) )
//#define va_arg(ap,t)    ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
//#define va_arg(ap,t)    (ap = ap + _INTSIZEOF(t), *(t *)(ap - _INTSIZEOF(t)))
#define va_arg(ap,t)    (*(t *)(ap = ap + _INTSIZEOF(t), ap - _INTSIZEOF(t)))
#define va_end(ap)      ( ap = (va_list)0 )	

struct  person{
	char *name;
	int  age;
	char score;
	int  id;
};

int push_test(const char *format, ...)
{
	int i;
	struct person per;

	va_list p;

	printf("arg1: %s\n", format);

	va_start(p, format);

	i = va_arg(p, int);
	printf("arg2 : %d\n", i);

	per = va_arg(p, struct person);
	printf("arg3: .name = %s, .age = %d, .socre=%c  .id=%d\n",\
		          per.name,   per.age,   per.score, per.id); 

	va_end(p); 

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
    // push_test("abcd",123,per,'c',2.79); 	
	 		
	return 0;
}	
