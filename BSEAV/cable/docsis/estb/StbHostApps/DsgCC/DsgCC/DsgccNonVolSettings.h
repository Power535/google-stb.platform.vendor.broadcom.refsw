//****************************************************************************
//
//  Copyright (c) 2001  Broadcom Corporation
//  All Rights Reserved
//  No portions of this material may be reproduced in any form without the
//  written permission of:
//          Broadcom Corporation
//          16215 Alton Parkway
//          Irvine, California 92619
//  All information contained in this document is Broadcom Corporation
//  company private, proprietary, and trade secret.
//
//****************************************************************************
//
//  Filename:       DsgccNonVolSettings.h
//  Author:         Kenny Lee
//  Creation Date:  Mar 15, 2007
//
//****************************************************************************
//  Description:
//      These are the Non-Volatile settings for Dsg Client Controller code
//
//      Note that the settings are independant of the device to which they are
//      being stored, allowing the same application settings to be used on
//      multiple target platforms and devices!
//
//****************************************************************************

#ifndef DsgccNonVolSettings_H
#define DsgccNonVolSettings_H

//********************** Include Files ***************************************

//********************** Include Files ***************************************
// My base class.
#include "NonVolSettings.h"

// My api and definitions...
#include "IpAddress.h"
#include "MacAddress.h"
#include "dsg_api.h"


//********************** Global Types ****************************************

//********************** Global Variables ************************************

//********************** Class Declaration ***********************************

class BcmDsgccNonVolSettings : public BcmNonVolSettings
{
public:

    // Default Constructor.  Initializes the state of the object, setting all
    // fields to good default values.
    //
    // Parameters:  None.
    //
    // Returns:  N/A
    //
    BcmDsgccNonVolSettings(void);

    // Destructor.  Frees up any memory/objects allocated, cleans up internal
    // state.
    //
    // Parameters:  N/A
    //
    // Returns:  N/A
    //
    virtual ~BcmDsgccNonVolSettings();

    // Allows the derived class to set its values to a known good state.  This
    // MUST be implemented by the derived class.
    //
    // Note that the derived class should set the version numbers to the desired
    // values in case they were changed (by ReadFrom(), etc).
    //
    // This method may be called if ReadFrom() returns false in order to ensure
    // that there are valid settings in the object.
    //
    // Parameters:
    //      section - the section that should be reset.
    //
    // Returns:  Nothing.
    //
    virtual void ResetDefaults(NonVolSection section);

    // Returns the pointer to the singleton instance for this class.  Most
    // objects in the system will use this method rather than being passed a
    // pointer to it.  The singleton pointer will be set up in the base-class
    // constructor.
    //
    // NOTES:  This can return NULL if a singleton has not been set up for the
    //         system, so you must check for this condition.
    //
    //         You must not delete this object!
    //
    //         You should not store a pointer to the object that is returned,
    //         since it may be deleted and replaced with a new one.
    //
    // Parameters:  None.
    //
    // Returns:
    //      A pointer to the instance that should be used by the system.
    //
    static BcmDsgccNonVolSettings *GetSingletonInstance(void);

	// Accessors to get/set the DSG global enable bit.
	//      mode =
	//      enum
	//      {
	//  	    kDsgDisabledMode = 0,
	//  	    kDsgBasicMode,
	//  	    kDsgAdvanceMode
	//      };
	//
	// Parameters:
	//      
	//
	// Returns:
	//
	uint8 DsgMode(void) const;
	void DsgMode(uint8 dsgMode);

#if defined(BCM_DSG_DUAL_PROCESSOR_INTERFACE)
	// Accessors to get/set IPC interface name
	//
	//
	const char * IpcInterfaceName(void);
	void IpcInterfaceName(const char *name);
#endif

protected:

    // This is where the derived class writes its members to the buffer in a
    // flat format.  It is up to the derived class to determine what order,
    // what data size, whether to use network or host byte order, etc.
    //
    // This MUST be implemented by the derived class.
    //
    // Parameters:
    //      outputBuffer - the buffer into which the settings should be written.
    //      section - the section that should be written.
    //
    // Returns:
    //      true if successful.
    //      false if there was a problem (usually the buffer is too small).
    //
    virtual bool WriteToImpl(BcmOctetBuffer &outputBuffer, NonVolSection section) const;

    // This is where the derived class reads its members from the buffer in a
    // flat format.  It is up to the derived class to determine what ordering,
    // etc., should be used.
    //
    // The currentOffset parameter tells the derived class where the app data
    // starts in the buffer.  It can be used directly in the calls to Read() or
    // NetworkRead() in BcmOctetBuffer (which expects an offset value).
    //
    // This MUST be implemented by the derived class.
    //
    // Parameters:
    //      inputBuffer - the buffer from which the settings should be read.
    //      currentOffset - the offset into the buffer from which the settings
    //                      should be read.  This is necessary to allow the
    //                      derived class to skip the header.
    //      section - the section that should be read.
    //
    // Returns:
    //      true if successful.
    //      false if there was a problem (version number not supported, not
    //          enough bytes in the buffer, etc.).
    //
    virtual bool ReadFromImpl(const BcmOctetBuffer &inputBuffer, 
                              unsigned int &currentOffset, NonVolSection section);

    // Allows the derived class to do formatted output of its settings to the
    // specified ostream, after the base class does its stuff.
    //
    // Parameters:
    //      outputStream - the ostream to be used.
    //
    // Returns:
    //      The reference to the ostream that was passed in.
    //
    virtual ostream & PrintImpl(ostream & outputStream) const;

protected:

    // Used to keep Reset() from printing warnings when it is called from the
    // constructor.
    bool fConstructorIsRunning;

public:


private:
    // This is the singleton instance of this class.
    static BcmDsgccNonVolSettings *pfSingletonInstance;

private:
    
    // Copy Constructor.  Not supported.
    //
    // Parameters:
    //      otherInstance - reference to the object to copy.
    //
    // Returns:  N/A
    //
    BcmDsgccNonVolSettings(const BcmDsgccNonVolSettings &otherInstance);

    // Assignment operator.  Not supported.
    //
    // Parameters:
    //      otherInstance - reference to the object to copy.
    //
    // Returns:
    //      A reference to "this" so that operator = can be chained.
    //
    BcmDsgccNonVolSettings & operator = (const BcmDsgccNonVolSettings &otherInstance);

private:
    
    // Settings for the Permanent Group:
    // ---------------------------------
    uint32 fDsgccNonVolU32NotUsed1;

	// Begin data members which are specific to DSG Operation Mode.
    //
	uint8 fDsgMode;

    uint32 fDsgccNonVolU32NotUsed2;
	BcmOctetBuffer fDsgDownloadCVC;

	//	This HAL is related to interface 18 in the
	//  ifTable, per eDOCSIS STB requirements.  Note that this interface
	//  and the interface from StbIpTransportHal may or may not be the 
	//  same. 
	uint8 fDsgccNonVolU8NotUsed3;
	uint8 fDsgccNonVolU8NotUsed4;
	uint8 fDsgccNonVolU8NotUsed5;

#if defined(BCM_DSG_DUAL_PROCESSOR_INTERFACE)
	// place holder for the interface name
	char fIpcInterfaceName[IPC_INTERFACE_NAME_MAX_LEN];
#endif

};

//********************** Inline Method Implementations ***********************


// This allows you to do the following:
//
//    cout << settingsInstance << endl;
//
inline ostream & operator << (ostream &outputStream, const BcmDsgccNonVolSettings &settingsInstance)
{
    return settingsInstance.Print(outputStream);
}

#endif

