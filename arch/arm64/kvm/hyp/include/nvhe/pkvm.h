/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2021 Google LLC
 * Author: Fuad Tabba <tabba@google.com>
 */

#ifndef __ARM64_KVM_NVHE_PKVM_H__
#define __ARM64_KVM_NVHE_PKVM_H__

#include <asm/kvm_pkvm.h>

/*
 * A container for the vcpu state that hyp needs to maintain for protected VMs.
 */
struct shadow_vcpu_state {
	struct kvm_shadow_vm *vm;
	struct kvm_vcpu vcpu;
};

/*
 * Holds the relevant data for running a protected vm.
 */
struct kvm_shadow_vm {
	/* A unique id to the shadow structs in the hyp shadow area. */
	int shadow_handle;

	/* A pointer to the s2 mmu for the protected vm.. */
	struct kvm_s2_mmu *mmu;

	/* Number of vcpus for the vm. */
	int created_vcpus;

	/* Pointers to the shadow vcpus of the shadow vm. */
	struct kvm_vcpu *vcpus[KVM_MAX_VCPUS];

	/* The host's kvm structure. */
	struct kvm *host_kvm;

	/* The total size of the donated shadow area. */
	size_t shadow_area_size;

	/* Array of the shadow state per vcpu. */
	struct shadow_vcpu_state shadow_vcpus[0];
};

extern struct kvm_shadow_vm **shadow_table;

int __pkvm_init_shadow(struct kvm *kvm, void *shadow_va, size_t size);
int __pkvm_teardown_shadow(struct kvm *kvm);
struct kvm_vcpu *hyp_get_shadow_vcpu(const struct kvm_vcpu *host_vcpu);

u64 pvm_read_id_reg(const struct kvm_vcpu *vcpu, u32 id);
bool kvm_handle_pvm_sysreg(struct kvm_vcpu *vcpu, u64 *exit_code);
bool kvm_handle_pvm_restricted(struct kvm_vcpu *vcpu, u64 *exit_code);
void kvm_reset_pvm_sys_regs(struct kvm_vcpu *vcpu);
int kvm_check_pvm_sysreg_table(void);

void pkvm_reset_vcpu(struct kvm_vcpu *vcpu);

bool pkvm_handle_hvc64(struct kvm_vcpu *vcpu);

struct kvm_vcpu *pvm_mpidr_to_vcpu(struct kvm_shadow_vm *vm, unsigned long mpidr);

#endif /* __ARM64_KVM_NVHE_PKVM_H__ */