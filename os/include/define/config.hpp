#ifndef INP_CONFIG_HPP
#define INP_CONFIG_HPP

/* Not same like define.hpp, but there a config a complex for definitions,
 * for #define not have filling anything, THROW to <define.hpp>,
 * for #define have a filling, OR have a complex function, try to THROW <config.hpp>
 ***/


//  __ATTRIBUTE__(())

// NOTHROW

// no calling func, remove any bunc to highly speed and reliable
#define ATT_LEAF __attribute__((__leaf__))

// 
#define ATT_COLD __attribute__((__cold__))

// 
#define ATT_HOT __attribute__((__hot__))

//
#define ATT_INLINE inline __attribute__((__always_inline__))

//  
#define ATT_NOTHROW __attribute__((__nothrow__))

// 
#define ATT_NORETURN __attribute__((__noreturn__))

// 
#define ATT_DEPR(msg) __attribute__((__deprecated__(msg)))

// 
#define ATT_FLATTEN __attribute__((__flatten__))

// 
#define ATT_NAKED __attribute__((__naked__))

// 
#define ATT_NONNULL(pointer) __attribute__((__nonnull__(pointer)))

// 
#define ATT_UNUSED __attribute__((__unused__))

// 
#define ATT_CONST __attribute__((__const__))

// 
#define ATT_USED __attribute__((__used__))

// 
#define ATT_FORMAT(...) __attribute__((__format__(__VA_ARGS__)))

// 
#define ATT_CONSTRUCTOR(priority) __attribute__((__constructor__(priority)))

// 
#define ATT_ALIAS(type) __attribute__((__alias__(type)))

// 
// 


// __attribute__((con))


// nothrow, leaf, cold and deprecated for function
#define ATT_NOHOT_DEPR(text)  \
__attribute__((__nothrow__, __leaf__, __cold__,__deprecated__(text) ))
#define ATT_NOCOLD_DEPR(text)  \
__attribute__((__nothrow__, __leaf__, __hot__,__deprecated__(text) ))

#define ATT_NOHOT_  \
__attribute__((__nothrow__, __leaf__, __cold__ ))
#define ATT_NOCOLD_  \
__attribute__((__nothrow__, __leaf__, __hot__ ))


// sys

#define LAPIC_BASE  0xFEE00000

#define LAPIC_ICR_LOW         (*(volatile uint32_t*)(LAPIC_BASE + 0x300))
#define LAPIC_ICR_HIGH        (*(volatile uint32_t*)(LAPIC_BASE + 0x310))
#define LAPIC_SPURIOUS        (*(volatile uint32_t*)(LAPIC_BASE + 0x0F0))

// sys

#endif