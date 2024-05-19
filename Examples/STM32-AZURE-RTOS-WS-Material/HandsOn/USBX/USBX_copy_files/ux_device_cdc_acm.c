/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ux_device_cdc_acm.c
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
#include "ux_device_cdc_acm.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "app_usbx_device.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define APP_RX_DATA_SIZE                          2048
#define APP_TX_DATA_SIZE                          2048

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
#if defined ( __ICCARM__ ) /* IAR Compiler */
#pragma location = 0x24028000
#elif defined ( __CC_ARM ) /* MDK ARM Compiler */
__attribute__((section(".UsbxAppSection")))
#elif defined ( __GNUC__ ) /* GNU Compiler */
__attribute__((section(".UsbxAppSection")))
#endif

/* Data received over uart are stored in this buffer */
uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

/* Data to send over USB CDC are stored in this buffer   */
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

UX_SLAVE_CLASS_CDC_ACM_LINE_CODING_PARAMETER CDC_VCP_LineCoding =
{
  115200, /* baud rate */
  0x00,   /* stop bits-1 */
  0x00,   /* parity - none */
  0x08    /* nb. of bits 8 */
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
extern void MX_USART3_UART_Init(void);
extern void Error_Handler(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
  * @brief  Initializes the CDC media low layer over the FS USB IP
  * @param  cdc Instance
  * @retval none
  */
void CDC_Init_FS(void *cdc_acm)
{
  /* Status */
  UINT ux_status = UX_SUCCESS;

  /* USER CODE BEGIN 3 */
  /* Set device_class_cdc_acm with default parameters */
  ux_status = ux_device_class_cdc_acm_ioctl(cdc_acm,
                                            UX_SLAVE_CLASS_CDC_ACM_IOCTL_SET_LINE_CODING,
                                            &CDC_VCP_LineCoding);
  /* Check Status */
  if (ux_status != UX_SUCCESS)
  {
    Error_Handler();
  }
  /* USER CODE END 3 */
}

/**
  * @brief  DeInitializes the CDC media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
void CDC_DeInit_FS(void *cdc_acm)
{
  /* USER CODE BEGIN 4 */

  /* USER CODE END 4 */
}

/**
  * @brief  Manage the CDC class requests
  * @param  cdc Instance
  * @retval none
  */
VOID ux_app_parameters_change(VOID *cdc_acm)
{
  UX_SLAVE_TRANSFER *transfer_request;
  UX_SLAVE_DEVICE   *device;
  ULONG             request;
  UINT ux_status = UX_SUCCESS;

  /* Get the pointer to the device.  */
  device = &_ux_system_slave -> ux_system_slave_device;

  /* Get the pointer to the transfer request associated with the control endpoint. */
  transfer_request = &device -> ux_slave_device_control_endpoint.
                     ux_slave_endpoint_transfer_request;

  /* Extract all necessary fields of the request. */
  request = *(transfer_request -> ux_slave_transfer_request_setup + UX_SETUP_REQUEST);

  /* Here we proceed only the standard request we know of at the device level.  */
  switch (request)
  {
    /* Set Line Coding Command */
    case UX_SLAVE_CLASS_CDC_ACM_SET_LINE_CODING :
    {
      /* Get the Line Coding parameters */
      ux_status = ux_device_class_cdc_acm_ioctl(cdc_acm,
                                                UX_SLAVE_CLASS_CDC_ACM_IOCTL_GET_LINE_CODING,
                                                &CDC_VCP_LineCoding);
      /* Check Status */
      if (ux_status != UX_SUCCESS)
      {
        Error_Handler();
      }
      break;
    }

    /* Get Line Coding Command */
    case UX_SLAVE_CLASS_CDC_ACM_GET_LINE_CODING :
    {
      ux_status = ux_device_class_cdc_acm_ioctl(cdc_acm,
                                                UX_SLAVE_CLASS_CDC_ACM_IOCTL_SET_LINE_CODING,
                                                &CDC_VCP_LineCoding);

      /* Check Status */
      if (ux_status != UX_SUCCESS)
      {
        Error_Handler();
      }
      break;
    }

    /* Set the the control line state */
    case UX_SLAVE_CLASS_CDC_ACM_SET_CONTROL_LINE_STATE :
    default :
      break;
  }
}

/**
  * @brief  Function implementing usbx_cdc_acm_thread_entry.
  * @param arg: Not used
  * @retval None
  */
void usbx_cdc_acm_thread_entry(ULONG arg)
{
  UX_SLAVE_DEVICE *device;
  UX_SLAVE_INTERFACE *data_interface;
  UX_SLAVE_CLASS_CDC_ACM *cdc_acm;
  ULONG actual_length;
  ULONG actual_length_write;
  ULONG ux_status = UX_SUCCESS;

  /* Get device */
  device = &_ux_system_slave->ux_system_slave_device;

  while (1)
  {
    /* Get Data interface */


    /* Check if device is configured */
    if (device->ux_slave_device_state == UX_DEVICE_CONFIGURED)
    {
    	data_interface = device->ux_slave_device_first_interface[1].ux_slave_interface_next_interface;
      /* Compares two memory blocks ux_slave_class_name and _ux_system_slave_class_cdc_acm_name */
      ux_status = ux_utility_memory_compare(data_interface->ux_slave_interface_class->ux_slave_class_name,
                                            _ux_system_slave_class_cdc_acm_name,
                                            ux_utility_string_length_get(_ux_system_slave_class_cdc_acm_name));

      /* Check Compares success */
      if (ux_status == UX_SUCCESS)
      {
        cdc_acm =  data_interface->ux_slave_interface_class_instance;

        /* Set transmission_status to UX_FALSE for the first time */
        cdc_acm -> ux_slave_class_cdc_acm_transmission_status = UX_FALSE;

        /* Read the received data in blocking mode */
        ux_device_class_cdc_acm_read(cdc_acm, (UCHAR *)UserRxBufferFS, 64,
                                     &actual_length);
        if (actual_length != 0)
        {
        	ux_status = ux_device_class_cdc_acm_write(cdc_acm,
        	                                                (UCHAR *)(&UserRxBufferFS[0]),
															actual_length, &actual_length_write);

        }
      }
    }
    else
    {
      tx_thread_sleep(1);
    }
  }
}

/* USER CODE END 0 */

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
