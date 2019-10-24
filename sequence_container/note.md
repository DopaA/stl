1. 序列式容器，是指其中的元素都可序(ordered)，但是不一定有序(sorted)。
2. vetcor:array是静态空间，vector是动态空间
3. vector的迭代器是普通指针。这是由于vector维护的是一个连续线性空间，所以无论元素型别为何，普通指针都可以作为vector的迭代器。所以vector提供的是random access iterators。
4. list本身和list的节点是不同的结构,需要分开设计。stl list是一个双向链表。因此用的是bidirectional iterators。
5. list重要性质：插入操作和接合操作(splice)不会造成原有的list迭代器失效。vector并没有这个性质。vector的插入操作**可能**造成记忆体重新配置，从而导致原有的迭代器全部失效。list的删除操作(erase)也只有"指向被删除元素"的迭代器失效。
6. list不仅是一个双向链表，而且是一个环状的双向链表。它只需一个指针就可完整表现整个链表。
7. 如果让指针node指向刻意置于尾端的一个空白节点，node就能符合stl的前闭后开的区间的要求,成为last迭代器。即list的尾端end()是一个空白节点，node.next是begin();
8. 插入操作后，新节点将位于哨兵迭代器(标示出插入点)所指之节点的前方--这是stl对于插入操作的标准规范。
