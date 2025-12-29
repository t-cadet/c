#ifndef C_LINUX_HEADER
#define C_LINUX_HEADER

// === linux.h: Cross-architecture Linux API ===================================
// 
// Contents:
//   * syscall number table            (jump: NR_fork_linux)
//   * TODO: syscall API types & constants
//   * syscallN generic wrappers       (jump: Syscall0_linux)
//   * TODO: syscall-specific wrappers (jump: fork_linux)
// 
// Usage:
//   linux.h is a libc-free & zero-dependency header-only library for C & C++
// 
//   #include "c/linux.h" // use as header file
//
//   #define C_LINUX_IMPLEMENTATION
//   #include "c/linux.h" // use as implementation file
//
// License:
//   MIT License (c) Tristan CADET
// 
// =============================================================================

// BY_ARCH_linux picks an argument depending on CPU architecture
#ifdef _MSC_VER
  #error "Unsupported compiler"
#elif defined(__x86_64__)
  #define BY_ARCH_linux(x86_64, b, c, d, e, f) x86_64
#elif defined(__aarch64__)
  #define BY_ARCH_linux(a, arm64, c, d, e, f) arm64
#elif defined(__riscv) && (__riscv_xlen == 64)
  #define BY_ARCH_linux(a, b, riscv64, d, e, f) riscv64
#elif defined(__i386__)
  #define BY_ARCH_linux(a, b, c, x86_32, e, f) x86_32
#elif defined(__arm__)
  #define BY_ARCH_linux(a, b, c, d, arm32, f) arm32
#elif defined(__riscv) && (__riscv_xlen == 32)
  #define BY_ARCH_linux(a, b, c, d, e, riscv32) riscv32
#else
  #error "Unsupported CPU architecture"
#endif

/*╔══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗*/
/*║                                                     LINUX SYSCALL TABLE                                                      ║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                                         Section List                                                         ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║  1. PROCESS & THREAD LIFECYCLE           11. SIGNALS                              21. NAMESPACES & CONTAINERS                ║*/
/*║  2. PROCESS ATTRIBUTES & CONTROL         12. PIPES & FIFOs                        22. PROCESS INSPECTION & CONTROL           ║*/
/*║  3. SCHEDULING & PRIORITIES              13. INTER-PROCESS COMMUNICATION          23. SYSTEM INFORMATION                     ║*/
/*║  4. MEMORY MANAGEMENT                    14. SOCKETS & NETWORKING                 24. KERNEL MODULES                         ║*/
/*║  5. FILE I/O OPERATIONS                  15. ASYNCHRONOUS I/O                     25. SYSTEM CONTROL & ADMINISTRATION        ║*/
/*║  6. FILE DESCRIPTOR MANAGEMENT           16. TIME & CLOCKS                        26. PERFORMANCE MONITORING & TRACING       ║*/
/*║  7. FILE METADATA                        17. RANDOM NUMBERS                       27. DEVICE & HARDWARE ACCESS               ║*/
/*║  8. DIRECTORY & NAMESPACE OPERATIONS     18. USER & GROUP IDENTITY                28. ARCHITECTURE-SPECIFIC OPERATIONS       ║*/
/*║  9. FILE SYSTEM OPERATIONS               19. CAPABILITIES & SECURITY              29. ADVANCED EXECUTION CONTROL             ║*/
/*║ 10. FILE SYSTEM MONITORING               20. RESOURCE LIMITS & ACCOUNTING         30. LEGACY, OBSOLETE & UNIMPLEMENTED       ║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                                1. PROCESS & THREAD LIFECYCLE                                                 ║*/
/*║                              Creation, execution, termination, and reaping of processes/threads                              ║*/
/*╠══════════════════════════════════════════════════════════════╦═════════╤═════════╤═════════╤═════════╤═════════╤═════════════╣*/
/*║                         Syscall Name                         ║ x86_64  │  arm64  │ riscv64 │ x86_32  │  arm32  │   riscv32   ║*/
/*╟──────────────────────────────────────────────────────────────╨─────────┴─────────┴─────────┴─────────┴─────────┴─────────────╢*/
/*║*/ #define NR_fork_linux                         BY_ARCH_linux(       57,     void,     void,        2,        2,     void) /*║*/
/*║*/ #define NR_vfork_linux                        BY_ARCH_linux(       58,     void,     void,      190,      190,     void) /*║*/
/*║*/ #define NR_clone_linux                        BY_ARCH_linux(       56,      220,      220,      120,      120,      220) /*║*/
/*║*/ #define NR_clone3_linux                       BY_ARCH_linux(      435,      435,      435,      435,      435,      435) /*║*/
/*║*/ #define NR_execve_linux                       BY_ARCH_linux(       59,      221,      221,       11,       11,      221) /*║*/
/*║*/ #define NR_execveat_linux                     BY_ARCH_linux(      322,      281,      281,      358,      387,      281) /*║*/
/*║*/ #define NR_exit_linux                         BY_ARCH_linux(       60,       93,       93,        1,        1,       93) /*║*/
/*║*/ #define NR_exit_group_linux                   BY_ARCH_linux(      231,       94,       94,      252,      248,       94) /*║*/
/*║*/ #define NR_wait4_linux                        BY_ARCH_linux(       61,      260,      260,      114,      114,     void) /*║*/
/*║*/ #define NR_waitid_linux                       BY_ARCH_linux(      247,       95,       95,      284,      280,       95) /*║*/
/*║*/ #define NR_waitpid_linux                      BY_ARCH_linux(     void,     void,     void,        7,     void,     void) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                               2. PROCESS ATTRIBUTES & CONTROL                                                ║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                      2a. Process identity, process groups and sessions                                       ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_getpid_linux                       BY_ARCH_linux(       39,      172,      172,       20,       20,      172) /*║*/
/*║*/ #define NR_getppid_linux                      BY_ARCH_linux(      110,      173,      173,       64,       64,      173) /*║*/
/*║*/ #define NR_gettid_linux                       BY_ARCH_linux(      186,      178,      178,      224,      224,      178) /*║*/
/*║*/ #define NR_getpgid_linux                      BY_ARCH_linux(      121,      155,      155,      132,      132,      155) /*║*/
/*║*/ #define NR_setpgid_linux                      BY_ARCH_linux(      109,      154,      154,       57,       57,      154) /*║*/
/*║*/ #define NR_getpgrp_linux                      BY_ARCH_linux(      111,     void,     void,       65,       65,     void) /*║*/
/*║*/ #define NR_getsid_linux                       BY_ARCH_linux(      124,      156,      156,      147,      147,      156) /*║*/
/*║*/ #define NR_setsid_linux                       BY_ARCH_linux(      112,      157,      157,       66,       66,      157) /*║*/
/*║*/ #define NR_set_tid_address_linux              BY_ARCH_linux(      218,       96,       96,      258,      256,       96) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                2b. Process control, personality, and miscellaneous attributes                                ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_prctl_linux                        BY_ARCH_linux(      157,      167,      167,      172,      172,      167) /*║*/
/*║*/ #define NR_personality_linux                  BY_ARCH_linux(      135,       92,       92,      136,      136,       92) /*║*/
/*║*/ #define NR_arch_prctl_linux                   BY_ARCH_linux(      158,     void,     void,      384,     void,     void) /*║*/
/*║*/ #define NR_modify_ldt_linux                   BY_ARCH_linux(      154,     void,     void,      123,     void,     void) /*║*/
/*║*/ #define NR_set_thread_area_linux              BY_ARCH_linux(      205,     void,     void,      243,     void,     void) /*║*/
/*║*/ #define NR_get_thread_area_linux              BY_ARCH_linux(      211,     void,     void,      244,     void,     void) /*║*/
/*║*/ #define NR_set_tls_linux                      BY_ARCH_linux(     void,     void,     void,     void, 0x0f0005,     void) /*║*/
/*║*/ #define NR_get_tls_linux                      BY_ARCH_linux(     void,     void,     void,     void, 0x0f0006,     void) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                                  3. SCHEDULING & PRIORITIES                                                  ║*/
/*╠══════════════════════════════════════════════════════════════╦═════════╤═════════╤═════════╤═════════╤═════════╤═════════════╣*/
/*║                         Syscall Name                         ║ x86_64  │  arm64  │ riscv64 │ x86_32  │  arm32  │   riscv32   ║*/
/*╟──────────────────────────────────────────────────────────────╨─────────┴─────────┴─────────┴─────────┴─────────┴─────────────╢*/
/*║*/ #define NR_sched_setscheduler_linux           BY_ARCH_linux(      144,      119,      119,      156,      156,      119) /*║*/
/*║*/ #define NR_sched_getscheduler_linux           BY_ARCH_linux(      145,      120,      120,      157,      157,      120) /*║*/
/*║*/ #define NR_sched_setparam_linux               BY_ARCH_linux(      142,      118,      118,      154,      154,      118) /*║*/
/*║*/ #define NR_sched_getparam_linux               BY_ARCH_linux(      143,      121,      121,      155,      155,      121) /*║*/
/*║*/ #define NR_sched_setattr_linux                BY_ARCH_linux(      314,      274,      274,      351,      380,      274) /*║*/
/*║*/ #define NR_sched_getattr_linux                BY_ARCH_linux(      315,      275,      275,      352,      381,      275) /*║*/
/*║*/ #define NR_sched_yield_linux                  BY_ARCH_linux(       24,      124,      124,      158,      158,      124) /*║*/
/*║*/ #define NR_sched_get_priority_max_linux       BY_ARCH_linux(      146,      125,      125,      159,      159,      125) /*║*/
/*║*/ #define NR_sched_get_priority_min_linux       BY_ARCH_linux(      147,      126,      126,      160,      160,      126) /*║*/
/*║*/ #define NR_sched_rr_get_interval_linux        BY_ARCH_linux(      148,      127,      127,      161,      161,     void) /*║*/
/*║*/ #define NR_sched_rr_get_interval_time64_linux BY_ARCH_linux(     void,      423,     void,      423,      423,      423) /*║*/
/*║*/ #define NR_sched_setaffinity_linux            BY_ARCH_linux(      203,      122,      122,      241,      241,      122) /*║*/
/*║*/ #define NR_sched_getaffinity_linux            BY_ARCH_linux(      204,      123,      123,      242,      242,      123) /*║*/
/*║*/ #define NR_nice_linux                         BY_ARCH_linux(     void,     void,     void,       34,       34,     void) /*║*/
/*║*/ #define NR_setpriority_linux                  BY_ARCH_linux(      141,      140,      140,       97,       97,      140) /*║*/
/*║*/ #define NR_getpriority_linux                  BY_ARCH_linux(      140,      141,      141,       96,       96,      141) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                                     4. MEMORY MANAGEMENT                                                     ║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                        4a. Memory mapping, allocation, and unmapping                                         ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_brk_linux                          BY_ARCH_linux(       12,      214,      214,       45,       45,      214) /*║*/
/*║*/ #define NR_mmap_linux                         BY_ARCH_linux(        9,      222,      222,       90,     void,     void) /*║*/
/*║*/ #define NR_mmap2_linux                        BY_ARCH_linux(     void,      222,     void,      192,      192,      222) /*║*/
/*║*/ #define NR_munmap_linux                       BY_ARCH_linux(       11,      215,      215,       91,       91,      215) /*║*/
/*║*/ #define NR_mremap_linux                       BY_ARCH_linux(       25,      216,      216,      163,      163,      216) /*║*/
/*║*/ #define NR_remap_file_pages_linux             BY_ARCH_linux(      216,      234,      234,      257,      253,      234) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                       4b. Memory protection, locking, and usage hints                                        ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_mprotect_linux                     BY_ARCH_linux(       10,      226,      226,      125,      125,      226) /*║*/
/*║*/ #define NR_pkey_mprotect_linux                BY_ARCH_linux(      329,      288,      288,      380,      394,      288) /*║*/
/*║*/ #define NR_madvise_linux                      BY_ARCH_linux(       28,      233,      233,      219,      220,      233) /*║*/
/*║*/ #define NR_process_madvise_linux              BY_ARCH_linux(      440,      440,      440,      440,      440,      440) /*║*/
/*║*/ #define NR_mlock_linux                        BY_ARCH_linux(      149,      228,      228,      150,      150,      228) /*║*/
/*║*/ #define NR_mlock2_linux                       BY_ARCH_linux(      325,      284,      284,      376,      390,      284) /*║*/
/*║*/ #define NR_munlock_linux                      BY_ARCH_linux(      150,      229,      229,      151,      151,      229) /*║*/
/*║*/ #define NR_mlockall_linux                     BY_ARCH_linux(      151,      230,      230,      152,      152,      230) /*║*/
/*║*/ #define NR_munlockall_linux                   BY_ARCH_linux(      152,      231,      231,      153,      153,      231) /*║*/
/*║*/ #define NR_mincore_linux                      BY_ARCH_linux(       27,      232,      232,      218,      219,      232) /*║*/
/*║*/ #define NR_msync_linux                        BY_ARCH_linux(       26,      227,      227,      144,      144,      227) /*║*/
/*║*/ #define NR_mseal_linux                        BY_ARCH_linux(      462,      462,      462,      462,      462,      462) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                          4c. NUMA memory policy and page migration                                           ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_mbind_linux                        BY_ARCH_linux(      237,      235,      235,      274,      319,      235) /*║*/
/*║*/ #define NR_set_mempolicy_linux                BY_ARCH_linux(      238,      237,      237,      276,      321,      237) /*║*/
/*║*/ #define NR_get_mempolicy_linux                BY_ARCH_linux(      239,      236,      236,      275,      320,      236) /*║*/
/*║*/ #define NR_set_mempolicy_home_node_linux      BY_ARCH_linux(      450,      450,      450,      450,      450,      450) /*║*/
/*║*/ #define NR_migrate_pages_linux                BY_ARCH_linux(      256,      238,      238,      294,      400,      238) /*║*/
/*║*/ #define NR_move_pages_linux                   BY_ARCH_linux(      279,      239,      239,      317,      344,      239) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                           4d. Anonymous file-backed memory regions                                           ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_memfd_create_linux                 BY_ARCH_linux(      319,      279,      279,      356,      385,      279) /*║*/
/*║*/ #define NR_memfd_secret_linux                 BY_ARCH_linux(      447,      447,      447,      447,     void,      447) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                             4e. Memory protection key management                                             ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_pkey_alloc_linux                   BY_ARCH_linux(      330,      289,      289,      381,      395,      289) /*║*/
/*║*/ #define NR_pkey_free_linux                    BY_ARCH_linux(      331,      290,      290,      382,      396,      290) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                       4f. Control-flow integrity, shadow stack mapping                                       ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_map_shadow_stack_linux             BY_ARCH_linux(      453,      453,      453,      453,      453,      453) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                                4g. Advanced memory operations                                                ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_userfaultfd_linux                  BY_ARCH_linux(      323,      282,      282,      374,      388,      282) /*║*/
/*║*/ #define NR_process_mrelease_linux             BY_ARCH_linux(      448,      448,      448,      448,      448,      448) /*║*/
/*║*/ #define NR_membarrier_linux                   BY_ARCH_linux(      324,      283,      283,      375,      389,      283) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                                    5. FILE I/O OPERATIONS                                                    ║*/
/*╠══════════════════════════════════════════════════════════════╦═════════╤═════════╤═════════╤═════════╤═════════╤═════════════╣*/
/*║                         Syscall Name                         ║ x86_64  │  arm64  │ riscv64 │ x86_32  │  arm32  │   riscv32   ║*/
/*╟──────────────────────────────────────────────────────────────╨─────────┴─────────┴─────────┴─────────┴─────────┴─────────────╢*/
/*║                                           5a. Opening, creating, and closing files                                           ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_open_linux                         BY_ARCH_linux(        2,     void,     void,        5,        5,     void) /*║*/
/*║*/ #define NR_openat_linux                       BY_ARCH_linux(      257,       56,       56,      295,      322,       56) /*║*/
/*║*/ #define NR_openat2_linux                      BY_ARCH_linux(      437,      437,      437,      437,      437,      437) /*║*/
/*║*/ #define NR_creat_linux                        BY_ARCH_linux(       85,     void,     void,        8,        8,     void) /*║*/
/*║*/ #define NR_close_linux                        BY_ARCH_linux(        3,       57,       57,        6,        6,       57) /*║*/
/*║*/ #define NR_close_range_linux                  BY_ARCH_linux(      436,      436,      436,      436,      436,      436) /*║*/
/*║*/ #define NR_open_by_handle_at_linux            BY_ARCH_linux(      304,      265,      265,      342,      371,      265) /*║*/
/*║*/ #define NR_name_to_handle_at_linux            BY_ARCH_linux(      303,      264,      264,      341,      370,      264) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                              5b. Reading and writing file data                                               ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_read_linux                         BY_ARCH_linux(        0,       63,       63,        3,        3,       63) /*║*/
/*║*/ #define NR_write_linux                        BY_ARCH_linux(        1,       64,       64,        4,        4,       64) /*║*/
/*║*/ #define NR_readv_linux                        BY_ARCH_linux(       19,       65,       65,      145,      145,       65) /*║*/
/*║*/ #define NR_writev_linux                       BY_ARCH_linux(       20,       66,       66,      146,      146,       66) /*║*/
/*║*/ #define NR_pread64_linux                      BY_ARCH_linux(       17,       67,       67,      180,      180,       67) /*║*/
/*║*/ #define NR_pwrite64_linux                     BY_ARCH_linux(       18,       68,       68,      181,      181,       68) /*║*/
/*║*/ #define NR_preadv_linux                       BY_ARCH_linux(      295,       69,       69,      333,      361,       69) /*║*/
/*║*/ #define NR_pwritev_linux                      BY_ARCH_linux(      296,       70,       70,      334,      362,       70) /*║*/
/*║*/ #define NR_preadv2_linux                      BY_ARCH_linux(      327,      286,      286,      378,      392,      286) /*║*/
/*║*/ #define NR_pwritev2_linux                     BY_ARCH_linux(      328,      287,      287,      379,      393,      287) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                               5c. Seeking and truncating files                                               ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_lseek_linux                        BY_ARCH_linux(        8,       62,       62,       19,       19,     void) /*║*/
/*║*/ #define NR_llseek_linux                       BY_ARCH_linux(     void,       62,     void,     void,     void,       62) /*║*/
/*║*/ #define NR__llseek_linux                      BY_ARCH_linux(     void,     void,     void,      140,      140,     void) /*║*/
/*║*/ #define NR_truncate_linux                     BY_ARCH_linux(       76,       45,       45,       92,       92,     void) /*║*/
/*║*/ #define NR_truncate64_linux                   BY_ARCH_linux(     void,       45,     void,      193,      193,       45) /*║*/
/*║*/ #define NR_ftruncate_linux                    BY_ARCH_linux(       77,       46,       46,       93,       93,     void) /*║*/
/*║*/ #define NR_ftruncate64_linux                  BY_ARCH_linux(     void,       46,     void,      194,      194,       46) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                              5d. Zero-copy and specialized I/O                                               ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_sendfile_linux                     BY_ARCH_linux(       40,       71,       71,      187,      187,     void) /*║*/
/*║*/ #define NR_sendfile64_linux                   BY_ARCH_linux(     void,       71,     void,      239,      239,       71) /*║*/
/*║*/ #define NR_splice_linux                       BY_ARCH_linux(      275,       76,       76,      313,      340,       76) /*║*/
/*║*/ #define NR_tee_linux                          BY_ARCH_linux(      276,       77,       77,      315,      342,       77) /*║*/
/*║*/ #define NR_vmsplice_linux                     BY_ARCH_linux(      278,       75,       75,      316,      343,       75) /*║*/
/*║*/ #define NR_copy_file_range_linux              BY_ARCH_linux(      326,      285,      285,      377,      391,      285) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                              5e. I/O hints and space allocation                                              ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_fadvise64_linux                    BY_ARCH_linux(      221,      223,      223,      250,     void,     void) /*║*/
/*║*/ #define NR_fadvise64_64_linux                 BY_ARCH_linux(     void,      223,     void,      272,     void,      223) /*║*/
/*║*/ #define NR_arm_fadvise64_64_linux             BY_ARCH_linux(     void,     void,     void,     void,      270,     void) /*║*/
/*║*/ #define NR_readahead_linux                    BY_ARCH_linux(      187,      213,      213,      225,      225,      213) /*║*/
/*║*/ #define NR_fallocate_linux                    BY_ARCH_linux(      285,       47,       47,      324,      352,       47) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                              5f. Flushing file data to storage                                               ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_sync_linux                         BY_ARCH_linux(      162,       81,       81,       36,       36,       81) /*║*/
/*║*/ #define NR_syncfs_linux                       BY_ARCH_linux(      306,      267,      267,      344,      373,      267) /*║*/
/*║*/ #define NR_fsync_linux                        BY_ARCH_linux(       74,       82,       82,      118,      118,       82) /*║*/
/*║*/ #define NR_fdatasync_linux                    BY_ARCH_linux(       75,       83,       83,      148,      148,       83) /*║*/
/*║*/ #define NR_sync_file_range_linux              BY_ARCH_linux(      277,       84,       84,      314,     void,       84) /*║*/
/*║*/ #define NR_arm_sync_file_range_linux          BY_ARCH_linux(     void,     void,     void,     void,      341,     void) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                                6. FILE DESCRIPTOR MANAGEMENT                                                 ║*/
/*╠══════════════════════════════════════════════════════════════╦═════════╤═════════╤═════════╤═════════╤═════════╤═════════════╣*/
/*║                         Syscall Name                         ║ x86_64  │  arm64  │ riscv64 │ x86_32  │  arm32  │   riscv32   ║*/
/*╟──────────────────────────────────────────────────────────────╨─────────┴─────────┴─────────┴─────────┴─────────┴─────────────╢*/
/*║                                       6a. Duplicating and controlling file descriptors                                       ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_dup_linux                          BY_ARCH_linux(       32,       23,       23,       41,       41,       23) /*║*/
/*║*/ #define NR_dup2_linux                         BY_ARCH_linux(       33,     void,     void,       63,       63,     void) /*║*/
/*║*/ #define NR_dup3_linux                         BY_ARCH_linux(      292,       24,       24,      330,      358,       24) /*║*/
/*║*/ #define NR_fcntl_linux                        BY_ARCH_linux(       72,       25,       25,       55,       55,     void) /*║*/
/*║*/ #define NR_fcntl64_linux                      BY_ARCH_linux(     void,       25,     void,      221,      221,       25) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                            6b. Device-specific control operations                                            ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_ioctl_linux                        BY_ARCH_linux(       16,       29,       29,       54,       54,       29) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                                     6c. I/O Multiplexing                                                     ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_select_linux                       BY_ARCH_linux(       23,     void,     void,       82,     void,     void) /*║*/
/*║*/ #define NR__newselect_linux                   BY_ARCH_linux(     void,     void,     void,      142,      142,     void) /*║*/
/*║*/ #define NR_pselect6_linux                     BY_ARCH_linux(      270,       72,       72,      308,      335,     void) /*║*/
/*║*/ #define NR_pselect6_time64_linux              BY_ARCH_linux(     void,      413,     void,      413,      413,      413) /*║*/
/*║*/ #define NR_poll_linux                         BY_ARCH_linux(        7,     void,     void,      168,      168,     void) /*║*/
/*║*/ #define NR_ppoll_linux                        BY_ARCH_linux(      271,       73,       73,      309,      336,     void) /*║*/
/*║*/ #define NR_ppoll_time64_linux                 BY_ARCH_linux(     void,      414,     void,      414,      414,      414) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                             6d. Scalable I/O event notification                                              ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_epoll_create_linux                 BY_ARCH_linux(      213,     void,     void,      254,      250,     void) /*║*/
/*║*/ #define NR_epoll_create1_linux                BY_ARCH_linux(      291,       20,       20,      329,      357,       20) /*║*/
/*║*/ #define NR_epoll_ctl_linux                    BY_ARCH_linux(      233,       21,       21,      255,      251,       21) /*║*/
/*║*/ #define NR_epoll_wait_linux                   BY_ARCH_linux(      232,     void,     void,      256,      252,     void) /*║*/
/*║*/ #define NR_epoll_pwait_linux                  BY_ARCH_linux(      281,       22,       22,      319,      346,       22) /*║*/
/*║*/ #define NR_epoll_pwait2_linux                 BY_ARCH_linux(      441,      441,      441,      441,      441,      441) /*║*/
/*║*/ #define NR_epoll_ctl_old_linux                BY_ARCH_linux(      214,     void,     void,     void,     void,     void) /*║*/
/*║*/ #define NR_epoll_wait_old_linux               BY_ARCH_linux(      215,     void,     void,     void,     void,     void) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                                       7. FILE METADATA                                                       ║*/
/*╠══════════════════════════════════════════════════════════════╦═════════╤═════════╤═════════╤═════════╤═════════╤═════════════╣*/
/*║                         Syscall Name                         ║ x86_64  │  arm64  │ riscv64 │ x86_32  │  arm32  │   riscv32   ║*/
/*╟──────────────────────────────────────────────────────────────╨─────────┴─────────┴─────────┴─────────┴─────────┴─────────────╢*/
/*║                                            7a. Getting file attributes and status                                            ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_stat_linux                         BY_ARCH_linux(        4,     void,     void,      106,      106,     void) /*║*/
/*║*/ #define NR_fstat_linux                        BY_ARCH_linux(        5,       80,       80,      108,      108,     void) /*║*/
/*║*/ #define NR_lstat_linux                        BY_ARCH_linux(        6,     void,     void,      107,      107,     void) /*║*/
/*║*/ #define NR_stat64_linux                       BY_ARCH_linux(     void,     void,     void,      195,      195,     void) /*║*/
/*║*/ #define NR_fstat64_linux                      BY_ARCH_linux(     void,       80,     void,      197,      197,     void) /*║*/
/*║*/ #define NR_lstat64_linux                      BY_ARCH_linux(     void,     void,     void,      196,      196,     void) /*║*/
/*║*/ #define NR_newfstatat_linux                   BY_ARCH_linux(      262,       79,       79,     void,     void,     void) /*║*/
/*║*/ #define NR_fstatat64_linux                    BY_ARCH_linux(     void,       79,     void,      300,      327,     void) /*║*/
/*║*/ #define NR_statx_linux                        BY_ARCH_linux(      332,      291,      291,      383,      397,      291) /*║*/
/*║*/ #define NR_oldstat_linux                      BY_ARCH_linux(     void,     void,     void,       18,     void,     void) /*║*/
/*║*/ #define NR_oldfstat_linux                     BY_ARCH_linux(     void,     void,     void,       28,     void,     void) /*║*/
/*║*/ #define NR_oldlstat_linux                     BY_ARCH_linux(     void,     void,     void,       84,     void,     void) /*║*/
/*║*/ #define NR_file_getattr_linux                 BY_ARCH_linux(      468,      468,      468,      468,      468,      468) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                         7b. Changing file permissions and ownership                                          ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_chmod_linux                        BY_ARCH_linux(       90,     void,     void,       15,       15,     void) /*║*/
/*║*/ #define NR_fchmod_linux                       BY_ARCH_linux(       91,       52,       52,       94,       94,       52) /*║*/
/*║*/ #define NR_fchmodat_linux                     BY_ARCH_linux(      268,       53,       53,      306,      333,       53) /*║*/
/*║*/ #define NR_fchmodat2_linux                    BY_ARCH_linux(      452,      452,      452,      452,      452,      452) /*║*/
/*║*/ #define NR_umask_linux                        BY_ARCH_linux(       95,      166,      166,       60,       60,      166) /*║*/
/*║*/ #define NR_chown_linux                        BY_ARCH_linux(       92,     void,     void,      182,      182,     void) /*║*/
/*║*/ #define NR_fchown_linux                       BY_ARCH_linux(       93,       55,       55,       95,       95,       55) /*║*/
/*║*/ #define NR_lchown_linux                       BY_ARCH_linux(       94,     void,     void,       16,       16,     void) /*║*/
/*║*/ #define NR_chown32_linux                      BY_ARCH_linux(     void,     void,     void,      212,      212,     void) /*║*/
/*║*/ #define NR_fchown32_linux                     BY_ARCH_linux(     void,     void,     void,      207,      207,     void) /*║*/
/*║*/ #define NR_lchown32_linux                     BY_ARCH_linux(     void,     void,     void,      198,      198,     void) /*║*/
/*║*/ #define NR_fchownat_linux                     BY_ARCH_linux(      260,       54,       54,      298,      325,       54) /*║*/
/*║*/ #define NR_file_setattr_linux                 BY_ARCH_linux(      469,      469,      469,      469,      469,      469) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                            7c. File access and modification times                                            ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_utime_linux                        BY_ARCH_linux(      132,     void,     void,       30,     void,     void) /*║*/
/*║*/ #define NR_utimes_linux                       BY_ARCH_linux(      235,     void,     void,      271,      269,     void) /*║*/
/*║*/ #define NR_futimesat_linux                    BY_ARCH_linux(      261,     void,     void,      299,      326,     void) /*║*/
/*║*/ #define NR_utimensat_linux                    BY_ARCH_linux(      280,       88,       88,      320,      348,     void) /*║*/
/*║*/ #define NR_utimensat_time64_linux             BY_ARCH_linux(     void,      412,     void,      412,      412,      412) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                                7d. Testing file accessibility                                                ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_access_linux                       BY_ARCH_linux(       21,     void,     void,       33,       33,     void) /*║*/
/*║*/ #define NR_faccessat_linux                    BY_ARCH_linux(      269,       48,       48,      307,      334,       48) /*║*/
/*║*/ #define NR_faccessat2_linux                   BY_ARCH_linux(      439,      439,      439,      439,      439,      439) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                    7e. Getting, setting, and listing extended attributes                                     ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_setxattr_linux                     BY_ARCH_linux(      188,        5,        5,      226,      226,        5) /*║*/
/*║*/ #define NR_lsetxattr_linux                    BY_ARCH_linux(      189,        6,        6,      227,      227,        6) /*║*/
/*║*/ #define NR_fsetxattr_linux                    BY_ARCH_linux(      190,        7,        7,      228,      228,        7) /*║*/
/*║*/ #define NR_setxattrat_linux                   BY_ARCH_linux(      463,      463,      463,      463,      463,      463) /*║*/
/*║*/ #define NR_getxattr_linux                     BY_ARCH_linux(      191,        8,        8,      229,      229,        8) /*║*/
/*║*/ #define NR_lgetxattr_linux                    BY_ARCH_linux(      192,        9,        9,      230,      230,        9) /*║*/
/*║*/ #define NR_fgetxattr_linux                    BY_ARCH_linux(      193,       10,       10,      231,      231,       10) /*║*/
/*║*/ #define NR_getxattrat_linux                   BY_ARCH_linux(      464,      464,      464,      464,      464,      464) /*║*/
/*║*/ #define NR_listxattr_linux                    BY_ARCH_linux(      194,       11,       11,      232,      232,       11) /*║*/
/*║*/ #define NR_llistxattr_linux                   BY_ARCH_linux(      195,       12,       12,      233,      233,       12) /*║*/
/*║*/ #define NR_flistxattr_linux                   BY_ARCH_linux(      196,       13,       13,      234,      234,       13) /*║*/
/*║*/ #define NR_listxattrat_linux                  BY_ARCH_linux(      465,      465,      465,      465,      465,      465) /*║*/
/*║*/ #define NR_removexattr_linux                  BY_ARCH_linux(      197,       14,       14,      235,      235,       14) /*║*/
/*║*/ #define NR_lremovexattr_linux                 BY_ARCH_linux(      198,       15,       15,      236,      236,       15) /*║*/
/*║*/ #define NR_fremovexattr_linux                 BY_ARCH_linux(      199,       16,       16,      237,      237,       16) /*║*/
/*║*/ #define NR_removexattrat_linux                BY_ARCH_linux(      466,      466,      466,      466,      466,      466) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                                  7f. Advisory file locking                                                   ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_flock_linux                        BY_ARCH_linux(       73,       32,       32,      143,      143,       32) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                             8. DIRECTORY & NAMESPACE OPERATIONS                                              ║*/
/*╠══════════════════════════════════════════════════════════════╦═════════╤═════════╤═════════╤═════════╤═════════╤═════════════╣*/
/*║                         Syscall Name                         ║ x86_64  │  arm64  │ riscv64 │ x86_32  │  arm32  │   riscv32   ║*/
/*╟──────────────────────────────────────────────────────────────╨─────────┴─────────┴─────────┴─────────┴─────────┴─────────────╢*/
/*║                                       8a. Creating, removing, and reading directories                                        ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_mkdir_linux                        BY_ARCH_linux(       83,     void,     void,       39,       39,     void) /*║*/
/*║*/ #define NR_mkdirat_linux                      BY_ARCH_linux(      258,       34,       34,      296,      323,       34) /*║*/
/*║*/ #define NR_rmdir_linux                        BY_ARCH_linux(       84,     void,     void,       40,       40,     void) /*║*/
/*║*/ #define NR_getdents_linux                     BY_ARCH_linux(       78,     void,     void,      141,      141,     void) /*║*/
/*║*/ #define NR_getdents64_linux                   BY_ARCH_linux(      217,       61,       61,      220,      217,       61) /*║*/
/*║*/ #define NR_readdir_linux                      BY_ARCH_linux(     void,     void,     void,       89,     void,     void) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                          8b. Getting and changing current directory                                          ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_getcwd_linux                       BY_ARCH_linux(       79,       17,       17,      183,      183,       17) /*║*/
/*║*/ #define NR_chdir_linux                        BY_ARCH_linux(       80,       49,       49,       12,       12,       49) /*║*/
/*║*/ #define NR_fchdir_linux                       BY_ARCH_linux(       81,       50,       50,      133,      133,       50) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                      8c. Creating and managing hard and symbolic links                                       ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_link_linux                         BY_ARCH_linux(       86,     void,     void,        9,        9,     void) /*║*/
/*║*/ #define NR_linkat_linux                       BY_ARCH_linux(      265,       37,       37,      303,      330,       37) /*║*/
/*║*/ #define NR_unlink_linux                       BY_ARCH_linux(       87,     void,     void,       10,       10,     void) /*║*/
/*║*/ #define NR_unlinkat_linux                     BY_ARCH_linux(      263,       35,       35,      301,      328,       35) /*║*/
/*║*/ #define NR_symlink_linux                      BY_ARCH_linux(       88,     void,     void,       83,       83,     void) /*║*/
/*║*/ #define NR_symlinkat_linux                    BY_ARCH_linux(      266,       36,       36,      304,      331,       36) /*║*/
/*║*/ #define NR_readlink_linux                     BY_ARCH_linux(       89,     void,     void,       85,       85,     void) /*║*/
/*║*/ #define NR_readlinkat_linux                   BY_ARCH_linux(      267,       78,       78,      305,      332,       78) /*║*/
/*║*/ #define NR_rename_linux                       BY_ARCH_linux(       82,     void,     void,       38,       38,     void) /*║*/
/*║*/ #define NR_renameat_linux                     BY_ARCH_linux(      264,       38,     void,      302,      329,     void) /*║*/
/*║*/ #define NR_renameat2_linux                    BY_ARCH_linux(      316,      276,      276,      353,      382,      276) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                           8d. Creating device and named pipe nodes                                           ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_mknod_linux                        BY_ARCH_linux(      133,     void,     void,       14,       14,     void) /*║*/
/*║*/ #define NR_mknodat_linux                      BY_ARCH_linux(      259,       33,       33,      297,      324,       33) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                                  9. FILE SYSTEM OPERATIONS                                                   ║*/
/*╠══════════════════════════════════════════════════════════════╦═════════╤═════════╤═════════╤═════════╤═════════╤═════════════╣*/
/*║                         Syscall Name                         ║ x86_64  │  arm64  │ riscv64 │ x86_32  │  arm32  │   riscv32   ║*/
/*╟──────────────────────────────────────────────────────────────╨─────────┴─────────┴─────────┴─────────┴─────────┴─────────────╢*/
/*║                                          9a. Mounting filesystems and changing root                                          ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_mount_linux                        BY_ARCH_linux(      165,       40,       40,       21,       21,       40) /*║*/
/*║*/ #define NR_umount_linux                       BY_ARCH_linux(     void,     void,     void,       22,     void,     void) /*║*/
/*║*/ #define NR_umount2_linux                      BY_ARCH_linux(      166,       39,       39,       52,       52,       39) /*║*/
/*║*/ #define NR_pivot_root_linux                   BY_ARCH_linux(      155,       41,       41,      217,      218,       41) /*║*/
/*║*/ #define NR_chroot_linux                       BY_ARCH_linux(      161,       51,       51,       61,       61,       51) /*║*/
/*║*/ #define NR_mount_setattr_linux                BY_ARCH_linux(      442,      442,      442,      442,      442,      442) /*║*/
/*║*/ #define NR_move_mount_linux                   BY_ARCH_linux(      429,      429,      429,      429,      429,      429) /*║*/
/*║*/ #define NR_open_tree_linux                    BY_ARCH_linux(      428,      428,      428,      428,      428,      428) /*║*/
/*║*/ #define NR_open_tree_attr_linux               BY_ARCH_linux(      467,      467,      467,      467,      467,      467) /*║*/
/*║*/ #define NR_fsconfig_linux                     BY_ARCH_linux(      431,      431,      431,      431,      431,      431) /*║*/
/*║*/ #define NR_fsmount_linux                      BY_ARCH_linux(      432,      432,      432,      432,      432,      432) /*║*/
/*║*/ #define NR_fsopen_linux                       BY_ARCH_linux(      430,      430,      430,      430,      430,      430) /*║*/
/*║*/ #define NR_fspick_linux                       BY_ARCH_linux(      433,      433,      433,      433,      433,      433) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                              9b. Getting filesystem statistics                                               ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_statfs_linux                       BY_ARCH_linux(      137,       43,       43,       99,       99,     void) /*║*/
/*║*/ #define NR_fstatfs_linux                      BY_ARCH_linux(      138,       44,       44,      100,      100,     void) /*║*/
/*║*/ #define NR_statfs64_linux                     BY_ARCH_linux(     void,       43,     void,      268,      266,       43) /*║*/
/*║*/ #define NR_fstatfs64_linux                    BY_ARCH_linux(     void,       44,     void,      269,      267,       44) /*║*/
/*║*/ #define NR_ustat_linux                        BY_ARCH_linux(      136,     void,     void,       62,       62,     void) /*║*/
/*║*/ #define NR_statmount_linux                    BY_ARCH_linux(      457,      457,      457,      457,      457,      457) /*║*/
/*║*/ #define NR_listmount_linux                    BY_ARCH_linux(      458,      458,      458,      458,      458,      458) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                                    9c. Disk quota control                                                    ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_quotactl_linux                     BY_ARCH_linux(      179,       60,       60,      131,      131,       60) /*║*/
/*║*/ #define NR_quotactl_fd_linux                  BY_ARCH_linux(      443,      443,      443,      443,      443,      443) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                                  10. FILE SYSTEM MONITORING                                                  ║*/
/*╠══════════════════════════════════════════════════════════════╦═════════╤═════════╤═════════╤═════════╤═════════╤═════════════╣*/
/*║                         Syscall Name                         ║ x86_64  │  arm64  │ riscv64 │ x86_32  │  arm32  │   riscv32   ║*/
/*╟──────────────────────────────────────────────────────────────╨─────────┴─────────┴─────────┴─────────┴─────────┴─────────────╢*/
/*║                                              10a. Monitoring filesystem events                                               ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_inotify_init_linux                 BY_ARCH_linux(      253,     void,     void,      291,      316,     void) /*║*/
/*║*/ #define NR_inotify_init1_linux                BY_ARCH_linux(      294,       26,       26,      332,      360,       26) /*║*/
/*║*/ #define NR_inotify_add_watch_linux            BY_ARCH_linux(      254,       27,       27,      292,      317,       27) /*║*/
/*║*/ #define NR_inotify_rm_watch_linux             BY_ARCH_linux(      255,       28,       28,      293,      318,       28) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                           10b. Filesystem-wide event notification                                            ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_fanotify_init_linux                BY_ARCH_linux(      300,      262,      262,      338,      367,      262) /*║*/
/*║*/ #define NR_fanotify_mark_linux                BY_ARCH_linux(      301,      263,      263,      339,      368,      263) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                                         11. SIGNALS                                                          ║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                               11a. Setting up signal handlers                                                ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_signal_linux                       BY_ARCH_linux(     void,     void,     void,       48,     void,     void) /*║*/
/*║*/ #define NR_sigaction_linux                    BY_ARCH_linux(     void,     void,     void,       67,       67,     void) /*║*/
/*║*/ #define NR_rt_sigaction_linux                 BY_ARCH_linux(       13,      134,      134,      174,      174,      134) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                              11b. Sending signals to processes                                               ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_kill_linux                         BY_ARCH_linux(       62,      129,      129,       37,       37,      129) /*║*/
/*║*/ #define NR_tkill_linux                        BY_ARCH_linux(      200,      130,      130,      238,      238,      130) /*║*/
/*║*/ #define NR_tgkill_linux                       BY_ARCH_linux(      234,      131,      131,      270,      268,      131) /*║*/
/*║*/ #define NR_rt_sigqueueinfo_linux              BY_ARCH_linux(      129,      138,      138,      178,      178,      138) /*║*/
/*║*/ #define NR_rt_tgsigqueueinfo_linux            BY_ARCH_linux(      297,      240,      240,      335,      363,      240) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                             11c. Blocking and unblocking signals                                             ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_sigprocmask_linux                  BY_ARCH_linux(     void,     void,     void,      126,      126,     void) /*║*/
/*║*/ #define NR_rt_sigprocmask_linux               BY_ARCH_linux(       14,      135,      135,      175,      175,      135) /*║*/
/*║*/ #define NR_sgetmask_linux                     BY_ARCH_linux(     void,     void,     void,       68,     void,     void) /*║*/
/*║*/ #define NR_ssetmask_linux                     BY_ARCH_linux(     void,     void,     void,       69,     void,     void) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                            11d. Waiting for and querying signals                                             ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_sigpending_linux                   BY_ARCH_linux(     void,     void,     void,       73,       73,     void) /*║*/
/*║*/ #define NR_rt_sigpending_linux                BY_ARCH_linux(      127,      136,      136,      176,      176,      136) /*║*/
/*║*/ #define NR_sigsuspend_linux                   BY_ARCH_linux(     void,     void,     void,       72,       72,     void) /*║*/
/*║*/ #define NR_rt_sigsuspend_linux                BY_ARCH_linux(      130,      133,      133,      179,      179,      133) /*║*/
/*║*/ #define NR_pause_linux                        BY_ARCH_linux(       34,     void,     void,       29,       29,     void) /*║*/
/*║*/ #define NR_rt_sigtimedwait_linux              BY_ARCH_linux(      128,      137,      137,      177,      177,     void) /*║*/
/*║*/ #define NR_rt_sigtimedwait_time64_linux       BY_ARCH_linux(     void,      421,     void,      421,      421,      421) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                     11e. Alternate signal stack and return from handlers                                     ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_sigaltstack_linux                  BY_ARCH_linux(      131,      132,      132,      186,      186,      132) /*║*/
/*║*/ #define NR_sigreturn_linux                    BY_ARCH_linux(     void,     void,     void,      119,      119,     void) /*║*/
/*║*/ #define NR_rt_sigreturn_linux                 BY_ARCH_linux(       15,      139,      139,      173,      173,      139) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                          11f. Signal delivery via file descriptors                                           ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_signalfd_linux                     BY_ARCH_linux(      282,     void,     void,      321,      349,     void) /*║*/
/*║*/ #define NR_signalfd4_linux                    BY_ARCH_linux(      289,       74,       74,      327,      355,       74) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                                      12. PIPES & FIFOs                                                       ║*/
/*╠══════════════════════════════════════════════════════════════╦═════════╤═════════╤═════════╤═════════╤═════════╤═════════════╣*/
/*║                         Syscall Name                         ║ x86_64  │  arm64  │ riscv64 │ x86_32  │  arm32  │   riscv32   ║*/
/*╟──────────────────────────────────────────────────────────────╨─────────┴─────────┴─────────┴─────────┴─────────┴─────────────╢*/
/*║*/ #define NR_pipe_linux                         BY_ARCH_linux(       22,     void,     void,       42,       42,     void) /*║*/
/*║*/ #define NR_pipe2_linux                        BY_ARCH_linux(      293,       59,       59,      331,      359,       59) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                               13. INTER-PROCESS COMMUNICATION                                                ║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                              13a. System V IPC - Shared Memory                                               ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_shmget_linux                       BY_ARCH_linux(       29,      194,      194,      395,      307,      194) /*║*/
/*║*/ #define NR_shmat_linux                        BY_ARCH_linux(       30,      196,      196,      397,      305,      196) /*║*/
/*║*/ #define NR_shmdt_linux                        BY_ARCH_linux(       67,      197,      197,      398,      306,      197) /*║*/
/*║*/ #define NR_shmctl_linux                       BY_ARCH_linux(       31,      195,      195,      396,      308,      195) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                              13b. System V IPC - Message Queues                                              ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_msgget_linux                       BY_ARCH_linux(       68,      186,      186,      399,      303,      186) /*║*/
/*║*/ #define NR_msgsnd_linux                       BY_ARCH_linux(       69,      189,      189,      400,      301,      189) /*║*/
/*║*/ #define NR_msgrcv_linux                       BY_ARCH_linux(       70,      188,      188,      401,      302,      188) /*║*/
/*║*/ #define NR_msgctl_linux                       BY_ARCH_linux(       71,      187,      187,      402,      304,      187) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                                13c. System V IPC - Semaphores                                                ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_semget_linux                       BY_ARCH_linux(       64,      190,      190,      393,      299,      190) /*║*/
/*║*/ #define NR_semop_linux                        BY_ARCH_linux(       65,      193,      193,     void,      298,      193) /*║*/
/*║*/ #define NR_semctl_linux                       BY_ARCH_linux(       66,      191,      191,      394,      300,      191) /*║*/
/*║*/ #define NR_semtimedop_linux                   BY_ARCH_linux(      220,      192,      192,     void,      312,     void) /*║*/
/*║*/ #define NR_semtimedop_time64_linux            BY_ARCH_linux(     void,      420,     void,      420,      420,      420) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                                  13d. POSIX Message Queues                                                   ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_mq_open_linux                      BY_ARCH_linux(      240,      180,      180,      277,      274,      180) /*║*/
/*║*/ #define NR_mq_unlink_linux                    BY_ARCH_linux(      241,      181,      181,      278,      275,      181) /*║*/
/*║*/ #define NR_mq_timedsend_linux                 BY_ARCH_linux(      242,      182,      182,      279,      276,     void) /*║*/
/*║*/ #define NR_mq_timedsend_time64_linux          BY_ARCH_linux(     void,      418,     void,      418,      418,      418) /*║*/
/*║*/ #define NR_mq_timedreceive_linux              BY_ARCH_linux(      243,      183,      183,      280,      277,     void) /*║*/
/*║*/ #define NR_mq_timedreceive_time64_linux       BY_ARCH_linux(     void,      419,     void,      419,      419,      419) /*║*/
/*║*/ #define NR_mq_notify_linux                    BY_ARCH_linux(      244,      184,      184,      281,      278,      184) /*║*/
/*║*/ #define NR_mq_getsetattr_linux                BY_ARCH_linux(      245,      185,      185,      282,      279,      185) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                          13e. Synchronization Primitives - Futexes                                           ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_futex_linux                        BY_ARCH_linux(      202,       98,       98,      240,      240,     void) /*║*/
/*║*/ #define NR_futex_time64_linux                 BY_ARCH_linux(     void,      422,     void,      422,      422,      422) /*║*/
/*║*/ #define NR_futex_wait_linux                   BY_ARCH_linux(      455,      455,      455,      455,      455,      455) /*║*/
/*║*/ #define NR_futex_wake_linux                   BY_ARCH_linux(      454,      454,      454,      454,      454,      454) /*║*/
/*║*/ #define NR_futex_waitv_linux                  BY_ARCH_linux(      449,      449,      449,      449,      449,      449) /*║*/
/*║*/ #define NR_futex_requeue_linux                BY_ARCH_linux(      456,      456,      456,      456,      456,      456) /*║*/
/*║*/ #define NR_set_robust_list_linux              BY_ARCH_linux(      273,       99,       99,      311,      338,       99) /*║*/
/*║*/ #define NR_get_robust_list_linux              BY_ARCH_linux(      274,      100,      100,      312,      339,      100) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                     13f. Synchronization Primitives - Event Notification                                     ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_eventfd_linux                      BY_ARCH_linux(      284,     void,     void,      323,      351,     void) /*║*/
/*║*/ #define NR_eventfd2_linux                     BY_ARCH_linux(      290,       19,       19,      328,      356,       19) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                                   14. SOCKETS & NETWORKING                                                   ║*/
/*╠══════════════════════════════════════════════════════════════╦═════════╤═════════╤═════════╤═════════╤═════════╤═════════════╣*/
/*║                         Syscall Name                         ║ x86_64  │  arm64  │ riscv64 │ x86_32  │  arm32  │   riscv32   ║*/
/*╟──────────────────────────────────────────────────────────────╨─────────┴─────────┴─────────┴─────────┴─────────┴─────────────╢*/
/*║                                            14a. Creating and configuring sockets                                             ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_socket_linux                       BY_ARCH_linux(       41,      198,      198,      359,      281,      198) /*║*/
/*║*/ #define NR_socketpair_linux                   BY_ARCH_linux(       53,      199,      199,      360,      288,      199) /*║*/
/*║*/ #define NR_bind_linux                         BY_ARCH_linux(       49,      200,      200,      361,      282,      200) /*║*/
/*║*/ #define NR_listen_linux                       BY_ARCH_linux(       50,      201,      201,      363,      284,      201) /*║*/
/*║*/ #define NR_accept_linux                       BY_ARCH_linux(       43,      202,      202,     void,      285,      202) /*║*/
/*║*/ #define NR_accept4_linux                      BY_ARCH_linux(      288,      242,      242,      364,      366,      242) /*║*/
/*║*/ #define NR_connect_linux                      BY_ARCH_linux(       42,      203,      203,      362,      283,      203) /*║*/
/*║*/ #define NR_shutdown_linux                     BY_ARCH_linux(       48,      210,      210,      373,      293,      210) /*║*/
/*║*/ #define NR_socketcall_linux                   BY_ARCH_linux(     void,     void,     void,      102,     void,     void) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                          14b. Sending and receiving data on sockets                                          ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_send_linux                         BY_ARCH_linux(     void,     void,     void,     void,      289,     void) /*║*/
/*║*/ #define NR_sendto_linux                       BY_ARCH_linux(       44,      206,      206,      369,      290,      206) /*║*/
/*║*/ #define NR_sendmsg_linux                      BY_ARCH_linux(       46,      211,      211,      370,      296,      211) /*║*/
/*║*/ #define NR_sendmmsg_linux                     BY_ARCH_linux(      307,      269,      269,      345,      374,      269) /*║*/
/*║*/ #define NR_recv_linux                         BY_ARCH_linux(     void,     void,     void,     void,      291,     void) /*║*/
/*║*/ #define NR_recvfrom_linux                     BY_ARCH_linux(       45,      207,      207,      371,      292,      207) /*║*/
/*║*/ #define NR_recvmsg_linux                      BY_ARCH_linux(       47,      212,      212,      372,      297,      212) /*║*/
/*║*/ #define NR_recvmmsg_linux                     BY_ARCH_linux(      299,      243,      243,      337,      365,     void) /*║*/
/*║*/ #define NR_recvmmsg_time64_linux              BY_ARCH_linux(     void,      417,     void,      417,      417,      417) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                           14c. Getting and setting socket options                                            ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_getsockopt_linux                   BY_ARCH_linux(       55,      209,      209,      365,      295,      209) /*║*/
/*║*/ #define NR_setsockopt_linux                   BY_ARCH_linux(       54,      208,      208,      366,      294,      208) /*║*/
/*║*/ #define NR_getsockname_linux                  BY_ARCH_linux(       51,      204,      204,      367,      286,      204) /*║*/
/*║*/ #define NR_getpeername_linux                  BY_ARCH_linux(       52,      205,      205,      368,      287,      205) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                                     15. ASYNCHRONOUS I/O                                                     ║*/
/*╠══════════════════════════════════════════════════════════════╦═════════╤═════════╤═════════╤═════════╤═════════╤═════════════╣*/
/*║                         Syscall Name                         ║ x86_64  │  arm64  │ riscv64 │ x86_32  │  arm32  │   riscv32   ║*/
/*╟──────────────────────────────────────────────────────────────╨─────────┴─────────┴─────────┴─────────┴─────────┴─────────────╢*/
/*║                                             15a. AIO: asynchronous I/O interface                                             ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_io_setup_linux                     BY_ARCH_linux(      206,        0,        0,      245,      243,        0) /*║*/
/*║*/ #define NR_io_destroy_linux                   BY_ARCH_linux(      207,        1,        1,      246,      244,        1) /*║*/
/*║*/ #define NR_io_submit_linux                    BY_ARCH_linux(      209,        2,        2,      248,      246,        2) /*║*/
/*║*/ #define NR_io_cancel_linux                    BY_ARCH_linux(      210,        3,        3,      249,      247,        3) /*║*/
/*║*/ #define NR_io_getevents_linux                 BY_ARCH_linux(      208,        4,        4,      247,      245,     void) /*║*/
/*║*/ #define NR_io_pgetevents_linux                BY_ARCH_linux(      333,      292,      292,      385,      399,     void) /*║*/
/*║*/ #define NR_io_pgetevents_time64_linux         BY_ARCH_linux(     void,      416,     void,      416,      416,      416) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                       15b. io_uring: high-performance asynchronous I/O                                       ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_io_uring_setup_linux               BY_ARCH_linux(      425,      425,      425,      425,      425,      425) /*║*/
/*║*/ #define NR_io_uring_enter_linux               BY_ARCH_linux(      426,      426,      426,      426,      426,      426) /*║*/
/*║*/ #define NR_io_uring_register_linux            BY_ARCH_linux(      427,      427,      427,      427,      427,      427) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                                      16. TIME & CLOCKS                                                       ║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                        16a. Reading current time from various clocks                                         ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_time_linux                         BY_ARCH_linux(      201,     void,     void,       13,     void,     void) /*║*/
/*║*/ #define NR_gettimeofday_linux                 BY_ARCH_linux(       96,      169,      169,       78,       78,     void) /*║*/
/*║*/ #define NR_clock_gettime_linux                BY_ARCH_linux(      228,      113,      113,      265,      263,     void) /*║*/
/*║*/ #define NR_clock_gettime64_linux              BY_ARCH_linux(     void,      403,     void,      403,      403,      403) /*║*/
/*║*/ #define NR_clock_getres_linux                 BY_ARCH_linux(      229,      114,      114,      266,      264,     void) /*║*/
/*║*/ #define NR_clock_getres_time64_linux          BY_ARCH_linux(     void,      406,     void,      406,      406,      406) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                        16b. Setting system time and adjusting clocks                                         ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_settimeofday_linux                 BY_ARCH_linux(      164,      170,      170,       79,       79,     void) /*║*/
/*║*/ #define NR_clock_settime_linux                BY_ARCH_linux(      227,      112,      112,      264,      262,     void) /*║*/
/*║*/ #define NR_clock_settime64_linux              BY_ARCH_linux(     void,      404,     void,      404,      404,      404) /*║*/
/*║*/ #define NR_stime_linux                        BY_ARCH_linux(     void,     void,     void,       25,     void,     void) /*║*/
/*║*/ #define NR_adjtimex_linux                     BY_ARCH_linux(      159,      171,      171,      124,      124,     void) /*║*/
/*║*/ #define NR_clock_adjtime_linux                BY_ARCH_linux(      305,      266,      266,      343,      372,     void) /*║*/
/*║*/ #define NR_clock_adjtime64_linux              BY_ARCH_linux(     void,      405,     void,      405,      405,      405) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                        16c. Suspending execution for a period of time                                        ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_nanosleep_linux                    BY_ARCH_linux(       35,      101,      101,      162,      162,     void) /*║*/
/*║*/ #define NR_clock_nanosleep_linux              BY_ARCH_linux(      230,      115,      115,      267,      265,     void) /*║*/
/*║*/ #define NR_clock_nanosleep_time64_linux       BY_ARCH_linux(     void,      407,     void,      407,      407,      407) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                           16d. Setting periodic or one-shot timers                                           ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_alarm_linux                        BY_ARCH_linux(       37,     void,     void,       27,     void,     void) /*║*/
/*║*/ #define NR_setitimer_linux                    BY_ARCH_linux(       38,      103,      103,      104,      104,      103) /*║*/
/*║*/ #define NR_getitimer_linux                    BY_ARCH_linux(       36,      102,      102,      105,      105,      102) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                         16e. Per-process timers with precise control                                         ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_timer_create_linux                 BY_ARCH_linux(      222,      107,      107,      259,      257,      107) /*║*/
/*║*/ #define NR_timer_settime_linux                BY_ARCH_linux(      223,      110,      110,      260,      258,     void) /*║*/
/*║*/ #define NR_timer_settime64_linux              BY_ARCH_linux(     void,      409,     void,      409,      409,      409) /*║*/
/*║*/ #define NR_timer_gettime_linux                BY_ARCH_linux(      224,      108,      108,      261,      259,     void) /*║*/
/*║*/ #define NR_timer_gettime64_linux              BY_ARCH_linux(     void,      408,     void,      408,      408,      408) /*║*/
/*║*/ #define NR_timer_getoverrun_linux             BY_ARCH_linux(      225,      109,      109,      262,      260,      109) /*║*/
/*║*/ #define NR_timer_delete_linux                 BY_ARCH_linux(      226,      111,      111,      263,      261,      111) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                         16f. Timers accessible via file descriptors                                          ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_timerfd_create_linux               BY_ARCH_linux(      283,       85,       85,      322,      350,       85) /*║*/
/*║*/ #define NR_timerfd_settime_linux              BY_ARCH_linux(      286,       86,       86,      325,      353,     void) /*║*/
/*║*/ #define NR_timerfd_settime64_linux            BY_ARCH_linux(     void,      411,     void,      411,      411,      411) /*║*/
/*║*/ #define NR_timerfd_gettime_linux              BY_ARCH_linux(      287,       87,       87,      326,      354,     void) /*║*/
/*║*/ #define NR_timerfd_gettime64_linux            BY_ARCH_linux(     void,      410,     void,      410,      410,      410) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                                      17. RANDOM NUMBERS                                                      ║*/
/*╠══════════════════════════════════════════════════════════════╦═════════╤═════════╤═════════╤═════════╤═════════╤═════════════╣*/
/*║                         Syscall Name                         ║ x86_64  │  arm64  │ riscv64 │ x86_32  │  arm32  │   riscv32   ║*/
/*╟──────────────────────────────────────────────────────────────╨─────────┴─────────┴─────────┴─────────┴─────────┴─────────────╢*/
/*║*/ #define NR_getrandom_linux                    BY_ARCH_linux(      318,      278,      278,      355,      384,      278) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                                  18. USER & GROUP IDENTITY                                                   ║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                              18a. Getting and setting user IDs                                               ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_getuid_linux                       BY_ARCH_linux(      102,      174,      174,       24,       24,      174) /*║*/
/*║*/ #define NR_geteuid_linux                      BY_ARCH_linux(      107,      175,      175,       49,       49,      175) /*║*/
/*║*/ #define NR_setuid_linux                       BY_ARCH_linux(      105,      146,      146,       23,       23,      146) /*║*/
/*║*/ #define NR_setreuid_linux                     BY_ARCH_linux(      113,      145,      145,       70,       70,      145) /*║*/
/*║*/ #define NR_setresuid_linux                    BY_ARCH_linux(      117,      147,      147,      164,      164,      147) /*║*/
/*║*/ #define NR_getresuid_linux                    BY_ARCH_linux(      118,      148,      148,      165,      165,      148) /*║*/
/*║*/ #define NR_setfsuid_linux                     BY_ARCH_linux(      122,      151,      151,      138,      138,      151) /*║*/
/*║*/ #define NR_getuid32_linux                     BY_ARCH_linux(     void,     void,     void,      199,      199,     void) /*║*/
/*║*/ #define NR_geteuid32_linux                    BY_ARCH_linux(     void,     void,     void,      201,      201,     void) /*║*/
/*║*/ #define NR_setuid32_linux                     BY_ARCH_linux(     void,     void,     void,      213,      213,     void) /*║*/
/*║*/ #define NR_setreuid32_linux                   BY_ARCH_linux(     void,     void,     void,      203,      203,     void) /*║*/
/*║*/ #define NR_setresuid32_linux                  BY_ARCH_linux(     void,     void,     void,      208,      208,     void) /*║*/
/*║*/ #define NR_getresuid32_linux                  BY_ARCH_linux(     void,     void,     void,      209,      209,     void) /*║*/
/*║*/ #define NR_setfsuid32_linux                   BY_ARCH_linux(     void,     void,     void,      215,      215,     void) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                              18b. Getting and setting group IDs                                              ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_getgid_linux                       BY_ARCH_linux(      104,      176,      176,       47,       47,      176) /*║*/
/*║*/ #define NR_getegid_linux                      BY_ARCH_linux(      108,      177,      177,       50,       50,      177) /*║*/
/*║*/ #define NR_setgid_linux                       BY_ARCH_linux(      106,      144,      144,       46,       46,      144) /*║*/
/*║*/ #define NR_setregid_linux                     BY_ARCH_linux(      114,      143,      143,       71,       71,      143) /*║*/
/*║*/ #define NR_setresgid_linux                    BY_ARCH_linux(      119,      149,      149,      170,      170,      149) /*║*/
/*║*/ #define NR_getresgid_linux                    BY_ARCH_linux(      120,      150,      150,      171,      171,      150) /*║*/
/*║*/ #define NR_setfsgid_linux                     BY_ARCH_linux(      123,      152,      152,      139,      139,      152) /*║*/
/*║*/ #define NR_getgid32_linux                     BY_ARCH_linux(     void,     void,     void,      200,      200,     void) /*║*/
/*║*/ #define NR_getegid32_linux                    BY_ARCH_linux(     void,     void,     void,      202,      202,     void) /*║*/
/*║*/ #define NR_setgid32_linux                     BY_ARCH_linux(     void,     void,     void,      214,      214,     void) /*║*/
/*║*/ #define NR_setregid32_linux                   BY_ARCH_linux(     void,     void,     void,      204,      204,     void) /*║*/
/*║*/ #define NR_setresgid32_linux                  BY_ARCH_linux(     void,     void,     void,      210,      210,     void) /*║*/
/*║*/ #define NR_getresgid32_linux                  BY_ARCH_linux(     void,     void,     void,      211,      211,     void) /*║*/
/*║*/ #define NR_setfsgid32_linux                   BY_ARCH_linux(     void,     void,     void,      216,      216,     void) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                            18c. Managing supplementary group list                                            ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_getgroups_linux                    BY_ARCH_linux(      115,      158,      158,       80,       80,      158) /*║*/
/*║*/ #define NR_setgroups_linux                    BY_ARCH_linux(      116,      159,      159,       81,       81,      159) /*║*/
/*║*/ #define NR_getgroups32_linux                  BY_ARCH_linux(     void,     void,     void,      205,      205,     void) /*║*/
/*║*/ #define NR_setgroups32_linux                  BY_ARCH_linux(     void,     void,     void,      206,      206,     void) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                                 19. CAPABILITIES & SECURITY                                                  ║*/
/*╠══════════════════════════════════════════════════════════════╦═════════╤═════════╤═════════╤═════════╤═════════╤═════════════╣*/
/*║                         Syscall Name                         ║ x86_64  │  arm64  │ riscv64 │ x86_32  │  arm32  │   riscv32   ║*/
/*╟──────────────────────────────────────────────────────────────╨─────────┴─────────┴─────────┴─────────┴─────────┴─────────────╢*/
/*║                                             19a. Fine-grained privilege control                                              ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_capget_linux                       BY_ARCH_linux(      125,       90,       90,      184,      184,       90) /*║*/
/*║*/ #define NR_capset_linux                       BY_ARCH_linux(      126,       91,       91,      185,      185,       91) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                            19b. Syscall filtering and sandboxing                                             ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_seccomp_linux                      BY_ARCH_linux(      317,      277,      277,      354,      383,      277) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                            19c. Linux Security Module interfaces                                             ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_security_linux                     BY_ARCH_linux(      185,     void,     void,     void,     void,     void) /*║*/
/*║*/ #define NR_lsm_get_self_attr_linux            BY_ARCH_linux(      459,      459,      459,      459,      459,      459) /*║*/
/*║*/ #define NR_lsm_set_self_attr_linux            BY_ARCH_linux(      460,      460,      460,      460,      460,      460) /*║*/
/*║*/ #define NR_lsm_list_modules_linux             BY_ARCH_linux(      461,      461,      461,      461,      461,      461) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                               19d. Unprivileged access control                                               ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_landlock_create_ruleset_linux      BY_ARCH_linux(      444,      444,      444,      444,      444,      444) /*║*/
/*║*/ #define NR_landlock_add_rule_linux            BY_ARCH_linux(      445,      445,      445,      445,      445,      445) /*║*/
/*║*/ #define NR_landlock_restrict_self_linux       BY_ARCH_linux(      446,      446,      446,      446,      446,      446) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                              19e. Kernel key retention service                                               ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_add_key_linux                      BY_ARCH_linux(      248,      217,      217,      286,      309,      217) /*║*/
/*║*/ #define NR_request_key_linux                  BY_ARCH_linux(      249,      218,      218,      287,      310,      218) /*║*/
/*║*/ #define NR_keyctl_linux                       BY_ARCH_linux(      250,      219,      219,      288,      311,      219) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                               20. RESOURCE LIMITS & ACCOUNTING                                               ║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                       20a. Getting and setting process resource limits                                       ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_getrlimit_linux                    BY_ARCH_linux(       97,      163,      163,       76,     void,     void) /*║*/
/*║*/ #define NR_setrlimit_linux                    BY_ARCH_linux(      160,      164,      164,       75,       75,     void) /*║*/
/*║*/ #define NR_prlimit64_linux                    BY_ARCH_linux(      302,      261,      261,      340,      369,      261) /*║*/
/*║*/ #define NR_ugetrlimit_linux                   BY_ARCH_linux(     void,     void,     void,      191,      191,     void) /*║*/
/*║*/ #define NR_ulimit_linux                       BY_ARCH_linux(     void,     void,     void,       58,     void,     void) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                       20b. Getting resource usage and time statistics                                        ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_getrusage_linux                    BY_ARCH_linux(       98,      165,      165,       77,       77,      165) /*║*/
/*║*/ #define NR_times_linux                        BY_ARCH_linux(      100,      153,      153,       43,       43,      153) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                             20c. System-wide process accounting                                              ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_acct_linux                         BY_ARCH_linux(      163,       89,       89,       51,       51,       89) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                                 21. NAMESPACES & CONTAINERS                                                  ║*/
/*╠══════════════════════════════════════════════════════════════╦═════════╤═════════╤═════════╤═════════╤═════════╤═════════════╣*/
/*║                         Syscall Name                         ║ x86_64  │  arm64  │ riscv64 │ x86_32  │  arm32  │   riscv32   ║*/
/*╟──────────────────────────────────────────────────────────────╨─────────┴─────────┴─────────┴─────────┴─────────┴─────────────╢*/
/*║*/ #define NR_unshare_linux                      BY_ARCH_linux(      272,       97,       97,      310,      337,       97) /*║*/
/*║*/ #define NR_setns_linux                        BY_ARCH_linux(      308,      268,      268,      346,      375,      268) /*║*/
/*║*/ #define NR_listns_linux                       BY_ARCH_linux(      470,      470,      470,      470,      470,      470) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                               22. PROCESS INSPECTION & CONTROL                                               ║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                                   22a. Process comparison                                                    ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_kcmp_linux                         BY_ARCH_linux(      312,      272,      272,      349,      378,      272) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                                22b. Process file descriptors                                                 ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_pidfd_open_linux                   BY_ARCH_linux(      434,      434,      434,      434,      434,      434) /*║*/
/*║*/ #define NR_pidfd_getfd_linux                  BY_ARCH_linux(      438,      438,      438,      438,      438,      438) /*║*/
/*║*/ #define NR_pidfd_send_signal_linux            BY_ARCH_linux(      424,      424,      424,      424,      424,      424) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                                  22c. Process memory access                                                  ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_process_vm_readv_linux             BY_ARCH_linux(      310,      270,      270,      347,      376,      270) /*║*/
/*║*/ #define NR_process_vm_writev_linux            BY_ARCH_linux(      311,      271,      271,      348,      377,      271) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                                     22d. Process tracing                                                     ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_ptrace_linux                       BY_ARCH_linux(      101,      117,      117,       26,       26,      117) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                                    23. SYSTEM INFORMATION                                                    ║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                           23a. System name and domain information                                            ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_uname_linux                        BY_ARCH_linux(       63,      160,      160,      122,      122,      160) /*║*/
/*║*/ #define NR_olduname_linux                     BY_ARCH_linux(     void,     void,     void,      109,     void,     void) /*║*/
/*║*/ #define NR_oldolduname_linux                  BY_ARCH_linux(     void,     void,     void,       59,     void,     void) /*║*/
/*║*/ #define NR_sethostname_linux                  BY_ARCH_linux(      170,      161,      161,       74,       74,      161) /*║*/
/*║*/ #define NR_setdomainname_linux                BY_ARCH_linux(      171,      162,      162,      121,      121,      162) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                        23b. Overall system information and statistics                                        ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_sysinfo_linux                      BY_ARCH_linux(       99,      179,      179,      116,      116,      179) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                               23c. Reading kernel log messages                                               ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_syslog_linux                       BY_ARCH_linux(      103,      116,      116,      103,      103,      116) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                          23d. Getting CPU and NUMA node information                                          ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_getcpu_linux                       BY_ARCH_linux(      309,      168,      168,      318,      345,      168) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                         23e. Kernel filesystem information interface                                         ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_sysfs_linux                        BY_ARCH_linux(      139,     void,     void,      135,      135,     void) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                                      24. KERNEL MODULES                                                      ║*/
/*║                                       Loading, unloading, and querying kernel modules                                        ║*/
/*╠══════════════════════════════════════════════════════════════╦═════════╤═════════╤═════════╤═════════╤═════════╤═════════════╣*/
/*║                         Syscall Name                         ║ x86_64  │  arm64  │ riscv64 │ x86_32  │  arm32  │   riscv32   ║*/
/*╟──────────────────────────────────────────────────────────────╨─────────┴─────────┴─────────┴─────────┴─────────┴─────────────╢*/
/*║*/ #define NR_create_module_linux                BY_ARCH_linux(      174,     void,     void,      127,     void,     void) /*║*/
/*║*/ #define NR_init_module_linux                  BY_ARCH_linux(      175,      105,      105,      128,      128,      105) /*║*/
/*║*/ #define NR_finit_module_linux                 BY_ARCH_linux(      313,      273,      273,      350,      379,      273) /*║*/
/*║*/ #define NR_delete_module_linux                BY_ARCH_linux(      176,      106,      106,      129,      129,      106) /*║*/
/*║*/ #define NR_query_module_linux                 BY_ARCH_linux(      178,     void,     void,      167,     void,     void) /*║*/
/*║*/ #define NR_get_kernel_syms_linux              BY_ARCH_linux(      177,     void,     void,      130,     void,     void) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                             25. SYSTEM CONTROL & ADMINISTRATION                                              ║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                         25a. Rebooting and shutting down the system                                          ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_reboot_linux                       BY_ARCH_linux(      169,      142,      142,       88,       88,      142) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                            25b. Enabling and disabling swap areas                                            ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_swapon_linux                       BY_ARCH_linux(      167,      224,      224,       87,       87,      224) /*║*/
/*║*/ #define NR_swapoff_linux                      BY_ARCH_linux(      168,      225,      225,      115,      115,      225) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                            25c. Loading and executing new kernels                                            ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_kexec_load_linux                   BY_ARCH_linux(      246,      104,      104,      283,      347,      104) /*║*/
/*║*/ #define NR_kexec_file_load_linux              BY_ARCH_linux(      320,      294,      294,     void,      401,      294) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                         25d. Other system administration operations                                          ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_vhangup_linux                      BY_ARCH_linux(      153,       58,       58,      111,      111,       58) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                             26. PERFORMANCE MONITORING & TRACING                                             ║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                      26a. Hardware and software performance monitoring                                       ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_perf_event_open_linux              BY_ARCH_linux(      298,      241,      241,      336,      364,      241) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                                26b. Userspace dynamic tracing                                                ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_uprobe_linux                       BY_ARCH_linux(      336,     void,     void,     void,     void,     void) /*║*/
/*║*/ #define NR_uretprobe_linux                    BY_ARCH_linux(      335,     void,     void,     void,     void,     void) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                          26c. Programmable Kernel Extensions (eBPF)                                          ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_bpf_linux                          BY_ARCH_linux(      321,      280,      280,      357,      386,      280) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                                 27. DEVICE & HARDWARE ACCESS                                                 ║*/
/*╠══════════════════════════════════════════════════════════════╦═════════╤═════════╤═════════╤═════════╤═════════╤═════════════╣*/
/*║                         Syscall Name                         ║ x86_64  │  arm64  │ riscv64 │ x86_32  │  arm32  │   riscv32   ║*/
/*╟──────────────────────────────────────────────────────────────╨─────────┴─────────┴─────────┴─────────┴─────────┴─────────────╢*/
/*║                                             27a. Direct hardware I/O port access                                             ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_ioperm_linux                       BY_ARCH_linux(      173,     void,     void,      101,     void,     void) /*║*/
/*║*/ #define NR_iopl_linux                         BY_ARCH_linux(      172,     void,     void,      110,     void,     void) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                             27b. Setting I/O scheduling priority                                             ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_ioprio_set_linux                   BY_ARCH_linux(      251,       30,       30,      289,      314,       30) /*║*/
/*║*/ #define NR_ioprio_get_linux                   BY_ARCH_linux(      252,       31,       31,      290,      315,       31) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                             27c. PCI device configuration access                                             ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_pciconfig_read_linux               BY_ARCH_linux(     void,     void,     void,     void,      272,     void) /*║*/
/*║*/ #define NR_pciconfig_write_linux              BY_ARCH_linux(     void,     void,     void,     void,      273,     void) /*║*/
/*║*/ #define NR_pciconfig_iobase_linux             BY_ARCH_linux(     void,     void,     void,     void,      271,     void) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                              27d. CPU cache control operations                                               ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_cacheflush_linux                   BY_ARCH_linux(     void,     void,     void,     void, 0x0f0002,     void) /*║*/
/*║*/ #define NR_cachestat_linux                    BY_ARCH_linux(      451,      451,      451,      451,      451,      451) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                             28. ARCHITECTURE-SPECIFIC OPERATIONS                                             ║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                             28a. RISC-V architecture operations                                              ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_riscv_flush_icache_linux           BY_ARCH_linux(     void,     void,      259,     void,     void,      259) /*║*/
/*║*/ #define NR_riscv_hwprobe_linux                BY_ARCH_linux(     void,     void,      258,     void,     void,      258) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                               28b. x86 architecture operations                                               ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_vm86_linux                         BY_ARCH_linux(     void,     void,     void,      166,     void,     void) /*║*/
/*║*/ #define NR_vm86old_linux                      BY_ARCH_linux(     void,     void,     void,      113,     void,     void) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                             28c. Intel MPX support (deprecated)                                              ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_mpx_linux                          BY_ARCH_linux(     void,     void,     void,       56,     void,     void) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                                29. ADVANCED EXECUTION CONTROL                                                ║*/
/*╠══════════════════════════════════════════════════════════════╦═════════╤═════════╤═════════╤═════════╤═════════╤═════════════╣*/
/*║                         Syscall Name                         ║ x86_64  │  arm64  │ riscv64 │ x86_32  │  arm32  │   riscv32   ║*/
/*╟──────────────────────────────────────────────────────────────╨─────────┴─────────┴─────────┴─────────┴─────────┴─────────────╢*/
/*║                                                  29a. Restartable sequences                                                  ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_rseq_linux                         BY_ARCH_linux(      334,      293,      293,      386,      398,      293) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                                     29b. Restart syscall                                                     ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_restart_syscall_linux              BY_ARCH_linux(      219,      128,      128,        0,        0,      128) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                                  29c. Directory entry cache                                                  ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_lookup_dcookie_linux               BY_ARCH_linux(      212,       18,       18,      253,      249,       18) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                                             30. LEGACY, OBSOLETE & UNIMPLEMENTED                                             ║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║*/ #define NR__sysctl_linux                      BY_ARCH_linux(      156,     void,     void,      149,      149,     void) /*║*/
/*║*/ #define NR_ipc_linux                          BY_ARCH_linux(     void,     void,     void,      117,     void,     void) /*║*/
/*║*/ #define NR_profil_linux                       BY_ARCH_linux(     void,     void,     void,       98,     void,     void) /*║*/
/*║*/ #define NR_prof_linux                         BY_ARCH_linux(     void,     void,     void,       44,     void,     void) /*║*/
/*║*/ #define NR_afs_syscall_linux                  BY_ARCH_linux(      183,     void,     void,      137,     void,     void) /*║*/
/*║*/ #define NR_break_linux                        BY_ARCH_linux(     void,     void,     void,       17,     void,     void) /*║*/
/*║*/ #define NR_ftime_linux                        BY_ARCH_linux(     void,     void,     void,       35,     void,     void) /*║*/
/*║*/ #define NR_gtty_linux                         BY_ARCH_linux(     void,     void,     void,       32,     void,     void) /*║*/
/*║*/ #define NR_idle_linux                         BY_ARCH_linux(     void,     void,     void,      112,     void,     void) /*║*/
/*║*/ #define NR_lock_linux                         BY_ARCH_linux(     void,     void,     void,       53,     void,     void) /*║*/
/*║*/ #define NR_nfsservctl_linux                   BY_ARCH_linux(      180,       42,       42,      169,      169,       42) /*║*/
/*║*/ #define NR_getpmsg_linux                      BY_ARCH_linux(      181,     void,     void,      188,     void,     void) /*║*/
/*║*/ #define NR_putpmsg_linux                      BY_ARCH_linux(      182,     void,     void,      189,     void,     void) /*║*/
/*║*/ #define NR_stty_linux                         BY_ARCH_linux(     void,     void,     void,       31,     void,     void) /*║*/
/*║*/ #define NR_tuxcall_linux                      BY_ARCH_linux(      184,     void,     void,     void,     void,     void) /*║*/
/*║*/ #define NR_vserver_linux                      BY_ARCH_linux(      236,     void,     void,      273,      313,     void) /*║*/
/*║*/ #define NR_bdflush_linux                      BY_ARCH_linux(     void,     void,     void,      134,      134,     void) /*║*/
/*║*/ #define NR_uselib_linux                       BY_ARCH_linux(      134,     void,     void,       86,       86,     void) /*║*/
/*╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣*/
/*║                   generated by https://github.com/t-cadet/c-resources/blob/main/linux/get_syscall_tables.c                   ║*/
/*╚══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝*/

#define Syscall0_linux(number, ret2)                   _Syscall0_linux(number, (long*)(ret2))
#define Syscall1_linux(number, a, ret2)                _Syscall1_linux(number, (long)(a), (long*)(ret2))
#define Syscall2_linux(number, a, b, ret2)             _Syscall2_linux(number, (long)(a), (long)(b), (long*)(ret2))
#define Syscall3_linux(number, a, b, c, ret2)          _Syscall3_linux(number, (long)(a), (long)(b), (long)(c), (long*)(ret2))
#define Syscall4_linux(number, a, b, c, d, ret2)       _Syscall4_linux(number, (long)(a), (long)(b), (long)(c), (long)(d), (long*)(ret2))
#define Syscall5_linux(number, a, b, c, d, e, ret2)    _Syscall5_linux(number, (long)(a), (long)(b), (long)(c), (long)(d), (long)(e), (long*)(ret2))
#define Syscall6_linux(number, a, b, c, d, e, f, ret2) _Syscall6_linux(number, (long)(a), (long)(b), (long)(c), (long)(d), (long)(e), (long)(f), (long*)(ret2))

long _Syscall0_linux(long number, long* ret2);
long _Syscall1_linux(long number, long a, long* ret2);
long _Syscall2_linux(long number, long a, long b, long* ret2);
long _Syscall3_linux(long number, long a, long b, long c, long* ret2);
long _Syscall4_linux(long number, long a, long b, long c, long d, long* ret2);
long _Syscall5_linux(long number, long a, long b, long c, long d, long e, long* ret2);
long _Syscall6_linux(long number, long a, long b, long c, long d, long e, long f, long* ret2);

#endif // C_LINUX_HEADER
#ifdef C_LINUX_IMPLEMENTATION

#if defined(__x86_64__)
  long _Syscall0_linux(long number, long* ret2) {
    register long rax __asm__("rax") = number;
    register long rdx __asm__("rdx");
    __asm__ volatile (
      "syscall"
      : "=r" (rax), "=r" (rdx)
      : "r" (rax)
      : "rcx", "r11", "memory"
    );
    if (ret2) {
      *ret2 = rdx;
    }
    return rax;
  }

  long _Syscall1_linux(long number, long a, long* ret2) {
    register long rax __asm__("rax") = number;
    register long rdi __asm__("rdi") = a;
    register long rdx __asm__("rdx");
    __asm__ volatile (
      "syscall"
      : "=r" (rax), "=r" (rdx)
      : "r" (rax), "r" (rdi)
      : "rcx", "r11", "memory"
    );
    if (ret2) {
      *ret2 = rdx;
    }
    return rax;
  }

  long _Syscall2_linux(long number, long a, long b, long* ret2) {
    register long rax __asm__("rax") = number;
    register long rdi __asm__("rdi") = a;
    register long rsi __asm__("rsi") = b;
    register long rdx __asm__("rdx");
    __asm__ volatile (
      "syscall"
      : "=r" (rax), "=r" (rdx)
      : "r" (rax), "r" (rdi), "r" (rsi)
      : "rcx", "r11", "memory"
    );
    if (ret2) {
      *ret2 = rdx;
    }
    return rax;
  }

  long _Syscall3_linux(long number, long a, long b, long c, long* ret2) {
    register long rax __asm__("rax") = number;
    register long rdi __asm__("rdi") = a;
    register long rsi __asm__("rsi") = b;
    register long rdx __asm__("rdx") = c;
    __asm__ volatile (
      "syscall"
      : "=r" (rax), "=r" (rdx)
      : "r" (rax), "r" (rdi), "r" (rsi), "r" (rdx)
      : "rcx", "r11", "memory"
    );
    if (ret2) {
      *ret2 = rdx;
    }
    return rax;
  }

  long _Syscall4_linux(long number, long a, long b, long c, long d, long* ret2) {
    register long rax __asm__("rax") = number;
    register long rdi __asm__("rdi") = a;
    register long rsi __asm__("rsi") = b;
    register long rdx __asm__("rdx") = c;
    register long r10 __asm__("r10") = d;
    __asm__ volatile (
      "syscall"
      : "=r" (rax), "=r" (rdx)
      : "r" (rax), "r" (rdi), "r" (rsi), "r" (rdx), "r" (r10)
      : "rcx", "r11", "memory"
    );
    if (ret2) {
      *ret2 = rdx;
    }
    return rax;
  }

  long _Syscall5_linux(long number, long a, long b, long c, long d, long e, long* ret2) {
    register long rax __asm__("rax") = number;
    register long rdi __asm__("rdi") = a;
    register long rsi __asm__("rsi") = b;
    register long rdx __asm__("rdx") = c;
    register long r10 __asm__("r10") = d;
    register long r8 __asm__("r8") = e;
    __asm__ volatile (
      "syscall"
      : "=r" (rax), "=r" (rdx)
      : "r" (rax), "r" (rdi), "r" (rsi), "r" (rdx), "r" (r10), "r" (r8)
      : "rcx", "r11", "memory"
    );
    if (ret2) {
      *ret2 = rdx;
    }
    return rax;
  }

  long _Syscall6_linux(long number, long a, long b, long c, long d, long e, long f, long* ret2) {
    register long rax __asm__("rax") = number;
    register long rdi __asm__("rdi") = a;
    register long rsi __asm__("rsi") = b;
    register long rdx __asm__("rdx") = c;
    register long r10 __asm__("r10") = d;
    register long r8 __asm__("r8") = e;
    register long r9 __asm__("r9") = f;
    __asm__ volatile (
      "syscall"
      : "=r" (rax), "=r" (rdx)
      : "r" (rax), "r" (rdi), "r" (rsi), "r" (rdx), "r" (r10), "r" (r8), "r" (r9)
      : "rcx", "r11", "memory"
    );
    if (ret2) {
      *ret2 = rdx;
    }
    return rax;
  }
#elif defined(__aarch64__)
  long _Syscall0_linux(long number, long* ret2) {
    register long x8 __asm__("x8") = number;
    register long x0 __asm__("x0");
    register long x1 __asm__("x1");
    __asm__ volatile (
      "svc #0"
      : "=r" (x0), "=r" (x1)
      : "r" (x8)
      : "memory", "cc"
    );
    if (ret2) {
      *ret2 = x1;
    }
    return x0;
  }

  long _Syscall1_linux(long number, long a, long* ret2) {
    register long x8 __asm__("x8") = number;
    register long x0 __asm__("x0") = a;
    register long x1 __asm__("x1");
    __asm__ volatile (
      "svc #0"
      : "=r" (x0), "=r" (x1)
      : "r" (x8), "r" (x0)
      : "memory", "cc"
    );
    if (ret2) {
      *ret2 = x1;
    }
    return x0;
  }

  long _Syscall2_linux(long number, long a, long b, long* ret2) {
    register long x8 __asm__("x8") = number;
    register long x0 __asm__("x0") = a;
    register long x1 __asm__("x1") = b;
    __asm__ volatile (
      "svc #0"
      : "=r" (x0), "=r" (x1)
      : "r" (x8), "r" (x0), "r" (x1)
      : "memory", "cc"
    );
    if (ret2) {
      *ret2 = x1;
    }
    return x0;
  }

  long _Syscall3_linux(long number, long a, long b, long c, long* ret2) {
    register long x8 __asm__("x8") = number;
    register long x0 __asm__("x0") = a;
    register long x1 __asm__("x1") = b;
    register long x2 __asm__("x2") = c;
    __asm__ volatile (
      "svc #0"
      : "=r" (x0), "=r" (x1)
      : "r" (x8), "r" (x0), "r" (x1), "r" (x2)
      : "memory", "cc"
    );
    if (ret2) {
      *ret2 = x1;
    }
    return x0;
  }

  long _Syscall4_linux(long number, long a, long b, long c, long d, long* ret2) {
    register long x8 __asm__("x8") = number;
    register long x0 __asm__("x0") = a;
    register long x1 __asm__("x1") = b;
    register long x2 __asm__("x2") = c;
    register long x3 __asm__("x3") = d;
    __asm__ volatile (
      "svc #0"
      : "=r" (x0), "=r" (x1)
      : "r" (x8), "r" (x0), "r" (x1), "r" (x2), "r" (x3)
      : "memory", "cc"
    );
    if (ret2) {
      *ret2 = x1;
    }
    return x0;
  }

  long _Syscall5_linux(long number, long a, long b, long c, long d, long e, long* ret2) {
    register long x8 __asm__("x8") = number;
    register long x0 __asm__("x0") = a;
    register long x1 __asm__("x1") = b;
    register long x2 __asm__("x2") = c;
    register long x3 __asm__("x3") = d;
    register long x4 __asm__("x4") = e;
    __asm__ volatile (
      "svc #0"
      : "=r" (x0), "=r" (x1)
      : "r" (x8), "r" (x0), "r" (x1), "r" (x2), "r" (x3), "r" (x4)
      : "memory", "cc"
    );
    if (ret2) {
      *ret2 = x1;
    }
    return x0;
  }

  long _Syscall6_linux(long number, long a, long b, long c, long d, long e, long f, long* ret2) {
    register long x8 __asm__("x8") = number;
    register long x0 __asm__("x0") = a;
    register long x1 __asm__("x1") = b;
    register long x2 __asm__("x2") = c;
    register long x3 __asm__("x3") = d;
    register long x4 __asm__("x4") = e;
    register long x5 __asm__("x5") = f;
    __asm__ volatile (
      "svc #0"
      : "=r" (x0), "=r" (x1)
      : "r" (x8), "r" (x0), "r" (x1), "r" (x2), "r" (x3), "r" (x4), "r" (x5)
      : "memory", "cc"
    );
    if (ret2) {
      *ret2 = x1;
    }
    return x0;
  }
#elif defined(__riscv)
  // riscv32 & riscv64 have the same syscall conventions
  long _Syscall0_linux(long number, long* ret2) {
    register long a7 __asm__("a7") = number;
    register long a0 __asm__("a0");
    register long a1 __asm__("a1");
    __asm__ volatile (
      "ecall"
      : "=r" (a0), "=r" (a1)
      : "r" (a7)
      : "memory"
    );
    if (ret2) {
      *ret2 = a1;
    }
    return a0;
  }

  long _Syscall1_linux(long number, long a, long* ret2) {
    register long a7 __asm__("a7") = number;
    register long a0 __asm__("a0") = a;
    register long a1 __asm__("a1");
    __asm__ volatile (
      "ecall"
      : "=r" (a0), "=r" (a1)
      : "r" (a7), "r" (a0)
      : "memory"
    );
    if (ret2) {
      *ret2 = a1;
    }
    return a0;
  }

  long _Syscall2_linux(long number, long a, long b, long* ret2) {
    register long a7 __asm__("a7") = number;
    register long a0 __asm__("a0") = a;
    register long a1 __asm__("a1") = b;
    __asm__ volatile (
      "ecall"
      : "=r" (a0), "=r" (a1)
      : "r" (a7), "r" (a0), "r" (a1)
      : "memory"
    );
    if (ret2) {
      *ret2 = a1;
    }
    return a0;
  }
  long _Syscall3_linux(long number, long a, long b, long c, long* ret2) {
    register long a7 __asm__("a7") = number;
    register long a0 __asm__("a0") = a;
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    __asm__ volatile (
      "ecall"
      : "=r" (a0), "=r" (a1)
      : "r" (a7), "r" (a0), "r" (a1), "r" (a2)
      : "memory"
    );
    if (ret2) {
      *ret2 = a1;
    }
    return a0;
  }

  long _Syscall4_linux(long number, long a, long b, long c, long d, long* ret2) {
    register long a7 __asm__("a7") = number;
    register long a0 __asm__("a0") = a;
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    __asm__ volatile (
      "ecall"
      : "=r" (a0), "=r" (a1)
      : "r" (a7), "r" (a0), "r" (a1), "r" (a2), "r" (a3)
      : "memory"
    );
    if (ret2) {
      *ret2 = a1;
    }
    return a0;
  }

  long _Syscall5_linux(long number, long a, long b, long c, long d, long e, long* ret2) {
    register long a7 __asm__("a7") = number;
    register long a0 __asm__("a0") = a;
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    __asm__ volatile (
      "ecall"
      : "=r" (a0), "=r" (a1)
      : "r" (a7), "r" (a0), "r" (a1), "r" (a2), "r" (a3), "r" (a4)
      : "memory"
    );
    if (ret2) {
      *ret2 = a1;
    }
    return a0;
  }

  long _Syscall6_linux(long number, long a, long b, long c, long d, long e, long f, long* ret2) {
    register long a7 __asm__("a7") = number;
    register long a0 __asm__("a0") = a;
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    register long a5 __asm__("a5") = f;
    __asm__ volatile (
      "ecall"
      : "=r" (a0), "=r" (a1)
      : "r" (a7), "r" (a0), "r" (a1), "r" (a2), "r" (a3), "r" (a4), "r" (a5)
      : "memory"
    );
    if (ret2) {
      *ret2 = a1;
    }
    return a0;
  }
#elif defined(__i386__)
  long _Syscall0_linux(long number, long* ret2) {
    register long eax __asm__("eax") = number;
    register long edx __asm__("edx");
    __asm__ volatile (
      "int $0x80"
      : "=r" (eax), "=r" (edx)
      : "r" (eax)
      : "memory"
    );
    if (ret2) {
      *ret2 = edx;
    }
    return eax;
  }

  long _Syscall1_linux(long number, long a, long* ret2) {
    register long eax __asm__("eax") = number;
    register long ebx __asm__("ebx") = a;
    register long edx __asm__("edx");
    __asm__ volatile (
      "int $0x80"
      : "=r" (eax), "=r" (edx)
      : "r" (eax), "r" (ebx)
      : "memory"
    );
    if (ret2) {
      *ret2 = edx;
    }
    return eax;
  }

  long _Syscall2_linux(long number, long a, long b, long* ret2) {
    register long eax __asm__("eax") = number;
    register long ebx __asm__("ebx") = a;
    register long ecx __asm__("ecx") = b;
    register long edx __asm__("edx");
    __asm__ volatile (
      "int $0x80"
      : "=r" (eax), "=r" (edx)
      : "r" (eax), "r" (ebx), "r" (ecx)
      : "memory"
    );
    if (ret2) {
      *ret2 = edx;
    }
    return eax;
  }

  long _Syscall3_linux(long number, long a, long b, long c, long* ret2) {
    register long eax __asm__("eax") = number;
    register long ebx __asm__("ebx") = a;
    register long ecx __asm__("ecx") = b;
    register long edx __asm__("edx") = c;
    __asm__ volatile (
      "int $0x80"
      : "=r" (eax), "=r" (edx)
      : "r" (eax), "r" (ebx), "r" (ecx), "r" (edx)
      : "memory"
    );
    if (ret2) {
      *ret2 = edx;
    }
    return eax;
  }

  long _Syscall4_linux(long number, long a, long b, long c, long d, long* ret2) {
    register long eax __asm__("eax") = number;
    register long ebx __asm__("ebx") = a;
    register long ecx __asm__("ecx") = b;
    register long edx __asm__("edx") = c;
    register long esi __asm__("esi") = d;
    __asm__ volatile (
      "int $0x80"
      : "=r" (eax), "=r" (edx)
      : "r" (eax), "r" (ebx), "r" (ecx), "r" (edx), "r" (esi)
      : "memory"
    );
    if (ret2) {
      *ret2 = edx;
    }
    return eax;
  }

  long _Syscall5_linux(long number, long a, long b, long c, long d, long e, long* ret2) {
    register long eax __asm__("eax") = number;
    register long ebx __asm__("ebx") = a;
    register long ecx __asm__("ecx") = b;
    register long edx __asm__("edx") = c;
    register long esi __asm__("esi") = d;
    register long edi __asm__("edi") = e;
    __asm__ volatile (
      "int $0x80"
      : "=r" (eax), "=r" (edx)
      : "r" (eax), "r" (ebx), "r" (ecx), "r" (edx), "r" (esi), "r" (edi)
      : "memory"
    );
    if (ret2) {
      *ret2 = edx;
    }
    return eax;
  }

  long _Syscall6_linux(long number, long a, long b, long c, long d, long e, long f, long* ret2) {
    register long eax __asm__("eax") = number;
    register long ebx __asm__("ebx") = a;
    register long ecx __asm__("ecx") = b;
    register long edx __asm__("edx") = c;
    register long esi __asm__("esi") = d;
    register long edi __asm__("edi") = e;
    __asm__ volatile (
      // f is passed through the stack base register,
      // we must save and restore it manually
      "pushl %[f]\n\t"
      "pushl %%ebp\n\t"
      "movl  4(%%esp), %%ebp\n\t"
      "int   $0x80\n\t"
      "popl  %%ebp\n\t"
      "addl  $4, %%esp\n\t"
      : "=r" (eax), "=r" (edx)
      : "r" (eax), "r" (ebx), "r" (ecx), "r" (edx), "r" (esi), "r" (edi), [f] "g" (f)
      : "memory"
    );
    if (ret2) {
      *ret2 = edx;
    }
    return eax;
  }
#elif defined(__arm__)
  long _Syscall0_linux(long number, long* ret2) {
    register long r7 __asm__("r7") = number;
    register long r0 __asm__("r0");
    register long r1 __asm__("r1");
    __asm__ volatile (
      "svc #0"
      : "=r" (r0), "=r" (r1)
      : "r" (r7)
      : "memory", "cc"
    );
    if (ret2) {
      *ret2 = r1;
    }
    return r0;
  }

  long _Syscall1_linux(long number, long a, long* ret2) {
    register long r7 __asm__("r7") = number;
    register long r0 __asm__("r0") = a;
    register long r1 __asm__("r1");
    __asm__ volatile (
      "svc #0"
      : "=r" (r0), "=r" (r1)
      : "r" (r7), "r" (r0)
      : "memory", "cc"
    );
    if (ret2) {
      *ret2 = r1;
    }
    return r0;
  }

  long _Syscall2_linux(long number, long a, long b, long* ret2) {
    register long r7 __asm__("r7") = number;
    register long r0 __asm__("r0") = a;
    register long r1 __asm__("r1") = b;
    __asm__ volatile (
      "svc #0"
      : "=r" (r0), "=r" (r1)
      : "r" (r7), "r" (r0), "r" (r1)
      : "memory", "cc"
    );
    if (ret2) {
      *ret2 = r1;
    }
    return r0;
  }

  long _Syscall3_linux(long number, long a, long b, long c, long* ret2) {
    register long r7 __asm__("r7") = number;
    register long r0 __asm__("r0") = a;
    register long r1 __asm__("r1") = b;
    register long r2 __asm__("r2") = c;
    __asm__ volatile (
      "svc #0"
      : "=r" (r0), "=r" (r1)
      : "r" (r7), "r" (r0), "r" (r1), "r" (r2)
      : "memory", "cc"
    );
    if (ret2) {
      *ret2 = r1;
    }
    return r0;
  }

  long _Syscall4_linux(long number, long a, long b, long c, long d, long* ret2) {
    register long r7 __asm__("r7") = number;
    register long r0 __asm__("r0") = a;
    register long r1 __asm__("r1") = b;
    register long r2 __asm__("r2") = c;
    register long r3 __asm__("r3") = d;
    __asm__ volatile (
      "svc #0"
      : "=r" (r0), "=r" (r1)
      : "r" (r7), "r" (r0), "r" (r1), "r" (r2), "r" (r3)
      : "memory", "cc"
    );
    if (ret2) {
      *ret2 = r1;
    }
    return r0;
  }

  long _Syscall5_linux(long number, long a, long b, long c, long d, long e, long* ret2) {
    register long r7 __asm__("r7") = number;
    register long r0 __asm__("r0") = a;
    register long r1 __asm__("r1") = b;
    register long r2 __asm__("r2") = c;
    register long r3 __asm__("r3") = d;
    register long r4 __asm__("r4") = e;
    __asm__ volatile (
      "svc #0"
      : "=r" (r0), "=r" (r1)
      : "r" (r7), "r" (r0), "r" (r1), "r" (r2), "r" (r3), "r" (r4)
      : "memory", "cc"
    );
    if (ret2) {
      *ret2 = r1;
    }
    return r0;
  }

  long _Syscall6_linux(long number, long a, long b, long c, long d, long e, long f, long* ret2) {
    register long r7 __asm__("r7") = number;
    register long r0 __asm__("r0") = a;
    register long r1 __asm__("r1") = b;
    register long r2 __asm__("r2") = c;
    register long r3 __asm__("r3") = d;
    register long r4 __asm__("r4") = e;
    register long r5 __asm__("r5") = f;
    __asm__ volatile (
      "svc #0"
      : "=r" (r0), "=r" (r1)
      : "r" (r7), "r" (r0), "r" (r1), "r" (r2), "r" (r3), "r" (r4), "r" (r5)
      : "memory", "cc"
    );
    if (ret2) {
      *ret2 = r1;
    }
    return r0;
  }
#endif

#endif // C_LINUX_IMPLEMENTATION

