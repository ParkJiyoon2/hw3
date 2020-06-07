
#include <stdio.h>
#include <string.h>
#include "page.h"

static struct frame *free_list;

struct frame * get_free_frame() {
	free_list = free_list->n;			
	return free_list;					
}										

void page_init() {

	struct frame *p;		//previous
	struct frame *n;		//next

	// initialize pg_frames
	memset(page_frame, 0, sizeof(page_frame));	

	for (int i = 0; i < MAX_FRAMES; i++) {
		// fill in free page frames list
		page_frame[i].pfn = i;
		if (i > 0)
			page_frame[i].p = &page_frame[i - 1];

		if (i < MAX_FRAMES - 1)
			page_frame[i].n = &page_frame[i + 1];
	}
	free_list = &page_frame[0];	
}


