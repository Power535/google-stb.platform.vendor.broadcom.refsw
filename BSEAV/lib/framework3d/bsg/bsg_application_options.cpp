/******************************************************************************
 *   (c)2011-2012 Broadcom Corporation
 *
 * This program is the proprietary software of Broadcom Corporation and/or its
 * licensors, and may only be used, duplicated, modified or distributed
 * pursuant to the terms and conditions of a separate, written license
 * agreement executed between you and Broadcom (an "Authorized License").
 * Except as set forth in an Authorized License, Broadcom grants no license
 * (express or implied), right to use, or waiver of any kind with respect to
 * the Software, and Broadcom expressly reserves all rights in and to the
 * Software and all intellectual property rights therein.  IF YOU HAVE NO
 * AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
 * AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
 * SOFTWARE.  
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use all
 * reasonable efforts to protect the confidentiality thereof, and to use this
 * information only in connection with your use of Broadcom integrated circuit
 * products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
 * "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
 * IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
 * A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
 * ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE
 * ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR
 * ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
 * INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
 * RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
 * EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
 * FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
 *****************************************************************************/

#include "bsg_application_options.h"
#include "bsg_parse_utils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

namespace bsg
{

void ApplicationOptions::AddDefaultPath(const std::string &path)
{
   if (ParseUtils::IsDirectory(path))
      m_defaultResourcePaths.push_back(path + "/");
}

ApplicationOptions::ApplicationOptions() :
   m_useMultisample(false),
   m_stretchToFit(false),
   m_usePreservingSwap(false),
   m_renderToPixmap(false),
   m_showFps(false),
   m_showDevHUD(false),
   m_showFpsHUD(false),
   m_log(true),
   m_showRuntime(false),
   m_perfMonitoring(false),
   m_stereo(false),
   m_quad(1, 1),
#ifdef BSG_USE_ES3
   m_apiVersion(eGLES_3_0),
#else
   m_apiVersion(eGLES_2_0),
#endif
   m_noAutoContext(false),
   m_forceHDMI(false),
   m_displayRefreshRate(60),
   m_displayInterlace(false),
   m_showRenderer(false),
   m_authenticatedClient(true),
   m_depthBits(24),
   m_stencilBits(8),
   m_alphaBits(0),
   m_lastFrame(0),
   m_firstFrame(0),
   m_x(0),
   m_y(0),
   m_vpW(512),
   m_vpH(512),
   m_bpp(24),
   m_swapInterval(1),
   m_monitorInterval(1),
   m_monitorHw(0),
   m_monitorL3c(0),
   m_bandwidth(0),
   m_memcFrequency(288),
   m_clientID(-1),
   m_rateMultiplier(1.0f),
   m_autoSwapbuffer(true),
   m_headless(false),
   m_bgColour(-1.0f, -1.0f, -1.0f, -1.0f),
   m_finalAlpha(-1.0f),
   m_zOrder(~0u),
   m_argc(0),
   m_argv(0)
{
   AddDefaultPath(".");
   AddDefaultPath("resources");
   AddDefaultPath("common/resources");
   AddDefaultPath("../resources");
   AddDefaultPath("../common/resources");
   AddDefaultPath("../../resources");
   AddDefaultPath("../../common/resources");
   AddDefaultPath("../../../resources");
   AddDefaultPath("../../../common/resources");
   AddDefaultPath("../../../../resources");
   AddDefaultPath("../../../../common/resources");
}

void ApplicationOptions::AddResourcePath(const std::string &path)
{
   if (path != "")
   {
      std::string sep;

      if (path[path.size() - 1] != '/')
         sep = "/";

      m_resourcePaths.push_back(path + sep);
      m_resourcePaths.push_back(path + sep + "../../common/resources/");
   }
}

const std::string ApplicationOptions::FindResource(const std::string &file) const
{
   std::string fname = file;

   if (fname.c_str()[0] == '~')
   {
      char *home = getenv("HOME");
      if (home)
         fname = std::string(home) + fname.substr(1, std::string::npos);
   }

   std::string folder = FindResourceFolder(fname);

   return folder + fname;
}

const std::string ApplicationOptions::FindResourceFolder(const std::string &file) const
{
   // Look in user specified paths first
   uint32_t num = m_resourcePaths.size();
   for (uint32_t i = 0; i < num; i++)
   {
      std::string f = m_resourcePaths[i] + file;
      if (ParseUtils::IsFile(f))
         return m_resourcePaths[i];
   }

   // Now look in default paths
   num = m_defaultResourcePaths.size();
   for (uint32_t i = 0; i < num; i++)
   {
      std::string f = m_defaultResourcePaths[i] + file;

      if (ParseUtils::IsFile(f))
         return m_defaultResourcePaths[i];
   }

   return std::string("");
}

std::vector<char *>  ApplicationOptions::GetPlatformArgs() const
{
   std::vector<char *>  args(m_platformArgs.size());

   for (uint32_t i = 0; i < m_platformArgs.size(); ++i)
      args[i] = const_cast<char *>(m_platformArgs[i].c_str());

   return args;
}

bool ApplicationOptions::CheckSanity() const
{
   bool hasAlpha  = m_bgColour.W() != -1.0 && m_bgColour.W() != 1.0;
   bool needAlpha = m_finalAlpha != -1.0f || hasAlpha;

   if (needAlpha && m_bpp != 32)
   {
      fprintf(stderr, "Display surface has no alpha, but alpha values have been requested\n"
                      "Try bpp=32 on command line\n");

      return false;
   }

   return true;
}

bool ApplicationOptions::ParseCommandLine(int argc, char **argv, ArgumentParser *extraParser/* = nullptr */)
{
   // Use for the initialisation of Trellis platform
   m_argc = argc;
   m_argv = argv;

   bool ok = true;

   if (argc < 1)
   {
      PrintUsage("???", "", nullptr);
      ok = false;
   }

   // Add app specific resource paths
   std::string exePath = ParseUtils::GetPathname(std::string(argv[0]));
   std::string baseName = ParseUtils::GetBasename(std::string(argv[0]));

   AddDefaultPath(exePath + std::string("/resources_") + ParseUtils::GetBasename(std::string(argv[0])));
   AddDefaultPath(exePath + std::string("/resources"));
   AddDefaultPath(exePath + std::string("/common/resources"));
   AddDefaultPath(std::string("resources_") + ParseUtils::GetBasename(std::string(argv[0])));
   AddDefaultPath(exePath);
   AddDefaultPath(exePath + "/resources");
   AddDefaultPath(exePath + "/resources_common");
   AddDefaultPath(exePath + "/common/resources");
   AddDefaultPath(exePath + "/../resources");
   AddDefaultPath(exePath + "/../common/resources");
   AddDefaultPath(exePath + "/../../resources");
   AddDefaultPath(exePath + "/../../common/resources");
   AddDefaultPath(exePath + "/../../../resources");
   AddDefaultPath(exePath + "/../../../common/resources");
   AddDefaultPath(exePath + "/../../../../resources");
   AddDefaultPath(exePath + "/../../../../common/resources");

   for (int arg = 1; ok && arg < argc; ++arg)
   {
      ok = ProcessArg(argv[arg]);
      if (!ok && extraParser != nullptr)
         ok = extraParser->ParseArgument(std::string(argv[arg]));

  
      if (!ok)
         PrintUsage(argv[0], argv[arg], extraParser);
   }

   if (ok)
      ok = CheckSanity();

   return ok;
}

// Calculates any derived option values i.e. values that are based on the argument values
ApplicationOptions ApplicationOptions::CalculateDerived() const
{
   ApplicationOptions  result(*this);

   // Sort out the quad mode and screen size options
   // - if quad mode is selected adjust the window size accordingly
   // - if quad mode is not selected and the screen size is too big then enable quad mode

   if (m_quad.X() != 1 || m_quad.Y() != 1)
   {
      result.m_vpW = m_vpW / m_quad.X();
      result.m_vpH = m_vpH / m_quad.Y();
   }
   else
   {
#ifndef BSG_VC5
      uint32_t vpW = m_vpW;
      uint32_t vpH = m_vpH;
      uint32_t qx  = m_quad.X();
      uint32_t qy  = m_quad.Y();

      while (vpW > 2047)
      {
         qx  = qx + 1;
         vpW = m_vpW / qx;
      }

      while (vpH > 2047)
      {
         qy  = qy + 1;
         vpH = m_vpH / qy;
      }

      if (qy > qx)
      {
         qx  = qy;
         vpW = m_vpW / qx;
      }

      result.m_vpW      = vpW;
      result.m_vpH      = vpH;
      result.m_quad.X() = qx;
      result.m_quad.Y() = qy;
#endif
   }

   return result;
}

bool ApplicationOptions::UIntMatch(uint32_t *x, const char *arg, const char *fmt)
{
   if (x == 0)
      return false;

   if (sscanf(arg, fmt, x) == 1)
      return true;

   return false;
}

bool ApplicationOptions::IntMatch(int32_t *x, const char *arg, const char *fmt)
{
   if (x == 0)
      return false;

   if (sscanf(arg, fmt, x) == 1)
      return true;

   return false;
}

bool ApplicationOptions::FloatMatch(float *x, const char *arg, const char *fmt)
{
   if (x == 0)
      return false;

   if (sscanf(arg, fmt, x) == 1)
      return true;

   return false;
}

bool ApplicationOptions::StringMatch(std::vector<char> *str, const char *arg, const char *fmt)
{
   if (str == 0)
      return false;

   str->resize(strlen(arg));

   if (sscanf(arg, fmt, &(*str)[0]) == 1)
      return true;

   return false;
}

bool ApplicationOptions::CoordMatch(int32_t *x, int32_t *y, const char *arg, const char *fmt)
{
   if (x == 0 || y == 0)
      return false;

   if (sscanf(arg, fmt, x, y) == 2)
      return true;

   return false;
}

bool ApplicationOptions::FlagMatch(bool *flag, const char *arg, const char *name)
{
   if (flag == 0)
      return false;

   std::string on  = std::string("+") + name;
   std::string off = std::string("-") + name;

   if (ApplicationOptions::ArgMatch(arg, on.c_str()))
   {
      *flag = true;
      return true;
   }
   else if (ApplicationOptions::ArgMatch(arg, off.c_str()))
   {
      *flag = false;
      return true;
   }

   return false;
}

bool ApplicationOptions::ProcessArg(char *arg)
{
   if (CoordMatch((int32_t *)&m_vpW, (int32_t *)&m_vpH, arg, "d=%dx%d"))
   {
      if (m_vpW > 0 && m_vpH > 0)
         return true;

      return false;
   }

   if (CoordMatch(&m_x, &m_y, arg, "o=%dx%d"))
      return true;

   if (CoordMatch(&m_quad.X(), &m_quad.Y(), arg, "quad=%dx%d"))
   {
      if (m_quad.X() >= m_quad.Y() && !m_stereo)
         return true;
   }

   if (FlagMatch(&m_useMultisample, arg, "m"))
      return true;

   if (FlagMatch(&m_usePreservingSwap, arg, "p"))
      return true;

   if (FlagMatch(&m_stretchToFit, arg, "s"))
      return true;

   if (FlagMatch(&m_headless, arg, "headless"))
      return true;

   if (ArgMatch(arg, "-depth"))
   {
      m_depthBits = 0;
      return true;
   }

   if (ArgMatch(arg, "-stencil"))
   {
      m_stencilBits = 0;
      return true;
   }
   
   if (FlagMatch(&m_authenticatedClient, arg, "auth"))
      return true;

   if (UIntMatch(&m_bpp, arg, "bpp=%d"))
   {
      if (m_bpp == 32 || m_bpp == 24 || m_bpp == 16)
         return true;

      return false;
   }

   if (UIntMatch(&m_alphaBits, arg, "alpha_bits=%d"))
      return true;

   if (UIntMatch(&m_depthBits, arg, "depth_bits=%d"))
      return true;

   if (UIntMatch(&m_stencilBits, arg, "stencil_bits=%d"))
      return true;
   
   if (UIntMatch(&m_lastFrame, arg, "last=%d"))
   {
      if (m_lastFrame > 0)
         return true;

      return false;
   }
   
   if (UIntMatch(&m_firstFrame, arg, "first=%d"))
   {
      if (m_firstFrame > 0)
         return true;

      return false;
   }
   
   if (UIntMatch(&m_swapInterval, arg, "swap=%d"))
      return true;

   if (IntMatch(&m_clientID, arg, "client=%d"))
      return true;

   if (FlagMatch(&m_showFps, arg, "fps"))
      return true;

   if (FlagMatch(&m_showDevHUD, arg, "hud"))
      return true;

   if (FlagMatch(&m_showFpsHUD, arg, "fps_hud"))
      return true;

   if (FlagMatch(&m_showRuntime, arg, "runtime"))
      return true;

   if (FlagMatch(&m_stereo, arg, "stereo"))
   {
      if (m_quad.X() == 1 && m_quad.Y() == 1)
         return true;
   }

   if (FlagMatch(&m_forceHDMI, arg, "force_hdmi"))
      return true;

   if (FlagMatch(&m_showRenderer, arg, "show_renderer"))
      return true;

   if (FloatMatch(&m_rateMultiplier, arg, "rate=%f"))
      return true;

   uint32_t d = 0;

   if (UIntMatch(&d, arg, "dump_stats=%d"))
   {
      switch (d)
      {
      case 0:
         m_perfMonitoring = true;
         m_monitorHw = 0;
         m_monitorL3c = 0;
         return true;

      case 1:
         m_perfMonitoring = true;
         m_monitorHw = 1;
         m_monitorL3c = 1;
         return true;
      }

      return false;
   }

   if (UIntMatch(&m_monitorInterval, arg, "stats_interval=%d"))
      return true;

   if (UIntMatch(&m_bandwidth, arg, "bandwidth=%d"))
      return true;

   if (UIntMatch(&m_memcFrequency, arg, "memc_frequency=%d"))
      return true;

   std::vector<char>   str;

   if (StringMatch(&str, arg, "res=%s"))
   {
      AddResourcePath(&str[0]);
      return true;
   }

   if (StringMatch(&str, arg, "parg=%s"))
   {
      m_platformArgs.push_back(&str[0]);
      return true;
   }

   uint32_t version;

   if (UIntMatch((uint32_t *)&version, arg, "es=%d"))
   {
      if (version == 1)
         m_apiVersion = eGLES_1_1;
      else if (version == 2)
         m_apiVersion = eGLES_2_0;
      else if (version == 3)
         m_apiVersion  =eGLES_3_0;
      else
         return false;

      return true;
   }

   uint32_t rate;

   if (UIntMatch(&rate, arg, "disp_refresh=%d"))
   {
      m_displayRefreshRate = (uint8_t)(rate & 0xFF);
      return true;
   }

   if (FlagMatch(&m_displayInterlace, arg, "disp_interlace"))
      return true;

   uint32_t colour;

   if (UIntMatch(&colour, arg, "bg=%x"))
   {
      if (colour <= 0xffffffff)
      {
         m_bgColour.X() = ((colour >> 24) & 0xff) / 255.0f;
         m_bgColour.Y() = ((colour >> 16) & 0xff) / 255.0f;
         m_bgColour.Z() = ((colour >>  8) & 0xff) / 255.0f;
         m_bgColour.W() = ((colour >>  0) & 0xff) / 255.0f;

         return true;
      }

      return false;
   }

   if (UIntMatch(&colour, arg, "alpha=%x"))
   {
      if (colour <= 0xff)
      {
          m_finalAlpha = colour / 255.0f;

          return true;
      }

      return false;
   }

   // This doesn't work, so suppress for now
   //if (UIntMatch(&m_zOrder, arg, "z=%d"))
   //{
   //   return true;
   //}

   return false;
}

bool ApplicationOptions::ArgMatch(const char *arg, const char *str)
{
   if (str[strlen(str) - 1] == '=')
      return strncmp(str, arg, strlen(str)) == 0;
   else
      return strlen(arg) == strlen(str) && strncmp(str, arg, strlen(str)) == 0;
}

void ApplicationOptions::PrintUsage(const char *progName, const char *badArg, ArgumentParser *extraParser)
{
   if (badArg != nullptr)
      fprintf(stderr, "Error in argument (%s)\n\n", badArg);

   fprintf(stderr, 
      "Usage: %s options\n"
      "+m                 use multi-sampling\n"
      "+p                 preserve during swap\n"
      "+s                 stretch to fit display panel\n"
      "d=WWWxHHH          set window size (WWW, HHH > 0)\n"
      "o=XXXxYYY          set window origin\n"
      "swap=N             set minimum swap interval to N (>=0) frames\n"
      "rate=N             set animation rate multiplier to N (float, default 1.0)\n"
      "last=N             set the last frame to N (> 0)\n"
      "first=N            set the first displayed frame to N (> 0)\n"
      "bpp=[16|24|32]     set bits-per-pixel\n"
      "alpha_bits=N       set alpha bits (e.g. bpp=16 alpha_bits=4 would give a 4444 config)\n"
      "depth_bits=N       set depth bits\n"
      "stencil_bits=N     set stencil bits\n"
      "client=N           set multiprocess client id\n"
      "-auth              don't use authenticated client connection\n"
      "-depth             don't request a depth buffer\n"
      "-stencil           don't request a stencil buffer\n"
      "+fps               report frames per second to the console\n"
      "+hud               show developer HUD\n"
      "+fps_hud           show developer FPS HUD\n"
      "+stereo            render in stereo 3D (quad not supported)\n"
      // "z=N                sets the z-order for composited platforms (0 is bottom)\n"
      "alpha=X            sets the final alpha of the graphics plane to X (hex)\n"
      "bg=RRGGBBAA        sets the clear colour to RRGGBBAA e.g. 770077ff (opaque magenta)\n"
      "quad=XxY           render XxY panels for oversized displays (X >= Y) (stereo not supported)\n"
      "+force_hdmi        force hdmi output resolution, no hotplug callback\n"
      "+show_renderer     print the GL renderer information string\n"
      "dump_stats=[0|1]   collect and dump one bank of monitoring statistics\n"
      "stats_interval=N   print monitor data every N seconds (default 1)\n"
      "bandwidth=N        set the bandwidth in MB/s\n"
      "memc_frequency=N   set the MEMC frequency in MHz (default 288)\n"
      "res=<path>         add path for resource lookups\n"
      "disp_refresh=N     set refresh rate in hertz (will match next highest rate)\n"
      "+disp_interlace    use interlaced display if possible\n"
      "es=N               set the ES version to N = {1, 2, 3}\n"
      "+headless          headless display mode (only on Nexus platforms)\n"
      "parg=<arg>         specify a platform argument\n", progName);

   if (extraParser)
      fprintf(stderr, "%s\n", extraParser->UsageString().c_str());
}

}
