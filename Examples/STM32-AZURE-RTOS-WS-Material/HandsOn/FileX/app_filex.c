/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_filex.c
  * @author  MCD Application Team
  * @brief   FileX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_filex.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEFAULT_SECTOR_SIZE              512
#define DEFAULT_STACK_SIZE               (2 * 1024)
/* Thread_0 priority */
#define DEFAULT_THREAD_PRIO              10
/* Thread_0 preemption priority */
#define DEFAULT_PREEMPTION_THRESHOLD     DEFAULT_THREAD_PRIO
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
UCHAR            *media_memory;
/* Define FileX global data structures.  */
FX_MEDIA        ram_disk;
FX_FILE         fx_file;
/* Define ThreadX global data structures.  */
TX_THREAD       fx_thread;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void fx_thread_entry(ULONG thread_input);
void Error_Handler(void);
/* USER CODE END PFP */

/**
  * @brief  Application FileX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT MX_FileX_Init(VOID *memory_ptr)
{
  UINT ret = FX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN MX_FileX_MEM_POOL */
  (void)byte_pool;
  /* USER CODE END MX_FileX_MEM_POOL */

  /* USER CODE BEGIN MX_FileX_Init */
  VOID *pointer;

  /* Allocate memory for the FileX thread's stack */
  ret = tx_byte_allocate(byte_pool, &pointer, DEFAULT_STACK_SIZE, TX_NO_WAIT);

  if (ret != FX_SUCCESS)
  {
    /* Failed at allocating memory */
    Error_Handler();
  }

  /* Create the main thread.  */
  tx_thread_create(&fx_thread, "thread 0", fx_thread_entry, 0, pointer, DEFAULT_STACK_SIZE, DEFAULT_THREAD_PRIO,
		  DEFAULT_THREAD_PRIO, TX_NO_TIME_SLICE, TX_AUTO_START);

  /* Allocate memory for the media cache */
  ret = tx_byte_allocate(byte_pool, (VOID**) &media_memory, DEFAULT_SECTOR_SIZE, TX_NO_WAIT);

  if (ret != FX_SUCCESS)
  {
    /* Failed at allocating memory */
    Error_Handler();
  }

  /* Initialize FileX.  */
  fx_system_initialize();
  /* USER CODE END MX_FileX_Init */
  return ret;
}

/* USER CODE BEGIN 1 */
void fx_thread_entry(ULONG thread_input)
{
  UINT status;
  ULONG bytes_read;
  ULONG available_space_post;
  CHAR read_buffer[32];
  CHAR data[] = "This is FileX working on STM32\n";

  /* Format the NOR flash as FAT */
  status =  fx_media_format(
		  &ram_disk,
		  fx_stm32_sram_driver,       // Driver entry
		  0,   						  // Device info pointer
		  (UCHAR *) media_memory,     // Media buffer pointer
		  DEFAULT_SECTOR_SIZE,	      // Media buffer size
		  "RAM_DISK",             	  // Volume Name
		  1,                          // Number of FATs
		  32,                         // Directory Entries
		  0,                          // Hidden sectors
		  64,						  // Total sectors
		  DEFAULT_SECTOR_SIZE,        // Sector size
		  8,                          // Sectors per cluster
		  1,                          // Heads
		  1                           // Sectors per track
  );

  /* Check if the format status */
  if (status != FX_SUCCESS)
  {
    Error_Handler();
  }

  /* Open the RAM disk.  */
  status =  fx_media_open(&ram_disk, "RAM DISK", fx_stm32_sram_driver, 0, (VOID *) media_memory, DEFAULT_SECTOR_SIZE);

  /* Check the media open status.  */
  if (status != FX_SUCCESS)
  {
    Error_Handler();
  }

  /* Create a file called STM32.TXT in the root directory.  */
  status =  fx_file_create(&ram_disk, "FXTEST.TXT");

  /* Check the create status.  */
  if (status != FX_SUCCESS)
  {
    /* Check for an already created status. This is expected on the
    second pass of this loop!  */
    if (status != FX_ALREADY_CREATED)
    {
      /* Create error, call error handler.  */
      Error_Handler();
    }
  }

  /* Open the test file.  */
  status =  fx_file_open(&ram_disk, &fx_file, "FXTEST.TXT", FX_OPEN_FOR_WRITE);

  /* Check the file open status.  */
  if (status != FX_SUCCESS)
  {
    /* Error opening file, call error handler.  */
    Error_Handler();
  }

  /* Seek to the beginning of the test file.  */
  status =  fx_file_seek(&fx_file, 0);

  /* Check the file seek status.  */
  if (status != FX_SUCCESS)
  {
    /* Error performing file seek, call error handler.  */
    Error_Handler();
  }

  /* Write a string to the test file.  */
  status =  fx_file_write(&fx_file, data, sizeof(data));

  /* Check the file write status.  */
  if (status != FX_SUCCESS)
  {
    /* Error writing to a file, call error handler.  */
    Error_Handler();
  }

  /* Close the test file.  */
  status =  fx_file_close(&fx_file);

  /* Check the file close status.  */
  if (status != FX_SUCCESS)
  {
    /* Error closing the file, call error handler.  */
    Error_Handler();
  }

   /* Open the test file.  */
  status =  fx_file_open(&ram_disk, &fx_file, "FXTEST.TXT", FX_OPEN_FOR_READ);

  /* Check the file open status.  */
  if (status != FX_SUCCESS)
  {
    /* Error opening file, call error handler.  */
    Error_Handler();
  }

  /* Seek to the beginning of the test file.  */
  status =  fx_file_seek(&fx_file, 0);

  /* Check the file seek status.  */
  if (status != FX_SUCCESS)
  {
    /* Error performing file seek, call error handler.  */
    Error_Handler();
  }

  /* Read the first 28 bytes of the test file.  */
  status =  fx_file_read(&fx_file, read_buffer, sizeof(data), &bytes_read);

  /* Check the file read status.  */
  if ((status != FX_SUCCESS) || (bytes_read != sizeof(data)))
  {
    /* Error reading file, call error handler.  */
    Error_Handler();
   }

  /* Close the test file.  */
  status =  fx_file_close(&fx_file);

  /* Check the file close status.  */
  if (status != FX_SUCCESS)
  {
    /* Error closing the file, call error handler.  */
    Error_Handler();
  }

  /* Get the available usable space, after the file has been created */
  status =  fx_media_space_available(&ram_disk, &available_space_post);


  /* Check the get available state request status.  */
  if (status != FX_SUCCESS)
  {
    Error_Handler();
  }

  /* Close the media.  */
  status =  fx_media_close(&ram_disk);

  /* Check the media close status.  */
  if (status != FX_SUCCESS)
  {
    /* Error closing the media, call error handler.  */
    Error_Handler();
  }
}
/* USER CODE END 1 */
