/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.brcm.scalar.conf $
 */
#ifndef DOCSDEVFILTER_H
#define DOCSDEVFILTER_H

config_require(bcmV2)

    /*
     * function declarations 
     */
     void            init_docsDevFilter(void);
     Netsnmp_Node_Handler do_docsDevFilterIpDefault;
     Netsnmp_Node_Handler do_docsDevFilterLLCUnmatchedAction;

#endif                          /* DOCSDEVFILTER_H */
