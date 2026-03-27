// macros.h

#pragma once

#define GLOBAL(type, address) (*((type*)(address)))

#define DECL_FUNC(ret, name, ...)        typedef ret (*pfn_##name)(__VA_ARGS__);
#define IMPORT_FUNC(name, addr)          volatile unsigned int _addr_##name = addr; \
                                         pfn_##name name = (pfn_##name)_addr_##name;
