/**
  ******************************************************************************
  * @file    Character_LCD.cpp
  * @author  Hadi Dastour
  * @version 1.0
  * @brief   Character LCD controller class.
  *          This file provides firmware functions to manage a Character LCD
  * @support Email: Hadi.Dastoor@gmail.com
  *			 Github: https://github.com/H-Dastour
  ******************************************************************************
  * Attention: This class is for character LCDs with HD44780 LCD driver
**/

/* Includes ------------------------------------------------------------------*/
#include "Character_LCD.hpp"

/**
  * @brief  Character LCD Controller. Version 1
  */
namespace Character_LCD_Ver1
{
	/* Variables -----------------------------------------------------------------*/

	/* Functions Definitions -----------------------------------------------------*/
	/**
	  * @brief  Constructor function
	  * @param  _pxLCD_Pins: Enter the display pins
	  *			_pxLCD_InitData: Enter the display initialization data
	  * @retval None
	  */
	Character_LCD::Character_LCD(LCDPins_TypeDef* _pxLCD_Pins, LCDInit_TypeDef* _pxLCD_InitData)
	{
		this->pxLCD_Pins = _pxLCD_Pins;
		this->pxLCD_InitData = _pxLCD_InitData;

		this->LCD_Init();
	}

	/**
	  * @brief  Destructor function
	  * @param  None
	  * @retval None
	  */
	Character_LCD::~Character_LCD(void)
	{

	}

	/**
	  * @brief  This function Initialize the LCD according to the pxLCD_Pins & pxLCD_InitData that were entered into the constructor function.
	  * @param  None
	  * @retval None
	  */
	void Character_LCD::LCD_Init(void)
	{
		HAL_Delay(WATE_FOR_POWER_ON);	//Wate for turning the LCD on and be ready to use

		switch (this->pxLCD_InitData->LCDInterface)
		{
		case _4Bit_Interface:
			this->LCD_SendCommand(0x33);	//Config LCD as 4bit interface
			this->LCD_SendCommand(0x32);	//Config LCD as 4bit interface
			
			switch (this->pxLCD_InitData->LCDLine)
			{
			case Display_1Line:
				switch (this->pxLCD_InitData->LCDFont)
				{
				case Font5x7:
					this->LCD_SendCommand(0x20);
					break;

				case Font5x10:
					this->LCD_SendCommand(0x24);
					break;
				}
				break;

			case Display_2Line:
				switch (this->pxLCD_InitData->LCDFont)
				{
				case Font5x7:
					this->LCD_SendCommand(0x28);
					break;

				case Font5x10:
					this->LCD_SendCommand(0x2C);
					break;
				}
				break;
			}
			break;

		case _8Bit_Interface:
			switch (this->pxLCD_InitData->LCDLine)
			{
			case Display_1Line:
				switch (this->pxLCD_InitData->LCDFont)
				{
				case Font5x7:
					this->LCD_SendCommand(0x30);
					break;

				case Font5x10:
					this->LCD_SendCommand(0x34);
					break;
				}
				break;

			case Display_2Line:
				switch (this->pxLCD_InitData->LCDFont)
				{
				case Font5x7:
					this->LCD_SendCommand(0x38);
					break;

				case Font5x10:
					this->LCD_SendCommand(0x3C);
					break;
				}
				break;
			}
			break;
		}
		this->LCD_Clear();
		
		this->LCD_TurnOn(Cursor_Off);
		this->LCD_SendCommand(RIGHT_ALIGNMENT_CURSOR);
	}

	/**
	  * @brief  This function send a command to the LCD
	  * @param  _ucCommand: Command value ==> 8bit
	  * @retval None
	  */
	void Character_LCD::LCD_SendCommand(uint8_t _ucCommand)
	{
		HAL_Delay(5);

		HAL_GPIO_WritePin(this->pxLCD_Pins->RW.GPIO_Port, this->pxLCD_Pins->RW.GPIO_Pin, GPIO_PIN_RESET);	//Reset the RW pin ==> Write Operation
		HAL_GPIO_WritePin(this->pxLCD_Pins->RS.GPIO_Port, this->pxLCD_Pins->RS.GPIO_Pin, GPIO_PIN_RESET);	//Reset the RS pin ==> Sending Command Operation

		switch (this->pxLCD_InitData->LCDInterface)
		{
		case _4Bit_Interface:

			/************************* Sending 4 MSB *************************/
			this->GPIO_SendData((_ucCommand >> 4), _4Bit_Interface);	//Send 4 bit data to the data port
			
			HAL_GPIO_WritePin(this->pxLCD_Pins->Enable.GPIO_Port, this->pxLCD_Pins->Enable.GPIO_Pin, GPIO_PIN_SET);		//Activate the Enable pin ==> LCD Read the data port
			HAL_Delay(1);	//Wait for reading data by LCD
			HAL_GPIO_WritePin(this->pxLCD_Pins->Enable.GPIO_Port, this->pxLCD_Pins->Enable.GPIO_Pin, GPIO_PIN_RESET);	//Deactivate the Enable pin
			/************************* Sending 4 LSB *************************/
			this->GPIO_SendData(_ucCommand, _4Bit_Interface);	//Send 4 bit data to the data port

			HAL_GPIO_WritePin(this->pxLCD_Pins->Enable.GPIO_Port, this->pxLCD_Pins->Enable.GPIO_Pin, GPIO_PIN_SET);		//Activate the Enable pin ==> LCD Read the data port
			HAL_Delay(1);	//Wait for reading data by LCD
			HAL_GPIO_WritePin(this->pxLCD_Pins->Enable.GPIO_Port, this->pxLCD_Pins->Enable.GPIO_Pin, GPIO_PIN_RESET);	//Deactivate the Enable pin
			break;

		case _8Bit_Interface:
			this->GPIO_SendData(_ucCommand, _8Bit_Interface);	//Send 8 bit data to the data port

			HAL_GPIO_WritePin(this->pxLCD_Pins->Enable.GPIO_Port, this->pxLCD_Pins->Enable.GPIO_Pin, GPIO_PIN_SET);		//Activate the Enable pin ==> LCD Read the data port
			HAL_Delay(1);	//Wait for reading data by LCD
			HAL_GPIO_WritePin(this->pxLCD_Pins->Enable.GPIO_Port, this->pxLCD_Pins->Enable.GPIO_Pin, GPIO_PIN_RESET);	//Deactivate the Enable pin
			break;

		default:
			break;
		}
	}

	/**
	  * @brief  This function sends a data to the LCD
	  * @param  _ucData: Data Value ==> 8bit
	  * @retval None
	  */
	void Character_LCD::LCD_SendData(uint8_t _ucData)
	{
		HAL_GPIO_WritePin(this->pxLCD_Pins->RW.GPIO_Port, this->pxLCD_Pins->RW.GPIO_Pin, GPIO_PIN_RESET);	//Reset the RW pin ==> Write Operation
		HAL_GPIO_WritePin(this->pxLCD_Pins->RS.GPIO_Port, this->pxLCD_Pins->RS.GPIO_Pin, GPIO_PIN_SET);		//Set the RS pin ==> Sending Data Operation

		switch (this->pxLCD_InitData->LCDInterface)
		{
		case _4Bit_Interface:

			/************************* Sending 4 MSB *************************/
			this->GPIO_SendData((_ucData >> 4), _4Bit_Interface);	//Send 4 bit data to the data port

			HAL_GPIO_WritePin(this->pxLCD_Pins->Enable.GPIO_Port, this->pxLCD_Pins->Enable.GPIO_Pin, GPIO_PIN_SET);		//Activate the Enable pin ==> LCD Read the data port
			HAL_Delay(1);	//Wait for reading data by LCD
			HAL_GPIO_WritePin(this->pxLCD_Pins->Enable.GPIO_Port, this->pxLCD_Pins->Enable.GPIO_Pin, GPIO_PIN_RESET);	//Deactivate the Enable pin

			/************************* Sending 4 LSB *************************/
			this->GPIO_SendData(_ucData, _4Bit_Interface);	//Send 4 bit data to the data port

			HAL_GPIO_WritePin(this->pxLCD_Pins->Enable.GPIO_Port, this->pxLCD_Pins->Enable.GPIO_Pin, GPIO_PIN_SET);		//Activate the Enable pin ==> LCD Read the data port
			HAL_Delay(1);	//Wait for reading data by LCD
			HAL_GPIO_WritePin(this->pxLCD_Pins->Enable.GPIO_Port, this->pxLCD_Pins->Enable.GPIO_Pin, GPIO_PIN_RESET);	//Deactivate the Enable pin
			break;

		case _8Bit_Interface:
			this->GPIO_SendData(_ucData, _8Bit_Interface);	//Send 8 bit data to the data port

			HAL_GPIO_WritePin(this->pxLCD_Pins->Enable.GPIO_Port, this->pxLCD_Pins->Enable.GPIO_Pin, GPIO_PIN_SET);		//Activate the Enable pin ==> LCD Read the data port
			HAL_Delay(1);	//Wait for reading data by LCD
			HAL_GPIO_WritePin(this->pxLCD_Pins->Enable.GPIO_Port, this->pxLCD_Pins->Enable.GPIO_Pin, GPIO_PIN_RESET);	//Deactivate the Enable pin
			break;

		default:
			break;
		}
	}

	/**
	  * @brief  This function sends data into the pins of the LCD as parallel
	  * @param  _ucData: Data Value ==> 8bit
	  *			_xDataBus: Data bus length. It can be _8Bit_Interface or _4Bit_Interface.
	  * @retval None
	  */
	void Character_LCD::GPIO_SendData(uint8_t _ucData, LCDInterface_Typedef _xDataBus)
	{
		uint8_t Counter = 0;
		switch (_xDataBus)
		{
		case _4Bit_Interface:
			while (Counter < 4)	//It should send 4 bit to the data bus
			{
				if ((_ucData >> Counter) & 0x01 == 0x01)	//Check the bit value
				{
					HAL_GPIO_WritePin(this->pxLCD_Pins->Data[Counter].GPIO_Port, this->pxLCD_Pins->Data[Counter].GPIO_Pin, GPIO_PIN_SET);	//Send data to the GPIO
				}
				else
				{
					HAL_GPIO_WritePin(this->pxLCD_Pins->Data[Counter].GPIO_Port, this->pxLCD_Pins->Data[Counter].GPIO_Pin, GPIO_PIN_RESET);	//Send data to the GPIO
				}
				Counter++;
			}
			break;

		case _8Bit_Interface:
			while (Counter < 8)	//It should send 8 bit to the data bus
			{
				if ((_ucData >> Counter) & 0x01 == 0x01)	//Check the bit value
				{
					HAL_GPIO_WritePin(this->pxLCD_Pins->Data[Counter].GPIO_Port, this->pxLCD_Pins->Data[Counter].GPIO_Pin, GPIO_PIN_SET);	//Send data to the GPIO
				}
				else
				{
					HAL_GPIO_WritePin(this->pxLCD_Pins->Data[Counter].GPIO_Port, this->pxLCD_Pins->Data[Counter].GPIO_Pin, GPIO_PIN_RESET);	//Send data to the GPIO
				}
				Counter++;
			}
			break;
		}	
	}

	/**
	  * @brief  This function turns the LCD on
	  * @param  _xCursor_Status: It specifies the cursor status. It can be Cursor_Off, Cursor_Blink_On and Cursor_Blink_Off.
	  * @retval None
	  */
	void Character_LCD::LCD_TurnOn(Cursor_TypeDef _xCursor_Status)
	{
		if (_xCursor_Status == Cursor_Off)
		{
			this->LCD_SendCommand(CURSOR_OFF);
		}
		else if (_xCursor_Status == Cursor_Blink_On)
		{
			this->LCD_SendCommand(CURSOR_BLINK_ON);
		}
		else if (_xCursor_Status == Cursor_Blink_Off)
		{
			this->LCD_SendCommand(CURSOR_BLINK_OFF);
		}
	}

	/**
	  * @brief  This function turns the LCD off
	  * @param  None
	  * @retval None
	  */
	void Character_LCD::LCD_TurnOff(void)
	{
		this->LCD_SendCommand(DISPLAY_OFF);
	}

	/**
	  * @brief  This function clears the LCD
	  * @param  None
	  * @retval None
	  */
	void Character_LCD::LCD_Clear(void)
	{
		this->LCD_SendCommand(CLEAR_DISPLAY);
		this->LCD_SendCommand(RETURN_HOME);
	}

	/**
	  * @brief  This function write a character on the LCD
	  * @param  _ucChar: Character value ==> ASCII Code
	  * @retval None
	  */
	void Character_LCD::LCD_PutChar(uint8_t _ucChar)
	{
		this->LCD_SendData(_ucChar);
	}

	/**
	  * @brief  This function write a string on the LCD
	  * @param  _pucString: It is a string to write on the LCD
	  * @retval HAL Status.
	  */
	void Character_LCD::LCD_PutString(uint8_t* _pucString)
	{
		uint8_t* pucString = _pucString;

		while (*pucString != 0)
		{
			this->LCD_PutChar(*pucString++);
		}
	}
}
/*****************************END OF FILE*****************************/
