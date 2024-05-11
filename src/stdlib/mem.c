#include <stdlib.h>
#include <sys/syscall.h>
#include <stddef.h>


typedef struct mlibc_mem_t
{
    ubase_t address;
	size_t used;
	size_t max;
	size_t total;
} mem_t;

typedef struct mlibc_block_item
{
	ubase_t pool_ptr;
	size_t prev;
	size_t next;
} block_item_t;

typedef struct mlibc_block_management
{
    mem_t parent;
	size_t mem_align_size;
	uint8_t *heap_ptr;
	block_item_t *lfree;
	block_item_t *heap_end;
} block_mgt_t;


#define HEAP_MAGIC 0x1ea0
#define MEM_MASK ((~(size_t)0) - 1)
#define MEM_USED_CODE(base) ((((ubase_t)(base)) & MEM_MASK) | 0x1)       // generate used code base on 'base'
#define MEM_FREE_CODE(base) ((((ubase_t)(base)) & MEM_MASK) | 0x0)      // generate free code base on 'base'
#define MEM_IS_USE(_mem)    \
                    (((base_t)(((block_item_t)(_mem))->pool_ptr)) & (~MEM_MASK))

#define MEM_ITEM_SIZE       MLIBC_ALIGN(sizeof(block_item_t), MLIBC_ALIGN_SIZE)
#define MIN_SIZE            12
#define MEM_MIN_SIZE        MLIBC_ALIGN(MIN_SIZE, MLIBC_ALIGN_SIZE)

mem_t *mlibc_mem_init(void* addr, size_t size)
{
    block_mgt_t *mem_mgt;
    block_item_t *mem_item;
    ubase_t mem_size;
    ubase_t begin_addr, end_addr;

    /* reserve place for block_mgt_t */
    mem_mgt = (block_mgt_t *)MLIBC_ALIGN((ubase_t)addr, MLIBC_ALIGN_SIZE);
    begin_addr = MLIBC_ALIGN((ubase_t)mem_mgt + sizeof(*mem_mgt), MLIBC_ALIGN_SIZE);
    end_addr = MLIBC_ALIGN_DOWN((ubase_t)addr + size, MLIBC_ALIGN_SIZE);

    /* reserve place for begin mem_item and end mem_item */
    if ((end_addr > (2 * MEM_ITEM_SIZE)) &&
        ((end_addr - 2 * MEM_ITEM_SIZE) >= begin_addr))
    {
        /* calculate the useful memory size */
        mem_size = end_addr - begin_addr - 2 * MEM_ITEM_SIZE;
    }
    else
    {
        /* error operation */
        return NULL;
    }

    /* init mem_mgt */
    memset(mem_mgt, 0, sizeof(*mem_mgt));
    mem_mgt->parent.address = begin_addr;
    mem_mgt->parent.total = mem_size;
    mem_mgt->mem_align_size = mem_size;
    mem_mgt->heap_ptr = (uint8_t *)begin_addr;

    /* init begin mem_item */
    mem_item = (block_item_t *)mem_mgt->heap_ptr;
    mem_item->pool_ptr = MEM_FREE_CODE(mem_mgt);
    /* point to end item */
    mem_item->next = mem_mgt->mem_align_size + MEM_ITEM_SIZE;
    mem_item->prev = 0;

    /* init end mem_item */
    mem_mgt->heap_end = (block_item_t *)&mem_mgt[mem_item->next];
    mem_mgt->heap_end->pool_ptr = MEM_USED_CODE(mem_mgt);
    /* point to end item itself */
    mem_mgt->heap_end->next = mem_mgt->mem_align_size + MEM_ITEM_SIZE;
    mem_mgt->heap_end->prev = mem_mgt->mem_align_size + MEM_ITEM_SIZE;

    /* initialize the lowest-free pointer to the start of the heap */
    mem_mgt->lfree = (mem_item *)(mem_mgt->heap_ptr);

    return &mem_mgt->parent;
}

void *mlibc_malloc(mem_t m, size_t size)
{
    size_t ptr, ptr2;
    block_mgt_t *mgt;
    block_item_t *mem, *mem2;

    if(size == 0 && m == NULL)
        return NULL;

    /* change mem_t to block_mgt_t */
    mgt = (block_mgt_t *)m;
    size = MLIBC_ALIGN(size, MLIBC_ALIGN_SIZE);

    if(size < MEM_MIN_SIZE)
        size = MEM_MIN_SIZE;
    
    if(size > mgt->mem_align_size)
    {
        /* error operation */
        return NULL;
    }

    /* Scan the list of memory blocks */
    for(ptr = (uint8_t *)(mgt->lfree) - mgt->heap_ptr; // calculate relative position of first free block
        ptr <= mgt->mem_align_size - size;
        ptr = ((block_item_t *)&(mgt->heap_ptr[ptr])))
    {
        mem = (block_item_t *)&mgt->heap_ptr[ptr];
        /* First-fit */
        /* mem is not used and big enough */
        if((!MEM_IS_USE(mem)) && (mem->next - (ptr + MEM_ITEM_SIZE)) >= size)
        {
            /* the rest place is big enough to c*/
            if((mem->next - (ptr + MEM_ITEM_SIZE) - size) >= (MEM_ITEM_SIZE + MEM_MIN_SIZE))
            {
                /* The remaining memory block is large enough to serve as a new memory block. */
                ptr2 = ptr + MEM_ITEM_SIZE + size;

                /* create mem2 */
                mem2 = (block_item_t *)&mgt->heap_ptr[ptr2];
                mem2->pool_ptr = MEM_FREE_CODE(mgt);
                mem2->next = mem->next;
                mem2->prev = mem;

                mem->next = ptr2;
                /* Check if the next position after mem2 is the end item.*/
                if (mem2->next != small_mem->mem_size_aligned + MEM_ITEM_SIZE)
                {
                    ((block_item_t *)&small_mem->heap_ptr[mem2->next])->prev = ptr2;
                }
                mgt->parent += (size + MEM_ITEM_SIZE);
            }
            else
            {
                mgt->parent.used += mem->next - ((uint8_t *)mem - mgt->heap_ptr);
            }

            if (small_mem->parent.max < small_mem->parent.used)
            {
                small_mem->parent.max = small_mem->parent.used;
            }
            
            mem->pool_ptr = MEM_USED_CODE(mgt);
            if(mem == mgt->lfree)
            {
                /* If mem is the first free memory block, then lfree needs to be moved to the next free memory block. */
                while(MEM_IS_USE(mgt->lfree) && mgt->lfree != mgt->heap_end)
                    mgt->lfree = (block_item_t *)&mgt->heap_ptr[mgt->lfree->next];

                if(mgt->lfree == mgt->heap_end)
                {
                    /* error operation */
                    return NULL;
                }
            }

            if (mem + MEM_ITEM_SIZE + size > mgt->heap_end ||
                (ubase_t)((uint8_t *)mem + MEM_ITEM_SIZE) % MLIBC_ALIGN_SIZE != 0 ||
                (((ubase_t)mem) & (MLIBC_ALIGN_SIZE - 1)) != 0)
            {
                /* error operation */
                return NULL;
            }

            return (uint8_t *)mem + MEM_ITEM_SIZE;
        }
    }

    return NULL;
}


