#ifndef ENDIAN_H
#define ENDIAN_H "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_bios_arm/branches/haspoc/arm64_v8/plf_bios_arm/endian.h $ $Revision: 18831 $"

static inline uint16_t __i386_bswap_16(uint16_t x)
{
	__asm__("xchgb %b0,%h0\n\t"
		: "=q" (x)
		: "0" (x));
	return x;
}

static inline uint32_t __i386_bswap_32(uint32_t x)
{
	__asm__("xchgb %b0,%h0\n\t"
		"rorl $16,%0\n\t"
		"xchgb %b0,%h0"
		: "=q" (x)
		: "0" (x));
	return x;
}


#define __bswap_constant_16(x) \
	((uint16_t)((((uint16_t)(x) & 0x00ff) << 8) | \
		(((uint16_t)(x) & 0xff00) >> 8)))

#define __bswap_constant_32(x) \
	((uint32_t)((((uint32_t)(x) & 0x000000ffU) << 24) | \
		(((uint32_t)(x) & 0x0000ff00U) <<  8) | \
		(((uint32_t)(x) & 0x00ff0000U) >>  8) | \
		(((uint32_t)(x) & 0xff000000U) >> 24)))

#define __bswap_16(x) \
	((uint16_t)(__builtin_constant_p(x) ? \
	__bswap_constant_16(x) : \
	__i386_bswap_16(x)))


#define __bswap_32(x) \
	((uint32_t)(__builtin_constant_p(x) ? \
	__bswap_constant_32(x) : \
	__i386_bswap_32(x)))


#define __BYTE_ORDER __LITTLE_ENDIAN

#define le32_to_cpup(x) (*(uint32_t *)(x))
#define cpu_to_le16p(x) (*(uint16_t*)(x))

#define ntohl(x)        __bswap_32(x)
#define htonl(x)        __bswap_32(x)
#define ntohs(x)        __bswap_16(x)
#define htons(x)        __bswap_16(x)
#define cpu_to_le32(x)  (x)
#define cpu_to_le16(x)  (x)
#define cpu_to_be32(x)  __bswap_32(x)
#define cpu_to_be16(x)  __bswap_16(x)
#define le32_to_cpu(x)  (x)
#define le16_to_cpu(x)  (x)
#define be32_to_cpu(x)  __bswap_32(x)
#define be16_to_cpu(x)  __bswap_16(x)

#endif //ENDIAN_H

