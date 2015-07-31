#ifndef BE_COMPAT_H
#define BE_COMPAT_H

#include <linux/netdevice.h>

#ifndef VLAN_N_VID
#define VLAN_N_VID              4096
#endif

#ifndef HAVE_VLAN_HWACCEL_RECEIVE_SKB
#define USE_NEW_VLAN_MODEL
#endif

#if defined(USE_NEW_VLAN_MODEL) || LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 0)
/* vlan_gro_frags() can be safely called when vlan_group is NULL
 *  * for kernels >= 3.0 or when kernels uses USE_NEW_VLAN_MODEL.
 */
#define NULL_VLAN_GRP_SAFE
#endif

static inline struct sk_buff *__vlan_put_tag_fixed(struct sk_buff *skb,
						   __be16 vlan_proto,
						   ushort vlan_tag)
{
#ifdef HAVE_VLAN_INSERT_TAG_SET_PROTO
	struct sk_buff *new_skb = vlan_insert_tag_set_proto(skb, vlan_proto,
							    vlan_tag);
#else
#ifdef HAVE_3_PARAMS_FOR_VLAN_PUT_TAG
	struct sk_buff *new_skb = __vlan_put_tag(skb, vlan_proto, vlan_tag);
#else
	struct sk_buff *new_skb = __vlan_put_tag(skb, vlan_tag);
#endif
#endif
	return new_skb;
}

#ifdef USE_NEW_VLAN_MODEL
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 1, 0)
struct vlan_group {
	char dummy;
};
#endif

static inline int vlan_hwaccel_receive_skb_compat(struct sk_buff *skb,
						  struct vlan_group *grp,
						  u16 vlan_tci)
{
#ifdef HAVE_3_PARAMS_FOR_VLAN_HWACCEL_PUT_TAG
	__vlan_hwaccel_put_tag(skb, htons(ETH_P_8021Q), vlan_tci);
#else
	__vlan_hwaccel_put_tag(skb, vlan_tci);
#endif
	return netif_receive_skb(skb);
}

static inline gro_result_t vlan_gro_frags_compat(struct napi_struct *napi,
						 struct vlan_group *grp,
						 unsigned int vlan_tci)
{
#ifdef HAVE_3_PARAMS_FOR_VLAN_HWACCEL_PUT_TAG
	__vlan_hwaccel_put_tag(napi->skb, htons(ETH_P_8021Q), vlan_tci);
#else
	__vlan_hwaccel_put_tag(napi->skb, vlan_tci);
#endif
	return napi_gro_frags(napi);
}

#define vlan_hwaccel_receive_skb                vlan_hwaccel_receive_skb_compat
#define vlan_gro_frags                          vlan_gro_frags_compat
#endif

#ifdef CONFIG_PCI_IOV
int be_find_vfs(struct pci_dev *pdev, int vf_state);
#ifndef HAVE_PCI_VF_ASSIGNED
int pci_vfs_assigned(struct pci_dev *pdev);
int pci_num_vf(struct pci_dev *pdev);
int pci_sriov_get_totalvfs(struct pci_dev *pdev);
#endif
#else
#define pci_vfs_assigned(x)                     0
#define pci_num_vf(x)                           0
#endif /* CONFIG_PCI_IOV */
void be_wait_for_vfs_detach(struct pci_dev *pdev);

#ifndef NETIF_F_HW_VLAN_CTAG_TX
#define NETIF_F_HW_VLAN_CTAG_TX         NETIF_F_HW_VLAN_TX
#endif
#ifndef NETIF_F_HW_VLAN_CTAG_RX
#define NETIF_F_HW_VLAN_CTAG_RX         NETIF_F_HW_VLAN_RX
#endif
#ifndef NETIF_F_HW_VLAN_CTAG_FILTER
#define NETIF_F_HW_VLAN_CTAG_FILTER     NETIF_F_HW_VLAN_FILTER
#endif

#ifndef HAVE_NETDEV_HW_FEATURES
#define hw_features                     features
#endif

#ifndef netdev_for_each_mc_addr
#define netdev_for_each_mc_addr(h, n)   for (h = (n)->mc_list; h; h = h->next)
#endif

/* When new mc-list macros were used in 2.6.35, dev_mc_list was dropped */
#ifndef HAVE_NETDEV_DEV_ADDR
#define DMI_ADDR                        addr
#else
#define DMI_ADDR                        dmi_addr
#endif /* dev_mc_list */

#ifndef DUPLEX_UNKNOWN
#define DUPLEX_UNKNOWN 0xFF
#endif

#ifndef HAVE_ETHER_ADDR_COPY
static inline void ether_addr_copy(u8 *dst, const u8 *src)
{
#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS)
	*(u32 *)dst = *(const u32 *)src;
	*(u16 *)(dst + 4) = *(const u16 *)(src + 4);
#else
	u16 *a = (u16 *)dst;
	const u16 *b = (const u16 *)src;

	a[0] = b[0];
	a[1] = b[1];
	a[2] = b[2];
#endif
}
#endif

#ifndef SUPPORTED_1000baseKX_Full
#define SUPPORTED_1000baseKX_Full       (1 << 17)
#endif

#ifndef SUPPORTED_10000baseKX4_Full
#define SUPPORTED_10000baseKX4_Full     (1 << 18)
#endif

#ifndef SUPPORTED_10000baseKR_Full
#define SUPPORTED_10000baseKR_Full      (1 << 19)
#endif

#ifndef SUPPORTED_20000baseKR2_Full
#define SUPPORTED_20000baseKR2_Full     (1 << 22)
#endif

#ifndef SUPPORTED_40000baseKR4_Full
#define SUPPORTED_40000baseKR4_Full     (1 << 23)
#endif

#ifndef SUPPORTED_40000baseCR4_Full
#define SUPPORTED_40000baseCR4_Full     (1 << 24)
#endif

#ifndef SUPPORTED_40000baseSR4_Full
#define SUPPORTED_40000baseSR4_Full     (1 << 25)
#endif

#ifndef SUPPORTED_40000baseLR4_Full
#define SUPPORTED_40000baseLR4_Full     (1 << 26)
#endif

#ifndef ADVERTISED_1000baseKX_Full
#define ADVERTISED_1000baseKX_Full      (1 << 17)
#endif

#ifndef ADVERTISED_10000baseKX4_Full
#define ADVERTISED_10000baseKX4_Full    (1 << 18)
#endif

#ifndef ADVERTISED_10000baseKR_Full
#define ADVERTISED_10000baseKR_Full     (1 << 19)
#endif

#ifndef ADVERTISED_20000baseKR2_Full
#define ADVERTISED_20000baseKR2_Full    (1 << 22)
#endif

#ifndef ADVERTISED_40000baseKR4_Full
#define ADVERTISED_40000baseKR4_Full    (1 << 23)
#endif

#endif                          /* BE_COMPAT_H */
