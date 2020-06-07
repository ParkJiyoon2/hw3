// free frame list

struct frame {
	int pfn;				
	struct frame *p;		//pre frame
	struct frame *n;		//next frame 
};

#define MAX_FRAMES 0x1000		
struct frame page_frame[MAX_FRAMES];

struct frame * get_free_frame();		


void page_init();


#define NPROC		1 

#define ADDR_SIZE	(1<<16)		
#define PG_SIZE		0x1000		
#define PG_TBL_SIZE	(ADDR_SIZE / PG_SIZE)		

int pg_table[NPROC][PG_TBL_SIZE];


