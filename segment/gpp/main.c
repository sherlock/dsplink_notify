Void LogCallback (Uint32 event, Pvoid arg, Pvoid info) 
{ 
  static char PrintBuffer[512]; 
  // get data from DSP: 
  int status = PROC_read(0, (int)info, 512, PrintBuffer); 
  if (status == DSP_SOK) 
    printf ("DSP-LOG: %s", PrintBuffer); 
  else 
    printf ("DEBUG: Unable to read dsp-mem %p\n", info); 
  // notify the DSP-side: 
  NOTIFY_notify (0, 0, 6, 0); 
} 

int main (int argc, char **args) 
{ 
  // add your dsplink init / LPM rest here: 
  // put this between PROC_load and PROC_start in your dsplink init-code: 
  NOTIFY_register (0, 0, 6, LogCallback, 0); 
  for (;;) {} 
  }
