// need a semaphore for syncronization: 
static SEM_Handle dprint_sema; 
// buffer for data-transfer, L2 cache aligned. 
#pragma DATA_ALIGN(printf_buffer, 128); 
static char printf_buffer[512]; 
void dprintf (const char * format, ...) 
{ 
  int n; 
  va_list args; 
  va_start (args, format); 
  n = vsprintf (printf_buffer,format, args); 
  va_end (args); 
  if (n<=0) return; 
  // writeback cache: 
  BCACHE_wb (printf_buffer, n, 1); 
  // notify GPP: 
  NOTIFY_notify (ID_GPP, 0, 6, (Uint32)printf_buffer); 
  // wait for GPP acknowledge 
  SEM_pendBinary (dprint_sema, SYS_FOREVER); 
} 

Void dprint_callback (Uint32 EventNo, Ptr arg, Ptr a_Info) 
{ 
  // acknowledge gpp is done with reading the print-buffer: 
  SEM_postBinary (dprint_sema); 
} 

static int TaskMain() 
{ 
  dprintf ("Hello world from dsp-side\n"); 
  // your app goes here.. 
  for (;;) {} 
} 

Void main(Int argc, Char *argv[]) 
{ 
  // create the semaphore 
  dprint_sema = SEM_create (0, NULL); 
  // connect with gpp: 
  DSPLINK_init (); 
  // register callback from the gpp side: 
  NOTIFY_register (ID_GPP, 0, 6, dprint_callback, 0); 
  // start task: 
  TSK_create(TaskMain, NULL, &0); 
  // task will start as soon as main ends! 
  // don't call dprintf from here! 
} 

