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

/*��ũ���̸�, ������ �ƴմϴ�. �׸���, ��ũ�δ� ������ �ÿ� 1:1�� ������������ ���ε˴ϴ�*/
/* Byte-wise swap two items of size SIZE. */


/* size������ ũ���� a�� b�� ���޹����� ���� �ּҸ� ����Ʈ ������ ���� �ٲ��ִ� ��ũ�� �Լ���.  */
#define SWAP(a, b, size)			     \ /* ��ũ�� �Լ��� �� ������ ��ɾ �����ϸ� �� a,b�� size�� �޾� ��ġ�� �������ִ� �Լ���.*/
do									     \ /* \�� �ǹ̴� �ڵ� ��ü�� \n�� �͵� ���� �ƴ϶� �̾����ٴ� ���̴�. */
	{								      \
	register size_t __size = (size);	      \ /* register�� ���������� �޸𸮰� �ƴ� CPU register�� ���� ���������μ� 
																������ ����� �� �ӵ��� ���� �� �ִ�. ���� register�� �ڸ��� ���ٸ� �޸������Ѵ�.
																size_t �� ���ȯ�渶�� �ٸ� int���� �ƴ� ������ size�μ� ȯ���� �޶����� size�� �Ȱ��� ������*/
register char *__a = (a), *__b = (b);	      \		/* �Է¹��� a�� b�� �ּҸ� �Ҵ��Ѵ�.*/

/* a�� ���� �ּҰ��� �ӽ÷� �����ϴ� __tmp������ a�� �����ּҰ��� b�� ���� �ּҰ��� �Ҵ��ϰ� �����ּҷ� �Ѿ��
b�� ���� �ּҰ��� �Ҵ��Ų�� �� �� b�� �����ּҸ� ����Ų��.
�� swap�� size�� 0�� �� ������ �ݺ��Ѵ�.*/
do						      \
	{					      \
	char __tmp = *__a;	      \
	*__a++ = *__b;		      \
	*__b++ = __tmp;		      \
	} while (--__size > 0);	\ 
	} while (0)			/* �ѹ� �����ϴ� while���� ������ ��ũ�δ� �ϳ��� ��ɾ �����ϹǷ� ���� swap����� �ϴ� ������ �Լ�ó�� ����ϱ� ����
						��¿ �� ���� �ϳ��� do while������ ������ �Ѱ��̴�.*/

/* Discontinue quicksort algorithm when partition gets below this size.
   This particular magic number was chosen to work best on a Sun 4/260. */

   /* �������� ��Ƽ���� ũ�Ⱑ �� MAX_THRESH���� ���ٸ� �ǹ��� ������ �ʰ� �������� �����.*/
#define MAX_THRESH 4

/* Stack node declarations used to store unfulfilled partition obligations. */
/* �ϳ��� ���ó��� ��Ƽ������ ���ɰ��̴�. */
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

#define STACK_SIZE	(CHAR_BIT * sizeof(size_t)) /* ������ ������μ� CHAR_BIT�� charŸ�� ������ bit size�� ��Ÿ���� ��, 1byte��
												sizeof(size_t)�� �ϳ���............... */

/* ������ ����ϱ� ���� ������ �Լ����̴� */
#define PUSH(low, high)	((void) ((top->lo = (low)), (top->hi = (high)), ++top))  /* ���ñ���ü top�� lo���� �Է¹��� low�� �ϰ�
																				hi���� �Է¹��� high�� �ϰ� 
																				�����͸� �ϳ� �̵����� �������� ����Ų��.*/

#define	POP(low, high)	((void) (--top, (low = top->lo), (high = top->hi)))  /* ���������͸� �ϳ� ���ҽ��� ������ ������ ������
																				�Է¹��� low������ ���ñ���ü top�� low���� �Ҵ��Ű��
																				�Է¹��� high�� top�� hi�� �Ҵ��Ų��.*/

/* ������ ����°��� �˾ƺ���. stack�� ��ġ�� top���� �۴ٸ� ������ �����Ƿ� ������ ��ȯ�Ѵ�. */
#define	STACK_NOT_EMPTY	(stack < top)


/* Order size using quicksort.  This implementation incorporates
   four optimizations discussed in Sedgewick:
   1. Non- recursive �������� ������ ����ؼ� ������ �޴�.
   1. Non-recursive, using an explicit stack of pointer that store the
   next array partition to sort.  To save time, this maximum amount
   of space required to store an array of SIZE_MAX is allocated on the
   stack.  Assuming a 32-bit (64 bit) integer for size_t, this needs
   only 32 * sizeof(stack_node) == 256 bytes (for 64 bit: 1024 bytes).
   Pretty cheap, actually.

   2. �ǹ��� 3���� �Ѱ����� �����Ͼ� ����Ѵ�.
   2. Chose the pivot element using a median-of-three decision tree.
   This reduces the probability of selecting a bad pivot value and
   eliminates certain extraneous comparisons.

   3. ������ �Ϸ��� ��ü ������  MAX_THRESH ���� �۴ٸ� ������ ���߰�
   insertion sorting���� �����Ѵ�. �̴� ���� �翡�� ���� ����������
   ����Ͽ� �ӵ��� ����Ű�� �����̴�.
   3. Only quicksorts TOTAL_ELEMS / MAX_THRESH partitions, leaving
   insertion sort to order the MAX_THRESH items within each partition.
   This is a big win, since insertion sort is faster for small, mostly
   sorted array segments.

   4. �ǹ����� ū �κ��� ��Ƽ�ǵ��� �׻� ���� ���ÿ� ����ȴ�. �׸��� ���� ��Ƽ���� �������
   ó���Ѵ�.
   4. The larger of the two sub-partitions is always pushed onto the
   stack first, with the algorithm then concentrating on the
   smaller partition.  This *guarantees* no more than log (total_elems)
   stack size is needed (actually O(1) in this case)!  */


/* 
void *const pbase : �����Ϸ��� �񱳹迭�� �ּҴ�. void�� �����ͷμ� type�� ������ �����ʾ� �������� �̵��� �����Ӵ�.
size_t total_elems : ������ �����ϱ����� �񱳹迭�� �ε��� ��ü ������.
size_t size : ������ �����Ϸ��� �񱳹迭 ������ �� �������̴�.
__compar_fn_t ���ϴ� �Լ��� �����͸� ���Ѵ�. c�� �����Ǿ� �ִ� Ÿ���̴�. */
void _quicksort(void *const pbase, size_t total_elems, size_t size, __compar_fn_t cmp)
{
	register char *base_ptr = (char *)pbase;	/* �������Ϳ� ����Ǵ� ������ void������ ���� ���� �ּҴ� */

	/* Allocating SIZE bytes for a pivot buffer facilitates a better
	   algorithm below since we can do comparisons directly on the pivot. */
	char *pivot_buffer = (char *)__alloca(size);	/* __alloca�� ���ÿ� �����Ҵ��� �ϴ� �Լ��μ�
													char�� �������� Ÿ���� ������ size��ŭ ���ÿ� �Ҵ��Ѵ�. 
													�� ���ÿ� �Ҵ�� �޸𸮴� �Լ��� ������ ��� �������.  
													*/

	const size_t max_thresh = MAX_THRESH * size;	/* ������ �����μ� (�� MAX_THRESH���� * �ϳ��� �񱳴�� ������) ������ 
													���Ŀ� .....*/

	/* �񱳴���� 0����. */
	if (total_elems == 0)
		/* Avoid lossage with unsigned arithmetic below.  */
		return;

	/* ���� ������ ������ insertion sorting ������ �� ���� MAX_THRESH���� ���� ������? */
	if (total_elems > MAX_THRESH)
	{
		char *lo = base_ptr;	/* ���ô����� ó������ �ּ� */
		char *hi = &lo[size * (total_elems - 1)]; /* ���ô����� ó�� �ּҿ��� (�ϳ��� ������*(��ü ����-1))�� ��
												  �� ���ô����� �������� �ּ� */
		stack_node stack[STACK_SIZE];	/* stack ����ü�� �������ش�. */
		stack_node *top = stack + 1;	/* stack ����ü�� �ּҺ�����. ó�� �����ϳ��� ���ΰ� ���� �����ּҸ� ���´�.  */

		/* ������ ������� �ʴٸ� ����ؼ� �������� �����Ѵ�.*/
		while (STACK_NOT_EMPTY)
		{
			char *left_ptr;		/* �����ý� ���Ǵ� ���� �����ͷ� �ǹ����� ���� ������ ����� ��Ҵ�*/
			char *right_ptr;	/* �����ý� ���Ǵ� ���� �����ͷ� �ǹ����� ū ������ ����� ��Ҵ�*/

			char *pivot = pivot_buffer;	/* �ǹ��� �ּҸ� �����ϴ� ������ */

			/* Select median value from among LO, MID, and HI. Rearrange
			   LO and HI so the three values are sorted. This lowers the
			   probability of picking a pathological pivot value and
			   skips a comparison for both the LEFT_PTR and RIGHT_PTR in
			   the while loops. */

			/* �ǹ��� ���� �ո������� ���ϱ����ؼ��� 3���� ��ġ�� ���񱳰� �ʿ��ϴ�.
			���� ���� ��ġ�� ������ ��ġ �׸��� ��� ��ġ�� ���� ���ؾ��Ѵ�.
			�߰���ġ�� ���� ��� ������ ���Ѵ� */
			char *mid = lo + size * ((hi - lo) / size >> 1); /* (hi - lo) / size => ���� ������ġ�� ��������ġ�� ������ ���ϰ� >>1 �����ν� 2�γ������̵ȴ�.
															 �� �� ���ô�� �Ѱ��� �������� size�� �������μ� ���� ���� ��ġ���� �߰����� �������ִ����� ����Ʈ�����
															 �� �� �ִ�. �� �����ŭ ���� ���� ���ϸ� �߰���ġ�� �ּҰ� ���´�. */
			/* ���� ���ϴ� �Լ��� ����Ͽ� ���� �߰���ġ�� ���� ��������� ������ �۴ٸ�
			����� swap���ش�. */
			if ((*cmp) ((void *)mid, (void *)lo) < 0)
				SWAP(mid, lo, size);

			/* ���� ���ϴ� �Լ��� ����Ͽ� ���� ���� �������� ���� �߰��� ������ �۴ٸ�
			����� swap���ش�. ���� ������ ���� ũ�ٸ� lo < mid < hi �����̹Ƿ� jump_over�� �ǳʶ��. */
			if ((*cmp) ((void *)hi, (void *)mid) < 0)
				SWAP(mid, hi, size);
			else
				goto jump_over;
			/* ������ ������ ���� �߰����� swap�Ǿ����Ƿ� ���� ���ʰ��� swap����� �߰����� ���Ͽ� ��ġ�� �ٲپ� �ش�.*/
			if ((*cmp) ((void *)mid, (void *)lo) < 0)
				SWAP(mid, lo, size);
		jump_over:;
			memcpy(pivot, mid, size);	/* pivot�� mid�� size��ŭ �޸𸮸� �����Ѵ�. �ǹ����� �����صд�. */
			pivot = pivot_buffer;	/* ����ּҸ� ���� ���ÿ� �Ҵ�� �ǹ������� �ּҷ��Ѵ� */

			left_ptr = lo + size;	/* ���� ������ �����ʹ� ���� ���ʿ��� ���������� �Ѵ�.
									�ֳ��ϸ� ���� ���ʰ��� pivot���� ���Ҷ� pivot������ ���� ������ �����߱� �����̴�.*/
			right_ptr = hi - size;  /* ���� ������ �����ʹ� ���� �����ʿ��� ���������� �Ѵ�.
									�ֳ��ϸ� ���� �����ʰ��� pivot���� ���Ҷ� pivot������ ū ������ �����߱� �����̴�.*/

			/* Here's the famous ``collapse the walls'' section of quicksort.
			   Gotta like those tight inner loops!  They are the main reason
			   that this algorithm runs much faster than others. */

			/* ������ pivot������ ���ʰ� �������� ������ ���Ͽ� �ڸ��� ��ȯ���ִ� ������ �ݺ������� �Ѵ�. */
			do
			{
				/* ������������ ���� �ǹ����� ���Ͽ� �ǹ����� �� ũ�ٸ� �ùٸ� ��ġ�� �����Ƿ�
				���� ���� ���ϱ����� size��ŭ �����͸� �̵���Ų��. */
				while ((*cmp) ((void *)left_ptr, (void *)pivot) < 0)
					left_ptr += size;

				/* �������������� ���� �ǹ����� ���Ͽ� �ǹ����� �� �۴ٸ� �ùٸ� ��ġ�� �����Ƿ�
				���� ���� ���ϱ����� size��ŭ �����͸� �̵���Ų��. */
				while ((*cmp) ((void *)pivot, (void *)right_ptr) < 0)
					right_ptr -= size;

				/* ���� �������̵��� �ǹ������� ū ���� ���� ���� ������ ���� �������� �Ʒ� �ڵ带 �����ϰ� �ȴ�. 
				Ȥ�� �� �����Ͱ� ��������, �� �������� �������� ���� �Ʒ� �ڵ带 �����Ѵ�.
				�̸� �Ǻ��ϱ� ���� ������������ ��ġ�� ������ �������� ��ġ�� ���Ͽ� ���� ��Ȳ�� �ƴ϶��
				�� ���� swap�ؾ��ϹǷ� swap�� �Ѵ�. �� �� ���� ������ ���� ���ϱ����� ������ ��ġ�� �Ű��ش�. */
				if (left_ptr < right_ptr)
				{
					SWAP(left_ptr, right_ptr, size);
					left_ptr += size;
					right_ptr -= size;
				}

				/* ���ʰ� �������� �����Ͱ� ������ ��Ȳ�� ��� swap�� ������ ��Ȳ���� 
				������ ��� ������ �������� ū������ ��Ƽ���� �����Ǿ��� ��Ȳ�̴�. �� �̻�
				�񱳸� �����ʰ� ������. �ݺ�����*/
				else if (left_ptr == right_ptr)
				{
					left_ptr += size;
					right_ptr -= size;
					break;
				}
			} while (left_ptr <= right_ptr); /* Ȥ�� swap�� �̵��� �����͵��� ���ų� ���� �����Ĺ��ȴٸ� �ݺ����� ������. */

			/* Set up pointers for next iteration.  First determine whether
			   left and right partitions are below the threshold size.  If so,
			   ignore one or both.  Otherwise, push the larger partition's
			   bounds on the stack and continue sorting the smaller one. */

			/* �Ʒ��� �ڵ�� ������ �����Ͱ� ������ ������ ��Ȳ���� 
			left_ptr�� �ǹ��� �߽����� ������ ��Ƽ���� ó����ġ�̸�
			right_ptr�� ���� ��Ƽ���� ������ ��ġ�̴�. */

			/* ������ �����Ϳ��� ������ġ ������ ����(�������� ���� ��Ƽ���� ������)��
			insertion sorting�� �����ϱ⿡ ���� ȿ������ ��������	*/
			if ((size_t)(right_ptr - lo) <= max_thresh)
			{
				/* ���� �����Ϳ��� �����̾��� ������ ����(�������� ���� ��Ƽ���� ������)��
				insertion sorting�� �����ϱ⿡ ���� ȿ������ �������� ���� ��� �����ÿ��� �����ϱ�����
				���ÿ��� ����Ƽ���� ������ ��� ������ pop��Ų��. */
				if ((size_t)(hi - left_ptr) <= max_thresh)
					/* Ignore both small partitions. */
					POP(lo, hi);
				/* ������ ��Ƽ���� insertion sorting�� �����ϱ� ȿ������ ����� �ƴ϶��
				������ ������ ���� ���� �����͸� ������ ��Ƽ���� ó����ġ�� �����Ѵ�. */
				else
					/* Ignore small left partition. */
					lo = left_ptr;
			}

			/* ������ ���� ��Ƽ���� �������� ��� �����ؾ� �ϸ� �Ʒ��� �ڵ带 �����Ѵ�.
			���� �����Ϳ��� ����ġ ������ ����(�������� ���� ��Ƽ���� ������)��
			insertion sorting�� �����ϱ⿡ ���� ȿ������ �������� ������ ��Ƽ���� �����ÿ��� ���ܵǰ�
			���� ��Ƽ���� �� ������ ���� ���������� ������ ���� ���� ��Ƽ���� ������ ��ġ�� �Ҵ��Ѵ�. */
			else if ((size_t)(hi - left_ptr) <= max_thresh)
				/* Ignore small right partition. */
				hi = right_ptr;

			/* ���� ������ ��Ƽ�� ��� insertion sorting���� �������� ȿ�����̰�
			���� ��Ƽ���� �� ū �������� ���� ��Ƽ���� ���߿� ����ϱ����� ���ʰ� ������ ��ġ��
			���ÿ� �����Ѵ�.
			������ ��Ƽ���� �����ϱ����� ���� ����� ���� �����͸� ������ ��Ƽ���� ó����ġ�� �Ҵ��Ѵ�. */
			else if ((right_ptr - lo) > (hi - left_ptr))
			{
				/* Push larger left partition indices. */
				PUSH(lo, right_ptr);
				lo = left_ptr;
			}

			/* ���� ���� ��� �������� �����ؾ� �ϰ� ������ ��Ƽ���� �� ū �������� ������ ��Ƽ���� ���߿� ����ϱ� ����
			������ ��Ƽ���� ������ġ�� �� ��ġ�� ���ÿ� �����Ѵ�. 
			���� ��Ƽ���� �����ϱ����� ���� ����� �� �����͸� ���� ��Ƽ���� ��������ġ�� �Ҵ��Ѵ�. */
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

	/* �Ʒ��� insertion sorting�� �����ϴ� �ڵ��. ������ ��� */
#define min(x, y) ((x) < (y) ? (x) : (y))

	{
		char *const end_ptr = &base_ptr[size * (total_elems - 1)]; /* �����ϰ����ϴ� �迭�� ������ ���� ������ ��ġ */
		char *tmp_ptr = base_ptr;	/* �ӽ÷� ���ô��迭�� �ּҸ� ����Ų��. */
		char *thresh = min(end_ptr, base_ptr + max_thresh); /* ���Ѽ����� insert sorting�� ������ �� ��ġ�� ���Ѵ�.
															���� 5���ǰ��� �����ϴ� �迭�� 4�� ���ּ��̸�
															���� 3������ ���� ������ �迭�� 3�� ������ ���Ѽ��� �Ǵ°��̴�.*/
		register char *run_ptr;	/* ������ �����ϰԵǴ� �����ʹ� */

		/* Find smallest element in first threshold and place it at the
		   array's beginning.  This is the smallest array element,
		   and the operation speeds up insertion sort's inner loop. */

		/* ���������͸� �ӽ���ġ�� ���� ������ �ϰ� ���� �����ʹ� ���Ѽ��� ���� ������ ��� �̵��Ѵ�.
		���� �����ʹ� �������� ���� ã�´�. */
		for (run_ptr = tmp_ptr + size; run_ptr <= thresh; run_ptr += size)
			/* �ӽ���ġ�� ������������ġ�� ���� ���Ͽ� ���������Ͱ� �� �۴ٸ� �ӽ������ʹ� ������������ ��ġ�� �ȴ�.
			��, swap�� ������ ��ġ�� ���̴�. ���� �ӽ���ġ���� ������ġ�� �� ũ�ٸ� ��� �̵��Ͽ� ���� ���� ���Ѵ�.*/
		if ((*cmp) ((void *)run_ptr, (void *)tmp_ptr) < 0)
			tmp_ptr = run_ptr;

		/* ���� swap�ؾ��� �ӽ� ��ġ�� ������ġ�� �ƴ϶�� ���� swap�Ѵ�. �׷��� �������� ���� ������ġ�� �´�. */
		if (tmp_ptr != base_ptr)
			SWAP(tmp_ptr, base_ptr, size);

		/* Insertion sort, running from left-hand-side up to right-hand-side.  */

		/* ���������͸� swap�Ǿ� ������������ �� ������ġ�� �����Ϳ� ��ġ��Ų��. */
		run_ptr = base_ptr + size;

		/*	���������Ͱ� ������ ��ġ�� �����ų� �Ѿ�� �ʴ� �� ���� ���� �д� ������ �ݺ������Ѵ�. */
		while ((run_ptr += size) <= end_ptr)
		{
			tmp_ptr = run_ptr - size;	/* �񱳵� �ӽ� ��ġ�� ������������ �������̴�. */
			/* ������������ ���� �ӽ���ġ�� ������ �۴ٸ� �ݺ������� �ӽ� ��ġ�� ���������� �̵��Ѵ�.
			���������ʹ� �����Ȼ��¿��� �ӽ���ġ���� ���������� �����Ѵ�.*/
			while ((*cmp) ((void *)run_ptr, (void *)tmp_ptr) < 0)
				tmp_ptr -= size;

			/* ��� ���������� �̵��ϴ� �ӽ���ġ�� ���� �̵� ��������ġ�� ������ ���� ���� ���� ���´� */
			tmp_ptr += size; /* �ӽ���ġ�� �������� ����Ų��. */
			/* ���� �ӽ���ġ�� �̵���ġ�� ���� ��ġ�� ����Ű�� �ʴ´ٸ�  */
			if (tmp_ptr != run_ptr)
			{
				char *trav;

				trav = run_ptr + size;	/* �̵��������� ������ġ�� ����Ű�� ������. ��, ������ ���� ���� ��Ƽ���̴�. */
				/* ���� ������ġ�� ���� �̵������ͺ��� �۾����� �ʴ� �ٸ� �ݺ����� ���� */
				while (--trav >= run_ptr)
				{
					char c = *trav;
					char *hi, *lo;

					/* lo�� ���������� �̵��ϸ鼭 �ӽ���ġ���� �۾��������� �ݺ��Ѵ�. 
					for�� ������ ���� �� �� hi���� lo���� �ȴ�.*/
					for (hi = lo = trav; (lo -= size) >= tmp_ptr; hi = lo)
						*hi = *lo;	/* �ּҸ� �Ҵ��Ѵ�. */
					*hi = c; /* c�� ��ġ�� ����Ų��. */
				}
			}
		}
	}
}
