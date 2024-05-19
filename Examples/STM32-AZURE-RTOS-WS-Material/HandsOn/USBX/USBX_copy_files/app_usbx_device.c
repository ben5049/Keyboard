/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_usbx_device.c
  * @author  MCD Application Team
  * @brief   USBX Device applicative file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_usbx_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usb_otg.h"
#include "ux_api.h"
#include "ux_system.h"
#include "ux_utility.h"
#include "ux_device_stack.h"
#include "ux_dcd_stm32.h"
#include "ux_device_descriptors.h"
#include "ux_device_mouse.h"
#include "ux_device_cdc_acm.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* Define constants.  */
#define USBX_APP_STACK_SIZE                          1024
#define USBX_MEMORY_SIZE                             (15104)

TX_THREAD    ux_app_thread;
TX_THREAD    ux_hid_thread;

TX_THREAD               ux_cdc_thread;

/* CDC Class Calling Parameter structure */
UX_SLAVE_CLASS_CDC_ACM_PARAMETER    cdc_acm_parameter;

UX_SLAVE_CLASS_HID_PARAMETER hid_parameter;
UX_SLAVE_CLASS_HID_EVENT     mouse_hid_event;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void  usbx_app_thread_entry(ULONG arg);
/* USER CODE END PFP */
/**
  * @brief  Application USBX Device Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT MX_USBX_Device_Init(VOID *memory_ptr)
{
  UINT ret = UX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;
  /* USER CODE BEGIN App_USBX_Device_Init */

  CHAR *pointer;
  ULONG device_framework_hs_length;
  ULONG device_framework_fs_length;
  ULONG string_framework_length;
  ULONG languge_id_framework_length;
  UCHAR *device_framework_high_speed;
  UCHAR *device_framework_full_speed;
  UCHAR *string_framework;
  UCHAR *language_id_framework;

  /* Allocate the stack for thread 0.  */
  ret =  tx_byte_allocate(byte_pool, (VOID **) &pointer, USBX_MEMORY_SIZE, TX_NO_WAIT);
  if (ret != UX_SUCCESS)
  {
    Error_Handler();
    return ret;
  }
  /* Initialize USBX Memory */
  ux_system_initialize(pointer, USBX_MEMORY_SIZE, UX_NULL, 0);

  /* Get_Device_Framework_High_Speed and get the length */
  device_framework_high_speed = USBD_Get_Device_Framework_Speed(USBD_HIGH_SPEED,
                                &device_framework_hs_length);

  /* Get_Device_Framework_Full_Speed and get the length */
  device_framework_full_speed = USBD_Get_Device_Framework_Speed(USBD_FULL_SPEED,
                                &device_framework_fs_length);

  /* Get_String_Framework and get the length */
  string_framework = USBD_Get_String_Framework(&string_framework_length);

  /* Get_Language_Id_Framework and get the length */
  language_id_framework = USBD_Get_Language_Id_Framework(&languge_id_framework_length);

  /* The code below is required for installing the device portion of USBX.
  In this application */
  ret =  ux_device_stack_initialize(device_framework_high_speed,
                                     device_framework_hs_length,
                                     device_framework_full_speed,
                                     device_framework_fs_length,
                                     string_framework,
                                     string_framework_length,
                                     language_id_framework,
                                     languge_id_framework_length, UX_NULL);

  /* Check the Stack initialize status */
  if (ret != UX_SUCCESS)
  {
    Error_Handler();
    return ret;
  }

  /* Initialize the hid class parameters for the device. */
  hid_parameter.ux_device_class_hid_parameter_report_address = USBD_Get_Device_HID_MOUSE_ReportDesc();

  hid_parameter.ux_device_class_hid_parameter_report_length = USBD_HID_MOUSE_REPORT_DESC_SIZE;

  hid_parameter.ux_device_class_hid_parameter_report_id = UX_TRUE;
  hid_parameter.ux_device_class_hid_parameter_callback = app_usbx_device_thread_hid_callback;

  /* Initialize the device hid class. The class is connected with interface 0 */
  ret = ux_device_stack_class_register(_ux_system_slave_class_hid_name,
                                       ux_device_class_hid_entry, 1, 0, (VOID *)&hid_parameter);

  /* Check the device stack class status */
  if (ret != UX_SUCCESS)
  {
    Error_Handler();
    return ret;
  }

  /* Put system definition stuff in here, e.g. thread creates and other assorted
  create information.  */

  /* Allocate the stack for main_usbx_app_thread_entry.  */
  ret = tx_byte_allocate(byte_pool, (VOID **) &pointer, USBX_APP_STACK_SIZE, TX_NO_WAIT);
  if (UX_SUCCESS != ret)
        {
          Error_Handler();
          return ret;
        }

  /* Create the main thread.  */
  ret = tx_thread_create(&ux_app_thread, "main_usbx_app_thread_entry", usbx_app_thread_entry, 0,
                   pointer, USBX_APP_STACK_SIZE, 20, 20, 1, TX_AUTO_START);
  if (UX_SUCCESS != ret)
        {
          Error_Handler();
          return ret;
        }

  /* Allocate the stack for hid_usbx_app_thread_entry.  */
  ret = tx_byte_allocate(byte_pool, (VOID **) &pointer, USBX_APP_STACK_SIZE, TX_NO_WAIT);
  if (UX_SUCCESS != ret)
        {
          Error_Handler();
          return ret;
        }
  /* Create threads 1 and 2. These threads pass information through a ThreadX
  message queue.  It is also interesting to note that these threads have a time
  slice.  */
  ret = tx_thread_create(&ux_hid_thread, "hid_usbx_app_thread_entry", usbx_hid_thread_entry, 1,
                   pointer, USBX_APP_STACK_SIZE, 20, 20, 1, TX_AUTO_START);
  if (UX_SUCCESS != ret)
        {
          Error_Handler();
          return ret;
        }

  /*------------------------------------------------------------------------------------------------*/
  /* Initialize the cdc class parameters for the device. */
  cdc_acm_parameter.ux_slave_class_cdc_acm_instance_activate = CDC_Init_FS;

  /* Deinitialize the cdc class parameters for the device. */
  cdc_acm_parameter.ux_slave_class_cdc_acm_instance_deactivate = CDC_DeInit_FS;

  /* Manage the CDC class requests */
  cdc_acm_parameter.ux_slave_class_cdc_acm_parameter_change = ux_app_parameters_change;

  /* registers a slave class to the slave stack. The class is connected with
     interface 1 */
  ret = ux_device_stack_class_register(_ux_system_slave_class_cdc_acm_name,
                                       ux_device_class_cdc_acm_entry, 1, 1,
                                       (VOID *)&cdc_acm_parameter);

  /* Check the device stack class status */
  if (ret != UX_SUCCESS)
  {
    Error_Handler();
    return ret;
  }

    /* Allocate the stack for usbx_cdc_acm_read_thread_entry. */
  ret = tx_byte_allocate(byte_pool, (VOID **) &pointer,
                                   USBX_APP_STACK_SIZE, TX_NO_WAIT);

      /* Check memory allocation */
      if (UX_SUCCESS != ret)
      {
        Error_Handler();
        return ret;
      }

      /* Create the usbx_cdc_acm_thread_entry thread. */
      ret = tx_thread_create(&ux_cdc_thread, "cdc_acm_usbx_app_thread_entry",
    		  	  	  	  	  	  usbx_cdc_acm_thread_entry, 2,
                                   pointer, USBX_APP_STACK_SIZE, 22, 22, TX_NO_TIME_SLICE,
                                   TX_AUTO_START);

      /* Check usbx_cdc_acm_read_thread_entry creation */
      if (UX_SUCCESS != ret)
      {
        Error_Handler();
        return ret;
      }
  /* USER CODE END App_USBX_Device_Init */

  return ret;
}

/* USER CODE BEGIN 1 */
/**
  * @brief  Function implementing usbx_app_thread_entry.
  * @param arg: Not used
  * @retval None
  */
void usbx_app_thread_entry(ULONG arg)
{
  /* Sleep for 100 ms */
  tx_thread_sleep(0.1 * TX_TIMER_TICKS_PER_SECOND);

  /* Initialization of USB device */
  MX_USB_Device_Init();
}

/**
  * @brief MX_USB_Device_Init
  *        Initialization of USB device.
  * Init USB device Library, add supported class and start the library
  * @retval None
  */
void MX_USB_Device_Init(void)
{
  /* USER CODE BEGIN USB_Device_Init_PreTreatment_0 */
  /* USER CODE END USB_Device_Init_PreTreatment_0 */
	UINT ret = UX_SUCCESS;

  HAL_PWREx_EnableUSBVoltageDetector();

  MX_USB_OTG_HS_PCD_Init();

  /* USER CODE BEGIN USB_Device_Init_PreTreatment_1 */
  HAL_PCDEx_SetRxFiFo(&hpcd_USB_OTG_HS, 0x200);
  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_HS, 0, 0x40);
  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_HS, 1, 0x40);
  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_HS, 2, 0x40);
  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_HS, 3, 0x40);
  /* USER CODE END USB_Device_Init_PreTreatment_1 */

  /* initialize the device controller driver*/
  ret = _ux_dcd_stm32_initialize((ULONG)USB_OTG_HS, (ULONG)&hpcd_USB_OTG_HS);
  if (UX_SUCCESS != ret)
        {
          Error_Handler();

        }

  HAL_PCD_Start(&hpcd_USB_OTG_HS);

  /* USER CODE BEGIN USB_Device_Init_PostTreatment */
  /* USER CODE END USB_Device_Init_PostTreatment */
}

/* USER CODE END 1 */
