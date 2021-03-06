/***************************************************************************
                          can_setup.c  -  description
                             -------------------
    begin             : 24.12.2011
    copyright         : Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
    author            : Saravanan
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software, you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License           *
 *   version 2.1 as published by the Free Software Foundation.             *
 *                                                                         *
 ***************************************************************************/

/**
    Library to talk to Tiny-CAN devices. You find the latest versions at
       http://www.tiny-can.com/
**/
#include <windows.h>
#include <basetyps.h>
#include <commctrl.h>
#include <windowsx.h>
#include "global.h"
#include "mhs_types.h"
#include "resource.h"
#include "util.h"
#include "gui_util.h"
#include "mhsbmcfg.h"
#include "can_setup.h"



static struct TMhsCanCfg *CanCfg = NULL;


#define CanSpeedTabSize 9
static const char *CanSpeedTabStr[] = {"10 kBit/s", "20 kBit/s", "50 kBit/s", "100 kBit/s",
                          "125 kBit/s", "250 kBit/s", "500 kBit/s", "800 kBit/s",
                          "1 MBit/s"};
static const uint32_t CanSpeedTab[] = {10, 20, 50, 100, 125, 250, 500, 800, 1000};

static void InitCanSetupDlg(HWND hdlg, struct TMhsCanCfg *cfg)
{
	SetDlgItemText(hdlg, IDC_SNR_EDIT, cfg->CanSnrStr);
	SetDlgItemHex(hdlg, IDC_BTR_EDIT, HEX_WORD, cfg->CanBtrValue);

	FillComboBox(hdlg, IDC_CAN_SPEED, CanSpeedTabStr, CanSpeedTab,
				 CanSpeedTabSize, cfg->CanSpeed);

	// Set default value as bitrate
	CheckRadioButton(hDlg, IDC_RADIOBTN_BITRATE, IDC_RADIOBTN_BTR, IDC_RADIOBTN_BITRATE);
	EnableDisableControls(hDlg, IDC_RADIOBTN_BITRATE);
}


static void SaveCanSetup(HWND hdlg, struct TMhsCanCfg *cfg)
{
GetDlgItemTextCpy(cfg->CanSnrStr, hdlg, IDC_SNR_EDIT, MHS_STR_SIZE);
cfg->CanBtrValue = GetDlgItemHex(hdlg, IDC_BTR_EDIT);
cfg->CanSpeed = GetComboBox(hdlg, IDC_CAN_SPEED);
}

static void EnableDisableControls(HWND hdlg, int nDlgItem)
{	
	HWND hWndBitRate = GetDlgItem(hdlg, IDC_RADIOBTN_BITRATE);
	HWND hWndBTR = GetDlgItem(hdlg, IDC_RADIOBTN_BTR);
	
	if (nDlgItem != IDC_RADIOBTN_BITRATE)
	{
		EnableWindow(IDC_CAN_SPEED, TRUE);	// Enable the bit rate combo box
		EnableWindow(IDC_BTR_EDIT, FALSE);	// Disable the BTR edit box
	}
	else
	{
		EnableWindow(IDC_CAN_SPEED, FALSE);	// Disable the bit rate combo box
		EnableWindow(IDC_BTR_EDIT, TRUE);	// Enable the BTR edit box
	}
}

static BOOL CALLBACK CanSetupDlgProc(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
switch(uMessage)
  {
  case WM_INITDIALOG : {
                       InitCanSetupDlg(hdlg, CanCfg);
                       break;
                       }
  case WM_COMMAND    : {
                       switch (LOWORD(wparam))
                         {
                         case IDOK     : {
                                         SaveCanSetup(hdlg, CanCfg);
                                         EndDialog(hdlg, TRUE);
                                         return(TRUE);
                                         }
                         case IDCANCEL : {
                                         EndDialog(hdlg, TRUE);
                                         return(TRUE);
                                         }
						 case BN_CLICKED: {
										  EnableDisableControls(hdlg, lparam);
										  }
                         }
                       break;
                       }
  }
return(FALSE);
}


int ShowCanSetupDlg(HINSTANCE hInstance, HWND hwnd, struct TMhsCanCfg *cfg)
{
CanCfg = cfg;
if (((DWORD)DialogBox(hInstance, MAKEINTRESOURCE(IDD_CAN_SETUP), hwnd, CanSetupDlgProc)) == TRUE)
  return(1);
else
  return(0);
}