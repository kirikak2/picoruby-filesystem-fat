#ifndef FAT_DEFINED_H_
#define FAT_DEFINED_H_

#if defined(PICORB_VM_MRUBY)
  #include <mruby.h>
#elif defined(PICORB_VM_MRUBYC)
  #include <mrubyc.h>
#endif

#include "../lib/ff14b/source/ff.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

/* Sector size of the volume fs is on (512 for an SD card). */
static inline int
FAT_volume_sector_size(const FATFS *fs)
{
#if FF_MAX_SS == FF_MIN_SS
  (void)fs;
  return FF_MAX_SS;
#else
  return fs->ssize;
#endif
}

#if defined(PICORB_VM_MRUBY)

mrb_value mrb_fat__exist_p(mrb_state *mrb, mrb_value klass);
mrb_value mrb_fat__unlink(mrb_state *mrb, mrb_value klass);
mrb_value mrb_fat__rename(mrb_state *mrb, mrb_value klass);

typedef struct prb_vfs_methods
{
  mrb_value (*file_new)(mrb_state *mrb, mrb_value self);
  mrb_value (*file_close)(mrb_state *mrb, mrb_value self);
  mrb_value (*file_read)(mrb_state *mrb, mrb_value self);
  mrb_value (*file_write)(mrb_state *mrb, mrb_value self);
  mrb_value (*file_seek)(mrb_state *mrb, mrb_value self);
  mrb_value (*file_tell)(mrb_state *mrb, mrb_value self);
  mrb_value (*file_size)(mrb_state *mrb, mrb_value self);
  mrb_value (*file_fsync)(mrb_state *mrb, mrb_value self);
  mrb_value (*file_exist_q)(mrb_state *mrb, mrb_value self);
  mrb_value (*file_unlink)(mrb_state *mrb, mrb_value self);
  mrb_value (*file_stat)(mrb_state *mrb, mrb_value self);
} prb_vfs_methods;

void mrb_raise_iff_f_error(mrb_state *mrb, FRESULT res, const char *func);
void mrb_init_class_FAT_File(mrb_state *mrb, struct RClass *class_FAT);
void mrb_init_class_FAT_Dir(mrb_state *mrb, struct RClass *class_FAT);

#elif defined(PICORB_VM_MRUBYC)

void c_fat__exist_q(mrbc_vm *vm, mrbc_value v[], int argc);
void c_fat__unlink(mrbc_vm *vm, mrbc_value v[], int argc);
void c_fat__rename(mrbc_vm *vm, mrbc_value v[], int argc);

typedef struct prb_vfs_methods
{
  void (*file_new)(mrbc_vm *vm, mrbc_value *v, int argc);
  void (*file_close)(mrbc_vm *vm, mrbc_value *v, int argc);
  void (*file_read)(mrbc_vm *vm, mrbc_value *v, int argc);
  void (*file_write)(mrbc_vm *vm, mrbc_value *v, int argc);
  void (*file_seek)(mrbc_vm *vm, mrbc_value *v, int argc);
  void (*file_tell)(mrbc_vm *vm, mrbc_value *v, int argc);
  void (*file_size)(mrbc_vm *vm, mrbc_value *v, int argc);
  void (*file_fsync)(mrbc_vm *vm, mrbc_value *v, int argc);
  void (*file_exist_q)(mrbc_vm *vm, mrbc_value *v, int argc);
  void (*file_unlink)(mrbc_vm *vm, mrbc_value *v, int argc);
  void (*file_stat)(mrbc_vm *vm, mrbc_value *v, int argc);
} prb_vfs_methods;

void mrbc_raise_iff_f_error(mrbc_vm *vm, FRESULT res, const char *func);
void mrbc_init_class_FAT_File(mrbc_vm *vm, mrbc_class *class_FAT);
void mrbc_init_class_FAT_Dir(mrbc_vm *vm, mrbc_class *class_FAT);

#endif /* PICORB_VM_MRUBYC */

#ifdef USE_FAT_SD_DISK
int FAT_set_spi_unit(const char* name, int sck, int cipo, int copi, int cs);
int FAT_set_sdmmc_pins(int clk, int cmd, int d0, int slot, int freq_khz);
#endif

#ifdef __cplusplus
}
#endif

#endif /* FAT_DEFINED_H_ */

