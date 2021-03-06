/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.brcm.scalar.conf $
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "docsDevSoftware.h"

/*
 * Define the OID pointer to the top of the mib tree that we're
 * registering underneath
 */
oid             docsDevSoftware_module_oid[] =
    { 1, 3, 6, 1, 2, 1, 69, 1, 3 };
int             docsDevSoftware_module_oid_len =
    sizeof(docsDevSoftware_module_oid) / sizeof(oid);
int             docsDevSoftware_module_count = 0;


/** Initializes the docsDevSoftware module */
void
init_docsDevSoftware(void)
{
    static oid      docsDevSwFilename_oid[] =
        { 1, 3, 6, 1, 2, 1, 69, 1, 3, 2, 0 };
    static oid      docsDevSwOperStatus_oid[] =
        { 1, 3, 6, 1, 2, 1, 69, 1, 3, 4, 0 };
    static oid      docsDevSwAdminStatus_oid[] =
        { 1, 3, 6, 1, 2, 1, 69, 1, 3, 3, 0 };
    static oid      docsDevSwCurrentVers_oid[] =
        { 1, 3, 6, 1, 2, 1, 69, 1, 3, 5, 0 };
    static oid      docsDevSwServer_oid[] =
        { 1, 3, 6, 1, 2, 1, 69, 1, 3, 1, 0 };

    DEBUGMSGTL(("docsDevSoftware", "Initializing\n"));

    netsnmp_register_instance(netsnmp_create_handler_registration
                              ("docsDevSwFilename",
                               do_docsDevSwFilename,
                               docsDevSwFilename_oid,
                               OID_LENGTH(docsDevSwFilename_oid),
                               HANDLER_CAN_RWRITE));
    netsnmp_register_read_only_instance(netsnmp_create_handler_registration
                                        ("docsDevSwOperStatus",
                                         get_docsDevSwOperStatus,
                                         docsDevSwOperStatus_oid,
                                         OID_LENGTH
                                         (docsDevSwOperStatus_oid),
                                         HANDLER_CAN_RONLY));
    netsnmp_register_instance(netsnmp_create_handler_registration
                              ("docsDevSwAdminStatus",
                               do_docsDevSwAdminStatus,
                               docsDevSwAdminStatus_oid,
                               OID_LENGTH(docsDevSwAdminStatus_oid),
                               HANDLER_CAN_RWRITE));
    netsnmp_register_read_only_instance(netsnmp_create_handler_registration
                                        ("docsDevSwCurrentVers",
                                         get_docsDevSwCurrentVers,
                                         docsDevSwCurrentVers_oid,
                                         OID_LENGTH
                                         (docsDevSwCurrentVers_oid),
                                         HANDLER_CAN_RONLY));
    netsnmp_register_instance(netsnmp_create_handler_registration
                              ("docsDevSwServer", do_docsDevSwServer,
                               docsDevSwServer_oid,
                               OID_LENGTH(docsDevSwServer_oid),
                               HANDLER_CAN_RWRITE));
}

int
do_docsDevSwFilename(netsnmp_mib_handler *handler,
                     netsnmp_handler_registration *reginfo,
                     netsnmp_agent_request_info *reqinfo,
                     netsnmp_request_info *requests)
{
    /*
     * We are never called for a GETNEXT if it's registered as a
     * "instance", as it's "magically" handled for us.  
     */

    /*
     * a instance handler also only hands us one request at a time, so
     * we don't need to loop over a list of requests; we'll only get one. 
     */

    int             retVal = SNMP_ERR_NOERROR;

    u_char          docsDevSwFilename_var[STRINGMAX];
    size_t          docsDevSwFilename_varLen;

    switch (reqinfo->mode) {

    case MODE_GET:
        retVal =
            v2Get(requests->requestvb->name,
                  requests->requestvb->name_length, docsDevSwFilename_var,
                  &docsDevSwFilename_varLen, reqinfo);
        if (retVal == SNMP_ERR_NOERROR)
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
                                     docsDevSwFilename_var,
                                     docsDevSwFilename_varLen);
        else
            netsnmp_set_request_error(reqinfo, requests, retVal);
        break;

        /*
         * SET REQUEST
         *
         */
    case MODE_SET_RESERVE1:
        if (requests->requestvb->type != ASN_OCTET_STR) {
            netsnmp_set_request_error(reqinfo, requests,
                                      SNMP_ERR_WRONGTYPE);
            retVal = SNMP_ERR_WRONGTYPE;
        } else {
            retVal = v2Test(requests->requestvb->name,
                            requests->requestvb->name_length,
                            requests->requestvb->val.string,
                            requests->requestvb->val_len,
                            requests->requestvb->type, reqinfo);
            if (retVal != SNMP_ERR_NOERROR)
                netsnmp_set_request_error(reqinfo, requests, retVal);
        }
        break;

    case MODE_SET_RESERVE2:
        break;

    case MODE_SET_FREE:
        break;

    case MODE_SET_ACTION:
        break;

    case MODE_SET_COMMIT:
        /*
         * really do the set! 
         */
        retVal = v2Set(requests->requestvb->name,
                       requests->requestvb->name_length,
                       requests->requestvb->val.string,
                       requests->requestvb->val_len,
                       requests->requestvb->type, reqinfo);
        if (retVal != SNMP_ERR_NOERROR)
            netsnmp_set_request_error(reqinfo, requests, retVal);
        break;

    case MODE_SET_UNDO:
        /*
         * Can't undo V2 sets! 
         */
        break;

    default:
        /*
         * we should never get here, so this is a really bad error 
         */
        return SNMP_ERR_GENERR;
    }

    return retVal;
}

int
get_docsDevSwOperStatus(netsnmp_mib_handler *handler,
                        netsnmp_handler_registration *reginfo,
                        netsnmp_agent_request_info *reqinfo,
                        netsnmp_request_info *requests)
{
    /*
     * We are never called for a GETNEXT if it's registered as a
     * "instance", as it's "magically" handled for us.  
     */

    /*
     * a instance handler also only hands us one request at a time, so
     * we don't need to loop over a list of requests; we'll only get one. 
     */

    int             retVal = SNMP_ERR_NOERROR;

    u_char          docsDevSwOperStatus_var[STRINGMAX];
    size_t          docsDevSwOperStatus_varLen;

    switch (reqinfo->mode) {

    case MODE_GET:
        retVal =
            v2Get(requests->requestvb->name,
                  requests->requestvb->name_length,
                  docsDevSwOperStatus_var, &docsDevSwOperStatus_varLen,
                  reqinfo);
        if (retVal == SNMP_ERR_NOERROR)
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
                                     docsDevSwOperStatus_var,
                                     docsDevSwOperStatus_varLen);
        else
            netsnmp_set_request_error(reqinfo, requests, retVal);
        break;


    default:
        /*
         * we should never get here, so this is a really bad error 
         */
        return SNMP_ERR_GENERR;
    }

    return retVal;
}

int
do_docsDevSwAdminStatus(netsnmp_mib_handler *handler,
                        netsnmp_handler_registration *reginfo,
                        netsnmp_agent_request_info *reqinfo,
                        netsnmp_request_info *requests)
{
    /*
     * We are never called for a GETNEXT if it's registered as a
     * "instance", as it's "magically" handled for us.  
     */

    /*
     * a instance handler also only hands us one request at a time, so
     * we don't need to loop over a list of requests; we'll only get one. 
     */

    int             retVal = SNMP_ERR_NOERROR;

    u_char          docsDevSwAdminStatus_var[STRINGMAX];
    size_t          docsDevSwAdminStatus_varLen;

    switch (reqinfo->mode) {

    case MODE_GET:
        retVal =
            v2Get(requests->requestvb->name,
                  requests->requestvb->name_length,
                  docsDevSwAdminStatus_var, &docsDevSwAdminStatus_varLen,
                  reqinfo);
        if (retVal == SNMP_ERR_NOERROR)
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
                                     docsDevSwAdminStatus_var,
                                     docsDevSwAdminStatus_varLen);
        else
            netsnmp_set_request_error(reqinfo, requests, retVal);
        break;

        /*
         * SET REQUEST
         *
         */
    case MODE_SET_RESERVE1:
        if (requests->requestvb->type != ASN_INTEGER) {
            netsnmp_set_request_error(reqinfo, requests,
                                      SNMP_ERR_WRONGTYPE);
            retVal = SNMP_ERR_WRONGTYPE;
        } else {
            retVal = v2Test(requests->requestvb->name,
                            requests->requestvb->name_length,
                            requests->requestvb->val.string,
                            requests->requestvb->val_len,
                            requests->requestvb->type, reqinfo);
            if (retVal != SNMP_ERR_NOERROR)
                netsnmp_set_request_error(reqinfo, requests, retVal);
        }
        break;

    case MODE_SET_RESERVE2:
        break;

    case MODE_SET_FREE:
        break;

    case MODE_SET_ACTION:
        break;

    case MODE_SET_COMMIT:
        /*
         * really do the set! 
         */
        retVal = v2Set(requests->requestvb->name,
                       requests->requestvb->name_length,
                       requests->requestvb->val.string,
                       requests->requestvb->val_len,
                       requests->requestvb->type, reqinfo);
        if (retVal != SNMP_ERR_NOERROR)
            netsnmp_set_request_error(reqinfo, requests, retVal);
        break;

    case MODE_SET_UNDO:
        /*
         * Can't undo V2 sets! 
         */
        break;

    default:
        /*
         * we should never get here, so this is a really bad error 
         */
        return SNMP_ERR_GENERR;
    }

    return retVal;
}

int
get_docsDevSwCurrentVers(netsnmp_mib_handler *handler,
                         netsnmp_handler_registration *reginfo,
                         netsnmp_agent_request_info *reqinfo,
                         netsnmp_request_info *requests)
{
    /*
     * We are never called for a GETNEXT if it's registered as a
     * "instance", as it's "magically" handled for us.  
     */

    /*
     * a instance handler also only hands us one request at a time, so
     * we don't need to loop over a list of requests; we'll only get one. 
     */

    int             retVal = SNMP_ERR_NOERROR;

    u_char          docsDevSwCurrentVers_var[STRINGMAX];
    size_t          docsDevSwCurrentVers_varLen;

    switch (reqinfo->mode) {

    case MODE_GET:
        retVal =
            v2Get(requests->requestvb->name,
                  requests->requestvb->name_length,
                  docsDevSwCurrentVers_var, &docsDevSwCurrentVers_varLen,
                  reqinfo);
        if (retVal == SNMP_ERR_NOERROR)
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
                                     docsDevSwCurrentVers_var,
                                     docsDevSwCurrentVers_varLen);
        else
            netsnmp_set_request_error(reqinfo, requests, retVal);
        break;


    default:
        /*
         * we should never get here, so this is a really bad error 
         */
        return SNMP_ERR_GENERR;
    }

    return retVal;
}

int
do_docsDevSwServer(netsnmp_mib_handler *handler,
                   netsnmp_handler_registration *reginfo,
                   netsnmp_agent_request_info *reqinfo,
                   netsnmp_request_info *requests)
{
    /*
     * We are never called for a GETNEXT if it's registered as a
     * "instance", as it's "magically" handled for us.  
     */

    /*
     * a instance handler also only hands us one request at a time, so
     * we don't need to loop over a list of requests; we'll only get one. 
     */

    int             retVal = SNMP_ERR_NOERROR;

    u_char          docsDevSwServer_var[STRINGMAX];
    size_t          docsDevSwServer_varLen;

    switch (reqinfo->mode) {

    case MODE_GET:
        retVal =
            v2Get(requests->requestvb->name,
                  requests->requestvb->name_length, docsDevSwServer_var,
                  &docsDevSwServer_varLen, reqinfo);
        if (retVal == SNMP_ERR_NOERROR)
            snmp_set_var_typed_value(requests->requestvb, ASN_IPADDRESS,
                                     docsDevSwServer_var,
                                     docsDevSwServer_varLen);
        else
            netsnmp_set_request_error(reqinfo, requests, retVal);
        break;

        /*
         * SET REQUEST
         *
         */
    case MODE_SET_RESERVE1:
        if (requests->requestvb->type != ASN_IPADDRESS) {
            netsnmp_set_request_error(reqinfo, requests,
                                      SNMP_ERR_WRONGTYPE);
            retVal = SNMP_ERR_WRONGTYPE;
        } else {
            retVal = v2Test(requests->requestvb->name,
                            requests->requestvb->name_length,
                            requests->requestvb->val.string,
                            requests->requestvb->val_len,
                            requests->requestvb->type, reqinfo);
            if (retVal != SNMP_ERR_NOERROR)
                netsnmp_set_request_error(reqinfo, requests, retVal);
        }
        break;

    case MODE_SET_RESERVE2:
        break;

    case MODE_SET_FREE:
        break;

    case MODE_SET_ACTION:
        break;

    case MODE_SET_COMMIT:
        /*
         * really do the set! 
         */
        retVal = v2Set(requests->requestvb->name,
                       requests->requestvb->name_length,
                       requests->requestvb->val.string,
                       requests->requestvb->val_len,
                       requests->requestvb->type, reqinfo);
        if (retVal != SNMP_ERR_NOERROR)
            netsnmp_set_request_error(reqinfo, requests, retVal);
        break;

    case MODE_SET_UNDO:
        /*
         * Can't undo V2 sets! 
         */
        break;

    default:
        /*
         * we should never get here, so this is a really bad error 
         */
        return SNMP_ERR_GENERR;
    }

    return retVal;
}
