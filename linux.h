#ifndef C_LINUX_HEADER
#define C_LINUX_HEADER

// === linux.h: Cross-architecture Linux API ===================================
// 
// Contents:
//   * syscall number table                (jump: NR_fork_linux)
//   * WIP: syscall API types & constants  (jump: CSIGNAL_linux)
//   * syscallN generic wrappers           (jump: Syscall0_linux)
//   * WIP: syscall-specific wrappers      (jump: fork_linux)
// 
// Usage:
//   linux.h is a libc-free & zero-dependency header-only library for C & C++
// 
//   #include "c/linux.h" // use as header file
//
//   #define C_LINUX_IMPLEMENTATION
//   #include "c/linux.h" // use as implementation file
//
// /!\ Warning:
//   Wrappers that require a fallback are tricky and may contain subtle bugs,
//   audit the code before using it for anything important.
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

#ifndef C_BITS
  #define LO32_bits(x) ((unsigned int)(x))
  #define HI32_bits(x) ((unsigned int)((x) >> 32))
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
/*║                                             2b. Process control and personality                                              ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_prctl_linux                        BY_ARCH_linux(      157,      167,      167,      172,      172,      167) /*║*/
/*║*/ #define NR_personality_linux                  BY_ARCH_linux(      135,       92,       92,      136,      136,       92) /*║*/
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
/*║*/ #define NR_gethostname_linux                  BY_ARCH_linux(     void,     void,     void,     void,     void,     void) /*║*/
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
/*║                                               28a. x86 architecture operations                                               ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_arch_prctl_linux                   BY_ARCH_linux(      158,     void,     void,      384,     void,     void) /*║*/
/*║*/ #define NR_modify_ldt_linux                   BY_ARCH_linux(      154,     void,     void,      123,     void,     void) /*║*/
/*║*/ #define NR_set_thread_area_linux              BY_ARCH_linux(      205,     void,     void,      243,     void,     void) /*║*/
/*║*/ #define NR_get_thread_area_linux              BY_ARCH_linux(      211,     void,     void,      244,     void,     void) /*║*/
/*║*/ #define NR_vm86_linux                         BY_ARCH_linux(     void,     void,     void,      166,     void,     void) /*║*/
/*║*/ #define NR_vm86old_linux                      BY_ARCH_linux(     void,     void,     void,      113,     void,     void) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                               28b. ARM architecture operations                                               ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_set_tls_linux                      BY_ARCH_linux(     void,     void,     void,     void, 0x0f0005,     void) /*║*/
/*║*/ #define NR_get_tls_linux                      BY_ARCH_linux(     void,     void,     void,     void, 0x0f0006,     void) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                             28c. RISC-V architecture operations                                              ║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║*/ #define NR_riscv_flush_icache_linux           BY_ARCH_linux(     void,     void,      259,     void,     void,      259) /*║*/
/*║*/ #define NR_riscv_hwprobe_linux                BY_ARCH_linux(     void,     void,      258,     void,     void,      258) /*║*/
/*╟──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╢*/
/*║                                             28d. Intel MPX support (deprecated)                                              ║*/
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

#define EPERM_linux            1
#define ENOENT_linux           2
#define ESRCH_linux            3
#define EINTR_linux            4
#define EIO_linux              5
#define ENXIO_linux            6
#define E2BIG_linux            7
#define ENOEXEC_linux          8
#define EBADF_linux            9
#define ECHILD_linux           10
#define EAGAIN_linux           11
#define ENOMEM_linux           12
#define EACCES_linux           13
#define EFAULT_linux           14
#define ENOTBLK_linux          15
#define EBUSY_linux            16
#define EEXIST_linux           17
#define EXDEV_linux            18
#define ENODEV_linux           19
#define ENOTDIR_linux          20
#define EISDIR_linux           21
#define EINVAL_linux           22
#define ENFILE_linux           23
#define EMFILE_linux           24
#define ENOTTY_linux           25
#define ETXTBSY_linux          26
#define EFBIG_linux            27
#define ENOSPC_linux           28
#define ESPIPE_linux           29
#define EROFS_linux            30
#define EMLINK_linux           31
#define EPIPE_linux            32
#define EDOM_linux             33
#define ERANGE_linux           34
#define ENOSYS_linux           38

#define CSIGNAL_linux              0x000000ff
#define CLONE_VM_linux             0x00000100
#define CLONE_FS_linux             0x00000200
#define CLONE_FILES_linux          0x00000400
#define CLONE_SIGHAND_linux        0x00000800
#define CLONE_PIDFD_linux          0x00001000
#define CLONE_PTRACE_linux         0x00002000
#define CLONE_VFORK_linux          0x00004000
#define CLONE_PARENT_linux         0x00008000
#define CLONE_THREAD_linux         0x00010000
#define CLONE_NEWNS_linux          0x00020000
#define CLONE_SYSVSEM_linux        0x00040000
#define CLONE_SETTLS_linux         0x00080000
#define CLONE_PARENT_SETTID_linux  0x00100000
#define CLONE_CHILD_CLEARTID_linux 0x00200000
#define CLONE_DETACHED_linux       0x00400000
#define CLONE_UNTRACED_linux       0x00800000
#define CLONE_CHILD_SETTID_linux   0x01000000
#define CLONE_NEWCGROUP_linux      0x02000000
#define CLONE_NEWUTS_linux         0x04000000
#define CLONE_NEWIPC_linux         0x08000000
#define CLONE_NEWUSER_linux        0x10000000
#define CLONE_NEWPID_linux         0x20000000
#define CLONE_NEWNET_linux         0x40000000
#define CLONE_IO_linux             0x80000000
#define CLONE_CLEAR_SIGHAND_linux  0x100000000ULL
#define CLONE_INTO_CGROUP_linux    0x200000000ULL
#define CLONE_NEWTIME_linux        0x00000080

#define CLONE_ARGS_SIZE_VER0_linux 64
#define CLONE_ARGS_SIZE_VER1_linux 80
#define CLONE_ARGS_SIZE_VER2_linux 88

#define SCHED_NORMAL_linux                    0
#define SCHED_FIFO_linux                      1
#define SCHED_RR_linux                        2
#define SCHED_BATCH_linux                     3
#define SCHED_IDLE_linux                      5
#define SCHED_DEADLINE_linux                  6
#define SCHED_EXT_linux                       7

#define SCHED_RESET_ON_FORK_linux             0x40000000

#define SCHED_FLAG_RESET_ON_FORK_linux        0x01
#define SCHED_FLAG_RECLAIM_linux              0x02
#define SCHED_FLAG_DL_OVERRUN_linux           0x04
#define SCHED_FLAG_KEEP_POLICY_linux          0x08
#define SCHED_FLAG_KEEP_PARAMS_linux          0x10
#define SCHED_FLAG_UTIL_CLAMP_MIN_linux       0x20
#define SCHED_FLAG_UTIL_CLAMP_MAX_linux       0x40

#define SCHED_FLAG_KEEP_ALL_linux             (SCHED_FLAG_KEEP_POLICY_linux | SCHED_FLAG_KEEP_PARAMS_linux)
#define SCHED_FLAG_UTIL_CLAMP_linux           (SCHED_FLAG_UTIL_CLAMP_MIN_linux | SCHED_FLAG_UTIL_CLAMP_MAX_linux)
#define SCHED_FLAG_ALL_linux                  (SCHED_FLAG_RESET_ON_FORK_linux | SCHED_FLAG_RECLAIM_linux | SCHED_FLAG_DL_OVERRUN_linux | SCHED_FLAG_KEEP_ALL_linux | SCHED_FLAG_UTIL_CLAMP_linux)

#define SCHED_ATTR_SIZE_VER0_linux            48
#define SCHED_ATTR_SIZE_VER1_linux            56

#define PRIO_MIN_linux                        (-20)
#define PRIO_MAX_linux                        20

#define PRIO_PROCESS_linux                    0
#define PRIO_PGRP_linux                       1
#define PRIO_USER_linux                       2

#define EXIT_SUCCESS_linux 0
#define EXIT_FAILURE_linux 1

#define EX_OK_linux          0
#define EX__BASE_linux       64
#define EX_USAGE_linux       64
#define EX_DATAERR_linux     65
#define EX_NOINPUT_linux     66
#define EX_NOUSER_linux      67
#define EX_NOHOST_linux      68
#define EX_UNAVAILABLE_linux 69
#define EX_SOFTWARE_linux    70
#define EX_OSERR_linux       71
#define EX_OSFILE_linux      72
#define EX_CANTCREAT_linux   73
#define EX_IOERR_linux       74
#define EX_TEMPFAIL_linux    75
#define EX_PROTOCOL_linux    76
#define EX_NOPERM_linux      77
#define EX_CONFIG_linux      78
#define EX__MAX_linux        78

#define WNOHANG_linux     0x00000001
#define WUNTRACED_linux   0x00000002
#define WSTOPPED_linux    WUNTRACED_linux
#define WEXITED_linux     0x00000004
#define WCONTINUED_linux  0x00000008
#define WNOWAIT_linux     0x01000000
#define __WNOTHREAD_linux 0x20000000
#define __WALL_linux      0x40000000
#define __WCLONE_linux    0x80000000

#define WAIT_ANY_linux    -1
#define WAIT_MYPGRP_linux  0

#define WEXITSTATUS_linux(s)  (((s) & 0xff00) >> 8)
#define WTERMSIG_linux(s)     ((s) & 0x7f)
#define WSTOPSIG_linux(s)     WEXITSTATUS(s)
#define WCOREDUMP_linux(s)    ((s) & 0x80)
#define WIFEXITED_linux(s)    (!WTERMSIG(s))
#define WIFSTOPPED_linux(s)   ((short)((((s)&0xffff)*0x10001U)>>8) > 0x7f00)
#define WIFSIGNALED_linux(s)  (((s)&0xffff)-1U < 0xffu)
#define WIFCONTINUED_linux(s) ((s) == 0xffff)

#define P_ALL_linux   0
#define P_PID_linux   1
#define P_PGID_linux  2
#define P_PIDFD_linux 3

#define _NSIG_linux       64
#define _NSIG_BPW_linux   (sizeof(long)*8)
#define _NSIG_WORDS_linux (_NSIG_linux / _NSIG_BPW_linux)

#define SIGHUP_linux     1
#define SIGINT_linux     2
#define SIGQUIT_linux    3
#define SIGILL_linux     4
#define SIGTRAP_linux    5
#define SIGABRT_linux    6
#define SIGIOT_linux     6
#define SIGBUS_linux     7
#define SIGFPE_linux     8
#define SIGKILL_linux    9
#define SIGUSR1_linux   10
#define SIGSEGV_linux   11
#define SIGUSR2_linux   12
#define SIGPIPE_linux   13
#define SIGALRM_linux   14
#define SIGTERM_linux   15
#define SIGSTKFLT_linux 16
#define SIGCHLD_linux   17
#define SIGCONT_linux   18
#define SIGSTOP_linux   19
#define SIGTSTP_linux   20
#define SIGTTIN_linux   21
#define SIGTTOU_linux   22
#define SIGURG_linux    23
#define SIGXCPU_linux   24
#define SIGXFSZ_linux   25
#define SIGVTALRM_linux 26
#define SIGPROF_linux   27
#define SIGWINCH_linux  28
#define SIGIO_linux     29
#define SIGPOLL_linux   SIGIO
// #define SIGLOST_linux  29
#define SIGPWR_linux    30
#define SIGSYS_linux    31
#define SIGUNUSED_linux 31

#define SIGRTMIN_linux 32
#define SIGRTMAX_linux _NSIG_linux

#define SA_NOCLDSTOP_linux      0x00000001
#define SA_NOCLDWAIT_linux      0x00000002
#define SA_SIGINFO_linux        0x00000004
#define SA_UNSUPPORTED_linux    0x00000400
#define SA_EXPOSE_TAGBITS_linux 0x00000800
#define SA_ONSTACK_linux        0x08000000
#define SA_RESTART_linux        0x10000000
#define SA_NODEFER_linux        0x40000000
#define SA_RESETHAND_linux      0x80000000
#define SA_NOMASK_linux         SA_NODEFER_linux
#define SA_ONESHOT_linux        SA_RESETHAND_linux

#define SIG_BLOCK_linux    0
#define SIG_UNBLOCK_linux  1
#define SIG_SETMASK_linux  2

#define SIG_DFL_linux   ((void (*)(int))0)
#define SIG_IGN_linux   ((void (*)(int))1)
#define SIG_ERR_linux   ((void (*)(int))-1)

#define SI_USER_linux       0
#define SI_KERNEL_linux  0x80
#define SI_QUEUE_linux     -1
#define SI_TIMER_linux     -2
#define SI_MESGQ_linux     -3
#define SI_ASYNCIO_linux   -4
#define SI_SIGIO_linux     -5
#define SI_TKILL_linux     -6
#define SI_DETHREAD_linux  -7
#define SI_ASYNCNL_linux  -60

#define SI_FROMUSER_linux(siptr) ((siptr)->si_code <= 0)
#define SI_FROMKERNEL_linux(siptr) ((siptr)->si_code > 0)

#define ILL_ILLOPC_linux    1
#define ILL_ILLOPN_linux    2
#define ILL_ILLADR_linux    3
#define ILL_ILLTRP_linux    4
#define ILL_PRVOPC_linux    5
#define ILL_PRVREG_linux    6
#define ILL_COPROC_linux    7
#define ILL_BADSTK_linux    8
#define ILL_BADIADDR_linux  9
#define __ILL_BREAK_linux  10
#define __ILL_BNDMOD_linux 11
#define NSIGILL_linux      11

#define FPE_INTDIV_linux    1
#define FPE_INTOVF_linux    2
#define FPE_FLTDIV_linux    3
#define FPE_FLTOVF_linux    4
#define FPE_FLTUND_linux    5
#define FPE_FLTRES_linux    6
#define FPE_FLTINV_linux    7
#define FPE_FLTSUB_linux    8
#define __FPE_DECOVF_linux  9
#define __FPE_DECDIV_linux 10
#define __FPE_DECERR_linux 11
#define __FPE_INVASC_linux 12
#define __FPE_INVDEC_linux 13
#define FPE_FLTUNK_linux   14
#define FPE_CONDTRAP_linux 15
#define NSIGFPE_linux      15

#define SEGV_MAPERR_linux   1
#define SEGV_ACCERR_linux   2
#define SEGV_BNDERR_linux   3
#define SEGV_PKUERR_linux   4
#define SEGV_ACCADI_linux   5
#define SEGV_ADIDERR_linux  6
#define SEGV_ADIPERR_linux  7
#define SEGV_MTEAERR_linux  8
#define SEGV_MTESERR_linux  9
#define SEGV_CPERR_linux   10
#define NSIGSEGV_linux     10

#define BUS_ADRALN_linux    1
#define BUS_ADRERR_linux    2
#define BUS_OBJERR_linux    3
#define BUS_MCEERR_AR_linux 4
#define BUS_MCEERR_AO_linux 5
#define NSIGBUS_linux       5

#define TRAP_BRKPT_linux  1
#define TRAP_TRACE_linux  2
#define TRAP_BRANCH_linux 3
#define TRAP_HWBKPT_linux 4
#define TRAP_UNK_linux    5
#define TRAP_PERF_linux   6
#define NSIGTRAP_linux    6

#define TRAP_PERF_FLAG_ASYNC_linux (1u << 0)

#define CLD_EXITED_linux    1
#define CLD_KILLED_linux    2
#define CLD_DUMPED_linux    3
#define CLD_TRAPPED_linux   4
#define CLD_STOPPED_linux   5
#define CLD_CONTINUED_linux 6
#define NSIGCHLD_linux      6

#define POLL_IN_linux  1
#define POLL_OUT_linux 2
#define POLL_MSG_linux 3
#define POLL_ERR_linux 4
#define POLL_PRI_linux 5
#define POLL_HUP_linux 6
#define NSIGPOLL_linux 6

#define SYS_SECCOMP_linux       1
#define SYS_USER_DISPATCH_linux 2
#define NSIGSYS_linux           2

#define EMT_TAGOVF_linux 1
#define NSIGEMT_linux    1

#define SIGEV_SIGNAL_linux    0
#define SIGEV_NONE_linux      1
#define SIGEV_THREAD_linux    2
#define SIGEV_THREAD_ID_linux 4

#define PR_SET_PDEATHSIG_linux      1
#define PR_GET_PDEATHSIG_linux      2
#define PR_GET_DUMPABLE_linux       3
#define PR_SET_DUMPABLE_linux       4
#define PR_GET_UNALIGN_linux        5
#define PR_SET_UNALIGN_linux        6

#define PR_UNALIGN_NOPRINT_linux    1
#define PR_UNALIGN_SIGBUS_linux     2

#define PR_GET_KEEPCAPS_linux       7
#define PR_SET_KEEPCAPS_linux       8
#define PR_GET_FPEMU_linux          9
#define PR_SET_FPEMU_linux         10

#define PR_FPEMU_NOPRINT_linux      1
#define PR_FPEMU_SIGFPE_linux       2

#define PR_GET_FPEXC_linux         11
#define PR_SET_FPEXC_linux         12

#define PR_FP_EXC_SW_ENABLE_linux    0x80
#define PR_FP_EXC_DIV_linux          0x010000
#define PR_FP_EXC_OVF_linux          0x020000
#define PR_FP_EXC_UND_linux          0x040000
#define PR_FP_EXC_RES_linux          0x080000
#define PR_FP_EXC_INV_linux          0x100000
#define PR_FP_EXC_DISABLED_linux     0
#define PR_FP_EXC_NONRECOV_linux     1
#define PR_FP_EXC_ASYNC_linux        2
#define PR_FP_EXC_PRECISE_linux      3

#define PR_GET_TIMING_linux       13
#define PR_SET_TIMING_linux       14

#define PR_TIMING_STATISTICAL_linux    0
#define PR_TIMING_TIMESTAMP_linux      1

#define PR_SET_NAME_linux       15
#define PR_GET_NAME_linux       16
#define PR_GET_ENDIAN_linux     19
#define PR_SET_ENDIAN_linux     20

#define PR_ENDIAN_BIG_linux          0
#define PR_ENDIAN_LITTLE_linux       1
#define PR_ENDIAN_PPC_LITTLE_linux   2

#define PR_GET_SECCOMP_linux                  21
#define PR_SET_SECCOMP_linux                  22
#define PR_CAPBSET_READ_linux                 23
#define PR_CAPBSET_DROP_linux                 24
#define PR_GET_TSC_linux                      25
#define PR_SET_TSC_linux                      26

#define PR_TSC_ENABLE_linux                   1
#define PR_TSC_SIGSEGV_linux                  2

#define PR_GET_SECUREBITS_linux               27
#define PR_SET_SECUREBITS_linux               28
#define PR_SET_TIMERSLACK_linux               29
#define PR_GET_TIMERSLACK_linux               30
#define PR_TASK_PERF_EVENTS_DISABLE_linux     31
#define PR_TASK_PERF_EVENTS_ENABLE_linux      32
#define PR_MCE_KILL_linux                     33

#define PR_MCE_KILL_CLEAR_linux               0
#define PR_MCE_KILL_SET_linux                 1
#define PR_MCE_KILL_LATE_linux                0
#define PR_MCE_KILL_EARLY_linux               1
#define PR_MCE_KILL_DEFAULT_linux             2

#define PR_MCE_KILL_GET_linux                 34
#define PR_SET_MM_linux                       35

#define PR_SET_MM_START_CODE_linux            1
#define PR_SET_MM_END_CODE_linux              2
#define PR_SET_MM_START_DATA_linux            3
#define PR_SET_MM_END_DATA_linux              4
#define PR_SET_MM_START_STACK_linux           5
#define PR_SET_MM_START_BRK_linux             6
#define PR_SET_MM_BRK_linux                   7
#define PR_SET_MM_ARG_START_linux             8
#define PR_SET_MM_ARG_END_linux               9
#define PR_SET_MM_ENV_START_linux             10
#define PR_SET_MM_ENV_END_linux               11
#define PR_SET_MM_AUXV_linux                  12
#define PR_SET_MM_EXE_FILE_linux              13
#define PR_SET_MM_MAP_linux                   14
#define PR_SET_MM_MAP_SIZE_linux              15

#define PR_SET_PTRACER_linux                  0x59616d61
#define PR_SET_PTRACER_ANY_linux              ((unsigned long)-1)
#define PR_SET_CHILD_SUBREAPER_linux          36
#define PR_GET_CHILD_SUBREAPER_linux          37
#define PR_SET_NO_NEW_PRIVS_linux             38
#define PR_GET_NO_NEW_PRIVS_linux             39
#define PR_GET_TID_ADDRESS_linux              40
#define PR_SET_THP_DISABLE_linux              41

#define PR_THP_DISABLE_EXCEPT_ADVISED_linux   (1 << 1)

#define PR_GET_THP_DISABLE_linux              42
#define PR_MPX_ENABLE_MANAGEMENT_linux        43
#define PR_MPX_DISABLE_MANAGEMENT_linux       44
#define PR_SET_FP_MODE_linux                  45
#define PR_GET_FP_MODE_linux                  46

#define PR_FP_MODE_FR_linux                   (1 << 0)
#define PR_FP_MODE_FRE_linux                  (1 << 1)

#define PR_CAP_AMBIENT_linux                  47
#define PR_CAP_AMBIENT_IS_SET_linux           1
#define PR_CAP_AMBIENT_RAISE_linux            2
#define PR_CAP_AMBIENT_LOWER_linux            3
#define PR_CAP_AMBIENT_CLEAR_ALL_linux        4

#ifdef __aarch64__
#define PR_SVE_SET_VL_linux                   50
#define PR_SVE_SET_VL_ONEXEC_linux            (1 << 18)
#define PR_SVE_GET_VL_linux                   51
#define PR_SVE_VL_LEN_MASK_linux              0xffff
#define PR_SVE_VL_INHERIT_linux               (1 << 17)
#endif

#define PR_GET_SPECULATION_CTRL_linux         52
#define PR_SET_SPECULATION_CTRL_linux         53

#define PR_SPEC_STORE_BYPASS_linux            0
#define PR_SPEC_INDIRECT_BRANCH_linux         1
#define PR_SPEC_L1D_FLUSH_linux               2
#define PR_SPEC_NOT_AFFECTED_linux            0
#define PR_SPEC_PRCTL_linux                   (1UL << 0)
#define PR_SPEC_ENABLE_linux                  (1UL << 1)
#define PR_SPEC_DISABLE_linux                 (1UL << 2)
#define PR_SPEC_FORCE_DISABLE_linux           (1UL << 3)
#define PR_SPEC_DISABLE_NOEXEC_linux          (1UL << 4)

#ifdef __aarch64__
#define PR_PAC_RESET_KEYS_linux               54

#define PR_PAC_APIAKEY_linux                  (1UL << 0)
#define PR_PAC_APIBKEY_linux                  (1UL << 1)
#define PR_PAC_APDAKEY_linux                  (1UL << 2)
#define PR_PAC_APDBKEY_linux                  (1UL << 3)
#define PR_PAC_APGAKEY_linux                  (1UL << 4)
#endif

#if defined(__aarch64__) || defined(__riscv)
#define PR_SET_TAGGED_ADDR_CTRL_linux         55
#define PR_GET_TAGGED_ADDR_CTRL_linux         56

#define PR_TAGGED_ADDR_ENABLE_linux           (1UL << 0)
#endif

#ifdef __aarch64__
#define PR_MTE_TCF_NONE_linux                 0UL
#define PR_MTE_TCF_SYNC_linux                 (1UL << 1)
#define PR_MTE_TCF_ASYNC_linux                (1UL << 2)
#define PR_MTE_TCF_MASK_linux                 (PR_MTE_TCF_SYNC_linux | PR_MTE_TCF_ASYNC_linux)
#define PR_MTE_TAG_SHIFT_linux                3
#define PR_MTE_TAG_MASK_linux                 (0xffffUL << PR_MTE_TAG_SHIFT_linux)
#define PR_MTE_STORE_ONLY_linux               (1UL << 19)
#endif

#ifdef __riscv
#define PR_PMLEN_SHIFT_linux                  24
#define PR_PMLEN_MASK_linux                   (0x7fUL << PR_PMLEN_SHIFT_linux)
#endif

#define PR_SET_IO_FLUSHER_linux               57
#define PR_GET_IO_FLUSHER_linux               58
#define PR_SET_SYSCALL_USER_DISPATCH_linux    59

#define PR_SYS_DISPATCH_OFF_linux             0
#define PR_SYS_DISPATCH_EXCLUSIVE_ON_linux    1
#define PR_SYS_DISPATCH_INCLUSIVE_ON_linux    2
#define SYSCALL_DISPATCH_FILTER_ALLOW_linux   0
#define SYSCALL_DISPATCH_FILTER_BLOCK_linux   1

#ifdef __aarch64__
#define PR_PAC_SET_ENABLED_KEYS_linux         60
#define PR_PAC_GET_ENABLED_KEYS_linux         61
#endif

#define PR_SCHED_CORE_linux                   62

#define PR_SCHED_CORE_GET_linux               0
#define PR_SCHED_CORE_CREATE_linux            1
#define PR_SCHED_CORE_SHARE_TO_linux          2
#define PR_SCHED_CORE_SHARE_FROM_linux        3
#define PR_SCHED_CORE_MAX_linux               4
#define PR_SCHED_CORE_SCOPE_THREAD_linux      0

#define PR_SCHED_CORE_SCOPE_THREAD_GROUP_linux 1
#define PR_SCHED_CORE_SCOPE_PROCESS_GROUP_linux 2

#ifdef __aarch64__
#define PR_SME_SET_VL_linux                   63
#define PR_SME_SET_VL_ONEXEC_linux            (1 << 18)
#define PR_SME_GET_VL_linux                   64
#define PR_SME_VL_LEN_MASK_linux              0xffff
#define PR_SME_VL_INHERIT_linux               (1 << 17)
#endif

#define PR_SET_MDWE_linux                     65

#define PR_MDWE_REFUSE_EXEC_GAIN_linux        (1UL << 0)
#define PR_MDWE_NO_INHERIT_linux              (1UL << 1)

#define PR_GET_MDWE_linux                     66
#define PR_SET_VMA_linux                      0x53564d41

#define PR_SET_VMA_ANON_NAME_linux            0

#define PR_GET_AUXV_linux                     0x41555856
#define PR_SET_MEMORY_MERGE_linux             67
#define PR_GET_MEMORY_MERGE_linux             68

#ifdef __riscv
#define PR_RISCV_V_SET_CONTROL_linux          69
#define PR_RISCV_V_GET_CONTROL_linux          70

#define PR_RISCV_V_VSTATE_CTRL_DEFAULT_linux  0
#define PR_RISCV_V_VSTATE_CTRL_OFF_linux      1
#define PR_RISCV_V_VSTATE_CTRL_ON_linux       2
#define PR_RISCV_V_VSTATE_CTRL_INHERIT_linux  (1 << 4)
#define PR_RISCV_V_VSTATE_CTRL_CUR_MASK_linux 0x3
#define PR_RISCV_V_VSTATE_CTRL_NEXT_MASK_linux 0xc
#define PR_RISCV_V_VSTATE_CTRL_MASK_linux     0x1f

#define PR_RISCV_SET_ICACHE_FLUSH_CTX_linux   71

#define PR_RISCV_CTX_SW_FENCEI_ON_linux       0
#define PR_RISCV_CTX_SW_FENCEI_OFF_linux      1
#define PR_RISCV_SCOPE_PER_PROCESS_linux      0
#define PR_RISCV_SCOPE_PER_THREAD_linux       1
#endif

#define PR_GET_SHADOW_STACK_STATUS_linux      74
#define PR_SET_SHADOW_STACK_STATUS_linux      75

#define PR_SHADOW_STACK_ENABLE_linux          (1UL << 0)
#define PR_SHADOW_STACK_WRITE_linux           (1UL << 1)
#define PR_SHADOW_STACK_PUSH_linux            (1UL << 2)

#define PR_LOCK_SHADOW_STACK_STATUS_linux     76
#define PR_TIMER_CREATE_RESTORE_IDS_linux     77

#define PR_TIMER_CREATE_RESTORE_IDS_OFF_linux 0
#define PR_TIMER_CREATE_RESTORE_IDS_ON_linux  1
#define PR_TIMER_CREATE_RESTORE_IDS_GET_linux 2

#define PR_FUTEX_HASH_linux                   78

#define PR_FUTEX_HASH_SET_SLOTS_linux         1
#define PR_FUTEX_HASH_GET_SLOTS_linux         2

#define UNAME26_linux               0x0020000
#define ADDR_NO_RANDOMIZE_linux     0x0040000
#define FDPIC_FUNCPTRS_linux        0x0080000
#define MMAP_PAGE_ZERO_linux        0x0100000
#define ADDR_COMPAT_LAYOUT_linux    0x0200000
#define READ_IMPLIES_EXEC_linux     0x0400000
#define ADDR_LIMIT_32BIT_linux      0x0800000
#define SHORT_INODE_linux           0x1000000
#define WHOLE_SECONDS_linux         0x2000000
#define STICKY_TIMEOUTS_linux       0x4000000
#define ADDR_LIMIT_3GB_linux        0x8000000

#define PER_LINUX_linux         0x0000
#define PER_LINUX_32BIT_linux   (0x0000 | ADDR_LIMIT_32BIT_linux)
#define PER_LINUX_FDPIC_linux   (0x0000 | FDPIC_FUNCPTRS_linux)
#define PER_SVR4_linux          (0x0001 | STICKY_TIMEOUTS_linux | MMAP_PAGE_ZERO_linux)
#define PER_SVR3_linux          (0x0002 | STICKY_TIMEOUTS_linux | SHORT_INODE_linux)
#define PER_SCOSVR3_linux       (0x0003 | STICKY_TIMEOUTS_linux | WHOLE_SECONDS_linux | SHORT_INODE_linux)
#define PER_OSR5_linux          (0x0003 | STICKY_TIMEOUTS_linux | WHOLE_SECONDS_linux)
#define PER_WYSEV386_linux      (0x0004 | STICKY_TIMEOUTS_linux | SHORT_INODE_linux)
#define PER_ISCR4_linux         (0x0005 | STICKY_TIMEOUTS_linux)
#define PER_BSD_linux           0x0006
#define PER_SUNOS_linux         (0x0006 | STICKY_TIMEOUTS_linux)
#define PER_XENIX_linux         (0x0007 | STICKY_TIMEOUTS_linux | SHORT_INODE_linux)
#define PER_LINUX32_linux       0x0008
#define PER_LINUX32_3GB_linux   (0x0008 | ADDR_LIMIT_3GB_linux)
#define PER_IRIX32_linux        (0x0009 | STICKY_TIMEOUTS_linux)
#define PER_IRIXN32_linux       (0x000a | STICKY_TIMEOUTS_linux)
#define PER_IRIX64_linux        (0x000b | STICKY_TIMEOUTS_linux)
#define PER_RISCOS_linux        0x000c
#define PER_SOLARIS_linux       (0x000d | STICKY_TIMEOUTS_linux)
#define PER_UW7_linux           (0x000e | STICKY_TIMEOUTS_linux | MMAP_PAGE_ZERO_linux)
#define PER_OSF4_linux          0x000f
#define PER_HPUX_linux          0x0010
#define PER_MASK_linux          0x00ff

#if defined(__x86_64__) || defined(__i386__)
#define ARCH_SET_GS_linux                     0x1001
#define ARCH_SET_FS_linux                     0x1002
#define ARCH_GET_FS_linux                     0x1003
#define ARCH_GET_GS_linux                     0x1004
#define ARCH_GET_CPUID_linux                  0x1011
#define ARCH_SET_CPUID_linux                  0x1012
#define ARCH_GET_XCOMP_SUPP_linux             0x1021
#define ARCH_GET_XCOMP_PERM_linux             0x1022
#define ARCH_REQ_XCOMP_PERM_linux             0x1023
#define ARCH_GET_XCOMP_GUEST_PERM_linux       0x1024
#define ARCH_REQ_XCOMP_GUEST_PERM_linux       0x1025
#define ARCH_XCOMP_TILECFG_linux              17
#define ARCH_XCOMP_TILEDATA_linux             18
#define ARCH_MAP_VDSO_X32_linux               0x2001
#define ARCH_MAP_VDSO_32_linux                0x2002
#define ARCH_MAP_VDSO_64_linux                0x2003
#define ARCH_GET_UNTAG_MASK_linux             0x4001
#define ARCH_ENABLE_TAGGED_ADDR_linux         0x4002
#define ARCH_GET_MAX_TAG_BITS_linux           0x4003
#define ARCH_FORCE_TAGGED_SVA_linux           0x4004
#define ARCH_SHSTK_ENABLE_linux               0x5001
#define ARCH_SHSTK_DISABLE_linux              0x5002
#define ARCH_SHSTK_LOCK_linux                 0x5003
#define ARCH_SHSTK_UNLOCK_linux               0x5004
#define ARCH_SHSTK_STATUS_linux               0x5005
#define ARCH_SHSTK_SHSTK_linux                (1ULL << 0)
#define ARCH_SHSTK_WRSS_linux                 (1ULL << 1)

#define LDT_ENTRIES_linux      8192
#define LDT_ENTRY_SIZE_linux   8

#define MODIFY_LDT_CONTENTS_DATA_linux     0
#define MODIFY_LDT_CONTENTS_STACK_linux    1
#define MODIFY_LDT_CONTENTS_CODE_linux     2
#endif

#define PROT_NONE_linux           0x0
#define PROT_READ_linux           0x1
#define PROT_WRITE_linux          0x2
#define PROT_EXEC_linux           0x4
#define PROT_SEM_linux            0x8
#ifdef __aarch64__
#define PROT_BTI_linux            0x10
#define PROT_MTE_linux            0x20
#endif
#define PROT_GROWSDOWN_linux      0x01000000
#define PROT_GROWSUP_linux        0x02000000

#define MAP_SHARED_linux          0x01
#define MAP_PRIVATE_linux         0x02
#define MAP_SHARED_VALIDATE_linux 0x03
#define MAP_TYPE_linux            0x0f
#define MAP_FIXED_linux           0x10
#define MAP_ANONYMOUS_linux       0x20
#ifdef __x86_64__
#define MAP_32BIT_linux           0x40
#endif
#define MAP_GROWSDOWN_linux       0x0100
#define MAP_DENYWRITE_linux       0x0800
#define MAP_EXECUTABLE_linux      0x1000
#define MAP_LOCKED_linux          0x2000
#define MAP_NORESERVE_linux       0x4000
#define MAP_POPULATE_linux        0x008000
#define MAP_NONBLOCK_linux        0x010000
#define MAP_STACK_linux           0x020000
#define MAP_HUGETLB_linux         0x040000
#define MAP_SYNC_linux            0x080000
#define MAP_FIXED_NOREPLACE_linux 0x100000
#define MAP_UNINITIALIZED_linux   0x4000000

#define MAP_HUGE_SHIFT_linux      26
#define MAP_HUGE_MASK_linux       0x3f
#define MAP_HUGE_16KB_linux       (14U << MAP_HUGE_SHIFT_linux)
#define MAP_HUGE_64KB_linux       (16U << MAP_HUGE_SHIFT_linux)
#define MAP_HUGE_512KB_linux      (19U << MAP_HUGE_SHIFT_linux)
#define MAP_HUGE_1MB_linux        (20U << MAP_HUGE_SHIFT_linux)
#define MAP_HUGE_2MB_linux        (21U << MAP_HUGE_SHIFT_linux)
#define MAP_HUGE_8MB_linux        (23U << MAP_HUGE_SHIFT_linux)
#define MAP_HUGE_16MB_linux       (24U << MAP_HUGE_SHIFT_linux)
#define MAP_HUGE_32MB_linux       (25U << MAP_HUGE_SHIFT_linux)
#define MAP_HUGE_256MB_linux      (28U << MAP_HUGE_SHIFT_linux)
#define MAP_HUGE_512MB_linux      (29U << MAP_HUGE_SHIFT_linux)
#define MAP_HUGE_1GB_linux        (30U << MAP_HUGE_SHIFT_linux)
#define MAP_HUGE_2GB_linux        (31U << MAP_HUGE_SHIFT_linux)
#define MAP_HUGE_16GB_linux       (34U << MAP_HUGE_SHIFT_linux)

#define MREMAP_MAYMOVE_linux      1
#define MREMAP_FIXED_linux        2
#define MREMAP_DONTUNMAP_linux    4

#define MADV_NORMAL_linux          0
#define MADV_RANDOM_linux          1
#define MADV_SEQUENTIAL_linux      2
#define MADV_WILLNEED_linux        3
#define MADV_DONTNEED_linux        4
#define MADV_FREE_linux            8
#define MADV_REMOVE_linux          9
#define MADV_DONTFORK_linux        10
#define MADV_DOFORK_linux          11
#define MADV_MERGEABLE_linux       12
#define MADV_UNMERGEABLE_linux     13
#define MADV_HUGEPAGE_linux        14
#define MADV_NOHUGEPAGE_linux      15
#define MADV_DONTDUMP_linux        16
#define MADV_DODUMP_linux          17
#define MADV_WIPEONFORK_linux      18
#define MADV_KEEPONFORK_linux      19
#define MADV_COLD_linux            20
#define MADV_PAGEOUT_linux         21
#define MADV_POPULATE_READ_linux   22
#define MADV_POPULATE_WRITE_linux  23
#define MADV_DONTNEED_LOCKED_linux 24
#define MADV_COLLAPSE_linux        25
#define MADV_HWPOISON_linux        100
#define MADV_SOFT_OFFLINE_linux    101
#define MADV_GUARD_INSTALL_linux   102
#define MADV_GUARD_REMOVE_linux    103

#define MLOCK_ONFAULT_linux       0x01

#define MCL_CURRENT_linux         1
#define MCL_FUTURE_linux          2
#define MCL_ONFAULT_linux         4

#define MS_ASYNC_linux            1
#define MS_INVALIDATE_linux       2
#define MS_SYNC_linux             4

#define PKEY_UNRESTRICTED_linux    0x0
#define PKEY_DISABLE_ACCESS_linux  0x1
#define PKEY_DISABLE_WRITE_linux   0x2
#ifdef __aarch64__
#define PKEY_DISABLE_EXECUTE_linux 0x4
#define PKEY_DISABLE_READ_linux    0x8
#define PKEY_ACCESS_MASK_linux     (PKEY_DISABLE_ACCESS_linux | PKEY_DISABLE_WRITE_linux | PKEY_DISABLE_READ_linux | PKEY_DISABLE_EXECUTE_linux)
#else
#define PKEY_ACCESS_MASK_linux     (PKEY_DISABLE_ACCESS_linux | PKEY_DISABLE_WRITE_linux)
#endif

#define MPOL_DEFAULT_linux             0
#define MPOL_PREFERRED_linux           1
#define MPOL_BIND_linux                2
#define MPOL_INTERLEAVE_linux          3
#define MPOL_LOCAL_linux               4
#define MPOL_PREFERRED_MANY_linux      5
#define MPOL_WEIGHTED_INTERLEAVE_linux 6
#define MPOL_MAX_linux                 7

#define MPOL_F_STATIC_NODES_linux     (1 << 15)
#define MPOL_F_RELATIVE_NODES_linux   (1 << 14)
#define MPOL_F_NUMA_BALANCING_linux   (1 << 13)

#define MPOL_F_NODE_linux             (1 << 0)
#define MPOL_F_ADDR_linux             (1 << 1)
#define MPOL_F_MEMS_ALLOWED_linux     (1 << 2)

#define MPOL_MF_STRICT_linux          (1 << 0)
#define MPOL_MF_MOVE_linux            (1 << 1)
#define MPOL_MF_MOVE_ALL_linux        (1 << 2)
#define MPOL_MF_LAZY_linux            (1 << 3)

#define MFD_CLOEXEC_linux             0x0001U
#define MFD_ALLOW_SEALING_linux       0x0002U
#define MFD_HUGETLB_linux             0x0004U
#define MFD_NOEXEC_SEAL_linux         0x0008U
#define MFD_EXEC_linux                0x0010U

#define MFD_HUGE_SHIFT_linux          26
#define MFD_HUGE_MASK_linux           0x3f
#define MFD_HUGE_64KB_linux           (16U << MFD_HUGE_SHIFT_linux)
#define MFD_HUGE_512KB_linux          (19U << MFD_HUGE_SHIFT_linux)
#define MFD_HUGE_1MB_linux            (20U << MFD_HUGE_SHIFT_linux)
#define MFD_HUGE_2MB_linux            (21U << MFD_HUGE_SHIFT_linux)
#define MFD_HUGE_8MB_linux            (23U << MFD_HUGE_SHIFT_linux)
#define MFD_HUGE_16MB_linux           (24U << MFD_HUGE_SHIFT_linux)
#define MFD_HUGE_32MB_linux           (25U << MFD_HUGE_SHIFT_linux)
#define MFD_HUGE_256MB_linux          (28U << MFD_HUGE_SHIFT_linux)
#define MFD_HUGE_512MB_linux          (29U << MFD_HUGE_SHIFT_linux)
#define MFD_HUGE_1GB_linux            (30U << MFD_HUGE_SHIFT_linux)
#define MFD_HUGE_2GB_linux            (31U << MFD_HUGE_SHIFT_linux)
#define MFD_HUGE_16GB_linux           (34U << MFD_HUGE_SHIFT_linux)

#define SHADOW_STACK_SET_TOKEN_linux  (1ULL << 0)
#define SHADOW_STACK_SET_MARKER_linux (1ULL << 1)

#define UFFD_USER_MODE_ONLY_linux     1

#define MEMBARRIER_CMD_QUERY_linux                                0
#define MEMBARRIER_CMD_GLOBAL_linux                               (1 << 0)
#define MEMBARRIER_CMD_GLOBAL_EXPEDITED_linux                     (1 << 1)
#define MEMBARRIER_CMD_REGISTER_GLOBAL_EXPEDITED_linux            (1 << 2)
#define MEMBARRIER_CMD_PRIVATE_EXPEDITED_linux                    (1 << 3)
#define MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED_linux           (1 << 4)
#define MEMBARRIER_CMD_PRIVATE_EXPEDITED_SYNC_CORE_linux          (1 << 5)
#define MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED_SYNC_CORE_linux (1 << 6)
#define MEMBARRIER_CMD_PRIVATE_EXPEDITED_RSEQ_linux               (1 << 7)
#define MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED_RSEQ_linux      (1 << 8)
#define MEMBARRIER_CMD_GET_REGISTRATIONS_linux                    (1 << 9)

#define MEMBARRIER_CMD_FLAG_CPU_linux                             (1 << 0)

#define O_ACCMODE_linux             00000003
#define O_RDONLY_linux              00000000
#define O_WRONLY_linux              00000001
#define O_RDWR_linux                00000002
#define O_CREAT_linux               00000100
#define O_EXCL_linux                00000200
#define O_NOCTTY_linux              00000400
#define O_TRUNC_linux               00001000
#define O_APPEND_linux              00002000
#define O_NONBLOCK_linux            00004000
#define O_DSYNC_linux               00010000
#define O_DIRECT_linux              BY_ARCH_linux(00040000, 0200000, 00040000, 00040000, 0200000, 00040000)
#define O_LARGEFILE_linux           BY_ARCH_linux(00100000, 0400000, 00100000, 00100000, 0400000, 00100000)
#define O_DIRECTORY_linux           BY_ARCH_linux(00200000, 0040000, 00200000, 00200000, 0040000, 00200000)
#define O_NOFOLLOW_linux            BY_ARCH_linux(00400000, 0100000, 00400000, 00400000, 0100000, 00400000)
#define O_NOATIME_linux             01000000
#define O_CLOEXEC_linux             02000000
#define __O_SYNC_linux              04000000
#define O_SYNC_linux                (__O_SYNC_linux | O_DSYNC_linux)
#define O_PATH_linux                010000000
#define __O_TMPFILE_linux           020000000
#define O_TMPFILE_linux             (__O_TMPFILE_linux | O_DIRECTORY_linux)
#define AT_EACCESS_linux            0x200
#define RENAME_NOREPLACE_linux      (1 << 0)
#define RENAME_EXCHANGE_linux       (1 << 1)
#define RENAME_WHITEOUT_linux       (1 << 2)

#define RWF_HIPRI_linux             0x00000001
#define RWF_DSYNC_linux             0x00000002
#define RWF_SYNC_linux              0x00000004
#define RWF_NOWAIT_linux            0x00000008
#define RWF_APPEND_linux            0x00000010
#define RWF_NOAPPEND_linux          0x00000020
#define RWF_ATOMIC_linux            0x00000040
#define RWF_DONTCACHE_linux         0x00000080
#define RWF_NOSIGNAL_linux          0x00000100

#define SEEK_SET_linux              0
#define SEEK_CUR_linux              1
#define SEEK_END_linux              2
#define SEEK_DATA_linux             3
#define SEEK_HOLE_linux             4
#define SEEK_MAX_linux              SEEK_HOLE_linux

#define SPLICE_F_MOVE_linux         0x01
#define SPLICE_F_NONBLOCK_linux     0x02
#define SPLICE_F_MORE_linux         0x04
#define SPLICE_F_GIFT_linux         0x08

#define AT_FDCWD_linux              -100
#define AT_SYMLINK_NOFOLLOW_linux   0x100
#define AT_REMOVEDIR_linux          0x200
#define AT_SYMLINK_FOLLOW_linux     0x400
#define AT_NO_AUTOMOUNT_linux       0x800
#define AT_EMPTY_PATH_linux         0x1000

#define RESOLVE_NO_XDEV_linux       0x01
#define RESOLVE_NO_MAGICLINKS_linux 0x02
#define RESOLVE_NO_SYMLINKS_linux   0x04
#define RESOLVE_BENEATH_linux       0x08
#define RESOLVE_IN_ROOT_linux       0x10
#define RESOLVE_CACHED_linux        0x20

#define CLOSE_RANGE_UNSHARE_linux   (1U << 1)
#define CLOSE_RANGE_CLOEXEC_linux   (1U << 2)

#define POSIX_FADV_NORMAL_linux     0
#define POSIX_FADV_RANDOM_linux     1
#define POSIX_FADV_SEQUENTIAL_linux 2
#define POSIX_FADV_WILLNEED_linux   3
#define POSIX_FADV_DONTNEED_linux   4
#define POSIX_FADV_NOREUSE_linux    5

#define FALLOC_FL_KEEP_SIZE_linux      0x01
#define FALLOC_FL_PUNCH_HOLE_linux     0x02
#define FALLOC_FL_COLLAPSE_RANGE_linux 0x08
#define FALLOC_FL_ZERO_RANGE_linux     0x10
#define FALLOC_FL_INSERT_RANGE_linux   0x20
#define FALLOC_FL_UNSHARE_RANGE_linux  0x40

#define SYNC_FILE_RANGE_WAIT_BEFORE_linux    1
#define SYNC_FILE_RANGE_WRITE_linux          2
#define SYNC_FILE_RANGE_WAIT_AFTER_linux     4
#define SYNC_FILE_RANGE_WRITE_AND_WAIT_linux (SYNC_FILE_RANGE_WRITE_linux | SYNC_FILE_RANGE_WAIT_BEFORE_linux | SYNC_FILE_RANGE_WAIT_AFTER_linux)

#define F_DUPFD_linux               0
#define F_GETFD_linux               1
#define F_SETFD_linux               2
#define F_GETFL_linux               3
#define F_SETFL_linux               4
#define F_GETLK_linux               5
#define F_SETLK_linux               6
#define F_SETLKW_linux              7
#define F_SETOWN_linux              8
#define F_GETOWN_linux              9
#define F_SETSIG_linux              10
#define F_GETSIG_linux              11
#define F_GETLK64_linux             12
#define F_SETLK64_linux             13
#define F_SETLKW64_linux            14
#define F_SETOWN_EX_linux           15
#define F_GETOWN_EX_linux           16
#define F_GETOWNER_UIDS_linux       17
#define F_OFD_GETLK_linux           36
#define F_OFD_SETLK_linux           37
#define F_OFD_SETLKW_linux          38

#define F_OWNER_TID_linux           0
#define F_OWNER_PID_linux           1
#define F_OWNER_PGRP_linux          2
#define F_OWNER_GID_linux           2

#define FD_CLOEXEC_linux            1

#define F_RDLCK_linux               0
#define F_WRLCK_linux               1
#define F_UNLCK_linux               2
#define F_EXLCK_linux               4
#define F_SHLCK_linux               8

#define LOCK_SH_linux               1
#define LOCK_EX_linux               2
#define LOCK_NB_linux               4
#define LOCK_UN_linux               8
#define LOCK_MAND_linux             32
#define LOCK_READ_linux             64
#define LOCK_WRITE_linux            128
#define LOCK_RW_linux               192

#define F_LINUX_SPECIFIC_BASE_linux 1024
#define F_SETLEASE_linux            (F_LINUX_SPECIFIC_BASE_linux + 0)
#define F_GETLEASE_linux            (F_LINUX_SPECIFIC_BASE_linux + 1)
#define F_NOTIFY_linux              (F_LINUX_SPECIFIC_BASE_linux + 2)
#define F_DUPFD_QUERY_linux         (F_LINUX_SPECIFIC_BASE_linux + 3)
#define F_CREATED_QUERY_linux       (F_LINUX_SPECIFIC_BASE_linux + 4)
#define F_CANCELLK_linux            (F_LINUX_SPECIFIC_BASE_linux + 5)
#define F_DUPFD_CLOEXEC_linux       (F_LINUX_SPECIFIC_BASE_linux + 6)
#define F_SETPIPE_SZ_linux          (F_LINUX_SPECIFIC_BASE_linux + 7)
#define F_GETPIPE_SZ_linux          (F_LINUX_SPECIFIC_BASE_linux + 8)
#define F_ADD_SEALS_linux           (F_LINUX_SPECIFIC_BASE_linux + 9)
#define F_GET_SEALS_linux           (F_LINUX_SPECIFIC_BASE_linux + 10)
#define F_GET_RW_HINT_linux         (F_LINUX_SPECIFIC_BASE_linux + 11)
#define F_SET_RW_HINT_linux         (F_LINUX_SPECIFIC_BASE_linux + 12)
#define F_GET_FILE_RW_HINT_linux    (F_LINUX_SPECIFIC_BASE_linux + 13)
#define F_SET_FILE_RW_HINT_linux    (F_LINUX_SPECIFIC_BASE_linux + 14)
#define F_GETDELEG_linux            (F_LINUX_SPECIFIC_BASE_linux + 15)
#define F_SETDELEG_linux            (F_LINUX_SPECIFIC_BASE_linux + 16)

#define F_SEAL_SEAL_linux           0x0001
#define F_SEAL_SHRINK_linux         0x0002
#define F_SEAL_GROW_linux           0x0004
#define F_SEAL_WRITE_linux          0x0008
#define F_SEAL_FUTURE_WRITE_linux   0x0010
#define F_SEAL_EXEC_linux           0x0020

#define RWH_WRITE_LIFE_NOT_SET_linux 0
#define RWH_WRITE_LIFE_NONE_linux    1
#define RWH_WRITE_LIFE_SHORT_linux   2
#define RWH_WRITE_LIFE_MEDIUM_linux  3
#define RWH_WRITE_LIFE_LONG_linux    4
#define RWH_WRITE_LIFE_EXTREME_linux 5
#define RWF_WRITE_LIFE_NOT_SET_linux RWH_WRITE_LIFE_NOT_SET_linux

#define DN_ACCESS_linux             0x00000001
#define DN_MODIFY_linux             0x00000002
#define DN_CREATE_linux             0x00000004
#define DN_DELETE_linux             0x00000008
#define DN_RENAME_linux             0x00000010
#define DN_ATTRIB_linux             0x00000020
#define DN_MULTISHOT_linux          0x80000000

#define _IOC_NRBITS_linux       8
#define _IOC_TYPEBITS_linux     8
#define _IOC_SIZEBITS_linux     14
#define _IOC_DIRBITS_linux      2

#define _IOC_NRMASK_linux       ((1 << _IOC_NRBITS_linux)-1)
#define _IOC_TYPEMASK_linux     ((1 << _IOC_TYPEBITS_linux)-1)
#define _IOC_SIZEMASK_linux     ((1 << _IOC_SIZEBITS_linux)-1)
#define _IOC_DIRMASK_linux      ((1 << _IOC_DIRBITS_linux)-1)

#define _IOC_NRSHIFT_linux      0
#define _IOC_TYPESHIFT_linux    (_IOC_NRSHIFT_linux+_IOC_NRBITS_linux)
#define _IOC_SIZESHIFT_linux    (_IOC_TYPESHIFT_linux+_IOC_TYPEBITS_linux)
#define _IOC_DIRSHIFT_linux     (_IOC_SIZESHIFT_linux+_IOC_SIZEBITS_linux)

#define _IOC_NONE_linux         0U
#define _IOC_WRITE_linux        1U
#define _IOC_READ_linux         2U

#define _IOC_linux(dir,type,nr,size) \
        (((dir)  << _IOC_DIRSHIFT_linux) | \
         ((type) << _IOC_TYPESHIFT_linux) | \
         ((nr)   << _IOC_NRSHIFT_linux) | \
         ((size) << _IOC_SIZESHIFT_linux))

#define _IO_linux(type,nr)            _IOC_linux(_IOC_NONE_linux,(type),(nr),0)
#define _IOR_linux(type,nr,size)      _IOC_linux(_IOC_READ_linux,(type),(nr),(size))
#define _IOW_linux(type,nr,size)      _IOC_linux(_IOC_WRITE_linux,(type),(nr),(size))
#define _IOWR_linux(type,nr,size)     _IOC_linux(_IOC_READ_linux|_IOC_WRITE_linux,(type),(nr),(size))

#define TCGETS_linux            0x5401
#define TCSETS_linux            0x5402
#define TCSETSW_linux           0x5403
#define TCSETSF_linux           0x5404
#define TCGETA_linux            0x5405
#define TCSETA_linux            0x5406
#define TCSETAW_linux           0x5407
#define TCSETAF_linux           0x5408
#define TCSBRK_linux            0x5409
#define TCXONC_linux            0x540A
#define TCFLSH_linux            0x540B
#define TIOCEXCL_linux          0x540C
#define TIOCNXCL_linux          0x540D
#define TIOCSCTTY_linux         0x540E
#define TIOCGPGRP_linux         0x540F
#define TIOCSPGRP_linux         0x5410
#define TIOCOUTQ_linux          0x5411
#define TIOCSTI_linux           0x5412
#define TIOCGWINSZ_linux        0x5413
#define TIOCSWINSZ_linux        0x5414
#define TIOCMGET_linux          0x5415
#define TIOCMBIS_linux          0x5416
#define TIOCMBIC_linux          0x5417
#define TIOCMSET_linux          0x5418

#define FIONREAD_linux          0x541B
#define TIOCINQ_linux           FIONREAD_linux
#define TIOCLINUX_linux         0x541C
#define TIOCCONS_linux          0x541D
#define TIOCGSERIAL_linux       0x541E
#define TIOCSSERIAL_linux       0x541F
#define TIOCPKT_linux           0x5420
#define FIONBIO_linux           0x5421
#define TIOCNOTTY_linux         0x5422
#define TIOCSETD_linux          0x5423
#define TIOCGETD_linux          0x5424
#define TCSBRKP_linux           0x5425
#define TIOCSBRK_linux          0x5427
#define TIOCCBRK_linux          0x5428
#define TIOCGSID_linux          0x5429
#define TCGETS2_linux           _IOR_linux('T', 0x2A, sizeof(struct termios2_linux))
#define TCSETS2_linux           _IOW_linux('T', 0x2B, sizeof(struct termios2_linux))
#define TCSETSW2_linux          _IOW_linux('T', 0x2C, sizeof(struct termios2_linux))
#define TCSETSF2_linux          _IOW_linux('T', 0x2D, sizeof(struct termios2_linux))
#define TIOCGPTN_linux          _IOR_linux('T', 0x30, sizeof(unsigned int))
#define TIOCSPTLCK_linux        _IOW_linux('T', 0x31, sizeof(int))
#define TIOCGDEV_linux          _IOR_linux('T', 0x32, sizeof(unsigned int))
#define TIOCSIG_linux           _IOW_linux('T', 0x36, sizeof(int))
#define TIOCVHANGUP_linux       0x5437
#define TIOCGPKT_linux          _IOR_linux('T', 0x38, sizeof(int))
#define TIOCGPTLCK_linux        _IOR_linux('T', 0x39, sizeof(int))
#define TIOCGEXCL_linux         _IOR_linux('T', 0x40, sizeof(int))
#define TIOCGPTPEER_linux       _IO_linux('T', 0x41)

#define FIONCLEX_linux          0x5450
#define FIOCLEX_linux           0x5451
#define FIOASYNC_linux          0x5452

#ifdef __arm__
#define FIOQSIZE_linux          0x545E
#else
#define FIOQSIZE_linux          0x5460
#endif

#define FS_IOC_GETFLAGS_linux           _IOR_linux('f', 1, sizeof(long))
#define FS_IOC_SETFLAGS_linux           _IOW_linux('f', 2, sizeof(long))
#define FS_IOC_GETVERSION_linux         _IOR_linux('v', 1, sizeof(long))
#define FS_IOC_SETVERSION_linux         _IOW_linux('v', 2, sizeof(long))
#define FS_IOC_FSGETXATTR_linux         _IOR_linux('X', 31, sizeof(struct fsxattr_linux))
#define FS_IOC_FSSETXATTR_linux         _IOW_linux('X', 32, sizeof(struct fsxattr_linux))

#define FS_SECRM_FL_linux               0x00000001
#define FS_UNRM_FL_linux                0x00000002
#define FS_COMPR_FL_linux               0x00000004
#define FS_SYNC_FL_linux                0x00000008
#define FS_IMMUTABLE_FL_linux           0x00000010
#define FS_APPEND_FL_linux              0x00000020
#define FS_NODUMP_FL_linux              0x00000040
#define FS_NOATIME_FL_linux             0x00000080
#define FS_DIRTY_FL_linux               0x00000100
#define FS_COMPRBLK_FL_linux            0x00000200
#define FS_NOCOMP_FL_linux              0x00000400
#define FS_ENCRYPT_FL_linux             0x00000800
#define FS_BTREE_FL_linux               0x00001000
#define FS_INDEX_FL_linux               0x00001000
#define FS_IMAGIC_FL_linux              0x00002000
#define FS_JOURNAL_DATA_FL_linux        0x00004000
#define FS_NOTAIL_FL_linux              0x00008000
#define FS_DIRSYNC_FL_linux             0x00010000
#define FS_TOPDIR_FL_linux              0x00020000
#define FS_HUGE_FILE_FL_linux           0x00040000
#define FS_EXTENTS_FL_linux             0x00080000
#define FS_VERITY_FL_linux              0x00100000
#define FS_EA_INODE_FL_linux            0x00200000
#define FS_EOFBLOCKS_FL_linux           0x00400000
#define FS_NOCOW_FL_linux               0x00800000
#define FS_DAX_FL_linux                 0x02000000
#define FS_INLINE_DATA_FL_linux         0x10000000
#define FS_PROJINHERIT_FL_linux         0x20000000
#define FS_CASEFOLD_FL_linux            0x40000000
#define FS_RESERVED_FL_linux            0x80000000

#define VINTR_linux             0
#define VQUIT_linux             1
#define VERASE_linux            2
#define VKILL_linux             3
#define VEOF_linux              4
#define VTIME_linux             5
#define VMIN_linux              6
#define VSWTC_linux             7
#define VSTART_linux            8
#define VSTOP_linux             9
#define VSUSP_linux             10
#define VEOL_linux              11
#define VREPRINT_linux          12
#define VDISCARD_linux          13
#define VWERASE_linux           14
#define VLNEXT_linux            15
#define VEOL2_linux             16

#define IGNBRK_linux    0000001
#define BRKINT_linux    0000002
#define IGNPAR_linux    0000004
#define PARMRK_linux    0000010
#define INPCK_linux     0000020
#define ISTRIP_linux    0000040
#define INLCR_linux     0000100
#define IGNCR_linux     0000200
#define ICRNL_linux     0000400
#define IUCLC_linux     0001000
#define IXON_linux      0002000
#define IXANY_linux     0004000
#define IXOFF_linux     0010000
#define IMAXBEL_linux   0020000
#define IUTF8_linux     0040000

#define OPOST_linux     0000001
#define OLCUC_linux     0000002
#define ONLCR_linux     0000004
#define OCRNL_linux     0000010
#define ONOCR_linux     0000020
#define ONLRET_linux    0000040
#define OFILL_linux     0000100
#define OFDEL_linux     0000200

#define B0_linux        0000000
#define B50_linux       0000001
#define B75_linux       0000002
#define B110_linux      0000003
#define B134_linux      0000004
#define B150_linux      0000005
#define B200_linux      0000006
#define B300_linux      0000007
#define B600_linux      0000010
#define B1200_linux     0000011
#define B1800_linux     0000012
#define B2400_linux     0000013
#define B4800_linux     0000014
#define B9600_linux     0000015
#define B19200_linux    0000016
#define B38400_linux    0000017

#define B57600_linux    0010001
#define B115200_linux   0010002
#define B230400_linux   0010003
#define B460800_linux   0010004
#define B500000_linux   0010005
#define B576000_linux   0010006
#define B921600_linux   0010007
#define B1000000_linux  0010010
#define B1152000_linux  0010011
#define B1500000_linux  0010012
#define B2000000_linux  0010013
#define B2500000_linux  0010014
#define B3000000_linux  0010015
#define B3500000_linux  0010016
#define B4000000_linux  0010017

#define CSIZE_linux     0000060
#define CS5_linux       0000000
#define CS6_linux       0000020
#define CS7_linux       0000040
#define CS8_linux       0000060
#define CSTOPB_linux    0000100
#define CREAD_linux     0000200
#define PARENB_linux    0000400
#define PARODD_linux    0001000
#define HUPCL_linux     0002000
#define CLOCAL_linux    0004000

#define ISIG_linux      0000001
#define ICANON_linux    0000002
#define XCASE_linux     0000004
#define ECHO_linux      0000010
#define ECHOE_linux     0000020
#define ECHOK_linux     0000040
#define ECHONL_linux    0000100
#define NOFLSH_linux    0000200
#define TOSTOP_linux    0000400
#define ECHOCTL_linux   0001000
#define ECHOPRT_linux   0002000
#define ECHOKE_linux    0004000
#define FLUSHO_linux    0010000
#define PENDIN_linux    0040000
#define IEXTEN_linux    0100000

#define TCSANOW_linux   0
#define TCSADRAIN_linux 1
#define TCSAFLUSH_linux 2

#define POLLIN_linux            0x0001
#define POLLPRI_linux           0x0002
#define POLLOUT_linux           0x0004
#define POLLERR_linux           0x0008
#define POLLHUP_linux           0x0010
#define POLLNVAL_linux          0x0020
#define POLLRDNORM_linux        0x0040
#define POLLRDBAND_linux        0x0080
#define POLLWRNORM_linux        0x0100
#define POLLWRBAND_linux        0x0200
#define POLLMSG_linux           0x0400
#define POLLREMOVE_linux        0x1000
#define POLLRDHUP_linux         0x2000

#define EPOLL_CTL_ADD_linux 1
#define EPOLL_CTL_DEL_linux 2
#define EPOLL_CTL_MOD_linux 3
#define EPOLL_CLOEXEC_linux O_CLOEXEC_linux

#define EPOLLIN_linux       0x00000001
#define EPOLLPRI_linux      0x00000002
#define EPOLLOUT_linux      0x00000004
#define EPOLLERR_linux      0x00000008
#define EPOLLHUP_linux      0x00000010
#define EPOLLNVAL_linux     0x00000020
#define EPOLLRDNORM_linux   0x00000040
#define EPOLLRDBAND_linux   0x00000080
#define EPOLLWRNORM_linux   0x00000100
#define EPOLLWRBAND_linux   0x00000200
#define EPOLLMSG_linux      0x00000400
#define EPOLLRDHUP_linux    0x00002000
#define EPOLLEXCLUSIVE_linux (1U << 28)
#define EPOLLWAKEUP_linux    (1U << 29)
#define EPOLLONESHOT_linux   (1U << 30)
#define EPOLLET_linux        (1U << 31)

#define FICLONE_linux           _IOW_linux(0x94, 9, sizeof(int))
#define FICLONERANGE_linux      _IOW_linux(0x94, 13, sizeof(struct file_clone_range_linux))
#define FIDEDUPERANGE_linux     _IOWR_linux(0x94, 54, sizeof(struct file_dedupe_range_linux))

#define NS_GET_USERNS_linux           _IO_linux(0xb7, 0x1)
#define NS_GET_PARENT_linux           _IO_linux(0xb7, 0x2)
#define NS_GET_NSTYPE_linux           _IO_linux(0xb7, 0x3)
#define NS_GET_OWNER_UID_linux        _IO_linux(0xb7, 0x4)
#define NS_GET_PID_FROM_PIDNS_linux   _IOR_linux(0xb7, 0x6, sizeof(int))
#define NS_GET_TGID_FROM_PIDNS_linux  _IOR_linux(0xb7, 0x7, sizeof(int))
#define NS_GET_PID_IN_PIDNS_linux     _IOR_linux(0xb7, 0x8, sizeof(int))
#define NS_GET_TGID_IN_PIDNS_linux    _IOR_linux(0xb7, 0x9, sizeof(int))
#define NS_GET_MNTNS_ID_linux         _IOR_linux(0xb7, 5, sizeof(unsigned long long))
#define NS_GET_ID_linux               _IOR_linux(0xb7, 13, sizeof(unsigned long long))

#define UFFDIO_linux            0xAA
#define UFFDIO_API_linux        _IOWR_linux(UFFDIO_linux, 0x3F, sizeof(struct uffdio_api_linux))
#define UFFDIO_REGISTER_linux    _IOWR_linux(UFFDIO_linux, 0x00, sizeof(struct uffdio_register_linux))
#define UFFDIO_UNREGISTER_linux  _IOR_linux(UFFDIO_linux, 0x01, sizeof(struct uffdio_range_linux))
#define UFFDIO_WAKE_linux        _IOR_linux(UFFDIO_linux, 0x02, sizeof(struct uffdio_range_linux))
#define UFFDIO_COPY_linux        _IOWR_linux(UFFDIO_linux, 0x03, sizeof(struct uffdio_copy_linux))
#define UFFDIO_ZEROPAGE_linux    _IOWR_linux(UFFDIO_linux, 0x04, sizeof(struct uffdio_zeropage_linux))
#define UFFDIO_MOVE_linux        _IOWR_linux(UFFDIO_linux, 0x05, sizeof(struct uffdio_move_linux))
#define UFFDIO_WRITEPROTECT_linux _IOWR_linux(UFFDIO_linux, 0x06, sizeof(struct uffdio_writeprotect_linux))
#define UFFDIO_CONTINUE_linux    _IOWR_linux(UFFDIO_linux, 0x07, sizeof(struct uffdio_continue_linux))
#define UFFDIO_POISON_linux      _IOWR_linux(UFFDIO_linux, 0x08, sizeof(struct uffdio_poison_linux))

#define RNDGETENTCNT_linux      _IOR_linux('R', 0x00, sizeof(int))
#define RNDADDTOENTCNT_linux    _IOW_linux('R', 0x01, sizeof(int))
#define RNDGETPOOL_linux        _IOR_linux('R', 0x02, sizeof(int[2]))
#define RNDADDENTROPY_linux     _IOW_linux('R', 0x03, sizeof(int[2]))
#define RNDZAPENTCNT_linux      _IO_linux('R', 0x04)
#define RNDCLEARPOOL_linux      _IO_linux('R', 0x06)
#define RNDRESEEDCRNG_linux     _IO_linux('R', 0x07)

#define PIDFS_IOCTL_MAGIC_linux 0xFF
#define PIDFD_GET_CGROUP_NAMESPACE_linux            _IO_linux(PIDFS_IOCTL_MAGIC_linux, 1)
#define PIDFD_GET_IPC_NAMESPACE_linux               _IO_linux(PIDFS_IOCTL_MAGIC_linux, 2)
#define PIDFD_GET_MNT_NAMESPACE_linux               _IO_linux(PIDFS_IOCTL_MAGIC_linux, 3)
#define PIDFD_GET_NET_NAMESPACE_linux               _IO_linux(PIDFS_IOCTL_MAGIC_linux, 4)
#define PIDFD_GET_PID_NAMESPACE_linux               _IO_linux(PIDFS_IOCTL_MAGIC_linux, 5)
#define PIDFD_GET_PID_FOR_CHILDREN_NAMESPACE_linux  _IO_linux(PIDFS_IOCTL_MAGIC_linux, 6)
#define PIDFD_GET_TIME_NAMESPACE_linux              _IO_linux(PIDFS_IOCTL_MAGIC_linux, 7)
#define PIDFD_GET_TIME_FOR_CHILDREN_NAMESPACE_linux _IO_linux(PIDFS_IOCTL_MAGIC_linux, 8)
#define PIDFD_GET_USER_NAMESPACE_linux              _IO_linux(PIDFS_IOCTL_MAGIC_linux, 9)
#define PIDFD_GET_UTS_NAMESPACE_linux               _IO_linux(PIDFS_IOCTL_MAGIC_linux, 10)
#define PIDFD_GET_INFO_linux                        _IOWR_linux(PIDFS_IOCTL_MAGIC_linux, 11, sizeof(struct pidfd_info_linux))

#define SIOCADDRT_linux       0x890B
#define SIOCDELRT_linux       0x890C
#define SIOCGIFNAME_linux     0x8910
#define SIOCSIFLINK_linux     0x8911
#define SIOCGIFCONF_linux     0x8912
#define SIOCGIFFLAGS_linux    0x8913
#define SIOCSIFFLAGS_linux    0x8914
#define SIOCGIFADDR_linux     0x8915
#define SIOCSIFADDR_linux     0x8916
#define SIOCGIFDSTADDR_linux  0x8917
#define SIOCSIFDSTADDR_linux  0x8918
#define SIOCGIFBRDADDR_linux  0x8919
#define SIOCSIFBRDADDR_linux  0x891a
#define SIOCGIFNETMASK_linux  0x891b
#define SIOCSIFNETMASK_linux  0x891c
#define SIOCGIFMETRIC_linux   0x891d
#define SIOCSIFMETRIC_linux   0x891e
#define SIOCGIFMTU_linux      0x8921
#define SIOCSIFMTU_linux      0x8922
#define SIOCSIFNAME_linux     0x8923
#define SIOCSIFHWADDR_linux   0x8924
#define SIOCGIFHWADDR_linux   0x8927

#define LOOP_SET_FD_linux       0x4C00
#define LOOP_CLR_FD_linux       0x4C01
#define LOOP_SET_STATUS_linux    0x4C02
#define LOOP_GET_STATUS_linux    0x4C03
#define LOOP_SET_STATUS64_linux  0x4C04
#define LOOP_GET_STATUS64_linux  0x4C05
#define LOOP_CHANGE_FD_linux     0x4C06
#define LOOP_SET_CAPACITY_linux  0x4C07
#define LOOP_SET_DIRECT_IO_linux 0x4C08
#define LOOP_SET_BLOCK_SIZE_linux 0x4C09
#define LOOP_CONFIGURE_linux     0x4C0A
#define LOOP_CTL_ADD_linux       0x4C80
#define LOOP_CTL_REMOVE_linux    0x4C81
#define LOOP_CTL_GET_FREE_linux  0x4C82

#define TUNSETNOCSUM_linux  _IOW_linux('T', 200, sizeof(int))
#define TUNSETDEBUG_linux   _IOW_linux('T', 201, sizeof(int))
#define TUNSETIFF_linux     _IOW_linux('T', 202, sizeof(int))
#define TUNSETPERSIST_linux _IOW_linux('T', 203, sizeof(int))
#define TUNSETOWNER_linux   _IOW_linux('T', 204, sizeof(int))
#define TUNSETLINK_linux    _IOW_linux('T', 205, sizeof(int))
#define TUNSETGROUP_linux   _IOW_linux('T', 206, sizeof(int))
#define TUNGETFEATURES_linux _IOR_linux('T', 207, sizeof(unsigned int))
#define TUNGETIFF_linux      _IOR_linux('T', 210, sizeof(unsigned int))
#define TUNGETSNDBUF_linux   _IOR_linux('T', 211, sizeof(int))
#define TUNSETSNDBUF_linux   _IOW_linux('T', 212, sizeof(int))
#define TUNGETVNETHDRSZ_linux _IOR_linux('T', 215, sizeof(int))
#define TUNSETVNETHDRSZ_linux _IOW_linux('T', 216, sizeof(int))
#define TUNSETQUEUE_linux    _IOW_linux('T', 217, sizeof(int))
#define TUNSETIFINDEX_linux  _IOW_linux('T', 218, sizeof(unsigned int))
#define TUNSETCARRIER_linux  _IOW_linux('T', 226, sizeof(int))

#define EPOLL_IOC_TYPE_linux 0x8A
#define EPIOCSPARAMS_linux   _IOW_linux(EPOLL_IOC_TYPE_linux, 0x01, sizeof(struct epoll_params_linux))
#define EPIOCGPARAMS_linux   _IOR_linux(EPOLL_IOC_TYPE_linux, 0x02, sizeof(struct epoll_params_linux))

#define IFF_TUN_linux           0x0001
#define IFF_TAP_linux           0x0002
#define IFF_NAPI_linux          0x0010
#define IFF_NO_PI_linux         0x1000
#define IFF_ONE_QUEUE_linux     0x2000
#define IFF_VNET_HDR_linux      0x4000
#define IFF_TUN_EXCL_linux      0x8000
#define IFF_MULTI_QUEUE_linux   0x0100

#define STATX_TYPE_linux                0x00000001U
#define STATX_MODE_linux                0x00000002U
#define STATX_NLINK_linux               0x00000004U
#define STATX_UID_linux                 0x00000008U
#define STATX_GID_linux                 0x00000010U
#define STATX_ATIME_linux               0x00000020U
#define STATX_MTIME_linux               0x00000040U
#define STATX_CTIME_linux               0x00000080U
#define STATX_INO_linux                 0x00000100U
#define STATX_SIZE_linux                0x00000200U
#define STATX_BLOCKS_linux              0x00000400U
#define STATX_BASIC_STATS_linux         0x000007ffU
#define STATX_BTIME_linux               0x00000800U
#define STATX_MNT_ID_linux              0x00001000U
#define STATX_DIOALIGN_linux            0x00002000U
#define STATX_MNT_ID_UNIQUE_linux       0x00004000U
#define STATX_SUBVOL_linux              0x00008000U
#define STATX_WRITE_ATOMIC_linux        0x00010000U
#define STATX_DIO_READ_ALIGN_linux      0x00020000U

#define STATX_ATTR_COMPRESSED_linux     0x00000004
#define STATX_ATTR_IMMUTABLE_linux      0x00000010
#define STATX_ATTR_APPEND_linux         0x00000020
#define STATX_ATTR_NODUMP_linux         0x00000040
#define STATX_ATTR_ENCRYPTED_linux      0x00000800
#define STATX_ATTR_AUTOMOUNT_linux      0x00001000
#define STATX_ATTR_MOUNT_ROOT_linux     0x00002000
#define STATX_ATTR_VERITY_linux         0x00004000
#define STATX_ATTR_DAX_linux            0x00008000

#define FS_XFLAG_REALTIME_linux         0x00000001
#define FS_XFLAG_PREALLOC_linux         0x00000002
#define FS_XFLAG_IMMUTABLE_linux        0x00000008
#define FS_XFLAG_APPEND_linux           0x00000010
#define FS_XFLAG_SYNC_linux             0x00000020
#define FS_XFLAG_NOATIME_linux          0x00000040
#define FS_XFLAG_NODUMP_linux           0x00000080
#define FS_XFLAG_RTINHERIT_linux        0x00000100
#define FS_XFLAG_PROJINHERIT_linux      0x00000200
#define FS_XFLAG_NOSYMLINKS_linux       0x00000400
#define FS_XFLAG_EXTSIZE_linux          0x00000800
#define FS_XFLAG_EXTSZINHERIT_linux     0x00001000
#define FS_XFLAG_NODEFRAG_linux         0x00002000
#define FS_XFLAG_FILESTREAM_linux       0x00004000
#define FS_XFLAG_DAX_linux              0x00008000
#define FS_XFLAG_COWEXTSIZE_linux       0x00010000
#define FS_XFLAG_HASATTR_linux          0x80000000

#define S_IFMT_linux            00170000
#define S_IFSOCK_linux          0140000
#define S_IFLNK_linux           0120000
#define S_IFREG_linux           0100000
#define S_IFBLK_linux           0060000
#define S_IFDIR_linux           0040000
#define S_IFCHR_linux           0020000
#define S_IFIFO_linux           0010000
#define S_ISUID_linux           0004000
#define S_ISGID_linux           0002000
#define S_ISVTX_linux           0001000

#define S_IRWXU_linux           00700
#define S_IRUSR_linux           00400
#define S_IWUSR_linux           00200
#define S_IXUSR_linux           00100

#define S_IRWXG_linux           00070
#define S_IRGRP_linux           00040
#define S_IWGRP_linux           00020
#define S_IXGRP_linux           00010

#define S_IRWXO_linux           00007
#define S_IROTH_linux           00004
#define S_IWOTH_linux           00002
#define S_IXOTH_linux           00001

#define UTIME_NOW_linux         ((1l << 30) - 1l)
#define UTIME_OMIT_linux        ((1l << 30) - 2l)

#define R_OK_linux    4
#define W_OK_linux    2
#define X_OK_linux    1
#define F_OK_linux    0

#define AT_EACCESS_linux 0x200

#define XATTR_CREATE_linux      0x1
#define XATTR_REPLACE_linux     0x2

#define XATTR_OS2_PREFIX_linux          "os2."
#define XATTR_MAC_OSX_PREFIX_linux      "osx."
#define XATTR_BTRFS_PREFIX_linux        "btrfs."
#define XATTR_HURD_PREFIX_linux         "gnu."
#define XATTR_SECURITY_PREFIX_linux     "security."
#define XATTR_SYSTEM_PREFIX_linux       "system."
#define XATTR_TRUSTED_PREFIX_linux      "trusted."
#define XATTR_USER_PREFIX_linux         "user."

#define LOCK_SH_linux   1
#define LOCK_EX_linux   2
#define LOCK_NB_linux   4
#define LOCK_UN_linux   8

typedef struct {
  unsigned long long flags;
  unsigned long long pidfd;
  unsigned long long child_tid;
  unsigned long long parent_tid;
  unsigned long long exit_signal;
  unsigned long long stack;
  unsigned long long stack_size;
  unsigned long long tls;
  unsigned long long set_tid;
  unsigned long long set_tid_size;
  unsigned long long cgroup;
} clone_args_linux;

typedef struct {
  long tv_sec;
  long tv_usec;
} __kernel_old_timeval_linux;

typedef struct {
  __kernel_old_timeval_linux ru_utime;
  __kernel_old_timeval_linux ru_stime;
  long ru_maxrss;
  long ru_ixrss;
  long ru_idrss;
  long ru_isrss;
  long ru_minflt;
  long ru_majflt;
  long ru_nswap;
  long ru_inblock;
  long ru_oublock;
  long ru_msgsnd;
  long ru_msgrcv;
  long ru_nsignals;
  long ru_nvcsw;
  long ru_nivcsw;
} rusage_linux;

typedef struct {
    unsigned long sig[_NSIG_WORDS_linux];
} sigset_t_linux;

typedef union {
  int sival_int;
  void *sival_ptr;
} sigval_t_linux;

#define ADDR_BND_PKEY_PAD_linux  (__alignof__(void *) < sizeof(short) ?  sizeof(short) : __alignof__(void *))
#if defined(__x86_64__) || defined(__aarch64__) || (defined(__riscv) && (__riscv_xlen == 64))
    #define ARCH_SI_ATTRIBUTES_linux __attribute__((aligned(8)))
#else
    #define ARCH_SI_ATTRIBUTES_linux
#endif

typedef union {
  struct {
    int _pid;
    unsigned int _uid;
  } _kill;

  struct {
    int _tid;
    int _overrun;
    sigval_t_linux _sigval;
    int _sys_private;
  } _timer;

  struct {
    int _pid;
    unsigned int _uid;
    sigval_t_linux _sigval;
  } _rt;

  struct {
    int _pid;
    unsigned int _uid;
    int _status;
    long _utime;
    long _stime;
  } _sigchld;

  struct {
    void *_addr;

    union {
     int _trapno;
     short _addr_lsb;
     struct {
      char _dummy_bnd[ADDR_BND_PKEY_PAD_linux];
      void *_lower;
      void *_upper;
     } _addr_bnd;
     struct {
      char _dummy_pkey[ADDR_BND_PKEY_PAD_linux];
      unsigned int _pkey;
     } _addr_pkey;
     struct {
      unsigned long _data;
      unsigned int _type;
      unsigned int _flags;
     } _perf;
    };
  } _sigfault;

  struct {
    long _band;
    int _fd;
  } _sigpoll;

  struct {
    void *_call_addr;
    int _syscall;
    unsigned int _arch;
  } _sigsys;
} __sifields_linux;

typedef struct {
  union {
    struct {
     int si_signo;
     int si_errno;
     int si_code;
      __sifields_linux _sifields;
    };
    int _si_pad[128/sizeof(int)];
  };
} ARCH_SI_ATTRIBUTES_linux siginfo_t_linux;

#undef ADDR_BND_PKEY_PAD_linux
#undef ARCH_SI_ATTRIBUTES_linux

#define si_pid_linux         _sifields._kill._pid
#define si_uid_linux         _sifields._kill._uid
#define si_tid_linux         _sifields._timer._tid
#define si_overrun_linux     _sifields._timer._overrun
#define si_sys_private_linux _sifields._timer._sys_private
#define si_status_linux      _sifields._sigchld._status
#define si_utime_linux       _sifields._sigchld._utime
#define si_stime_linux       _sifields._sigchld._stime
#define si_value_linux       _sifields._rt._sigval
#define si_int_linux         _sifields._rt._sigval.sival_int
#define si_ptr_linux         _sifields._rt._sigval.sival_ptr
#define si_addr_linux        _sifields._sigfault._addr
#define si_trapno_linux      _sifields._sigfault._trapno
#define si_addr_lsb_linux    _sifields._sigfault._addr_lsb
#define si_lower_linux       _sifields._sigfault._addr_bnd._lower
#define si_upper_linux       _sifields._sigfault._addr_bnd._upper
#define si_pkey_linux        _sifields._sigfault._addr_pkey._pkey
#define si_perf_data_linux   _sifields._sigfault._perf._data
#define si_perf_type_linux   _sifields._sigfault._perf._type
#define si_perf_flags_linux  _sifields._sigfault._perf._flags
#define si_band_linux        _sifields._sigpoll._band
#define si_fd_linux          _sifields._sigpoll._fd
#define si_call_addr_linux   _sifields._sigsys._call_addr
#define si_syscall_linux     _sifields._sigsys._syscall
#define si_arch_linux        _sifields._sigsys._arch

typedef struct {
  sigval_t_linux sigev_value;
  int sigev_signo;
  int sigev_notify;
  union {
    int _pad[((64 - (sizeof(int) * 2 + sizeof(sigval_t_linux))) / sizeof(int))];
     int _tid;

    struct {
     void (*_function)(sigval_t_linux);
     void *_attribute;
    } _sigev_thread;
  } _sigev_un;
} sigevent_t_linux;

#define sigev_notify_function_linux   _sigev_un._sigev_thread._function
#define sigev_notify_attributes_linux _sigev_un._sigev_thread._attribute
#define sigev_notify_thread_id_linux  _sigev_un._tid

typedef struct {
  unsigned long long start_code;
  unsigned long long end_code;
  unsigned long long start_data;
  unsigned long long end_data;
  unsigned long long start_brk;
  unsigned long long brk;
  unsigned long long start_stack;
  unsigned long long arg_start;
  unsigned long long arg_end;
  unsigned long long env_start;
  unsigned long long env_end;
  unsigned long long *auxv;
  unsigned int auxv_size;
  unsigned int exe_fd;
} prctl_mm_map_linux;

#if defined(__x86_64__) || defined(__i386__)
typedef struct {
  unsigned int  entry_number;
  unsigned int  base_addr;
  unsigned int  limit;
  unsigned int  seg_32bit:1;
  unsigned int  contents:2;
  unsigned int  read_exec_only:1;
  unsigned int  limit_in_pages:1;
  unsigned int  seg_not_present:1;
  unsigned int  useable:1;
#ifdef __x86_64__
  unsigned int  lm:1;
#endif
} user_desc_linux;
#endif

typedef struct {
  int sched_priority;
} sched_param_linux;

typedef struct {
  unsigned int       size;
  unsigned int       sched_policy;
  unsigned long long sched_flags;
  int                sched_nice;
  unsigned int       sched_priority;
  unsigned long long sched_runtime;
  unsigned long long sched_deadline;
  unsigned long long sched_period;
  unsigned int       sched_util_min;
  unsigned int       sched_util_max;
} sched_attr_linux;

typedef struct {
  long long tv_sec;
  long long tv_nsec;
} __kernel_timespec_linux;

typedef struct {
  long tv_sec;
  long tv_nsec;
} __kernel_old_timespec_linux;

typedef struct {
  void *iov_base;
  unsigned long iov_len;
} iovec_linux;

typedef struct {
  unsigned char event;
  unsigned char reserved1;
  unsigned short reserved2;
  unsigned int reserved3;
  union {
    struct {
      unsigned long long flags;
      unsigned long long address;
      union {
        unsigned int ptid;
      } feat;
    } pagefault;
    struct {
      unsigned int ufd;
    } fork;
    struct {
      unsigned long long from;
      unsigned long long to;
      unsigned long long len;
    } remap;
    struct {
      unsigned long long start;
      unsigned long long end;
    } remove;
    unsigned long long reserved;
  } arg;
} __attribute__((packed)) uffd_msg_linux;

typedef struct {
  unsigned long long flags;
  unsigned long long mode;
  unsigned long long resolve;
} open_how_linux;

typedef struct {
  unsigned int handle_bytes;
  int handle_type;
  unsigned char f_handle[];
} file_handle_linux;

typedef struct {
  short l_type;
  short l_whence;
  long l_start;
  long l_len;
  int l_pid;
} flock_t_linux;

typedef struct {
  short l_type;
  short l_whence;
  long long l_start;
  long long l_len;
  int l_pid;
} flock64_linux;

typedef struct {
  int type;
  int pid;
} f_owner_ex_linux;

typedef struct {
  unsigned int d_flags;
  unsigned short d_type;
  unsigned short __pad;
} delegation_linux;

typedef struct {
  unsigned short ws_row;
  unsigned short ws_col;
  unsigned short ws_xpixel;
  unsigned short ws_ypixel;
} winsize_linux;

#define NCCS_linux 19
typedef struct {
  unsigned int   c_iflag;
  unsigned int   c_oflag;
  unsigned int   c_cflag;
  unsigned int   c_lflag;
  unsigned char  c_line;
  unsigned char  c_cc[NCCS_linux];
} termios_linux;

typedef struct {
  unsigned int   c_iflag;
  unsigned int   c_oflag;
  unsigned int   c_cflag;
  unsigned int   c_lflag;
  unsigned char  c_line;
  unsigned char  c_cc[NCCS_linux];
  unsigned int   c_ispeed;
  unsigned int   c_ospeed;
} termios2_linux;

typedef struct {
  unsigned int   fsx_xflags;
  unsigned int   fsx_extsize;
  unsigned int   fsx_nextents;
  unsigned int   fsx_projid;
  unsigned int   fsx_cowextsize;
  unsigned char  fsx_pad[8];
} fsxattr_linux;

typedef struct {
  unsigned long fds_bits[1024 / (8 * sizeof(long))];
} fd_set_linux;

typedef struct {
  unsigned long  n;
  fd_set_linux   *inp;
  fd_set_linux   *outp;
  fd_set_linux   *exp;
  __kernel_old_timeval_linux *tvp;
} sel_arg_struct_linux;

typedef struct {
  int   fd;
  short events;
  short revents;
} pollfd_linux;

typedef struct {
  sigset_t_linux *p;
  unsigned long  size;
} sigset_argpack_linux;

typedef struct {
  unsigned int   busy_poll_usecs;
  unsigned short busy_poll_budget;
  unsigned char  prefer_busy_poll;
  unsigned char  __pad;
} epoll_params_linux;

#ifdef __x86_64__
#define EPOLL_PACKED_linux __attribute__((packed))
#else
#define EPOLL_PACKED_linux
#endif

typedef struct {
  unsigned int       events;
  unsigned long long data;
} EPOLL_PACKED_linux epoll_event_linux;

typedef struct {
  unsigned long long src_offset;
  unsigned long long src_length;
  unsigned short dest_count;
  unsigned short reserved1;
  unsigned int reserved2;
  struct {
    int dest_fd;
    unsigned long long dest_offset;
    unsigned long long bytes_deduped;
    int status;
    unsigned int reserved;
  } info[];
} file_dedupe_range_linux;

typedef struct {
  unsigned long long src_offset;
  unsigned long long src_length;
  unsigned int dest_fd;
  unsigned int reserved;
  unsigned long long dest_offset;
} file_clone_range_linux;

typedef struct {
  unsigned long long fe_logical;
  unsigned long long fe_physical;
  unsigned long long fe_length;
  unsigned long long fe_reserved64[2];
  unsigned int fe_flags;
  unsigned int fe_reserved[3];
} fiemap_extent_linux;

typedef struct {
  unsigned long long fm_start;
  unsigned long long fm_length;
  unsigned int fm_flags;
  unsigned int fm_mapped_extents;
  unsigned int fm_extent_count;
  unsigned int fm_reserved;
  fiemap_extent_linux fm_extents[];
} fiemap_linux;

typedef struct {
  unsigned long long start;
  unsigned long long len;
} uffdio_range_linux;

typedef struct {
  unsigned long long api;
  unsigned long long features;
  unsigned long long ioctls;
} uffdio_api_linux;

typedef struct {
  uffdio_range_linux range;
  unsigned long long mode;
  unsigned long long ioctls;
} uffdio_register_linux;

typedef struct {
  unsigned long long dst;
  unsigned long long src;
  unsigned long long len;
  unsigned long long mode;
  long long copy;
} uffdio_copy_linux;

typedef struct {
  uffdio_range_linux range;
  unsigned long long mode;
  long long zeropage;
} uffdio_zeropage_linux;

typedef struct {
  uffdio_range_linux range;
  unsigned long long mode;
} uffdio_writeprotect_linux;

typedef struct {
  uffdio_range_linux range;
  unsigned long long mode;
  long long mapped;
} uffdio_continue_linux;

typedef struct {
  uffdio_range_linux range;
  unsigned long long mode;
  long long updated;
} uffdio_poison_linux;

typedef struct {
  unsigned long long dst;
  unsigned long long src;
  unsigned long long len;
  unsigned long long mode;
  long long move;
} uffdio_move_linux;

typedef struct {
  unsigned long long mask;
  unsigned long long cgroupid;
  unsigned int pid;
  unsigned int tgid;
  unsigned int ppid;
  unsigned int ruid;
  unsigned int rgid;
  unsigned int euid;
  unsigned int egid;
  unsigned int suid;
  unsigned int sgid;
  unsigned int fsuid;
  unsigned int fsgid;
  int exit_code;
  struct {
    unsigned int coredump_mask;
    unsigned int coredump_signal;
  };
  unsigned long long supported_mask;
} pidfd_info_linux;

typedef struct {
  unsigned short sa_family;
  char sa_data[14];
} sockaddr_linux;

typedef struct {
  unsigned short ss_family;
  char __ss_padding[128 - sizeof(unsigned short) - sizeof(unsigned long)];
  unsigned long __ss_align;
} sockaddr_storage_linux;

typedef struct {
  unsigned long mem_start;
  unsigned long mem_end;
  unsigned short base_addr;
  unsigned char irq;
  unsigned char dma;
  unsigned char port;
} ifmap_linux;

typedef struct {
  unsigned int type;
  unsigned int size;
  union {
    void *ifs_ifsu;
  };
} if_settings_linux;

typedef struct {
  union {
    char ifrn_name[16];
  } ifr_ifrn;
  union {
    sockaddr_linux ifru_addr;
    sockaddr_linux ifru_dstaddr;
    sockaddr_linux ifru_broadaddr;
    sockaddr_linux ifru_netmask;
    sockaddr_linux ifru_hwaddr;
    short ifru_flags;
    int ifru_ivalue;
    int ifru_mtu;
    ifmap_linux ifru_map;
    char ifru_slave[16];
    char ifru_newname[16];
    void *ifru_data;
    if_settings_linux ifru_settings;
  } ifr_ifru;
} ifreq_linux;

typedef struct {
  int ifc_len;
  union {
    char *ifcu_buf;
    ifreq_linux *ifcu_req;
  } ifc_ifcu;
} ifconf_linux;

typedef struct {
  unsigned long long lo_device;
  unsigned long long lo_inode;
  unsigned long long lo_rdevice;
  unsigned long long lo_offset;
  unsigned long long lo_sizelimit;
  unsigned int lo_number;
  unsigned int lo_encrypt_type;
  unsigned int lo_encrypt_key_size;
  unsigned int lo_flags;
  unsigned char lo_file_name[64];
  unsigned char lo_crypt_name[64];
  unsigned char lo_encrypt_key[32];
  unsigned long long lo_init[2];
} loop_info64_linux;

typedef struct {
  unsigned int fd;
  unsigned int block_size;
  loop_info64_linux info;
  unsigned long long __reserved[8];
} loop_config_linux;

typedef struct {
  unsigned short flags;
  unsigned short proto;
} tun_pi_linux;

typedef struct {
  long long tv_sec;
  unsigned int tv_nsec;
  int __reserved;
} statx_timestamp_linux;

typedef struct {
  unsigned int stx_mask;
  unsigned int stx_blksize;
  unsigned long long stx_attributes;
  unsigned int stx_nlink;
  unsigned int stx_uid;
  unsigned int stx_gid;
  unsigned short stx_mode;
  unsigned short __spare0[1];
  unsigned long long stx_ino;
  unsigned long long stx_size;
  unsigned long long stx_blocks;
  unsigned long long stx_attributes_mask;
  statx_timestamp_linux stx_atime;
  statx_timestamp_linux stx_btime;
  statx_timestamp_linux stx_ctime;
  statx_timestamp_linux stx_mtime;
  unsigned int stx_rdev_major;
  unsigned int stx_rdev_minor;
  unsigned int stx_dev_major;
  unsigned int stx_dev_minor;
  unsigned long long stx_mnt_id;
  unsigned int stx_dio_mem_align;
  unsigned int stx_dio_offset_align;
  unsigned long long stx_subvol;
  unsigned int stx_atomic_write_unit_min;
  unsigned int stx_atomic_write_unit_max;
  unsigned int stx_atomic_write_segments_max;
  unsigned int stx_dio_read_offset_align;
  unsigned int stx_atomic_write_unit_max_opt;
  unsigned int __spare2[1];
  unsigned long long __spare3[8];
} statx_t_linux;

typedef struct {
  unsigned long long fa_xflags;
  unsigned int fa_extsize;
  unsigned int fa_nextents;
  unsigned int fa_projid;
  unsigned int fa_cowextsize;
} file_attr_linux;

#if defined(__x86_64__)
typedef struct {
  unsigned long st_dev;
  unsigned long st_ino;
  unsigned long st_nlink;
  unsigned int st_mode;
  unsigned int st_uid;
  unsigned int st_gid;
  unsigned int __pad0;
  unsigned long st_rdev;
  long st_size;
  long st_blksize;
  long st_blocks;
  unsigned long st_atime;
  unsigned long st_atime_nsec;
  unsigned long st_mtime;
  unsigned long st_mtime_nsec;
  unsigned long st_ctime;
  unsigned long st_ctime_nsec;
  long __unused[3];
} stat_t_linux;

typedef struct {
  unsigned short st_dev;
  unsigned short st_ino;
  unsigned short st_mode;
  unsigned short st_nlink;
  unsigned short st_uid;
  unsigned short st_gid;
  unsigned short st_rdev;
  unsigned int   st_size;
  unsigned int   st_atime;
  unsigned int   st_mtime;
  unsigned int   st_ctime;
} __old_kernel_stat_linux;

typedef stat_t_linux stat64_t_linux;

#elif defined(__i386__)
typedef struct {
  unsigned long  st_dev;
  unsigned long  st_ino;
  unsigned short st_mode;
  unsigned short st_nlink;
  unsigned short st_uid;
  unsigned short st_gid;
  unsigned long  st_rdev;
  unsigned long  st_size;
  unsigned long  st_blksize;
  unsigned long  st_blocks;
  unsigned long  st_atime;
  unsigned long  st_atime_nsec;
  unsigned long  st_mtime;
  unsigned long  st_mtime_nsec;
  unsigned long  st_ctime;
  unsigned long  st_ctime_nsec;
  unsigned long  __unused4;
  unsigned long  __unused5;
} stat_t_linux;

typedef struct {
  unsigned long long st_dev;
  unsigned char      __pad0[4];
  unsigned long      __st_ino;
  unsigned int       st_mode;
  unsigned int       st_nlink;
  unsigned long      st_uid;
  unsigned long      st_gid;
  unsigned long long st_rdev;
  unsigned char      __pad3[4];
  long long          st_size;
  unsigned long      st_blksize;
  unsigned long long st_blocks;
  unsigned long      st_atime;
  unsigned long      st_atime_nsec;
  unsigned long      st_mtime;
  unsigned int       st_mtime_nsec;
  unsigned long      st_ctime;
  unsigned long      st_ctime_nsec;
  unsigned long long st_ino;
} stat64_t_linux;

typedef struct {
  unsigned short st_dev;
  unsigned short st_ino;
  unsigned short st_mode;
  unsigned short st_nlink;
  unsigned short st_uid;
  unsigned short st_gid;
  unsigned short st_rdev;
  unsigned long  st_size;
  unsigned long  st_atime;
  unsigned long  st_mtime;
  unsigned long  st_ctime;
} __old_kernel_stat_linux;

#elif defined(__arm__)
typedef struct {
  unsigned short st_dev;
  unsigned short st_ino;
  unsigned short st_mode;
  unsigned short st_nlink;
  unsigned short st_uid;
  unsigned short st_gid;
  unsigned short st_rdev;
  unsigned long  st_size;
  unsigned long  st_atime;
  unsigned long  st_mtime;
  unsigned long  st_ctime;
} __old_kernel_stat_linux;

typedef struct {
  unsigned long  st_dev;
  unsigned long  st_ino;
  unsigned short st_mode;
  unsigned short st_nlink;
  unsigned short st_uid;
  unsigned short st_gid;
  unsigned long  st_rdev;
  unsigned long  st_size;
  unsigned long  st_blksize;
  unsigned long  st_blocks;
  unsigned long  st_atime;
  unsigned long  st_atime_nsec;
  unsigned long  st_mtime;
  unsigned long  st_mtime_nsec;
  unsigned long  st_ctime;
  unsigned long  st_ctime_nsec;
  unsigned long  __unused4;
  unsigned long  __unused5;
} stat_t_linux;

typedef struct {
  unsigned long long st_dev;
  unsigned char      __pad0[4];
  unsigned long      __st_ino;
  unsigned int       st_mode;
  unsigned int       st_nlink;
  unsigned long      st_uid;
  unsigned long      st_gid;
  unsigned long long st_rdev;
  unsigned char      __pad3[4];
  long long          st_size;
  unsigned long      st_blksize;
  unsigned long long st_blocks;
  unsigned long      st_atime;
  unsigned long      st_atime_nsec;
  unsigned long      st_mtime;
  unsigned long      st_mtime_nsec;
  unsigned long      st_ctime;
  unsigned long      st_ctime_nsec;
  unsigned long long st_ino;
} stat64_t_linux;

#elif defined(__aarch64__) || (defined(__riscv) && (__riscv_xlen == 64))
typedef struct {
  unsigned long st_dev;
  unsigned long st_ino;
  unsigned int st_mode;
  unsigned int st_nlink;
  unsigned int st_uid;
  unsigned int st_gid;
  unsigned long st_rdev;
  unsigned long __pad1;
  long st_size;
  int st_blksize;
  int __pad2;
  long st_blocks;
  long st_atime;
  unsigned long st_atime_nsec;
  long st_mtime;
  unsigned long st_mtime_nsec;
  long st_ctime;
  unsigned long st_ctime_nsec;
  unsigned int __unused4;
  unsigned int __unused5;
} stat_t_linux;

typedef stat_t_linux stat64_t_linux;

#elif defined(__riscv) && (__riscv_xlen == 32)
typedef struct {
  unsigned long st_dev;
  unsigned long st_ino;
  unsigned int st_mode;
  unsigned int st_nlink;
  unsigned int st_uid;
  unsigned int st_gid;
  unsigned long st_rdev;
  unsigned long __pad1;
  long st_size;
  int st_blksize;
  int __pad2;
  long st_blocks;
  long st_atime;
  unsigned long st_atime_nsec;
  long st_mtime;
  unsigned long st_mtime_nsec;
  long st_ctime;
  unsigned long st_ctime_nsec;
  unsigned int __unused4;
  unsigned int __unused5;
} stat_t_linux;

typedef struct {
  unsigned long long st_dev;
  unsigned long long st_ino;
  unsigned int st_mode;
  unsigned int st_nlink;
  unsigned int st_uid;
  unsigned int st_gid;
  unsigned long long st_rdev;
  unsigned long long __pad1;
  long long st_size;
  int st_blksize;
  int __pad2;
  long long st_blocks;
  int st_atime;
  unsigned int st_atime_nsec;
  int st_mtime;
  unsigned int st_mtime_nsec;
  int st_ctime;
  unsigned int st_ctime_nsec;
  unsigned int __unused4;
  unsigned int __unused5;
} stat64_t_linux;

#endif

typedef struct {
  long actime;
  long modtime;
} utimbuf_linux;

typedef struct {
  void          *value;
  unsigned int  size;
  unsigned int  flags;
} xattr_args_linux;

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

//
// 1. PROCESS & THREAD LIFECYCLE
//
long fork_linux(void);
long vfork_linux(void);
long clone_linux(unsigned long clone_flags, unsigned long newsp, int *parent_tidptr, int *child_tidptr, unsigned long tls);
long clone3_linux(clone_args_linux *uargs, unsigned long size);
long execve_linux(const char *filename, const char *const *argv, const char *const *envp);
long execveat_linux(int dfd, const char *filename, const char *const *argv, const char *const *envp, int flags);
__attribute__((noreturn)) void exit_linux(int error_code);
__attribute__((noreturn)) void exit_group_linux(int error_code);
long wait4_linux(int pid, int *stat_addr, int options, rusage_linux *ru);
long waitid_linux(int which, int pid, siginfo_t_linux *infop, int options, rusage_linux *ru);
long waitpid_linux(int pid, int *stat_addr, int options);
//
// 2. PROCESS ATTRIBUTES & CONTROL
//
// 2a. Process identity, process groups and sessions
long getpid_linux(void);
long getppid_linux(void);
long gettid_linux(void);
long getpgid_linux(int pid);
long setpgid_linux(int pid, int pgid);
long getpgrp_linux(void);
long getsid_linux(int pid);
long setsid_linux(void);
long set_tid_address_linux(int *tidptr);
// 2b. Process control and personality
long prctl_linux(int option, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5);
long personality_linux(unsigned int personality);
//
// 3. SCHEDULING & PRIORITIES
//
long sched_setscheduler_linux(int pid, int policy, sched_param_linux *param);
long sched_getscheduler_linux(int pid);
long sched_setparam_linux(int pid, sched_param_linux *param);
long sched_getparam_linux(int pid, sched_param_linux *param);
long sched_setattr_linux(int pid, sched_attr_linux *attr, unsigned int flags);
long sched_getattr_linux(int pid, sched_attr_linux *attr, unsigned int size, unsigned int flags);
long sched_yield_linux(void);
long sched_get_priority_max_linux(int policy);
long sched_get_priority_min_linux(int policy);
// Disabled wrapper: long sched_rr_get_interval_linux(int pid, __kernel_old_timespec_linux *interval);
long sched_rr_get_interval_time64_linux(int pid, __kernel_timespec_linux *interval);
long sched_setaffinity_linux(int pid, unsigned int len, unsigned long *user_mask_ptr);
long sched_getaffinity_linux(int pid, unsigned int len, unsigned long *user_mask_ptr);
long nice_linux(int increment);
long setpriority_linux(int which, int who, int niceval);
long getpriority_linux(int which, int who);
//
// 4. MEMORY MANAGEMENT
//
// 4a. Memory mapping, allocation, and unmapping
long brk_linux(unsigned long brk);
long mmap_linux(unsigned long addr, unsigned long len, unsigned long prot, unsigned long flags, unsigned long fd, unsigned long long off);
long mmap2_linux(unsigned long addr, unsigned long len, unsigned long prot, unsigned long flags, unsigned long fd, unsigned long pgoff);
long munmap_linux(unsigned long addr, unsigned long len);
long mremap_linux(unsigned long addr, unsigned long old_len, unsigned long new_len, unsigned long flags, unsigned long new_addr);
long remap_file_pages_linux(unsigned long start, unsigned long size, unsigned long prot, unsigned long pgoff, unsigned long flags);
// 4b. Memory protection, locking, and usage hints
long mprotect_linux(unsigned long start, unsigned long len, unsigned long prot);
long pkey_mprotect_linux(unsigned long start, unsigned long len, unsigned long prot, int pkey);
long madvise_linux(unsigned long start, unsigned long len, int behavior);
long process_madvise_linux(int pidfd, const iovec_linux *vec, unsigned long vlen, int behavior, unsigned int flags);
long mlock_linux(unsigned long start, unsigned long len);
long mlock2_linux(unsigned long start, unsigned long len, int flags);
long munlock_linux(unsigned long start, unsigned long len);
long mlockall_linux(int flags);
long munlockall_linux(void);
long mincore_linux(unsigned long start, unsigned long len, unsigned char * vec);
long msync_linux(unsigned long start, unsigned long len, int flags);
long mseal_linux(unsigned long start, unsigned long len, unsigned long flags);
// 4c. NUMA memory policy and page migration
long mbind_linux(unsigned long start, unsigned long len, unsigned long mode, const unsigned long *nmask, unsigned long maxnode, unsigned flags);
long set_mempolicy_linux(int mode, const unsigned long *nmask, unsigned long maxnode);
long get_mempolicy_linux(int *policy, unsigned long *nmask, unsigned long maxnode, unsigned long addr, unsigned long flags);
long set_mempolicy_home_node_linux(unsigned long start, unsigned long len, unsigned long home_node, unsigned long flags);
long migrate_pages_linux(int pid, unsigned long maxnode, const unsigned long *from, const unsigned long *to);
long move_pages_linux(int pid, unsigned long nr_pages, const void * *pages, const int *nodes, int *status, int flags);
// 4d. Anonymous file-backed memory regions
long memfd_create_linux(const char *uname_ptr, unsigned int flags);
#if !defined(__arm__)
long memfd_secret_linux(unsigned int flags);
#endif
// 4e. Memory protection key management
long pkey_alloc_linux(unsigned long flags, unsigned long init_val);
long pkey_free_linux(int pkey);
// 4f. Control-flow integrity, shadow stack mapping
long map_shadow_stack_linux(unsigned long addr, unsigned long size, unsigned int flags);
// 4g. Advanced memory operations
long userfaultfd_linux(int flags);
long process_mrelease_linux(int pidfd, unsigned int flags);
long membarrier_linux(int cmd, unsigned int flags, int cpu_id);
//
// 5. FILE I/O OPERATIONS
//
// 5a. Opening, creating, and closing files
long open_linux(const char *filename, int flags, unsigned int mode);
long openat_linux(int dfd, const char *filename, int flags, unsigned int mode);
long openat2_linux(int dfd, const char *filename, open_how_linux *how, unsigned long size);
long creat_linux(const char *pathname, unsigned int mode);
long close_linux(unsigned int fd);
long close_range_linux(unsigned int fd, unsigned int max_fd, unsigned int flags);
long open_by_handle_at_linux(int mountdirfd, file_handle_linux *handle, int flags);
long name_to_handle_at_linux(int dfd, const char *name, file_handle_linux *handle, void *mnt_id, int flag);
// 5b. Reading and writing file data
long read_linux(unsigned int fd, char *buf, unsigned long count);
long write_linux(unsigned int fd, const char *buf, unsigned long count);
long readv_linux(unsigned long fd, const iovec_linux *vec, unsigned long vlen);
long writev_linux(unsigned long fd, const iovec_linux *vec, unsigned long vlen);
long pread64_linux(unsigned int fd, char *buf, unsigned long count, long long pos);
long pwrite64_linux(unsigned int fd, const char *buf, unsigned long count, long long pos);
long preadv_linux(unsigned long fd, const iovec_linux *vec, unsigned long vlen, unsigned long pos_l, unsigned long pos_h);
long pwritev_linux(unsigned long fd, const iovec_linux *vec, unsigned long vlen, unsigned long pos_l, unsigned long pos_h);
long preadv2_linux(unsigned long fd, const iovec_linux *vec, unsigned long vlen, unsigned long pos_l, unsigned long pos_h, int flags);
long pwritev2_linux(unsigned long fd, const iovec_linux *vec, unsigned long vlen, unsigned long pos_l, unsigned long pos_h, int flags);
// 5c. Seeking and truncating files
// Disabled wrapper: long lseek_linux(unsigned int fd, long offset, unsigned int whence);
long llseek_linux(unsigned int fd, unsigned long long offset, long long *result, unsigned int whence);
// Disabled wrapper: long _llseek_linux(unsigned int fd, unsigned long offset_high, unsigned long offset_low, long long *result, unsigned int whence);
// Disabled wrapper: long truncate_linux(const char *path, long length);
long truncate64_linux(const char *path, long long length);
// Disabled wrapper: long ftruncate_linux(unsigned int fd, long length);
long ftruncate64_linux(unsigned int fd, long long length);
// 5d. Zero-copy and specialized I/O
// Disabled wrapper: long sendfile_linux(int out_fd, int in_fd, long *offset, unsigned long count);
long sendfile64_linux(int out_fd, int in_fd, long long *offset, unsigned long count);
long splice_linux(int fd_in, long long *off_in, int fd_out, long long *off_out, unsigned long len, unsigned int flags);
long tee_linux(int fdin, int fdout, unsigned long len, unsigned int flags);
long vmsplice_linux(int fd, const iovec_linux *iov, unsigned long nr_segs, unsigned int flags);
long copy_file_range_linux(int fd_in, long long *off_in, int fd_out, long long *off_out, unsigned long len, unsigned int flags);
// 5e. I/O hints and space allocation
// Disabled wrapper: long fadvise64_linux(int fd, long long offset, unsigned long len, int advice);
long fadvise64_64_linux(int fd, long long offset, long long len, int advice);
// Disabled wrapper: long arm_fadvise64_64_linux(int fd, int advice, long long offset, long long len);
long readahead_linux(int fd, long long offset, unsigned long count);
long fallocate_linux(int fd, int mode, long long offset, long long len);
// 5f. Flushing file data to storage
long sync_linux(void);
long syncfs_linux(int fd);
long fsync_linux(unsigned int fd);
long fdatasync_linux(unsigned int fd);
long sync_file_range_linux(int fd, long long offset, long long nbytes, unsigned int flags);
// Disabled wrapper: long arm_sync_file_range_linux(int fd, long long offset, long long nbytes, unsigned int flags);
//
// 6. FILE DESCRIPTOR MANAGEMENT
//
// 6a. Duplicating and controlling file descriptors
long dup_linux(unsigned int fildes);
// Disabled wrapper: long dup2_linux(unsigned int oldfd, unsigned int newfd);
long dup3_linux(unsigned int oldfd, unsigned int newfd, int flags);
// Disabled wrapper: long fcntl_linux(unsigned int fd, unsigned int cmd, unsigned long arg);
long fcntl64_linux(unsigned int fd, unsigned int cmd, unsigned long arg);
// 6b. Device-specific control operations
long ioctl_linux(unsigned int fd, unsigned int cmd, unsigned long arg);
// 6c. I/O Multiplexing
// Disabled wrapper: long select_linux(int n, fd_set_linux *inp, fd_set_linux *outp, fd_set_linux *exp, __kernel_old_timeval *tvp);
// Disabled wrapper: long _newselect_linux(int n, fd_set_linux *inp, fd_set_linux *outp, fd_set_linux *exp, __kernel_old_timeval *tvp);
// Disabled wrapper: pselect6_linux(int n, fd_set_linux *inp, fd_set_linux *outp, fd_set_linux *exp, __kernel_old_timespec_linux *tsp, void *sig);
long pselect6_time64_linux(int n, fd_set_linux *inp, fd_set_linux *outp, fd_set_linux *exp, __kernel_timespec_linux *tsp, void *sig);
long poll_linux(pollfd_linux *ufds, unsigned int nfds, int timeout);
// Disabled wrapper: long ppoll_linux(pollfd_linux *, unsigned int, __kernel_old_timespec_linux *, const sigset_t_linux *, unsigned long);
long ppoll_time64_linux(pollfd_linux *ufds, unsigned int nfds, __kernel_timespec_linux *tsp, const sigset_t_linux *sigmask, unsigned long sigsetsize);
// 6d. Scalable I/O event notification
// Disabled wrapper: long epoll_create_linux(int size);
long epoll_create1_linux(int flags);
long epoll_ctl_linux(int epfd, int op, int fd, epoll_event_linux *event);
long epoll_wait_linux(int epfd, epoll_event_linux *events, int maxevents, int timeout);
long epoll_pwait_linux(int epfd, epoll_event_linux *events, int maxevents, int timeout, const sigset_t_linux *sigmask, unsigned long sigsetsize);
long epoll_pwait2_linux(int epfd, epoll_event_linux *events, int maxevents, const __kernel_timespec_linux *timeout, const sigset_t_linux *sigmask, unsigned long sigsetsize);
// Disabled wrapper: long epoll_ctl_old_linux(int epfd, int op, int fd, epoll_event_linux *event);
// Disabled wrapper: long epoll_wait_old_linux(int epfd, epoll_event_linux *events, int maxevents, int timeout);
//
// 7. FILE METADATA
//
// 7a. Getting file attributes and status
// Disabled wrapper: long stat_linux(const char *filename, __old_kernel_stat *statbuf);
// Disabled wrapper: long fstat_linux(unsigned int fd, __old_kernel_stat *statbuf);
// Disabled wrapper: long lstat_linux(const char *filename, __old_kernel_stat *statbuf);
// Disabled wrapper: long stat64_linux(const char *filename, stat64_t_linux *statbuf);
// Disabled wrapper: long fstat64_linux(unsigned long fd, stat64_t_linux *statbuf);
// Disabled wrapper: long lstat64_linux(const char *filename, stat64_t_linux *statbuf);
// Disabled wrapper: long newfstatat_linux(int dfd, const char *filename, stat_t_linux *statbuf, int flag);
// Disabled wrapper: long fstatat64_linux(int dfd, const char *filename, stat64_t_linux *statbuf, int flag);
long statx_linux(int dfd, const char *path, unsigned flags, unsigned mask, statx_t_linux *buffer);
// Disabled wrapper: long oldstat_linux(const char *filename, __old_kernel_stat *statbuf);
// Disabled wrapper: long oldfstat_linux(unsigned int fd, __old_kernel_stat *statbuf);
// Disabled wrapper: long oldlstat_linux(const char *filename, __old_kernel_stat *statbuf);
long file_getattr_linux(int dfd, const char *filename, file_attr_linux *attr, unsigned long usize, unsigned int at_flags);
// 7b. Changing file permissions and ownership
long chmod_linux(const char *filename, unsigned int mode);
long fchmod_linux(unsigned int fd, unsigned int mode);
long fchmodat_linux(int dfd, const char *filename, unsigned int mode);
long fchmodat2_linux(int dfd, const char *filename, unsigned int mode, unsigned int flags);
long umask_linux(int mask);
// Disabled wrapper: long chown_linux(const char *filename, unsigned int user, unsigned int group);
// Disabled wrapper: long fchown_linux(unsigned int fd, unsigned int user, unsigned int group);
// Disabled wrapper: long lchown_linux(const char *filename, unsigned int user, unsigned int group);
long chown32_linux(const char *filename, unsigned int user, unsigned int group);
long fchown32_linux(unsigned int fd, unsigned int user, unsigned int group);
long lchown32_linux(const char *filename, unsigned int user, unsigned int group);
long fchownat_linux(int dfd, const char *filename, unsigned int user, unsigned int group, int flag);
long file_setattr_linux(int dfd, const char *filename, file_attr_linux *attr, unsigned long usize, unsigned int at_flags);
// 7c. File access and modification times
// Disabled wrapper: long utime_linux(char *filename, utimbuf_linux *times);
// Disabled wrapper: long utimes_linux(char *filename, __kernel_old_timeval *utimes);
// Disabled wrapper: long futimesat_linux(int dfd, const char *filename, __kernel_old_timeval *utimes);
// Disabled wrapper: long utimensat_linux(int dfd, const char *filename, __kernel_old_timespec_linux *utimes, int flags);
long utimensat_time64_linux(int dfd, const char *filename, __kernel_timespec_linux *t, int flags);
// 7d. Testing file accessibility
long access_linux(const char *filename, int mode);
long faccessat_linux(int dfd, const char *filename, int mode);
long faccessat2_linux(int dfd, const char *filename, int mode, int flags);
// 7e. Getting, setting, and listing extended attributes
long setxattr_linux(const char *path, const char *name, const void *value, unsigned long size, int flags);
long lsetxattr_linux(const char *path, const char *name, const void *value, unsigned long size, int flags);
long fsetxattr_linux(int fd, const char *name, const void *value, unsigned long size, int flags);
long setxattrat_linux(int dfd, const char *path, unsigned int at_flags, const char *name, const xattr_args_linux *args, unsigned long size);
long getxattr_linux(const char *path, const char *name, void *value, unsigned long size);
long lgetxattr_linux(const char *path, const char *name, void *value, unsigned long size);
long fgetxattr_linux(int fd, const char *name, void *value, unsigned long size);
long getxattrat_linux(int dfd, const char *path, unsigned int at_flags, const char *name, xattr_args_linux *args, unsigned long size);
long listxattr_linux(const char *path, char *list, unsigned long size);
long llistxattr_linux(const char *path, char *list, unsigned long size);
long flistxattr_linux(int fd, char *list, unsigned long size);
long listxattrat_linux(int dfd, const char *path, unsigned int at_flags, char *list, unsigned long size);
long removexattr_linux(const char *path, const char *name);
long lremovexattr_linux(const char *path, const char *name);
long fremovexattr_linux(int fd, const char *name);
long removexattrat_linux(int dfd, const char *path, unsigned int at_flags, const char *name);
// 7f. Advisory file locking
long flock_linux(unsigned int fd, unsigned int cmd);
#if 0 // WIP
//
// 8. DIRECTORY & NAMESPACE OPERATIONS
//
// 8a. Creating, removing, and reading directories
long mkdir_linux(const char *pathname, unsigned int mode);
long mkdirat_linux(int dfd, const char * pathname, unsigned int mode);
long rmdir_linux(const char *pathname);
long getdents_linux(unsigned int fd, linux_dirent *dirent, unsigned int count);
long getdents64_linux(unsigned int fd, linux_dirent64 *dirent, unsigned int count);
long readdir_linux(unsigned int fd, old_linux_dirent *dirent, unsigned int count);
// 8b. Getting and changing current directory
long getcwd_linux(char *buf, unsigned long size);
long chdir_linux(const char *filename);
long fchdir_linux(unsigned int fd);
// 8c. Creating and managing hard and symbolic links
long link_linux(const char *oldname, const char *newname);
long linkat_linux(int olddfd, const char *oldname, int newdfd, const char *newname, int flags);
long unlink_linux(const char *pathname);
long unlinkat_linux(int dfd, const char * pathname, int flag);
long symlink_linux(const char *old, const char *new);
long symlinkat_linux(const char * oldname, int newdfd, const char * newname);
long readlink_linux(const char *path, char *buf, int bufsiz);
long readlinkat_linux(int dfd, const char *path, char *buf, int bufsiz);
long rename_linux(const char *oldname, const char *newname);
long renameat_linux(int olddfd, const char * oldname, int newdfd, const char * newname);
long renameat2_linux(int olddfd, const char *oldname, int newdfd, const char *newname, unsigned int flags);
// 8d. Creating device and named pipe nodes
long mknod_linux(const char *filename, unsigned int mode, unsigned dev);
long mknodat_linux(int dfd, const char * filename, unsigned int mode, unsigned dev);
//
// 9. FILE SYSTEM OPERATIONS
//
// 9a. Mounting filesystems and changing root
long mount_linux(char *dev_name, char *dir_name, char *type, unsigned long flags, void *data);
long umount_linux(char *name, int flags);
long umount2_linux(char *name, int flags);
long pivot_root_linux(const char *new_root, const char *put_old);
long chroot_linux(const char *filename);
long mount_setattr_linux(int dfd, const char *path, unsigned int flags, mount_attr *uattr, unsigned long usize);
long move_mount_linux(int from_dfd, const char *from_path, int to_dfd, const char *to_path, unsigned int ms_flags);
long open_tree_linux(int dfd, const char *path, unsigned flags);
long open_tree_attr_linux(int dfd, const char *path, unsigned flags, mount_attr *uattr, unsigned long usize);
long fsconfig_linux(int fs_fd, unsigned int cmd, const char *key, const void *value, int aux);
long fsmount_linux(int fs_fd, unsigned int flags, unsigned int ms_flags);
long fsopen_linux(const char *fs_name, unsigned int flags);
long fspick_linux(int dfd, const char *path, unsigned int flags);
// 9b. Getting filesystem statistics
long statfs_linux(const char * path, statfs *buf);
long fstatfs_linux(unsigned int fd, statfs *buf);
long statfs64_linux(const char *path, unsigned long sz, statfs64 *buf);
long fstatfs64_linux(unsigned int fd, unsigned long sz, statfs64 *buf);
long ustat_linux(unsigned dev, ustat *ubuf);
long statmount_linux(const mnt_id_req *req, statmount *buf, unsigned long bufsize, unsigned int flags);
long listmount_linux(const mnt_id_req *req, u64 *mnt_ids, unsigned long nr_mnt_ids, unsigned int flags);
// 9c. Disk quota control
long quotactl_linux(unsigned int cmd, const char *special, qid_t id, void *addr);
long quotactl_fd_linux(unsigned int fd, unsigned int cmd, qid_t id, void *addr);
//
// 10. FILE SYSTEM MONITORING
//
// 10a. Monitoring filesystem events
long inotify_init_linux(void);
long inotify_init1_linux(int flags);
long inotify_add_watch_linux(int fd, const char *path, u32 mask);
long inotify_rm_watch_linux(int fd, __s32 wd);
// 10b. Filesystem-wide event notification
long fanotify_init_linux(unsigned int flags, unsigned int event_f_flags);
long fanotify_mark_linux(int fanotify_fd, unsigned int flags, u64 mask, int fd, const char *pathname);
//
// 11. SIGNALS
//
// 11a. Setting up signal handlers
long signal_linux(int sig, __sighandler_t handler);
long sigaction_linux(int sig, const old_sigaction *act, old_sigaction *oact);
long rt_sigaction_linux(int sig, const sigaction *act, sigaction *oact, unsigned long sigsetsize);
// 11b. Sending signals to processes
long kill_linux(int pid, int sig);
long tkill_linux(int pid, int sig);
long tgkill_linux(int tgid, int pid, int sig);
long rt_sigqueueinfo_linux(int pid, int sig, siginfo_t *uinfo);
long rt_tgsigqueueinfo_linux(int tgid, int pid, int sig, siginfo_t *uinfo);
// 11c. Blocking and unblocking signals
long sigprocmask_linux(int how, old_sigset_t *set, old_sigset_t *oset);
long rt_sigprocmask_linux(int how, sigset_t_linux *set, sigset_t_linux *oset, unsigned long sigsetsize);
long sgetmask_linux(void);
long ssetmask_linux(int newmask);
// 11d. Waiting for and querying signals
long sigpending_linux(old_sigset_t *uset);
long rt_sigpending_linux(sigset_t_linux *set, unsigned long sigsetsize);
long sigsuspend_linux(old_sigset_t mask);
long rt_sigsuspend_linux(sigset_t_linux *unewset, unsigned long sigsetsize);
long pause_linux(void);
// Disabled wrapper: long rt_sigtimedwait_linux(const sigset_t_linux *uthese, siginfo_t *uinfo, const __kernel_old_timespec_linux *uts, unsigned long sigsetsize);
long rt_sigtimedwait_time64_linux(compat_sigset_t *uthese, compat_siginfo *uinfo, __kernel_timespec_linux *uts, compat_size_t sigsetsize);
// 11e. Alternate signal stack and return from handlers
long sigaltstack_linux(const sigaltstack *uss, sigaltstack *uoss);
long sigreturn_linux(pt_regs *regs);
long rt_sigreturn_linux(pt_regs *regs);
// 11f. Signal delivery via file descriptors
long signalfd_linux(int ufd, sigset_t_linux *user_mask, unsigned long sizemask);
long signalfd4_linux(int ufd, sigset_t_linux *user_mask, unsigned long sizemask, int flags);
//
// 12. PIPES & FIFOs
//
long pipe_linux(int *fildes);
long pipe2_linux(int *fildes, int flags);
//
// 13. INTER-PROCESS COMMUNICATION
//
// 13a. System V IPC - Shared Memory
long shmget_linux(key_t key, unsigned long size, int flag);
long shmat_linux(int shmid, char *shmaddr, int shmflg);
long shmdt_linux(char *shmaddr);
long shmctl_linux(int shmid, int cmd, shmid_ds *buf);
// 13b. System V IPC - Message Queues
long msgget_linux(key_t key, int msgflg);
long msgsnd_linux(int msqid, msgbuf *msgp, unsigned long msgsz, int msgflg);
long msgrcv_linux(int msqid, msgbuf *msgp, unsigned long msgsz, long msgtyp, int msgflg);
long msgctl_linux(int msqid, int cmd, msqid_ds *buf);
// 13c. System V IPC - Semaphores
long semget_linux(key_t key, int nsems, int semflg);
long semop_linux(int semid, sembuf *sops, unsigned nsops);
long semctl_linux(int semid, int semnum, int cmd, unsigned long arg);
// Disabled wrapper: long semtimedop_linux(int semid, sembuf *sops, unsigned nsops, const __kernel_old_timespec_linux *timeout);
long semtimedop_time64_linux(int semid, sembuf *tsops, unsigned int nsops, const __kernel_timespec_linux *timeout);
// 13d. POSIX Message Queues
long mq_open_linux(const char *name, int oflag, unsigned int mode, mq_attr *attr);
long mq_unlink_linux(const char *name);
// Disabled wrapper: long mq_timedsend_linux(mqd_t mqdes, const char *msg_ptr, unsigned long msg_len, unsigned int msg_prio, const __kernel_old_timespec_linux *abs_timeout);
long mq_timedsend_time64_linux(mqd_t mqdes, const char *u_msg_ptr, unsigned long msg_len, unsigned int msg_prio, const __kernel_timespec_linux *u_abs_timeout);
// Disabled wrapper: long mq_timedreceive_linux(mqd_t mqdes, char *msg_ptr, unsigned long msg_len, unsigned int *msg_prio, const __kernel_old_timespec_linux *abs_timeout);
long mq_timedreceive_time64_linux(mqd_t mqdes, char *u_msg_ptr, unsigned long msg_len, unsigned int *u_msg_prio, const __kernel_timespec_linux *u_abs_timeout);
long mq_notify_linux(mqd_t mqdes, const sigevent *notification);
long mq_getsetattr_linux(mqd_t mqdes, const mq_attr *mqstat, mq_attr *omqstat);
// 13e. Synchronization Primitives - Futexes
// Disabled wrapper: long futex_linux(u32 *uaddr, int op, u32 val, const __kernel_old_timespec_linux *utime, u32 *uaddr2, u32 val3);
long futex_time64_linux(u32 *uaddr, int op, u32 val, const __kernel_timespec_linux *utime, u32 *uaddr2, u32 val3);
long futex_wait_linux(void *uaddr, unsigned long val, unsigned long mask, unsigned int flags, __kernel_timespec_linux *timespec, clockid_t clockid);
long futex_wake_linux(void *uaddr, unsigned long mask, int nr, unsigned int flags);
long futex_waitv_linux(futex_waitv *waiters, unsigned int nr_futexes, unsigned int flags, __kernel_timespec_linux *timeout, clockid_t clockid);
long futex_requeue_linux(futex_waitv *waiters, unsigned int flags, int nr_wake, int nr_requeue);
long set_robust_list_linux(robust_list_head *head, unsigned long len);
long get_robust_list_linux(int pid, robust_list_head * *head_ptr, unsigned long *len_ptr);
// 13f. Synchronization Primitives - Event Notification
long eventfd_linux(unsigned int count);
long eventfd2_linux(unsigned int count, int flags);
//
// 14. SOCKETS & NETWORKING
//
// 14a. Creating and configuring sockets
long socket_linux(int family, int type, int protocol);
long socketpair_linux(int family, int type, int protocol, int *usockvec);
long bind_linux(int fd, sockaddr_linux *umyaddr, int addrlen);
long listen_linux(int fd, int backlog);
long accept_linux(int fd, sockaddr_linux *upeer_sockaddr, int *upeer_addrlen);
long accept4_linux(int fd, sockaddr_linux *upeer_sockaddr, int *upeer_addrlen, int flags);
long connect_linux(int fd, sockaddr_linux *uservaddr, int addrlen);
long shutdown_linux(int fd, int how);
long socketcall_linux(int call, unsigned long *args);
// 14b. Sending and receiving data on sockets
long send_linux(int fd, void *buff, unsigned long len, unsigned int flags);
long sendto_linux(int fd, void *buff, unsigned long len, unsigned int flags, sockaddr_linux *addr, int addr_len);
long sendmsg_linux(int fd, user_msghdr *msg, unsigned flags);
long sendmmsg_linux(int fd, mmsghdr *msg, unsigned int vlen, unsigned flags);
long recv_linux(int fd, void *ubuf, unsigned long size, unsigned int flags);
long recvfrom_linux(int fd, void *ubuf, unsigned long size, unsigned int flags, sockaddr_linux *addr, int *addr_len);
long recvmsg_linux(int fd, user_msghdr *msg, unsigned flags);
// Disabled wrapper: long recvmmsg_linux(int fd, mmsghdr *msg, unsigned int vlen, unsigned flags, __kernel_old_timespec_linux *timeout);
long recvmmsg_time64_linux(int fd, mmsghdr *mmsg, unsigned int vlen, unsigned int flags, __kernel_timespec_linux *timeout);
// 14c. Getting and setting socket options
long getsockopt_linux(int fd, int level, int optname, char *optval, int *optlen);
long setsockopt_linux(int fd, int level, int optname, char *optval, int optlen);
long getsockname_linux(int fd, sockaddr_linux *usockaddr, int *usockaddr_len);
long getpeername_linux(int fd, sockaddr_linux *usockaddr, int *usockaddr_len);
//
// 15. ASYNCHRONOUS I/O
//
// 15a. AIO: asynchronous I/O interface
long io_setup_linux(unsigned nr_reqs, aio_context_t *ctx);
long io_destroy_linux(aio_context_t ctx);
long io_submit_linux(aio_context_t ctx_id, long nr, iocb * *iocbpp);
long io_cancel_linux(aio_context_t ctx_id, iocb *iocb, io_event *result);
long io_getevents_linux(aio_context_t ctx_id, long min_nr, long nr, io_event *events, __kernel_timespec_linux *timeout);
// Disabled wrapper: long io_pgetevents_linux(aio_context_t ctx_id, long min_nr, long nr, io_event *events, __kernel_old_timespec_linux *timeout, const __aio_sigset *sig);
long io_pgetevents_time64_linux(aio_context_t ctx_id, long min_nr, long nr, io_event *events, __kernel_timespec_linux *timeout, const __aio_sigset *sig);
// 15b. io_uring: high-performance asynchronous I/O
long io_uring_setup_linux(u32 entries, io_uring_params *p);
long io_uring_enter_linux(unsigned int fd, u32 to_submit, u32 min_complete, u32 flags, const void *argp, unsigned long argsz);
long io_uring_register_linux(unsigned int fd, unsigned int op, void *arg, unsigned int nr_args);
//
// 16. TIME & CLOCKS
//
// 16a. Reading current time from various clocks
long time_linux(__kernel_old_time_t *tloc);
long gettimeofday_linux(__kernel_old_timeval *tv, timezone *tz);
// Disabled wrapper: long clock_gettime_linux(clockid_t which_clock, __kernel_old_timespec_linux *tp);
long clock_gettime64_linux(clockid_t which_clock, __kernel_timespec_linux *tp);
// Disabled wrapper: long clock_getres_linux(clockid_t which_clock, __kernel_old_timespec_linux *tp);
long clock_getres_time64_linux(clockid_t which_clock, __kernel_timespec_linux *tp);
// 16b. Setting system time and adjusting clocks
long settimeofday_linux(__kernel_old_timeval *tv, timezone *tz);
// Disabled wrapper: long clock_settime_linux(clockid_t which_clock, const __kernel_old_timespec_linux *tp);
long clock_settime64_linux(clockid_t which_clock, const __kernel_timespec_linux *tp);
long stime_linux(__kernel_old_time_t *tptr);
long adjtimex_linux(__kernel_timex *txc_p);
long clock_adjtime_linux(clockid_t which_clock, __kernel_timex *tx);
long clock_adjtime64_linux(clockid_t which_clock, __kernel_timex *tx);
// 16c. Suspending execution for a period of time
long nanosleep_linux(__kernel_timespec_linux *rqtp, __kernel_timespec_linux *rmtp);
// Disabled wrapper: long clock_nanosleep_linux(clockid_t which_clock, int flags, const __kernel_old_timespec_linux *rqtp, __kernel_old_timespec_linux *rmtp);
long clock_nanosleep_time64_linux(clockid_t which_clock, int flags, const __kernel_timespec_linux *rqtp, __kernel_timespec_linux *rmtp);
// 16d. Setting periodic or one-shot timers
long alarm_linux(unsigned int seconds);
long setitimer_linux(int which, __kernel_old_itimerval *value, __kernel_old_itimerval *ovalue);
long getitimer_linux(int which, __kernel_old_itimerval *value);
// 16e. Per-process timers with precise control
long timer_create_linux(clockid_t which_clock, sigevent *timer_event_spec, timer_t * created_timer_id);
// Disabled wrapper: long timer_settime_linux(timer_t timer_id, int flags, const __kernel_itimerspec *new_setting, __kernel_itimerspec *old_setting);
long timer_settime64_linux(timer_t timerid, int flags, const __kernel_timespec_linux *new_setting, __kernel_timespec_linux *old_setting);
// Disabled wrapper: long timer_gettime_linux(timer_t timer_id, __kernel_itimerspec *setting);
long timer_gettime64_linux(timer_t timerid, __kernel_timespec_linux *setting);
long timer_getoverrun_linux(timer_t timer_id);
long timer_delete_linux(timer_t timer_id);
// 16f. Timers accessible via file descriptors
long timerfd_create_linux(int clockid, int flags);
// Disabled wrapper: long timerfd_settime_linux(int ufd, int flags, const __kernel_itimerspec *utmr, __kernel_itimerspec *otmr);
long timerfd_settime64_linux(int ufd, int flags, const __kernel_timespec_linux *utmr, __kernel_timespec_linux *otmr);
// Disabled wrapper: long timerfd_gettime_linux(int ufd, __kernel_itimerspec *otmr);
long timerfd_gettime64_linux(int ufd, __kernel_timespec_linux *otmr);
//
// 17. RANDOM NUMBERS
//
long getrandom_linux(char *buf, unsigned long count, unsigned int flags);
//
// 18. USER & GROUP IDENTITY
//
// 18a. Getting and setting user IDs
long getuid_linux(void);
long geteuid_linux(void);
long setuid_linux(unsigned int uid);
long setreuid_linux(unsigned int ruid, unsigned int euid);
long setresuid_linux(unsigned int ruid, unsigned int euid, unsigned int suid);
long getresuid_linux(unsigned int *ruid, unsigned int *euid, unsigned int *suid);
long setfsuid_linux(unsigned int uid);
long getuid32_linux(void);
long geteuid32_linux(void);
long setuid32_linux(unsigned int uid);
long setreuid32_linux(unsigned int ruid, unsigned int euid);
long setresuid32_linux(unsigned int ruid, unsigned int euid, unsigned int suid);
long getresuid32_linux(unsigned int *ruid, unsigned int *euid, unsigned int *suid);
long setfsuid32_linux(unsigned int uid);
// 18b. Getting and setting group IDs
long getgid_linux(void);
long getegid_linux(void);
long setgid_linux(unsigned int gid);
long setregid_linux(unsigned int rgid, unsigned int egid);
long setresgid_linux(unsigned int rgid, unsigned int egid, unsigned int sgid);
long getresgid_linux(unsigned int *rgid, unsigned int *egid, unsigned int *sgid);
long setfsgid_linux(unsigned int gid);
long getgid32_linux(void);
long getegid32_linux(void);
long setgid32_linux(unsigned int gid);
long setregid32_linux(unsigned int rgid, unsigned int egid);
long setresgid32_linux(unsigned int rgid, unsigned int egid, unsigned int sgid);
long getresgid32_linux(unsigned int *rgid, unsigned int *egid, unsigned int *sgid);
long setfsgid32_linux(unsigned int gid);
// 18c. Managing supplementary group list
long getgroups_linux(int gidsetsize, unsigned int *grouplist);
long setgroups_linux(int gidsetsize, unsigned int *grouplist);
long getgroups32_linux(int gidsetsize, unsigned int *grouplist);
long setgroups32_linux(int gidsetsize, unsigned int *grouplist);
//
// 19. CAPABILITIES & SECURITY
//
// 19a. Fine-grained privilege control
long capget_linux(cap_user_header_t header, cap_user_data_t dataptr);
long capset_linux(cap_user_header_t header, const cap_user_data_t data);
// 19b. Syscall filtering and sandboxing
long seccomp_linux(unsigned int op, unsigned int flags, void *uargs);
// 19c. Linux Security Module interfaces
long security_linux(void);
long lsm_get_self_attr_linux(unsigned int attr, lsm_ctx *ctx, u32 *size, u32 flags);
long lsm_set_self_attr_linux(unsigned int attr, lsm_ctx *ctx, u32 size, u32 flags);
long lsm_list_modules_linux(u64 *ids, u32 *size, u32 flags);
// 19d. Unprivileged access control
long landlock_create_ruleset_linux(const landlock_ruleset_attr *attr, unsigned long size, __u32 flags);
long landlock_add_rule_linux(int ruleset_fd, enum landlock_rule_type rule_type, const void *rule_attr, __u32 flags);
long landlock_restrict_self_linux(int ruleset_fd, __u32 flags);
// 19e. Kernel key retention service
long add_key_linux(const char *_type, const char *_description, const void *_payload, unsigned long plen, key_serial_t destringid);
long request_key_linux(const char *_type, const char *_description, const char *_callout_info, key_serial_t destringid);
long keyctl_linux(int cmd, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5);
//
// 20. RESOURCE LIMITS & ACCOUNTING
//
// 20a. Getting and setting process resource limits
long getrlimit_linux(unsigned int resource, rlimit *rlim);
long setrlimit_linux(unsigned int resource, rlimit *rlim);
long prlimit64_linux(int pid, unsigned int resource, const rlimit64 *new_rlim, rlimit64 *old_rlim);
long ugetrlimit_linux(unsigned int resource, rlimit *rlim);
long ulimit_linux(int cmd, long newval);
// 20b. Getting resource usage and time statistics
long getrusage_linux(int who, rusage_linux *ru);
long times_linux(tms *tbuf);
// 20c. System-wide process accounting
long acct_linux(const char *name);
//
// 21. NAMESPACES & CONTAINERS
//
long unshare_linux(unsigned long unshare_flags);
long setns_linux(int fd, int nstype);
long listns_linux(const ns_id_req *req, u64 *ns_ids, unsigned long nr_ns_ids, unsigned int flags);
//
// 22. PROCESS INSPECTION & CONTROL
//
// 22a. Process comparison
long kcmp_linux(int pid1, int pid2, int type, unsigned long idx1, unsigned long idx2);
// 22b. Process file descriptors
long pidfd_open_linux(int pid, unsigned int flags);
long pidfd_getfd_linux(int pidfd, int fd, unsigned int flags);
long pidfd_send_signal_linux(int pidfd, int sig, siginfo_t *info, unsigned int flags);
// 22c. Process memory access
long process_vm_readv_linux(int pid, const iovec_linux *lvec, unsigned long liovcnt, const iovec_linux *rvec, unsigned long riovcnt, unsigned long flags);
long process_vm_writev_linux(int pid, const iovec_linux *lvec, unsigned long liovcnt, const iovec_linux *rvec, unsigned long riovcnt, unsigned long flags);
// 22d. Process tracing
long ptrace_linux(long request, long pid, unsigned long addr, unsigned long data);
//
// 23. SYSTEM INFORMATION
//
// 23a. System name and domain information
long uname_linux(old_utsname *);
long olduname_linux(oldold_utsname *);
long oldolduname_linux(oldold_utsname *name);
long gethostname_linux(char *name, int len);
long sethostname_linux(char *name, int len);
long setdomainname_linux(char *name, int len);
// 23b. Overall system information and statistics
long sysinfo_linux(sysinfo *info);
// 23c. Reading kernel log messages
long syslog_linux(int type, char *buf, int len);
// 23d. Getting CPU and NUMA node information
long getcpu_linux(unsigned *cpu, unsigned *node, getcpu_cache *cache);
// 23e. Kernel filesystem information interface
long sysfs_linux(int option, unsigned long arg1, unsigned long arg2);
//
// 24. KERNEL MODULES
//
long create_module_linux(const char *name, unsigned long size);
long init_module_linux(void *umod, unsigned long len, const char *uargs);
long finit_module_linux(int fd, const char *uargs, int flags);
long delete_module_linux(const char *name_user, unsigned int flags);
long query_module_linux(const char *name, int which, void *buf, unsigned long bufsize, unsigned long *ret);
long get_kernel_syms_linux(kernel_sym *table);
//
// 25. SYSTEM CONTROL & ADMINISTRATION
//
// 25a. Rebooting and shutting down the system
long reboot_linux(int magic1, int magic2, unsigned int cmd, void *arg);
// 25b. Enabling and disabling swap areas
long swapon_linux(const char *specialfile, int swap_flags);
long swapoff_linux(const char *specialfile);
// 25c. Loading and executing new kernels
long kexec_load_linux(unsigned long entry, unsigned long nr_segments, kexec_segment *segments, unsigned long flags);
long kexec_file_load_linux(int kernel_fd, int initrd_fd, unsigned long cmdline_len, const char *cmdline_ptr, unsigned long flags);
// 25d. Other system administration operations
long vhangup_linux(void);
//
// 26. PERFORMANCE MONITORING & TRACING
//
// 26a. Hardware and software performance monitoring
long perf_event_open_linux(perf_event_attr *attr_uptr, int pid, int cpu, int group_fd, unsigned long flags);
// 26b. Userspace dynamic tracing
long uprobe_linux(void);
long uretprobe_linux(void);
// 26c. Programmable Kernel Extensions (eBPF)
long bpf_linux(int cmd, union bpf_attr *attr, unsigned int size);
//
// 27. DEVICE & HARDWARE ACCESS
//
// 27a. Direct hardware I/O port access
long ioperm_linux(unsigned long from, unsigned long num, int on);
long iopl_linux(unsigned int level);
// 27b. Setting I/O scheduling priority
long ioprio_set_linux(int which, int who, int ioprio);
long ioprio_get_linux(int which, int who);
// 27c. PCI device configuration access
long pciconfig_read_linux(unsigned long bus, unsigned long dfn, unsigned long off, unsigned long len, void *buf);
long pciconfig_write_linux(unsigned long bus, unsigned long dfn, unsigned long off, unsigned long len, void *buf);
long pciconfig_iobase_linux(long which, unsigned long bus, unsigned long devfn);
// 27d. CPU cache control operations
long cacheflush_linux(unsigned long start, unsigned long end, int flags);
long cachestat_linux(unsigned int fd, cachestat_range *cstat_range, cachestat *cstat, unsigned int flags);
//
// 28. ARCHITECTURE-SPECIFIC OPERATIONS
//
// 28a. x86 architecture operations
long arch_prctl_linux(int option, unsigned long addr);
long modify_ldt_linux(int func, void *ptr, unsigned long bytecount);
long set_thread_area_linux(user_desc *u_info);
long get_thread_area_linux(user_desc *u_info);
long vm86_linux(unsigned long cmd, unsigned long arg);
long vm86old_linux(vm86_struct *user_vm86);
// 28b. ARM architecture operations
long set_tls_linux(unsigned long val);
long get_tls_linux(void);
// 28c. RISC-V architecture operations
long riscv_flush_icache_linux(uintptr_t start, uintptr_t end, uintptr_t flags);
long riscv_hwprobe_linux(riscv_hwprobe *pairs, unsigned long pair_count, unsigned long cpu_count, unsigned long *cpumask, unsigned int flags);
// 28d. Intel MPX support (deprecated)
long mpx_linux(void);
//
// 29. ADVANCED EXECUTION CONTROL
//
// 29a. Restartable sequences
long rseq_linux(rseq *rseq, uint32_t rseq_len, int flags, uint32_t sig);
// 29b. Restart syscall
long restart_syscall_linux(void);
// 29c. Directory entry cache
long lookup_dcookie_linux(u64 cookie64, char *buf, unsigned long len);
//
// 30. LEGACY, OBSOLETE & UNIMPLEMENTED
//
long _sysctl_linux(__sysctl_args *args);
long ipc_linux(unsigned int call, int first, unsigned long second, unsigned long third, void *ptr, long fifth);
long profil_linux(unsigned short *sample_buffer, unsigned long size, unsigned long offset, unsigned int scale);
long prof_linux(void);
long afs_syscall_linux(void);
long break_linux(void);
long ftime_linux(void);
long gtty_linux(void);
long idle_linux(void);
long lock_linux(void);
long nfsservctl_linux(int cmd, nfsctl_arg *arg, union nfsctl_res *res);
long getpmsg_linux(int fd, strbuf *ctlptr, strbuf *dataptr, int *bandp, int *flagsp);
long putpmsg_linux(int fd, strbuf *ctlptr, strbuf *dataptr, int band, int flags);
long stty_linux(void);
long tuxcall_linux(void);
long vserver_linux(void);
long bdflush_linux(int func, long data);
long uselib_linux(const char *library);
#endif // WIP

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

//
// 1. PROCESS & THREAD LIFECYCLE
//
long fork_linux(void) {
  return clone_linux(SIGCHLD_linux, 0, 0, 0, 0);
}
long vfork_linux(void) {
  return clone_linux(CLONE_VFORK_linux | CLONE_VM_linux | SIGCHLD_linux, 0, 0, 0, 0);
}
long clone_linux(unsigned long clone_flags, unsigned long newsp, int *parent_tidptr, int *child_tidptr, unsigned long tls) {
#if defined(__x86_64__)
  return Syscall5_linux(NR_clone_linux, clone_flags, newsp,  parent_tidptr, child_tidptr, tls, 0);
#else
  return Syscall5_linux(NR_clone_linux, clone_flags, newsp,  parent_tidptr, tls, child_tidptr, 0);
#endif
}
long clone3_linux(clone_args_linux *uargs, unsigned long size) {
  return Syscall2_linux(NR_clone3_linux, uargs, size, 0);
}
long execve_linux(const char *filename, const char *const *argv, const char *const *envp) {
  return Syscall3_linux(NR_execve_linux, filename, argv, envp, 0);
}
long execveat_linux(int dfd, const char *filename, const char *const *argv, const char *const *envp, int flags) {
  return Syscall5_linux(NR_execveat_linux, dfd, filename, argv, envp, flags, 0);
}
__attribute__((noreturn)) void exit_linux(int error_code) {
  Syscall1_linux(NR_exit_linux, error_code, 0);
  __builtin_unreachable();
}
__attribute__((noreturn)) void exit_group_linux(int error_code) {
  Syscall1_linux(NR_exit_group_linux, error_code, 0);
  __builtin_unreachable();
}
long wait4_linux(int pid, int *stat_addr, int options, rusage_linux *ru) {
#if !(defined(__riscv) && (__riscv_xlen == 32))
  return Syscall4_linux(NR_wait4_linux, pid, stat_addr, options, ru, 0);
#else
  int which = P_PID_linux;
  if (pid < -1) {
    which = P_PGID_linux;
    pid = -pid;
  } else if (pid == -1) {
    which = P_ALL_linux;
  } else if (pid == 0) {
    which = P_PGID_linux;
  }

  siginfo_t_linux infop;
  infop.si_pid_linux = 0;

  long ret = Syscall5_linux(NR_waitid_linux, which, pid, &infop, options | WEXITED_linux, ru, 0);

  if (ret >= 0) {
    ret = infop.si_pid_linux;
    if (infop.si_pid_linux && stat_addr) {
      switch (infop.si_code) {
        case CLD_EXITED_linux: *stat_addr = (infop.si_status_linux & 0xff) << 8; break;
        case CLD_KILLED_linux: *stat_addr = infop.si_status_linux & 0x7f; break;
        case CLD_DUMPED_linux: *stat_addr = (infop.si_status_linux & 0x7f) | 0x80; break;
        case CLD_TRAPPED_linux:
        case CLD_STOPPED_linux: *stat_addr = (infop.si_status_linux << 8) | 0x7f; break;
        case CLD_CONTINUED_linux: *stat_addr = 0xffff; break;
        default: *stat_addr = 0; break;
      }
    }
  }

  return ret;
#endif
}
long waitid_linux(int which, int pid, siginfo_t_linux *infop, int options, rusage_linux *ru) {
  return Syscall5_linux(NR_waitid_linux, which, pid, infop, options, ru, 0);
}
long waitpid_linux(int pid, int *stat_addr, int options) {
  return wait4_linux(pid, stat_addr, options, 0);
}
//
// 2. PROCESS ATTRIBUTES & CONTROL
//
// 2a. Process identity, process groups and sessions
long getpid_linux(void) {
  return Syscall0_linux(NR_getpid_linux, 0);
}
long getppid_linux(void) {
  return Syscall0_linux(NR_getppid_linux, 0);
}
long gettid_linux(void) {
  return Syscall0_linux(NR_gettid_linux, 0);
}
long getpgid_linux(int pid) {
  return Syscall1_linux(NR_getpgid_linux, pid, 0);
}
long setpgid_linux(int pid, int pgid) {
  return Syscall2_linux(NR_setpgid_linux, pid, pgid, 0);
}
long getpgrp_linux(void) {
  return getpgid_linux(0);
}
long getsid_linux(int pid) {
  return Syscall1_linux(NR_getsid_linux, pid, 0);
}
long setsid_linux(void) {
  return Syscall0_linux(NR_setsid_linux, 0);
}
long set_tid_address_linux(int *tidptr) {
  return Syscall1_linux(NR_set_tid_address_linux, tidptr, 0);
}
// 2b. Process control and personality
long prctl_linux(int option, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5) {
  return Syscall5_linux(NR_prctl_linux, option, arg2, arg3, arg4, arg5, 0);
}
long personality_linux(unsigned int personality) {
  return Syscall1_linux(NR_personality_linux, personality, 0);
}
//
// 3. SCHEDULING & PRIORITIES
//
long sched_setscheduler_linux(int pid, int policy, sched_param_linux *param) {
  return Syscall3_linux(NR_sched_setscheduler_linux, pid, policy, param, 0);
}
long sched_getscheduler_linux(int pid) {
  return Syscall1_linux(NR_sched_getscheduler_linux, pid, 0);
}
long sched_setparam_linux(int pid, sched_param_linux *param) {
  return Syscall2_linux(NR_sched_setparam_linux, pid, param, 0);
}
long sched_getparam_linux(int pid, sched_param_linux *param) {
  return Syscall2_linux(NR_sched_getparam_linux, pid, param, 0);
}
long sched_setattr_linux(int pid, sched_attr_linux *attr, unsigned int flags) {
  return Syscall3_linux(NR_sched_setattr_linux, pid, attr, flags, 0);
}
long sched_getattr_linux(int pid, sched_attr_linux *attr, unsigned int size, unsigned int flags) {
  return Syscall4_linux(NR_sched_getattr_linux, pid, attr, size, flags, 0);
}
long sched_yield_linux(void) {
  return Syscall0_linux(NR_sched_yield_linux, 0);
}
long sched_get_priority_max_linux(int policy) {
  return Syscall1_linux(NR_sched_get_priority_max_linux, policy, 0);
}
long sched_get_priority_min_linux(int policy) {
  return Syscall1_linux(NR_sched_get_priority_min_linux, policy, 0);
}
// Disabled wrapper: long sched_rr_get_interval_linux(int pid, __kernel_old_timespec_linux *interval);
long sched_rr_get_interval_time64_linux(int pid, __kernel_timespec_linux *interval) {
#if defined(__x86_64__) || (defined(__riscv) && (__riscv_xlen == 64))
  return Syscall2_linux(NR_sched_rr_get_interval_linux, pid, interval, 0);
#else
  return Syscall2_linux(NR_sched_rr_get_interval_time64_linux, pid, interval, 0);
#endif
}
long sched_setaffinity_linux(int pid, unsigned int len, unsigned long *user_mask_ptr) {
  return Syscall3_linux(NR_sched_setaffinity_linux, pid, len, user_mask_ptr, 0);
}
long sched_getaffinity_linux(int pid, unsigned int len, unsigned long *user_mask_ptr) {
  return Syscall3_linux(NR_sched_getaffinity_linux, pid, len, user_mask_ptr, 0);
}
long nice_linux(int increment) {
  long ret = getpriority_linux(PRIO_PROCESS_linux, 0);
  if (ret < 0) return ret;
  return setpriority_linux(PRIO_PROCESS_linux, 0, (int)(20 - ret + increment));
}
long setpriority_linux(int which, int who, int niceval) {
  return Syscall3_linux(NR_setpriority_linux, which, who, niceval, 0);
}
long getpriority_linux(int which, int who) {
  return Syscall2_linux(NR_getpriority_linux, which, who, 0);
}
//
// 4. MEMORY MANAGEMENT
//
// 4a. Memory mapping, allocation, and unmapping
long brk_linux(unsigned long brk) {
  return Syscall1_linux(NR_brk_linux, brk, 0);
}
long mmap_linux(unsigned long addr, unsigned long len, unsigned long prot, unsigned long flags, unsigned long fd, unsigned long long off) {
#if defined(__x86_64__) || defined(__aarch64__) || (defined(__riscv) && (__riscv_xlen == 64))
  return Syscall6_linux(NR_mmap_linux, addr, len, prot, flags, fd, off, 0);
#else
  return Syscall6_linux(NR_mmap2_linux, addr, len, prot, flags, fd, off / 4096, 0);
#endif
}
long mmap2_linux(unsigned long addr, unsigned long len, unsigned long prot, unsigned long flags, unsigned long fd, unsigned long pgoff) {
#if defined(__x86_64__) || defined(__aarch64__) || (defined(__riscv) && (__riscv_xlen == 64))
  return Syscall6_linux(NR_mmap_linux, addr, len, prot, flags, fd, pgoff * 4096, 0);
#else
  return Syscall6_linux(NR_mmap2_linux, addr, len, prot, flags, fd, pgoff, 0);
#endif
}
long munmap_linux(unsigned long addr, unsigned long len) {
  return Syscall2_linux(NR_munmap_linux, addr, len, 0);
}
long mremap_linux(unsigned long addr, unsigned long old_len, unsigned long new_len, unsigned long flags, unsigned long new_addr) {
  return Syscall5_linux(NR_mremap_linux, addr, old_len, new_len, flags, new_addr, 0);
}
long remap_file_pages_linux(unsigned long start, unsigned long size, unsigned long prot, unsigned long pgoff, unsigned long flags) {
  return Syscall5_linux(NR_remap_file_pages_linux, start, size, prot, pgoff, flags, 0);
}
// 4b. Memory protection, locking, and usage hints
long mprotect_linux(unsigned long start, unsigned long len, unsigned long prot) {
  return Syscall3_linux(NR_mprotect_linux, start, len, prot, 0);
}
long pkey_mprotect_linux(unsigned long start, unsigned long len, unsigned long prot, int pkey) {
  return Syscall4_linux(NR_pkey_mprotect_linux, start, len, prot, pkey, 0);
}
long madvise_linux(unsigned long start, unsigned long len, int behavior) {
  return Syscall3_linux(NR_madvise_linux, start, len, behavior, 0);
}
long process_madvise_linux(int pidfd, const iovec_linux *vec, unsigned long vlen, int behavior, unsigned int flags) {
  return Syscall5_linux(NR_process_madvise_linux, pidfd, vec, vlen, behavior, flags, 0);
}
long mlock_linux(unsigned long start, unsigned long len) {
  return Syscall2_linux(NR_mlock_linux, start, len, 0);
}
long mlock2_linux(unsigned long start, unsigned long len, int flags) {
  return Syscall3_linux(NR_mlock2_linux, start, len, flags, 0);
}
long munlock_linux(unsigned long start, unsigned long len) {
  return Syscall2_linux(NR_munlock_linux, start, len, 0);
}
long mlockall_linux(int flags) {
  return Syscall1_linux(NR_mlockall_linux, flags, 0);
}
long munlockall_linux(void) {
  return Syscall0_linux(NR_munlockall_linux, 0);
}
long mincore_linux(unsigned long start, unsigned long len, unsigned char * vec) {
  return Syscall3_linux(NR_mincore_linux, start, len, vec, 0);
}
long msync_linux(unsigned long start, unsigned long len, int flags) {
  return Syscall3_linux(NR_msync_linux, start, len, flags, 0);
}
long mseal_linux(unsigned long start, unsigned long len, unsigned long flags) {
  return Syscall3_linux(NR_mseal_linux, start, len, flags, 0);
}
// 4c. NUMA memory policy and page migration
long mbind_linux(unsigned long start, unsigned long len, unsigned long mode, const unsigned long *nmask, unsigned long maxnode, unsigned flags) {
  return Syscall6_linux(NR_mbind_linux, start, len, mode, nmask, maxnode, flags, 0);
}
long set_mempolicy_linux(int mode, const unsigned long *nmask, unsigned long maxnode) {
  return Syscall3_linux(NR_set_mempolicy_linux, mode, nmask, maxnode, 0);
}
long get_mempolicy_linux(int *policy, unsigned long *nmask, unsigned long maxnode, unsigned long addr, unsigned long flags) {
  return Syscall5_linux(NR_get_mempolicy_linux, policy, nmask, maxnode, addr, flags, 0);
}
long set_mempolicy_home_node_linux(unsigned long start, unsigned long len, unsigned long home_node, unsigned long flags) {
  return Syscall4_linux(NR_set_mempolicy_home_node_linux, start, len, home_node, flags, 0);
}
long migrate_pages_linux(int pid, unsigned long maxnode, const unsigned long *from, const unsigned long *to) {
  return Syscall4_linux(NR_migrate_pages_linux, pid, maxnode, from, to, 0);
}
long move_pages_linux(int pid, unsigned long nr_pages, const void * *pages, const int *nodes, int *status, int flags) {
  return Syscall6_linux(NR_move_pages_linux, pid, nr_pages, pages, nodes, status, flags, 0);
}
// 4d. Anonymous file-backed memory regions
long memfd_create_linux(const char *uname_ptr, unsigned int flags) {
  return Syscall2_linux(NR_memfd_create_linux, uname_ptr, flags, 0);
}
#if !defined(__arm__)
long memfd_secret_linux(unsigned int flags) {
  return Syscall1_linux(NR_memfd_secret_linux, flags, 0);
}
#endif
// 4e. Memory protection key management
long pkey_alloc_linux(unsigned long flags, unsigned long init_val) {
  return Syscall2_linux(NR_pkey_alloc_linux, flags, init_val, 0);
}
long pkey_free_linux(int pkey) {
  return Syscall1_linux(NR_pkey_free_linux, pkey, 0);
}
// 4f. Control-flow integrity, shadow stack mapping
long map_shadow_stack_linux(unsigned long addr, unsigned long size, unsigned int flags) {
  return Syscall3_linux(NR_map_shadow_stack_linux, addr, size, flags, 0);
}
// 4g. Advanced memory operations
long userfaultfd_linux(int flags) {
  return Syscall1_linux(NR_userfaultfd_linux, flags, 0);
}
long process_mrelease_linux(int pidfd, unsigned int flags) {
  return Syscall2_linux(NR_process_mrelease_linux, pidfd, flags, 0);
}
long membarrier_linux(int cmd, unsigned int flags, int cpu_id) {
  return Syscall3_linux(NR_membarrier_linux, cmd, flags, cpu_id, 0);
}
//
// 5. FILE I/O OPERATIONS
//
// 5a. Opening, creating, and closing files
long open_linux(const char *filename, int flags, unsigned int mode) {
  return openat_linux(AT_FDCWD_linux, filename, flags, mode);
}
long openat_linux(int dfd, const char *filename, int flags, unsigned int mode) {
  return Syscall4_linux(NR_openat_linux, dfd, filename, flags, mode, 0);
}
long openat2_linux(int dfd, const char *filename, open_how_linux *how, unsigned long size) {
  return Syscall4_linux(NR_openat2_linux, dfd, filename, how, size, 0);
}
long creat_linux(const char *pathname, unsigned int mode) {
  return open_linux(pathname, O_CREAT_linux | O_WRONLY_linux | O_TRUNC_linux, mode);
}
long close_linux(unsigned int fd) {
  return Syscall1_linux(NR_close_linux, fd, 0);
}
long close_range_linux(unsigned int fd, unsigned int max_fd, unsigned int flags) {
  return Syscall3_linux(NR_close_range_linux, fd, max_fd, flags, 0);
}
long open_by_handle_at_linux(int mountdirfd, file_handle_linux *handle, int flags) {
  return Syscall3_linux(NR_open_by_handle_at_linux, mountdirfd, handle, flags, 0);
}
long name_to_handle_at_linux(int dfd, const char *name, file_handle_linux *handle, void *mnt_id, int flag) {
  return Syscall5_linux(NR_name_to_handle_at_linux, dfd, name, handle, mnt_id, flag, 0);
}
// 5b. Reading and writing file data
long read_linux(unsigned int fd, char *buf, unsigned long count) {
  return Syscall3_linux(NR_read_linux, fd, buf, count, 0);
}
long write_linux(unsigned int fd, const char *buf, unsigned long count) {
  return Syscall3_linux(NR_write_linux, fd, buf, count, 0);
}
long readv_linux(unsigned long fd, const iovec_linux *vec, unsigned long vlen) {
  return Syscall3_linux(NR_readv_linux, fd, vec, vlen, 0);
}
long writev_linux(unsigned long fd, const iovec_linux *vec, unsigned long vlen) {
  return Syscall3_linux(NR_writev_linux, fd, vec, vlen, 0);
}
long pread64_linux(unsigned int fd, char *buf, unsigned long count, long long pos) {
#if defined(__x86_64__) || defined(__aarch64__) || (defined(__riscv) && (__riscv_xlen == 64))
  return Syscall4_linux(NR_pread64_linux, fd, buf, count, pos, 0);
#elif defined(__i386__)
  return Syscall5_linux(NR_pread64_linux, fd, buf, count, LO32_bits(pos), HI32_bits(pos), 0);
#elif defined(__arm__) || (defined(__riscv) && (__riscv_xlen == 32))
  return Syscall6_linux(NR_pread64_linux, fd, buf, count, 0, LO32_bits(pos), HI32_bits(pos), 0);
#endif
}
long pwrite64_linux(unsigned int fd, const char *buf, unsigned long count, long long pos) {
#if defined(__x86_64__) || defined(__aarch64__) || (defined(__riscv) && (__riscv_xlen == 64))
  return Syscall4_linux(NR_pwrite64_linux, fd, buf, count, pos, 0);
#elif defined(__i386__)
  return Syscall5_linux(NR_pwrite64_linux, fd, buf, count, LO32_bits(pos), HI32_bits(pos), 0);
#elif defined(__arm__) || (defined(__riscv) && (__riscv_xlen == 32))
  return Syscall6_linux(NR_pwrite64_linux, fd, buf, count, 0, LO32_bits(pos), HI32_bits(pos), 0);
#endif
}
long preadv_linux(unsigned long fd, const iovec_linux *vec, unsigned long vlen, unsigned long pos_l, unsigned long pos_h) {
  return Syscall5_linux(NR_preadv_linux, fd, vec, vlen, pos_l, pos_h, 0);
}
long pwritev_linux(unsigned long fd, const iovec_linux *vec, unsigned long vlen, unsigned long pos_l, unsigned long pos_h) {
  return Syscall5_linux(NR_pwritev_linux, fd, vec, vlen, pos_l, pos_h, 0);
}
long preadv2_linux(unsigned long fd, const iovec_linux *vec, unsigned long vlen, unsigned long pos_l, unsigned long pos_h, int flags) {
  return Syscall6_linux(NR_preadv2_linux, fd, vec, vlen, pos_l, pos_h, flags, 0);
}
long pwritev2_linux(unsigned long fd, const iovec_linux *vec, unsigned long vlen, unsigned long pos_l, unsigned long pos_h, int flags) {
  return Syscall6_linux(NR_pwritev2_linux, fd, vec, vlen, pos_l, pos_h, flags, 0);
}
// 5c. Seeking and truncating files
// Disabled wrapper: long lseek_linux(unsigned int fd, long offset, unsigned int whence);
long llseek_linux(unsigned int fd, unsigned long long offset, long long *result, unsigned int whence) {
#if defined(__x86_64__) || defined(__aarch64__) || (defined(__riscv) && (__riscv_xlen == 64))
  long ret = Syscall3_linux(NR_lseek_linux, fd, offset, whence, 0);
  if (ret >= 0 && result) {
    *result = ret;
    ret = 0;
  }
  return ret;
#elif defined(__riscv) && (__riscv_xlen == 32)
  return Syscall5_linux(NR_llseek_linux, fd, HI32_bits(offset), LO32_bits(offset), result, whence, 0);
#else
  return Syscall5_linux(NR__llseek_linux, fd, HI32_bits(offset), LO32_bits(offset), result, whence, 0);
#endif
}
// Disabled wrapper: long _llseek_linux(unsigned int fd, unsigned long offset_high, unsigned long offset_low, long long *result, unsigned int whence);
// Disabled wrapper: long truncate_linux(const char *path, long length);
long truncate64_linux(const char *path, long long length) {
#if defined(__x86_64__) || defined(__aarch64__) || (defined(__riscv) && (__riscv_xlen == 64))
  return Syscall2_linux(NR_truncate_linux, path, length, 0);
#elif defined(__i386__)
  return Syscall3_linux(NR_truncate64_linux, path, LO32_bits(length), HI32_bits(length), 0);
#elif defined(__arm__) || (defined(__riscv) && (__riscv_xlen == 32))
  return Syscall4_linux(NR_truncate64_linux, path, 0, LO32_bits(length), HI32_bits(length), 0);
#endif
}
// Disabled wrapper: long ftruncate_linux(unsigned int fd, long length);
long ftruncate64_linux(unsigned int fd, long long length) {
#if defined(__x86_64__) || defined(__aarch64__) || (defined(__riscv) && (__riscv_xlen == 64))
  return Syscall2_linux(NR_ftruncate_linux, fd, length, 0);
#elif defined(__i386__)
  return Syscall3_linux(NR_ftruncate64_linux, fd, LO32_bits(length), HI32_bits(length), 0);
#elif defined(__arm__) || (defined(__riscv) && (__riscv_xlen == 32))
  return Syscall4_linux(NR_ftruncate64_linux, fd, 0, LO32_bits(length), HI32_bits(length), 0);
#endif
}
// 5d. Zero-copy and specialized I/O
// Disabled wrapper: long sendfile_linux(int out_fd, int in_fd, long *offset, unsigned long count);
long sendfile64_linux(int out_fd, int in_fd, long long *offset, unsigned long count) {
#if defined(__x86_64__) || defined(__aarch64__) || (defined(__riscv) && (__riscv_xlen == 64))
  return Syscall4_linux(NR_sendfile_linux, out_fd, in_fd, offset, count, 0);
#else
  return Syscall4_linux(NR_sendfile64_linux, out_fd, in_fd, offset, count, 0);
#endif
}
long splice_linux(int fd_in, long long *off_in, int fd_out, long long *off_out, unsigned long len, unsigned int flags) {
  return Syscall6_linux(NR_splice_linux, fd_in, off_in, fd_out, off_out, len, flags, 0);
}
long tee_linux(int fdin, int fdout, unsigned long len, unsigned int flags) {
  return Syscall4_linux(NR_tee_linux, fdin, fdout, len, flags, 0);
}
long vmsplice_linux(int fd, const iovec_linux *iov, unsigned long nr_segs, unsigned int flags) {
  return Syscall4_linux(NR_vmsplice_linux, fd, iov, nr_segs, flags, 0);
}
long copy_file_range_linux(int fd_in, long long *off_in, int fd_out, long long *off_out, unsigned long len, unsigned int flags) {
  return Syscall6_linux(NR_copy_file_range_linux, fd_in, off_in, fd_out, off_out, len, flags, 0);
}
// 5e. I/O hints and space allocation
// Disabled wrapper: long fadvise64_linux(int fd, long long offset, unsigned long len, int advice);
long fadvise64_64_linux(int fd, long long offset, long long len, int advice) {
#if defined(__x86_64__) || defined(__aarch64__) || (defined(__riscv) && (__riscv_xlen == 64))
  return Syscall4_linux(NR_fadvise64_linux, fd, offset, len, advice, 0);
#elif defined(__i386__)
  return Syscall6_linux(NR_fadvise64_64_linux, fd, LO32_bits(offset), HI32_bits(offset), LO32_bits(len), HI32_bits(len), advice, 0);
#elif defined(__arm__)
  return Syscall6_linux(NR_arm_fadvise64_64_linux, fd, advice, LO32_bits(offset), HI32_bits(offset), LO32_bits(len), HI32_bits(len), 0);
#elif defined(__riscv) && (__riscv_xlen == 32)
   return Syscall6_linux(NR_fadvise64_64_linux, fd, advice, 0, LO32_bits(offset), HI32_bits(offset), LO32_bits(len), HI32_bits(len), 0);
#endif
}
// Disabled wrapper: long arm_fadvise64_64_linux(int fd, int advice, long long offset, long long len);
long readahead_linux(int fd, long long offset, unsigned long count) {
#if defined(__x86_64__) || defined(__aarch64__) || (defined(__riscv) && (__riscv_xlen == 64))
  return Syscall3_linux(NR_readahead_linux, fd, offset, count, 0);
#elif defined(__i386__)
  return Syscall4_linux(NR_readahead_linux, fd, LO32_bits(offset), HI32_bits(offset), count, 0);
#elif defined(__arm__) || (defined(__riscv) && (__riscv_xlen == 32))
  return Syscall5_linux(NR_readahead_linux, fd, 0, LO32_bits(offset), HI32_bits(offset), count, 0);
#endif
}
long fallocate_linux(int fd, int mode, long long offset, long long len) {
#if defined(__x86_64__) || defined(__aarch64__) || (defined(__riscv) && (__riscv_xlen == 64))
  return Syscall4_linux(NR_fallocate_linux, fd, mode, offset, len, 0);
#else
  return Syscall6_linux(NR_fallocate_linux, fd, mode, LO32_bits(offset), HI32_bits(offset), LO32_bits(len), HI32_bits(len), 0);
#endif
}
// 5f. Flushing file data to storage
long sync_linux(void) {
  return Syscall0_linux(NR_sync_linux, 0);
}
long syncfs_linux(int fd) {
  return Syscall1_linux(NR_syncfs_linux, fd, 0);
}
long fsync_linux(unsigned int fd) {
  return Syscall1_linux(NR_fsync_linux, fd, 0);
}
long fdatasync_linux(unsigned int fd) {
  return Syscall1_linux(NR_fdatasync_linux, fd, 0);
}
long sync_file_range_linux(int fd, long long offset, long long nbytes, unsigned int flags) {
#if defined(__x86_64__) || defined(__aarch64__) || (defined(__riscv) && (__riscv_xlen == 64))
  return Syscall4_linux(NR_sync_file_range_linux, fd, offset, nbytes, flags, 0);
#elif defined(__i386__)
  return Syscall6_linux(NR_sync_file_range_linux, fd, LO32_bits(offset), HI32_bits(offset), LO32_bits(nbytes), HI32_bits(nbytes), flags, 0);
#elif defined(__arm__)
  return Syscall6_linux(NR_arm_sync_file_range_linux, fd, flags, LO32_bits(offset), HI32_bits(offset), LO32_bits(nbytes), HI32_bits(nbytes), 0);
#elif defined(__riscv) && (__riscv_xlen == 32)
  return Syscall6_linux(NR_sync_file_range_linux, fd, flags, LO32_bits(offset), HI32_bits(offset), LO32_bits(nbytes), HI32_bits(nbytes), 0);
#endif
}
// Disabled wrapper: long arm_sync_file_range_linux(int fd, long long offset, long long nbytes, unsigned int flags);
//
// 6. FILE DESCRIPTOR MANAGEMENT
//
// 6a. Duplicating and controlling file descriptors
long dup_linux(unsigned int fildes) {
  return Syscall1_linux(NR_dup_linux, fildes, 0);
}
// Disabled wrapper: long dup2_linux(unsigned int oldfd, unsigned int newfd);
long dup3_linux(unsigned int oldfd, unsigned int newfd, int flags) {
  return Syscall3_linux(NR_dup3_linux, oldfd, newfd, flags, 0);
}
// Disabled wrapper: long fcntl_linux(unsigned int fd, unsigned int cmd, unsigned long arg);
long fcntl64_linux(unsigned int fd, unsigned int cmd, unsigned long arg) {
#if defined(__x86_64__) || defined(__aarch64__) || (defined(__riscv) && (__riscv_xlen == 64))
  return Syscall3_linux(NR_fcntl_linux, fd, cmd, arg, 0);
#else
  return Syscall3_linux(NR_fcntl64_linux, fd, cmd, arg, 0);
#endif
}
// 6b. Device-specific control operations
long ioctl_linux(unsigned int fd, unsigned int cmd, unsigned long arg) {
  return Syscall3_linux(NR_ioctl_linux, fd, cmd, arg, 0);
}
// 6c. I/O Multiplexing
// Disabled wrapper: long select_linux(int n, fd_set_linux *inp, fd_set_linux *outp, fd_set_linux *exp, __kernel_old_timeval *tvp);
// Disabled wrapper: long _newselect_linux(int n, fd_set_linux *inp, fd_set_linux *outp, fd_set_linux *exp, __kernel_old_timeval *tvp);
// Disabled wrapper: pselect6_linux(int n, fd_set_linux *inp, fd_set_linux *outp, fd_set_linux *exp, __kernel_old_timespec_linux *tsp, void *sig);
long pselect6_time64_linux(int n, fd_set_linux *inp, fd_set_linux *outp, fd_set_linux *exp, __kernel_timespec_linux *tsp, void *sig) {
#if defined(__x86_64__) || defined(__aarch64__) || (defined(__riscv) && (__riscv_xlen == 64))
  return Syscall6_linux(NR_pselect6_linux, n, inp, outp, exp, tsp, sig, 0);
#else
  return Syscall6_linux(NR_pselect6_time64_linux, n, inp, outp, exp, tsp, sig, 0);
#endif
}
long poll_linux(pollfd_linux *ufds, unsigned int nfds, int timeout) {
  __kernel_timespec_linux ts;
  __kernel_timespec_linux *tsp = 0;
  if (timeout >= 0) {
    ts.tv_sec = timeout / 1000;
    ts.tv_nsec = (timeout % 1000) * 1000000;
    tsp = &ts;
  }
  return ppoll_time64_linux(ufds, nfds, tsp, 0, _NSIG_WORDS_linux * sizeof(unsigned long));
}
// Disabled wrapper: long ppoll_linux(pollfd_linux *, unsigned int, __kernel_old_timespec_linux *, const sigset_t_linux *, unsigned long);
long ppoll_time64_linux(pollfd_linux *ufds, unsigned int nfds, __kernel_timespec_linux *tsp, const sigset_t_linux *sigmask, unsigned long sigsetsize) {
#if defined(__x86_64__) || defined(__aarch64__) || (defined(__riscv) && (__riscv_xlen == 64))
  return Syscall5_linux(NR_ppoll_linux, ufds, nfds, tsp, sigmask, sigsetsize, 0);
#else
  return Syscall5_linux(NR_ppoll_time64_linux, ufds, nfds, tsp, sigmask, sigsetsize, 0);
#endif
}
// 6d. Scalable I/O event notification
// Disabled wrapper: long epoll_create_linux(int size);
long epoll_create1_linux(int flags) {
  return Syscall1_linux(NR_epoll_create1_linux, flags, 0);
}
long epoll_ctl_linux(int epfd, int op, int fd, epoll_event_linux *event) {
  return Syscall4_linux(NR_epoll_ctl_linux, epfd, op, fd, event, 0);
}
long epoll_wait_linux(int epfd, epoll_event_linux *events, int maxevents, int timeout) {
  return epoll_pwait_linux(epfd, events, maxevents, timeout, 0, 0);
}
long epoll_pwait_linux(int epfd, epoll_event_linux *events, int maxevents, int timeout, const sigset_t_linux *sigmask, unsigned long sigsetsize) {
  return Syscall6_linux(NR_epoll_pwait_linux, epfd, events, maxevents, timeout, sigmask, sigsetsize, 0);
}
long epoll_pwait2_linux(int epfd, epoll_event_linux *events, int maxevents, const __kernel_timespec_linux *timeout, const sigset_t_linux *sigmask, unsigned long sigsetsize) {
  return Syscall6_linux(NR_epoll_pwait2_linux, epfd, events, maxevents, timeout, sigmask, sigsetsize, 0);
}
// Disabled wrapper: long epoll_ctl_old_linux(int epfd, int op, int fd, epoll_event_linux *event);
// Disabled wrapper: long epoll_wait_old_linux(int epfd, epoll_event_linux *events, int maxevents, int timeout);
//
// 7. FILE METADATA
//
// 7a. Getting file attributes and status
// Disabled wrapper: long stat_linux(const char *filename, __old_kernel_stat *statbuf);
// Disabled wrapper: long fstat_linux(unsigned int fd, __old_kernel_stat *statbuf);
// Disabled wrapper: long lstat_linux(const char *filename, __old_kernel_stat *statbuf);
// Disabled wrapper: long stat64_linux(const char *filename, stat64_t_linux *statbuf);
// Disabled wrapper: long fstat64_linux(unsigned long fd, stat64_t_linux *statbuf);
// Disabled wrapper: long lstat64_linux(const char *filename, stat64_t_linux *statbuf);
// Disabled wrapper: long newfstatat_linux(int dfd, const char *filename, stat_t_linux *statbuf, int flag);
// Disabled wrapper: long fstatat64_linux(int dfd, const char *filename, stat64_t_linux *statbuf, int flag);
long statx_linux(int dfd, const char *path, unsigned flags, unsigned mask, statx_t_linux *buffer) {
  return Syscall5_linux(NR_statx_linux, dfd, path, flags, mask, buffer, 0);
}
// Disabled wrapper: long oldstat_linux(const char *filename, __old_kernel_stat *statbuf);
// Disabled wrapper: long oldfstat_linux(unsigned int fd, __old_kernel_stat *statbuf);
// Disabled wrapper: long oldlstat_linux(const char *filename, __old_kernel_stat *statbuf);
long file_getattr_linux(int dfd, const char *filename, file_attr_linux *attr, unsigned long usize, unsigned int at_flags) {
  return Syscall5_linux(NR_file_getattr_linux, dfd, filename, attr, usize, at_flags, 0);
}
// 7b. Changing file permissions and ownership
long chmod_linux(const char *filename, unsigned int mode) {
  return fchmodat_linux(AT_FDCWD_linux, filename, mode);
}
long fchmod_linux(unsigned int fd, unsigned int mode) {
  return Syscall2_linux(NR_fchmod_linux, fd, mode, 0);
}
long fchmodat_linux(int dfd, const char *filename, unsigned int mode) {
  return Syscall3_linux(NR_fchmodat_linux, dfd, filename, mode, 0);
}
long fchmodat2_linux(int dfd, const char *filename, unsigned int mode, unsigned int flags) {
  return Syscall4_linux(NR_fchmodat2_linux, dfd, filename, mode, flags, 0);
}
long umask_linux(int mask) {
  return Syscall1_linux(NR_umask_linux, mask, 0);
}
// Disabled wrapper: long chown_linux(const char *filename, unsigned int user, unsigned int group);
// Disabled wrapper: long fchown_linux(unsigned int fd, unsigned int user, unsigned int group);
// Disabled wrapper: long lchown_linux(const char *filename, unsigned int user, unsigned int group);
long chown32_linux(const char *filename, unsigned int user, unsigned int group) {
  return fchownat_linux(AT_FDCWD_linux, filename, user, group, 0);
}
long fchown32_linux(unsigned int fd, unsigned int user, unsigned int group) {
  return fchownat_linux(fd, "", user, group, AT_EMPTY_PATH_linux);
}
long lchown32_linux(const char *filename, unsigned int user, unsigned int group) {
  return fchownat_linux(AT_FDCWD_linux, filename, user, group, AT_SYMLINK_NOFOLLOW_linux);
}
long fchownat_linux(int dfd, const char *filename, unsigned int user, unsigned int group, int flag) {
  return Syscall5_linux(NR_fchownat_linux, dfd, filename, user, group, flag, 0);
}
long file_setattr_linux(int dfd, const char *filename, file_attr_linux *attr, unsigned long usize, unsigned int at_flags) {
  return Syscall5_linux(NR_file_setattr_linux, dfd, filename, attr, usize, at_flags, 0);
}
// 7c. File access and modification times
// Disabled wrapper: long utime_linux(char *filename, utimbuf_linux *times);
// Disabled wrapper: long utimes_linux(char *filename, __kernel_old_timeval *utimes);
// Disabled wrapper: long futimesat_linux(int dfd, const char *filename, __kernel_old_timeval *utimes);
// Disabled wrapper: long utimensat_linux(int dfd, const char *filename, __kernel_old_timespec_linux *utimes, int flags);
long utimensat_time64_linux(int dfd, const char *filename, __kernel_timespec_linux *t, int flags) {
#if defined(__x86_64__) || defined(__aarch64__) || (defined(__riscv) && (__riscv_xlen == 64))
  return Syscall4_linux(NR_utimensat_linux, dfd, filename, t, flags, 0);
#else
  return Syscall4_linux(NR_utimensat_time64_linux, dfd, filename, t, flags, 0);
#endif
}
// 7d. Testing file accessibility
long access_linux(const char *filename, int mode) {
  return faccessat_linux(AT_FDCWD_linux, filename, mode);
}
long faccessat_linux(int dfd, const char *filename, int mode) {
  return Syscall3_linux(NR_faccessat_linux, dfd, filename, mode, 0);
}
long faccessat2_linux(int dfd, const char *filename, int mode, int flags) {
  return Syscall4_linux(NR_faccessat2_linux, dfd, filename, mode, flags, 0);
}
// 7e. Getting, setting, and listing extended attributes
long setxattr_linux(const char *path, const char *name, const void *value, unsigned long size, int flags) {
  return Syscall5_linux(NR_setxattr_linux, path, name, value, size, flags, 0);
}
long lsetxattr_linux(const char *path, const char *name, const void *value, unsigned long size, int flags) {
  return Syscall5_linux(NR_lsetxattr_linux, path, name, value, size, flags, 0);
}
long fsetxattr_linux(int fd, const char *name, const void *value, unsigned long size, int flags) {
  return Syscall5_linux(NR_fsetxattr_linux, fd, name, value, size, flags, 0);
}
long setxattrat_linux(int dfd, const char *path, unsigned int at_flags, const char *name, const xattr_args_linux *args, unsigned long size) {
  return Syscall6_linux(NR_setxattrat_linux, dfd, path, at_flags, name, args, size, 0);
}
long getxattr_linux(const char *path, const char *name, void *value, unsigned long size) {
  return Syscall4_linux(NR_getxattr_linux, path, name, value, size, 0);
}
long lgetxattr_linux(const char *path, const char *name, void *value, unsigned long size) {
  return Syscall4_linux(NR_lgetxattr_linux, path, name, value, size, 0);
}
long fgetxattr_linux(int fd, const char *name, void *value, unsigned long size) {
  return Syscall4_linux(NR_fgetxattr_linux, fd, name, value, size, 0);
}
long getxattrat_linux(int dfd, const char *path, unsigned int at_flags, const char *name, xattr_args_linux *args, unsigned long size) {
  return Syscall6_linux(NR_getxattrat_linux, dfd, path, at_flags, name, args, size, 0);
}
long listxattr_linux(const char *path, char *list, unsigned long size) {
  return Syscall3_linux(NR_listxattr_linux, path, list, size, 0);
}
long llistxattr_linux(const char *path, char *list, unsigned long size) {
  return Syscall3_linux(NR_llistxattr_linux, path, list, size, 0);
}
long flistxattr_linux(int fd, char *list, unsigned long size) {
  return Syscall3_linux(NR_flistxattr_linux, fd, list, size, 0);
}
long listxattrat_linux(int dfd, const char *path, unsigned int at_flags, char *list, unsigned long size) {
  return Syscall5_linux(NR_listxattrat_linux, dfd, path, at_flags, list, size, 0);
}
long removexattr_linux(const char *path, const char *name) {
  return Syscall2_linux(NR_removexattr_linux, path, name, 0);
}
long lremovexattr_linux(const char *path, const char *name) {
  return Syscall2_linux(NR_lremovexattr_linux, path, name, 0);
}
long fremovexattr_linux(int fd, const char *name) {
  return Syscall2_linux(NR_fremovexattr_linux, fd, name, 0);
}
long removexattrat_linux(int dfd, const char *path, unsigned int at_flags, const char *name) {
  return Syscall4_linux(NR_removexattrat_linux, dfd, path, at_flags, name, 0);
}
// 7f. Advisory file locking
long flock_linux(unsigned int fd, unsigned int cmd) {
  return Syscall2_linux(NR_flock_linux, fd, cmd, 0);
}
#if 0 // WIP
//
// 8. DIRECTORY & NAMESPACE OPERATIONS
//
// 8a. Creating, removing, and reading directories
long mkdir_linux(const char *pathname, unsigned int mode) {
  return Syscall2_linux(NR_mkdir_linux, pathname, mode, 0);
}
long mkdirat_linux(int dfd, const char * pathname, unsigned int mode) {
  return Syscall3_linux(NR_mkdirat_linux, dfd, pathname, mode, 0);
}
long rmdir_linux(const char *pathname) {
  return Syscall1_linux(NR_rmdir_linux, pathname, 0);
}
long getdents_linux(unsigned int fd, linux_dirent *dirent, unsigned int count) {
  return Syscall3_linux(NR_getdents_linux, fd, dirent, count, 0);
}
long getdents64_linux(unsigned int fd, linux_dirent64 *dirent, unsigned int count) {
  return Syscall3_linux(NR_getdents64_linux, fd, dirent, count, 0);
}
long readdir_linux(unsigned int fd, old_linux_dirent *dirent, unsigned int count) {
  return Syscall3_linux(NR_readdir_linux, fd, dirent, count, 0);
}
// 8b. Getting and changing current directory
long getcwd_linux(char *buf, unsigned long size) {
  return Syscall2_linux(NR_getcwd_linux, buf, size, 0);
}
long chdir_linux(const char *filename) {
  return Syscall1_linux(NR_chdir_linux, filename, 0);
}
long fchdir_linux(unsigned int fd) {
  return Syscall1_linux(NR_fchdir_linux, fd, 0);
}
// 8c. Creating and managing hard and symbolic links
long link_linux(const char *oldname, const char *newname) {
  return Syscall2_linux(NR_link_linux, oldname, newname, 0);
}
long linkat_linux(int olddfd, const char *oldname, int newdfd, const char *newname, int flags) {
  return Syscall5_linux(NR_linkat_linux, olddfd, oldname, newdfd, newname, flags, 0);
}
long unlink_linux(const char *pathname) {
  return Syscall1_linux(NR_unlink_linux, pathname, 0);
}
long unlinkat_linux(int dfd, const char * pathname, int flag) {
  return Syscall3_linux(NR_unlinkat_linux, dfd, pathname, flag, 0);
}
long symlink_linux(const char *old, const char *new) {
  return Syscall2_linux(NR_symlink_linux, old, new, 0);
}
long symlinkat_linux(const char * oldname, int newdfd, const char * newname) {
  return Syscall3_linux(NR_symlinkat_linux, oldname, newdfd, newname, 0);
}
long readlink_linux(const char *path, char *buf, int bufsiz) {
  return Syscall3_linux(NR_readlink_linux, path, buf, bufsiz, 0);
}
long readlinkat_linux(int dfd, const char *path, char *buf, int bufsiz) {
  return Syscall4_linux(NR_readlinkat_linux, dfd, path, buf, bufsiz, 0);
}
long rename_linux(const char *oldname, const char *newname) {
  return Syscall2_linux(NR_rename_linux, oldname, newname, 0);
}
long renameat_linux(int olddfd, const char * oldname, int newdfd, const char * newname) {
  return Syscall4_linux(NR_renameat_linux, olddfd, oldname, newdfd, newname, 0);
}
long renameat2_linux(int olddfd, const char *oldname, int newdfd, const char *newname, unsigned int flags) {
  return Syscall5_linux(NR_renameat2_linux, olddfd, oldname, newdfd, newname, flags, 0);
}
// 8d. Creating device and named pipe nodes
long mknod_linux(const char *filename, unsigned int mode, unsigned dev) {
  return Syscall3_linux(NR_mknod_linux, filename, mode, dev, 0);
}
long mknodat_linux(int dfd, const char * filename, unsigned int mode, unsigned dev) {
  return Syscall4_linux(NR_mknodat_linux, dfd, filename, mode, dev, 0);
}
//
// 9. FILE SYSTEM OPERATIONS
//
// 9a. Mounting filesystems and changing root
long mount_linux(char *dev_name, char *dir_name, char *type, unsigned long flags, void *data) {
  return Syscall5_linux(NR_mount_linux, dev_name, dir_name, type, flags, data, 0);
}
long umount_linux(char *name, int flags) {
  return Syscall2_linux(NR_umount_linux, name, flags, 0);
}
long umount2_linux(char *name, int flags) {
  return Syscall2_linux(NR_umount2_linux, name, flags, 0);
}
long pivot_root_linux(const char *new_root, const char *put_old) {
  return Syscall2_linux(NR_pivot_root_linux, new_root, put_old, 0);
}
long chroot_linux(const char *filename) {
  return Syscall1_linux(NR_chroot_linux, filename, 0);
}
long mount_setattr_linux(int dfd, const char *path, unsigned int flags, mount_attr *uattr, unsigned long usize) {
  return Syscall5_linux(NR_mount_setattr_linux, dfd, path, flags, uattr, usize, 0);
}
long move_mount_linux(int from_dfd, const char *from_path, int to_dfd, const char *to_path, unsigned int ms_flags) {
  return Syscall5_linux(NR_move_mount_linux, from_dfd, from_path, to_dfd, to_path, ms_flags, 0);
}
long open_tree_linux(int dfd, const char *path, unsigned flags) {
  return Syscall3_linux(NR_open_tree_linux, dfd, path, flags, 0);
}
long open_tree_attr_linux(int dfd, const char *path, unsigned flags, mount_attr *uattr, unsigned long usize) {
  return Syscall5_linux(NR_open_tree_attr_linux, dfd, path, flags, uattr, usize, 0);
}
long fsconfig_linux(int fs_fd, unsigned int cmd, const char *key, const void *value, int aux) {
  return Syscall5_linux(NR_fsconfig_linux, fs_fd, cmd, key, value, aux, 0);
}
long fsmount_linux(int fs_fd, unsigned int flags, unsigned int ms_flags) {
  return Syscall3_linux(NR_fsmount_linux, fs_fd, flags, ms_flags, 0);
}
long fsopen_linux(const char *fs_name, unsigned int flags) {
  return Syscall2_linux(NR_fsopen_linux, fs_name, flags, 0);
}
long fspick_linux(int dfd, const char *path, unsigned int flags) {
  return Syscall3_linux(NR_fspick_linux, dfd, path, flags, 0);
}
// 9b. Getting filesystem statistics
long statfs_linux(const char * path, statfs *buf) {
  return Syscall2_linux(NR_statfs_linux, path, buf, 0);
}
long fstatfs_linux(unsigned int fd, statfs *buf) {
  return Syscall2_linux(NR_fstatfs_linux, fd, buf, 0);
}
long statfs64_linux(const char *path, unsigned long sz, statfs64 *buf) {
  return Syscall3_linux(NR_statfs64_linux, path, sz, buf, 0);
}
long fstatfs64_linux(unsigned int fd, unsigned long sz, statfs64 *buf) {
  return Syscall3_linux(NR_fstatfs64_linux, fd, sz, buf, 0);
}
long ustat_linux(unsigned dev, ustat *ubuf) {
  return Syscall2_linux(NR_ustat_linux, dev, ubuf, 0);
}
long statmount_linux(const mnt_id_req *req, statmount *buf, unsigned long bufsize, unsigned int flags) {
  return Syscall4_linux(NR_statmount_linux, req, buf, bufsize, flags, 0);
}
long listmount_linux(const mnt_id_req *req, u64 *mnt_ids, unsigned long nr_mnt_ids, unsigned int flags) {
  return Syscall4_linux(NR_listmount_linux, req, mnt_ids, nr_mnt_ids, flags, 0);
}
// 9c. Disk quota control
long quotactl_linux(unsigned int cmd, const char *special, qid_t id, void *addr) {
  return Syscall4_linux(NR_quotactl_linux, cmd, special, id, addr, 0);
}
long quotactl_fd_linux(unsigned int fd, unsigned int cmd, qid_t id, void *addr) {
  return Syscall4_linux(NR_quotactl_fd_linux, fd, cmd, id, addr, 0);
}
//
// 10. FILE SYSTEM MONITORING
//
// 10a. Monitoring filesystem events
long inotify_init_linux(void) {
  return Syscall0_linux(NR_inotify_init_linux, 0);
}
long inotify_init1_linux(int flags) {
  return Syscall1_linux(NR_inotify_init1_linux, flags, 0);
}
long inotify_add_watch_linux(int fd, const char *path, u32 mask) {
  return Syscall3_linux(NR_inotify_add_watch_linux, fd, path, mask, 0);
}
long inotify_rm_watch_linux(int fd, __s32 wd) {
  return Syscall2_linux(NR_inotify_rm_watch_linux, fd, wd, 0);
}
// 10b. Filesystem-wide event notification
long fanotify_init_linux(unsigned int flags, unsigned int event_f_flags) {
  return Syscall2_linux(NR_fanotify_init_linux, flags, event_f_flags, 0);
}
long fanotify_mark_linux(int fanotify_fd, unsigned int flags, u64 mask, int fd, const char *pathname) {
  return Syscall5_linux(NR_fanotify_mark_linux, fanotify_fd, flags, mask, fd, pathname, 0);
}
//
// 11. SIGNALS
//
// 11a. Setting up signal handlers
long signal_linux(int sig, __sighandler_t handler) {
  return Syscall2_linux(NR_signal_linux, sig, handler, 0);
}
long sigaction_linux(int sig, const old_sigaction *act, old_sigaction *oact) {
  return Syscall3_linux(NR_sigaction_linux, sig, act, oact, 0);
}
long rt_sigaction_linux(int sig, const sigaction *act, sigaction *oact, unsigned long sigsetsize) {
  return Syscall4_linux(NR_rt_sigaction_linux, sig, act, oact, sigsetsize, 0);
}
// 11b. Sending signals to processes
long kill_linux(int pid, int sig) {
  return Syscall2_linux(NR_kill_linux, pid, sig, 0);
}
long tkill_linux(int pid, int sig) {
  return Syscall2_linux(NR_tkill_linux, pid, sig, 0);
}
long tgkill_linux(int tgid, int pid, int sig) {
  return Syscall3_linux(NR_tgkill_linux, tgid, pid, sig, 0);
}
long rt_sigqueueinfo_linux(int pid, int sig, siginfo_t *uinfo) {
  return Syscall3_linux(NR_rt_sigqueueinfo_linux, pid, sig, uinfo, 0);
}
long rt_tgsigqueueinfo_linux(int tgid, int pid, int sig, siginfo_t *uinfo) {
  return Syscall4_linux(NR_rt_tgsigqueueinfo_linux, tgid, pid, sig, uinfo, 0);
}
// 11c. Blocking and unblocking signals
long sigprocmask_linux(int how, old_sigset_t *set, old_sigset_t *oset) {
  return Syscall3_linux(NR_sigprocmask_linux, how, set, oset, 0);
}
long rt_sigprocmask_linux(int how, sigset_t_linux *set, sigset_t_linux *oset, unsigned long sigsetsize) {
  return Syscall4_linux(NR_rt_sigprocmask_linux, how, set, oset, sigsetsize, 0);
}
long sgetmask_linux(void) {
  return Syscall0_linux(NR_sgetmask_linux, 0);
}
long ssetmask_linux(int newmask) {
  return Syscall1_linux(NR_ssetmask_linux, newmask, 0);
}
// 11d. Waiting for and querying signals
long sigpending_linux(old_sigset_t *uset) {
  return Syscall1_linux(NR_sigpending_linux, uset, 0);
}
long rt_sigpending_linux(sigset_t_linux *set, unsigned long sigsetsize) {
  return Syscall2_linux(NR_rt_sigpending_linux, set, sigsetsize, 0);
}
long sigsuspend_linux(old_sigset_t mask) {
  return Syscall1_linux(NR_sigsuspend_linux, mask, 0);
}
long rt_sigsuspend_linux(sigset_t_linux *unewset, unsigned long sigsetsize) {
  return Syscall2_linux(NR_rt_sigsuspend_linux, unewset, sigsetsize, 0);
}
long pause_linux(void) {
  return Syscall0_linux(NR_pause_linux, 0);
}
// Disabled wrapper: long rt_sigtimedwait_linux(const sigset_t_linux *uthese, siginfo_t *uinfo, const __kernel_old_timespec_linux *uts, unsigned long sigsetsize);
long rt_sigtimedwait_time64_linux(compat_sigset_t *uthese, compat_siginfo *uinfo, __kernel_timespec_linux *uts, compat_size_t sigsetsize) {
  return Syscall4_linux(NR_rt_sigtimedwait_time64_linux, uthese, uinfo, uts, sigsetsize, 0);
}
// 11e. Alternate signal stack and return from handlers
long sigaltstack_linux(const sigaltstack *uss, sigaltstack *uoss) {
  return Syscall2_linux(NR_sigaltstack_linux, uss, uoss, 0);
}
long sigreturn_linux(pt_regs *regs) {
  return Syscall1_linux(NR_sigreturn_linux, regs, 0);
}
long rt_sigreturn_linux(pt_regs *regs) {
  return Syscall1_linux(NR_rt_sigreturn_linux, regs, 0);
}
// 11f. Signal delivery via file descriptors
long signalfd_linux(int ufd, sigset_t_linux *user_mask, unsigned long sizemask) {
  return Syscall3_linux(NR_signalfd_linux, ufd, user_mask, sizemask, 0);
}
long signalfd4_linux(int ufd, sigset_t_linux *user_mask, unsigned long sizemask, int flags) {
  return Syscall4_linux(NR_signalfd4_linux, ufd, user_mask, sizemask, flags, 0);
}
//
// 12. PIPES & FIFOs
//
long pipe_linux(int *fildes) {
  return Syscall1_linux(NR_pipe_linux, fildes, 0);
}
long pipe2_linux(int *fildes, int flags) {
  return Syscall2_linux(NR_pipe2_linux, fildes, flags, 0);
}
//
// 13. INTER-PROCESS COMMUNICATION
//
// 13a. System V IPC - Shared Memory
long shmget_linux(key_t key, unsigned long size, int flag) {
  return Syscall3_linux(NR_shmget_linux, key, size, flag, 0);
}
long shmat_linux(int shmid, char *shmaddr, int shmflg) {
  return Syscall3_linux(NR_shmat_linux, shmid, shmaddr, shmflg, 0);
}
long shmdt_linux(char *shmaddr) {
  return Syscall1_linux(NR_shmdt_linux, shmaddr, 0);
}
long shmctl_linux(int shmid, int cmd, shmid_ds *buf) {
  return Syscall3_linux(NR_shmctl_linux, shmid, cmd, buf, 0);
}
// 13b. System V IPC - Message Queues
long msgget_linux(key_t key, int msgflg) {
  return Syscall2_linux(NR_msgget_linux, key, msgflg, 0);
}
long msgsnd_linux(int msqid, msgbuf *msgp, unsigned long msgsz, int msgflg) {
  return Syscall4_linux(NR_msgsnd_linux, msqid, msgp, msgsz, msgflg, 0);
}
long msgrcv_linux(int msqid, msgbuf *msgp, unsigned long msgsz, long msgtyp, int msgflg) {
  return Syscall5_linux(NR_msgrcv_linux, msqid, msgp, msgsz, msgtyp, msgflg, 0);
}
long msgctl_linux(int msqid, int cmd, msqid_ds *buf) {
  return Syscall3_linux(NR_msgctl_linux, msqid, cmd, buf, 0);
}
// 13c. System V IPC - Semaphores
long semget_linux(key_t key, int nsems, int semflg) {
  return Syscall3_linux(NR_semget_linux, key, nsems, semflg, 0);
}
long semop_linux(int semid, sembuf *sops, unsigned nsops) {
  return Syscall3_linux(NR_semop_linux, semid, sops, nsops, 0);
}
long semctl_linux(int semid, int semnum, int cmd, unsigned long arg) {
  return Syscall4_linux(NR_semctl_linux, semid, semnum, cmd, arg, 0);
}
// Disabled wrapper: long semtimedop_linux(int semid, sembuf *sops, unsigned nsops, const __kernel_old_timespec_linux *timeout);
long semtimedop_time64_linux(int semid, sembuf *tsops, unsigned int nsops, const __kernel_timespec_linux *timeout) {
  return Syscall4_linux(NR_semtimedop_time64_linux, semid, tsops, nsops, timeout, 0);
}
// 13d. POSIX Message Queues
long mq_open_linux(const char *name, int oflag, unsigned int mode, mq_attr *attr) {
  return Syscall4_linux(NR_mq_open_linux, name, oflag, mode, attr, 0);
}
long mq_unlink_linux(const char *name) {
  return Syscall1_linux(NR_mq_unlink_linux, name, 0);
}
// Disabled wrapper: long mq_timedsend_linux(mqd_t mqdes, const char *msg_ptr, unsigned long msg_len, unsigned int msg_prio, const __kernel_old_timespec_linux *abs_timeout);
long mq_timedsend_time64_linux(mqd_t mqdes, const char *u_msg_ptr, unsigned long msg_len, unsigned int msg_prio, const __kernel_timespec_linux *u_abs_timeout) {
  return Syscall5_linux(NR_mq_timedsend_time64_linux, mqdes, u_msg_ptr, msg_len, msg_prio, u_abs_timeout, 0);
}
// Disabled wrapper: long mq_timedreceive_linux(mqd_t mqdes, char *msg_ptr, unsigned long msg_len, unsigned int *msg_prio, const __kernel_old_timespec_linux *abs_timeout);
long mq_timedreceive_time64_linux(mqd_t mqdes, char *u_msg_ptr, unsigned long msg_len, unsigned int *u_msg_prio, const __kernel_timespec_linux *u_abs_timeout) {
  return Syscall5_linux(NR_mq_timedreceive_time64_linux, mqdes, u_msg_ptr, msg_len, u_msg_prio, u_abs_timeout, 0);
}
long mq_notify_linux(mqd_t mqdes, const sigevent *notification) {
  return Syscall2_linux(NR_mq_notify_linux, mqdes, notification, 0);
}
long mq_getsetattr_linux(mqd_t mqdes, const mq_attr *mqstat, mq_attr *omqstat) {
  return Syscall3_linux(NR_mq_getsetattr_linux, mqdes, mqstat, omqstat, 0);
}
// 13e. Synchronization Primitives - Futexes
// Disabled wrapper: long futex_linux(u32 *uaddr, int op, u32 val, const __kernel_old_timespec_linux *utime, u32 *uaddr2, u32 val3);
long futex_time64_linux(u32 *uaddr, int op, u32 val, const __kernel_timespec_linux *utime, u32 *uaddr2, u32 val3) {
  return Syscall6_linux(NR_futex_time64_linux, uaddr, op, val, utime, uaddr2, val3, 0);
}
long futex_wait_linux(void *uaddr, unsigned long val, unsigned long mask, unsigned int flags, __kernel_timespec_linux *timespec, clockid_t clockid) {
  return Syscall6_linux(NR_futex_wait_linux, uaddr, val, mask, flags, timespec, clockid, 0);
}
long futex_wake_linux(void *uaddr, unsigned long mask, int nr, unsigned int flags) {
  return Syscall4_linux(NR_futex_wake_linux, uaddr, mask, nr, flags, 0);
}
long futex_waitv_linux(futex_waitv *waiters, unsigned int nr_futexes, unsigned int flags, __kernel_timespec_linux *timeout, clockid_t clockid) {
  return Syscall5_linux(NR_futex_waitv_linux, waiters, nr_futexes, flags, timeout, clockid, 0);
}
long futex_requeue_linux(futex_waitv *waiters, unsigned int flags, int nr_wake, int nr_requeue) {
  return Syscall4_linux(NR_futex_requeue_linux, waiters, flags, nr_wake, nr_requeue, 0);
}
long set_robust_list_linux(robust_list_head *head, unsigned long len) {
  return Syscall2_linux(NR_set_robust_list_linux, head, len, 0);
}
long get_robust_list_linux(int pid, robust_list_head * *head_ptr, unsigned long *len_ptr) {
  return Syscall3_linux(NR_get_robust_list_linux, pid, head_ptr, len_ptr, 0);
}
// 13f. Synchronization Primitives - Event Notification
long eventfd_linux(unsigned int count) {
  return Syscall1_linux(NR_eventfd_linux, count, 0);
}
long eventfd2_linux(unsigned int count, int flags) {
  return Syscall2_linux(NR_eventfd2_linux, count, flags, 0);
}
//
// 14. SOCKETS & NETWORKING
//
// 14a. Creating and configuring sockets
long socket_linux(int family, int type, int protocol) {
  return Syscall3_linux(NR_socket_linux, family, type, protocol, 0);
}
long socketpair_linux(int family, int type, int protocol, int *usockvec) {
  return Syscall4_linux(NR_socketpair_linux, family, type, protocol, usockvec, 0);
}
long bind_linux(int fd, sockaddr_linux *umyaddr, int addrlen) {
  return Syscall3_linux(NR_bind_linux, fd, umyaddr, addrlen, 0);
}
long listen_linux(int fd, int backlog) {
  return Syscall2_linux(NR_listen_linux, fd, backlog, 0);
}
long accept_linux(int fd, sockaddr_linux *upeer_sockaddr, int *upeer_addrlen) {
  return Syscall3_linux(NR_accept_linux, fd, upeer_sockaddr, upeer_addrlen, 0);
}
long accept4_linux(int fd, sockaddr_linux *upeer_sockaddr, int *upeer_addrlen, int flags) {
  return Syscall4_linux(NR_accept4_linux, fd, upeer_sockaddr, upeer_addrlen, flags, 0);
}
long connect_linux(int fd, sockaddr_linux *uservaddr, int addrlen) {
  return Syscall3_linux(NR_connect_linux, fd, uservaddr, addrlen, 0);
}
long shutdown_linux(int fd, int how) {
  return Syscall2_linux(NR_shutdown_linux, fd, how, 0);
}
long socketcall_linux(int call, unsigned long *args) {
  return Syscall2_linux(NR_socketcall_linux, call, args, 0);
}
// 14b. Sending and receiving data on sockets
long send_linux(int fd, void *buff, unsigned long len, unsigned int flags) {
  return Syscall4_linux(NR_send_linux, fd, buff, len, flags, 0);
}
long sendto_linux(int fd, void *buff, unsigned long len, unsigned int flags, sockaddr_linux *addr, int addr_len) {
  return Syscall6_linux(NR_sendto_linux, fd, buff, len, flags, addr, addr_len, 0);
}
long sendmsg_linux(int fd, user_msghdr *msg, unsigned flags) {
  return Syscall3_linux(NR_sendmsg_linux, fd, msg, flags, 0);
}
long sendmmsg_linux(int fd, mmsghdr *msg, unsigned int vlen, unsigned flags) {
  return Syscall4_linux(NR_sendmmsg_linux, fd, msg, vlen, flags, 0);
}
long recv_linux(int fd, void *ubuf, unsigned long size, unsigned int flags) {
  return Syscall4_linux(NR_recv_linux, fd, ubuf, size, flags, 0);
}
long recvfrom_linux(int fd, void *ubuf, unsigned long size, unsigned int flags, sockaddr_linux *addr, int *addr_len) {
  return Syscall6_linux(NR_recvfrom_linux, fd, ubuf, size, flags, addr, addr_len, 0);
}
long recvmsg_linux(int fd, user_msghdr *msg, unsigned flags) {
  return Syscall3_linux(NR_recvmsg_linux, fd, msg, flags, 0);
}
// Disabled wrapper: long recvmmsg_linux(int fd, mmsghdr *msg, unsigned int vlen, unsigned flags, __kernel_old_timespec_linux *timeout);
long recvmmsg_time64_linux(int fd, mmsghdr *mmsg, unsigned int vlen, unsigned int flags, __kernel_timespec_linux *timeout) {
  return Syscall5_linux(NR_recvmmsg_time64_linux, fd, mmsg, vlen, flags, timeout, 0);
}
// 14c. Getting and setting socket options
long getsockopt_linux(int fd, int level, int optname, char *optval, int *optlen) {
  return Syscall5_linux(NR_getsockopt_linux, fd, level, optname, optval, optlen, 0);
}
long setsockopt_linux(int fd, int level, int optname, char *optval, int optlen) {
  return Syscall5_linux(NR_setsockopt_linux, fd, level, optname, optval, optlen, 0);
}
long getsockname_linux(int fd, sockaddr_linux *usockaddr, int *usockaddr_len) {
  return Syscall3_linux(NR_getsockname_linux, fd, usockaddr, usockaddr_len, 0);
}
long getpeername_linux(int fd, sockaddr_linux *usockaddr, int *usockaddr_len) {
  return Syscall3_linux(NR_getpeername_linux, fd, usockaddr, usockaddr_len, 0);
}
//
// 15. ASYNCHRONOUS I/O
//
// 15a. AIO: asynchronous I/O interface
long io_setup_linux(unsigned nr_reqs, aio_context_t *ctx) {
  return Syscall2_linux(NR_io_setup_linux, nr_reqs, ctx, 0);
}
long io_destroy_linux(aio_context_t ctx) {
  return Syscall1_linux(NR_io_destroy_linux, ctx, 0);
}
long io_submit_linux(aio_context_t ctx_id, long nr, iocb * *iocbpp) {
  return Syscall3_linux(NR_io_submit_linux, ctx_id, nr, iocbpp, 0);
}
long io_cancel_linux(aio_context_t ctx_id, iocb *iocb, io_event *result) {
  return Syscall3_linux(NR_io_cancel_linux, ctx_id, iocb, result, 0);
}
long io_getevents_linux(aio_context_t ctx_id, long min_nr, long nr, io_event *events, __kernel_timespec_linux *timeout) {
  return Syscall5_linux(NR_io_getevents_linux, ctx_id, min_nr, nr, events, timeout, 0);
}
// Disabled wrapper: long io_pgetevents_linux(aio_context_t ctx_id, long min_nr, long nr, io_event *events, __kernel_old_timespec_linux *timeout, const __aio_sigset *sig);
long io_pgetevents_time64_linux(aio_context_t ctx_id, long min_nr, long nr, io_event *events, __kernel_timespec_linux *timeout, const __aio_sigset *sig) {
  return Syscall6_linux(NR_io_pgetevents_time64_linux, ctx_id, min_nr, nr, events, timeout, sig, 0);
}
// 15b. io_uring: high-performance asynchronous I/O
long io_uring_setup_linux(u32 entries, io_uring_params *p) {
  return Syscall2_linux(NR_io_uring_setup_linux, entries, p, 0);
}
long io_uring_enter_linux(unsigned int fd, u32 to_submit, u32 min_complete, u32 flags, const void *argp, unsigned long argsz) {
  return Syscall6_linux(NR_io_uring_enter_linux, fd, to_submit, min_complete, flags, argp, argsz, 0);
}
long io_uring_register_linux(unsigned int fd, unsigned int op, void *arg, unsigned int nr_args) {
  return Syscall4_linux(NR_io_uring_register_linux, fd, op, arg, nr_args, 0);
}
//
// 16. TIME & CLOCKS
//
// 16a. Reading current time from various clocks
long time_linux(__kernel_old_time_t *tloc) {
  return Syscall1_linux(NR_time_linux, tloc, 0);
}
long gettimeofday_linux(__kernel_old_timeval *tv, timezone *tz) {
  return Syscall2_linux(NR_gettimeofday_linux, tv, tz, 0);
}
// Disabled wrapper: long clock_gettime_linux(clockid_t which_clock, __kernel_old_timespec_linux *tp);
long clock_gettime64_linux(clockid_t which_clock, __kernel_timespec_linux *tp) {
  return Syscall2_linux(NR_clock_gettime64_linux, which_clock, tp, 0);
}
// Disabled wrapper: long clock_getres_linux(clockid_t which_clock, __kernel_old_timespec_linux *tp);
long clock_getres_time64_linux(clockid_t which_clock, __kernel_timespec_linux *tp) {
  return Syscall2_linux(NR_clock_getres_time64_linux, which_clock, tp, 0);
}
// 16b. Setting system time and adjusting clocks
long settimeofday_linux(__kernel_old_timeval *tv, timezone *tz) {
  return Syscall2_linux(NR_settimeofday_linux, tv, tz, 0);
}
// Disabled wrapper: long clock_settime_linux(clockid_t which_clock, const __kernel_old_timespec_linux *tp);
long clock_settime64_linux(clockid_t which_clock, const __kernel_timespec_linux *tp) {
  return Syscall2_linux(NR_clock_settime64_linux, which_clock, tp, 0);
}
long stime_linux(__kernel_old_time_t *tptr) {
  return Syscall1_linux(NR_stime_linux, tptr, 0);
}
long adjtimex_linux(__kernel_timex *txc_p) {
  return Syscall1_linux(NR_adjtimex_linux, txc_p, 0);
}
long clock_adjtime_linux(clockid_t which_clock, __kernel_timex *tx) {
  return Syscall2_linux(NR_clock_adjtime_linux, which_clock, tx, 0);
}
long clock_adjtime64_linux(clockid_t which_clock, __kernel_timex *tx) {
  return Syscall2_linux(NR_clock_adjtime64_linux, which_clock, tx, 0);
}
// 16c. Suspending execution for a period of time
long nanosleep_linux(__kernel_timespec_linux *rqtp, __kernel_timespec_linux *rmtp) {
  return Syscall2_linux(NR_nanosleep_linux, rqtp, rmtp, 0);
}
// Disabled wrapper: long clock_nanosleep_linux(clockid_t which_clock, int flags, const __kernel_old_timespec_linux *rqtp, __kernel_old_timespec_linux *rmtp);
long clock_nanosleep_time64_linux(clockid_t which_clock, int flags, const __kernel_timespec_linux *rqtp, __kernel_timespec_linux *rmtp) {
  return Syscall4_linux(NR_clock_nanosleep_time64_linux, which_clock, flags, rqtp, rmtp, 0);
}
// 16d. Setting periodic or one-shot timers
long alarm_linux(unsigned int seconds) {
  return Syscall1_linux(NR_alarm_linux, seconds, 0);
}
long setitimer_linux(int which, __kernel_old_itimerval *value, __kernel_old_itimerval *ovalue) {
  return Syscall3_linux(NR_setitimer_linux, which, value, ovalue, 0);
}
long getitimer_linux(int which, __kernel_old_itimerval *value) {
  return Syscall2_linux(NR_getitimer_linux, which, value, 0);
}
// 16e. Per-process timers with precise control
long timer_create_linux(clockid_t which_clock, sigevent *timer_event_spec, timer_t * created_timer_id) {
  return Syscall3_linux(NR_timer_create_linux, which_clock, timer_event_spec, created_timer_id, 0);
}
// Disabled wrapper: long timer_settime_linux(timer_t timer_id, int flags, const __kernel_itimerspec *new_setting, __kernel_itimerspec *old_setting);
long timer_settime64_linux(timer_t timerid, int flags, const __kernel_timespec_linux *new_setting, __kernel_timespec_linux *old_setting) {
  return Syscall4_linux(NR_timer_settime64_linux, timerid, flags, new_setting, old_setting, 0);
}
// Disabled wrapper: long timer_gettime_linux(timer_t timer_id, __kernel_itimerspec *setting);
long timer_gettime64_linux(timer_t timerid, __kernel_timespec_linux *setting) {
  return Syscall2_linux(NR_timer_gettime64_linux, timerid, setting, 0);
}
long timer_getoverrun_linux(timer_t timer_id) {
  return Syscall1_linux(NR_timer_getoverrun_linux, timer_id, 0);
}
long timer_delete_linux(timer_t timer_id) {
  return Syscall1_linux(NR_timer_delete_linux, timer_id, 0);
}
// 16f. Timers accessible via file descriptors
long timerfd_create_linux(int clockid, int flags) {
  return Syscall2_linux(NR_timerfd_create_linux, clockid, flags, 0);
}
// Disabled wrapper: long timerfd_settime_linux(int ufd, int flags, const __kernel_itimerspec *utmr, __kernel_itimerspec *otmr);
long timerfd_settime64_linux(int ufd, int flags, const __kernel_timespec_linux *utmr, __kernel_timespec_linux *otmr) {
  return Syscall4_linux(NR_timerfd_settime64_linux, ufd, flags, utmr, otmr, 0);
}
// Disabled wrapper: long timerfd_gettime_linux(int ufd, __kernel_itimerspec *otmr);
long timerfd_gettime64_linux(int ufd, __kernel_timespec_linux *otmr) {
  return Syscall2_linux(NR_timerfd_gettime64_linux, ufd, otmr, 0);
}
//
// 17. RANDOM NUMBERS
//
long getrandom_linux(char *buf, unsigned long count, unsigned int flags) {
  return Syscall3_linux(NR_getrandom_linux, buf, count, flags, 0);
}
//
// 18. USER & GROUP IDENTITY
//
// 18a. Getting and setting user IDs
long getuid_linux(void) {
  return Syscall0_linux(NR_getuid_linux, 0);
}
long geteuid_linux(void) {
  return Syscall0_linux(NR_geteuid_linux, 0);
}
long setuid_linux(unsigned int uid) {
  return Syscall1_linux(NR_setuid_linux, uid, 0);
}
long setreuid_linux(unsigned int ruid, unsigned int euid) {
  return Syscall2_linux(NR_setreuid_linux, ruid, euid, 0);
}
long setresuid_linux(unsigned int ruid, unsigned int euid, unsigned int suid) {
  return Syscall3_linux(NR_setresuid_linux, ruid, euid, suid, 0);
}
long getresuid_linux(unsigned int *ruid, unsigned int *euid, unsigned int *suid) {
  return Syscall3_linux(NR_getresuid_linux, ruid, euid, suid, 0);
}
long setfsuid_linux(unsigned int uid) {
  return Syscall1_linux(NR_setfsuid_linux, uid, 0);
}
long getuid32_linux(void) {
  return Syscall0_linux(NR_getuid32_linux, 0);
}
long geteuid32_linux(void) {
  return Syscall0_linux(NR_geteuid32_linux, 0);
}
long setuid32_linux(unsigned int uid) {
  return Syscall1_linux(NR_setuid32_linux, uid, 0);
}
long setreuid32_linux(unsigned int ruid, unsigned int euid) {
  return Syscall2_linux(NR_setreuid32_linux, ruid, euid, 0);
}
long setresuid32_linux(unsigned int ruid, unsigned int euid, unsigned int suid) {
  return Syscall3_linux(NR_setresuid32_linux, ruid, euid, suid, 0);
}
long getresuid32_linux(unsigned int *ruid, unsigned int *euid, unsigned int *suid) {
  return Syscall3_linux(NR_getresuid32_linux, ruid, euid, suid, 0);
}
long setfsuid32_linux(unsigned int uid) {
  return Syscall1_linux(NR_setfsuid32_linux, uid, 0);
}
// 18b. Getting and setting group IDs
long getgid_linux(void) {
  return Syscall0_linux(NR_getgid_linux, 0);
}
long getegid_linux(void) {
  return Syscall0_linux(NR_getegid_linux, 0);
}
long setgid_linux(unsigned int gid) {
  return Syscall1_linux(NR_setgid_linux, gid, 0);
}
long setregid_linux(unsigned int rgid, unsigned int egid) {
  return Syscall2_linux(NR_setregid_linux, rgid, egid, 0);
}
long setresgid_linux(unsigned int rgid, unsigned int egid, unsigned int sgid) {
  return Syscall3_linux(NR_setresgid_linux, rgid, egid, sgid, 0);
}
long getresgid_linux(unsigned int *rgid, unsigned int *egid, unsigned int *sgid) {
  return Syscall3_linux(NR_getresgid_linux, rgid, egid, sgid, 0);
}
long setfsgid_linux(unsigned int gid) {
  return Syscall1_linux(NR_setfsgid_linux, gid, 0);
}
long getgid32_linux(void) {
  return Syscall0_linux(NR_getgid32_linux, 0);
}
long getegid32_linux(void) {
  return Syscall0_linux(NR_getegid32_linux, 0);
}
long setgid32_linux(unsigned int gid) {
  return Syscall1_linux(NR_setgid32_linux, gid, 0);
}
long setregid32_linux(unsigned int rgid, unsigned int egid) {
  return Syscall2_linux(NR_setregid32_linux, rgid, egid, 0);
}
long setresgid32_linux(unsigned int rgid, unsigned int egid, unsigned int sgid) {
  return Syscall3_linux(NR_setresgid32_linux, rgid, egid, sgid, 0);
}
long getresgid32_linux(unsigned int *rgid, unsigned int *egid, unsigned int *sgid) {
  return Syscall3_linux(NR_getresgid32_linux, rgid, egid, sgid, 0);
}
long setfsgid32_linux(unsigned int gid) {
  return Syscall1_linux(NR_setfsgid32_linux, gid, 0);
}
// 18c. Managing supplementary group list
long getgroups_linux(int gidsetsize, unsigned int *grouplist) {
  return Syscall2_linux(NR_getgroups_linux, gidsetsize, grouplist, 0);
}
long setgroups_linux(int gidsetsize, unsigned int *grouplist) {
  return Syscall2_linux(NR_setgroups_linux, gidsetsize, grouplist, 0);
}
long getgroups32_linux(int gidsetsize, unsigned int *grouplist) {
  return Syscall2_linux(NR_getgroups32_linux, gidsetsize, grouplist, 0);
}
long setgroups32_linux(int gidsetsize, unsigned int *grouplist) {
  return Syscall2_linux(NR_setgroups32_linux, gidsetsize, grouplist, 0);
}
//
// 19. CAPABILITIES & SECURITY
//
// 19a. Fine-grained privilege control
long capget_linux(cap_user_header_t header, cap_user_data_t dataptr) {
  return Syscall2_linux(NR_capget_linux, header, dataptr, 0);
}
long capset_linux(cap_user_header_t header, const cap_user_data_t data) {
  return Syscall2_linux(NR_capset_linux, header, data, 0);
}
// 19b. Syscall filtering and sandboxing
long seccomp_linux(unsigned int op, unsigned int flags, void *uargs) {
  return Syscall3_linux(NR_seccomp_linux, op, flags, uargs, 0);
}
// 19c. Linux Security Module interfaces
long security_linux(void) {
  return Syscall0_linux(NR_security_linux, 0);
}
long lsm_get_self_attr_linux(unsigned int attr, lsm_ctx *ctx, u32 *size, u32 flags) {
  return Syscall4_linux(NR_lsm_get_self_attr_linux, attr, ctx, size, flags, 0);
}
long lsm_set_self_attr_linux(unsigned int attr, lsm_ctx *ctx, u32 size, u32 flags) {
  return Syscall4_linux(NR_lsm_set_self_attr_linux, attr, ctx, size, flags, 0);
}
long lsm_list_modules_linux(u64 *ids, u32 *size, u32 flags) {
  return Syscall3_linux(NR_lsm_list_modules_linux, ids, size, flags, 0);
}
// 19d. Unprivileged access control
long landlock_create_ruleset_linux(const landlock_ruleset_attr *attr, unsigned long size, __u32 flags) {
  return Syscall3_linux(NR_landlock_create_ruleset_linux, attr, size, flags, 0);
}
long landlock_add_rule_linux(int ruleset_fd, enum landlock_rule_type rule_type, const void *rule_attr, __u32 flags) {
  return Syscall4_linux(NR_landlock_add_rule_linux, ruleset_fd, rule_type, rule_attr, flags, 0);
}
long landlock_restrict_self_linux(int ruleset_fd, __u32 flags) {
  return Syscall2_linux(NR_landlock_restrict_self_linux, ruleset_fd, flags, 0);
}
// 19e. Kernel key retention service
long add_key_linux(const char *_type, const char *_description, const void *_payload, unsigned long plen, key_serial_t destringid) {
  return Syscall5_linux(NR_add_key_linux, _type, _description, _payload, plen, destringid, 0);
}
long request_key_linux(const char *_type, const char *_description, const char *_callout_info, key_serial_t destringid) {
  return Syscall4_linux(NR_request_key_linux, _type, _description, _callout_info, destringid, 0);
}
long keyctl_linux(int cmd, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5) {
  return Syscall5_linux(NR_keyctl_linux, cmd, arg2, arg3, arg4, arg5, 0);
}
//
// 20. RESOURCE LIMITS & ACCOUNTING
//
// 20a. Getting and setting process resource limits
long getrlimit_linux(unsigned int resource, rlimit *rlim) {
  return Syscall2_linux(NR_getrlimit_linux, resource, rlim, 0);
}
long setrlimit_linux(unsigned int resource, rlimit *rlim) {
  return Syscall2_linux(NR_setrlimit_linux, resource, rlim, 0);
}
long prlimit64_linux(int pid, unsigned int resource, const rlimit64 *new_rlim, rlimit64 *old_rlim) {
  return Syscall4_linux(NR_prlimit64_linux, pid, resource, new_rlim, old_rlim, 0);
}
long ugetrlimit_linux(unsigned int resource, rlimit *rlim) {
  return Syscall2_linux(NR_ugetrlimit_linux, resource, rlim, 0);
}
long ulimit_linux(int cmd, long newval) {
  return Syscall2_linux(NR_ulimit_linux, cmd, newval, 0);
}
// 20b. Getting resource usage and time statistics
long getrusage_linux(int who, rusage_linux *ru) {
  return Syscall2_linux(NR_getrusage_linux, who, ru, 0);
}
long times_linux(tms *tbuf) {
  return Syscall1_linux(NR_times_linux, tbuf, 0);
}
// 20c. System-wide process accounting
long acct_linux(const char *name) {
  return Syscall1_linux(NR_acct_linux, name, 0);
}
//
// 21. NAMESPACES & CONTAINERS
//
long unshare_linux(unsigned long unshare_flags) {
  return Syscall1_linux(NR_unshare_linux, unshare_flags, 0);
}
long setns_linux(int fd, int nstype) {
  return Syscall2_linux(NR_setns_linux, fd, nstype, 0);
}
long listns_linux(const ns_id_req *req, u64 *ns_ids, unsigned long nr_ns_ids, unsigned int flags) {
  return Syscall4_linux(NR_listns_linux, req, ns_ids, nr_ns_ids, flags, 0);
}
//
// 22. PROCESS INSPECTION & CONTROL
//
// 22a. Process comparison
long kcmp_linux(int pid1, int pid2, int type, unsigned long idx1, unsigned long idx2) {
  return Syscall5_linux(NR_kcmp_linux, pid1, pid2, type, idx1, idx2, 0);
}
// 22b. Process file descriptors
long pidfd_open_linux(int pid, unsigned int flags) {
  return Syscall2_linux(NR_pidfd_open_linux, pid, flags, 0);
}
long pidfd_getfd_linux(int pidfd, int fd, unsigned int flags) {
  return Syscall3_linux(NR_pidfd_getfd_linux, pidfd, fd, flags, 0);
}
long pidfd_send_signal_linux(int pidfd, int sig, siginfo_t *info, unsigned int flags) {
  return Syscall4_linux(NR_pidfd_send_signal_linux, pidfd, sig, info, flags, 0);
}
// 22c. Process memory access
long process_vm_readv_linux(int pid, const iovec_linux *lvec, unsigned long liovcnt, const iovec_linux *rvec, unsigned long riovcnt, unsigned long flags) {
  return Syscall6_linux(NR_process_vm_readv_linux, pid, lvec, liovcnt, rvec, riovcnt, flags, 0);
}
long process_vm_writev_linux(int pid, const iovec_linux *lvec, unsigned long liovcnt, const iovec_linux *rvec, unsigned long riovcnt, unsigned long flags) {
  return Syscall6_linux(NR_process_vm_writev_linux, pid, lvec, liovcnt, rvec, riovcnt, flags, 0);
}
// 22d. Process tracing
long ptrace_linux(long request, long pid, unsigned long addr, unsigned long data) {
  return Syscall4_linux(NR_ptrace_linux, request, pid, addr, data, 0);
}
//
// 23. SYSTEM INFORMATION
//
// 23a. System name and domain information
long uname_linux(old_utsname *) {
  return Syscall1_linux(NR_uname_linux, , 0);
}
long olduname_linux(oldold_utsname *) {
  return Syscall1_linux(NR_olduname_linux, , 0);
}
long oldolduname_linux(oldold_utsname *name) {
  return Syscall1_linux(NR_oldolduname_linux, name, 0);
}
long gethostname_linux(char *name, int len) {
  return Syscall2_linux(NR_gethostname_linux, name, len, 0);
}
long sethostname_linux(char *name, int len) {
  return Syscall2_linux(NR_sethostname_linux, name, len, 0);
}
long setdomainname_linux(char *name, int len) {
  return Syscall2_linux(NR_setdomainname_linux, name, len, 0);
}
// 23b. Overall system information and statistics
long sysinfo_linux(sysinfo *info) {
  return Syscall1_linux(NR_sysinfo_linux, info, 0);
}
// 23c. Reading kernel log messages
long syslog_linux(int type, char *buf, int len) {
  return Syscall3_linux(NR_syslog_linux, type, buf, len, 0);
}
// 23d. Getting CPU and NUMA node information
long getcpu_linux(unsigned *cpu, unsigned *node, getcpu_cache *cache) {
  return Syscall3_linux(NR_getcpu_linux, cpu, node, cache, 0);
}
// 23e. Kernel filesystem information interface
long sysfs_linux(int option, unsigned long arg1, unsigned long arg2) {
  return Syscall3_linux(NR_sysfs_linux, option, arg1, arg2, 0);
}
//
// 24. KERNEL MODULES
//
long create_module_linux(const char *name, unsigned long size) {
  return Syscall2_linux(NR_create_module_linux, name, size, 0);
}
long init_module_linux(void *umod, unsigned long len, const char *uargs) {
  return Syscall3_linux(NR_init_module_linux, umod, len, uargs, 0);
}
long finit_module_linux(int fd, const char *uargs, int flags) {
  return Syscall3_linux(NR_finit_module_linux, fd, uargs, flags, 0);
}
long delete_module_linux(const char *name_user, unsigned int flags) {
  return Syscall2_linux(NR_delete_module_linux, name_user, flags, 0);
}
long query_module_linux(const char *name, int which, void *buf, unsigned long bufsize, unsigned long *ret) {
  return Syscall5_linux(NR_query_module_linux, name, which, buf, bufsize, ret, 0);
}
long get_kernel_syms_linux(kernel_sym *table) {
  return Syscall1_linux(NR_get_kernel_syms_linux, table, 0);
}
//
// 25. SYSTEM CONTROL & ADMINISTRATION
//
// 25a. Rebooting and shutting down the system
long reboot_linux(int magic1, int magic2, unsigned int cmd, void *arg) {
  return Syscall4_linux(NR_reboot_linux, magic1, magic2, cmd, arg, 0);
}
// 25b. Enabling and disabling swap areas
long swapon_linux(const char *specialfile, int swap_flags) {
  return Syscall2_linux(NR_swapon_linux, specialfile, swap_flags, 0);
}
long swapoff_linux(const char *specialfile) {
  return Syscall1_linux(NR_swapoff_linux, specialfile, 0);
}
// 25c. Loading and executing new kernels
long kexec_load_linux(unsigned long entry, unsigned long nr_segments, kexec_segment *segments, unsigned long flags) {
  return Syscall4_linux(NR_kexec_load_linux, entry, nr_segments, segments, flags, 0);
}
long kexec_file_load_linux(int kernel_fd, int initrd_fd, unsigned long cmdline_len, const char *cmdline_ptr, unsigned long flags) {
  return Syscall5_linux(NR_kexec_file_load_linux, kernel_fd, initrd_fd, cmdline_len, cmdline_ptr, flags, 0);
}
// 25d. Other system administration operations
long vhangup_linux(void) {
  return Syscall0_linux(NR_vhangup_linux, 0);
}
//
// 26. PERFORMANCE MONITORING & TRACING
//
// 26a. Hardware and software performance monitoring
long perf_event_open_linux(perf_event_attr *attr_uptr, int pid, int cpu, int group_fd, unsigned long flags) {
  return Syscall5_linux(NR_perf_event_open_linux, attr_uptr, pid, cpu, group_fd, flags, 0);
}
// 26b. Userspace dynamic tracing
long uprobe_linux(void) {
  return Syscall0_linux(NR_uprobe_linux, 0);
}
long uretprobe_linux(void) {
  return Syscall0_linux(NR_uretprobe_linux, 0);
}
// 26c. Programmable Kernel Extensions (eBPF)
long bpf_linux(int cmd, union bpf_attr *attr, unsigned int size) {
  return Syscall3_linux(NR_bpf_linux, cmd, attr, size, 0);
}
//
// 27. DEVICE & HARDWARE ACCESS
//
// 27a. Direct hardware I/O port access
long ioperm_linux(unsigned long from, unsigned long num, int on) {
  return Syscall3_linux(NR_ioperm_linux, from, num, on, 0);
}
long iopl_linux(unsigned int level) {
  return Syscall1_linux(NR_iopl_linux, level, 0);
}
// 27b. Setting I/O scheduling priority
long ioprio_set_linux(int which, int who, int ioprio) {
  return Syscall3_linux(NR_ioprio_set_linux, which, who, ioprio, 0);
}
long ioprio_get_linux(int which, int who) {
  return Syscall2_linux(NR_ioprio_get_linux, which, who, 0);
}
// 27c. PCI device configuration access
long pciconfig_read_linux(unsigned long bus, unsigned long dfn, unsigned long off, unsigned long len, void *buf) {
  return Syscall5_linux(NR_pciconfig_read_linux, bus, dfn, off, len, buf, 0);
}
long pciconfig_write_linux(unsigned long bus, unsigned long dfn, unsigned long off, unsigned long len, void *buf) {
  return Syscall5_linux(NR_pciconfig_write_linux, bus, dfn, off, len, buf, 0);
}
long pciconfig_iobase_linux(long which, unsigned long bus, unsigned long devfn) {
  return Syscall3_linux(NR_pciconfig_iobase_linux, which, bus, devfn, 0);
}
// 27d. CPU cache control operations
long cacheflush_linux(unsigned long start, unsigned long end, int flags) {
  return Syscall3_linux(NR_cacheflush_linux, start, end, flags, 0);
}
long cachestat_linux(unsigned int fd, cachestat_range *cstat_range, cachestat *cstat, unsigned int flags) {
  return Syscall4_linux(NR_cachestat_linux, fd, cstat_range, cstat, flags, 0);
}
//
// 28. ARCHITECTURE-SPECIFIC OPERATIONS
//
// 28a. x86 architecture operations
long arch_prctl_linux(int option, unsigned long addr) {
  return Syscall2_linux(NR_arch_prctl_linux, option, addr, 0);
}
long modify_ldt_linux(int func, void *ptr, unsigned long bytecount) {
  return Syscall3_linux(NR_modify_ldt_linux, func, ptr, bytecount, 0);
}
long set_thread_area_linux(user_desc *u_info) {
  return Syscall1_linux(NR_set_thread_area_linux, u_info, 0);
}
long get_thread_area_linux(user_desc *u_info) {
  return Syscall1_linux(NR_get_thread_area_linux, u_info, 0);
}
long vm86_linux(unsigned long cmd, unsigned long arg) {
  return Syscall2_linux(NR_vm86_linux, cmd, arg, 0);
}
long vm86old_linux(vm86_struct *user_vm86) {
  return Syscall1_linux(NR_vm86old_linux, user_vm86, 0);
}
// 28b. ARM architecture operations
long set_tls_linux(unsigned long val) {
  return Syscall1_linux(NR_set_tls_linux, val, 0);
}
long get_tls_linux(void) {
  return Syscall0_linux(NR_get_tls_linux, 0);
}
// 28c. RISC-V architecture operations
long riscv_flush_icache_linux(uintptr_t start, uintptr_t end, uintptr_t flags) {
  return Syscall3_linux(NR_riscv_flush_icache_linux, start, end, flags, 0);
}
long riscv_hwprobe_linux(riscv_hwprobe *pairs, unsigned long pair_count, unsigned long cpu_count, unsigned long *cpumask, unsigned int flags) {
  return Syscall5_linux(NR_riscv_hwprobe_linux, pairs, pair_count, cpu_count, cpumask, flags, 0);
}
// 28d. Intel MPX support (deprecated)
long mpx_linux(void) {
  return Syscall0_linux(NR_mpx_linux, 0);
}
//
// 29. ADVANCED EXECUTION CONTROL
//
// 29a. Restartable sequences
long rseq_linux(rseq *rseq, uint32_t rseq_len, int flags, uint32_t sig) {
  return Syscall4_linux(NR_rseq_linux, rseq, rseq_len, flags, sig, 0);
}
// 29b. Restart syscall
long restart_syscall_linux(void) {
  return Syscall0_linux(NR_restart_syscall_linux, 0);
}
// 29c. Directory entry cache
long lookup_dcookie_linux(u64 cookie64, char *buf, unsigned long len) {
  return Syscall3_linux(NR_lookup_dcookie_linux, cookie64, buf, len, 0);
}
//
// 30. LEGACY, OBSOLETE & UNIMPLEMENTED
//
long _sysctl_linux(__sysctl_args *args) {
  return Syscall1_linux(NR__sysctl_linux, args, 0);
}
long ipc_linux(unsigned int call, int first, unsigned long second, unsigned long third, void *ptr, long fifth) {
  return Syscall6_linux(NR_ipc_linux, call, first, second, third, ptr, fifth, 0);
}
long profil_linux(unsigned short *sample_buffer, unsigned long size, unsigned long offset, unsigned int scale) {
  return Syscall4_linux(NR_profil_linux, sample_buffer, size, offset, scale, 0);
}
long prof_linux(void) {
  return Syscall0_linux(NR_prof_linux, 0);
}
long afs_syscall_linux(void) {
  return Syscall0_linux(NR_afs_syscall_linux, 0);
}
long break_linux(void) {
  return Syscall0_linux(NR_break_linux, 0);
}
long ftime_linux(void) {
  return Syscall0_linux(NR_ftime_linux, 0);
}
long gtty_linux(void) {
  return Syscall0_linux(NR_gtty_linux, 0);
}
long idle_linux(void) {
  return Syscall0_linux(NR_idle_linux, 0);
}
long lock_linux(void) {
  return Syscall0_linux(NR_lock_linux, 0);
}
long nfsservctl_linux(int cmd, nfsctl_arg *arg, union nfsctl_res *res) {
  return Syscall3_linux(NR_nfsservctl_linux, cmd, arg, res, 0);
}
long getpmsg_linux(int fd, strbuf *ctlptr, strbuf *dataptr, int *bandp, int *flagsp) {
  return Syscall5_linux(NR_getpmsg_linux, fd, ctlptr, dataptr, bandp, flagsp, 0);
}
long putpmsg_linux(int fd, strbuf *ctlptr, strbuf *dataptr, int band, int flags) {
  return Syscall5_linux(NR_putpmsg_linux, fd, ctlptr, dataptr, band, flags, 0);
}
long stty_linux(void) {
  return Syscall0_linux(NR_stty_linux, 0);
}
long tuxcall_linux(void) {
  return Syscall0_linux(NR_tuxcall_linux, 0);
}
long vserver_linux(void) {
  return Syscall0_linux(NR_vserver_linux, 0);
}
long bdflush_linux(int func, long data) {
  return Syscall2_linux(NR_bdflush_linux, func, data, 0);
}
long uselib_linux(const char *library) {
  return Syscall1_linux(NR_uselib_linux, library, 0);
}
#endif // WIP

#ifndef C_BITS
  #undef LO32_bits
  #undef HI32_bits
#endif

#endif // C_LINUX_IMPLEMENTATION

