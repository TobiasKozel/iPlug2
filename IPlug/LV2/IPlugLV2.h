/*
 ==============================================================================
 
 This file is part of the iPlug 2 library. Copyright (C) the iPlug 2 developers. 
 
 See LICENSE.txt for  more info.
 
 ==============================================================================
*/

#ifndef _IPLUGAPI_
#define _IPLUGAPI_

#include "IPlugAPIBase.h"
#include "IPlugProcessor.h"

#include "lv2/core/lv2.h"
#include <vector>

BEGIN_IPLUG_NAMESPACE


#ifndef PLUG_URI
#define PLUG_URI PLUG_URL_STR "/plugins/" PLUG_NAME
#endif

#ifndef PLUG_UI_URI
#define PLUG_UI_URI PLUG_URI "#ui"
#endif

#ifdef IPLUG_DSP

/** Used to pass various instance info to the API class */
struct InstanceInfo
{
  const LV2_Descriptor *descriptor;
  double rate;
  const char* bundle_path;
  const LV2_Feature* const* features;
};

/**  LV2 processor base class for an IPlug plug-in
*   @ingroup APIClasses */
class IPlugLV2DSP : public IPlugAPIBase
                  , public IPlugProcessor
{
public:
  IPlugLV2DSP(const InstanceInfo& info, const Config& config);
  ~IPlugLV2DSP();

  /*
  //IPlugAPIBase
  void BeginInformHostOfParamChange(int idx) override;
  void InformHostOfParamChange(int idx, double normalizedValue) override;
  void EndInformHostOfParamChange(int idx) override;
  void InformHostOfProgramChange() override;
  void HostSpecificInit() override;
  bool EditorResizeFromDelegate(int viewWidth, int viewHeight) override;

  //IPlugProcessor
  void SetLatency(int samples) override;
  bool SendSysEx(const ISysEx& msg) override;
  */
  bool SendMidiMsg(const IMidiMsg& msg) override;

  //LV2 methods
  void  connect_port(uint32_t port, void *data);
  void  activate();
  void  run(uint32_t n_samples);
  void  deactivate();

#ifdef LV2_CFG
  // support ttl generation
  int write_manifest();
  int write_also();
#endif

private:
  void **mPorts; // simpler then vector for AttachBuffers
};

#endif

#ifdef IPLUG_EDITOR

#include "lv2/ui/ui.h"
#include "xcbt.h"

/** Used to pass various instance info to the API class */
struct InstanceInfo
{
  const LV2UI_Descriptor*   descriptor;
  const char*               plugin_uri;
  const char*               bundle_path;
  LV2UI_Write_Function      write_function;
  LV2UI_Controller          controller;
  const LV2_Feature* const* features;
};

/**  LV2 editor base class for an IPlug plug-in
*   @ingroup APIClasses */
class IPlugLV2Editor : public IPlugAPIBase
{
public:
  IPlugLV2Editor(const InstanceInfo& info, const Config& config);
  ~IPlugLV2Editor();

  //IPlugAPIBase
  void InformHostOfParamChange(int idx, double normalizedValue) override;
  /*
  void BeginInformHostOfParamChange(int idx) override;
  void EndInformHostOfParamChange(int idx) override;
  void InformHostOfProgramChange() override;
  bool EditorResizeFromDelegate(int viewWidth, int viewHeight) override;
  */
  
  LV2UI_Widget CreateUI(); // post constructor to create window

  //LV2 UI methods
  void port_event(uint32_t port_index, uint32_t buffer_size, uint32_t format, const void*  buffer);
  int  ui_idle();

  //IEditorDelegate
  bool EditorResizeFromUI(int viewWidth, int viewHeight) override;
  
private:
  int mParameterPortOffset;

  LV2UI_Write_Function   mHostWrite;
  LV2UI_Controller       mHostController;
  bool                   mHostSupportIdle;
  LV2UI_Widget           mHostWidget;
  LV2UI_Resize          *mHostResize;
  
  xcbt_embed            *mEmbed;
};

#endif

END_IPLUG_NAMESPACE

#endif /* __IPLUGAPI_H */
