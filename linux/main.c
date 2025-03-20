/*
 * main.c - Entry point for linux_camera
 * Handles communication with ATmega and manages recording lifecycle accordingly.
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <string.h>
 #include <signal.h>
 #include "comms.h"
 #include "record.h"
 
 /*
  * cleanup - Handles SIGINT (Ctrl+C), ensuring recording is stopped before exit.
  * @note: This is mainly used for debugging purposes. In a real-world application,
  * recording should be stopped gracefully before a shutdown is initiated. 
  * @signo: Signal number.
  */
 static void cleanup(int signo)
 {
     (void) signo; /* Suppress unused parameter warning */
     
     printf("\nReceived SIGINT. Stopping recording and cleaning up...\n");
     end_record();
     printf("Cleanup complete. Exiting.\n");
 
     exit(0);
 }
 
 int main(void)
 {
     /* Register SIGINT handler */
     signal(SIGINT, cleanup);
 
     /* Initialize communication */
     comms_init();
     printf("Waiting for record commands from ATmega...\n");
 
     struct Message msg;
     recording_params_t params = {
         .shutter = 5000,
         .awb = "incandescent",
         .lens_position = 4.0,
         .bitrate = 20000000,
         .resolution = "1920x1080",
         .fps = 30,
         .gain = 1.0,
         .level = "4.2"
     };
 
     /* Main loop: process commands */
     while (1) {
         if (comms_receive_message(&msg)) {
             if (msg.message_type == COMMAND_RECORD_REQ_START) {
                 printf("Received RECORD START command!\n");
                 start_record(params);
             } else if (msg.message_type == COMMAND_RECORD_REQ_END) {
                 printf("Received RECORD STOP command!\n");
                 end_record();
             }
         }
         usleep(50000);  /* Sleep for 50ms  */
     }
 
     return 0;
 }
 