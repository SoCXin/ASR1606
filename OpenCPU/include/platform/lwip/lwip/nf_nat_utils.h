#ifndef __NF_NAT_UTILS_H__
#define __NF_NAT_UTILS_H__

typedef signed char     s8;
typedef unsigned char   u8;

typedef signed short    s16;
typedef unsigned short  u16;

typedef signed int      s32;
typedef unsigned int    u32;

typedef signed char     __s8;
typedef unsigned char   __u8;

typedef signed short    __s16;
typedef unsigned short  __u16;

typedef signed int      __s32;
typedef unsigned int    __u32;

typedef __u16           __le16;
typedef __u16           __be16;
typedef __u32           __le32;
typedef __u32           __be32;

typedef __u16           __sum16;
typedef __u32           __wsum;

typedef signed char	    int8_t;
typedef unsigned char   u_int8_t;
typedef short		    int16_t;
typedef unsigned short	u_int16_t;
typedef int		        int32_t;
typedef unsigned int	u_int32_t;

typedef unsigned char	u_char;



#define ETH_ALEN        6 /* Octets in one ethernet addr   */

#define ___constant_swab16(x) ((__u16)(				\
	(((__u16)(x) & (__u16)0x00ffU) << 8) |			\
	(((__u16)(x) & (__u16)0xff00U) >> 8)))

#define ___constant_swab32(x) ((__u32)(				\
	(((__u32)(x) & (__u32)0x000000ffUL) << 24) |		\
	(((__u32)(x) & (__u32)0x0000ff00UL) <<  8) |		\
	(((__u32)(x) & (__u32)0x00ff0000UL) >>  8) |		\
	(((__u32)(x) & (__u32)0xff000000UL) >> 24)))

#ifndef htonl
#define htonl(x) ___constant_swab32(x)
#define ntohl(x) ___constant_swab32(x)
#define htons(x) ___constant_swab16(x)
#define ntohs(x) ___constant_swab16(x)
#endif

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define BUG_ON() LWIP_DIAG(DIAG_ON_DEBUG, lwip_nf_nat_utils_62, "Bug at func %s line %d\n", __FUNCTION__, __LINE__)
#define WARN_ON(a) do{ if(a) \
        LWIP_DIAG(DIAG_ON_DEBUG, lwip_nf_nat_utils_64, "Warn at func %s line %d\n", __FUNCTION__, __LINE__); } while(0)
#define ERROR_ON() LWIP_DIAG(DIAG_ON_DEBUG, lwip_nf_nat_utils_65, "Error at func %s line %d\n", __FUNCTION__, __LINE__)
#define RETURN_ON() LWIP_DIAG(DIAG_ON_DEBUG, lwip_nf_nat_utils_66, "Return at func %s line %d\n", __FUNCTION__, __LINE__)

#define NF_DROP 0
#define NF_ACCEPT 1
#define NF_STOLEN 2
#define NF_QUEUE 3
#define NF_REPEAT 4
#define NF_STOP 5
#define NF_MAX_VERDICT NF_STOP

#ifndef AF_INET
#define AF_INET		2	/* Internet IP Protocol 	*/
#define AF_INET6	10	/* IP version 6			*/
#endif

#ifndef ENOENT
#define	ENOENT		 2	/* No such file or directory */
#define	EINVAL		22	/* Invalid argument */
#endif

/*
 * NOTE! This ctype does not handle EOF like the standard C
 * library is required to.
 */

#define _U	0x01	/* upper */
#define _L	0x02	/* lower */
#define _D	0x04	/* digit */
#define _C	0x08	/* cntrl */
#define _P	0x10	/* punct */
#define _S	0x20	/* white space (space/lf/tab) */
#define _X	0x40	/* hex digit */
#define _SP	0x80	/* hard space (0x20) */

extern const unsigned char _ctype[];

#define __ismask(x) (_ctype[(int)(unsigned char)(x)])

#define isalnum(c)	((__ismask(c)&(_U|_L|_D)) != 0)
#define isalpha(c)	((__ismask(c)&(_U|_L)) != 0)
#define iscntrl(c)	((__ismask(c)&(_C)) != 0)
#define isdigit(c)	((__ismask(c)&(_D)) != 0)
#define isgraph(c)	((__ismask(c)&(_P|_U|_L|_D)) != 0)
#define islower(c)	((__ismask(c)&(_L)) != 0)
#define isprint(c)	((__ismask(c)&(_P|_U|_L|_D|_SP)) != 0)
#define ispunct(c)	((__ismask(c)&(_P)) != 0)
/* Note: isspace() must return false for %NUL-terminator */
#define isspace(c)	((__ismask(c)&(_S)) != 0)
#define isupper(c)	((__ismask(c)&(_U)) != 0)
#define isxdigit(c)	((__ismask(c)&(_D|_X)) != 0)

#define isascii(c) (((unsigned char)(c))<=0x7f)
#define toascii(c) (((unsigned char)(c))&0x7f)

#ifndef __LITTLE_ENDIAN
#define __LITTLE_ENDIAN 1234
#endif

#ifndef __LITTLE_ENDIAN_BITFIELD
#define __LITTLE_ENDIAN_BITFIELD
#endif

struct iphdr {
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u8	ihl:4,
		version:4;
#elif defined (__BIG_ENDIAN_BITFIELD)
	__u8	version:4,
  		ihl:4;
#else
#error	"Please fix byte order"
#endif
	__u8	tos;
	__be16	tot_len;
	__be16	id;
	__be16	frag_off;
	__u8	ttl;
	__u8	protocol;
	__sum16	check;
	__be32	saddr;
	__be32	daddr;
	/*The options start here. */
} __attribute__ ((packed));

struct udphdr {
	__be16	source;
	__be16	dest;
	__be16	len;
	__sum16	check;
}  __attribute__ ((packed));

/* Internet address. */
#if 1
struct in_addr {
	__be32	s_addr;
};
#endif

/*
 *	IPv6 address structure
 */

struct in6_addr {
	union {
		__u8		u6_addr8[16];
		__be16		u6_addr16[8];
		__be32		u6_addr32[4];
	} in6_u;
#define s6_addr			in6_u.u6_addr8
#define s6_addr16		in6_u.u6_addr16
#define s6_addr32		in6_u.u6_addr32
};

union nf_inet_addr {
	__u32		all[4];
	__be32		ip;
	__be32		ip6[4];
	struct in_addr	in;
	struct in6_addr	in6;
};

/* Standard well-defined IP protocols.  */
#if 1
enum {
  IPPROTO_IP = 0,		/* Dummy protocol for TCP		*/
  IPPROTO_ICMP = 1,		/* Internet Control Message Protocol	*/
  IPPROTO_IGMP = 2,		/* Internet Group Management Protocol	*/
  IPPROTO_TCP = 6,		/* Transmission Control Protocol	*/
  IPPROTO_UDP = 17,		/* User Datagram Protocol		*/
  IPPROTO_GRE = 47,		/* Cisco GRE tunnels (rfc 1701,1702)	*/

  IPPROTO_IPV6	 = 41,		/* IPv6-in-IPv4 tunnelling		*/

  IPPROTO_ESP = 50,            /* Encapsulation Security Payload protocol */
  IPPROTO_AH = 51,             /* Authentication Header protocol       */
  IPPROTO_UDPLITE = 136,	/* UDP-Lite (RFC 3828)			*/
  IPPROTO_RAW	 = 255,		/* Raw IP packets			*/
  IPPROTO_MAX
};
#endif

enum {
    NF_DIR_OUT = 0,
    NF_DIR_IN = 1,
    NF_DIR_MAX
};

struct nf_ip_port_pair {
	__be32 ip;
	__be16 port;
};

struct nf_nat_tuple {
    struct nf_ip_port_pair src;
    struct nf_ip_port_pair dst;
    //__be16 rtp_port;
};


struct nf_conn {
    int ttl;
	u16 ip_type; // ipv4 or ipv6
	u16 ap_type; // tcp, udp, icmp

    struct nf_conn *rtp_ct;

    struct nf_nat_tuple tbl[NF_DIR_MAX];

	/*__be32 src_ip;
	__be16 src_port;
	__be16 rtp_src_port;

	__be32 nat_ip;
	__be16 nat_port; // nat_port must be odd
	__be16 rtp_nat_port; // nat_port +1
	//__be16 rtcp_nat_port; // rtp_nat_port +1

	__be32 dst_ip;
	__be16 dst_port;*/
};

enum {
	false	= 0,
	true	= 1
};

struct sk_buff {
	char			*data; 	// ip hdr
	char			*tail; 	// end of packet
	char			*end; 	// end of buffer
	int 			len;	// ip len
	int 			dir;	// input or output packet, 1 for input, 0 for output

	struct nf_conn 	*ct;	// type
};

extern int hex_to_bin(char ch);
#if 0
extern int nf_inet_addr_cmp(const __be32 ip1, const __be32 ip2);
#endif
extern int in4_pton(const char *src, int srclen,
	     u8 *dst,
	     int delim, const char **end);

extern int in6_pton(const char *src, int srclen,
	     u8 *dst,
	     int delim, const char **end);

#if 0
extern struct iphdr *ip_hdr(const struct sk_buff *skb);
extern unsigned int ip_hdrlen(const struct iphdr *ip_hdr);
extern int skb_tailroom(const struct sk_buff *skb);
extern char *skb_put(struct sk_buff *skb, unsigned int len);
extern void ip_send_check(struct iphdr *iph);
#endif

extern int nf_nat_mangle_udp_packet(struct sk_buff *skb,
			 struct nf_conn *ct,
			 unsigned int match_offset,
			 unsigned int match_len,
			 const char *rep_buffer,
			 unsigned int rep_len);

extern unsigned int ip_nat_sdp_port(struct sk_buff *skb, unsigned int dataoff,
				    const char **dptr, unsigned int *datalen,
				    unsigned int matchoff,
				    unsigned int matchlen,
				    u_int16_t port);

enum sdp_header_types;

extern unsigned int ip_nat_sdp_addr(struct sk_buff *skb, unsigned int dataoff,
				    const char **dptr, unsigned int *datalen,
				    unsigned int sdpoff,
				    enum sdp_header_types type,
				    enum sdp_header_types term,
				    const union nf_inet_addr *addr);

extern unsigned int ip_nat_sdp_session(struct sk_buff *skb, unsigned int dataoff,
				       const char **dptr, unsigned int *datalen,
				       unsigned int sdpoff,
				       const union nf_inet_addr *addr);

unsigned int ip_nat_sip(struct sk_buff *skb, unsigned int dataoff,
			       const char **dptr, unsigned int *datalen);

int sip_help_udp(struct sk_buff *skb, unsigned int protoff);


static inline struct iphdr *ip_hdr(const struct sk_buff *skb)
{
	return (struct iphdr *)skb->data;
}

static inline unsigned int ip_hdrlen(const struct iphdr *ip_hdr)
{
	return ip_hdr->ihl * 4;
}

static inline int skb_tailroom(const struct sk_buff *skb)
{
	return skb->end - skb->tail;
}

static inline char *skb_put(struct sk_buff *skb, unsigned int len)
{
	char *tmp = skb->tail;
	skb->tail += len;
	skb->len  += len;
	return tmp;
}

/* Generate a checksum for an outgoing IP datagram. */
static inline void ip_send_check(struct iphdr *iph)
{
	iph->check = 0;
	iph->check = ip_fast_csum((unsigned char *)iph, iph->ihl);
}

static inline int nf_inet_addr_cmp(const __be32 ip1, const __be32 ip2)
{
	return ip1 == ip2;
}


#endif /* __NF_NAT_UTILS_H__ */
