#ifndef DEFINES_H_
#define DEFINES_H_
//*** <<< Use Configuration Wizard in Context Menu >>> ***

//==========================================================
// <h> Communication

// <q> BLE Enable
#ifndef EN_BLE
#define EN_BLE              		0
#endif
// <q> Power Manage Enable
#ifndef EN_PWR_MANAGE
#define EN_PWR_MANAGE               0
#endif

// <q> UART Printing Enable
#ifndef EN_UART
#define EN_UART                     0
#endif

// </h> 
//==========================================================

//==========================================================
// <h> Battery Configuration

// <q> Battery Update Enable
// <i> When the system power is enabled, update the battery voltage every 2s. 
#ifndef EN_BATT_UPDATE
#define EN_BATT_UPDATE      		1
#endif

// <q> Battery Low Indication Enable
// <i> When the battery is below 7.3V, blink LED 0. 
#ifndef EN_BATT_LOW_IND 
#define EN_BATT_LOW_IND             1
#endif

// <q> Charging Enable - Enable Charging when the USB C cable is plugged in
#ifndef EN_CHARGING
#define EN_CHARGING                 1
#endif

// <q> Current Sweep Enable - Enable Charging Current Sweeping(Experimental)
#ifndef EN_CURRENT_SWEEP
#define EN_CURRENT_SWEEP            0
#endif

// </h> 
//==========================================================

//==========================================================
// <h> Control Methods

// <h> Direct Control

// <q> Direct Control Enable - Enable reading of the EMG Eleectrodes
#ifdef EN_DIRECT_CONTROL
#define EN_DIRECT_CONTROL   		0
#endif

// <q> Direct Control Print - Print the signal
#ifdef EN_DIR_UART
#define EN_DIR_UART                 0
#endif

// </h>

// <q> Pullstring Control Enable - Enable reading from the encoder to determine pullstring distance. 
#ifndef EN_PULLSTRING_CONTROL
#define EN_PULLSTRING_CONTROL       0
#endif

// <q> Co-apt Control Enable - Enable reading from Co-apt Controller.
#ifndef EN_COAPT_CONTROL
#define EN_COAPT_CONTROL            0
#endif

// <q> Timer2 Calibration Enable
#ifndef EN_CALIBRATE_TIMER
#define EN_CALIBRATE_TIMER          0
#endif

// </h> 
//==========================================================

//==========================================================
// <h> Pressure Sensing
// <q> Pressure Sensing Enable
#ifndef EN_PRESSURE_SENSING
#define EN_PRESSURE_SENSING         0
#endif

// <q> Contact Reflex Enable
#ifndef EN_CONTACT_REFLEX
#define EN_CONTACT_REFLEX   		0
#endif

// <q> Vibrator Enable
#ifndef EN_VIB
#define EN_VIB              		0
#endif

// </h> 
//==========================================================

// <q> Rotator Enable
#ifndef EN_ROTATOR
#define EN_ROTATOR                  0
#endif

//*** <<< end of configuration section >>>    ***
#endif
