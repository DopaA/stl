#####   有几个需要注意的地方。
1. sgi_stl内部是将配置内存和构造对象分离的，通过*:operator new和:operator delete*配置内存,通过构造函数和析构函数构造对象和析构对象。
