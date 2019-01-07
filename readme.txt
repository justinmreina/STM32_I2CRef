@brief 		Project Generation and Reference Notes
@details 	For I2C-Master Use
@auth 		Justin Reina
@date 		1/6/18

@note 	Defaults are used unless otherwise specified

@section 	Generation Procedure
	Open TrueSTUDIO STM32							(v9.0.1)
	  File->New C Project->Embedded C Project ("STM32_I2CRef")
	  Device->STM32F0->Boards->NUCLEO-F091RC
	  Runtime library->Library: 'Newlib standard'
	  Create Project								[#3C5E]
	  Compile										[#B39A]
	  Deploy & Debug to Nucleo-F091RC				[#]
	Open STM32CubeMX V1.0 							(Version 5.0.0)
	  Access to Board Selector->Part Number Search: 'NUCLEO-F091RC'
	  Start Project
	  Init to Defaults? 'Yes'
	Pinout & Config->Connectivity->I2C2
	  Pin Select: PB10, PB11
	  Primary Slave Address: 0x64 (Hex)
      Project Name: "STM32_I2CRef-CubeMx"
	  Toolchain: TrueSTUDIO
	  Generate Code									[#]

@section 	Pins Used
	I2C2_SCL: PB10	(CN10.25, +7 up,   Blue)
	I2C2_SDA: PB11	(CN10.18  -9 down, Green)

