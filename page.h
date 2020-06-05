// free frame list

struct frame
{
    int pfn;

    struct frame *p; //previous
    struct frame *n; //next
};

#define MAX_FRAMES 0x1000            // 4K 개, 최대갯수
struct frame page_frame[MAX_FRAMES]; // 4K frames: 4K*4K=16M 메모리 까지 커버

struct frame *get_free_frame();

void page_init();

#define NPROC 10 // assume total # proc.

//TODO: 주소 공간을 늘려서, pagetable 크기를 늘려

#define ADDR_SIZE (1 << 16)               // 2**16 : 주소공간 : 16비트 컴퓨터
#define PG_SIZE 0x1000                    // 1 << 12 = 2**12 = 4K
#define PG_TBL_SIZE (ADDR_SIZE / PG_SIZE) // 2**4 = 16

//TODO: 0으로 초기화되어있는데, -1 로 초기화.
int pg_table[NPROC][PG_TBL_SIZE];
void print2D(int arr[][PG_TBL_SIZE]);