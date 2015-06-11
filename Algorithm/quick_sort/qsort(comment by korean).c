/* Copyright (C) 1991, 1992, 1996, 1997, 1999 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Written by Douglas C. Schmidt (schmidt@ics.uci.edu).

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* If you consider tuning this algorithm, you should consult first:
   Engineering a sort function; Jon Bentley and M. Douglas McIlroy;
   Software - Practice and Experience; Vol. 23 (11), 1249-1265, 1993.  */

#include <alloca.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

/*매크로이며, 변수가 아닙니다. 그리고, 매크로는 컴파일 시에 1:1로 무조건적으로 맵핑됩니다*/
/* Byte-wise swap two items of size SIZE. */


/* size변수의 크기인 a와 b를 전달받으면 값의 주소를 바이트 단위로 서로 바꿔주는 매크로 함수다.  */
#define SWAP(a, b, size)			     \ /* 매크로 함수다 단 한줄의 명령어만 가능하며 두 a,b의 size를 받아 위치를 변경해주는 함수다.*/
do									     \ /* \의 의미는 코드 자체에 \n이 와도 끝이 아니라 이어진다는 말이다. */
	{								      \
	register size_t __size = (size);	      \ /* register란 변수저장을 메모리가 아닌 CPU register에 직접 저장함으로서 
																변수를 사용할 때 속도를 높일 수 있다. 만약 register에 자리가 없다면 메모리저장한다.
																size_t 는 사용환경마다 다른 int값이 아닌 고정된 size로서 환경이 달라져도 size는 똑같은 변수다*/
register char *__a = (a), *__b = (b);	      \		/* 입력받은 a와 b의 주소를 할당한다.*/

/* a의 현재 주소값을 임시로 저장하는 __tmp변수는 a의 현재주소값에 b의 현재 주소값을 할당하고 다음주소로 넘어가고
b의 현재 주소값에 할당시킨다 그 후 b는 다음주소를 가리킨다.
이 swap은 size가 0이 될 떄까지 반복한다.*/
do						      \
	{					      \
	char __tmp = *__a;	      \
	*__a++ = *__b;		      \
	*__b++ = __tmp;		      \
	} while (--__size > 0);	\ 
	} while (0)			/* 한번 진행하는 while문인 이유는 매크로는 하나의 명령어만 가능하므로 위의 swap기능을 하는 일종의 함수처럼 사용하기 위해
						어쩔 수 없이 하나의 do while문으로 구성을 한것이다.*/

/* Discontinue quicksort algorithm when partition gets below this size.
   This particular magic number was chosen to work best on a Sun 4/260. */

   /* 나누어진 파티션의 크기가 이 MAX_THRESH보다 적다면 피벗을 구하지 않고 퀵소팅을 멈춘다.*/
#define MAX_THRESH 4

/* Stack node declarations used to store unfulfilled partition obligations. */
/* 하나의 스택노드다 파티션으로 사용될것이다. */
typedef struct
{
	char *lo;
	char *hi;
} stack_node;

/* The next 4 #defines implement a very fast in-line stack abstraction. */
/* The stack needs log (total_elements) entries (we could even subtract
   log(MAX_THRESH)).  Since total_elements has type size_t, we get as
   upper bound for log (total_elements):
   bits per byte (CHAR_BIT) * sizeof(size_t).  */

#define STACK_SIZE	(CHAR_BIT * sizeof(size_t)) /* 스택의 사이즈로서 CHAR_BIT는 char타입 변수의 bit size를 나타낸다 즉, 1byte다
												sizeof(size_t)는 하나의............... */

/* 빠르게 사용하기 위해 구현된 함수들이다 */
#define PUSH(low, high)	((void) ((top->lo = (low)), (top->hi = (high)), ++top))  /* 스택구조체 top의 lo값을 입력받은 low로 하고
																				hi값을 입력받은 high로 하고 
																				포인터를 하나 이동시켜 다음곳을 가리킨다.*/

#define	POP(low, high)	((void) (--top, (low = top->lo), (high = top->hi)))  /* 스택포인터를 하나 감소시켜 이전의 스택을 가리켜
																				입력받은 low변수에 스택구조체 top의 low값을 할당시키고
																				입력받은 high에 top의 hi를 할당시킨다.*/

/* 스택이 비었는가를 알아본다. stack의 위치가 top보다 작다면 스택이 있으므로 참값을 반환한다. */
#define	STACK_NOT_EMPTY	(stack < top)


/* Order size using quicksort.  This implementation incorporates
   four optimizations discussed in Sedgewick:
   1. Non- recursive 포인터의 스택을 사용해서 구현을 햇다.
   1. Non-recursive, using an explicit stack of pointer that store the
   next array partition to sort.  To save time, this maximum amount
   of space required to store an array of SIZE_MAX is allocated on the
   stack.  Assuming a 32-bit (64 bit) integer for size_t, this needs
   only 32 * sizeof(stack_node) == 256 bytes (for 64 bit: 1024 bytes).
   Pretty cheap, actually.

   2. 피벗은 3값중 한가지를 선택하야 사용한다.
   2. Chose the pivot element using a median-of-three decision tree.
   This reduces the probability of selecting a bad pivot value and
   eliminates certain extraneous comparisons.

   3. 소팅을 하려는 전체 갯수가  MAX_THRESH 보다 작다면 소팅을 멈추고
   insertion sorting으로 소팅한다. 이는 적은 양에서 더욱 빠른소팅을
   사용하여 속도를 향상시키기 위함이다.
   3. Only quicksorts TOTAL_ELEMS / MAX_THRESH partitions, leaving
   insertion sort to order the MAX_THRESH items within each partition.
   This is a big win, since insertion sort is faster for small, mostly
   sorted array segments.

   4. 피벗보다 큰 부분의 파티션들은 항상 먼저 스택에 저장된다. 그리고 작은 파티션의 값들부터
   처리한다.
   4. The larger of the two sub-partitions is always pushed onto the
   stack first, with the algorithm then concentrating on the
   smaller partition.  This *guarantees* no more than log (total_elems)
   stack size is needed (actually O(1) in this case)!  */


/* 
void *const pbase : 소팅하려는 비교배열의 주소다. void형 포인터로서 type이 정해져 있지않아 포인터의 이동이 자유롭다.
size_t total_elems : 소팅을 진행하기위한 비교배열의 인덱스 전체 갯수다.
size_t size : 소팅을 진행하려는 비교배열 각각의 값 사이즈이다.
__compar_fn_t 비교하는 함수의 포인터를 말한다. c언어에 지정되어 있는 타입이다. */
void _quicksort(void *const pbase, size_t total_elems, size_t size, __compar_fn_t cmp)
{
	register char *base_ptr = (char *)pbase;	/* 레지스터에 저장되는 변수로 void형으로 받은 것의 주소다 */

	/* Allocating SIZE bytes for a pivot buffer facilitates a better
	   algorithm below since we can do comparisons directly on the pivot. */
	char *pivot_buffer = (char *)__alloca(size);	/* __alloca는 스택에 동적할당을 하는 함수로서
													char의 포인터형 타입의 지정된 size만큼 스택에 할당한다. 
													이 스택에 할당된 메모리는 함수가 끝나면 모두 사라진다.  
													*/

	const size_t max_thresh = MAX_THRESH * size;	/* 고정의 변수로서 (총 MAX_THRESH갯수 * 하나의 비교대상 사이즈) 값으로 
													추후에 .....*/

	/* 비교대상이 0개다. */
	if (total_elems == 0)
		/* Avoid lossage with unsigned arithmetic below.  */
		return;

	/* 소팅 대상들의 갯수가 insertion sorting 진행이 더 빠른 MAX_THRESH갯수 보다 많은가? */
	if (total_elems > MAX_THRESH)
	{
		char *lo = base_ptr;	/* 소팅대상들의 처음값의 주소 */
		char *hi = &lo[size * (total_elems - 1)]; /* 소팅대상들의 처음 주소에서 (하나의 사이즈*(전체 갯수-1))의 값
												  즉 소팅대상들의 마지막값 주소 */
		stack_node stack[STACK_SIZE];	/* stack 구조체를 선언해준다. */
		stack_node *top = stack + 1;	/* stack 구조체의 주소변수다. 처음 스택하나를 비어두고 다음 스택주소를 갖는다.  */

		/* 스택이 비어있지 않다면 계속해서 퀵소팅을 진행한다.*/
		while (STACK_NOT_EMPTY)
		{
			char *left_ptr;		/* 퀵소팅시 사용되는 왼쪽 포인터로 피벗보다 작은 값들이 저장될 장소다*/
			char *right_ptr;	/* 퀵소팅시 사용되는 왼쪽 포인터로 피벗보다 큰 값들이 저장될 장소다*/

			char *pivot = pivot_buffer;	/* 피벗의 주소를 저장하는 변수다 */

			/* Select median value from among LO, MID, and HI. Rearrange
			   LO and HI so the three values are sorted. This lowers the
			   probability of picking a pathological pivot value and
			   skips a comparison for both the LEFT_PTR and RIGHT_PTR in
			   the while loops. */

			/* 피벗의 값을 합리적으로 구하기위해서는 3가지 위치의 수비교가 필요하다.
			가장 왼쪽 위치와 오른쪽 위치 그리고 가운데 위치의 값을 비교해야한다.
			중간위치의 값을 가운데 값으로 정한다 */
			char *mid = lo + size * ((hi - lo) / size >> 1); /* (hi - lo) / size => 가장 왼쪽위치와 오른쪽위치의 갯수를 구하고 >>1 함으로써 2로나눈값이된다.
															 그 후 소팅대상 한개의 사이즈인 size를 곱함으로서 가장 왼쪽 위치에서 중간값이 떨어져있는지의 바이트사이즈를
															 알 수 있다. 그 사이즈만큼 왼쪽 값에 더하면 중간위치의 주소가 나온다. */
			/* 값을 비교하는 함수를 사용하여 만약 중간위치의 값이 가장왼쪽의 값보다 작다면
			가운데로 swap해준다. */
			if ((*cmp) ((void *)mid, (void *)lo) < 0)
				SWAP(mid, lo, size);

			/* 값을 비교하는 함수를 사용하여 만약 가장 오른쪽의 값이 중간의 값보다 작다면
			가운데로 swap해준다. 만약 오른쪽 값이 크다면 lo < mid < hi 순서이므로 jump_over로 건너띈다. */
			if ((*cmp) ((void *)hi, (void *)mid) < 0)
				SWAP(mid, hi, size);
			else
				goto jump_over;
			/* 위에서 오른쪽 값과 중간값이 swap되었으므로 가장 왼쪽값과 swap변경된 중간값과 비교하여 위치를 바꾸어 준다.*/
			if ((*cmp) ((void *)mid, (void *)lo) < 0)
				SWAP(mid, lo, size);
		jump_over:;
			memcpy(pivot, mid, size);	/* pivot에 mid를 size만큼 메모리를 복사한다. 피벗값을 저장해둔다. */
			pivot = pivot_buffer;	/* 비벗주소를 실제 스택에 할당된 피벗버퍼의 주소로한다 */

			left_ptr = lo + size;	/* 비교할 왼쪽의 포인터는 가장 왼쪽에서 다음값으로 한다.
									왜냐하면 가장 왼쪽값은 pivot값을 구할때 pivot값보다 작은 값으로 정렬했기 떄문이다.*/
			right_ptr = hi - size;  /* 비교할 오른쪽 포인터는 가장 오른쪽에서 이전값으로 한다.
									왜냐하면 가장 오른쪽값은 pivot값을 구할때 pivot값보다 큰 값으로 정렬했기 떄문이다.*/

			/* Here's the famous ``collapse the walls'' section of quicksort.
			   Gotta like those tight inner loops!  They are the main reason
			   that this algorithm runs much faster than others. */

			/* 구해진 pivot값으로 왼쪽과 오른쪽의 값들을 비교하여 자리를 교환해주는 연산을 반복적으로 한다. */
			do
			{
				/* 왼쪽포인터의 값과 피벗값을 비교하여 피벗값이 더 크다면 올바른 위치에 있으므로
				다음 값을 비교하기위해 size만큼 포인터를 이동시킨다. */
				while ((*cmp) ((void *)left_ptr, (void *)pivot) < 0)
					left_ptr += size;

				/* 오른쪽포인터의 값과 피벗값을 비교하여 피벗값이 더 작다면 올바른 위치에 있으므로
				다음 값을 비교하기위해 size만큼 포인터를 이동시킨다. */
				while ((*cmp) ((void *)pivot, (void *)right_ptr) < 0)
					right_ptr -= size;

				/* 위의 포인터이동중 피벗값보다 큰 왼쪽 값과 작은 오른쪽 값을 만났을때 아래 코드를 실행하게 된다. 
				혹은 두 포인터가 만났을때, 즉 같은값을 비교했을때 또한 아래 코드를 실행한다.
				이를 판별하기 위해 왼쪽포인터의 위치와 오른쪽 포인터의 위치를 비교하여 같은 상황이 아니라면
				두 값을 swap해야하므로 swap을 한다. 그 후 각각 다음의 값을 비교하기위해 포인터 위치를 옮겨준다. */
				if (left_ptr < right_ptr)
				{
					SWAP(left_ptr, right_ptr, size);
					left_ptr += size;
					right_ptr -= size;
				}

				/* 왼쪽과 오른쪽의 포인터가 만나느 상황은 모든 swap을 진행한 상황으로 
				왼쪽은 모두 작은값 오른쪽은 큰값으로 파티션이 구성되어진 상황이다. 더 이상
				비교를 하지않고 나간다. 반복문을*/
				else if (left_ptr == right_ptr)
				{
					left_ptr += size;
					right_ptr -= size;
					break;
				}
			} while (left_ptr <= right_ptr); /* 혹시 swap후 이동한 포인터들이 같거나 서로 지나쳐버렸다면 반복문을 나간다. */

			/* Set up pointers for next iteration.  First determine whether
			   left and right partitions are below the threshold size.  If so,
			   ignore one or both.  Otherwise, push the larger partition's
			   bounds on the stack and continue sorting the smaller one. */

			/* 아래의 코드는 양쪽의 포인터가 마주쳐 지나간 상황으로 
			left_ptr은 피벗을 중심으로 오른쪽 파티션의 처음위치이며
			right_ptr은 왼쪽 파티션의 마지막 위치이다. */

			/* 오른쪽 포인터에서 시작위치 값과의 차이(나누어진 왼쪽 파티션의 사이즈)가
			insertion sorting을 진행하기에 더욱 효율적인 사이즈라면	*/
			if ((size_t)(right_ptr - lo) <= max_thresh)
			{
				/* 왼쪽 포인터에서 끝값이었던 값과의 차이(나누어진 왼쪽 파티션의 사이즈)가
				insertion sorting을 진행하기에 더욱 효율적인 사이즈라면 양쪽 모두 퀵소팅에서 제외하기위해
				스택에서 두파티션의 정보가 담긴 스택을 pop시킨다. */
				if ((size_t)(hi - left_ptr) <= max_thresh)
					/* Ignore both small partitions. */
					POP(lo, hi);
				/* 오른쪽 파티션이 insertion sorting을 진행하기 효율적인 사이즈가 아니라면
				퀵소팅 진행을 위해 시작 포인터를 오른쪽 파티션의 처음위치로 지정한다. */
				else
					/* Ignore small left partition. */
					lo = left_ptr;
			}

			/* 위에서 왼쪽 파티션이 퀵소팅을 계속 진행해야 하면 아래의 코드를 실행한다.
			왼쪽 포인터에서 끝위치 값과의 차이(나누어진 왼쪽 파티션의 사이즈)가
			insertion sorting을 진행하기에 더욱 효율적인 사이즈라면 오른쪽 파티션은 퀵소팅에서 제외되고
			왼쪽 파티션의 퀵 소팅을 위해 비교포인터의 오른쪽 값을 왼쪽 파티션의 마지막 위치로 할당한다. */
			else if ((size_t)(hi - left_ptr) <= max_thresh)
				/* Ignore small right partition. */
				hi = right_ptr;

			/* 만약 양쪽의 파티션 모두 insertion sorting보다 퀵소팅이 효율적이고
			왼쪽 파티션이 더 큰 사이즈라면 왼쪽 파티션은 나중에 계산하기위해 왼쪽과 오른쪽 위치를
			스택에 저장한다.
			오른쪽 파티션을 소팅하기위해 비교할 대상의 시작 포인터를 오른쪽 파티션의 처음위치로 할당한다. */
			else if ((right_ptr - lo) > (hi - left_ptr))
			{
				/* Push larger left partition indices. */
				PUSH(lo, right_ptr);
				lo = left_ptr;
			}

			/* 만약 양쪽 모두 퀵소팅을 진행해야 하고 오른쪽 파티션이 더 큰 사이즈라면 오른쪽 파티션을 나중에 계산하기 위해
			오른쪽 파티션의 시작위치과 끝 위치를 스택에 저장한다. 
			왼쪽 파티션을 소팅하기위해 비교할 대상의 끝 포인터를 왼쪽 파티션의 마지막위치로 할당한다. */
			else
			{
				/* Push larger right partition indices. */
				PUSH(left_ptr, hi);
				hi = right_ptr;
			}
		}
	}

	/* Once the BASE_PTR array is partially sorted by quicksort the rest
	   is completely sorted using insertion sort, since this is efficient
	   for partitions below MAX_THRESH size. BASE_PTR points to the beginning
	   of the array to sort, and END_PTR points at the very last element in
	   the array (*not* one beyond it!). */

	/* 아래는 insertion sorting을 진행하는 코드다. 위에서 모든 */
#define min(x, y) ((x) < (y) ? (x) : (y))

	{
		char *const end_ptr = &base_ptr[size * (total_elems - 1)]; /* 소팅하고자하는 배열의 마지막 값의 포인터 위치 */
		char *tmp_ptr = base_ptr;	/* 임시로 소팅대상배열의 주소를 가리킨다. */
		char *thresh = min(end_ptr, base_ptr + max_thresh); /* 제한선으로 insert sorting을 진행할 끝 위치를 구한다.
															만약 5개의값이 존재하는 배열은 4가 끝주소이며
															만약 3개가의 값이 전부인 배열은 3이 소팅의 제한선이 되는것이다.*/
		register char *run_ptr;	/* 소팅을 진행하게되는 포인터다 */

		/* Find smallest element in first threshold and place it at the
		   array's beginning.  This is the smallest array element,
		   and the operation speeds up insertion sort's inner loop. */

		/* 진행포인터를 임시위치의 다음 값으로 하고 진행 포인터는 제한선을 넘지 않으며 계속 이동한다.
		진행 포인터는 가장작은 값을 찾는다. */
		for (run_ptr = tmp_ptr + size; run_ptr <= thresh; run_ptr += size)
			/* 임시위치와 진행포인터위치의 값을 비교하여 진행포인터가 더 작다면 임시포인터는 진행포인터의 위치가 된다.
			즉, swap을 진행할 위치인 것이다. 만약 임시위치보다 진행위치가 더 크다면 계속 이동하여 다음 값을 비교한다.*/
		if ((*cmp) ((void *)run_ptr, (void *)tmp_ptr) < 0)
			tmp_ptr = run_ptr;

		/* 만약 swap해야할 임시 위치가 시작위치가 아니라면 서로 swap한다. 그러면 가장작은 값이 시작위치에 온다. */
		if (tmp_ptr != base_ptr)
			SWAP(tmp_ptr, base_ptr, size);

		/* Insertion sort, running from left-hand-side up to right-hand-side.  */

		/* 진행포인터를 swap되어 가장작은값이 된 시작위치의 다음것에 위치시킨다. */
		run_ptr = base_ptr + size;

		/*	진행포인터가 마지막 위치를 만나거나 넘어가지 않는 한 다음 값을 읽는 행위를 반복진행한다. */
		while ((run_ptr += size) <= end_ptr)
		{
			tmp_ptr = run_ptr - size;	/* 비교될 임시 위치는 진행포인터의 이전값이다. */
			/* 진행포인터의 값이 임시위치의 값보다 작다면 반복적으로 임시 위치를 이전값으로 이동한다.
			진행포인터는 고정된상태에서 임시위치값만 이전값으로 진행한다.*/
			while ((*cmp) ((void *)run_ptr, (void *)tmp_ptr) < 0)
				tmp_ptr -= size;

			/* 계속 이전값으로 이동하던 임시위치의 값이 이동 포인터위치의 값보다 작은 값을 만난 상태다 */
			tmp_ptr += size; /* 임시위치의 다음값을 가리킨다. */
			/* 만약 임시위치와 이동위치가 같은 위치를 가리키지 않는다면  */
			if (tmp_ptr != run_ptr)
			{
				char *trav;

				trav = run_ptr + size;	/* 이동포인터의 다음위치를 가리키는 변수다. 즉, 소팅할 다음 작은 파티션이다. */
				/* 만약 다음위치의 값이 이동포인터보다 작아지지 않는 다면 반복문을 진행 */
				while (--trav >= run_ptr)
				{
					char c = *trav;
					char *hi, *lo;

					/* lo는 이전값으로 이동하면서 임시위치보다 작아질때까지 반복한다. 
					for문 루프를 진행 한 후 hi값은 lo값이 된다.*/
					for (hi = lo = trav; (lo -= size) >= tmp_ptr; hi = lo)
						*hi = *lo;	/* 주소를 할당한다. */
					*hi = c; /* c의 위치를 가리킨다. */
				}
			}
		}
	}
}
