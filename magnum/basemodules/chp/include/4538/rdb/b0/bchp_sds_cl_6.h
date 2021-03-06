/***************************************************************************
 *     Copyright (c) 1999-2013, Broadcom Corporation
 *     All Rights Reserved
 *     Confidential Property of Broadcom Corporation
 *
 *
 * THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED SOFTWARE LICENSE
 * AGREEMENT  BETWEEN THE USER AND BROADCOM.  YOU HAVE NO RIGHT TO USE OR
 * EXPLOIT THIS MATERIAL EXCEPT SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
 *
 * $brcm_Workfile: $
 * $brcm_Revision: $
 * $brcm_Date: $
 *
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 *
 * Date:           Generated on         Thu Mar 14 11:46:10 2013
 *                 MD5 Checksum         d41d8cd98f00b204e9800998ecf8427e
 *
 * Compiled with:  RDB Utility          combo_header.pl
 *                 RDB Parser           3.0
 *                 unknown              unknown
 *                 Perl Interpreter     5.008008
 *                 Operating System     linux
 *
 * Revision History:
 *
 * $brcm_Log: $
 * 
 ***************************************************************************/

#ifndef BCHP_SDS_CL_6_H__
#define BCHP_SDS_CL_6_H__

/***************************************************************************
 *SDS_CL_6 - 6 SDS Carrier Loop Register Set
 ***************************************************************************/
#define BCHP_SDS_CL_6_CLCTL1                     0x000e4180 /* Carrier Loop Control 1 (Formerly, CLQCFD,CLPDCTL,CLCTL) */
#define BCHP_SDS_CL_6_CLCTL2                     0x000e4184 /* Carrier Loop Control 2 (Formerly, CLOON,CLSTS,CLMISC2,CLMISC) */
#define BCHP_SDS_CL_6_FLLC                       0x000e4188 /* Set 1 front carrier loop linear coefficient and scaling values */
#define BCHP_SDS_CL_6_FLLC1                      0x000e418c /* Set 2 front carrier loop linear coefficient and scaling values */
#define BCHP_SDS_CL_6_FLIC                       0x000e4190 /* Set 1 front carrier loop integrator coefficient and scaling values */
#define BCHP_SDS_CL_6_FLIC1                      0x000e4194 /* Set 2 front carrier loop integrator coefficient and scaling values */
#define BCHP_SDS_CL_6_FLSW                       0x000e4198 /* Front carrier loop sweep rate */
#define BCHP_SDS_CL_6_FLI                        0x000e419c /* Front carrier loop integrator value */
#define BCHP_SDS_CL_6_FLIF                       0x000e41a0 /* Front carrier loop IF frequency control word */
#define BCHP_SDS_CL_6_FLPA                       0x000e41a4 /* Front carrier loop phase accumulator value */
#define BCHP_SDS_CL_6_FLTD                       0x000e41a8 /* Front carrier loop frequency detector threshold */
#define BCHP_SDS_CL_6_PEEST                      0x000e41ac /* Back carrier loop phase error estimator output */
#define BCHP_SDS_CL_6_PLTD                       0x000e41b0 /* Back carrier loop frequency detector threshold */
#define BCHP_SDS_CL_6_PLC                        0x000e41b4 /* 1st set of back carrier loop linear and integrator coefficients and scaling values */
#define BCHP_SDS_CL_6_PLC1                       0x000e41b8 /* 2nd set of back carrier loop linear and integrator coefficients and scaling values */
#define BCHP_SDS_CL_6_PLSW                       0x000e41bc /* Back carrier loop sweep rate */
#define BCHP_SDS_CL_6_PLI                        0x000e41c0 /* Back carrier loop integrator value */
#define BCHP_SDS_CL_6_PLPA                       0x000e41c4 /* Back carrier loop phase accumulator value */
#define BCHP_SDS_CL_6_CRBFD                      0x000e41c8 /* Carrier loop QAM/8PSK phase detector threshold */
#define BCHP_SDS_CL_6_CLHT                       0x000e41cc /* Carrier loop lock high threshold value */
#define BCHP_SDS_CL_6_CLLT                       0x000e41d0 /* Carrier loop lock low threshold value */
#define BCHP_SDS_CL_6_CLLA                       0x000e41d4 /* Carrier loop lock accumulator value */
#define BCHP_SDS_CL_6_CLCT                       0x000e41d8 /* Carrier loop lock control */
#define BCHP_SDS_CL_6_CLFFCTL                    0x000e41dc /* Fine Frequency Mixer Control */
#define BCHP_SDS_CL_6_FFLPA                      0x000e41e0 /* Fine frequency adjustment phase accumulator */
#define BCHP_SDS_CL_6_CLFBCTL                    0x000e41e4 /* Forward Backward Loop Control (Formerly, CLFBCTL2,CLFBCTL) */
#define BCHP_SDS_CL_6_FBLC                       0x000e41e8 /* Forward Backward Loop Filter Coefficient */
#define BCHP_SDS_CL_6_FBLI                       0x000e41ec /* Forward Backward Loop Filter Integrator */
#define BCHP_SDS_CL_6_FBPA                       0x000e41f0 /* Forward Backward Loop Phase Accumulator */
#define BCHP_SDS_CL_6_CLDAFECTL                  0x000e41f4 /* DAFE Loop Control */
#define BCHP_SDS_CL_6_DAFELI                     0x000e41f8 /* DAFE Loop Filter Integrator */
#define BCHP_SDS_CL_6_DAFEINT                    0x000e41fc /* DAFE Loop Filter Integrator */

#endif /* #ifndef BCHP_SDS_CL_6_H__ */

/* End of File */
