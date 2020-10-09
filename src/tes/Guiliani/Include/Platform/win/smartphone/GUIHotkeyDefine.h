/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __GUI_HOTKEY_DEFINE__H__
#define __GUI_HOTKEY_DEFINE__H__

/** This file defines the hotkey keycodes as used by
    the input wrapper (as of 19-Oct-2006 this is only
    used on the Smartphone).
*/

// Is we are running on the PC we need different hotkeys
// compared to the real device. This is defined via
// the define PC_EMULATION

// #define PC_EMULATION

const eC_UInt cuiCursorLeft     = 0x25;
const eC_UInt cuiCursorUp       = 0x26;
const eC_UInt cuiCursorRight    = 0x27;
const eC_UInt cuiCursorDown     = 0x28;
const eC_UInt cuiCursorPressed  = 0x0d;

#if defined PC_EMULATION

const eC_UInt cuiNumpadKey_Star = '*';
const eC_UInt cuiNumpadKey_Hash = '#';

const eC_UInt cuiLeftHotkey     = ',';
const eC_UInt cuiRightHotkey    = '.';

const eC_UInt cuiHome           = '!';
const eC_UInt cuiBack           = '@';
const eC_UInt cuiRecord         = '#';
const eC_UInt cuiVolumeUp       = '$';
const eC_UInt cuiVolumeDown     = '$';
const eC_UInt cuiGreen          = '%';
const eC_UInt cuiRed            = '^';

const eC_UInt cuiNumpadKey_0    = '0';
const eC_UInt cuiNumpadKey_1    = '1';
const eC_UInt cuiNumpadKey_2    = '2';
const eC_UInt cuiNumpadKey_3    = '3';
const eC_UInt cuiNumpadKey_4    = '4';
const eC_UInt cuiNumpadKey_5    = '5';
const eC_UInt cuiNumpadKey_6    = '6';
const eC_UInt cuiNumpadKey_7    = '7';
const eC_UInt cuiNumpadKey_8    = '8';
const eC_UInt cuiNumpadKey_9    = '9';

#else

const eC_UInt cuiNumpadKey_Star = 0xff00;
const eC_UInt cuiNumpadKey_Hash = 0xff01;

const eC_UInt cuiLeftHotkey     = 0xff02;
const eC_UInt cuiRightHotkey    = 0xff03;

const eC_UInt cuiHome           = 0xff04;
const eC_UInt cuiBack           = 0xff05;
const eC_UInt cuiRecord         = 0xff06;
const eC_UInt cuiVolumeUp       = 0xff07;
const eC_UInt cuiVolumeDown     = 0xff08;
const eC_UInt cuiGreen          = 0xff09;
const eC_UInt cuiRed            = 0xff0a;

const eC_UInt cuiNumpadKey_0    = 0xff0a;
const eC_UInt cuiNumpadKey_1    = 0xff0b;
const eC_UInt cuiNumpadKey_2    = 0xff0c;
const eC_UInt cuiNumpadKey_3    = 0xff0d;
const eC_UInt cuiNumpadKey_4    = 0xff0e;
const eC_UInt cuiNumpadKey_5    = 0xff0f;
const eC_UInt cuiNumpadKey_6    = 0xff10;
const eC_UInt cuiNumpadKey_7    = 0xff11;
const eC_UInt cuiNumpadKey_8    = 0xff12;
const eC_UInt cuiNumpadKey_9    = 0xff13;

#endif // PC_EMULATION

#endif // __GUI_HOTKEY_DEFINE__H__
