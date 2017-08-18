 /******************************************************************************
  *  Copyright (C) 2017 Broadcom.  The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
  *
  *  This program is the proprietary software of Broadcom and/or its licensors,
  *  and may only be used, duplicated, modified or distributed pursuant to the terms and
  *  conditions of a separate, written license agreement executed between you and Broadcom
  *  (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
  *  no license (express or implied), right to use, or waiver of any kind with respect to the
  *  Software, and Broadcom expressly reserves all rights in and to the Software and all
  *  intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
  *  HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
  *  NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
  *
  *  Except as expressly set forth in the Authorized License,
  *
  *  1.     This program, including its structure, sequence and organization, constitutes the valuable trade
  *  secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
  *  and to use this information only in connection with your use of Broadcom integrated circuit products.
  *
  *  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
  *  AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
  *  WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
  *  THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES
  *  OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,
  *  LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION
  *  OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF
  *  USE OR PERFORMANCE OF THE SOFTWARE.
  *
  *  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
  *  LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR
  *  EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR
  *  USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF
  *  THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT
  *  ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
  *  LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF
  *  ANY LIMITED REMEDY.
  ******************************************************************************/

/* CMP0 V0 2084_to_hlg_lmr_mosaic */

/* sel_nl2l = PQ, it is in ROM */
#if 0 /* duplicated */
static const uint32_t s_aulRamLut_CMP0_L2NL_Hlg[] =
{
    0x00000000, /*    0:     0.000000000000000000000000000 */
    0x00000005, /*    1:     0.000305175781250000000000000 */
    0x00000007, /*    2:     0.000427246093750000000000000 */
    0x00000008, /*    3:     0.000488281250000000000000000 */
    0x0000000a, /*    4:     0.000610351562500000000000000 */
    0x0000000b, /*    5:     0.000671386718750000000000000 */
    0x0000000c, /*    6:     0.000732421875000000000000000 */
    0x0000000d, /*    7:     0.000793457031250000000000000 */
    0x0000000e, /*    8:     0.000854492187500000000000000 */
    0x0000000f, /*    9:     0.000915527343750000000000000 */
    0x0000000f, /*   10:     0.000915527343750000000000000 */
    0x00000010, /*   11:     0.000976562500000000000000000 */
    0x00000011, /*   12:     0.001037597656250000000000000 */
    0x00000012, /*   13:     0.001098632812500000000000000 */
    0x00000012, /*   14:     0.001098632812500000000000000 */
    0x00000013, /*   15:     0.001159667968750000000000000 */
    0x00000014, /*   16:     0.001220703125000000000000000 */
    0x00000014, /*   17:     0.001220703125000000000000000 */
    0x00000015, /*   18:     0.001281738281250000000000000 */
    0x00000015, /*   19:     0.001281738281250000000000000 */
    0x00000016, /*   20:     0.001342773437500000000000000 */
    0x00000016, /*   21:     0.001342773437500000000000000 */
    0x00000017, /*   22:     0.001403808593750000000000000 */
    0x00000017, /*   23:     0.001403808593750000000000000 */
    0x00000018, /*   24:     0.001464843750000000000000000 */
    0x00000018, /*   25:     0.001464843750000000000000000 */
    0x00000019, /*   26:     0.001525878906250000000000000 */
    0x00000019, /*   27:     0.001525878906250000000000000 */
    0x0000001a, /*   28:     0.001586914062500000000000000 */
    0x0000001a, /*   29:     0.001586914062500000000000000 */
    0x0000001b, /*   30:     0.001647949218750000000000000 */
    0x0000001b, /*   31:     0.001647949218750000000000000 */
    0x0000001c, /*   32:     0.001708984375000000000000000 */
    0x00000027, /*   33:     0.002380371093750000000000000 */
    0x00000030, /*   34:     0.002929687500000000000000000 */
    0x00000037, /*   35:     0.003356933593750000000000000 */
    0x0000003e, /*   36:     0.003784179687500000000000000 */
    0x00000044, /*   37:     0.004150390625000000000000000 */
    0x00000049, /*   38:     0.004455566406250000000000000 */
    0x0000004e, /*   39:     0.004760742187500000000000000 */
    0x00000053, /*   40:     0.005065917968750000000000000 */
    0x00000058, /*   41:     0.005371093750000000000000000 */
    0x0000005c, /*   42:     0.005615234375000000000000000 */
    0x00000060, /*   43:     0.005859375000000000000000000 */
    0x00000064, /*   44:     0.006103515625000000000000000 */
    0x00000068, /*   45:     0.006347656250000000000000000 */
    0x0000006b, /*   46:     0.006530761718750000000000000 */
    0x0000006f, /*   47:     0.006774902343750000000000000 */
    0x00000072, /*   48:     0.006958007812500000000000000 */
    0x00000076, /*   49:     0.007202148437500000000000000 */
    0x00000079, /*   50:     0.007385253906250000000000000 */
    0x0000007c, /*   51:     0.007568359375000000000000000 */
    0x0000007f, /*   52:     0.007751464843750000000000000 */
    0x00000082, /*   53:     0.007934570312500000000000000 */
    0x00000085, /*   54:     0.008117675781250000000000000 */
    0x00000088, /*   55:     0.008300781250000000000000000 */
    0x0000008b, /*   56:     0.008483886718750000000000000 */
    0x0000008d, /*   57:     0.008605957031250000000000000 */
    0x00000090, /*   58:     0.008789062500000000000000000 */
    0x00000093, /*   59:     0.008972167968750000000000000 */
    0x00000095, /*   60:     0.009094238281250000000000000 */
    0x00000098, /*   61:     0.009277343750000000000000000 */
    0x0000009a, /*   62:     0.009399414062500000000000000 */
    0x0000009d, /*   63:     0.009582519531250000000000000 */
    0x0000009f, /*   64:     0.009704589843750000000000000 */
    0x000000a2, /*   65:     0.009887695312500000000000000 */
    0x000000a4, /*   66:     0.010009765625000000000000000 */
    0x000000a6, /*   67:     0.010131835937500000000000000 */
    0x000000a9, /*   68:     0.010314941406250000000000000 */
    0x000000ab, /*   69:     0.010437011718750000000000000 */
    0x000000ad, /*   70:     0.010559082031250000000000000 */
    0x000000af, /*   71:     0.010681152343750000000000000 */
    0x000000b1, /*   72:     0.010803222656250000000000000 */
    0x000000b4, /*   73:     0.010986328125000000000000000 */
    0x000000b6, /*   74:     0.011108398437500000000000000 */
    0x000000b8, /*   75:     0.011230468750000000000000000 */
    0x000000ba, /*   76:     0.011352539062500000000000000 */
    0x000000bc, /*   77:     0.011474609375000000000000000 */
    0x000000be, /*   78:     0.011596679687500000000000000 */
    0x000000c0, /*   79:     0.011718750000000000000000000 */
    0x000000c2, /*   80:     0.011840820312500000000000000 */
    0x000000c4, /*   81:     0.011962890625000000000000000 */
    0x000000c6, /*   82:     0.012084960937500000000000000 */
    0x000000c8, /*   83:     0.012207031250000000000000000 */
    0x000000ca, /*   84:     0.012329101562500000000000000 */
    0x000000cc, /*   85:     0.012451171875000000000000000 */
    0x000000ce, /*   86:     0.012573242187500000000000000 */
    0x000000cf, /*   87:     0.012634277343750000000000000 */
    0x000000d1, /*   88:     0.012756347656250000000000000 */
    0x000000d3, /*   89:     0.012878417968750000000000000 */
    0x000000d5, /*   90:     0.013000488281250000000000000 */
    0x000000d7, /*   91:     0.013122558593750000000000000 */
    0x000000d8, /*   92:     0.013183593750000000000000000 */
    0x000000da, /*   93:     0.013305664062500000000000000 */
    0x000000dc, /*   94:     0.013427734375000000000000000 */
    0x000000de, /*   95:     0.013549804687500000000000000 */
    0x00000110, /*   96:     0.016601562500000000000000000 */
    0x0000013a, /*   97:     0.019165039062500000000000000 */
    0x0000015f, /*   98:     0.021423339843750000000000000 */
    0x00000180, /*   99:     0.023437500000000000000000000 */
    0x0000019f, /*  100:     0.025329589843750000000000000 */
    0x000001bb, /*  101:     0.027038574218750000000000000 */
    0x000001d6, /*  102:     0.028686523437500000000000000 */
    0x000001f0, /*  103:     0.030273437500000000000000000 */
    0x00000208, /*  104:     0.031738281250000000000000000 */
    0x0000021f, /*  105:     0.033142089843750000000000000 */
    0x00000235, /*  106:     0.034484863281250000000000000 */
    0x0000024b, /*  107:     0.035827636718750000000000000 */
    0x0000025f, /*  108:     0.037048339843750000000000000 */
    0x00000273, /*  109:     0.038269042968750000000000000 */
    0x00000286, /*  110:     0.039428710937500000000000000 */
    0x00000299, /*  111:     0.040588378906250000000000000 */
    0x000002ab, /*  112:     0.041687011718750000000000000 */
    0x000002bd, /*  113:     0.042785644531250000000000000 */
    0x000002ce, /*  114:     0.043823242187500000000000000 */
    0x000002df, /*  115:     0.044860839843750000000000000 */
    0x000002f0, /*  116:     0.045898437500000000000000000 */
    0x00000300, /*  117:     0.046875000000000000000000000 */
    0x00000310, /*  118:     0.047851562500000000000000000 */
    0x0000031f, /*  119:     0.048767089843750000000000000 */
    0x0000032f, /*  120:     0.049743652343750000000000000 */
    0x0000033e, /*  121:     0.050659179687500000000000000 */
    0x0000034c, /*  122:     0.051513671875000000000000000 */
    0x0000035b, /*  123:     0.052429199218750000000000000 */
    0x00000369, /*  124:     0.053283691406250000000000000 */
    0x00000377, /*  125:     0.054138183593750000000000000 */
    0x000003df, /*  126:     0.060485839843750000000000000 */
    0x0000043e, /*  127:     0.066284179687500000000000000 */
    0x00000495, /*  128:     0.071594238281250000000000000 */
    0x000004e6, /*  129:     0.076538085937500000000000000 */
    0x00000532, /*  130:     0.081176757812500000000000000 */
    0x0000057a, /*  131:     0.085571289062500000000000000 */
    0x000005bf, /*  132:     0.089782714843750000000000000 */
    0x00000600, /*  133:     0.093750000000000000000000000 */
    0x0000063f, /*  134:     0.097595214843750000000000000 */
    0x0000067b, /*  135:     0.101257324218750000000000000 */
    0x000006b5, /*  136:     0.104797363281250000000000000 */
    0x000006ee, /*  137:     0.108276367187500000000000000 */
    0x00000724, /*  138:     0.111572265625000000000000000 */
    0x00000759, /*  139:     0.114807128906250000000000000 */
    0x0000078d, /*  140:     0.117980957031250000000000000 */
    0x000007bf, /*  141:     0.121032714843750000000000000 */
    0x000007f0, /*  142:     0.124023437500000000000000000 */
    0x00000820, /*  143:     0.126953125000000000000000000 */
    0x0000084e, /*  144:     0.129760742187500000000000000 */
    0x0000087c, /*  145:     0.132568359375000000000000000 */
    0x000008a9, /*  146:     0.135314941406250000000000000 */
    0x000008d5, /*  147:     0.138000488281250000000000000 */
    0x00000900, /*  148:     0.140625000000000000000000000 */
    0x0000092a, /*  149:     0.143188476562500000000000000 */
    0x00000954, /*  150:     0.145751953125000000000000000 */
    0x0000097d, /*  151:     0.148254394531250000000000000 */
    0x000009a5, /*  152:     0.150695800781250000000000000 */
    0x000009cc, /*  153:     0.153076171875000000000000000 */
    0x000009f3, /*  154:     0.155456542968750000000000000 */
    0x00000a19, /*  155:     0.157775878906250000000000000 */
    0x00000a3f, /*  156:     0.160095214843750000000000000 */
    0x00000a64, /*  157:     0.162353515625000000000000000 */
    0x00000a89, /*  158:     0.164611816406250000000000000 */
    0x00000aad, /*  159:     0.166809082031250000000000000 */
    0x00000ad1, /*  160:     0.169006347656250000000000000 */
    0x00000af4, /*  161:     0.171142578125000000000000000 */
    0x00000b17, /*  162:     0.173278808593750000000000000 */
    0x00000b3a, /*  163:     0.175415039062500000000000000 */
    0x00000b5c, /*  164:     0.177490234375000000000000000 */
    0x00000b7d, /*  165:     0.179504394531250000000000000 */
    0x00000b9e, /*  166:     0.181518554687500000000000000 */
    0x00000bbf, /*  167:     0.183532714843750000000000000 */
    0x00000be0, /*  168:     0.185546875000000000000000000 */
    0x00000c00, /*  169:     0.187500000000000000000000000 */
    0x00000c20, /*  170:     0.189453125000000000000000000 */
    0x00000c3f, /*  171:     0.191345214843750000000000000 */
    0x00000c5f, /*  172:     0.193298339843750000000000000 */
    0x00000c7d, /*  173:     0.195129394531250000000000000 */
    0x00000c9c, /*  174:     0.197021484375000000000000000 */
    0x00000cba, /*  175:     0.198852539062500000000000000 */
    0x00000cd8, /*  176:     0.200683593750000000000000000 */
    0x00000cf6, /*  177:     0.202514648437500000000000000 */
    0x00000d14, /*  178:     0.204345703125000000000000000 */
    0x00000d31, /*  179:     0.206115722656250000000000000 */
    0x00000d4e, /*  180:     0.207885742187500000000000000 */
    0x00000d6b, /*  181:     0.209655761718750000000000000 */
    0x00000d87, /*  182:     0.211364746093750000000000000 */
    0x00000da3, /*  183:     0.213073730468750000000000000 */
    0x00000dbf, /*  184:     0.214782714843750000000000000 */
    0x00000ddb, /*  185:     0.216491699218750000000000000 */
    0x00000f7e, /*  186:     0.242065429687500000000000000 */
    0x000010f8, /*  187:     0.265136718750000000000000000 */
    0x00001255, /*  188:     0.286437988281250000000000000 */
    0x00001399, /*  189:     0.306213378906250000000000000 */
    0x000014c9, /*  190:     0.324768066406250000000000000 */
    0x000015e9, /*  191:     0.342346191406250000000000000 */
    0x000016fa, /*  192:     0.359008789062500000000000000 */
    0x00001800, /*  193:     0.375000000000000000000000000 */
    0x000018fb, /*  194:     0.390319824218750000000000000 */
    0x000019ec, /*  195:     0.405029296875000000000000000 */
    0x00001ad5, /*  196:     0.419250488281250000000000000 */
    0x00001bb6, /*  197:     0.432983398437500000000000000 */
    0x00001c91, /*  198:     0.446350097656250000000000000 */
    0x00001d65, /*  199:     0.459289550781250000000000000 */
    0x00001e33, /*  200:     0.471862792968750000000000000 */
    0x00001efc, /*  201:     0.484130859375000000000000000 */
    0x00001fc0, /*  202:     0.496093750000000000000000000 */
    0x0000207d, /*  203:     0.507629394531250000000000000 */
    0x00002130, /*  204:     0.518554687500000000000000000 */
    0x000021d8, /*  205:     0.528808593750000000000000000 */
    0x00002277, /*  206:     0.538513183593750000000000000 */
    0x0000230e, /*  207:     0.547729492187500000000000000 */
    0x0000239d, /*  208:     0.556457519531250000000000000 */
    0x00002426, /*  209:     0.564819335937500000000000000 */
    0x000024a9, /*  210:     0.572814941406250000000000000 */
    0x00002526, /*  211:     0.580444335937500000000000000 */
    0x0000259e, /*  212:     0.587768554687500000000000000 */
    0x00002611, /*  213:     0.594787597656250000000000000 */
    0x00002680, /*  214:     0.601562500000000000000000000 */
    0x000026eb, /*  215:     0.608093261718750000000000000 */
    0x00002752, /*  216:     0.614379882812500000000000000 */
    0x000027b5, /*  217:     0.620422363281250000000000000 */
    0x00002816, /*  218:     0.626342773437500000000000000 */
    0x00002873, /*  219:     0.632019042968750000000000000 */
    0x000028cd, /*  220:     0.637512207031250000000000000 */
    0x00002925, /*  221:     0.642883300781250000000000000 */
    0x0000297a, /*  222:     0.648071289062500000000000000 */
    0x000029cd, /*  223:     0.653137207031250000000000000 */
    0x00002a1d, /*  224:     0.658020019531250000000000000 */
    0x00002a6b, /*  225:     0.662780761718750000000000000 */
    0x00002ab8, /*  226:     0.667480468750000000000000000 */
    0x00002b02, /*  227:     0.671997070312500000000000000 */
    0x00002b4b, /*  228:     0.676452636718750000000000000 */
    0x00002b91, /*  229:     0.680725097656250000000000000 */
    0x00002bd6, /*  230:     0.684936523437500000000000000 */
    0x00002c1a, /*  231:     0.689086914062500000000000000 */
    0x00002c5c, /*  232:     0.693115234375000000000000000 */
    0x00002c9c, /*  233:     0.697021484375000000000000000 */
    0x00002cdb, /*  234:     0.700866699218750000000000000 */
    0x00002d19, /*  235:     0.704650878906250000000000000 */
    0x00002d56, /*  236:     0.708374023437500000000000000 */
    0x00002d91, /*  237:     0.711975097656250000000000000 */
    0x00002dcb, /*  238:     0.715515136718750000000000000 */
    0x00002e04, /*  239:     0.718994140625000000000000000 */
    0x00002e3c, /*  240:     0.722412109375000000000000000 */
    0x00002e73, /*  241:     0.725769042968750000000000000 */
    0x00002ea9, /*  242:     0.729064941406250000000000000 */
    0x00002edd, /*  243:     0.732238769531250000000000000 */
    0x00002f11, /*  244:     0.735412597656250000000000000 */
    0x00002f44, /*  245:     0.738525390625000000000000000 */
    0x000030bf, /*  246:     0.761657714843750000000000000 */
    0x0000320f, /*  247:     0.782165527343750000000000000 */
    0x0000333c, /*  248:     0.800537109375000000000000000 */
    0x0000344d, /*  249:     0.817199707031250000000000000 */
    0x00003547, /*  250:     0.832458496093750000000000000 */
    0x0000362d, /*  251:     0.846496582031250000000000000 */
    0x00003702, /*  252:     0.859497070312500000000000000 */
    0x000037c9, /*  253:     0.871643066406250000000000000 */
    0x00003883, /*  254:     0.882995605468750000000000000 */
    0x00003932, /*  255:     0.893676757812500000000000000 */
    0x000039d7, /*  256:     0.903747558593750000000000000 */
    0x00003a74, /*  257:     0.913330078125000000000000000 */
    0x00003b08, /*  258:     0.922363281250000000000000000 */
    0x00003b96, /*  259:     0.931030273437500000000000000 */
    0x00003c1c, /*  260:     0.939208984375000000000000000 */
    0x00003c9d, /*  261:     0.947082519531250000000000000 */
    0x00003d19, /*  262:     0.954650878906250000000000000 */
    0x00003d8f, /*  263:     0.961853027343750000000000000 */
    0x00003e01, /*  264:     0.968811035156250000000000000 */
    0x00003e6f, /*  265:     0.975524902343750000000000000 */
    0x00003ed8, /*  266:     0.981933593750000000000000000 */
    0x00003f3e, /*  267:     0.988159179687500000000000000 */
    0x00003fa1, /*  268:     0.994201660156250000000000000 */
    0x00003fff  /*  269:     0.999938964843750000000000000 */
};

static const BVDC_P_RamLut s_RamLutCtrl_CMP0_L2NL_Hlg =
{
    6, 		/* numSeg */
    0x4, 	/* U1.2 xscl = 1.00000 */
    0, 		/* outIntBits */
    14, 		/* outFracBits */
    {     32,    95,   125,   185,   245,   269,     0,     0}, /* segEnd */
    {      0,     1,     2,     4,     4,     8,     0,     0}, /* segOffset */
    {     25,    20,    15,    12,     8,     5,     0,     0}, /* segIntBits */
    &s_aulRamLut_CMP0_L2NL_Hlg[0]
};
#endif /* #if 0  duplicated */
static const uint32_t s_aulRamLut_CMP0_LMR_2084_to_hlg_lmr_mosaic[] =
{
    0x000fffff, /*    0:     0.999999046325683593750000000 */
    0x0006ddf3, /*    1:     0.429186820983886718750000000 */
    0x00061e28, /*    2:     0.382362365722656250000000000 */
    0x0005b7d1, /*    3:     0.357377052307128906250000000 */
    0x00057349, /*    4:     0.340645790100097656250000000 */
    0x00054059, /*    5:     0.328209877014160156250000000 */
    0x0005181d, /*    6:     0.318387031555175781250000000 */
    0x0004f709, /*    7:     0.310311317443847656250000000 */
    0x0004db0f, /*    8:     0.303481101989746093750000000 */
    0x0004c2e5, /*    9:     0.297581672668457031250000000 */
    0x0004adad, /*   10:     0.292401313781738281250000000 */
    0x00049ace, /*   11:     0.287794113159179687500000000 */
    0x000489d5, /*   12:     0.283650398254394531250000000 */
    0x00047a6f, /*   13:     0.279891014099121093750000000 */
    0x00046c5d, /*   14:     0.276455879211425781250000000 */
    0x00045f6a, /*   15:     0.273294448852539062500000000 */
    0x00045370, /*   16:     0.270370483398437500000000000 */
    0x00040b13, /*   17:     0.252703666687011718750000000 */
    0x0003da9e, /*   18:     0.240873336791992187500000000 */
    0x0003b699, /*   19:     0.232079505920410156250000000 */
    0x00039a25, /*   20:     0.225132942199707031250000000 */
    0x000382c2, /*   21:     0.219423294067382812500000000 */
    0x00036efa, /*   22:     0.214593887329101562500000000 */
    0x00030f14, /*   23:     0.191181182861328125000000000 */
    0x0002dbe8, /*   24:     0.178688049316406250000000000 */
    0x0002b9a5, /*   25:     0.170323371887207031250000000 */
    0x0002a02d, /*   26:     0.164105415344238281250000000 */
    0x00028c0e, /*   27:     0.159193038940429687500000000 */
    0x00027b84, /*   28:     0.155155181884765625000000000 */
    0x00026d88, /*   29:     0.151741027832031250000000000 */
    0x00026173, /*   30:     0.148791313171386718750000000 */
    0x000256d7, /*   31:     0.146201133728027343750000000 */
    0x00024d67, /*   32:     0.143897056579589843750000000 */
    0x000244ea, /*   33:     0.141824722290039062500000000 */
    0x00023d38, /*   34:     0.139945983886718750000000000 */
    0x0002362e, /*   35:     0.138227462768554687500000000 */
    0x00022fb5, /*   36:     0.136647224426269531250000000 */
    0x000229b8, /*   37:     0.135185241699218750000000000 */
    0x00022427, /*   38:     0.133826255798339843750000000 */
    0x00021ef5, /*   39:     0.132557868957519531250000000 */
    0x00021a16, /*   40:     0.131368637084960937500000000 */
    0x00021581, /*   41:     0.130249977111816406250000000 */
    0x0002112f, /*   42:     0.129195213317871093750000000 */
    0x00020d19, /*   43:     0.128197669982910156250000000 */
    0x00020939, /*   44:     0.127251625061035156250000000 */
    0x00020589, /*   45:     0.126351356506347656250000000 */
    0x00020207, /*   46:     0.125494956970214843750000000 */
    0x00020000, /*   47:     0.125000000000000000000000000 */
    0x00020000, /*   48:     0.125000000000000000000000000 */
    0x00020000, /*   49:     0.125000000000000000000000000 */
    0x00020000, /*   50:     0.125000000000000000000000000 */
    0x00020000, /*   51:     0.125000000000000000000000000 */
    0x00020000, /*   52:     0.125000000000000000000000000 */
    0x00020000, /*   53:     0.125000000000000000000000000 */
    0x00020000, /*   54:     0.125000000000000000000000000 */
    0x00020000, /*   55:     0.125000000000000000000000000 */
    0x00020000, /*   56:     0.125000000000000000000000000 */
    0x00020000, /*   57:     0.125000000000000000000000000 */
    0x00020000, /*   58:     0.125000000000000000000000000 */
    0x00020000, /*   59:     0.125000000000000000000000000 */
    0x00020000, /*   60:     0.125000000000000000000000000 */
    0x00020000  /*   61:     0.125000000000000000000000000 */
};

static const BVDC_P_RamLut s_RamLutCtrl_CMP0_LMR_2084_to_hlg_lmr_mosaic =
{
    4, 		/* numSeg */
    0x4, 	/* U1.2 xscl = 1.00000 */
    0, 		/* outIntBits */
    20, 		/* outFracBits */
    {     16,    22,    53,    61,     0,     0,     0,     0}, /* segEnd */
    {      0,     2,     1,     1,     0,     0,     0,     0}, /* segOffset */
    {     14,    11,     8,     3,     0,     0,     0,     0}, /* segIntBits */
    &s_aulRamLut_CMP0_LMR_2084_to_hlg_lmr_mosaic[0]
};

static const uint32_t s_aulLmrAdj_CMP0_2084_to_hlg_lmr_mosaic[] =
{
    0x800, /* S7.8 LMA.A =    8.000000000000000000000000000 */
    0x0, /* S7.8 LMA.B =    0.000000000000000000000000000 */
    0x0  /* S9.15 LMA.C =    0.000000000000000000000000000 */
};

static const BVDC_P_LRangeAdjTable s_LRangeAdj_CMP0_2084_to_hlg_lmr_mosaic = BVDC_P_MAKE_LR_ADJ
(   8, /* number of PWL points */
    /*     x         y         m     e              (U0.15)x,   (U0.15)y,   (S0.15)m,   (S4)e   */
    0.000000, 0.000000, 0.624969,    4,   /* PWL 0, 0x00000000, 0x00000000, 0x00004fff, 0x00000004 */
    0.100006, 1.000000, 0.000000,    0,   /* PWL 1, 0x00000ccd, 0x00008000, 0x00000000, 0x00000000 */
    1.000000, 1.000000, 0.000000,    0,   /* PWL 2, 0x00008000, 0x00008000, 0x00000000, 0x00000000 */
    1.000000, 1.000000, 0.000000,    0,   /* PWL 3, 0x00008000, 0x00008000, 0x00000000, 0x00000000 */
    1.000000, 1.000000, 0.000000,    0,   /* PWL 4, 0x00008000, 0x00008000, 0x00000000, 0x00000000 */
    1.000000, 1.000000, 0.500000,    1,   /* PWL 5, 0x00008000, 0x00008000, 0x00004000, 0x00000001 */
    0.000000, 0.000000, 0.000000,    0,   /* PWL 6, 0x00000000, 0x00000000, 0x00000000, 0x00000000 */
    0.000000, 0.000000, 0.000000,    0    /* PWL 7, 0x00000000, 0x00000000, 0x00000000, 0x00000000 */
);


/* end of file */
