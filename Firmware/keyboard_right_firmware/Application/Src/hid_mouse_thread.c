/*
 * hid_mouse_thread.c
 *
 *  Created on: May 19, 2024
 *      Author: bens1
 */

#include "usb_threads.h"


UX_SLAVE_CLASS_HID *hid_mouse;


/**
  * @brief  USBD_HID_Mouse_Activate
  *         This function is called when insertion of a HID Mouse device.
  * @param  hid_instance: Pointer to the hid class instance.
  * @retval none
  */
VOID USBD_HID_Mouse_Activate(VOID *hid_instance)
{
  /* USER CODE BEGIN USBD_HID_Mouse_Activate */

  /* Save the HID mouse instance */
  hid_mouse = (UX_SLAVE_CLASS_HID*) hid_instance;

  /* USER CODE END USBD_HID_Mouse_Activate */

  return;
}

/**
  * @brief  USBD_HID_Mouse_Deactivate
  *         This function is called when extraction of a HID Mouse device.
  * @param  hid_instance: Pointer to the hid class instance.
  * @retval none
  */
VOID USBD_HID_Mouse_Deactivate(VOID *hid_instance)
{
  /* USER CODE BEGIN USBD_HID_Mouse_Deactivate */
  UX_PARAMETER_NOT_USED(hid_instance);

  /* Reset the HID mouse instance */
  hid_mouse = UX_NULL;

  /* USER CODE END USBD_HID_Mouse_Deactivate */

  return;
}

/**
  * @brief  USBD_HID_Mouse_SetReport
  *         This function is invoked when the host sends a HID SET_REPORT
  *         to the application over Endpoint 0.
  * @param  hid_instance: Pointer to the hid class instance.
  * @param  hid_event: Pointer to structure of the hid event.
  * @retval status
  */
UINT USBD_HID_Mouse_SetReport(UX_SLAVE_CLASS_HID *hid_instance,
                              UX_SLAVE_CLASS_HID_EVENT *hid_event)
{
  UINT status = UX_SUCCESS;

  /* USER CODE BEGIN USBD_HID_Mouse_SetReport */
  UX_PARAMETER_NOT_USED(hid_instance);
  UX_PARAMETER_NOT_USED(hid_event);
  /* USER CODE END USBD_HID_Mouse_SetReport */

  return status;
}

/**
  * @brief  USBD_HID_Mouse_GetReport
  *         This function is invoked when host is requesting event through
  *         control GET_REPORT request.
  * @param  hid_instance: Pointer to the hid class instance.
  * @param  hid_event: Pointer to structure of the hid event.
  * @retval status
  */
UINT USBD_HID_Mouse_GetReport(UX_SLAVE_CLASS_HID *hid_instance,
                              UX_SLAVE_CLASS_HID_EVENT *hid_event)
{
  UINT status = UX_SUCCESS;

  /* USER CODE BEGIN USBD_HID_Mouse_GetReport */
  UX_PARAMETER_NOT_USED(hid_instance);
  UX_PARAMETER_NOT_USED(hid_event);
  /* USER CODE END USBD_HID_Mouse_GetReport */

  return status;
}


/**
  * @brief  Function implementing usbx_hid_mouse_thread_entry.
  * @param  thread_input: not used
  * @retval none
  */
void hid_mouse_thread(uint32_t thread_input)
{
  UX_SLAVE_DEVICE *device;
  UX_SLAVE_CLASS_HID_EVENT hid_event;

  TX_PARAMETER_NOT_USED(thread_input);

  /* Get the pointer to the device */
  device = &_ux_system_slave -> ux_system_slave_device;

  /* Reset the HID event structure */
  ux_utility_memory_set(&hid_event, 0, sizeof(UX_SLAVE_CLASS_HID_EVENT));

  while (1)
  {
    /* Check if the device state already configured */
    if ((device->ux_slave_device_state == UX_DEVICE_CONFIGURED) && (hid_mouse != UX_NULL))
    {
      /* Sleep for 10ms */
      tx_thread_sleep(MS_TO_TICK(10));

//      /* Get JoyState */
//      if (JoyState != JOY_NONE)
//      {
//        /* Get the new position */
//        GetPointerData(&hid_event);
//        /* Send hid event */
//        ux_device_class_hid_event_set(hid_mouse, &hid_event);
//      }
//      /* Reset JoyStick state */
//      JoyState = JOY_NONE;
    }
    else
    {
      /* Sleep thread for 10ms */
      tx_thread_sleep(MS_TO_TICK(10));
    }
  }
}

///**
//  * @brief  GetPointerData
//  *         Gets Pointer Data.
//  * @param  pbuf: Pointer to report
//  * @retval None
//  */
//static VOID GetPointerData(UX_SLAVE_CLASS_HID_EVENT *hid_event)
//{
//  int8_t x = 0, y = 0;
//
//  switch (JoyState)
//  {
//    case JOY_LEFT:
//      x -= CURSOR_STEP;
//      break;
//
//    case JOY_RIGHT:
//      x += CURSOR_STEP;
//      break;
//
//    case JOY_UP:
//      y -= CURSOR_STEP;
//      break;
//
//    case JOY_DOWN:
//      y += CURSOR_STEP;
//      break;
//
//    default:
//      break;
//  }
//
//  /* Mouse event. Length is fixed to 4 */
//  hid_event->ux_device_class_hid_event_length = 4;
//
//  /* Set select position */
//  hid_event->ux_device_class_hid_event_buffer[0] = 0;
//
//  /* Set X position */
//  hid_event->ux_device_class_hid_event_buffer[1] = x;
//
//  /* Set Y position */
//  hid_event->ux_device_class_hid_event_buffer[2] = y;
//
//  /* Set wheel position */
//  hid_event->ux_device_class_hid_event_buffer[3] = 0;
//}






