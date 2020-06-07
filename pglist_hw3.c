
#include <stdio.h>
#include <string.h>
#include "page_HW3.h"

static struct frame *free_list;
// free frame list
struct frame * get_free_frame()
{       
	free_list = free_list->n; // 사용한 page_frame을free_list에서 제외
	return free_list;
}

void page_init()
{
	struct frame *p;
	struct frame *n;

	// initialize pg_frames
	memset(page_frame, 0, sizeof(page_frame));

	for (int i = 0 ; i < MAX_FRAMES ; i++) {
		// fill in free page frames list
		page_frame[i].pfn = i;
		if (i > 0)
			page_frame[i].p = &page_frame[i-1]; //전 page_frame

		if (i < MAX_FRAMES-1)
			page_frame[i].n = &page_frame[i+1]; //후 page_frame
	}
	free_list = &page_frame[0];
}


