/***************************************************************************
 *     Copyright (c) 2003-2014, Broadcom Corporation
 *     All Rights Reserved
 *     Confidential Property of Broadcom Corporation
 *
 *  THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED SOFTWARE LICENSE
 *  AGREEMENT  BETWEEN THE USER AND BROADCOM.  YOU HAVE NO RIGHT TO USE OR
 *  EXPLOIT THIS MATERIAL EXCEPT SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
 *
 * $brcm_Workfile: $
 * $brcm_Revision: $
 * $brcm_Date: $
 *
 * Module Description:
 *
 * Revision History:
 *
 * $brcm_Log: $
 *
 ***************************************************************************/
#ifndef BCHP_H__
#define BCHP_H__

#ifndef __ASSEMBLY__
#include "bstd.h"
#include "breg_mem.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*=************************ Module Overview ********************************
<verbatim>

Overview
The purpose of this module is to give information about the chip that you
are using, such as chip ID and revision, as well as provide the register
list required to program the chip.

The chip interface contains the defines for all registers and each field
of the register for the entire chip. These defines are automatically
generated from RDB (Register DataBase) and are contained in header files
found in the basemodules\chp\{chip}\rdb\ directory. Individual header
files may be included directly or all can be included by using the header
file bchp_{chip}.h.

Registers must be read using the functions found in the register interface.
Field access must be done using the macros provided here in the
chip interface. These macros are as follows:

   o BCHP_GET_FIELD_DATA
   o BCHP_FIELD_ENUM
   o BCHP_FIELD_DATA
   o BCHP_MASK


Design
  The design for BCHP PI API is broken into three parts.

o Part 1 (open/close/configuration):

    These APIs are used for opening and closing BCHP device.

o Part 2 (get):

    These APIs are used for getting chip information, like Family Id, Chip Id, etc.

o Part 3 (get macro/set macro):

    These macros are provided to access bit field inside a register using
    RDB documenation.  See above discussion on this topic.


Usage
The usage of BCHP involves the following:

   * Configure/Open of BCHP

      * Get chip defaults for chips that exist on target system
      * Using these chip defaults, for each chip that requires a chip handle it
	    default settings for that chip.

Sample Code

// Example of Bit Field Macros
{
	uint32_t ulReg;
	BREG_Read32(hReg, CMP_0_BLEND_0_CTRL, &ulReg);

	ulReg &= ~(
		BCHP_MASK(CMP_0_BLEND_0_CTRL, BLEND_SOURCE            ) |
		BCHP_MASK(CMP_0_BLEND_0_CTRL, CONSTANT_ALPHA          ) |
		BCHP_MASK(CMP_0_BLEND_0_CTRL, BACK_COLOR_BLEND_FACTOR ) |
		BCHP_MASK(CMP_0_BLEND_0_CTRL, FRONT_COLOR_BLEND_FACTOR));

	ulReg |= (
		BCHP_FIELD_ENUM(CMP_0_BLEND_0_CTRL, BLEND_SOURCE,
			SURFACE_V0    ) |
		BCHP_FIELD_DATA(CMP_0_BLEND_0_CTRL, CONSTANT_ALPHA,
			0xff          ) |
		BCHP_FIELD_ENUM(CMP_0_BLEND_0_CTRL, BACK_COLOR_BLEND_FACTOR,
			ZERO          ) |
		BCHP_FIELD_ENUM(CMP_0_BLEND_0_CTRL, FRONT_COLOR_BLEND_FACTOR,
			CONSTANT_ALPHA));

	BREG_Write32(hReg, CMP_0_BLEND_0_CTRL, ulReg);
}

//
// NOTE: The following sample code does not do any error checking.
//
// In this example, the target H/W board contains two chips that requires
// Chip Handles, Bcm7038 and Bcm3250.
//
static BCHP_Handle hChip7038;
static BREG_Handle hReg7038;
static BCHP_Handle hChip3250;
static BREG_Handle hReg3250;

main( void )
{
	BERR_Code retCode = BERR_SUCCESS;
	BCHP_Info info;

	// Initialize hReg7038, hReg3250 . . .

	// Now create the chip interface for the two chips
	BCHP_Open7038( &hChip7038, hReg7038 );
	BCHP_Open3250( &hChip3250, hReg3250 );

	// Get the chip information for Bcm7038
	BCHP_GetInfo( hChip7038, &info );
	printf( "Chip Information for Bcm7038\n" );
	printf( "ChipId=%d, ChipRev=%d\n", info.familyId, info.rev );

	// Get the chip information for Bcm3250
	BCHP_GetInfo( hChip3250, &info );
	printf( "Chip Information for Bcm3250\n" );
	printf( "ChipId=%d, ChipRev=%d\n", info.familyId, info.rev );
}


</verbatim>
***************************************************************************/


/***************************************************************************
Summary:
	Extracts the bitfield value of a register using RDB name.

Description:
	This macro extracts the bitfield value of a register using RDB name. It
	intends to hide away the mechanics of SHIFTing/ORing/MASKing to get the
	value.

Input:
	Memory - Memory variable of a register to extract the bitfield. The
	value of this variable is usually get filled with a BREG_ReadXX call.

	Register - Register instance name defined in RDB. These names are
	available in html documentations, and the RDB generated header files.
	Note this a  register instance name; not a regtype.

	Field - Field name defined in RDB. These names are available in html
	documentations, and the RDB generated header files.

Output:
	<None>

Returns:
	The extracted bitfield value.

Example:
	ulData = BCHP_GET_FIELD_DATA(ulReg, CMP_0_BLEND_0_CTRL, BLEND_SOURCE);

See Also:
	BCHP_FIELD_DATA, BCHP_FIELD_ENUM.
**************************************************************************/
#define BCHP_GET_FIELD_DATA(Memory,Register,Field) \
	((((Memory) & BCHP_MASK(Register,Field)) >> \
	BCHP_SHIFT(Register,Field)))

/***************************************************************************
Summary:
	Push a register bitfield value into a memory location using RDB
	name.

Description:
	This macro sets a register bitfield value into a memory location
	using an RDB name. It intends to hide away the mechanics of
	ANDing/SHIFTing/ORing/MASKing to set the value.

Input:
	Memory - Memory variable to set the value of the bitfield. The
	value of this variable is usually filled initially with a
	BREG_ReadXX call.

	Register - Register instance name defined in RDB. These names are
	available in html documentations, and the RDB generated header files.
	Note this a  register instance name; not a regtype.

	Field - Field name defined in RDB. The "Field" will be set to zero in
	"Memory" before "FieldValue" os ORed in. These names are available in
	html documentations, and the RDB generated header files.

	FieldValue - This is the value that the register field is to be set
	to. The FieldValue will SHIFTed and MASKed before being ORed into
	"Memory"

Output:
	The memory location specified by "Memory" will have the register bit
	field specified by "Field" set to "FieldValue".

Returns:
	<None>.

Example:
	ulReg = BREG_Read32( hRegister, BCHP_V3D_PLL_CTRL_PLL_6CH_CTRL_0 );
	BCHP_SET_FIELD_DATA( ulReg, V3D_PLL_CTRL_PLL_6CH_CTRL_0, POST_RESETB, 0 );
	BREG_Write32( hRegister, BCHP_V3D_PLL_CTRL_PLL_6CH_CTRL_0, ulReg );

See Also:
	BCHP_SET_FIELD_ENUM.
**************************************************************************/
#define BCHP_SET_FIELD_DATA(Memory,Register,Field,FieldValue) \
	((Memory)=(((Memory)&(~BCHP_MASK(Register,Field))) | \
	 BCHP_FIELD_DATA(Register,Field,FieldValue)))
#define BCHP_SET_FIELD_CONST_DATA(Memory,Register,Field,FieldValue) \
	((Memory)=(((Memory)&(~BCHP_MASK(Register,Field))) | \
	 BCHP_FIELD_CONST_DATA(Register,Field,FieldValue)))

/***************************************************************************
Summary:
	Push a register bitfield value into a memory location using RDB
	name.

Description:
	This macro sets a register bitfield value into a memory location
	using an RDB name. It intends to hide away the mechanics of
	ANDing/SHIFTing/ORing/MASKing to set the value.

Input:
	Memory - Memory variable to set the value of the bitfield. The
	value of this variable is usually filled initially with a
	BREG_ReadXX call.

	Register - Register instance name defined in RDB. These names are
	available in html documentations, and the RDB generated header files.
	Note this a  register instance name; not a regtype.

	Field - Field name defined in RDB. The "Field" will be set to zero in
	"Memory" before "FieldValue" os ORed in. These names are available in
	html documentations, and the RDB generated header files.

	Name - A value name that get push into the regiser bitfield.
	This name is available in html documentations (italicized) in the
	bitfield definition.

Output:
	The memory location specified by "Memory" will have the register bit
	field specified by "Field" set to "Name".

Returns:
	<None>.

Example:
	ulReg = BREG_Read32( hRegister, BCHP_VDEC_FE_0_SYNC_DETECT );
	BCHP_SET_FIELD_ENUM( ulReg, VDEC_FE_0_SYNC_DETECT, HEAD_SWITCH_SEL, FOUR_HEAD );
	BREG_Write32( hRegister, BCHP_VDEC_FE_0_SYNC_DETECT, ulReg );

See Also:
	BCHP_SET_FIELD_DATA
**************************************************************************/
#define BCHP_SET_FIELD_ENUM(Memory,Register,Field,Name) \
	((Memory)=(((Memory)&(~BCHP_MASK(Register,Field))) | \
	    BCHP_FIELD_ENUM(Register,Field,Name)))

/***************************************************************************
Summary:
	Push a value into a register bitfield using RDB name.

Description:
	This macro push a value into a register bitfield using RDB name. It
	intends to hide away the mechanics of SHIFTing/ORing/MASKing to push
	the value into a register bitfield.

Input:
	Memory - Memory variable of a register to be modified. The value
	of this variable is usually get filled with a BREG_ReadXX call.

	Register - Register instance name defined in RDB. These names are
	available in html documentations, and the RDB generated header files.
	Note this a register instance name; not a regtype.

	Field - Field name defined in RDB. These names are available in html
	documentations, and the RDB generated header files.

	Data - A data value that get push into the register bitfield.

Output:
	Memory - Memory variable will get modifled with the new value (Data)
	in approriate bitfield.

Returns:
	<None>

Example:
	ulReg |= BCHP_FIELD_DATA(CMP_0_BLEND_0_CTRL, CONSTANT_ALPHA, 0xff);

See Also:
	BCHP_GET_FIELD_DATA, BCHP_FIELD_ENUM, BCHP_SHIFT
**************************************************************************/
/* Debugging with BCHP_DEBUG_OVERFLOW
 * ----------------------------------
 *
 *  The preprocessor symbol BCHP_DEBUG_OVERFLOW controls a debugging feature in
 *  BCHP. Each bitfield write is tested to determine if the value written is
 *  too large to fit into the destination bitfield.
 *
 *  There are three ways to build software:
 *
 *  BCHP_DEBUG_OVERFLOW = 2:
 *
 *  Compile-time checking is activated. However, the resulting software cannot
 *  be used. In particular, most values written to registers will be incorrect!
 *
 *  BCHP_DEBUG_OVERFLOW = 1:
 *
 *  Run-time checking is activated. Performance will suffer.
 *
 *  BCHP_DEBUG_OVERFLOW = 0 (or undefined)
 *
 *  No checking, no performance penalty.
 *
 *  The following BCHP macros are affected
 *
 *  BCHP_FIELD_DATA
 *  BCHP_FIELD_CONST_DATA
 *  BCHP_FIELD_FORCE_DATA
 *  All three macros behave identically if debugging is disabled. For debugging
 *  purposes, please use BCHP_FIELD_CONST_DATA to initialize const data. Use
 *  BCHP_FIELD_FORCE_DATA in cases where bitfield overflow is known to occur,
 *  and is NOT an error. Use BCHP_FIELD_DATA for all other situations.
 *
 *  Similar rules apply to these two macros:
 *  BCHP_FIELD_ENUM
 *  BCHP_FIELD_CONST_ENUM
 *
 */
#if ((BCHP_DEBUG_OVERFLOW < 0) || (BCHP_DEBUG_OVERFLOW > 2))
	#error "BCHP_DEBUG_OVERFLOW must be 0, 1, 2, or <undefined>"
#elif (BCHP_DEBUG_OVERFLOW == 2)
	#define BCHP_FIELD_CONST_DATA(Register,Field,Data)                         \
		1 /                                                                    \
			((unsigned)(Data) <=                                               \
				(BCHP_MASK(Register,Field) >>                                  \
					BCHP_SHIFT(Register,Field)))
	#define BCHP_FIELD_DATA(Register,Field,Data)                               \
		(((Data) << BCHP_SHIFT(Register,Field))&BCHP_MASK(Register,Field))
#elif (BCHP_DEBUG_OVERFLOW == 1)
	#define BCHP_FIELD_DATA(Register,Field,Data)                               \
		(                                                                      \
			(BDBG_ASSERT (                                                     \
				(Data) <=                                                      \
					(BCHP_MASK(Register,Field) >>                              \
						BCHP_SHIFT(Register,Field)))),                         \
			(((unsigned)(Data) <<                                              \
			    BCHP_SHIFT(Register,Field))&BCHP_MASK(Register,Field))         \
		)
	#define BCHP_FIELD_CONST_DATA(Register,Field,Data)                         \
		(((Data) << BCHP_SHIFT(Register,Field))&BCHP_MASK(Register,Field))
#else
	#define BCHP_FIELD_DATA(Register,Field,Data)                               \
		(((Data) << BCHP_SHIFT(Register,Field))&BCHP_MASK(Register,Field))
	#define BCHP_FIELD_CONST_DATA(Register,Field,Data)                         \
		(((Data) << BCHP_SHIFT(Register,Field))&BCHP_MASK(Register,Field))
#endif
#define BCHP_FIELD_FORCE_DATA(Register,Field,Data)                         \
	(((Data) << BCHP_SHIFT(Register,Field))&BCHP_MASK(Register,Field))

/***************************************************************************
Summary:
	Push a  value name into a register bitfield using RDB name.

Description:
	This macro will push a value name into a register bitfield using RDB
        name. It intends to hide away the mechanics of MASKing/SHIFTing/ORing
        to push the value into a register bitfield.

Input:
	Memory - Memory variable of a register to be modified.  The value
	of this variable is usually get filled with a BREG_ReadXX call.

	Register - Register instance name defined in RDB.  These names are
	available in html documentations, and the RDB generated header files.
	Note this a register instance name; not a regtype.

	Field - Field name defined in RDB.  These names are available in html
	documentations, and the RDB generated header files.

	Name - A value name that get push into the regiser bitfield.
	This name is available in html documentations (italicized) in the
	bitfield definition.

Output:
	Memory - Memory variable will get modified with the new value name
	(Name) in approriate bitfield.

Returns:
	<None>

Example:
	ulReg |= BCHP_FIELD_ENUM(CMP_0_BLEND_0_CTRL,
		FRONT_COLOR_BLEND_FACTOR, CONSTANT_ALPHA)

See Also:
	BCHP_FIELD_DATA, BCHP_GET_FIELD_DATA, BCHP_SHIFT
**************************************************************************/
#define BCHP_FIELD_ENUM(Register,Field,Name) \
	BCHP_FIELD_DATA(Register,Field, BCHP_##Register##_##Field##_##Name)
#define BCHP_FIELD_CONST_ENUM(Register,Field,Name) \
	BCHP_FIELD_CONST_DATA(Register,Field, BCHP_##Register##_##Field##_##Name)

/*************************************************************************
Summary:
	Provide a mask for a specific register field.

Description:
	This macro returns a mask for the specified register field. This value
	is typically used with other masks to clear out bits of a register
	before setting in a read/modify/write operation.

Input:
	Register - Register instance name defined in RDB. These names are available
	in html documentations, and the RDB generated header files. Note this a
	register instance name; not a regtype.

	Field - Field name defined in RDB. These names are available in html
	documentations, and the RDB generated header files.

Output:
	<None>

Returns:
	The mask of the register field.

Example:
	ulReg &= ~BCHP_MASK(CMP_0_BLEND_0_CTRL, BLEND_SOURCE);

See Also:
	BCHP_GET_FIELD_DATA,
	BCHP_FIELD_DATA, BCHP_FIELD_ENUM, BCHP_SHIFT
**************************************************************************/
#define BCHP_MASK(Register,Field) \
	BCHP_##Register##_##Field##_MASK

/*************************************************************************
Summary:
	Provide a shift for a specific register field.

Description:
	This macro returns a shift value for the specified register field.
	This value is typically used by the BCHP_FIELD_NAME and
	BCHP_FIELD_ENUM macros to shift field values into the proper
	location. In other modules this value is used to determine a shift
	amount used in interrupt processing.

Input:
	Register - Register instance name defined in RDB. These names are available
	in html documentations, and the RDB generated header files. Note this a
	register instance name; not a regtype.

	Field - Field name defined in RDB. These names are available in html
	documentations, and the RDB generated header files.

Output:
	<None>

Returns:
	The shift value of the register field.

Example:
	ulShift = BCHP_SHIFT(SUN_TOP_CTRL_PIN_MUX_CTRL_4, gpio_00);

See Also:
	BCHP_MASK, BCHP_GET_FIELD_DATA,
	BCHP_FIELD_DATA, BCHP_FIELD_ENUM.
**************************************************************************/
#define BCHP_SHIFT(Register,Field) \
	BCHP_##Register##_##Field##_SHIFT

/***************************************************************************
Summary:
    Set mask and value into initial value

See Also:
	BCHP_UPDATE_FIELD_ENUM
	BCHP_UPDATE_FIELD_DATA
**************************************************************************/
#define BCHP_UPDATE_INIT(Mask,Value) do {Mask=0;Value=0;} while(0)

/***************************************************************************
Summary:
    Push a register bitfield mask and value into a memory location using RDB
    name.

Description:
    This macro sets a register bitfield mask and value into a memory location
    using an RDB name. It intends to hide away the mechanics of
    ANDing/SHIFTing/ORing/MASKing to set the value.

Input:
    Mask - Memory variable to set the mask of the bitfield. The
    value of this variable is usually filled initially with a
    BCHP_UPDATE_INIT call.

    Value - Memory variable to set the value of the bitfield. The
    value of this variable is usually filled initially with a
    BCHP_UPDATE_INIT call.

    Register - Register instance name defined in RDB. These names are
    available in html documentations, and the RDB generated header files.
    Note this a  register instance name; not a regtype.

    Field - Field name defined in RDB. These names are available in
    html documentations, and the RDB generated header files.

    FieldValue - This is the value that the register field is to be set
    to. The FieldValue will SHIFTed and MASKed before being ORed into
    "Value"

Output:
    The memory location specified by "Mask"  and "Value" will have the register bit
    field specified by "Field" set to "FieldValue".

Returns:
    <None>.

Example:
	BCHP_UPDATE_INIT(Mask,Value);
	BCHP_SET_FIELD_DATA(Mask, Value, V3D_PLL_CTRL_PLL_6CH_CTRL_0, POST_RESETB, 0 );
	BREG_Update32( hRegister, BCHP_V3D_PLL_CTRL_PLL_6CH_CTRL_0, Mask, Value);

See Also:
	BCHP_UPDATE_FIELD_ENUM.
**************************************************************************/
#define BCHP_UPDATE_FIELD_CONST_DATA(Mask, Value, Register,Field,FieldValue) do { Mask |= BCHP_MASK(Register,Field); Value |= BCHP_FIELD_CONST_DATA(Register,Field,FieldValue); } while(0)
#define BCHP_UPDATE_FIELD_DATA(Mask, Value, Register,Field,FieldValue) do { Mask |= BCHP_MASK(Register,Field); Value |= BCHP_FIELD_DATA(Register,Field,FieldValue); } while(0)

/***************************************************************************
Summary:
    Push a register bitfield mask and value into a memory location using RDB
    name.

Description:
    This macro sets a register bitfield value into a memory location
    using an RDB name. It intends to hide away the mechanics of
    ANDing/SHIFTing/ORing/MASKing to set the value.

Input:
    Mask - Memory variable to set the mask of the bitfield. The
    value of this variable is usually filled initially with a
    BCHP_UPDATE_INIT call.

    Value - Memory variable to set the value of the bitfield. The
    value of this variable is usually filled initially with a
    BCHP_UPDATE_INIT call.

    Register - Register instance name defined in RDB. These names are
    available in html documentations, and the RDB generated header files.
    Note this a  register instance name; not a regtype.

    Field - Field name defined in RDB. The "Field" will be set to zero in
    "Memory" before "FieldValue" os ORed in. These names are available in
    html documentations, and the RDB generated header files.

    Name - A value name that get push into the regiser bitfield.
    This name is available in html documentations (italicized) in the
    bitfield definition.

Output:
    The memory location specified by "Mask"  and "Value" will have the register bit
    field specified by "Field" set to "Name".

Returns:
    <None>.

Example:
	BCHP_UPDATE_INIT(Mask,Value);
	BCHP_UPDATE_FIELD_ENUM( Mask, Value. VDEC_FE_0_SYNC_DETECT, HEAD_SWITCH_SEL, FOUR_HEAD );
	BREG_Update32( hRegister, BCHP_VDEC_FE_0_SYNC_DETECT, Mask, Value);

See Also:
	BCHP_UPDATE_FIELD_DATA
**************************************************************************/
#define BCHP_UPDATE_FIELD_ENUM(mask, value, Register,Field,Name) do { mask |= BCHP_MASK(Register,Field); value |= BCHP_FIELD_ENUM(Register,Field,Name); } while(0)


/*{secret}******************************************************************

Summary:
	Used only by BCHP interface to create interrupt IDs

	Interrupt IDs consist of the left shifted L2 interrupt register
	address or'd with the L2 interrupt bit shift value.

	Since L2 registers are 32 bits in length the shift value must range
	from 0-31. This requires 5 bits to represent the shift value.

	Since all L2 interrupt register addresses are 32 bit aligned and
	the register address is shifted up by 3 and the unused lower 2 bits
	are reused to store the L2 interrupt shift.

	In some new chips L2 interrupt register addresses might have format
	0xh0xxxxxx, the leading hex digit 'h' is not 0, and will be overflowed
	off after it is left shifted by 3. But it is noticed that h & 0x1 == 0,
	and the shifted result all have leading hex digit as 0, therefore the
	the leading hex digit of the interrupt ID could be used to save 'h'.

See Also:
	BCHP_INT_ID_REG_SHIFT, BCHP_INT_ID_SHIFT_MASK,
	BCHP_INT_ID_LEAD_MASK, BCHP_INT_ID_ADDR_HOLE_MASK
	BCHP_INT_ID_CREATE, BCHP_INT_ID_GET_REG
**************************************************************************/
#define BCHP_INT_ID_REG_SHIFT            3

/*{secret}******************************************************************

Summary:
	Used only by BCHP interface to create interrupt IDs

	Interrupt IDs consist of the left shifted L2 interrupt register
	address or'd with the L2 interrupt bit shift value.

	Since L2 registers are 32 bits in length the shift value must range
	from 0-31. This requires 5 bits to represent the shift value.

	Since all L2 interrupt register addresses are 32 bit aligned and
	the register address is shifted up by 3 and the unused lower 2 bits
	are reused to store the L2 interrupt shift.

	In some new chips L2 interrupt register addresses might have format
	0xh0xxxxxx, the leading hex digit 'h' is not 0, and will be overflowed
	off after it is left shifted by 3. But it is noticed that h & 0x1 == 0,
	and the shifted result all have leading hex digit as 0, therefore the
	the leading hex digit of the interrupt ID could be used to save 'h'.

See Also:
	BCHP_INT_ID_REG_SHIFT, BCHP_INT_ID_SHIFT_MASK,
	BCHP_INT_ID_LEAD_MASK, BCHP_INT_ID_ADDR_HOLE_MASK
	BCHP_INT_ID_CREATE, BCHP_INT_ID_GET_REG
**************************************************************************/
#define BCHP_INT_ID_SHIFT_MASK           0x0000001F

/*{secret}******************************************************************

Summary:
	Used only by BCHP interface to create interrupt IDs

	Interrupt IDs consist of the left shifted L2 interrupt register
	address or'd with the L2 interrupt bit shift value.

	Since L2 registers are 32 bits in length the shift value must range
	from 0-31. This requires 5 bits to represent the shift value.

	Since all L2 interrupt register addresses are 32 bit aligned and
	the register address is shifted up by 3 and the unused lower 2 bits
	are reused to store the L2 interrupt shift.

	In some new chips L2 interrupt register addresses might have format
	0xh0xxxxxx, the leading hex digit 'h' is not 0, and will be overflowed
	off after it is left shifted by 3. But it is noticed that h & 0x1 == 0,
	and the shifted result all have leading hex digit as 0, therefore the
	the leading hex digit of the interrupt ID could be used to save 'h'.

See Also:
	BCHP_INT_ID_REG_SHIFT, BCHP_INT_ID_SHIFT_MASK,
	BCHP_INT_ID_LEAD_MASK, BCHP_INT_ID_ADDR_HOLE_MASK
	BCHP_INT_ID_CREATE, BCHP_INT_ID_GET_REG
**************************************************************************/
#define BCHP_INT_ID_LEAD_MASK            0xF0000000

/*{secret}******************************************************************

Summary:
	Used only by BCHP interface to create interrupt IDs

	Interrupt IDs consist of the left shifted L2 interrupt register
	address or'd with the L2 interrupt bit shift value.

	Since L2 registers are 32 bits in length the shift value must range
	from 0-31. This requires 5 bits to represent the shift value.

	Since all L2 interrupt register addresses are 32 bit aligned and
	the register address is shifted up by 3 and the unused lower 2 bits
	are reused to store the L2 interrupt shift.

	In some new chips L2 interrupt register addresses might have format
	0xh0xxxxxx, the leading hex digit 'h' is not 0, and will be overflowed
	off after it is left shifted by 3. But it is noticed that h & 0x1 == 0,
	and the shifted result all have leading hex digit as 0, therefore the
	the leading hex digit of the interrupt ID could be used to save 'h'.

See Also:
	BCHP_INT_ID_REG_SHIFT, BCHP_INT_ID_SHIFT_MASK,
	BCHP_INT_ID_LEAD_MASK, BCHP_INT_ID_ADDR_HOLE_MASK
	BCHP_INT_ID_CREATE, BCHP_INT_ID_GET_REG
**************************************************************************/
#define BCHP_INT_ID_ADDR_HOLE_MASK       0x1E000000

/*{secret}******************************************************************

Summary:
	Used only by BCHP interface to create interrupt IDs

See Also:
	BCHP_INT_ID_REG_SHIFT, BCHP_INT_ID_SHIFT_MASK, BCHP_INT_ID_LEAD_MASK
	BCHP_INT_ID_CREATE, BCHP_INT_ID_GET_REG, BCHP_INT_ID_REG_SHIFT
**************************************************************************/
#define BCHP_INT_ID_CREATE( reg, shift ) \
	(((reg) << BCHP_INT_ID_REG_SHIFT) | (shift) | ((reg) & BCHP_INT_ID_LEAD_MASK))

/*{secret}******************************************************************

Summary:
	Used by interrupt interface module to extract registers and shift values
	from a BINT_Id

See Also:
	BCHP_INT_ID_REG_SHIFT, BCHP_INT_ID_SHIFT_MASK, BCHP_INT_ID_LEAD_MASK
	BCHP_INT_ID_CREATE, BCHP_INT_ID_GET_REG
**************************************************************************/
#define BCHP_INT_ID_GET_REG( intId ) \
	((((intId) & (~BCHP_INT_ID_SHIFT_MASK) & (~BCHP_INT_ID_LEAD_MASK)) >> BCHP_INT_ID_REG_SHIFT) | \
	 ((intId) & BCHP_INT_ID_LEAD_MASK))

/*{secret}******************************************************************

Summary:
	Used by interrupt interface module to extract registers and shift
	values from a BINT_Id

See Also:
	BCHP_INT_ID_REG_SHIFT, BCHP_INT_ID_SHIFT_MASK,
	BCHP_INT_ID_CREATE, BCHP_INT_ID_GET_REG, BCHP_INT_ID_REG_SHIFT
**************************************************************************/
#define BCHP_INT_ID_GET_SHIFT( intId ) \
	((intId) & BCHP_INT_ID_SHIFT_MASK)

/*{secret}******************************************************************

Summary:
	Used to indicate an invalid stripe width

See Also:
	BCHP_MemoryInfo, BCHP_GetMemoryInfo
**************************************************************************/
#define BCHP_INVALID_STRIPE_WIDTH         (0xFFFFFFFF)

#ifndef __ASSEMBLY__
/***************************************************************************
Summary:
	Chip Interface Handle.

Description:
	TODO: Will add the rest of the API later.  I'm declaring it here
	to get things compile.

See Also:
	BCHP_Open(), BCHP_Close().
**************************************************************************/
typedef struct BCHP_P_Context *BCHP_Handle; /* Opaque handle */

/***************************************************************************
Summary:
Structure and sub-structures returned by BCHP_GetFeature as noted in BCHP_Feature comments.
***************************************************************************/
typedef struct BCHP_DisabledL2Registers
{
    unsigned address[8]; /* Physical address of L2 registers that should not be accessed. */
} BCHP_DisabledL2Registers;

typedef struct BCHP_FeatureData
{
    union
    {
        unsigned productId; /* Deprecated. Use BCHP_Info.productId instead. */
        BCHP_DisabledL2Registers disabledL2Registers;
    } data;
} BCHP_FeatureData;

/***************************************************************************
Summary:
	This enumeration represents possible chipset features to query.

Description:
	This enumeration is provided to a call to BCHP_GetFeature
	in order to determine the capabilities of a given chipset.

	Each enumeration has a specific type associated with it. All feature
	enumerations that end with 'Capable' refer to a bool type. All feature
	enumerations that end with a 'Count' refer to an int type.

See Also:
	BCHP_GetFeature.
***************************************************************************/
typedef enum BCHP_Feature
{
    /* The following use BCHP_FeatureData. The union member name should match the enum name exactly. */
    BCHP_Feature_eProductId, /* Deprecated. Use BCHP_Info.productId instead. */
    BCHP_Feature_eDisabledL2Registers,

    /* The following require typecasting. Please study the implementation to be sure it is correct. */
    BCHP_Feature_e3DGraphicsCapable,   /* 3D capable? (bool) */
    BCHP_Feature_eDvoPortCapable,      /* DVO Port capable? (bool) */
    BCHP_Feature_eMacrovisionCapable,  /* Chip is macrovision capable? (bool) */
    BCHP_Feature_eMpegDecoderCount,    /* Number of MPEG decoders (int) */
    BCHP_Feature_eHdcpCapable,         /* HDCP capable? (bool) */
    BCHP_Feature_e3desCapable,         /* 3DES capable? (bool) */
    BCHP_Feature_e1080pCapable,        /* 1080p Capable? (bool) */
    BCHP_Feature_eRfmCapable,          /* RFM capable? (bool) */
    BCHP_Feature_eMemCtrl1Capable,            /* Second DDR memory controller present (bool). */
    BCHP_Feature_eMemCtrl2Capable,            /* Third DDR memory controller present (bool). */
    BCHP_Feature_eMemCtrl0DDR3ModeCapable,    /* True = DDR3 */
    BCHP_Feature_eMemCtrl0DDRDeviceTechCount, /* Size of memory part in MBits ie: 256, 512, 1024 */
    BCHP_Feature_eMemCtrl0DramWidthCount,     /* Size of data path 16, 32, 64 bits */
    BCHP_Feature_eMemCtrl1DDR3ModeCapable,    /* True = DDR3 */
    BCHP_Feature_eMemCtrl1DDRDeviceTechCount, /* Size of memory part in MBits ie: 256, 512, 1024 */
    BCHP_Feature_eMemCtrl1DramWidthCount,     /* Size of data path 16, 32, 64 bits */
	BCHP_Feature_eMemCtrl2DDR3ModeCapable,	  /* True = DDR3 */
	BCHP_Feature_eMemCtrl2DDRDeviceTechCount, /* Size of memory part in MBits ie: 256, 512, 1024 */
	BCHP_Feature_eMemCtrl2DramWidthCount,	  /* Size of data path 16, 32, 64 bits */
    BCHP_Feature_eAVDCoreFreq                 /* AVD Core freqencey */
} BCHP_Feature;

/***************************************************************************
Summary:
	This structure represents settings for avs monitor

Description:
	This structure is provided to a call to BCHP_Monitor_Pvt
	Note that this data is only used by 65nm parts that support AVS hardware.

See Also:
	BCHP_Monitor_Pvt.
***************************************************************************/
typedef struct BCHP_AvsSettings
{
    unsigned hardStopOffset;         /* Range 0-15. This parameter should be programmed as per the
                                        board voltage drop (through PCB and filter components) between
                                        the output of the regulator [VREG_VFB_1P2 net] and the
                                        supply of the chip [D1.2V_BCM75XX]. Each unit is ~12.5mV.
                                        If the board design  has 20mV of drop then hardStopOffset=2
                                        should be programmed */
    unsigned maxVoltageStopOffset;   /* Percentage reduction 0-15 from maximum +16% Vreg setting (at regulator).
                                        Set this parameter to 4 if ambient temperature is expected
                                        to rise above +70C for extended period. */
} BCHP_AvsSettings;


/***************************************************************************
Summary:
    The data structure for receiving current AVS data.

Description:
    The AVS PVT Monitor can collect various pieces of information.
    This is used to return that information to the caller.
	Note that this data is only valid for 40nm parts that support AVS hardware.

See Also:
    BCHP_GetAvsData()

****************************************************************************/
typedef struct {
    unsigned voltage;     /* this is the last measured voltage (in millivolts) */
                          /* to convert use: float V = voltage/1000.; */
    signed temperature;   /* this is the last converted temperature (in thousands) read from internal chip thermister */
                          /* to convert use: float centigrade = temperature/1000.; */
    bool enabled;         /* AVS is enabled in the bootloader/CFE */
    bool tracking;        /* AVS-tracking is enabled in Nexus system software */
} BCHP_AvsData;


/***************************************************************************
Summary:
	Close a chip handle.

Description:
	Close the chip release internal resource.

See Also:
	BCHP_Open
**************************************************************************/
BERR_Code BCHP_Close(
	BCHP_Handle hChip                /* [in] Chip handle */
	);

/***************************************************************************
Deprecated. Use BCHP_GetInfo.
**************************************************************************/
BERR_Code BCHP_GetChipInfo(
    const BCHP_Handle hChip,
    uint16_t *pChipId, /* WARNING: may return family id or product id. cannot return 20 bit product id. */
    uint16_t *pChipRev
    );

/***************************************************************************
Summary:
Information returned by BCHP_GetInfo
**************************************************************************/
typedef struct BCHP_Info
{
    unsigned familyId;  /* hex id. corresponds to BCHP_CHIP. for example 0x7445. */
    unsigned productId; /* hex id. for example 0x7252. */
    unsigned rev;       /* 0x00 = A0, 0x01 = A1, 0x12 = B2, etc. */
} BCHP_Info;

/***************************************************************************
Summary:
Get information about the chip
**************************************************************************/
void BCHP_GetInfo(
    BCHP_Handle hChip,
    BCHP_Info *pInfo
    );

#if BCHP_UNIFIED_IMPL
typedef struct BCHP_OpenSettings
{
    BREG_Handle reg;
    unsigned productId; /* hex value. if non-zero, this will override BCHP_Info.productId. default is zero. */
} BCHP_OpenSettings;

void BCHP_GetDefaultOpenSettings(
    BCHP_OpenSettings *pSettings
    );

/***************************************************************************
Summary:
Generic open function for the SoC (system-on-a-chip)
**************************************************************************/
BERR_Code BCHP_Open(
    BCHP_Handle *phChip,
    const BCHP_OpenSettings *pSettings
    );
#endif

/***************************************************************************
Summary:
	Get a chip feature.

Description:
	This function returns specific information about a feature of a
	chip. If the feature enumeration is understood by the specific
	chip's interface, an appropriate value is returned. If the
	feature enumeration is unhandled, this function will return
	BERR_UNKNOWN meaning it is unknown whether this feature is supported
	by this chipset.

	The type of the returned value (pFeatureValue) is specified in the
	documentation of the feature enumeration. All feature enumerations
	that end with 'Capable' return a bool type. All feature enumerations
	that end with a 'Count' return an int type.

See Also:
	BCHP_Close
**************************************************************************/
BERR_Code BCHP_GetFeature(
	const BCHP_Handle hChip,         /* [in] Chip handle */
	const BCHP_Feature eFeature,     /* [in] Feature to be queried. */
	void *pFeatureValue              /* [out] Feature value. */
	);

/***************************************************************************
Summary:
	Type of DRAM on each memory controller
**************************************************************************/
typedef enum BCHP_DramType
{
    BCHP_DramType_eDDR2,
    BCHP_DramType_eDDR3,
    BCHP_DramType_eDDR4,
    BCHP_DramType_eGDDR5M,
    BCHP_DramType_eGDDR5,
    BCHP_DramType_eLPDDR4,
    BCHP_DramType_eMax
} BCHP_DramType;

/***************************************************************************
Summary:
	Information about DRAM on each memory controller
**************************************************************************/
typedef struct BCHP_MemoryInfo
{
    struct {
        uint64_t size; /* in bytes */
        unsigned width; /* width of MEMC PHY. in bits, typically 16 or 32. */
        unsigned deviceWidth; /* width of DRAM device. in bits, typically 8, 16 or 32. The number of DRAM parts per MEMC = width/deviceWidth. */
        BCHP_DramType type;
        bool ddr3Capable; /* deprecated */
        bool groupageEnabled;
        unsigned deviceTech; /* same as BCHP_Feature_eMemCtrlxDDRDeviceTechCount for backward compat */
        BSTD_DeviceOffset offset; /* device offset for the start of this memc */

        /* XVD/VCE/VDC striped memory parameters. */
        unsigned ulStripeWidth; /* in bytes */
        unsigned ulPageSize; /* in bytes */
        unsigned ulMbMultiplier; /* number of macroblocks in picture height = Multiplier*n + Remainder; */
        unsigned ulMbRemainder;
    } memc[3];
} BCHP_MemoryInfo;

/***************************************************************************
Summary:
	Get information about DRAM on each memory controller
**************************************************************************/
BERR_Code BCHP_GetMemoryInfo(
    BREG_Handle reg,
    BCHP_MemoryInfo *pInfo
    );

/***************************************************************************
Summary:
	Monitor adaptive voltage scaling.

Description:
	This function is called to adjust the voltage in response to
        ring oscillator frequencies

See Also:
	BCHP_Close
**************************************************************************/
void BCHP_MonitorPvt(
	BCHP_Handle hChip,              /* [in] Chip handle */
	BCHP_AvsSettings *pSettings     /* [in] AVS settings. */
	);

/***************************************************************************
Summary:
    Get the current AVS data.

Description:
    This can be used to get various data associated with the AVS hardware.
	This data will only be valid on 40nm parts that support AVS hardware.

See Also:
    BCHP_Open

**************************************************************************/
BERR_Code BCHP_GetAvsData_isrsafe(
    BCHP_Handle hChip,   /* [in] Chip handle */
    BCHP_AvsData *pData  /* [out] pointer to location to return data */
	);

/***************************************************************************
Summary:
    Enter chip Standby mode.

Description:
    This will perform chip specific functions needed to enter power stand-by mode.

See Also:
    BCHP_Open

**************************************************************************/
BERR_Code BCHP_Standby(
    BCHP_Handle hChip   /* [in] Chip handle */
	);

/***************************************************************************
Summary:
    Exit chip Standby mode.

Description:
    This will perform chip specific functions needed to exit power stand-by mode.

See Also:
    BCHP_Open

**************************************************************************/
BERR_Code BCHP_Resume(
    BCHP_Handle hChip   /* [in] Chip handle */
	);

/***************************************************************************
Summary:
    Get handle for AVS module.

Description:
    This is used for testing purposes to gain access to AVS priv functions.
	This will return NULL on platforms that do not support AVS.

See Also:
    BCHP_Open

**************************************************************************/
void *BCHP_GetAvsHandle(
    BCHP_Handle hChip   /* [in] Chip handle */
);

#endif /* ! __ASSEMBLY__ */

#include "bchp_ver_types.h"

/* Check for required definitions */
#ifndef BCHP_CHIP
#error BCHP_CHIP must be defined to the main chip id.
#endif

#ifndef BCHP_VER
#error BCHP_VER must be defined to a standard value.
#endif

#ifdef __cplusplus
}
#endif

#endif

/* End of File */
