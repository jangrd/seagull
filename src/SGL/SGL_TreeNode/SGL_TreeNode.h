#ifndef __SGL_TREENODE__
#define __SGL_TREENODE__

#include <stddef.h>

typedef struct {
    size_t arena_idx;
    size_t parent;
    size_t first_child;
    size_t next_sibling;
} _SGL_TreeNode;

#define SGL_TREENODE_NULL SIZE_MAX

#endif // _SGL_TREENODE__
