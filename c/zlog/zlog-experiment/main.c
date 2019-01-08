/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: orri
 *
 * Created on January 8, 2019, 5:07 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <zlog.h>

#define LOGGING_CONFIGURATION_FILE_NAME "zlog.conf"
#define LOGGING_CATEGORY "main"

/*
 * 
 */
int main(int argc, char** argv)
{
  int result;
  zlog_category_t* lc = 0;
  
  result = zlog_init(LOGGING_CONFIGURATION_FILE_NAME);
  if(result != 0) {
    fprintf(
      stderr,
      "Failed to initialize logging (%d)",
      errno);
    return EXIT_FAILURE;
  }
  
  lc = zlog_get_category(LOGGING_CATEGORY);
  if(!lc) {
    fprintf(
      stderr,
      "Failed to get category %s (%d)",
      LOGGING_CATEGORY,
      errno);
    zlog_fini();
    return EXIT_FAILURE;    
  }
  
  zlog_info(lc, "Simple zlog example");
  
  zlog_fini();
  
  return EXIT_SUCCESS;
}

