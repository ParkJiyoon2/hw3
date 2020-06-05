#include <stdio.h>
#include <string.h>
#include "page.h"

static struct frame *free_list;//물리기억장치?
// free frame list
struct frame * get_free_frame() // demand page 발생시
{
	// free_frame을 하나 가져온다.
	//gunwoo's code start!
	free_list =free_list->n;
	return free_list->p;
}

void page_init() //free_list를 초기화 한다
{
	struct frame *p;
	struct frame *n;

	// initialize pg_frames
	memset(page_frame, 0, sizeof(page_frame)); 
	
	//page_frame에 있는 frame들을 앞뒤로 연결하는 작업
	for (int i = 0 ; i < MAX_FRAMES ; i++) {
		// fill in free page frames list
		page_frame[i].pfn = i;
		if (i > 0)
			page_frame[i].p = &page_frame[i-1];

		if (i < MAX_FRAMES-1)
			page_frame[i].n = &page_frame[i+1];
	}
	free_list = &page_frame[0];
}

