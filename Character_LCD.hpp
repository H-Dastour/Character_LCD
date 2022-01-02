#pragma once
/**
  ******************************************************************************
  * @file    Character_LCD.hpp
  * @author  Hadi Dastour
  * @version 1.0
  * @brief   Header file of Character_LCD class.
  * @support Email: Hadi.Dastoor@gmail.com
  *			 Github: https://github.com/H-Dastour
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef Character_LCD_HPP
#define Character_LCD_HPP

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Macros --------------------------------------------------------------------*/
/* Constants -----------------------------------------------------------------*/

/************************** HD44780 Commands **************************/
#define CLEAR_DISPLAY	0x01
#define RETURN_HOME		0x02
#define LEFT_ALIGNMENT_CURSOR	0x04	//Cursor will be going to left when write a character ==> Entry Mode Set
#define RIGHT_ALIGNMENT_CURSOR	0x06	//Cursor will be going to rigth when write a character ==> Entry Mode Set

#define DISPLAY_OFF				0x08	//Turn the display off
#define CURSOR_OFF				0x0C	//Turn the display on and cursor is off
#define CURSOR_BLINK_ON			0x0F	//Turn the display on and cursor is blinking
#define CURSOR_BLINK_OFF		0x0E	//Turn the display on and cursor is constant

#define SHIFT_LEFT_LCD			0x18
#define SHIFT_RIGHT_LCD			0x1C
#define SHIFT_LEFT_CURSOR		0x10
#define SHIFT_RIGHT_CURSOR		0x14

#define SET_CG_RAM_ADDRESS		0x40
#define	SET_DD_RAM_ADDRESS		0x80

#define WATE_FOR_POWER_ON		50



/**
  * @brief  Character LCD Controller. Version 1
  */
namespace Character_LCD_Ver1
{
	/* Types ---------------------------------------------------------------------*/
	typedef struct
	{
		uint16_t GPIO_Pin;
		GPIO_TypeDef* GPIO_Port;
	}LCDGPIO_TypeDef;

	typedef struct
	{
		LCDGPIO_TypeDef* Data;
		LCDGPIO_TypeDef RS;
		LCDGPIO_TypeDef RW; 
		LCDGPIO_TypeDef Enable;
	}LCDPins_TypeDef;

	typedef enum
	{
		_8Bit_Interface = 0,
		_4Bit_Interface
	}LCDInterface_Typedef;

	typedef enum
	{
		Font5x10 = 0,
		Font5x7
	}LCDFont_Typedef;

	typedef enum
	{
		Display_1Line = 0,
		Display_2Line
	}LCDLine_Typedef;

	typedef struct
	{
		LCDInterface_Typedef LCDInterface;
		LCDFont_Typedef LCDFont;
		LCDLine_Typedef LCDLine;
	}LCDInit_TypeDef;

	typedef enum
	{
		Cursor_Off,
		Cursor_Blink_On,
		Cursor_Blink_Off,
	}Cursor_TypeDef;



	/* Class ---------------------------------------------------------------------*/
	class Character_LCD
	{
	public:
		Character_LCD(LCDPins_TypeDef* _pxLCD_Pins, LCDInit_TypeDef* _pxLCD_InitData);
		~Character_LCD(void);

		void LCD_TurnOn(Cursor_TypeDef _xCursor_Status);
		void LCD_TurnOff(void);
		void LCD_Clear(void);
		void LCD_PutChar(uint8_t _ucChar);
		void LCD_PutString(uint8_t* _pucString);

	private:
		LCDPins_TypeDef* pxLCD_Pins;
		LCDInit_TypeDef* pxLCD_InitData;

		void LCD_Init(void);
		void LCD_SendCommand(uint8_t _ucCommand);
		void LCD_SendData(uint8_t _ucData);
		void GPIO_SendData(uint8_t _ucData, LCDInterface_Typedef _xDataBus);
	};
}

#endif /* Character_LCD_HPP */
/*****************************END OF FILE*****************************/