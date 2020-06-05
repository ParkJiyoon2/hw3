// free frame list

struct frame
{
    int pfn;

    struct frame *p; //previous
    struct frame *n; //next
};

#define MAX_FRAMES 0x100000          // 1M 개, 최대갯수
struct frame page_frame[MAX_FRAMES]; // 1M frames: 1M*4K=4GB 메모리 까지 커버

struct frame *get_free_frame();

void page_init();

#define NPROC 10 // assume total # proc.

//TODO: 주소 공간을 늘려서, pagetable 크기를 늘려

#define ADDR_SIZE 0x0        // 2**32 : 주소공간 : 32비트 컴퓨터
#define PG_SIZE 0x1000       // 1 << 12 = 2**12 = 4K
#define PG_TBL_SIZE 0x100000 // 2**20 =1M

//TODO: 0으로 초기화되어있는데, -1 로 초기화.
int pg_table[NPROC][PG_TBL_SIZE];
void print2D(int arr[][PG_TBL_SIZE]);
void print2D_(int pid, int arr[][PG_TBL_SIZE]);