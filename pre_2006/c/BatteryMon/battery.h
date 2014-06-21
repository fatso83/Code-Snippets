typedef struct NODE
{
	struct NODE *next;
	int value;
} Node;

#define BUFSIZE 100
#define INTERVAL 2
#define MAX_CHARGE 36000
#define TRUE 1
#define DEBUG 	fprintf( stderr,"Linjenummer:%d\n", __LINE__);\
		fflush(stderr);

Node* add_rate(int);
int average_power_usage(void);
int  minutes_left(int);
